#include "GameActivity.h"
#include "ClixRenderer.h"
#include "CLiXApplication.h"
#include "CLiXNetwork.h"
#include "GamePackets.h"
#include "timer.h"
#include <iostream>
using namespace std;


#define PCODE_PACKET4			101
#define PCODE_PACKET8			102
#define PCODE_PACKET16			103
#define PCODE_PACKET32			104
#define PCODE_PACKET64			105
#define PCODE_PACKET128			106
#define PCODE_RESET				107

#define PCODE_SERVERPACKET4		111
#define PCODE_SERVERPACKET8		112

PowerUp_Item GameActivity::PU_Items[256];
char Buffer[2500];
ChronoTimer PachetTimer;

GameActivity::GameActivity() : AppActivity()
{
	newFrames = 0;
	Pause = true;
	TotalPowerups = 0;

	for (int i = 0; i < SnakesNr; i++)
		Snake[i] = new Snakey();

	/*for (int i = 0; i < SnakesNr; i++)
		Snake[i]->SetActive();*/


	for (int i = 0; i < 8; i++)
	{
		Player[i].ID = -1;
		Player[i].LeftKey = -1;
		Player[i].RightKey = -1;
		Player[i].Local = false;
		Player[i].Active = false;
	}

	Snake[0]->SetColor(CColor::white);
	Snake[1]->SetColor(CColor::pink);
	Snake[2]->SetColor(CColor::lime);
	Snake[3]->SetColor(CColor::green);
	Snake[4]->SetColor(CColor::red);
	Snake[5]->SetColor(CColor::yellow);
	Snake[6]->SetColor(CColor::orange);
	Snake[7]->SetColor(CColor::cyan);
	ActivePlayers = 0;
	PachetTimer.StartNow();

	PacketCount_receive = PacketCount_send = 0;
	//char FileName[30];
	//strcpy_s(FileName, sizeof(FileName), "client_send_0.log");
	//FileName[12] = (char)(48 + AppLocalID);
	//FSend.open(FileName);
	//strcpy_s(FileName, sizeof(FileName), "client_receive_0.log");
	//FileName[15] = (char)(48 + AppLocalID);
	//FRec.open(FileName);

	//for (int i = 0; i < 8; i++)
	//{
	//	Snake[i]->Validated.Clear();
	//	Snake[i]->Invalidated.Clear();
	//}
	
}

GameActivity::~GameActivity()
{
	for (int i = 0; i < SnakesNr; i++)
		delete Snake[i];
	//FRec.close();
	//FSend.close();
}

