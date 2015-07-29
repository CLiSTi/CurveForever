#pragma once
#include "Activity.h"

#include <iostream>
using namespace std;

class MainMenuActivity : AppActivity
{
	char TextInput[32], IPAdress[20], Port[10], NetPacket[20];
	wstring InputField, wsIP, wsPort, Name;
	int PortNo;
	bool InputingIP, InputingPort, IPInputed, PortInputed, InputingName;
public:
	MainMenuActivity();
	~MainMenuActivity();
	void Render();
	void Update();
	void ParsePacket(char *Buffer);
};