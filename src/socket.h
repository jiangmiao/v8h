#ifndef V8H_SOCKET_H
#define V8H_SOCKET_H
#include "v8h.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

namespace v8h
{
	class Socket
	{
	    public:
		static int listen(int fd, int backlog);
		static void create_sockaddr_in(const char *address, int port, sockaddr_in *addr, socklen_t *addr_len);
		static int bind_tcp(int fd, const char *address, int port);
		static int connect_tcp(int fd, const char *address, int port);
		static int accept_tcp(int fd, sockaddr_in *addr);
		static void create_sockaddr_un(const char *address, sockaddr_un *addr, socklen_t *addr_len);
		static int bind_unix(int fd, const char *address);
		static int connect_unix(int fd, const char *address);
		static int accept_unix(int fd, sockaddr_un *addr);
		static int set_nonblock(int fd);
	    public:
		static V8H_DEF_SYM(fd);
		static V8H_DEF_SYM(address);
		static V8H_DEF_SYM(port);
		static V8H_DEF_SYM(path);
		static V8H_FUNCTION(set_nonblock);
		// TCP
		static V8H_FUNCTION(create_tcp);
		static V8H_FUNCTION(bind_tcp);
		static V8H_FUNCTION(_accept_tcp);
		static V8H_FUNCTION(_connect_tcp);
		// UNIX
		static V8H_FUNCTION(create_unix);
		static V8H_FUNCTION(bind_unix);
		static V8H_FUNCTION(_connect_unix);
		static V8H_FUNCTION(_accept_unix);
		static V8H_FUNCTION(_close);
		static V8H_FUNCTION(_write_utf8);
		static V8H_FUNCTION(_write_buffer);
		// _read_buffer buffer
		static V8H_FUNCTION(_read_buffer);
		static V8H_FUNCTION(listen);
		static V8H_FUNCTION(constructor);
		static Handle<Function> create();
	};
}
#endif
