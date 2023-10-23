#include "MemSubLoader.hpp"

// Subtitles window controls handling
LRESULT CALLBACK subtitlesWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_PAINT:
		{
			// Initialization
			Gdiplus::Bitmap subtitlesBitmap(tmpConfig.areaWidth, tmpConfig.areaHeight, PixelFormat32bppARGB);
			HBITMAP bmp;
			Gdiplus::Graphics graphics(&subtitlesBitmap);
			HDC hdc = GetDC(hwnd);

			Gdiplus::Rect rect(0, 0, tmpConfig.areaWidth, tmpConfig.areaHeight);
			Gdiplus::Font font(hdc, &tmpConfig.subtitlesFont);
			Gdiplus::FontFamily fontFamily;
			font.GetFamily(&fontFamily);
			Gdiplus::FontStyle fontStyle = static_cast<Gdiplus::FontStyle>(font.GetStyle());
			int fontSize = font.GetSize();
			Gdiplus::StringFormat format;
			format.SetAlignment(getConfigAlignment(tmpConfig.horizontalAlignment));
			format.SetLineAlignment(getConfigAlignment(tmpConfig.verticalAlignment));

			Gdiplus::Color fontColor(tmpConfig.fontColorAlpha, GetRValue(tmpConfig.fontColor), GetGValue(tmpConfig.fontColor), GetBValue(tmpConfig.fontColor));
			Gdiplus::Color outlineColor(tmpConfig.outlineColorAlpha, GetRValue(tmpConfig.outlineColor), GetGValue(tmpConfig.outlineColor), GetBValue(tmpConfig.outlineColor));
			Gdiplus::Color shadowsColor(tmpConfig.shadowsColorAlpha, GetRValue(tmpConfig.shadowsColor), GetGValue(tmpConfig.shadowsColor), GetBValue(tmpConfig.shadowsColor));
			TextDesigner::OutlineText text;

			// Rendering settings & clearing
			graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
			graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
			graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
			graphics.Clear(Gdiplus::Color(0, 0, 0, 0));

			// Enable/disable area preview
			if (tmpConfig.areaPreview)
			{
				Gdiplus::Pen outlinePen(Gdiplus::Color(255, 0, 0), 4);
				graphics.DrawRectangle(&outlinePen, rect);
			}

			// Enable/diable outline
			if (tmpConfig.outlineWidth > 0)
			{
				text.TextOutline(fontColor, outlineColor, tmpConfig.outlineWidth);
			}
			else
			{
				text.TextNoOutline(fontColor);
			}
			
			// Enable/disable shadows
			if (tmpConfig.shadowsWidth > 0)
			{
				text.EnableShadow(true);

				if (tmpConfig.shadowsDiffuse)
				{
					text.DiffusedShadow(shadowsColor, tmpConfig.shadowsWidth, Point(tmpConfig.shadowsXOffset, tmpConfig.shadowsYOffset));
				}
				else
				{
					text.Shadow(shadowsColor, tmpConfig.shadowsWidth, Point(tmpConfig.shadowsXOffset, tmpConfig.shadowsYOffset));
				}
			}

			// Drawing subtitles
			text.DrawString(&graphics, &fontFamily, fontStyle, fontSize, textToDraw.c_str(), rect, &format); 

			// Transparency
			subtitlesBitmap.GetHBITMAP(Color(0, 0, 0, 0), &bmp);
			HDC memdc = CreateCompatibleDC(hdc);
			HGDIOBJ original = SelectObject(memdc, bmp);
			BLENDFUNCTION blend = { 0 };
			blend.BlendOp = AC_SRC_OVER;
			blend.SourceConstantAlpha = 255;
			blend.AlphaFormat = AC_SRC_ALPHA;
			POINT ptLocation = { tmpConfig.areaXPosition, tmpConfig.areaYPosition };
			SIZE szWnd = { tmpConfig.areaWidth, tmpConfig.areaHeight };
			POINT ptSrc = { 0, 0 };

			// Update subtitles window
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
