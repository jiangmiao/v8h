#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "v8h.h"
#include "buffer.h"
#include "socket.h"

V8H_NS_START

int  Socket::setNonblock(int fd)
{
	int flag = fcntl(fd, F_GETFL);
	if (flag == -1)
		return -1;
	return fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

V8H_FUNCTION(bind)
{
	auto self = args.This();
	int argc = args.Length();
	V8H_ENSURE(argc > 1);
	int fd = TO_INT(args[0]);
	V8_STR(address, args[1]);
	int port = V8H_ARG(2, TO_INT, 0);

	int domain = GET_INT(self, V8H_SYM(domain));
	int fd     = GET_INT(self, V8H_SYM(fd));

	sockaddr_in addr     = {0};
	addr.sin_family      = AF_INET;
	addr.sin_port        = htons(port);
	addr.sin_addr.s_addr = inet_addr(address);

	int bind_result = bind(fd, (sockaddr*)&addr, sizeof(addr));
	V8H_SYSTEM_ENSURE( bind_result != -1);

	SET(self, V8H_SYM(address), args[0]);
	SET(self, V8H_SYM(port), port);
	return self;
}

V8H_FUNCTION(connect)
{
	auto self = args.This();
	int  argc = args.Length();
	V8H_ENSURE(argc > 0);
	V8H_STR(address, args[0]);
	int port = V8H_ARG(1, TO_INT, 0);
	int domain = GET_INT(self, V8H_SYM(domain));
	int fd = GET_INT(self, V8H_SYM(fd));

	sockaddr_in addr     = {0};
	addr.sin_family      = AF_INET;
	addr.sin_port        = htons(port);
	addr.sin_addr.s_addr = inet_addr(address);

	int connect_result = connect(fd, (sockaddr*)&addr, sizeof(addr));
	V8H_SYSTEM_ENSURE(connect_result != -1)
	return self;
}

V8H_FUNCTION(accept)
{
	auto self = args.This();
	
}


V8H_FORWARD_1_1(Socket, setNonblock, TO_INT, V8_INT);
V8H_FORWARD_2_1(Socket, listen, TO_INT, TO_INT, V8_INT);

// Constructor
// ===========
V8H_FUNCTION(Socket::constructor)
{
	return args.This();
}

// Socket
// ======
V8H_FUNCTION(socket)
{
	auto self     = args.This();
	int  argc     = args.Length();
	int  domain   = V8H_ARG(0, TO_INT, PF_INET);
	int  type     = V8H_ARG(1, TO_INT, SOCK_STREAM);
	int  protocol = V8H_ARG(2, TO_INT, 0);
	int  fd       = socket(domain, type, protocol);
	V8H_ENSURE( fd != -1 )

	SET(self , V8H_SYM(family)   , domain);
	SET(self , V8H_SYM(type)     , type);
	SET(self , V8H_SYM(protocol) , protocol);
	SET(self , V8H_SYM(fd)       , fd);
}

V8H_FUNCTION(assign)
{
	auto self = args.This();
	int argc = args.Length();
	
}

V8H_FUNCTION(createServer)
{
	
}

Handle<Function> Socket::create()
{
	V8H_CREATE_START();

	V8H_SET_SYM(fd);
	V8H_SET_SYM(address);
	V8H_SET_SYM(path);
	V8H_SET_SYM(port);

	V8H_STATIC_IMPLEMENT(_close);
	V8H_STATIC_IMPLEMENT(listen);

	V8H_STATIC_IMPLEMENT(bind_tcp);
	V8H_STATIC_IMPLEMENT(bind_unix);
	V8H_STATIC_IMPLEMENT(create_tcp);
	V8H_STATIC_IMPLEMENT(create_unix);
	V8H_STATIC_IMPLEMENT(_accept_tcp);
	V8H_STATIC_IMPLEMENT(_accept_unix);
	V8H_STATIC_IMPLEMENT(_connect_tcp);
	V8H_STATIC_IMPLEMENT(_connect_unix);

	V8H_STATIC_IMPLEMENT(_read_buffer);
	V8H_STATIC_IMPLEMENT(_write_utf8);
	V8H_STATIC_IMPLEMENT(_write_buffer);

	V8H_CREATE_END();
}

V8H_DEF_SYM(Socket::fd);
V8H_DEF_SYM(Socket::address);
V8H_DEF_SYM(Socket::port);
V8H_DEF_SYM(Socket::path);

V8H_NS_END
