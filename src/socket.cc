#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "v8h.h"
#include "buffer.h"
#include "socket.h"

namespace v8h
{
	int  Socket::listen(int fd, int backlog)
	{
		return ::listen(fd, backlog);
	}
	void  Socket::create_sockaddr_in(const char *address, int port, sockaddr_in *addr, socklen_t *addr_len)
	{
		memset(addr, 0, sizeof(sockaddr_in));
		auto s_addr = inet_addr(address);
		addr->sin_family      = AF_INET;
		addr->sin_addr.s_addr = s_addr;
		addr->sin_port        = htons(port);
		*addr_len = sizeof(sockaddr_in);
	}

	int  Socket::bind_tcp(int fd, const char *address, int port)
	{
		sockaddr_in addr;
		socklen_t   addr_len;
		create_sockaddr_in(address, port, &addr, &addr_len);
		return ::bind(fd, (sockaddr*)&addr, addr_len);
	}

	int  Socket::connect_tcp(int fd, const char *address, int port)
	{
		sockaddr_in addr;
		socklen_t   addr_len;
		create_sockaddr_in(address, port, &addr, &addr_len);
		return ::connect(fd, (sockaddr*)&addr, addr_len);
	}

	int  Socket::accept_tcp(int fd, sockaddr_in *addr)
	{
		socklen_t addr_len = sizeof(sockaddr_in);
		return ::accept(fd, (sockaddr*)addr, &addr_len);
	}

	void  Socket::create_sockaddr_un(const char *address, sockaddr_un *addr, socklen_t *addr_len)
	{
		memset(addr, 0, sizeof(sockaddr_un));
		addr->sun_family = AF_UNIX;
		strcpy(addr->sun_path, address);
		*addr_len = SUN_LEN(addr);
		// char *last = stpcpy(addr->sun_path, address);
		// *addr_len = last - addr + 1;

	}

	int  Socket::bind_unix(int fd, const char *address)
	{
		sockaddr_un addr;
		socklen_t   addr_len;
		create_sockaddr_un(address, &addr, &addr_len);
		return ::bind(fd, (sockaddr*)&addr, addr_len);

	}

	int  Socket::connect_unix(int fd, const char *address)
	{
		sockaddr_un addr;
		socklen_t   addr_len;
		create_sockaddr_un(address, &addr, &addr_len);
		return ::connect(fd, (sockaddr*)&addr, addr_len);
	}


	int  Socket::accept_unix(int fd, sockaddr_un *addr)
	{
		socklen_t addr_len = sizeof(sockaddr_un);
		return ::accept(fd, (sockaddr*)addr, &addr_len);
	}

	int  Socket::set_nonblock(int fd)
	{
		int flag = fcntl(fd, F_GETFL);
		if (flag == -1)
			return -1;
		return fcntl(fd, F_SETFL, flag | O_NONBLOCK);
	}

	V8H_DEF_SYM(fd);
	V8H_DEF_SYM(address);
	V8H_DEF_SYM(port);
	V8H_DEF_SYM(path);

	V8H_FUNCTION(Socket::set_nonblock)
	{
		auto fd = TO_INT(args[0]);
		int result = set_nonblock(fd);
		V8H_RETURN_INT(result);
	}

	// TCP
	V8H_FUNCTION(Socket::create_tcp)
	{
		int fd = socket(PF_INET, SOCK_STREAM, 0);
		set_nonblock(fd);
		V8H_RETURN_INT(fd);
	}

	V8H_FUNCTION(Socket::bind_tcp)
	{
		auto fd = TO_INT(args[0]);
		String::Utf8Value address(args[1]);
		auto port = TO_INT(args[2]);
		auto reuseaddr = TO_INT(args[3]);
		setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
		int result = bind_tcp(fd, *address, port);
		V8H_RETURN_INT(result);
	}

