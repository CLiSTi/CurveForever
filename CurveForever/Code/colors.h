#pragma once
#include <d2d1.h>

class CColor
{
private:

public:
	int R, G, B, A;
	void set(int R, int G, int B, int A)
	{
		CColor::R = R;
		CColor::G = G;
		CColor::B = B;
		CColor::A = A;
	}
	void reset()
	{
		CColor::R = 0;
		CColor::G = 0;
		CColor::B = 0;
		CColor::A = 0;
	}
	static CColor RGBA(int R, int G, int B, int A)
	{
		CColor color;
		color.set(R, G, B, A);
		return color;
	}
	static CColor alphaToColor(CColor color, int A)
	{
		return CColor::RGBA(color.R, color.G, color.B, A);
	}
	static D2D1::ColorF ColorF(CColor  color)
	{
		float 
			r = (float)(color.R / (float)255),
			g = (float)(color.G / (float)255),
			b = (float)(color.B / (float)255),
			a = (float)(color.A / (float)255);
		return D2D1::ColorF(r, g, b, a);
	}
	static CColor red, green, blue, orange, yellow, lime, cyan, marin, purple, pink, white, black, gray, lightGray, darkGray;
};
