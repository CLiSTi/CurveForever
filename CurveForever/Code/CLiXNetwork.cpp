#include "CLiXNetwork.h"

#include "ClixWindow.h"
#include "CLiXApplication.h"

SOCKET			ClixNet::ClixSocket;
SOCKADDR_IN		ClixNet::from;
WSADATA			ClixNet::w;
int				ClixNet::from_size;
char			ClixNet::Buffer[CLIX_BUFFER_SIZE];
bool			ClixNet::Connected = false;
bool			ClixNet::isServer = false;
bool			ClixNet::isClient = false;
char*			ClixNet::HostIPAdress = nullptr;
int				ClixNet::PortNo = 0;
char			ClixNet::HostIP[20];

int ClixNet::ListenOnPort(int PortNo)
{
	int error = WSAStartup(0x0202, &w);
	if (error)
	{
		return -1;
	}

	if (w.wVersion != 0x0202)
	{
		WSACleanup();
		return -2;
	}
	SOCKADDR_IN sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(PortNo);
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ClixSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClixSocket == INVALID_SOCKET)
	{
		WSACleanup();
		return -3;
	}

	if (bind(ClixSocket, (LPSOCKADDR)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		WSACleanup();
		return -4;
	}
	listen(ClixSocket, 10);

	WSAAsyncSelect(ClixSocket, ClixWin::m_hwnd, CLIX_NETWORK_MESSAGE, FD_READ | FD_CONNECT | FD_CLOSE | FD_ACCEPT);

	cout << "My ip adress is " << inet_ntoa(sockaddr.sin_addr) << " ." << endl;

	isClient = false;
	isServer = true;
	Connected = true;

	return 1;
}

int ClixNet::ConnectToHost(int PortNo, char * IPAdress)
{
	int error = WSAStartup(0x0202, &w);
	if (error)
	{
		return -1;
	}
	if (w.wVersion != 0x0202)
	{
		WSACleanup();
		return -2;
	}

	ClixSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClixSocket == INVALID_SOCKET)
	{
		return -3;
	}
	SOCKADDR_IN target;
	target.sin_family = AF_INET;
	target.sin_port = htons(PortNo);
	target.sin_addr.s_addr = inet_addr(IPAdress);

	if (connect(ClixSocket, (SOCKADDR*)&target, sizeof(target)) == SOCKET_ERROR)
	{
		return -4;
	}

	WSAAsyncSelect(ClixSocket, ClixWin::m_hwnd, CLIX_NETWORK_MESSAGE, FD_READ | FD_CONNECT | FD_CLOSE);
	char buf[80];
	cout << "My ip adress is " << inet_ntoa(target.sin_addr) << " ." << endl;
	isClient = true;
	isServer = false;
	Connected = true;
	return 1;
}

int ClixNet::ParseMessage(UINT message, WPARAM wParam, LPARAM lParam)
{ 
	if (lParam == 658833440)	//un anumit cod de eroare pe care nu l-am inteles inca
	{
		cout << "Connection lost.  " << endl;
		CloseConection();
		AppMyActivity->Disconnect();
	}
	switch (lParam)
	{
	case FD_CLOSE:
		Connected = false;
		cout << "Connection closed (NORMAL WAY)!" << endl;
		CloseConection();
		AppMyActivity->Disconnect();
		break;
	case FD_CONNECT:
		Connected = true;
		cout << "Connection initiated!" << endl;
		break;
	case FD_READ:
		memset(Buffer, 0, sizeof(Buffer));
		recv(ClixSocket, Buffer, sizeof(Buffer) - 1, 0);
		if (ClixApp::myActivity)
			ClixApp::myActivity->ParsePacket(Buffer);
		break;
	}
	return 0;
}

int ClixNet::InitClient()
{
	cout << endl << "Client:" << endl;
	cout << "Connecting to " << HostIP << " using port " << PortNo << " ... ";

	int err = ConnectToHost(CLIX_DEFAULT_PORT, HostIP);
	switch (err)
	{
	case 1: cout << " Succes! " << endl; break;
	case -1:
	case -2:
	case -3:
	case -4: cout << " Error! " << endl; break;

	}
	return err;
}
