#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include "message.pb.h"

namespace {
	const char* HOME = "127.0.0.1";
	const char* NETWORK = "10.9.105.37";
}

std::string createMessage()
{
	SearchRequest sr;

	sr.set_query("Testing message");
	sr.set_page_number(42);
	sr.set_result_per_page(84);

	return sr.SerializeAsString();
}

void main()
{

	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		std::cout << "Can't start Winsock! " << wsOk;
		return;
	}

	sockaddr_in server;
	server.sin_family = AF_INET; 
	server.sin_port = htons(54000);
	inet_pton(AF_INET, "10.9.105.37", &server.sin_addr);

	SOCKET out = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	while (true)
	{
		std::string s = createMessage();
		int sendOk = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

		if (sendOk == SOCKET_ERROR)
		{
			std::cout << "That didn't work! " << WSAGetLastError() << std::endl;
		}

		Sleep(1000);
	}


	closesocket(out);

	WSACleanup();
}