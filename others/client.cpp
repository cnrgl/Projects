#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "2021"

int rx(SOCKET *con);
int tx(SOCKET *con);
int tx(SOCKET *con, const char* message);

int __cdecl main(int argc, char** argv)
{
	WSADATA wsaData;
	SOCKET conSocket = INVALID_SOCKET;
	struct addrinfo* res = NULL,
		* ptr = NULL,
		hints;
	int ires;
	

	if(argc!=2)
	{
		printf("usage :%s server-name\n",argv[0]);
		return 1;
	}
	//initialization...
	ires = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ires != 0)
	{
		printf("WSAStartup failed with error: %d\n",ires);
		return 1;
	}
	else
		printf("WSAStartup successfully...\n");
	
	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//resolve the server adress and port
	ires = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &res);
	if (ires != 0)
	{
		printf("getaddrinfo failed with error:%d \n", ires);
		WSACleanup();
		return 1;
	}
	else
		printf("addres get successfully...\n");

	//Attempt toconnect to an adress until one succeeds
	for (ptr = res; ptr != NULL; ptr = ptr->ai_next)
	{
		//create socket for connection server
		//socket can be change for different server adresses
		conSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (conSocket == INVALID_SOCKET)
		{
			printf("socket failed with error :%ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}
		else
			printf("socket attempt successfull...\n");
		//connect server
		ires = connect(conSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (ires == SOCKET_ERROR)
		{
			closesocket(conSocket);
			conSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(res);

	if (conSocket == INVALID_SOCKET)
	{
		printf("unable connect server :( ...\n");
		WSACleanup();
		return 1;
	}

	//Send hi
	tx(&conSocket, "hi");
	printf("hi sended successfully...\n");
	printf("waiting response from server:\n");
		//recieving
	//////////////////////////user area/////////////////////////////////////
		while (true)
		{
			if (rx(&conSocket) > 0);
			else
				break;
			if (tx(&conSocket) > 0);
			else
				break;
		}
	//////////////////////////////////////////////////////////////////////////
		//shutdown
		ires = shutdown(conSocket, SD_SEND);
		if (ires == SOCKET_ERROR)
		{
			printf("shutdown failed with error:%d\n", WSAGetLastError());
			closesocket(conSocket);
			WSACleanup();
			return 1;
		}
		printf("Connection closed\n");

		//cleanup
		closesocket(conSocket);
		WSACleanup();

	return 0;
}
int rx(SOCKET *con)
{
	char recvbuf[DEFAULT_BUFLEN];
	int recvlen = DEFAULT_BUFLEN;

	memset(recvbuf, '\0', recvlen);
	int ires = recv(*con, recvbuf, recvlen, 0);
	if (ires > 0)
	{
		printf("recv:%s", recvbuf);
		//escape control
		if (!strcmp(recvbuf, "bye"))
			return -1;
	}
	else
	{
		printf("recv failed with error :%d\n", WSAGetLastError());
		return -1;
	}
	
	return 1;
}
int tx(SOCKET *con, const char* message)
{
	int ires = send(*con, message, (int)strlen(message), 0);
	if (ires == SOCKET_ERROR)
	{
		printf("send failed with error:%d\n", WSAGetLastError());
		closesocket(*con);
		WSACleanup();
		return 1;
	}
	printf("sended successfully...\n");
	//escape control from server
	if (strcmp(message, "bye") != 0)
		printf("sended successfully...\n");
	else
		return -1;
	return 1;
}
int tx(SOCKET *con)
{
	char sendbuf[DEFAULT_BUFLEN] = { '\0' };
	memset(sendbuf,'\0',DEFAULT_BUFLEN);
	printf("\nsend:");
	scanf_s("%s", sendbuf, (unsigned)_countof(sendbuf));
	printf("test:%s\n", sendbuf);
	int ires = send(*con, sendbuf, (int)strlen(sendbuf), 0);
	if (ires == SOCKET_ERROR)
	{
		printf("send failed with error:%d\n", WSAGetLastError());
		closesocket(*con);
		WSACleanup();
		return 1;
	}
	//escape control from server
	if (strcmp(sendbuf, "bye") == 0)
		return -1;
	return 1;
}
