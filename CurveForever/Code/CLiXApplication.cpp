#include "CLiXApplication.h"
#include "CLiXRenderer.h"
#include "ClixWindow.h"
#include "CLiXNetwork.h"
using namespace std;

std::wstring ClixApp::myName;
bool ClixApp::isRunning = false;
ClixWin* ClixApp::myWindow = 0;
AppActivity* ClixApp::myActivity = 0;
int ClixApp::FrameCount = 0;

ChronoTimer AppMetrics::Timer;
float AppMetrics::FPS = 0.f;
int AppMetrics::MouseX = 0;
int AppMetrics::MouseY = 0;
int AppMetrics::LocalID = -1;
int AppMetrics::MouseLastX = 0;
int AppMetrics::MouseLastY = 0;
int AppMetrics::WindowX = 0;
int AppMetrics::WindowY = 0;
int AppMetrics::ScreenX = 900 + 16;
int AppMetrics::ScreenY = 700 + 38;
bool AppMetrics::MousePressed_Left = false;
bool AppMetrics::MousePressed_Right = false;
bool AppMetrics::MouseDragged_Left = false;
bool AppMetrics::MouseDragget_Right = false;;
bool AppMetrics::Keyz[255]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
						   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
bool AppMetrics::Release_key[255];
GameActivity* AppMetrics::myGame = nullptr;
LobbyActivity* AppMetrics::myLobby = nullptr;
MainMenuActivity* AppMetrics::myMenu = nullptr;
AppActivity* AppMetrics::myActivity = nullptr;
ClixApp* AppMetrics::myMainApp = nullptr;

ClixApp::ClixApp(std::wstring Name)
{
	isRunning = false;
	myName = Name;
	myWindow = nullptr;
}

ClixApp::ClixApp()
{
	myWindow = nullptr;
	isRunning = false;
	myName = L"CLiX";
}

ClixApp::~ClixApp()
{
	if (myWindow)
		delete myWindow;
}

int ClixApp::Init()
{
	int p;

	return 0;
}

int ClixApp::Start()
{
	isRunning = true;
	AppTimer.StartNow();
	ChronoTimer FPSTimer, FrameUpdateTimer;
	FPSTimer.StartNow();
	MSG msg;
	int p = 0;
	while (ClixWin::isActive)
	{
		OnPaint();
		if (FPSTimer.getMilisecDuration() >= 1000)
		{
			AppFPS = FrameCount;
			FrameCount = 0;
			FPSTimer.StartNow();
		//	char buff[80];
			//strcpy_s(buff, "CFjB185grkj489293rjfj");
		//	send(ClixNet::ClixSocket, buff, strlen(buff), 0);
		}
		Sleep(1);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			
		//	if (msg.message == WM_KEYDOWN)
			//std::cout << "message! " << p++ << std::endl;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (AppKeyPressed(27))
		{ 
			SendMessage(ClixWin::m_hwnd, WM_CLOSE, 0, 0);
			PostQuitMessage(0);
		}
		if (FrameUpdateTimer.getMilisecDuration() >= 17)
		{
			FrameUpdateTimer.StartNow();
		
		}
	
	}
	
	return 0;
}

int ClixApp::Stop()
{
	isRunning = false;
	return 0;
}

int ClixApp::Close()
{
	ClixNet::CloseConection();
	return 0;
}

void ClixApp::StartWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
	myWindow = new ClixWin();
	myWindow->Create(hInstance, hPrevInstance, lpCmdLine, nCmdShow, L"CLiX Application", myName);
}

//int ateatp = 0;

void ClixApp::OnPaint()
{
	//if (ClixRenderer::D2Factory)
	
	{
		//std::cout << "Paintin' : " << std::endl;
		PAINTSTRUCT ps;
		BeginPaint(ClixWin::m_hwnd, &ps);
		if (SUCCEEDED(ClixRenderer::StartDraw()))
		{
			if (myActivity)
			{
				FrameCount += 1.0f;
			//	if (ClixNet::Connected)
				{
					myActivity->Render();
					myActivity->Update();
					myActivity->NetClientSend();

				}		
			}
			ClixRenderer::EndDraw();
		}
		EndPaint(ClixWin::m_hwnd, &ps);

	}
	
}
