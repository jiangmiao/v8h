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
	// Socket
	// ======
	static V8H_FUNCTION(socket);
	static V8H_FUNCTION(shutdown);
	static V8H_FUNCTION(close);
	static V8H_FUNCTION(listen);

	static V8H_FUNCTION(bindTcp);
	static V8H_FUNCTION(bindUnix);

	static V8H_FUNCTION(connectTcp);
	static V8H_FUNCTION(connectUnix);

	static V8H_FUNCTION(acceptTcp);
	static V8H_FUNCTION(acceptUnix);

	// Writer
	// ======
	static V8H_FUNCTION(writeBuffer);

	// Reader
	// ======
	static V8H_FUNCTION(readBuffer);

	// Constructor
	// ===========
	static v8::Handle<v8::Value> create();
};
V8H_NS_END
#endif
