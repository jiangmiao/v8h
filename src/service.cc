#include <unistd.h>
#include "service.h"

V8H_NS_START

Service::Service(uint32_t maxEvents)
: maxEvents(maxEvents)
, fdsNumber(0)
{
	events = new epoll_event[maxEvents];
	fd     = epoll_create(maxEvents);
}

Service::~Service()
{
	delete events;
	::close(fd);
}

void  Service::mod(int fd, int events)
{
	epoll_event event = {0};
	event.events  = events;
	event.data.fd = fd;
	epoll_ctl(this->fd, EPOLL_CTL_MOD, fd, &event);
}

void  Service::add(int fd, int events)
{
	epoll_event event = {0};
	event.events  = events;
	event.data.fd = fd;
	epoll_ctl(this->fd, EPOLL_CTL_ADD, fd, &event);
	++fdsNumber;
}

void  Service::del(int fd)
{
	epoll_event event = {0};
	epoll_ctl(this->fd, EPOLL_CTL_DEL, fd, &event);
	--fdsNumber;
}

int Service::wait(int timeout) 
{
	return epoll_wait(fd, events, maxEvents, timeout);
}

int Service::getFdByEventId(int eventId)
{
	return events[eventId].data.fd;
}

v8::Handle<v8::Value> Service::inOrOut(const v8::Arguments &args, int events)
{
	auto self     = args.This();
	auto fd       = TO_INT(args[0]);
	auto callback = TO_OBJ(args[1]);

	auto epoll    = getInternal(self);
	auto fds      = GET_OBJ(self, V8H_SYM(fds));
	if(fds->Has(fd)) {
		epoll->mod(fd, events);
	} else {
		epoll->add(fd, events);
	}

	fds->Set(fd, callback);
	return self;
}


V8H_FUNCTION(Service::in)
{
	return inOrOut(args, EPOLLIN);
}


V8H_FUNCTION(Service::out)
{
	return inOrOut(args, EPOLLOUT);
}

V8H_FUNCTION(Service::del)
{
	auto self      = args.This();
	auto service   = getInternal(self);
	auto fd        = TO_INT(args[0]);
	auto fds       = GET_OBJ(self, V8H_SYM(fds));
	auto fdsError = GET_OBJ(self, V8H_SYM(fdsError));
	if (fds->Has(fd)) {
		service->del(fd);
		fds->Delete(fd);
	}
	fdsError->Delete(fd);
	return self;
}

V8H_FUNCTION(Service::error)
{
	auto self      = args.This();
	auto fd        = TO_INT(args[0]);
	auto callback  = TO_OBJ(args[1]);
	auto fdsError = GET_OBJ(self, V8H_SYM(fdsError));
	fdsError->Set(fd, callback);
	return self;
}


V8H_FUNCTION(Service::wait)
{
	auto self     = args.This();
	int  timeout = -1;
	if (args.Length() > 0)
		timeout = TO_INT(args[0]);
	auto service = getInternal(self);
	int n =  service->wait(timeout);
	return V8_INT(n);
}

V8H_FUNCTION(Service::close)
{
	auto self     = args.This();
	DEL_PTR<Service*>(self, FIELD_INTERNAL);
	return self;
}

V8H_FUNCTION(Service::getCallbackFromEventId)
{
	auto self     = args.This();
	auto eventId = TO_INT(args[0]);
	auto service  = getInternal(self);
	auto fds      = GET_OBJ(self, V8H_SYM(fds));
	auto fd       = service->getFdByEventId(eventId);
	auto callback = fds->Get(fd);
	return callback;
}



V8H_FUNCTION(Service::trigger)
{
	auto self     = args.This();
	auto fd       = TO_INT(args[0]);
	auto fds      = GET_OBJ(self, V8H_SYM(fds));
	auto callback = v8::Handle<v8::Function>::Cast(fds->Get(fd));
	auto result   = callback->Call(self, 0, NULL);
	return result;
}

V8H_FUNCTION(Service::triggerByEventId)
{
	auto self     = args.This();
	auto eventId = TO_INT(args[0]);
	auto service  = getInternal(self);
	auto fd      = service->getFdByEventId(eventId);
	auto fds      = GET_OBJ(self, V8H_SYM(fds));
	auto callback = v8::Handle<v8::Function>::Cast(fds->Get(fd));
	auto result   = callback->Call(self, 0, NULL);
	return result;
}

V8H_FUNCTION(Service::run)
{
	auto   self      = args.This();
	auto   service   = getInternal(self);
	auto   fds       = GET_OBJ(self, V8H_SYM(fds));
	auto   fdsError = GET_OBJ(self, V8H_SYM(fdsError));
	uint32_t times     = 0;
	uint32_t maxTimes = 0;
	if (maxTimes == 0) {
		maxTimes = 1e5;
	}
	while (service->fdsNumber) {
		int n = service->wait(-1);
		if ( n == -1 ) {
			perror("service - wait");
			break;
		}

		if ( n>0 ) {
			v8::HandleScope scope;
			for (int i=0; i<n; ++i) {
				int  fd       = service->getFdByEventId(i);
				int  events   = service->events[i].events;
				v8::Handle<v8::Function> callback;
				if (events & EPOLLERR || events & EPOLLHUP) {
					callback = v8::Handle<v8::Function>::Cast(fdsError->Get(fd));
					if (callback.IsEmpty()) {
						return THROW_EXCEPTION("uncaught error");
					}
				} else {
					callback = v8::Handle<v8::Function>::Cast(fds->Get(fd));
					if (callback.IsEmpty()) {
						return THROW_EXCEPTION("callback missing");
					}
				}

				auto result   = callback->Call(self, 0, NULL);
				if (result.IsEmpty()) {
					V8H_LEAVE(result);
				}
			}
		}

		times += n;
		if (times >= maxTimes) {
			times = 0;
			v8::V8::LowMemoryNotification();
		}

	}
	return self;
}

V8H_FUNCTION(Service::constructor)
{
	auto self       = args.This();

	uint32_t  maxEvents = 1024;
	if (args.Length() > 0)
		maxEvents = TO_UINT(args[0]);

	auto *service   = new Service(maxEvents);
	service->wrap(self);

	SET(self , V8H_SYM(fds)      , v8::Object::New());
	SET(self , V8H_SYM(fdsError) , v8::Object::New());
	return self;
}


V8H_FUNCTION(Service::getFdsNumber)
{
	auto self = args.This();
	auto epoll = getInternal(self);
	return V8_INT(epoll->fdsNumber);
}

v8::Handle<v8::Function> Service::create()
{
	V8H_CREATE_START_WITH_INTERNAL_FIELD();

	V8H_SET_SYM(fds);
	V8H_SET_SYM(event);
	V8H_SET_SYM(fd);
	V8H_SET_SYM(fdsError);

	V8H_IMPLEMENT(in);
	V8H_IMPLEMENT(out);
	V8H_IMPLEMENT(del);
	V8H_IMPLEMENT(error);
	V8H_IMPLEMENT(wait);
	V8H_IMPLEMENT(run);
	V8H_IMPLEMENT(close);
	V8H_IMPLEMENT(trigger);
	V8H_IMPLEMENT(triggerByEventId);
	V8H_IMPLEMENT(getCallbackFromEventId);

	V8H_IMPLEMENT(getFdsNumber);

	V8H_CREATE_END();
}


V8H_DEF_SYM(Service::fds);
V8H_DEF_SYM(Service::fd);
V8H_DEF_SYM(Service::event);
V8H_DEF_SYM(Service::fdsError);

V8H_NS_END
