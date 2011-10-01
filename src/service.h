#ifndef V8H_SERVICE_H
#define V8H_SERVICE_H
#include <unistd.h>
#include <sys/epoll.h>
#include "v8h.h"
#include "object.h"
#include "internal.h"

V8H_NS_START
class Service : public Object<Service>, public Internal<Service>
{
    protected:
	epoll_event *events;
	uint32_t      maxEvents;
	int         fd;
	int         fdsNumber;
    public:
	Service(uint32_t maxEvents);
	virtual ~Service();
	void mod(int fd, int events);
	void add(int fd, int events);
	void del(int fd);
	int wait(int timeout) ;
	int getFdByEventId(int eventId);
	typedef Service* InternalPointer;
    protected:
	static v8::Handle<v8::Value> inOrOut(const v8::Arguments &args, int events);
    public:
	static V8H_DEF_SYM(fds);
	static V8H_DEF_SYM(fdsError);
	static V8H_DEF_SYM(event);
	static V8H_DEF_SYM(fd);

	static V8H_FUNCTION(in);
	static V8H_FUNCTION(out);
	static V8H_FUNCTION(error);
	static V8H_FUNCTION(del);

	static V8H_FUNCTION(wait);
	static V8H_FUNCTION(run);
	static V8H_FUNCTION(close);

	static V8H_FUNCTION(trigger);
	static V8H_FUNCTION(triggerByEventId);

	static V8H_FUNCTION(getFdsNumber);
	static V8H_FUNCTION(getCallbackFromEventId);
	static V8H_FUNCTION(constructor);
	static v8::Handle<v8::Function> create();
};
V8H_NS_END
#endif
