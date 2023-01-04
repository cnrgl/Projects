#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>


//linking Ws2_32.lib or link with gcc cli.c -o cli.exe -lws2_32
#pragma comment (lib,"Ws2_32.lib")

#define DEF_BUFLEN 512
#define DEF_PORT "2021"

const char* rx(SOCKET *cli);
int tx(SOCKET *cli);
int tx(SOCKET *cli, const char* message);

int __cdecl main(void)
{
	WSADATA wsad;
	int ires;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo* res = NULL;
	struct addrinfo hints;

	int iSendres;
	
	ires = WSAStartup(MAKEWORD(2, 2), &wsad);
	if (ires != 0)
	{
		printf("WASAStartup failure...\n");
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	//get machine ip for server
		ires = getaddrinfo(NULL, DEF_PORT, &hints, &res);
		if (ires != 0)
		{
			printf("getaddrinfo failed(%d)...\n", ires);
			WSACleanup();
			return 1;
		}
		else
			printf("getaddrinfo success...\n");
	

	ListenSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("socket failed with error:%ld \n", WSAGetLastError());
		freeaddrinfo(res);
		WSACleanup();
		return 1;
	}
	else
		printf("socket success...\n");
	//binding
	ires = bind(ListenSocket, res->ai_addr, (int)res->ai_addrlen);
	if (ires == SOCKET_ERROR)
	{
		printf("bind failed(%d)...\n", WSAGetLastError());
		freeaddrinfo(res);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
		printf("binding success...\n");

	freeaddrinfo(res);

	ires = listen(ListenSocket, SOMAXCONN);//SOMAXCONN ???
	if (ires == SOCKET_ERROR)
	{
		printf("listen failed with error %d", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
		printf("listen success...\n");


	//Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);//accept every port
	if (ClientSocket == INVALID_SOCKET)
	{
		printf("acccept failed(%d)...\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
		printf("accept success...\n");

	//No longer need server socket
	closesocket(ListenSocket);
	//////////////////////////////////////USER AREA////////////////////////////////////////////////////////////////
	if ((strcmp(rx(&ClientSocket),"\0")))
		tx(&ClientSocket,"ok");

	const char* buf;
	while(true)
	{
		int del = 1;// atoi(buf);//delimeter of sentences
		buf = NULL;
		//while (del) {
			buf = rx(&ClientSocket);
			printf("recv: ");
			if (!strcmp(buf, "bye"))
			{
				closesocket(ClientSocket);
				WSACleanup();
				return 0;
			}
				//del = -1;
			else
				printf("%s",buf);
			//	del--;
		//}
		//if (del>0)//out exception
			//break;
		if (tx(&ClientSocket) > 0);
		else
			break;
	} 

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	ires = shutdown(ClientSocket, SD_SEND);
	if (ires == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		//not needed below this section
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	printf("shutdown completed...\n");
	//cleanup
	closesocket(ClientSocket);
	WSACleanup();


	return 0;
}
const char* rx(SOCKET *cli)
{
	int recvbuflen = DEF_BUFLEN;
	char recvbuf[DEF_BUFLEN];
	memset(recvbuf, '\0', recvbuflen);

	int ires = recv(*cli, recvbuf, recvbuflen, 0);
	if (ires < 0)
	{
		printf("recv failed with error:%d\n", WSAGetLastError());
		closesocket(*cli);
		WSACleanup();
		return "\0";
	}
	printf("%s\n",recvbuf);
		return (const char*)recvbuf;
}
int tx(SOCKET *cli)
{
	char sendbuf[DEF_BUFLEN];
	memset(sendbuf, '\0', DEF_BUFLEN);
	printf("\nsend:");
	scanf_s("%s", sendbuf, (unsigned)_countof(sendbuf));
	
	int ires = send(*cli, sendbuf,sizeof(sendbuf), 0);
	if (ires == SOCKET_ERROR)
	{
		printf("send failed with error:%d\n", WSAGetLastError());
		closesocket(*cli);
		WSACleanup();
		return -1;
	}
	
	//there is no escape option
	return 1;
}
int tx(SOCKET* cli,const char* message)
{
	int ires = send(*cli, message, sizeof(message), 0);
	if (ires == SOCKET_ERROR)
	{
		printf("send failed with error:%d\n", WSAGetLastError());
		closesocket(*cli);
		WSACleanup();
		return -1;
	}
	printf("sended successfully...\n");
	//there is no escape option
	return 1;
}

