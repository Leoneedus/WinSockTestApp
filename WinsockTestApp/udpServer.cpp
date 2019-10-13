#include "udpServer.h"

#include "message.pb.h"
#include <WS2tcpip.h>

#include <iostream>
#include <thread>

UDPServer::UDPServer() : 
	m_portNo(0)
{
}

UDPServer::UDPServer(const std::string& ipAddress, uint16_t portNo) :
	m_portNo(portNo)
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		std::cout << "Can't start Winsock! " << wsOk;
		return;
	}

	SOCKET in = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(54000);

	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		std::cout << "Can't bind socket! " << WSAGetLastError() << std::endl;
		return;
	}

	std::thread receiverThread([&in, this]() { receiver(in); });
	receiverThread.join();


	closesocket(in);
}

UDPServer::~UDPServer()
{
	WSACleanup();
}

void UDPServer::receiver(SOCKET& socket)
{
	sockaddr_in client;
	int clientLength = sizeof(client);

	char buf[4096];

	std::cout << "Start listening..." << std::endl;


	while (true)
	{
		ZeroMemory(&client, clientLength);
		ZeroMemory(buf, 1024);

		int bytesIn = recvfrom(socket, buf, 1024, 0, (sockaddr*)&client, &clientLength);
		SearchRequest sr;
		sr.ParseFromArray(&buf, bytesIn);

		std::cout << "query " << sr.query() << "\n";
		std::cout << "page number " << sr.page_number() << "\n";
		std::cout << "result per page " << sr.result_per_page() << "\n";

		if (bytesIn == SOCKET_ERROR)
		{
			std::cout << "Error receiving from client " << WSAGetLastError() << std::endl;
			continue;
			return;
		}

		char clientIp[256];
		ZeroMemory(clientIp, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		std::cout << "Message recv from " << clientIp << " : " << buf << std::endl;
	}
}
