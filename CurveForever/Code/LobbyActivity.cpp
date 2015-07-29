#include "LobbyActivity.h"
#include "ClixRenderer.h"
#include "ClixApplication.h"
#include "CLiXNetwork.h"
#include "GameActivity.h"

#define NETWORK_SLEEP_DURATION	200

LobbyActivity::LobbyActivity()
{
	NetTimer.StartNow();
	PlayerInfo.Ready = false;
	GameStarted = false;

	Lobby.ConnectedPlayers = 0;
	for (int i = 0; i < 8; i++)
	{
		Lobby.Player[i].Connected = false;
		Lobby.Player[i].Ready = false;
	}
	
}

LobbyActivity::~LobbyActivity()
{
}

void LobbyActivity::Render()
{
	AppRender->Clear();
	//AppRender->FillRectangle(D2D1::RectF(600, 200, 400, 400), AppBrush);
	wstring Name;
	CColor Color;
	RenderText(L"Connected players: " + std::to_wstring(Lobby.ConnectedPlayers), 50, 20, 200, 40, 10, CColor::white);
	for (int i = 0; i < 8; i++)
	{
		if (Lobby.Player[i].Connected)
		{
			Color = CColor::red;
			if (Lobby.Player[i].ID == AppLocalID)
				Color = CColor::lime;
			AppBrush->SetColor(CColor::ColorF(Color));
			Name.clear();
			for (int j = 0; j < strlen(Lobby.Player[i].Name); j++)
				Name.push_back(Lobby.Player[i].Name[j]);
			AppRender->FillRectangle(D2D1::RectF(150, i * 60 + 100, 400, i * 60 + 130), AppBrush);
			RenderText(Name + L"	Score : " + to_wstring(Lobby.Player[i].Score.FFA), 150, i * 60 + 110, 300, 20, 10, CColor::white);
			if (!Lobby.Player[i].Ready)
				AppBrush->SetColor(CColor::ColorF(CColor::gray));
			AppRender->FillRectangle(D2D1::RectF(450, i * 60 + 100, 475, i * 60 + 130), AppBrush);
		}
			
	}
}

void LobbyActivity::Update()
{
	int r, type;
	if (AppKeyPressed(82))
	{
		AppReleaseKey(82);
		if (!PlayerInfo.Ready)
			PlayerInfo.Ready = true;
		else
			PlayerInfo.Ready = false;
	}
	if (NetTimer.getMilisecDuration() > NETWORK_SLEEP_DURATION)
	{
		strcpy_s(buf, sizeof(buf), "");
		NetTimer.StartNow();

		/*cout << "Players Connected: " << Lobby.ConnectedPlayers << " : " << endl;
		for (int i = 0; i < Lobby.ConnectedPlayers; i++)
		{
			cout << "Player " << i + 1 << ":	Name: " << Lobby.Player[i].Name;
			cout << " ID: " << Lobby.Player[i].ID;
			cout << " Color: " << Lobby.Player[i].Color;
			cout << " Ready: " << Lobby.Player[i].Ready;
			cout << " Play: " << Lobby.Player[i].Play << endl << endl;
  		}*/
			
		strcpy_s(buf, sizeof(buf), "");
		SerializeT(buf, PlayerInfo, 175);
		r = send(ClixNet::ClixSocket, buf, sizeof(PlayerInfo) + 1, 0);

		if (Lobby.ConnectedPlayers > 0 && PlayerInfo.Ready)
		{
			GameStarted = true;
			for (int i = 0; i < 8; i++)
				if ((!Lobby.Player[i].Ready) && (Lobby.Player[i].Connected))
				GameStarted = false;
		}
	}

	//for (int i = 0; i < 8; i++)
	//{
	//	if (Lobby.Player[i].Ready)
	//		cout << "Player " << i << " is ready ! " << endl;
	//	else
	//		cout << "Player " << i << " is not ready ! " << endl;
	//	if (Lobby.Player[i].Connected)
	//		cout << "Player " << i << " is Connected ! " << endl;
	//	else
	//		cout << "Player " << i << " is not Connected ! " << endl;
	//}

	if (GameStarted)
	{
		AppGame = new GameActivity;
		MainApp->SetActivity((AppActivity*)AppGame);
		AppGame->SetLobbyInfo(Lobby);
		AppGame->SetLocalID(AppLocalID);
		delete AppLobby;
	}
}

void LobbyActivity::ParsePacket(char * Buffer)
{
	int type, r;
	//r = recv(ClixNet::ClixSocket, buf, sizeof(LobbyInfo) + 1, 0);
	DeserializeT(Buffer, &Lobby, &type);
}

void LobbyActivity::Disconnect()
{
	cout << "Disconnecting Lobby ! " << endl;
	AppMenu = new MainMenuActivity;
	MainApp->SetActivity((AppActivity*)AppMenu);
	delete AppLobby;
}