#include "MemSubLoader.hpp"

// Subtitles window controls handling
LRESULT CALLBACK subtitlesWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_PAINT:
		{
			Gdiplus::Bitmap subtitlesBitmap(config.areaWidth, config.areaHeight, PixelFormat32bppARGB);
			HBITMAP bmp;
			Gdiplus::Graphics graphics(&subtitlesBitmap);
			HDC hdc = GetDC(hwnd);

			Gdiplus::Rect rect(0, 0, config.areaWidth, config.areaHeight);
			Gdiplus::StringFormat format;
			format.SetAlignment(getConfigAlignment(config.horizontalAlignment));
			format.SetLineAlignment(getConfigAlignment(config.verticalAlignment));
			Gdiplus::Font font(hdc, &config.subtitlesFont);
			Gdiplus::FontFamily fontFamily;
			font.GetFamily(&fontFamily);
			Gdiplus::FontStyle fontStyle = static_cast<Gdiplus::FontStyle>(font.GetStyle());
			int fontSize = font.GetSize();
			Gdiplus::Color fontColor(config.fontColorAlpha, GetRValue(config.fontColor), GetGValue(config.fontColor), GetBValue(config.fontColor));
			Gdiplus::Color outlineColor(config.outlineColorAlpha, GetRValue(config.outlineColor), GetGValue(config.outlineColor), GetBValue(config.outlineColor));
			Gdiplus::Color shadowsColor(config.shadowsColorAlpha, GetRValue(config.shadowsColor), GetGValue(config.shadowsColor), GetBValue(config.shadowsColor));
			TextDesigner::OutlineText text;

			graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
			graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
			graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
			graphics.Clear(Gdiplus::Color(0, 0, 0, 0));

			text.TextOutline(fontColor, outlineColor, config.outlineWidth);
			text.EnableShadow(true);
			if (config.shadowsDiffuse)
			{
				text.DiffusedShadow(shadowsColor, config.shadowsWidth, Point(config.shadowsXOffset, config.shadowsYOffset));
			}
			else
			{
				text.Shadow(shadowsColor, config.shadowsWidth, Point(config.shadowsXOffset, config.shadowsYOffset));
			}
			text.DrawString(&graphics, &fontFamily, fontStyle, fontSize, textToDraw.c_str(), rect, &format); 
	
			subtitlesBitmap.GetHBITMAP(Color(0, 0, 0, 0), &bmp);
			
			HDC memdc = CreateCompatibleDC(hdc);
			HGDIOBJ original = SelectObject(memdc, bmp);

			BLENDFUNCTION blend = { 0 };
			blend.BlendOp = AC_SRC_OVER;
			blend.SourceConstantAlpha = 255;
			blend.AlphaFormat = AC_SRC_ALPHA;
			POINT ptLocation = { config.areaXPosition, config.areaYPosition };
			SIZE szWnd = { config.areaWidth, config.areaHeight };
			POINT ptSrc = { 0, 0 };
			UpdateLayeredWindow(hwnd, hdc, &ptLocation, &szWnd, memdc, &ptSrc, 0, &blend, ULW_ALPHA);
			SelectObject(hdc, original);
	
			DeleteObject(bmp);
			DeleteObject(memdc);
		}
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Subtitles window initialization
int createSubtitlesWindow(void)
{
	const wchar_t SETTINGS_CLASS_NAME[] = L"subtitlesWindowClass";
	WNDCLASS subtitlesWindowClass = {};

	subtitlesWindowClass.lpfnWndProc = subtitlesWindowProc;
	subtitlesWindowClass.hInstance = GetModuleHandle(NULL);
	subtitlesWindowClass.lpszClassName = SETTINGS_CLASS_NAME;
	RegisterClass(&subtitlesWindowClass);

	subtitlesHWND = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, SETTINGS_CLASS_NAME, L"Subtitles", WS_VISIBLE | WS_POPUP, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, NULL, NULL);
	if (!IsWindow(subtitlesHWND))
	{
		return 1;
	}
	SetWindowPos(subtitlesHWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	return 0;
}
