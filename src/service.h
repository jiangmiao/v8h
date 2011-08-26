#ifndef V8H_SERVICE_H
#define V8H_SERVICE_H
#include <unistd.h>
#include <sys/epoll.h>
#include <v8.h>
#include "v8h.h"
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
		Service(size_t max_events);
		virtual ~Service();
		void mod(int fd, int events);
		void add(int fd, int events);
		void del(int fd);
		int wait(int timeout) ;
		int get_fd_by_event_id(int event_id);
		typedef Service* InternalPointer;
	    protected:
		static Handle<Value> in_or_out(const Arguments &args, int events);
	    public:
		static V8H_DEF_SYM(fds);
		static V8H_DEF_SYM(fds_error);
		static V8H_DEF_SYM(event);
		static V8H_DEF_SYM(fd);
		enum {
			FIELD_INTERNAL,
			FIELDS_NUMBER
		};
		/**
		 * Set poll in callback for fd
		 *
		 * \param fd
		 * \param callback
		 *
		 * \return self
		 */
		static V8H_FUNCTION(in);
		/**
		 * Set poll out callback for fd
		 *
		 * \param fd
		 * \param callback
		 *
		 * \return self
		 */
		static V8H_FUNCTION(out);
		/**
		 * delete fds_error and fds callback
		 *
		 * \param fd
		 * \return self
		 */
		static V8H_FUNCTION(del);
		/**
		 * Set error trigger
		 *
		 * \param fd
		 * \param callback
		 *
		 * \return self
		 */
		static V8H_FUNCTION(error);
		/**
		 * Wait epoll events
		 *
		 * \param int timeout in millions, default value is -1, wait forever.
		 *
		 * \return the number of events
		 */
		static V8H_FUNCTION(wait);
		/**
		 * delete the service
		 */
		static V8H_FUNCTION(close);
		/**
		 * Get callback from event id
		 *
		 * \param event id
		 *
		 * \return the callback relate to the event id
		 */
		static V8H_FUNCTION(get_callback_from_event_id);
		/**
		 * Trigger a callback by the fd id
		 *
		 * \param fd id
		 *
		 * \return the same as callback returns
		 */
		static V8H_FUNCTION(trigger);
		/**
		 * Trigger a callback by the event id
		 *
		 * \param event id
		 *
		 * \return the same as callback returns
		 */
		static V8H_FUNCTION(trigger_by_event_id);
		/**
		 */
		static V8H_FUNCTION(run);
		static V8H_FUNCTION(constructor);
		static V8H_FUNCTION(get_fds_number);
		static Handle<Function> create();

		static V8H_DECLARE_GLOBAL();
		static V8H_DECLARE_GET_INTERNAL();
		static V8H_DECLARE_NEW_INSTANCE();
	};
}
#endif
