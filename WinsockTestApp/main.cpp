#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "udpServer.h"

namespace {
	const char* HOME = "127.0.0.1";
	const char* NETWORK = "10.9.105.37";
}

void main()
{
	UDPServer server{HOME, 5400};
}