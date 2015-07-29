#pragma once

#include "CLiXApplication.h"
#include "ClixWindow.h"

#include "ClixRenderer.h"
#include "PowerUp.h"
#include "colors.h"
#include "Random.h"
#include "timer.h"
#include "FineTuning.h"
#include <d2d1.h>
#include "GamePackets.h"


#define FPI 3.14159265359f
#define FPI2 6.28318530718f
#define FPID10 0.628318530718f

#define PI 3.14f
#define PI2 6.28f
#define PID10 0.62f
#define PID20 0.32f
#define PID100 0.0628f

//class PowerUp
//{
//public:
//	/*int green_speed, red_speed, thin, fat, eraser, team_eraser, walls, team_walls;
//	PowerUp() : green_speed(0), red_speed(0), thin(0), fat(0), eraser(0), team_eraser(0), walls(0), team_walls(0) { }
//	void Reset() { green_speed = red_speed = thin = fat = eraser = team_eraser = walls = team_walls = 0; }*/
//	PowerUpType Type;
//	int Duration;
//};

struct CircleStack
{
	int type = 0, percentage = 0;
};

class OPoint
{
private:
	int id;
	long time_created;
	bool isActive;
	float x, y, size;
public:
	static int counter;
	OPoint() : x(-1), y(-1), size(0), id(-1), isActive(false) { }
	
	bool touch(int ID, float X, float Y, long Time)
	{
		float psize = size;
		if (psize > 1.5)
			psize -= 1;
		if (!isActive)
			return false;
		if ((ID == id) && (Time - time_created < 10))
			return false;
		else
			if ((X	> x - psize) && (Y > y - psize) & (X < x + psize) && (Y < y + psize))
				return true;
	}
	bool Active()
	{
		return isActive;
	}
	int getID() { return id; }
	void reset()
	{
		id = -1;
		if (isActive) counter--;
		isActive = false;
		x = y = size = -1;
	}

	void Set(int ID, float X, float Y, float Size, long Time)
	{
		time_created = Time;
		if (!isActive)
			counter++; 
		isActive = true; 	
		id = ID;
		x = X;
		y = Y;
		size = Size;
	}

};

class Snakey
{
private:
	int internID, Team;
	CColor color;
	//MPoint Trail[trail_size];
	static OPoint ObsPoint[matrixsize][matrixsize];
	CircleStack PUStack[POWERUP_STACK_SIZE];
	int PUStackCount;
	int Frame = 0;
	PowerUp_Atom	Snake_PowerUp[POWERUP_STACK_SIZE];
	int parc, skips, count;
	bool skip, isAlive;
	bool Active;
	ChronoTimer SkipTimer;
	long Ticks;
	long Now;
	int SkipPenalty;
	ID2D1Bitmap* bitmap;
	ID2D1SolidColorBrush* brush;
	ID2D1BitmapRenderTarget* BitmapTarget;

//PowerUps:
	int PU_Speeds, PU_Thins, PU_Slows, PU_FAT, PU_Sharps, PU_Bigturns, PU_OneFinger, PU_Cheese;
	bool PU_Corner, PU_Walls, PU_Reverse, PU_Fly;
	int Key_Left, Key_Right;

public:
	
	PlayerPacket_8 myPacket;
	bool FirstFrame;
	int myPacketFrame = 0;
	float myLastX = 0, myLastY = 0, myLastXX = 0, myLastYY = 0;
	float llx, lly, lx, ly, x, y, speed, angle, size;
	int Frame_LastReceived;
	Snakey();
	~Snakey();
	void AssignKeyz(int Left_Key, int Right_Key) { Key_Left = Left_Key; Key_Right = Right_Key; }
	int LeftKey() { return Key_Left; }
	int RightKey() { return Key_Right; }
	static int IDCount;
	int PUCount;
	void SetActive() { Active = true; }
	void SetInactive() { Active = false; }
	bool GetActiveStatus() { return Active; }
	int GetID() { return internID; }
	void Render();
	void Update();
	CColor GetColor() { return color; }
	void SendPacket_8(PlayerPacket_8 Packet);
	void Init();
	void TurnLeft();
	void TurnRight();
	void ProcessPowerUp(PUType PowerUp);
	void IncSize() { size += 0.5f; }
	void DecSize() { size -= 0.5f; }
	bool HitSomething();
	void SetColor(CColor Color) { color = Color; }
	float getX() { return x; }
	float getY() { return y; }
	void setXY(float X, float Y) { x = X; y = Y; }
	void setAngle(float Angle) { angle = Angle; }
	void kill() { isAlive = false; }
	void makeDot(int time) { skip = true; SkipTimer.StartNow(); SkipPenalty = time; }
	float getAngle() { return angle; }
	float getSpeed() { return speed; }
	float getSize() { return size; }
	void EraseLines();
	void Split();
	void	StackPowerUp(PUType Type);
	void	UpdatePUStack();
};