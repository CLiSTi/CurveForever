#pragma once

#include "Activity.h"
#include "timer.h"
#include "GamePackets.h"
#include "CLiXApplication.h"
#include "MainMenuActivity.h"
#define CLIX_BUF_SIZE	2000

class LobbyActivity : AppActivity
{
private:
	ChronoTimer NetTimer;
	LobbyInfo Lobby;
	Lobby_PlayerInfo PlayerInfo;
	char	buf[CLIX_BUF_SIZE];
	bool GameStarted;
public:
	LobbyActivity();
	~LobbyActivity();
	void Render();
	void Update();
	void ParsePacket(char *Buffer);
	void Disconnect();
	
};