void GameActivity::Render()
{

	ClixRenderer::ClearScreen();
	ClixRenderer::D2Brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	ClixRenderer::DText(L"Screen Size: " + std::to_wstring(AppScreenX) + L"x" + std::to_wstring(AppScreenY), 20, 20, 250, 20, 15, CColor::yellow);
	ClixRenderer::DText(L"Mouse Pos: " + std::to_wstring(AppMouseX) + L"x" + std::to_wstring(AppMouseY), 20, 40, 250, 20, 15, CColor::white);


	wstring pkeyz = L"Pressed keyz: ";
	for (int i = 0; i < 255; i++)
	if (AppMetrics::Keyz[i])
		pkeyz += to_wstring(i) + L" ";

	ClixRenderer::DText(pkeyz, 20, 60, 350, 20, 15, 0, 0.5f, 1.f, 1);
	//ClixRenderer::D2Render->DrawTextW(L"aici!", 5, ClixRenderer::pTextFormat, D2D1::RectF(AppMouseX - 25, AppMouseY - 10, AppMouseX + 25, AppMouseY + 10), ClixRenderer::D2Brush);
	ClixRenderer::D2Render->DrawRectangle(D2D1::RectF(AppMouseX - 5, AppMouseY - 5, AppMouseX + 5, AppMouseY + 5), ClixRenderer::D2Brush);

	RenderText(L"FrameRate: " + ftw(AppFPS), 20, 620, 100, 20, 15, CColor::yellow);
//	RenderText(L"Frame: " + ftw(Snake[AppLocalID]->Invalidated.GetLastFrame()), 0, 640, 100, 20, 15, CColor::yellow);
	RenderText(L"Snakey x = " + ftw(Snake[0]->getX()) + L" y = " + ftw(Snake[0]->getY()) + L" angle = " + ftw(Snake[0]->getAngle()) + L" speed = " + ftw(Snake[0]->getSpeed()), 20, 660, 500, 28, 10, CColor::orange);
	RenderText(L"Time since start: " + ftw(AppTimer.getMilisecDuration()), 20, 80, 200, 20, 12, CColor::yellow);
	RenderText(L"Active Obstacles: " + ftw(OPoint::counter), 20, 100, 200, 20, 12, CColor::gray);
	RenderText(L"Player 0 Active Powerups: " + ftw(Snake[0]->PUCount), 20, 120, 200, 20, 12, CColor::lime);
	RenderText(L"Map Powerups: " + ftw(TotalPowerups), 20, 140, 200, 20, 12, CColor::lime);

//	if (ClixNet::isClient)	RenderText(L"CLIENT", 730, 5, 200, 50, 32, CColor::yellow);
//	if (ClixNet::isServer)	RenderText(L"SERVER", 730, 5, 200, 50, 32, CColor::yellow);

	//Snake[0]->Render();
	//Snake[1]->Render();
	for (int i = ActivePlayers - 1; i >= 0; i--)
		if (Snake[i]->GetActiveStatus())
		{
			Snake[i]->Render();
		//	RenderText(L"Player " + ftw(i) + L" size: " + ftw(Snake[i]->size), 900, 20 + i * 20, 100, 20, 12, Snake[i]->GetColor());
		}
	
	wstring Name;
	for (int i = 0; i < 8; i++)
	{
		Name.clear();
		for (int j = 0; j < strlen(Lobby.Player[i].Name); j++)
			Name.push_back(Lobby.Player[i].Name[j]);
		RenderText(Name, 20, 300 + 20 * i, 200, 50, 13, Snake[i]->GetColor());
	//	RenderText(L"x = " + ftw(Snake[AppLocalID]->Backup_DeSters.Frame[i].x) + L" y = " + ftw(Snake[AppLocalID]->Backup_DeSters.Frame[i].y) + L" size = "+ ftw(Snake[AppLocalID]->Backup_DeSters.Frame[i].size), 900, 20 + i * 20, 200, 20, 10, Snake[AppLocalID]->GetColor());
	}

	for (int i = 0; i < PU_MAX_NR; i++)
	{
		if (PU_Items[i].Active())
		{
			PU_Items[i].Render();
		}
	}
//	Snake[0]->Render();
	AppBrush->SetColor(CColor::ColorF(CColor::white));
	AppRender->DrawRectangle(D2D1::RectF(table_x, table_y, table_x + tablesize, table_y + tablesize), AppBrush, 2.f);
	
	/*AppBrush->SetColor(CColor::ColorF(CColor::lightGray));
	for (int i = 0; i < matrixsize + 10; i++)
		for (int j = 0; j < matrixsize + 10; j++)
		if (Snakey::Matrix[i][j] > 0)
			AppRender->DrawRectangle(D2D1::RectF(table_x + 400 + i, table_y + j, table_x + 400 + i + 0.5f, table_y + j + 0.5f), AppBrush);*/
}

void GameActivity::PowerUp_SendToEnemies(int ID, PUType PowerUp)
{
	for (int i = 0; i < ActivePlayers; i++)
		if (Snake[i]->GetID() != ID)
			Snake[i]->StackPowerUp(PowerUp);
}

void GameActivity::PowerUp_SendToAllies(int ID, PUType PowerUp)
{
}

void GameActivity::PowerUp_SendToAll(PUType PowerUp)
{
	for (int i = 0; i < ActivePlayers; i++)
			Snake[i]->StackPowerUp(PowerUp);
}

void GameActivity::SetLocalID(int LocalID, int LeftKey, int RightKey)
{
	AppLocalID = LocalID;
	Player[LocalID].Local = true;
	Player[LocalID].LeftKey = LeftKey;
	Player[LocalID].RightKey = RightKey;

	Snake[LocalID]->AssignKeyz(LeftKey, RightKey);

	//FSend << "Local ID: " << AppLocalID << " ID: " << Snake[LocalID]->GetID() << endl;
}

