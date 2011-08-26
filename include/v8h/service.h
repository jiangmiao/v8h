#ifndef EPOLL_H
#define EPOLL_H

#include <unistd.h>
#include <sys/epoll.h>
#include <v8.h>
#include "v8h.h"
#include "config.h"

namespace v8h
{
	using namespace v8;
	class Service
	{
	    protected:
		epoll_event *events;
		size_t      max_events;
		int         fd;
		int         fds_number;

	    public:
		Service(size_t max_events)
		: max_events(max_events)
		, fds_number(0)
		{
			events = new epoll_event[max_events];
			fd     = epoll_create(max_events);
		}

		virtual ~Service()
		{
			delete events;
			::close(fd);
		}

		void mod(int fd, int events)
		{
			epoll_event event = {0};
			event.events  = events;
			event.data.fd = fd;
			epoll_ctl(this->fd, EPOLL_CTL_MOD, fd, &event);
		}

		void add(int fd, int events)
		{
			epoll_event event = {0};
			event.events  = events;
			event.data.fd = fd;
			epoll_ctl(this->fd, EPOLL_CTL_ADD, fd, &event);
			++fds_number;
		}

		void del(int fd)
		{
			epoll_event event = {0};
			epoll_ctl(this->fd, EPOLL_CTL_DEL, fd, &event);
			--fds_number;
		}

		int wait(int timeout) 
		{
			return epoll_wait(fd, events, max_events, timeout);
		}

		int get_fd_by_event_id(int event_id)
		{
			return events[event_id].data.fd;
		}

		typedef Service* InternalPointer;

	    protected:
		static Handle<Value> in_or_out(const Arguments &args, int events)
		{
			auto self     = args.This();
			auto fd       = TO_INT(args[0]);
			auto callback = TO_OBJ(args[1]);

			auto epoll    = get_internal(self);
			auto fds      = GET_OBJ(self, V8H_SYM(fds));
			if(fds->Has(fd)) {
				epoll->mod(fd, events);
			} else {
				epoll->add(fd, events);
			}

			fds->Set(fd, callback);
			return self;
		}


	    public:
		static V8H_DEF_SYM(fds);
		static V8H_DEF_SYM(fds_error);
		static V8H_DEF_SYM(event);
		static V8H_DEF_SYM(fd);

		enum {
			FIELD_INTERNAL,
			FIELDS_NUMBER
		};

		static V8H_IMPORT_GET_INTERNAL();

		/**
		 * Set poll in callback for fd
		 *
		 * \param fd
		 * \param callback
		 *
		 * \return self
		 */
		static V8H_FUNCTION(in)
		{
			return in_or_out(args, EPOLLIN);
		}


		/**
		 * Set poll out callback for fd
		 *
		 * \param fd
		 * \param callback
		 *
		 * \return self
		 */
		static V8H_FUNCTION(out)
		{
			return in_or_out(args, EPOLLOUT);
		}

		/**
		 * delete fds_error and fds callback
		 *
		 * \param fd
		 * \return self
		 */
		static V8H_FUNCTION(del)
		{
			auto self      = args.This();
			auto service   = get_internal(self);
			auto fd        = TO_INT(args[0]);
			auto fds       = GET_OBJ(self, V8H_SYM(fds));
			auto fds_error = GET_OBJ(self, V8H_SYM(fds_error));
			if (fds->Has(fd)) {
				service->del(fd);
				fds->Delete(fd);
			}
			fds_error->Delete(fd);
			return self;
		}

		/**
		 * Set error trigger
		 *
		 * \param fd
		 * \param callback
		 *
		 * \return self
		 */
		static V8H_FUNCTION(error)
		{
			auto self      = args.This();
			auto fd        = TO_INT(args[0]);
			auto callback  = TO_OBJ(args[1]);
			auto fds_error = GET_OBJ(self, V8H_SYM(fds_error));
			fds_error->Set(fd, callback);
			return self;
		}


		/**
		 * Wait epoll events
		 *
		 * \param int timeout in millions, default value is -1, wait forever.
		 *
		 * \return the number of events
		 */
		static V8H_FUNCTION(wait)
		{
			auto self     = args.This();
			int  timeout = -1;
			if (args.Length() > 0)
				timeout = TO_INT(args[0]);
			auto service = get_internal(self);
			int n =  service->wait(timeout);
			V8H_RETURN_INT(n);
		}

		/**
		 * delete the service
		 */
		static V8H_FUNCTION(close)
		{
			auto self     = args.This();
			DEL_PTR<Service*>(self, FIELD_INTERNAL);
			return self;
		}

