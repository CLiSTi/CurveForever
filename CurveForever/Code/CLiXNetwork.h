#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <winsock.h>
#include "Serialize.h"

#pragma comment(lib, "ws2_32.lib")

#define CLIX_NETWORK_MESSAGE	9594
#define CLIX_DEFAULT_PORT		1495

#define CLIX_LOBBY_PACKEY 240
#define CLIX_GAME_PACKET  241

#define CLIX_BUFFER_SIZE 2000


#define AF_INET 2
//#define 

#define CLIX_NETWORK_SERVER 101
#define cLIX_NETWORK_CLIENT 102

class ClixNet
{
public:
	static char *HostIPAdress;
	static char HostIP[20];
	static int PortNo;
	static bool Connected;
	static bool isServer;
	static bool isClient;
	//static int ConType;
	static char Buffer[CLIX_BUFFER_SIZE];
	static SOCKADDR_IN from;
	static int from_size;
	static SOCKET ClixSocket;
	static WSADATA w;
	static int ListenOnPort(int PortNo);
	static int ConnectToHost(int PortNo, char* IPAdress);
	static int ParseMessage(UINT message, WPARAM wParam, LPARAM lParam);
	static void CloseConection() {
			if (ClixSocket) closesocket(ClixSocket);
			WSACleanup();
			Connected = false;
		}
	/*static int InitServer();*/
	static int InitClient();
};