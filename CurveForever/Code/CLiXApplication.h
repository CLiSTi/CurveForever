#pragma once
#include <iostream>
#include <string>
#include "ClixWindow.h"
#include "Activity.h"
#include "timer.h"

class GameActivity;
class MainMenuActivity;
class LobbyActivity;
class ClixApp;

class AppMetrics
{
public:
	static int MouseX, MouseY, MouseLastX, MouseLastY,
		WindowX, WindowY, ScreenX, ScreenY;
	static bool MousePressed_Left, MousePressed_Right, MouseDragged_Left, MouseDragget_Right;
	static bool Keyz[255];
	static bool Release_key[255];
	static bool key_pressed(int key) { return Keyz[key]; }
	static float FPS;
	static ChronoTimer Timer;
	static GameActivity* myGame;
	static AppActivity* myActivity;
	static MainMenuActivity* myMenu;
	static LobbyActivity* myLobby;
	static ClixApp* myMainApp;
	static int LocalID;
};

#define AppLocalID		AppMetrics::LocalID
#define AppGame			AppMetrics::myGame
#define AppMenu			AppMetrics::myMenu
#define AppLobby		AppMetrics::myLobby
#define AppMyActivity	AppMetrics::myActivity
#define MainApp			AppMetrics::myMainApp

#define AppFPS			AppMetrics::FPS
#define AppTimer		AppMetrics::Timer
#define AppMouseX		AppMetrics::MouseX
#define AppMouseY		AppMetrics::MouseY
#define AppMouseLastX	AppMetrics::MouseLastX
#define AppMouseLastY	AppMetrics::MouseLastY
#define AppWindowX		AppMetrics::WindowX
#define AppWindowY		AppMetrics::WindowY
#define AppScreenX		AppMetrics::ScreenX
#define AppScreenY		AppMetrics::ScreenY
#define AppRender		ClixRenderer::D2Render
#define CRender			ClixRenderer
#define AppBrush		ClixRenderer::D2Brush
#define RenderText		ClixRenderer::DText
#define AppMousePressed_Left	AppMetrics::MousePressed_Left
#define AppMousePressed_Right	AppMetrics::MousePressed_Right
#define AppMouseDragged_Left	AppMetrics::MouseDragged_Left
#define AppMouseDragged_Right	AppMetrics::MouseDragged_Right
#define AppPressKey(key)		AppMetrics::Keyz[key] = true
#define AppReleaseKey(key)		AppMetrics::Keyz[key] = false
#define AppKeyPressed(key)		AppMetrics::Keyz[key]
#define AppNeedKeyUp			AppMetrics::Release_key

#define ftw std::to_wstring

class ClixApp
{
	static std::wstring myName;
	static bool isRunning;
	static ClixWin* myWindow;
	static int FrameCount;
public:
	static AppActivity* myActivity;
	ClixApp(std::wstring Name);
	ClixApp();
	~ClixApp();
	static int Init();
	static int Start();
	static int Stop();
	static int Close();
	static void StartWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow);
	static void OnPaint();
	static AppActivity* GetActivity() { return myActivity; }
	static void SetActivity(AppActivity* Activity) { AppMetrics::myActivity = myActivity = Activity; }
	static void DeleteActualActivity() { if (myActivity) { delete myActivity; myActivity = nullptr; } }
};