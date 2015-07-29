#pragma once

#include "Activity.h"
#include "Snake.h"
#include "GamePackets.h"
#include "FineTuning.h"
#include "PowerUp.h"

#include <fstream>

#define SnakesNr 8
#define PU_MAX_NR 256

#define GamePowerUp	GameActivity::PU_Items



#define FRAME_LABEL_SIZE 128

//class FrameLabelContainer
//{
//private:
//	FrameLabel frame[FRAME_LABEL_SIZE];
//public:
//	int IDCount, FramesCount;
//	int Oldest, Newest, Iterator;
//	FrameLabelContainer();
//
//	void		AddFrame(float LLX, float LLY, float LX, float LY, float X, float Y, float Angle, float Size);
//	void		ShiftElements();
//	int			GetLabelID() {
//		return IDCount;
//	}
//
//	FrameLabel* GetFrameWithID(int ID)
//	{
//		for (int i = 0; i < FramesCount; i++)
//			if (frame[i].id == ID)
//				return &frame[i];
//	}
//	FrameLabel* GetFrameWithID(int ID, int &Index)
//	{
//		for (int i = 0; i < FramesCount; i++)
//			if (frame[i].id == ID)
//			{
//				Index = i;
//				return &frame[i];
//			}
//		return nullptr;
//	}
//	FrameLabel* GetFrame(int Index)
//	{
//		if (Index < FRAME_LABEL_SIZE)
//		return &frame[Index];
//		else return nullptr;
//	}
//
//	void		Iterator_Start() {
//		Iterator = 0;
//	}
//	FrameLabel* Iterator_Element() {
//		if (Iterator < FRAME_LABEL_SIZE)
//		return &frame[Iterator];
//		else return nullptr;
//	}
//	void		Iterator_Next() {
//		Iterator++;
//	}
//	bool		Iterator_Finish() {
//		if (Oldest + Iterator >= Newest)
//			return true;
//		return false;
//	}
//	bool		Iterator_NotFinished() {
//			if (Oldest + Iterator >= Newest)
//				return false;
//			return true;
//		}
//
//	void		Reset();
//};

struct PlayerConfig
{
	int ID;
	bool Local, Active;
	int LeftKey, RightKey;
};

class GameActivity : AppActivity
{
private:
	bool Pause;
	PlayerConfig Player[8];
	Snakey* Snake[SnakesNr];
	int ActivePlayers;
	LobbyInfo Lobby;
	//ofstream FRec, FSend;
	int PacketCount_receive, PacketCount_send, newFrames;
public:
	static PowerUp_Item PU_Items[256];
	int TotalPowerups;
	GameActivity();
	~GameActivity();
	void SetLobbyInfo(LobbyInfo Lobby_Info) 
	{ 
		Lobby = Lobby_Info; 
		ActivePlayers = 0;
		for (int i = 0; i < 8; i++)
		{
			Player[i].ID = i;
			Player[i].Active = Lobby.Player[i].Connected;
			Player[i].Local = false;

			if (Player[i].Active)
			{
				Snake[i]->SetActive();
				ActivePlayers++;
			}
				
		}
	}
	void SetLocalID(int LocalID, int LeftKey = 37, int RightKey = 39);
	void Update();
	void Render();
	void PowerUp_SendToEnemies(int ID, PUType PowerUp);
	void PowerUp_SendToAllies(int ID, PUType PowerUp);
	void PowerUp_SendToAll(PUType PowerUp);
	void SendInternalCommand(int Code);
	void ParsePacket(char *Buffer);
	void AddPowerUpItem(float x, float y, PUType Type);
	void SendPowerUp(PUType PowerUp, int ID);
	void SendPackets();

	void NetClientSend(char *Buffer);
	void NetClientSend();
	void NetClientReceive(char *Buffer);
	/*void NetServerSend(char *Buffer);
	void NetServerReceive(char *Buffer);*/
};