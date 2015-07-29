#pragma once

#define PACKET_BUFFER 1024

#include<iostream>
using namespace std;

struct PlayerStartInfo
{
	float x, y, angle;
};

struct GameStartInfo
{
	PlayerStartInfo PlayerInfo[8];
};

struct PlayerScore
{
	int FFA, TEAM, _1v1;
};

struct PlayerInfo
{
	int ID;
	int Color;
	char Name[20];
	PlayerScore Score;	
	bool Connected, Ready, Play;
};

struct LobbyInfo
{
	PlayerInfo Player[8];
	int ConnectedPlayers;
};

struct Lobby_PlayerInfo
{
	int Ready, Play, Online;
};

struct Notif
{
	short Type;
	float Param1, Param2, Param3;
	int Param;
};

struct GamePacket {
	//Notif Notification[8];
	int CurrentFrame;
};

struct FramePacket
{
	float x, y, angle, size;
	bool skip;
	int skip_duration;
};

struct PlayerPacket_128
{
	FramePacket Frame[128];
	short PlayerID;
	int StartFrame;
	//int EndFrame;
};

struct PlayerPacket_64
{
	FramePacket Frame[64];
	short PlayerID;
	int StartFrame;
	//int EndFrame;
};

struct PlayerPacket_32
{
	FramePacket Frame[32];
	short PlayerID; 
	int StartFrame;
	//int EndFrame;
};

struct PlayerPacket_16
{
	FramePacket Frame[16];
	short PlayerID;
	int StartFrame;
	//int EndFrame;
};

struct PlayerPacket_8
{
	FramePacket Frame[8];
	short PlayerID;
	int StartFrame;
	//int EndFrame;
};

struct PlayerPacket_4
{
	FramePacket Frame[4];
	short PlayerID;
	int StartFrame;
	//int EndFrame;
};

struct ServerPacket_128
{
	PlayerPacket_128 Player[8];
	GamePacket Game;
	int StartFrameID, EndFrameID;
};

struct ServerPacket_64
{
	PlayerPacket_64 Player[8];
	GamePacket Game;
	int StartFrameID, EndFrameID;
};

struct ServerPacket_32
{
	PlayerPacket_32 Player[8];
	GamePacket Game;
	int StartFrameID, EndFrameID;
};

struct ServerPacket_16
{
	GamePacket Game;
	PlayerPacket_16 Player[8];
	int StartFrameID, EndFrameID;
};

struct ServerPacket_8
{
	PlayerPacket_8 Player[8];
	GamePacket Game;
	int StartFrameID, EndFrameID;
};

struct ServerPacket_4
{
	PlayerPacket_4 Player[8];
	GamePacket Game;
	int StartFrameID, EndFrameID;
};

class ClientFrameContainer
{
private:
	int ID;
	int Index, LastFrame;
	FramePacket Frame[PACKET_BUFFER];
	void ShiftLeft()
	{
		for (int i = 0; i < Index && i < PACKET_BUFFER; i++)
			Frame[i] = Frame[i + 1];
		Index--;
		//FirstFrame++;
	}
public:
	ClientFrameContainer() : Frame(), LastFrame(0), Index(0) { }

	int GetLastFrame() { return LastFrame; }
	int GetIndex() { return Index; }
	void SetID(int PlayerID) {
		ID = PlayerID;
	}
	int GetID() {
		return ID;
	}

	int AddFrame(float x, float y, float angle, float size, bool skip, int skip_duration)
	{
		while (Index >= PACKET_BUFFER)
		{
			ShiftLeft();
		}
		//	if (Index < PACKET_BUFFER)
		{
			Frame[Index].x = x;
			Frame[Index].y = y;
			Frame[Index].angle = angle;
			Frame[Index].size = size;
			Frame[Index].skip = skip;
			Frame[Index++].skip_duration = skip_duration;
		}
		return ++LastFrame;
	}

