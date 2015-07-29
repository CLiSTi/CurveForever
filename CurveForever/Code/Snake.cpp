#include "Snake.h"
#include "GameActivity.h"
#include <math.h>
#include "CLiXNetwork.h"
//#include <wrl.h>
using namespace std;
//using namespace Microsoft::WRL;

//int Snakey::Matrix[matrixsize + 10][matrixsize + 10];
int Snakey::IDCount = 0;
OPoint Snakey::ObsPoint[matrixsize][matrixsize];

int OPoint::counter = 0;
char buff[80];

Snakey::Snakey() : color(CColor::white), x(RandomInt(200, 400)), y(RandomInt(200, 400)), speed(1.f), angle(RandomDouble(0, 6.f)), size(3.0f)
{
	Team = 0;
	parc = count = 0;
	lx = llx = x;
	ly = lly = y;
	bitmap = nullptr;
	BitmapTarget = nullptr;
	isAlive = true;
	internID = IDCount++;
	myPacket.PlayerID = internID;
	Ticks = 0;
	PUCount = 0;
	FirstFrame = true;
	myLastX = myLastXX = myLastY = myLastYY = -1000;
}

Snakey::~Snakey()
{
	BitmapTarget->Release();
	bitmap->Release();
}

void Snakey::Init()
{
	myLastX = myLastXX = myLastY = myLastYY = -100000;
	for (int i = 0; i < 8; i++)
		myPacket.Frame[i].x =
		myPacket.Frame[i].y =
		myPacket.Frame[i].angle =
		myPacket.Frame[i].size = -10000;

	for (int i = 0; i < matrixsize; i++)
		for (int j = 0; j < matrixsize; j++)
		if (ObsPoint[i][j].getID() == internID)
		ObsPoint[i][j].reset();
	isAlive = true;
	if (BitmapTarget)
	{
		BitmapTarget->Release();
		BitmapTarget = nullptr;
	}

	Ticks = 0;
	for (int i = 0; i < POWERUP_STACK_SIZE; i++)
		Snake_PowerUp[i].Reset();
}

bool pfac = false;

void Snakey::Update()
{
	pfac = false;
	Now = AppTimer.getMilisecDuration();
	UpdatePUStack();

	if (isAlive)
	{
		Ticks++;
		lx = llx;
		ly = lly;
		llx = x;
		lly = y;
		for (int i = 0; i <= PU_Speeds; i++)
		{
			x += ((frame_dist + speed_modifier) / (PU_Slows + 1)) * cos(angle);
			y += ((frame_dist + speed_modifier) / (PU_Slows + 1)) * sin(angle);
			if (x < 0)
			{
				x = tablesize;
				lx = llx = x;
				if (!PU_Walls)
					isAlive = false;
				pfac = true;

			}
			if (x > tablesize)
			{
				x = 0;
				lx = llx = x;
				if (!PU_Walls)
					isAlive = false;
				pfac = true;
			}
			if (y < 0)
			{
				y = tablesize;
				ly = lly = y;
				if (!PU_Walls)
					isAlive = false;
				pfac = true;
			}
			if (y > tablesize)
			{
				y = 0;
				ly = lly = y;
				if (!PU_Walls)
					isAlive = false;
				pfac = true;
			}
			int dur = SkipPenalty - SkipTimer.getMilisecDuration();
			if (dur < 0) dur = 0;
			if (myPacketFrame < 8)
			{
				myPacket.Frame[myPacketFrame].x = x;
				myPacket.Frame[myPacketFrame].y = y;
				myPacket.Frame[myPacketFrame].angle = angle;
				myPacket.Frame[myPacketFrame].size = size;
				myPacket.Frame[myPacketFrame].skip = skip;
				myPacket.Frame[myPacketFrame].skip_duration = dur;
			}

			myPacketFrame++;
		
			if (!skip)
				if (HitSomething())
				{
					if (internID == AppLocalID)
						isAlive = false;
				}

			if (isAlive)
			{
				if (skip)
				{
					if (SkipTimer.getMilisecDuration() > SkipPenalty)
					{
						skip = false;
						lx = llx = x;
						ly = lly = y;
					}

				}
				else
				{
					if (RandomInt(0, 100) == 1)
						//	if (ID == 1)
					{
						skip = true;
						SkipTimer.StartNow();
						SkipPenalty = RandomInt(160, 220) / (PU_Speeds + 1) * (PU_Slows + 1);
					}
				}
			}
			for (int i = 0; i < PU_MAX_NR; i++)
				if (GamePowerUp[i].Active())
				if (GamePowerUp[i].Touch(x, y))
				ProcessPowerUp(GamePowerUp[i].Grab());
		}
	}
}