void GameActivity::Update()
{
	newFrames++;
	if (AppKeyPressed(67) || AppKeyPressed(99))
		if (!ClixNet::Connected)
		{
			if (ClixNet::isClient) ClixNet::InitClient();
		}

	for (int i = 0; i < 8; i++)
	{
		if (AppKeyPressed(Snake[i]->LeftKey()))
			Snake[i]->TurnLeft();
		if (AppKeyPressed(Snake[i]->RightKey()))
			Snake[i]->TurnRight();
	}

	if (AppKeyPressed(38))
	{
		Snake[0]->IncSize();
		AppReleaseKey(38);
	}
	if (AppKeyPressed(40))
	{
		Snake[0]->DecSize();
		AppReleaseKey(40);
	}

	if (AppKeyPressed(83))
	{
		Snake[1]->IncSize();
		AppReleaseKey(83);
	}
	if (AppKeyPressed(87))
	{
		Snake[1]->DecSize();
		AppReleaseKey(87);
	}

	if (AppKeyPressed(68) || AppKeyPressed(100))
	{
		cout << "Closing connection ... ";
		ClixNet::CloseConection();
		AppReleaseKey(100);
		AppReleaseKey(68);
	}

	if (AppKeyPressed(73))
	{
		AppReleaseKey(73);
		/*for (int i = 0; i < ActivePlayers; i++)
			if (Snake[i])
				Snake[i]->Init();*/
		for (int i = 0; i < PU_MAX_NR; i++)
			PU_Items[i].SetInactive();
		//if (ClixNet::Connected)
		{
			char buf[32];
		
			buf[0] = PCODE_RESET;
			buf[1] = PCODE_RESET;
			buf[2] = PCODE_RESET;
			buf[3] = PCODE_RESET;
			send(ClixNet::ClixSocket, buf, 4, 0);
		}
			
	}
	

	if (AppKeyPressed(69))
		Snake[0]->StackPowerUp(PU_GREEN_ERASER);

	if (AppKeyPressed(49))
	{
		Snake[0]->StackPowerUp(PU_GREEN_SPEED);
		AppReleaseKey(49);
	}
	if (AppKeyPressed(50))
	{
		Snake[0]->StackPowerUp(PU_RED_REVERSE);
		AppReleaseKey(50);
	}
	if (AppKeyPressed(51))
	{
		Snake[0]->StackPowerUp(PU_GREEN_CORNER);
		AppReleaseKey(51);
	}

	for (int i = 0; i < ActivePlayers; i++)
		if (Snake[i]->GetActiveStatus())
			Snake[i]->Update();

	if (RandomInt(0, 10) == 1)
	{
		//int tp = ;
		switch (RandomInt(0, 4))
		{
		case 1: 
			//AddPowerUpItem(RandomDouble(0, tablesize), RandomDouble(0, tablesize), (PUType)RandomInt(0, 8));
			//AddPowerUpItem(RandomDouble(0, tablesize), RandomDouble(0, tablesize), PU_GREEN_THIN);
			break;
		case 2:
		//	AddPowerUpItem(RandomDouble(0, tablesize), RandomDouble(0, tablesize), PU_RED_SPEED);
			//AddPowerUpItem(RandomDouble(0, tablesize), RandomDouble(0, tablesize), (PUType)RandomInt(TOTAL_PU_GREEN + 1, TOTAL_PU_GREEN + 8));
			break;
		case 3:
		//	AddPowerUpItem(RandomDouble(0, tablesize), RandomDouble(0, tablesize), PU_CYAN_ERASER);
			//AddPowerUpItem(RandomDouble(0, tablesize), RandomDouble(0, tablesize), (PUType)RandomInt(TOTAL_PU_GREEN + TOTAL_PU_RED+ 1, TOTAL_PU_GREEN + TOTAL_PU_RED + 8));
			break;
		default:
			//AddPowerUpItem(RandomDouble(0, tablesize), RandomDouble(0, tablesize), PU_CYAN_WALLS);
			break;
		}
	}
}

void GameActivity::SendInternalCommand(int code)
{

}

#define CFM_INIT_PLAYER		1
#define CFM_INIT_PLAYERS	2
#define CFM_PLAYER_XYA		10
#define CFM_SPAWN_PU		15
#define CFM_PLAYER_LEFT		20
#define CFM_PLAYER_RIGHT	21


void GameActivity::ParsePacket(char * Buffer)
{
	NetClientReceive(Buffer);
}

void GameActivity::AddPowerUpItem(float x, float y, PUType Type)
{
	for (int i = 0; i < PU_MAX_NR; i++)
	{
		if (!PU_Items[i].Active())
		{
			PU_Items[i].Set(x, y, Type);
			TotalPowerups++;
			break;
		}
	}
}

void GameActivity::SendPowerUp(PUType PowerUp, int ID)
{
	switch (PowerUp)
	{
		case	PU_GREEN_SLOW:break;
		case	PU_GREEN_SPEED:break;
		case	PU_GREEN_THIN:break;
		case	PU_GREEN_CORNER:break;
		case	PU_GREEN_FLY:break;
		case	PU_GREEN_ERASER: break;
		case	PU_GREEN_WALLS:	PowerUp_SendToAllies(ID, PowerUp); break;
		case	PU_GREEN_SPLIT:break;
		case	PU_GREEN_SHARPCORNERS:	break;

		case	PU_RED_SLOW:	
		case	PU_RED_SPEED:
		case	PU_RED_FAT:
		case	PU_RED_CORNER:
		case	PU_RED_REVERSE:
		case	PU_RED_CHEESE:
		case	PU_RED_ONEFINGER:
		case	PU_RED_BIGTURNS:	PowerUp_SendToEnemies(ID, PowerUp); break;

		case	PU_CYAN_ERASER:		PowerUp_SendToAll(PU_CYAN_ERASER); break;
		case	PU_CYAN_CLOSINGWALLS:
		case	PU_CYAN_WALLS:		PowerUp_SendToAll(PU_CYAN_WALLS); break;
		case	PU_CYAN_BUBBLES:
		case	PU_CYAN_SWITCH:
		case	PU_CYAN_MOVINGPOWERUPS:
		case	PU_CYAN_NODRAWAREA:
		case	PU_CYAN_DARKNESS:
			break;
	}
}

