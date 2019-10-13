#pragma once

#include <stdint.h>
#include <string>

#include <WinSock2.h>

class UDPServer
{
public:
	UDPServer();
	explicit UDPServer(const std::string& ipAddress, uint16_t portNo);
	~UDPServer();

private:

	void receiver(SOCKET& socket);

	uint16_t m_portNo;
};

