#include "MainMenuActivity.h"
#include "CLiXApplication.h"
#include "ClixRenderer.h"
#include "CLiXNetwork.h"
#include "CLiXApplication.h"
#include "colors.h"
#include "Activity.h"
#include "LobbyActivity.h"

MainMenuActivity::MainMenuActivity() : AppActivity()
{
	InputingIP = IPInputed = PortInputed = InputingPort = false;
	wsIP = L"127.0.0.1";
	strcpy_s(IPAdress, sizeof(IPAdress), "127.0.0.1");
	PortNo = CLIX_DEFAULT_PORT;
}

MainMenuActivity::~MainMenuActivity()
{
}

void MainMenuActivity::Render()
{
	AppBrush->SetColor(CColor::ColorF(CColor::black));
	AppRender->FillRectangle(D2D1::RectF(0, 0, AppScreenX, AppScreenY), AppBrush);
	AppBrush->SetColor(CColor::ColorF(CColor::red));
	AppRender->FillRectangle(D2D1::RectF(600, 100, AppScreenX - 100, AppScreenY - 100), AppBrush);
	if (InputingIP)
		RenderText(L"Enter IP : " + InputField, 20, 20, 400, 20, 13, CColor::lime);
	if (InputingPort)
		RenderText(L"Enter Port : " + InputField, 20, 20, 400, 20, 13, CColor::lime);
	if (InputingName)
		RenderText(L"Enter Name : " + InputField, 20, 20, 400, 20, 13, CColor::lime);
	//else 
	//	if (IPInputed)
	RenderText(L"Host IP Adress : " + wsIP + L":" + std::to_wstring(PortNo), 20, AppScreenY - 40, 400, 20, 13, CColor::lime);
	RenderText(L"Username : " + Name, 20, AppScreenY - 60, 400, 20, 13, CColor::cyan);
}

#include "GameActivity.h"

void MainMenuActivity::Update()
{
	if (InputingIP)
	{
		if (AppKeyPressed(8))
			if (InputField.length() > 0)
			{
				InputField.pop_back();
				AppReleaseKey(8);
			}

		if (AppKeyPressed(190))
		{
			InputField += (char)46;
			AppReleaseKey(190);
		}
		for (int i = 48; i <= 59; i++)
			if (AppKeyPressed(i))
			{
				InputField += (char)i;
				AppReleaseKey(i);
			}

		for (int i = 96; i <= 105; i++)
			if (AppKeyPressed(i))
			{
				InputField += (char)i - 48;
				AppReleaseKey(i);
			}
		/*for (int i = 65; i <= 90; i++)
			if (AppKeyPressed(i))
			{
				InputField += (char)i;
				AppReleaseKey(i);
			}*/
		if (AppKeyPressed(13))
		{
			strcpy_s(IPAdress, sizeof(IPAdress), "");
			for (int i = 0; i < InputField.length(); i++)
				IPAdress[i] = InputField[i];
			cout << "Entered IP : " << IPAdress << endl;
			wsIP = InputField;
			InputField.clear();
			InputingIP = false;
			//InputingPort = true;
			IPInputed = true;
			AppReleaseKey(13);
			strcpy_s(ClixNet::HostIP, sizeof(IPAdress), IPAdress);
		}

	}
	if (InputingPort)
	{
		if (AppKeyPressed(8))
			if (InputField.length() > 0)
			{
				InputField.pop_back();
				AppReleaseKey(8);
			}

		/*	if (AppKeyPressed(190))
			{
				InputField += (char)46;
				AppReleaseKey(190);
			}*/
		for (int i = 48; i <= 59; i++)
			if (AppKeyPressed(i))
			{
				InputField += (char)i;
				AppReleaseKey(i);
			}

		for (int i = 96; i <= 105; i++)
			if (AppKeyPressed(i))
			{
				InputField += (char)i - 48;
				AppReleaseKey(i);
			}
		/*for (int i = 65; i <= 90; i++)
		if (AppKeyPressed(i))
		{
		InputField += (char)i;
		AppReleaseKey(i);
		}*/
		if (AppKeyPressed(13))
		{
			strcpy_s(Port, sizeof(Port), "");
			for (int i = 0; i < InputField.length(); i++)
				Port[i] = InputField[i];
			PortNo = atoi(Port);
			cout << "Entered Port : " << PortNo << endl;
			wsPort = InputField;
			InputField.clear();
			InputingIP = false;
			InputingPort = false;
			PortInputed = true;
			ClixNet::PortNo = PortNo;
			AppReleaseKey(13);

		}
	}
	if (InputingName)
	{
		if (AppKeyPressed(8))
			if (InputField.length() > 0)
			{
				InputField.pop_back();
				AppReleaseKey(8);
			}

		if (AppKeyPressed(190))
		{
			InputField += (char)46;
			AppReleaseKey(190);
		}
		for (int i = 48; i <= 59; i++)
			if (AppKeyPressed(i))
			{
				InputField += (char)i;
				AppReleaseKey(i);
			}

		for (int i = 96; i <= 105; i++)
			if (AppKeyPressed(i))
			{
				InputField += (char)i - 48;
				AppReleaseKey(i);
			}
		for (int i = 65; i <= 90; i++)
			if (AppKeyPressed(i))
			{
				InputField += (char)i;
				AppReleaseKey(i);
			}
		if (AppKeyPressed(13))
		{
			Name = InputField;
			InputingName = false;
			InputField.clear();
			AppReleaseKey(13);

		}
	}
	else
	{
		if (AppKeyPressed(78))
		{
			InputingName = true;
			AppReleaseKey(78);
		}
		if (AppKeyPressed(73))
		{
			InputingIP = true;
			AppReleaseKey(73);
		}
		if (AppKeyPressed(80))
		{
			InputingPort = true;
			AppReleaseKey(80);
		}
		if (AppKeyPressed(67))
		{

			ClixNet::PortNo = PortNo;
			char ID[10];
			strcpy_s(ClixNet::HostIP, sizeof(IPAdress), IPAdress);
			int err = ClixNet::InitClient();
			AppReleaseKey(67);
			if (err == 1)
			{
				//SerializeT(NetPacket, Name, 120);
				int i;
				strcpy_s(NetPacket, sizeof(NetPacket), "");
				for (i = 0; i < 20; i++)
					NetPacket[i] = Name[i];
				int bytes_sent = send(ClixNet::ClixSocket, NetPacket, 20, 0);
				cout << "Name sent: " << NetPacket << " Bytes sent: " << bytes_sent << endl;
				int f = 0, lf = 0;
				do
				{
					strcpy_s(ID, sizeof(ID), "");
					f = recv(ClixNet::ClixSocket, ID, sizeof(ID), 0);
					if ((int)ID[0] == 48 && (int)ID[1] == 96)
					{
						AppLocalID = (int)ID[4] - 10;
						break;
					}
					if (f > -1)
						lf++;
					cout << "Size of packet: " << f << " :: " << (int)ID[0] << " " << (int)ID[1] << " " << (int)ID[2] << " " << (int)ID[3] << " " << (int)ID[4] << " " << endl;
					Sleep(20);
				} while (lf<10);
				cout << "ID Received: " << AppLocalID << endl;
				
				AppLobby = new LobbyActivity;
				MainApp->SetActivity((AppActivity*)AppLobby);
				delete AppMenu;
			}
		}
		if (AppKeyPressed(83))
		{
			AppReleaseKey(83);

			AppGame = new GameActivity;
			MainApp->SetActivity((AppActivity*)AppGame);
			delete AppMenu;
		}

	}
}

void MainMenuActivity::ParsePacket(char * Buffer)
{
}
