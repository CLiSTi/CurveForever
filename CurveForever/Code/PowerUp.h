#pragma once
#include "ClixRenderer.h"
#include "CLiXApplication.h"
#include "FineTuning.h"
#include "colors.h"

#define TOTAL_PU_GREEN	20
#define TOTAL_PU_RED	20
#define TOTAL_PU_CYAN	20

#define PUSize 16
#define POWERUP_STACK_SIZE 128

enum PU_AffectType { AT_GREEN, AT_RED, AT_CYAN };

#pragma region PU_DURATION

#define PU_DURATION_GREEN_SLOW				10000
#define PU_DURATION_GREEN_SPEED				3000
#define PU_DURATION_GREEN_FLY				6000
#define PU_DURATION_GREEN_THIN				15000
#define PU_DURATION_GREEN_CORNER			15000
#define PU_DURATION_GREEN_WALLS				15000	
#define PU_DURATION_GREEN_SHARPCORNERS		15000

#define PU_DURATION_RED_SLOW				5000
#define PU_DURATION_RED_SPEED				3000
#define PU_DURATION_RED_FAT					7000
#define PU_DURATION_RED_REVERSE				5000
#define PU_DURATION_RED_CHEESE				5000	//NEED CORECTION
#define PU_DURATION_RED_CORNER				7000
#define PU_DURATION_RED_ONEFINGER			5000
#define PU_DURATION_RED_BIGTURNS			3000	//NEED CORECTION

#define PU_DURATION_CYAN_CLOSINGWALLS		10000	//NEED CORECTION
#define PU_DURATION_CYAN_WALLS				10000						
#define PU_DURATION_CYAN_SWITCH				2500
#define PU_DURATION_CYAN_MOVINGPOWERUPS		10000
#define PU_DURATION_CYAN_NODRAWAREA			10000	//NEED CORECTION
#define PU_DURATION_CYAN_DARKNESS			5000	//NEED CORECTION

#pragma endregion PU_DURATION		

enum PUType {
	PU_NOTHING = -1,

	PU_GREEN_SLOW,
	PU_GREEN_SPEED,
	PU_GREEN_THIN,
	PU_GREEN_CORNER,
	PU_GREEN_FLY,
	PU_GREEN_ERASER,
	PU_GREEN_WALLS,
	PU_GREEN_SPLIT,
	PU_GREEN_SHARPCORNERS,

	PU_RED_SLOW = TOTAL_PU_GREEN + 1,
	PU_RED_SPEED,
	PU_RED_FAT,
	PU_RED_CORNER,
	PU_RED_REVERSE,
	PU_RED_CHEESE,
	PU_RED_ONEFINGER,
	PU_RED_BIGTURNS,

	PU_CYAN_ERASER = TOTAL_PU_GREEN + TOTAL_PU_RED + 1,
	PU_CYAN_CLOSINGWALLS,
	PU_CYAN_WALLS,
	PU_CYAN_BUBBLES,
	PU_CYAN_SWITCH,
	PU_CYAN_MOVINGPOWERUPS,
	PU_CYAN_NODRAWAREA,
	PU_CYAN_DARKNESS,

	PU_TOTAL
};

class PowerUp_Item
{
	float x, y;
	PUType type;
	PU_AffectType Affects;
	bool isActive;
public:
	PowerUp_Item() : x(0), y(0), type(PU_NOTHING), isActive(false) { }
	void			Set(float X, float Y, PUType Type) 
					{ 
						if (Type != PU_NOTHING)
						{
							x = X;
							y = Y;
							type = Type;
							isActive = true;
							if (type < 20)
								Affects = AT_GREEN;
							else
								if (type < 40)
									Affects = AT_RED;
								else
									Affects = AT_CYAN;
						}
						else isActive = false;

					}
	bool			Touch(float X, float Y)
					{ 
						if (X > x - PUSize && X < x + PUSize && Y > y - PUSize && Y < y + PUSize)
							return true; 						
						return false; 
					}
	PUType			GetType() { return type; }
	void			SetInactive() { isActive = false; }
	bool			Active() { return isActive; }
	PUType			Grab() { isActive = false; return type; }
	PU_AffectType	GetAffectsType() { return Affects; }
	void			Render()
					{
						if (isActive)
						{
							switch (Affects)
							{
							case AT_GREEN:	AppBrush->SetColor(CColor::ColorF(CColor::lime));
								break;
							case AT_RED:	AppBrush->SetColor(CColor::ColorF(CColor::red));
								break;
							case AT_CYAN:	AppBrush->SetColor(CColor::ColorF(CColor::marin));
								break;
							}
							AppRender->FillEllipse(D2D1::Ellipse(D2D1::Point2F(table_x + x, table_y + y), PUSize, PUSize), AppBrush);
							RenderText(ftw(type), table_x + x - 5, table_y + y - 5, 20, 20, 10, CColor::white);
						}
					}
};