	void RemoveOld(int StartFrame)
	{
		if (StartFrame > LastFrame)
			StartFrame = LastFrame;
		int FirstFrame = GetFirstFrame();
		while (StartFrame > FirstFrame)
		{
			ShiftLeft();
			FirstFrame = GetFirstFrame();
		}
	}

	int GetFirstFrame()
	{
		return LastFrame - Index + 1;
	}

	int GetFrameIndex(int Frame)
	{
		return Index - (LastFrame - Frame) - 1;
	}

	int GetFrame(int Index)
	{
		return GetFirstFrame() + Index;
	}

	FramePacket GetFramePacket(int Index)
	{
		return Frame[Index];
	}

	void Print()
	{
		cout << "LF: " << LastFrame;
		cout << " FF: " << GetFirstFrame();
		cout << " FI of 43: " << GetFrameIndex(43) << endl;
		for (int i = 0; i < PACKET_BUFFER; i++)
		{
			cout << "Index: " << i << " Frame: " << GetFrame(i) << " x = " << Frame[i].x << endl;
		}
	}

	void Clear()
	{
		Index = 0;
		LastFrame = 0;
		for (int i = 0; i < PACKET_BUFFER; i++)
			Frame[i].angle = Frame[i].size = Frame[i].x = Frame[i].y = 0;
	}

	PlayerPacket_4 GetPacket_4(int StartFrame)
	{
		PlayerPacket_4 Aux;
		Aux.StartFrame = StartFrame;
		//Aux.EndFrame = StartFrame + 4;
		Aux.PlayerID = ID;
		int ind = GetFrameIndex(StartFrame);
		for (int i = 0; i < 4; i++)
			Aux.Frame[i] = Frame[ind + i];
		return Aux;
	}

	PlayerPacket_8 GetPacket_8(int StartFrame)
	{
		PlayerPacket_8 Aux;
		Aux.StartFrame = StartFrame;
//		Aux.EndFrame = StartFrame + 8;
		Aux.PlayerID = ID;
		int ind = 0;
		ind = GetFrameIndex(StartFrame);
		if (ind < 0)
			return Aux;
		else
		for (int i = 0; i < 8; i++)
			if (ind + i < PACKET_BUFFER)
			Aux.Frame[i] = Frame[ind + i];
		return Aux;
	}

	PlayerPacket_16 GetPacket_16(int StartFrame)
	{
		PlayerPacket_16 Aux;
		Aux.StartFrame = StartFrame;
	//	Aux.EndFrame = StartFrame + 16;
		Aux.PlayerID = ID;
		int ind = GetFrameIndex(StartFrame);
		for (int i = 0; i < 16; i++)
			Aux.Frame[i] = Frame[ind + i];
		return Aux;
	}

	PlayerPacket_32 GetPacket_32(int StartFrame)
	{
		PlayerPacket_32 Aux;
		Aux.StartFrame = StartFrame;
		//Aux.EndFrame = StartFrame + 32;
		Aux.PlayerID = ID;
		int ind = GetFrameIndex(StartFrame);
		for (int i = 0; i < 32; i++)
			Aux.Frame[i] = Frame[ind + i];
		return Aux;
	}

	PlayerPacket_64 GetPacket_64(int StartFrame)
	{
		PlayerPacket_64 Aux;
		Aux.StartFrame = StartFrame;
		//Aux.EndFrame = StartFrame + 64;
		Aux.PlayerID = ID;
		int ind = GetFrameIndex(StartFrame);
		for (int i = 0; i < 64; i++)
			Aux.Frame[i] = Frame[ind + i];
		return Aux;
	}

	PlayerPacket_128 GetPacket_128(int StartFrame)
	{
		PlayerPacket_128 Aux;
		Aux.StartFrame = StartFrame;
		//Aux.EndFrame = StartFrame + 128;
		Aux.PlayerID = ID;
		int ind = GetFrameIndex(StartFrame);
		for (int i = 0; i < 128; i++)
			Aux.Frame[i] = Frame[ind + i];
		return Aux;
	}
	
};

class ServerFrameContainer
{
public:
	ClientFrameContainer Player[8];
};