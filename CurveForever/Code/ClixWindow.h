#pragma once

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>

class ClixWin
{
private:
	
	static HINSTANCE hInst;

public:
	static HWND m_hwnd;
	static bool isActive;
	ClixWin() { }
	static BOOL Create(HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			PCWSTR lpCmdLine,
			int nCmdShow, std::wstring szWindowClass, std::wstring szTitle);
	static HWND GetWindow()  { return m_hwnd; }
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};