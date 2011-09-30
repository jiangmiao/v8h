#ifndef V8H_SOCKET_H
#define V8H_SOCKET_H
#include "v8h.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

V8H_NS_START
class Socket : public Object<Socket>
{
    public:
	static int setNonblock(int fd);

    public:
	V8H_DEF_SYM(fd);
	V8H_DEF_SYM(address);
	V8H_DEF_SYM(port);

	// Socket
	// ======
	static V8H_FUNCTION(bind);
	static V8H_FUNCTION(connect);
	static V8H_FUNCTION(accept);
	static V8H_FUNCTION(listen);

	// Writer
	// ======
	static V8H_FUNCTION(writeBuffer);

	// Reader
	// ======
	static V8H_FUNCTION(readBuffer);

	// Constructor
	// ===========
	static V8H_FUNCTION(constructor);
	static v8::Handle<v8::Function> create();
};
V8H_NS_END
#endif
