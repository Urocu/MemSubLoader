#include "MemSubLoader.hpp"

LRESULT CALLBACK subtitlesWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_PAINT:
		{
			printf("%S\n", textToDraw.c_str());

			Bitmap softwareBitmap(SUBTITLES_WIDTH, SUBTITLES_HEIGHT, PixelFormat32bppARGB);
			HBITMAP bmp;
			Gdiplus::Graphics graphics(&softwareBitmap);

			SolidBrush blackBrush(Color(255, 255, 255, 0));
			Font myFont(L"Arial", 16);
			PointF origin(30.0f, 10.0f);
			//Gdiplus::Font font(hdc, &config.subtitlesFont);
			//Gdiplus::Color fontColor(GetRValue(config.subtitlesColor), GetGValue(config.subtitlesColor), GetBValue(config.subtitlesColor));
			//Gdiplus::SolidBrush solidBrush(fontColor);


			graphics.Clear(Gdiplus::Color(0, 0, 0, 0));
			graphics.DrawString(textToDraw.c_str(), textToDraw.length(), &myFont, origin, &blackBrush);
			softwareBitmap.GetHBITMAP(Color(0, 0, 0, 0), &bmp);
			HDC hdc = GetDC(hwnd);
			HDC memdc = CreateCompatibleDC(hdc);
			HGDIOBJ original = SelectObject(memdc, bmp);

			BLENDFUNCTION blend = { 0 };
			blend.BlendOp = AC_SRC_OVER;
			blend.SourceConstantAlpha = 255;
			blend.AlphaFormat = AC_SRC_ALPHA;
			POINT ptLocation = { 0, 300 };
			SIZE szWnd = { 640, 100 };
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

void createSubtitlesWindow(void)
{
	const wchar_t SETTINGS_CLASS_NAME[] = L"SubtitlesWindowClass";
	WNDCLASS subtitlesWindowClass = {};

	subtitlesWindowClass.lpfnWndProc = subtitlesWindowProc;
	subtitlesWindowClass.hInstance = GetModuleHandle(NULL);
	subtitlesWindowClass.lpszClassName = SETTINGS_CLASS_NAME;
	RegisterClass(&subtitlesWindowClass);

	subtitlesWin = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, SETTINGS_CLASS_NAME, L"Subtitles", WS_VISIBLE | WS_POPUP, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, NULL, NULL);
}