class PowerUp_Atom
{
public:
	PUType type;
	bool active;
	long start_time, duration;
	PowerUp_Atom() : type(PU_NOTHING), start_time(0) {
		duration = 0;
		active = false;
	};

	void Reset()
	{
		type = PU_NOTHING;
		//duration = 0;
		active = false;
	}

	void Set(PUType Type, long Time_Now)
	{
		start_time = Time_Now;
		type = Type;
		active = true;
		switch (type)
		{
		case PU_NOTHING: duration = 0; break;

		case	PU_GREEN_SLOW:			duration = PU_DURATION_GREEN_SLOW; break;
		case	PU_GREEN_SPEED:			duration = PU_DURATION_GREEN_SPEED; break;
		case	PU_GREEN_THIN:			duration = PU_DURATION_GREEN_THIN; break;
		case	PU_GREEN_CORNER:		duration = PU_DURATION_GREEN_CORNER; break;
		case	PU_GREEN_FLY:			duration = PU_DURATION_GREEN_FLY; break;
		case	PU_GREEN_ERASER:		duration = 0; break;
		case	PU_GREEN_WALLS:			duration = PU_DURATION_GREEN_WALLS; break;
		case	PU_GREEN_SPLIT:			duration = 0; break;
		case	PU_GREEN_SHARPCORNERS:	duration = PU_DURATION_GREEN_SHARPCORNERS; break;

		case	PU_RED_SLOW:			duration = PU_DURATION_RED_SLOW; break;
		case	PU_RED_SPEED:			duration = PU_DURATION_RED_SPEED; break;
		case	PU_RED_FAT:				duration = PU_DURATION_RED_FAT; break;
		case	PU_RED_CORNER:			duration = PU_DURATION_RED_CORNER; break;
		case	PU_RED_REVERSE:			duration = PU_DURATION_RED_REVERSE; break;
		case	PU_RED_CHEESE:			duration = PU_DURATION_RED_CHEESE; break;
		case	PU_RED_ONEFINGER:		duration = PU_DURATION_RED_ONEFINGER; break;
		case	PU_RED_BIGTURNS:		duration = PU_DURATION_RED_BIGTURNS; break;

		case	PU_CYAN_ERASER:			duration = 0; break;
		case	PU_CYAN_CLOSINGWALLS:	duration = PU_DURATION_CYAN_CLOSINGWALLS; break;
		case	PU_CYAN_WALLS:			duration = PU_DURATION_CYAN_WALLS; break;
		case	PU_CYAN_BUBBLES:		duration = 0; break;
		case	PU_CYAN_SWITCH:			duration = PU_DURATION_CYAN_SWITCH; break;
		case	PU_CYAN_MOVINGPOWERUPS: duration = PU_DURATION_CYAN_MOVINGPOWERUPS; break;
		case	PU_CYAN_NODRAWAREA:		duration = PU_DURATION_CYAN_NODRAWAREA; break;
		case	PU_CYAN_DARKNESS:		duration = PU_DURATION_CYAN_DARKNESS; break;
		}
	}

	bool Active(long Time_Now)
	{
		if (active)
			if (Time_Now - start_time <= duration)
				return true;
			else
			{
				active = false;
				return false;
			}
		else
			return false;
	}
	bool isActive() { return active; }
};
