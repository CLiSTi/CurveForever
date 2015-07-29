#pragma once

class AppActivity
{
protected:
public:
	AppActivity() { }
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual ~AppActivity() { };
	virtual void ParsePacket(char *Buffer) {};
	virtual void Disconnect() { }
	virtual void NetClientSend(char *Buffer) {};
	virtual void NetClientSend() {};
	virtual void NetClientReceive(char *Buffer) {};
	//virtual void SendPackets() = 0;
};