#define _CRT_SECURE_NO_WARNINGS

#include "ClixWindow.h"
#include "CLiXApplication.h"
#include "CLiXRenderer.h"
#include "CLiXNetwork.h"
#include <Windowsx.h>

HWND ClixWin::m_hwnd = 0;
HINSTANCE ClixWin::hInst = 0;
bool ClixWin::isActive = false;

BOOL ClixWin::Create(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PCWSTR lpCmdLine,
	int nCmdShow, std::wstring szWindowClass, std::wstring szTitle)
{

	ClixRenderer::CreateDIR();

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ClixWin::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass.c_str();
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance; // Store instance handle in our global variable

					   // The parameters to CreateWindow explained:
					   // szWindowClass: the name of the application
					   // szTitle: the text that appears in the title bar
					   // WS_OVERLAPPEDWINDOW: the type of window to create
					   // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
					   // 500, 100: initial size (width, length)
					   // NULL: the parent of this window
					   // NULL: this application does not have a menu bar
					   // hInstance: the first parameter from WinMain
					   // NULL: not used in this application
	m_hwnd = CreateWindow(
		szWindowClass.c_str(),
		szTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		AppScreenX, AppScreenY,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!m_hwnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(m_hwnd,
		nCmdShow);
	UpdateWindow(m_hwnd);
	isActive = true;

	// Main message loop:
	/*MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}*/

	return 0;
}




LRESULT ClixWin::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int akey;
	switch (message)
	{
	case WM_CREATE: {
		
		ClixNet::isServer = false;
		ClixNet::isClient = false;

		break; }
	case WM_MOUSEMOVE:{
		AppMouseLastX = AppMouseX;
		AppMouseLastY = AppMouseY;
		AppMouseX = GET_X_LPARAM(lParam);
		AppMouseY = GET_Y_LPARAM(lParam);
		break; }
	case WM_LBUTTONDOWN: {
		AppMousePressed_Left = true;
		break; }
	case WM_LBUTTONUP: {
		AppMousePressed_Left = false;
		break; }
	case WM_RBUTTONDOWN: {
		AppMousePressed_Right = true;
		break; }
	case WM_RBUTTONUP: {
		AppMousePressed_Right = false;
		break; }
	case WM_KEYDOWN: {
		akey = (int)wParam;
		cout << "PK : " << akey << endl;
		if (!AppNeedKeyUp[akey])
			AppPressKey(akey);
		AppNeedKeyUp[akey] = true;
		break; }
	case WM_KEYUP: {
		akey = (int)wParam;
		AppReleaseKey(akey);
		AppNeedKeyUp[akey] = false;
		break; }
	case WM_PAINT: {
		ClixApp::OnPaint();
		//ValidateRect(m_hwnd, NULL);
		break; }
	case WM_DESTROY: {
		//	PostQuitMessage(0);
		//	MessageBox(NULL, L"Happily Destroyed!", L"Application: ", 0);
		cout << "WM_DESTROY called!" << endl;

		isActive = false;
		break; }
	case WM_MOVE: {
		//InvalidateRect(m_hwnd, NULL, FALSE);
		ClixApp::OnPaint();
		DefWindowProc(hWnd, message, wParam, lParam);
		
		break; }
	case WM_SIZE: {
		AppScreenX = GET_X_LPARAM(lParam);
		AppScreenY = GET_Y_LPARAM(lParam);
		if (ClixRenderer::D2Render != NULL)
		{
			RECT rc;
			GetClientRect(m_hwnd, &rc);
			ClixRenderer::D2Render->Resize(D2D1::SizeU(rc.right, rc.bottom));
			//InvalidateRect(m_hwnd, NULL, FALSE);
		}
		DefWindowProc(hWnd, message, wParam, lParam);
		break; }
	case CLIX_NETWORK_MESSAGE: {
		ClixNet::ParseMessage(message, wParam, lParam);
		break; }
	case WM_CLOSE: {
		isActive = false;
		ClixNet::CloseConection();
		//	MessageBox(NULL, L"Happily Closed!", L"Application: ", 0);
		cout << "WM_CLOSE called!" << endl; 
		//	break;
	}
	default: {
		return DefWindowProc(hWnd, message, wParam, lParam);
		break; }
	}
	return 0;
}