float ModulDe(float x)
{
	if (x < 0)
		return -x;
	return x;
}


void Snakey::SendPacket_8(PlayerPacket_8 Packet)
{
	if (BitmapTarget && isAlive)
	{
		BitmapTarget->BeginDraw();
		brush->SetColor(CColor::ColorF(color));

		float SafeX, SafeY;

		for (int i = 0; i < 8; i++)
			//if (internID != AppLocalID)
			if (!Packet.Frame[i].skip && Packet.Frame[i].x >= 0 && Packet.Frame[i].y >= 0)
				ObsPoint[(int)Packet.Frame[i].x][(int)Packet.Frame[i].y].Set(internID, Packet.Frame[i].x, y, Packet.Frame[i].size, Ticks);

		if (!FirstFrame)
		{
			if (!Packet.Frame[0].skip)
			{
				SafeX = myLastXX;
				SafeY = myLastYY;
				if (ModulDe(SafeX - Packet.Frame[0].x) > 400 || ModulDe(SafeY - Packet.Frame[0].y) > 400)
				{
					SafeX = Packet.Frame[0].x;
					SafeY = Packet.Frame[0].y;
				}
				BitmapTarget->DrawLine(D2D1::Point2F(SafeX, SafeY), D2D1::Point2F(Packet.Frame[0].x, Packet.Frame[0].y), brush, Packet.Frame[0].size * 2);
			}

			if (!Packet.Frame[1].skip)
			{
				SafeX = myLastX;
				SafeY = myLastY;
				if (ModulDe(SafeX - Packet.Frame[1].x) > 400 || ModulDe(SafeY - Packet.Frame[1].y) > 400)
				{
					SafeX = Packet.Frame[1].x;
					SafeY = Packet.Frame[1].y;
				}
				BitmapTarget->DrawLine(D2D1::Point2F(SafeX, SafeY), D2D1::Point2F(Packet.Frame[1].x, Packet.Frame[1].y), brush, Packet.Frame[1].size * 2);
			}
		}
		else
			FirstFrame = false;

		for (int i = 2; i < 8; i++)
			if (!Packet.Frame[i].skip)
			{
				SafeX = Packet.Frame[i - 2].x;
				SafeY = Packet.Frame[i - 2].y;
				if (ModulDe(SafeX - Packet.Frame[i].x) > 400 || ModulDe(SafeY - Packet.Frame[i].y) > 400)
				{
					SafeX = Packet.Frame[i].x;
					SafeY = Packet.Frame[i].y;
				}
				BitmapTarget->DrawLine(D2D1::Point2F(SafeX, SafeY), D2D1::Point2F(Packet.Frame[i].x, Packet.Frame[i].y), brush, Packet.Frame[i].size * 2);
			}

		myLastXX = Packet.Frame[6].x;
		myLastYY = Packet.Frame[6].y;
		myLastX = Packet.Frame[7].x;
		myLastY = Packet.Frame[7].y;
		if (internID != AppLocalID)
		{
			x = myLastX;
			y = myLastY;
			angle = Packet.Frame[7].angle;
			size = Packet.Frame[7].size;
		}
		myPacketFrame = 0;
		BitmapTarget->EndDraw();
	}
}

#define AngleModifier				 turn_velocity + (PU_Speeds * speed_modifier) + (PU_Sharps * sharp_modifier)

void Snakey::TurnLeft()
{
	float modifier = AngleModifier;
	if (PU_Corner)
	{
		modifier = 3.14f / 2;
		AppReleaseKey(Key_Left);
	}
	if (!PU_Reverse)
		angle -= modifier;
	else
		angle += modifier;
}

void Snakey::TurnRight()
{
	float modifier = AngleModifier;
	if (PU_Corner)
	{
		modifier = 3.14f / 2;
		AppReleaseKey(Key_Right);
	}

	if (!PU_Reverse)
		angle += modifier;
	else
		angle -= modifier;
}

void Snakey::ProcessPowerUp(PUType PowerUp)
{
	if (PowerUp < TOTAL_PU_GREEN + 1)
		StackPowerUp(PowerUp);
	else// if (PowerUp < TOTAL_PU_GREEN + TOTAL_PU_RED + 1)
		AppGame->SendPowerUp(PowerUp, internID);
}