	V8H_FUNCTION(Socket::_accept_tcp)
	{
		auto server = TO_INT(args[0]);
		sockaddr_in addr = {0};
		auto fd = accept_tcp(server, &addr);
		if (fd == -1)
			V8H_RETURN_INT(fd);
		set_nonblock(fd);

		auto client = Object::New();
		SET(client , V8H_SYM(fd)      , fd);
		SET(client , V8H_SYM(address) , String::New(inet_ntoa(addr.sin_addr)));
		SET(client , V8H_SYM(port)    , ntohs(addr.sin_port));
		V8H_RETURN(client);
	}

	V8H_FUNCTION(Socket::_connect_tcp)
	{
		auto client_fd = TO_INT(args[0]);
		String::Utf8Value address(args[1]);
		auto port = TO_INT(args[2]);

		auto result = connect_tcp(client_fd, *address, port);
		V8H_RETURN_INT(result);
	}

	// UNIX
	V8H_FUNCTION(Socket::create_unix)
	{
		int fd = socket(PF_UNIX, SOCK_STREAM, 0);
		set_nonblock(fd);
		V8H_RETURN_INT(fd);
	}

	V8H_FUNCTION(Socket::bind_unix)
	{
		auto fd = TO_INT(args[0]);
		String::Utf8Value address(args[1]);
		auto result = bind_unix(fd, *address);
		V8H_RETURN_INT(result);
	}

	V8H_FUNCTION(Socket::_connect_unix)
	{
		auto fd = TO_INT(args[0]);
		String::Utf8Value address(args[1]);
		auto result = connect_unix(fd, *address);
		V8H_RETURN_INT(result);
	}

	V8H_FUNCTION(Socket::_accept_unix)
	{
		auto server = TO_INT(args[0]);
		sockaddr_un addr = {0};
		auto fd = accept_unix(server, &addr);
		if (fd == -1)
			V8H_RETURN_INT(fd);
		set_nonblock(fd);

		auto client = Object::New();
		SET(client , V8H_SYM(fd)      , fd);
		SET(client , V8H_SYM(path) , String::New(addr.sun_path));
		V8H_RETURN(client);
	}

	V8H_FUNCTION(Socket::_close)
	{
		auto fd     = TO_INT(args[0]);
		int  result = ::close(fd);
		V8H_RETURN_INT(result);
	}

	V8H_FUNCTION(Socket::_write_utf8)
	{
		auto fd = TO_INT(args[0]);
		String::Utf8Value data(args[1]);
		int n = send(fd, *data, data.length(), 0);
		V8H_RETURN_INT(n);
	}

	V8H_FUNCTION(Socket::_write_buffer)
	{
		auto fd            = TO_INT(args[0]);
		auto buffer_object = TO_OBJ(args[1]);
		auto buffer        = Buffer::get_internal(buffer_object);
		int n = 0;
		while (buffer->size()) {
			n = send(fd, buffer->data(), buffer->size(), 0);
			if (n > 0) {
				buffer->consume(n);
				continue;
			}
			if (n == -1) {
				if (errno == EAGAIN)
					n = 0;
				break;
			}
		}
		V8H_RETURN_INT(n);

	}

	// _read_buffer buffer
	V8H_FUNCTION(Socket::_read_buffer)
	{
		auto   fd            = TO_INT(args[0]);
		auto   buffer_object = TO_OBJ(args[1]);
		Buffer *buffer       = Buffer::get_internal(buffer_object);
		char *dest = buffer->prepare(8192);
		size_t n = recv(fd, dest, buffer->remain(), 0);
		if ( n == 0) {
			errno = ENETDOWN;
			n = -1;
		} else if ( n > 0) {
			buffer->commit(n);
		}
		V8H_RETURN_INT(n);
	}

	V8H_FUNCTION(Socket::listen)
	{
		auto fd = TO_INT(args[0]);
		auto backlog = TO_INT(args[1]);
		auto result = listen(fd, backlog);
		V8H_RETURN_INT(result);
	}

	V8H_FUNCTION(Socket::constructor)
	{
		V8H_SELF();
		V8H_RETURN_SELF();
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

}