void GameActivity::SendPackets()
{

}

void GameActivity::NetClientSend(char * Buffer)
{
	send(ClixNet::ClixSocket, Buffer, (int)sizeof(Buffer), 0);
}

void GameActivity::NetClientSend()
{
	//if (newFrames % 4 == 0 && newFrames > 0)
	//{
	//	newFrames = 0;
	//	for (int i = 0; i < 8; i++)
	//		Snake[i]->myPacketFrame = 0;
	//	strcpy_s(Buffer, sizeof(Buffer), "");	
	//	PacketCount_send++;
	////	FSend << "Sent for player: " << AppLocalID << " local ID : " << Snake[AppLocalID]->myPacket_4.PlayerID << endl;
	//	SerializeT(Buffer, Snake[AppLocalID]->myPacket_4, PCODE_PACKET4);
	////	for (int i = 0; i < 4; i++)
	//	//	FSend << "Frame: " << i << " x: " << Snake[AppLocalID]->myPacket_4.Frame[i].x << " y: " << Snake[AppLocalID]->myPacket_4.Frame[i].y << endl;
	//	send(ClixNet::ClixSocket, Buffer, sizeof(PlayerPacket_4) + 1, 0);
	//	PachetTimer.StartNow();
	//}

	if (newFrames % 8 == 0 && newFrames > 0)
	{
		newFrames = 0;
		for (int i = 0; i < 8; i++)
			Snake[i]->myPacketFrame = 0;
		strcpy_s(Buffer, sizeof(Buffer), "");
		PacketCount_send++;
		//	FSend << "Sent for player: " << AppLocalID << " local ID : " << Snake[AppLocalID]->myPacket.PlayerID << endl;
		SerializeT(Buffer, Snake[AppLocalID]->myPacket, PCODE_PACKET8);
		//	for (int i = 0; i < 4; i++)
		//	FSend << "Frame: " << i << " x: " << Snake[AppLocalID]->myPacket.Frame[i].x << " y: " << Snake[AppLocalID]->myPacket.Frame[i].y << endl;
		send(ClixNet::ClixSocket, Buffer, sizeof(PlayerPacket_8) + 1, 0);
		PachetTimer.StartNow();
	}
}


void GameActivity::NetClientReceive(char * Buffer)
{
	//MessageBox(NULL, L"NetClientReceive", L"TAYY", 1);
	switch ((int)Buffer[0])
	{
	//case PCODE_SERVERPACKET4:
	//	//FRec << "\nReceived Packet_4 (yay) ! \n";
	//	ServerPacket_4 Packet_4;
	//	DeserializeT(Buffer, &Packet_4);
	//	for (int i = 0; i < 8; i++)
	//	{
	////		FRec << "\nReceived for player " << i << ": " << endl;
	//	//	for (int j = 0; j < 4; j++)
	//	//		FRec << "Frame: " << j << " x: " << Packet.Player[i].Frame[j].x << " y: " << Packet.Player[i].Frame[j].y << "\n";
	//		Snake[i]->SendPacket_4(Packet_4.Player[i]);
	//	}
	//		
	//	break;
	case PCODE_SERVERPACKET8:
		ServerPacket_8 Packet_8;
		DeserializeT(Buffer, &Packet_8);
		for (int i = 0; i < 8; i++)
		{		
			Snake[i]->SendPacket_8(Packet_8.Player[i]);
		}
		break;
	case PCODE_RESET:
		for (int i = 0; i < ActivePlayers; i++)
			if (Snake[i])
			Snake[i]->Init();
		for (int i = 0; i < PU_MAX_NR; i++)
			PU_Items[i].SetInactive();
		newFrames = 0;
		//MessageBox(NULL, L"Reset", L"BAAAH", 0);
		break;
	case 160:
		GameStartInfo SInfo;
		DeserializeT(Buffer, &SInfo);
		for (int i = 0; i < 8; i++)
		{
			Snake[i]->x = SInfo.PlayerInfo[i].x;
			Snake[i]->y = SInfo.PlayerInfo[i].y;
			Snake[i]->angle = SInfo.PlayerInfo[i].angle;
			cout << "Info Received: " << SInfo.PlayerInfo[i].x << " - " << SInfo.PlayerInfo[i].y << " - " << SInfo.PlayerInfo[i].angle << " - " << endl;
		}
		
		break;
	default:
		//cout << "Dropped pachet code " << (int)Buffer[0] << endl;
		break;
	}
}