bool Snakey::HitSomething()
{
	int px = (int)x;
	int py = (int)y;
	float ax, ay;
	int psize = (int)size + 2;
	if (psize < 2) psize = 2;

	for (int i = -1; i < 2; i++)
		for (int j = -1; j < 2; j++)
		if (px + i > 0 && py + j > 0 && px + i < matrixsize && py + j < matrixsize)
		if (ObsPoint[px + i][py + j].touch(internID, x, y, Ticks))
		return true;
	if (size > 2.f)
		for (int i = 0; i < 10; i++)
		{
			ax = x + ((size - 1) * cos(i * PID10));
			ay = y + ((size - 1) * sin(i * PID10));
			px = (int)ax;
			py = (int)ay;

			for (int i = -1; i < 2; i++)
				for (int j = -1; j < 2; j++)
				if (px + i > 0 && py + j > 0 && px + i < matrixsize && py + j < matrixsize)
				if (ObsPoint[px + i][py + j].touch(internID, ax, ay, Ticks))
				return true;
		}
	return false;
}

void Snakey::EraseLines()
{
	for (int i = 0; i < matrixsize; i++)
		for (int j = 0; j < matrixsize; j++)
		if (ObsPoint[i][j].getID() == internID)
		ObsPoint[i][j].reset();
	if (BitmapTarget)
	{
		BitmapTarget->Release();
		BitmapTarget = nullptr;
	}

}

void Snakey::Split()
{
}

void Snakey::StackPowerUp(PUType Type)
{
	switch (Type)
	{
	case PU_GREEN_ERASER: EraseLines(); break;
	case PU_CYAN_ERASER: EraseLines(); break;
	case PU_GREEN_SPLIT: Split(); break;
	case PU_GREEN_WALLS:
	case PU_CYAN_WALLS:
	{
		if (PU_Walls == 0)
		{
			for (int i = 0; i < POWERUP_STACK_SIZE; i++)
				if (!Snake_PowerUp[i].active)
				{
					Snake_PowerUp[i].Set(Type, Now);
					break;
				}
		}
		else
			for (int i = 0; i < POWERUP_STACK_SIZE; i++)
			if ((Snake_PowerUp[i].type == PU_CYAN_WALLS) || (Snake_PowerUp[i].type == PU_GREEN_WALLS))
			{
				if (Snake_PowerUp[i].active)
				{
					Snake_PowerUp[i].start_time = Now;
					break;
				}
			}
		break;


	}
	default:
		for (int i = 0; i < POWERUP_STACK_SIZE; i++)
			if (!Snake_PowerUp[i].active)
			{
				Snake_PowerUp[i].Set(Type, Now);
				break;
			}
	}


}

int pawgag = 0;

void Snakey::UpdatePUStack()
{
	PUCount = 0;
	PU_Speeds = PU_Thins = PU_Slows = PU_FAT = PU_Sharps = PU_Bigturns = PU_OneFinger = PU_Cheese = 0;
	PU_Corner = PU_Walls = PU_Reverse = PU_Fly = false;
	for (int i = 0; i < POWERUP_STACK_SIZE; i++)
		if (Snake_PowerUp[i].Active(Now))
		{
			float duration = Snake_PowerUp[i].duration;
			if (duration)
				PUStack[PUCount].percentage = (int)((100 * (Now - Snake_PowerUp[i].start_time)) / duration);
			PUCount++;
			switch (Snake_PowerUp[i].type)
			{
			case	PU_GREEN_SLOW:			PU_Slows++; break;
			case	PU_GREEN_SPEED:			if (PU_Speeds == 0) PU_Speeds++; else PU_Speeds *= 2; break;
			case	PU_GREEN_THIN:			PU_Thins++; break;
			case	PU_GREEN_CORNER:		PU_Corner = true; break;
			case	PU_GREEN_FLY:			PU_Fly = true; break;
			case	PU_GREEN_ERASER:		EraseLines(); break;
			case	PU_GREEN_WALLS:			PU_Walls = true; break;
			case	PU_GREEN_SPLIT:			Split(); break;
			case	PU_GREEN_SHARPCORNERS:	PU_Sharps++; break;

			case	PU_RED_SLOW:			PU_Slows++; break;
			case	PU_RED_SPEED:			if (PU_Speeds == 0) PU_Speeds++; else PU_Speeds *= 2; break;
			case	PU_RED_FAT:				PU_FAT++; break;
			case	PU_RED_CORNER:			PU_Corner = true; break;
			case	PU_RED_REVERSE:			PU_Reverse = true; break;
			case	PU_RED_CHEESE:			PU_Cheese++; break;
			case	PU_RED_ONEFINGER:		PU_OneFinger++; break;
			case	PU_RED_BIGTURNS:		PU_Bigturns++; break;

			case	PU_CYAN_ERASER:			EraseLines(); break;
			case	PU_CYAN_WALLS:			PU_Walls = true; break;
			}
		}
	if (PU_Speeds > 32) PU_Speeds = 32;
	//	size = 3.0f - (PU_Thins * 0.5f);
	size = 3.f / (PU_Thins + 1);
	if (size < 1.f) size = 1.f;

	PU_Walls = 1;
}

