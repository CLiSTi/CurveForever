#define _CRT_SECURE_NO_WARNINGS

#define CLIX_DEFAULT_PORT		1495

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock.h>

#include "Serialize.h"
#include "GamePackets.h"

#include <iostream>
#include <fstream>
using namespace std;

#pragma region random
#include "time.h"

bool reset = false;

void RandomReset()
{
	reset = true;
	srand(time(NULL));
}
int RandomInt(int low, int high)
{
	if (!reset) RandomReset();
	if (high - low == 0) return high;
	return rand() % (high - low) + low;
}

double RandomDouble(double low, double high)
{
	if (!reset) RandomReset();
	double f = (double)rand() / RAND_MAX;
	return low + f * (high - low);
}

#pragma endregion random

#pragma comment(lib, "ws2_32.lib")

struct DATA_PACKET
{
	float x, y, size, angle;
	int FrameID;
};
int id_count = 0;

bool GameStarted = false;

#define TYPE_SERVER_PACKET_8	101
#define TYPE_ID_REQ				200
#define TYPE_ID					201
#define TYPE_STARTED			202
#define ID_WAIT					10
#define ID_INVALID				12
#define ID_REQUEST				13
#define ID_OBTAINED				14

#define PCODE_SERVERPACKET4		111
#define PCODE_SERVERPACKET8		112

#define PLAYER_READY			180
#define PLAYER_UNREADY			181

#define BUF_SIZE				2000

//ServerFrameContainer	FrameContainer;
LobbyInfo				Lobby;

bool PendingReset[8];

GameStartInfo NewGameInfo;
//ServerPacket_4 UniPacket_4;
ServerPacket_8 UniPacket_8;
SOCKET mySocket;

PlayerPacket_8 Packet;
//Se atribuie date structurii Packet
void SendPacket()
{
	char Buffer[2000];
	SerializeT(Buffer, Packet, 120);
	int SentBytes = send(mySocket, Buffer, sizeof(PlayerPacket_8), 0);
}

void ReceivePacket()
{
	char Buffer[2000];
	PlayerPacket_8 RecPacket;
	int PacketType;
	int ReceivedBytes = recv(mySocket, Buffer, sizeof(Buffer), 0);
	if (Buffer[0] == 120)
		DeserializeT(Buffer, &RecPacket, &PacketType);
	else
		cout << "Packetul nu este de tip PlayerPacket_8.";
}

