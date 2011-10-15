#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "v8h.h"
#include "buffer.h"
#include "socket.h"

V8H_NS_START

void makeSockaddrIn(sockaddr_in *dest, v8::Handle<v8::Value> address, v8::Handle<v8::Value> port)
{
	dest->sin_family      = AF_INET;
	dest->sin_port        = htons(TO_INT(port));
	dest->sin_addr.s_addr = inet_addr(*v8::String::Utf8Value(address));
}

void makeSockaddrUn(sockaddr_un *dest, v8::Handle<v8::Value> address)
{
	dest->sun_family = AF_UNIX;
	snprintf(dest->sun_path, sizeof(dest->sun_path), "%s", *v8::String::Utf8Value(address));
}

int  Socket::setNonblock(int fd)
{
	int flag = fcntl(fd, F_GETFL);
	if (flag == -1)
		return -1;
	return fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

// Socket
// ======
V8H_FUNCTION(Socket::socket)
{
	V8H_THROW(args.Length() != 3);
	auto fd = ::socket(TO_INT(args[0]), TO_INT(args[1]), TO_INT(args[2]));
	if (fd > 0)
		setNonblock(fd);
	return V8_INT(fd);
}
V8H_C_1_1(Socket, close, TO_INT, V8_INT);
V8H_C_2_1(Socket, shutdown, TO_INT, TO_INT, V8_INT);


V8H_FUNCTION(Socket::bindTcp)
{
	V8H_ASSERT(args.Length() == 3);
	sockaddr_in addr     = {0};
	makeSockaddrIn(&addr, args[1], args[2]);
	return V8_INT(::bind(TO_INT(args[0]), (sockaddr*)&addr, sizeof(addr)));
}

V8H_FUNCTION(Socket::bindUnix)
{
	V8H_ASSERT(args.Length() == 2);
	sockaddr_un addr = {0};
	makeSockaddrUn(&addr, args[1]);
	return V8_INT(::bind(TO_INT(args[0]), (sockaddr*)&addr, SUN_LEN(&addr)));
}


V8H_FUNCTION(Socket::connectTcp)
{
	V8H_ASSERT(args.Length() == 3);
	sockaddr_in addr     = {0};
	makeSockaddrIn(&addr, args[1], args[2]);
	return V8_INT(::connect(TO_INT(args[0]), (sockaddr*)&addr, sizeof(addr)));
}

V8H_FUNCTION(Socket::connectUnix)
{
	V8H_ASSERT(args.Length() == 2);
	sockaddr_un addr = {0};
	makeSockaddrUn(&addr, args[1]);
	return V8_INT(::connect(TO_INT(args[0]), (sockaddr*)&addr, SUN_LEN(&addr)));
}


V8H_FUNCTION(Socket::acceptTcp)
{
	V8H_ASSERT(args.Length() == 1);
	sockaddr_in addr = {0};
	socklen_t addr_len = sizeof(addr);
	int fd =  ::accept(TO_INT(args[0]), (sockaddr*)&addr, &addr_len);
	if (fd == -1)
		return v8::Null();
	setNonblock(fd);
	auto result = v8::Array::New();
	result->Set(0, V8_INT(fd));
	result->Set(1, v8::String::New(inet_ntoa(addr.sin_addr)));
	result->Set(2, V8_INT(ntohs(addr.sin_port)));
	return result;
}

V8H_FUNCTION(Socket::acceptUnix)
{
	V8H_ASSERT(args.Length() == 1);
	sockaddr_un addr = {0};
	socklen_t addr_len = sizeof(addr);
	int fd = ::accept(TO_INT(args[0]), (sockaddr*)&addr, &addr_len);
	if (fd == -1)
		return v8::Null();
	setNonblock(fd);
	auto result = v8::Array::New();
	result->Set(0, V8_INT(fd));
	result->Set(1, v8::String::New(addr.sun_path));
	result->Set(2, V8_INT(0));
	return result;
}


V8H_C_2_1(Socket, listen, TO_INT, TO_INT, V8_INT);

// writeBuffer(fd, buffer)
V8H_FUNCTION(Socket::writeBuffer)
{
	int argc = args.Length();
	V8H_ASSERT(argc == 2);
	int  fd          = TO_INT(args[0]);
	auto buffer      = Buffer::getInternal(args[1]);

	int n = ::send(fd, buffer->data(), buffer->size(), 0);
	if (n > 0)
		buffer->consume(n);
	return V8_INT(n);
}

// readBuffer(fd, buffer)
V8H_FUNCTION(Socket::readBuffer)
{
	int  argc       = args.Length();
	V8H_ASSERT(argc == 2);
	int  fd         = TO_INT(args[0]);
	auto buffer     = Buffer::getInternal(args[1]);
	int  total  = 0;
	char *data  = NULL;
	int  remain = 0;
	int  n = 0;
	do {
		data   = buffer->prepare(512);
		remain = buffer->remain();
		n = recv(fd, data, remain, 0);
		if (total == 0 && (n < 0 )) {
			return V8_INT(n);
		}
		total += n;
		buffer->commit(n);
	} while( remain == n);
	return V8_INT(total);
}

v8::Handle<v8::Value> Socket::create()
{
	V8H_CREATE_OBJECT_START();

	V8H_STATIC_IMPLEMENT(socket);
	V8H_STATIC_IMPLEMENT(close);
	V8H_STATIC_IMPLEMENT(shutdown);
	V8H_STATIC_IMPLEMENT(listen);

	V8H_STATIC_IMPLEMENT(bindTcp);
	V8H_STATIC_IMPLEMENT(bindUnix);

	V8H_STATIC_IMPLEMENT(connectTcp);
	V8H_STATIC_IMPLEMENT(connectUnix);

	V8H_STATIC_IMPLEMENT(acceptTcp);
	V8H_STATIC_IMPLEMENT(acceptUnix);

	V8H_STATIC_IMPLEMENT(readBuffer);
	V8H_STATIC_IMPLEMENT(writeBuffer);

	V8H_CONSTANT_INT(PF_INET);
	V8H_CONSTANT_INT(PF_UNIX);
	V8H_CONSTANT_INT(AF_INET);
	V8H_CONSTANT_INT(SOCK_STREAM);
	V8H_CONSTANT_INT(SHUT_RD);
	V8H_CONSTANT_INT(SHUT_WR);
	V8H_CONSTANT_INT(SHUT_RDWR);

	V8H_CREATE_END();
}

V8H_NS_END