void Snakey::Render()
{
	if (bitmap)
	{
		bitmap->Release();
		bitmap = nullptr;
	}

	if (!BitmapTarget)
	{
		AppRender->CreateCompatibleRenderTarget(D2D1::SizeF(tablesize, tablesize), &BitmapTarget);
		BitmapTarget->CreateSolidColorBrush(CColor::ColorF(color), &brush);
	}

	BitmapTarget->GetBitmap(&bitmap);

	brush->SetColor(CColor::ColorF(color));
	if (!FirstFrame)
	{
		if (!myPacket.Frame[0].skip)
			if (ModulDe(myLastXX - myPacket.Frame[0].x) < 200 && ModulDe(myLastYY - myPacket.Frame[0].y) < 200)
			AppRender->DrawLine(D2D1::Point2F(table_x + myLastXX, table_y + myLastYY), D2D1::Point2F(table_x + myPacket.Frame[0].x, table_y + myPacket.Frame[0].y), brush, size * 2);
		if (!myPacket.Frame[1].skip)
			if (ModulDe(myLastX - myPacket.Frame[1].x) < 200 && ModulDe(myLastY - myPacket.Frame[1].y) < 200)
			AppRender->DrawLine(D2D1::Point2F(table_x + myLastX, table_y + myLastY), D2D1::Point2F(table_x + myPacket.Frame[1].x, table_y + myPacket.Frame[1].y), brush, size * 2);
	}
	for (int i = 2; i < myPacketFrame; i++)
		if (!myPacket.Frame[i].skip)
			if (ModulDe(myPacket.Frame[i - 2].x - myPacket.Frame[i].x) < 200 && ModulDe(myPacket.Frame[i-2].y - myPacket.Frame[i].y) < 200)
			AppRender->DrawLine(D2D1::Point2F(table_x + myPacket.Frame[i - 2].x, table_y + myPacket.Frame[i - 2].y), D2D1::Point2F(table_x + myPacket.Frame[i].x, table_y + myPacket.Frame[i].y), brush, size*2);


	if (bitmap)
	{
		AppBrush->SetColor(CColor::ColorF(color));
		AppRender->DrawBitmap(bitmap, D2D1::RectF(table_x, table_y, table_x + tablesize, table_y + tablesize), 1.f);
		AppRender->FillEllipse(D2D1::Ellipse(D2D1::Point2F(table_x + x, table_y + y), size, size), brush);
	
		float ax, ay, aax, aay;
		for (int i = 0; i < PUCount; i++)
		{
			ax = ay = aax = aay = 0;
			for (int j = 0; j < 100 - PUStack[i].percentage; j += 2)
			{
				aax = ax;
				aay = ay;
				ax = ((size * 4) + i * 4) * cos(j * PID100);
				ay = ((size * 4) + i * 4) * sin(j * PID100);
				AppBrush->SetColor(CColor::ColorF(CColor::lightGray));

				if (aax != 0)
					AppRender->DrawLine(D2D1::Point2F(table_x + ax + x, table_y + ay + y), D2D1::Point2F(table_x + aax + x, table_y + aay + y), AppBrush, 1.f);
			}
		}
	}
	//if (!isAlive)
	//{
	//	/*AppBrush->SetColor(CColor::ColorF(CColor::white));
	//	AppRender->DrawRectangle(D2D1::RectF(table_x + x - 15, table_y + y - 15, table_x + x + 15, table_y + y + 15), AppBrush);*/
	//}

}