DWORD WINAPI ReceiveCMDS(LPVOID lpParam)
{
	//ofstream FRec, FSend, FLog;

	int ID = -1;
	bool disconnected = false;
	for (int i = 0; i < 8; i++)
		if (!Lobby.Player[i].Connected)
		{
			ID = i; 
			break;
		}
	
	if (ID == -1)
	{
		ExitThread(0);
		return 0;
	}
	
	Lobby_PlayerInfo PlayerInfo;
	//FLog << "thread created! \n";
	SOCKET current_client = (SOCKET)lpParam;

	char buf[BUF_SIZE];
	strcpy_s(buf, sizeof(buf), "");
	char SendData[BUF_SIZE];

	int res;
	int cmd, type, rec;
	//PlayerPacket_8 packet_8_source;
	//ServerPacket_8 server_packet;
	
	//server_packet.StartFrameID = 0;
	//server_packet.EndFrameID = 0;
	res = recv(current_client, buf, 20, 0);
	//FLog << "Numele:  " << buf << endl;
	Lobby.ConnectedPlayers++;
	//FLog << "Total clients: " << Lobby.ConnectedPlayers << endl;
	Lobby.Player[ID].Connected = true;
	for (int i = 0; i < strlen(buf); i++)
		Lobby.Player[ID].Name[i] = buf[i];
	Lobby.Player[ID].Ready = false;
	Lobby.Player[ID].Score.FFA = 700;
	Lobby.Player[ID].Score.TEAM = 700;
	Lobby.Player[ID].Score._1v1 = 700;
	//FLog << "Waiting for player ask for ID " << ID << " : " << endl;
	char ID_S[20];
	for (int i = 0; i < 10; i++)
	{
		strcpy_s(ID_S, sizeof(ID_S), "");
		ID_S[0] = (char)48;
		ID_S[1] = (char)96;
		ID_S[2] = (char)100;
		ID_S[3] = (char)101;
		ID_S[4] = ID + 10;
		send(current_client, ID_S, sizeof(ID_S), 0);
		Sleep(20);
	}
	
	//FLog << "ID SENT: " << ID << endl;
	
	int Response;
	while (!GameStarted && !disconnected)
	{
		strcpy_s(buf, sizeof(buf), "");

		SerializeT(buf, Lobby, 150);			// Lobby Information
		send(current_client, buf, sizeof(LobbyInfo) + 1, 0);
		Response = recv(current_client, buf, BUF_SIZE, 0);
		if (Response == -1)
			disconnected = true;
			//break;
		//cout << "Player " << ID << " response size " << Response << endl;
		DeserializeT(buf, &PlayerInfo, &type);
		Lobby.Player[ID].Ready = PlayerInfo.Ready;
		Lobby.Player[ID].Play = PlayerInfo.Play;
		//cout << "Player " << ID << " : Ready = " << PlayerInfo.Ready;
		//cout << " Play = " << PlayerInfo.Play << endl;
		GameStarted = true;
		for (int i = 0; i < 8; i++)
			if (!Lobby.Player[i].Ready && Lobby.Player[i].Connected)
			GameStarted = false;
		Sleep(50);
	}

	strcpy_s(buf, sizeof(buf), "");
	SerializeT(buf, Lobby, 150);			// Lobby Information
	send(current_client, buf, sizeof(LobbyInfo) + 1, 0);
	recv(current_client, buf, sizeof(buf), 0);
	//cout << "Starting game ! " << endl;

	Sleep(20);
	strcpy_s(buf, sizeof(buf), "");
	SerializeT(buf, NewGameInfo, 160);
	send(current_client, buf, sizeof(NewGameInfo) + 1, 0);
	//cout << "Game info sent!" << endl;
	int pt = 0;
	long PacketCount = 0;
	int faar = 0;
	while(1)
	{
		if (PendingReset[ID])
		{
			strcpy(buf, "");
			buf[0] = 107;
			buf[1] = 107;
			buf[2] = 107;
			buf[3] = 107;
			send(current_client, buf, 4, 0);
			//FLog << "Player " << ID << " successfully reseted!" << endl;
			//FSend << "Reset! \n" << endl;
			PendingReset[ID] = false;
			//Sleep(20);
		}
		strcpy(buf, "");
		int p = recv(current_client, buf, sizeof(buf), 0);
		//FLog << p << "\n";
		if (p <= 0)
		{
			//FLog << "stopped !\n";
			break;
		}
		else if ((int)buf[0] == 102)
		{
			PacketCount++;
			//PlayerPacket_4 Packet_4;
			PlayerPacket_8 Packet_8;
		//	DeserializeT(buf, &Packet_4);
			DeserializeT(buf, &Packet_8);
		//	UniPacket_4.Player[ID] = Packet_4;
			UniPacket_8.Player[ID] = Packet_8;
			//FRec << "Packet no#" << PacketCount << "\n";
			//FRec << "\nReceived for " << ID << ":\n";
			//for (int i = 0; i < 4; i++)
				//FRec << "Frame " << i << ": x = " << UniPacket.Player[ID].Frame[i].x << ": y = " << UniPacket.Player[ID].Frame[i].y << endl;
			strcpy(buf, "");
		//	SerializeT(buf, UniPacket_4, PCODE_SERVERPACKET4);
			SerializeT(buf, UniPacket_8, PCODE_SERVERPACKET8);
			send(current_client, buf, sizeof(ServerPacket_8), 0);
			
			//for (int i = 0; i < 8; i++)
			//{
				//FSend << "Sending to Player " << i << ": \n";
				//for (int j = 0; j < 4; j++)
					//FSend << "Frame " << i << ": x = " << Packet.Frame[j].x << " y = " << Packet.Frame[j].y << endl;
			//}
		}
		else if (p != 0)
		{
			//FLog << " size of pachet: " << p << endl;
			//FLog << "Player " << ID << " received code " << (int)buf[0] << " with text " << buf << endl;
		}			

		switch ((int)buf[0])
		{
		case 107:
			for (int i = 0; i < 8; i++)
			{
				PendingReset[i] = true;
				//FLog << "Reset for player " << i << " pending... " << endl;
			}
			break;
		}
		Sleep(15);
	}

	//Sleep(5000);
	//FLog << "Player ID" << ID << "Disconnected!" << endl;
	Lobby.Player[ID].Connected = false;
	Lobby.ConnectedPlayers--;
	//FRec.close();
	//FSend.close();
	closesocket(current_client);
	ExitThread(0);

}




int main()
{
	printf("Starting up #CLIX Server. \n");
	
	SOCKET sock;

	DWORD thread;

	WSADATA wsaData;
	sockaddr_in server;

	int ret = WSAStartup(0x0202, &wsaData);
	if (ret != 0)
		return -1;


	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(CLIX_DEFAULT_PORT);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
	{
		return -2;
	}

	if (bind(sock, (sockaddr*)&server, sizeof(server)) != 0)
	{
		return -3;
	}

	if (listen(sock, 5) != 0)
	{
		return -4;
	}

	SOCKET client;
	sockaddr_in from;
	int fromlen = sizeof(from);
	
	Lobby.ConnectedPlayers = 0;
	for (int i = 0; i < 8; i++)
	{
		Lobby.Player[i].ID = i;
		Lobby.Player[i].Ready = false;
		Lobby.Player[i].Connected = false;
		PendingReset[i] = false;
	}
		

	while (true)
	{
		if (Lobby.ConnectedPlayers == 0)
		{
			cout << "No players. Game started = false." << endl;
			GameStarted = false;
		}
			
		client = accept(sock, (sockaddr*)&from, &fromlen);
		printf("Client connected ! \n");

		CreateThread(NULL, 0, ReceiveCMDS, (LPVOID)client, 0, &thread);
		for (int i = 0; i < 8; i++)
		{
			NewGameInfo.PlayerInfo[i].x = RandomDouble(100.f, 500.f);
			NewGameInfo.PlayerInfo[i].y = RandomDouble(100.f, 500.f);
			NewGameInfo.PlayerInfo[i].angle = RandomDouble(0.f, 6.24f);
		}
	}

	closesocket(sock);
	WSACleanup();

	return 0;
}