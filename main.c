// bY SP
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")
#define PORT "80"
#define IP "wwww.icanhazip.com"
#define REQUEST "GET /\r\n\r\n"
#define BUFFSIZE 256
int main() {
	SOCKET ConnectSocket = INVALID_SOCKET;
	int iResult;
	char recvBuff[BUFFSIZE] ={0};
	WSADATA wsaData;
	struct addrinfo hints, *result = NULL, *ptr = NULL;
	ZeroMemory(&hints, sizeof(hints));
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult !=0) {
		printf("<WSASTARTUP> a échoué %i\n", iResult);
		return 1;
	}
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	iResult = getaddrinfo(IP,PORT,&hints, &result);
	if(iResult != 0) {
		printf("<GETADDRINFO> a échoué %i\n", iResult);
		WSACleanup();
		return 1;
	}
	ConnectSocket = socket(result->ai_family,result->ai_socktype,result->ai_protocol);
	if(ConnectSocket == INVALID_SOCKET) {
		printf("Initialisation de socket a echoué  %i\n",WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	iResult = connect(ConnectSocket, result->ai_addr,(int)result->ai_addrlen);
	if(iResult == SOCKET_ERROR) {
		printf("<CONNECT> a échoué %i\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	iResult = send(ConnectSocket, REQUEST,(int)strlen(REQUEST), 0);
	if(iResult == SOCKET_ERROR) {
		printf("<SEND> a échoué %i\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	printf("Octets a été envoyés %i\n", iResult);
	iResult = recv(ConnectSocket, recvBuff, BUFFSIZE, 0);
	if(iResult > 0) {
		printf("Octets reçus %d\n", iResult);
		printf("Votre adresse IP publique est  %s", recvBuff);
	} else {
		printf("<RECV> a echoué %d\n", WSAGetLastError());
	}
	closesocket(ConnectSocket);
	WSACleanup();
	system("pause");
}
