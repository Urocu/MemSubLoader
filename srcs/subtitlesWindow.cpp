#include "MemSubLoader.hpp"

// Subtitles window controls handling
LRESULT CALLBACK subtitlesWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_PAINT:
		{
			Config config;
			setDefaultConfig(config);// set to default in case the identifier is wrong
			if (livePreview)
			{
				config = tmpConfig;
				textToDraw = L"This is an example text to test subtitles configuration.";
			}
			else
			{
			    wchar_t identifier[255];
                wcscpy(identifier, testidentifier.c_str());
				std::map<wchar_t *, Config>::iterator iter = getConfig(identifier);
				if (iter != configs.end())
				{
					config = iter->second;
				}
				else
                {
                    wcscpy(identifier, L"DEFAULT");
                    iter = getConfig(identifier);
                    if (iter != configs.end())
                    {
                        config = iter->second;
                    }
                }
			}

            if(screenWidth != GetSystemMetrics(SM_CXSCREEN) || screenHeight != GetSystemMetrics(SM_CYSCREEN))
            {
                screenWidth = GetSystemMetrics(SM_CXSCREEN);
                screenHeight = GetSystemMetrics(SM_CYSCREEN);
                checkConfig(config);
            }

			// Initialization
			Gdiplus::Bitmap subtitlesBitmap(config.areaWidth*screenWidth/100, config.areaHeight*screenHeight/100, PixelFormat32bppARGB);
			HBITMAP bmp;
			Gdiplus::Graphics graphics(&subtitlesBitmap);
			HDC hdc = GetDC(hwnd);
			Gdiplus::Rect rect(0, 0, config.areaWidth*screenWidth/100, config.areaHeight*screenHeight/100);
			Gdiplus::Font font(hdc, &config.subtitlesFont);
			Gdiplus::FontFamily fontFamily;
			font.GetFamily(&fontFamily);
			Gdiplus::FontStyle fontStyle = static_cast<Gdiplus::FontStyle>(font.GetStyle());
			int fontSize = font.GetSize();
			Gdiplus::StringFormat format;
			format.SetAlignment(getConfigAlignment(config.horizontalAlignment));
			format.SetLineAlignment(getConfigAlignment(config.verticalAlignment));

			Gdiplus::Color fontColor(config.fontColorAlpha, GetRValue(config.fontColor), GetGValue(config.fontColor), GetBValue(config.fontColor));
			Gdiplus::Color outlineColor(config.outlineColorAlpha, GetRValue(config.outlineColor), GetGValue(config.outlineColor), GetBValue(config.outlineColor));
			Gdiplus::Color shadowsColor(config.shadowsColorAlpha, GetRValue(config.shadowsColor), GetGValue(config.shadowsColor), GetBValue(config.shadowsColor));
			TextDesigner::OutlineText text;

			// Rendering settings & clearing
			graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
			graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
			graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
			graphics.Clear(Gdiplus::Color(0, 0, 0, 0));

			// Enable/disable area preview
			if (config.areaPreview)
			{
				Gdiplus::Pen outlinePen(Gdiplus::Color(255, 0, 0), 4);
				graphics.DrawRectangle(&outlinePen, rect);
			}

			// Enable/diable outline
			if (config.outlineWidth > 0)
			{
				text.TextOutline(fontColor, outlineColor, config.outlineWidth);
			}
			else
			{
				text.TextNoOutline(fontColor);
			}

			// Enable/disable shadows
			if (config.shadowsWidth > 0)
			{
				text.EnableShadow(true);

				if (config.shadowsDiffuse)
				{
					text.DiffusedShadow(shadowsColor, config.shadowsWidth, Point(config.shadowsXOffset, config.shadowsYOffset));
				}
				else
				{
					text.Shadow(shadowsColor, config.shadowsWidth, Point(config.shadowsXOffset, config.shadowsYOffset));
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
			POINT ptLocation = { config.areaXPosition*(screenWidth-1)/100, config.areaYPosition*(screenHeight-1)/100 };
			SIZE szWnd = { config.areaWidth*screenWidth/100, config.areaHeight*screenHeight/100 };
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

	subtitlesHWND = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, SETTINGS_CLASS_NAME, L"Subtitles", WS_VISIBLE | WS_POPUP, 0, 0, screenWidth, screenHeight, NULL, NULL, NULL, NULL);
	if (!IsWindow(subtitlesHWND))
	{
		return 1;
	}

	// Window icon
	SendMessage(subtitlesHWND, WM_SETICON, ICON_SMALL, (LPARAM)iconSubtitles);
	SendMessage(subtitlesHWND, WM_SETICON, ICON_BIG, (LPARAM)iconSubtitles);

	SetWindowPos(subtitlesHWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	return 0;
}
