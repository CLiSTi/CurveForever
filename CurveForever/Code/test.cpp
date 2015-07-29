#include <iostream>
using namespace std;

#include<Windows.h>
#include<WinUser.h>
#include<string>

HWND found = 0;

BOOL CALLBACK enumWindowsProc(__in HWND hWnd, __in LPARAM lParam)
{
	/*if (!::IsIconic(hWnd)) {
		return TRUE;
	}*/
	int length = ::GetWindowTextLength(hWnd);
	//if (0 == length) return TRUE;

	TCHAR* buffer;
	buffer = new TCHAR[length + 1];
	memset(buffer, 0, (length + 1) * sizeof(TCHAR));

	GetWindowText(hWnd, buffer, length + 1);
	DWORD dx = GetWindowThreadProcessId(hWnd, NULL);
	//SendMessage(hWnd, 12321, 12321, 12321);
	/*if ((int)hWnd == 27591058)
	{
		wcout << "FOUND!! " << endl;
		found = hWnd;
		SendMessage(hWnd, WM_MBUTTONDOWN, 35, 0);
	}*/

	//if (buffer == "Black Window")
	//{
	//	wcout << "FOUND by name!! " << endl;
	//	//SendMessage(hWnd, WM_CLOSE, 0, 0);
	//}
	//
	//tstring windowTitle = tstring(buffer);
	

	wcout << hWnd << " " << (int)hWnd << " PID: " << dx << " : '" << buffer <<"'" << std::endl;
//	SendMessage(hWnd, WM_KEYDOWN, 32, 0);
	delete[] buffer;
	return TRUE;
}

int main()
{
	cout << "Handle finder!" << endl;

	wcout << TEXT("Enumerating Windows...") << endl;
	BOOL enumeratingWindowsSucceeded = ::EnumWindows(enumWindowsProc, NULL);
	//MSG msg;
	/*while (GetMessage(&msg, found, 0, 0))
	{
		wcout << "Message : " << msg.message << " " << msg.wParam << " " << msg.lParam << endl;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		SendMessage(found, WM_CLOSE, 0, 0);
	}*/
	/*while (1) { 
		cout << "sending...";
		SendMessage(found, 12321, 12321, 12321);
		Sleep(500);
	}*/
	//SendMessage((HWND)27591058, WM_CLOSE, 0, 0);
	cin.get();
	return 0;
}