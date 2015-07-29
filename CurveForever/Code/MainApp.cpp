

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include "CLiXApplication.h"
#include "GameActivity.h"
#include "MainMenuActivity.h"

using namespace std;

#pragma region __Console
//#define CONSOLE

#ifdef CONSOLE
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <Windows.h>
#include <fcntl.h>
#include <iostream>
#endif
void InitConsole()
{
#ifdef CONSOLE
	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;
#endif
}
#pragma endregion __Console

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
#ifdef CONSOLE
	InitConsole();
#endif
	std::cout << "Curve Forever v0.01" << std::endl;
	cout << "Hello!";
	MainApp = new ClixApp(L"Curve Forever");
	MainApp->StartWindow(hInstance, nullptr, pCmdLine, nCmdShow);
	MainApp->Init();
//	AppGame = new GameActivity;
	AppMenu = new MainMenuActivity();
	//myApp->SetActivity((AppActivity*)AppGame);
	MainApp->SetActivity((AppActivity*)AppMenu);
	MainApp->Start();
	MainApp->Close();

	delete MainApp;
#ifdef CONSOLE
	int i = 5;
	cout << "Application closed.. ";
	while (i > 0)
	{
		cout << "The console will shut down in " << i << " seconds. " << endl;
		Sleep(1000);
		i--;
	}
#endif
	//cin >> i;
	//MessageBox(NULL, L"Happily Closed!", L"Application: ", 0);
	return 0;
}