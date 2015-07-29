#include "CLiXRenderer.h"
//#include <iostream>

template <class Interface>
void SafeRelease(Interface **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

ID2D1Factory*				ClixRenderer::D2Factory = 0;
ID2D1HwndRenderTarget*		ClixRenderer::D2Render = 0;
ID2D1SolidColorBrush*		ClixRenderer::D2Brush = 0;
IDWriteFactory*				ClixRenderer::pDWriteFactory = 0;
IDWriteTextFormat*			ClixRenderer::pTextFormat = 0;
ID2D1BitmapRenderTarget*	ClixRenderer::BitmapRenderer = 0;
ID2D1SolidColorBrush *		ClixRenderer::BitmapRendererBrush = 0;
//ID2D1DeviceContext*			ClixRenderer::deviceContext = 0;

ClixRenderer::ClixRenderer()
{
}

ClixRenderer::~ClixRenderer()
{
}

bool done = false;

HRESULT ClixRenderer::CreateDDR()
{
	RECT rc;
	GetClientRect(ClixWin::GetWindow(), &rc);

	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
	HRESULT hr = D2Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(ClixWin::GetWindow(), size), &D2Render);
	if (SUCCEEDED(hr))
		hr = D2Render->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &D2Brush);
	D2Render->CreateCompatibleRenderTarget(D2D1::SizeF(400, 400), &BitmapRenderer);
	BitmapRenderer->CreateSolidColorBrush(CColor::ColorF(CColor::red), &BitmapRendererBrush);
#ifdef CONSOLE
	if (SUCCEEDED(hr))
		std::wcout << "DeviceDependendResources Succesfully created!" << std::endl;
	else
		std::wcout << "DeviceDependedResources Error: " << std::hex << hr << std::endl;
#endif
	if (SUCCEEDED(hr))
		done = true;
	return hr;
}

HRESULT ClixRenderer::CreateDIR()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2Factory);
	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&pDWriteFactory)
			);
	}
	if (SUCCEEDED(hr))
	{
		hr = pDWriteFactory->CreateTextFormat(
			L"Microsoft Sans Serif Regular",                // Font family name.
			NULL,                       // Font collection (NULL sets it to use the system font collection).
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			15.0f,
			L"en-us",
			&pTextFormat
			);
	}



	// Center align (horizontally) the text.
	if (SUCCEEDED(hr))
	{
		hr = pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	}

	if (SUCCEEDED(hr))
	{
		hr = pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}


#ifdef CONSOLE
	if (SUCCEEDED(hr))
		std::wcout << "DeviceIndependendResources Succesfully created!" << std::endl;
	else
		std::wcout << "DeviceIndependedResources Error: " << std::hex << hr << std::endl;
#endif
	return hr;
}

void ClixRenderer::DiscardDR()
{
	SafeRelease(&D2Render);
	SafeRelease(&D2Brush);
	SafeRelease(&BitmapRenderer);
	SafeRelease(&BitmapRendererBrush);
}

HRESULT ClixRenderer::StartDraw()
{
	HRESULT hr = S_OK;
	if (!done)
		hr = CreateDDR();
	if (SUCCEEDED(hr))
	{
		D2Render->BeginDraw();
		D2Render->SetTransform(D2D1::Matrix3x2F::Identity());
	}
	return hr;
	//return S_OK;
}

void ClixRenderer::EndDraw()
{
	HRESULT hr = D2Render->EndDraw();
	if (hr = D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		//DiscardDR();
	}
}

void ClixRenderer::DText(std::wstring text, float x, float y, float w, float h, float size, float r, float g, float b, float a)
{
	IDWriteTextLayout* textLayout;
	D2Brush->SetColor(D2D1::ColorF(r, g, b, a));
	pDWriteFactory->CreateTextLayout(text.c_str(), text.length(), pTextFormat, w, h, &textLayout);
	DWRITE_TEXT_RANGE range = { 0, text.length() };
	textLayout->SetFontSize(size, range);
	D2Render->DrawTextLayout(D2D1::Point2F(x, y), textLayout, D2Brush);
	textLayout->Release();
}

void ClixRenderer::DText(std::wstring text, float x, float y, float w, float h, float size, CColor color)
{
	IDWriteTextLayout* textLayout;
	D2Brush->SetColor(CColor::ColorF(color));
	pDWriteFactory->CreateTextLayout(text.c_str(), text.length(), pTextFormat, w, h, &textLayout);
	DWRITE_TEXT_RANGE range = { 0, text.length() };
	textLayout->SetFontSize(size, range);
	D2Render->DrawTextLayout(D2D1::Point2F(x, y), textLayout, D2Brush);
	textLayout->Release();
}

void ClixRenderer::OnResize(UINT width, UINT height)
{
	if (D2Render)
		D2Render->Resize(D2D1::SizeU(width, height));
}

void ClixRenderer::ClearScreen()
{
	D2Render->Clear(D2D1::ColorF(D2D1::ColorF::Black));
}