		/**
		 * Get callback from event id
		 *
		 * \param event id
		 *
		 * \return the callback relate to the event id
		 */
		static V8H_FUNCTION(get_callback_from_event_id)
		{
			auto self     = args.This();
			auto event_id = TO_INT(args[0]);
			auto service  = get_internal(self);
			auto fds      = GET_OBJ(self, V8H_SYM(fds));
			auto fd       = service->get_fd_by_event_id(event_id);
			auto callback = fds->Get(fd);
			return callback;
		}


		/**
		 * Trigger a callback by the fd id
		 *
		 * \param fd id
		 *
		 * \return the same as callback returns
		 */

		static V8H_FUNCTION(trigger)
		{
			auto self     = args.This();
			auto fd       = TO_INT(args[0]);
			auto service  = get_internal(self);
			auto fds      = GET_OBJ(self, V8H_SYM(fds));
			auto callback = Handle<Function>::Cast(fds->Get(fd));
			auto result   = callback->Call(self, 0, NULL);
			return result;
		}

		/**
		 * Trigger a callback by the event id
		 *
		 * \param event id
		 *
		 * \return the same as callback returns
		 */
		static V8H_FUNCTION(trigger_by_event_id)
		{
			auto self     = args.This();
			auto event_id = TO_INT(args[0]);
			auto service  = get_internal(self);
			auto fd      = service->get_fd_by_event_id(event_id);
			auto fds      = GET_OBJ(self, V8H_SYM(fds));
			auto callback = Handle<Function>::Cast(fds->Get(fd));
			auto result   = callback->Call(self, 0, NULL);
			return result;
		}

		/**
		 */
		static V8H_FUNCTION(run)
		{
			auto   self      = args.This();
			auto   config    = Config::global();
			auto   service   = get_internal(self);
			auto   fds       = GET_OBJ(self, V8H_SYM(fds));
			auto   fds_error = GET_OBJ(self, V8H_SYM(fds_error));
			size_t times     = 0;
			size_t max_times = 0;
			if (max_times == 0) {
				max_times = 1e6;
			}
			while (service->fds_number) {
				int n = service->wait(80);
				if ( n == -1 ) {
					perror("service - wait");
					break;
				}

				if ( n>0 ) {
					HandleScope scope;
					for (int i=0; i<n; ++i) {
						int  fd       = service->get_fd_by_event_id(i);
						int  events   = service->events[i].events;
						Handle<Function> callback;
						if (events & EPOLLERR || events & EPOLLHUP) {
							callback = Handle<Function>::Cast(fds_error->Get(fd));
						} else {
							callback = Handle<Function>::Cast(fds->Get(fd));
						}
						auto result   = callback->Call(self, 0, NULL);
						if (result.IsEmpty()) {
							V8H_LEAVE(result);
						}
					}
				}

				times += n;
				if (times >= max_times) {
					times = 0;
					V8::LowMemoryNotification();
				}

			}
			return self;
		}

		static V8H_FUNCTION(constructor)
		{
			auto self       = args.This();

			int  max_events = 1024;
			if (args.Length() > 0)
				max_events = TO_INT(args[0]);

			auto *service   = new Service(max_events);

			SET_PTR(self , FIELD_INTERNAL     , service);
			SET(self     , V8H_SYM(fds)       , Object::New());
			SET(self     , V8H_SYM(fds_error) , Object::New());
			return self;
		}


		static V8H_FUNCTION(get_fds_number)
		{
			auto self = args.This();
			auto epoll = get_internal(self);
			return V8_INT(epoll->fds_number);
		}

		static Handle<Function> create()
		{
			V8H_CREATE_START_WITH_INTERNAL_FIELD();

			V8H_SET_SYM(fds);
			V8H_SET_SYM(event);
			V8H_SET_SYM(fd);
			V8H_SET_SYM(fds_error);

			V8H_IMPLEMENT(in);
			V8H_IMPLEMENT(out);
			V8H_IMPLEMENT(del);
			V8H_IMPLEMENT(error);
			V8H_IMPLEMENT(wait);
			V8H_IMPLEMENT(run);
			V8H_IMPLEMENT(close);
			V8H_IMPLEMENT(trigger);
			V8H_IMPLEMENT(trigger_by_event_id);
			V8H_IMPLEMENT(get_callback_from_event_id);

			V8H_IMPLEMENT(get_fds_number);

			V8H_CREATE_END();
		}

		static V8H_IMPORT_NEW_INSTANCE();
		static V8H_IMPORT_GLOBAL();
	};

	V8H_DEF_SYM(Service::fds);
	V8H_DEF_SYM(Service::fd);
	V8H_DEF_SYM(Service::event);
	V8H_DEF_SYM(Service::fds_error);
}

#endif
