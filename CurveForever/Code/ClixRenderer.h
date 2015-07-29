#pragma once

#include "ClixWindow.h"
#include "colors.h"

#include <d2d1.h>
//#include <D2d1_1.h>
//#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dwrite.lib" )

template <class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease);

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif



#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class ClixRenderer
{
private:
	
public:
	static ID2D1Factory*				D2Factory;
	static ID2D1HwndRenderTarget*		D2Render;
	static ID2D1SolidColorBrush*		D2Brush;
	static IDWriteFactory *				pDWriteFactory;
	static IDWriteTextFormat*			pTextFormat;
	static ID2D1BitmapRenderTarget*		BitmapRenderer;
	static ID2D1SolidColorBrush *		BitmapRendererBrush;
	//static ID2D1DeviceContext* deviceContext;

	ClixRenderer();
	~ClixRenderer();
	static HRESULT CreateDDR();
	static HRESULT CreateDIR();
	static void DiscardDR();
	static HRESULT StartDraw();
	static void EndDraw();
	static void DText(std::wstring text, float x, float y, float w, float h, float size, float r, float g, float b, float a);
	static void DText(std::wstring text, float x, float y, float w, float h, float size, CColor color);
	static void OnResize(UINT width,UINT height);
	static void ClearScreen();
};