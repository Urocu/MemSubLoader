/*
Text Designer Outline Text Library 

Copyright (c) 2009 Wong Shao Voon

The Code Project Open License (CPOL)
http://www.codeproject.com/info/cpol10.aspx
*/

#include "StdAfx.h"
#include "TextDblGlowStrategy.h"
#include "GDIPath.h"

using namespace TextDesigner;

TextDblGlowStrategy::TextDblGlowStrategy(void)
:
m_nThickness1(2),
m_nThickness2(2),
m_pbrushText(NULL),
m_bClrText(true)
{
}

TextDblGlowStrategy::~TextDblGlowStrategy(void)
{
}

ITextStrategy* TextDblGlowStrategy::Clone()
{
	TextDblGlowStrategy* p = new TextDblGlowStrategy();
	if(m_bClrText)
		p->Init(m_clrText, m_clrOutline1, m_clrOutline2, m_nThickness1, m_nThickness2);
	else
		p->Init(m_pbrushText, m_clrOutline1, m_clrOutline2, m_nThickness1, m_nThickness2);

	return static_cast<ITextStrategy*>(p);
}

void TextDblGlowStrategy::Init(
	Gdiplus::Color clrText, 
	Gdiplus::Color clrOutline1, 
	Gdiplus::Color clrOutline2, 
	int nThickness1, 
	int nThickness2)
{
	m_clrText = clrText; 
	m_bClrText = true;
	m_clrOutline1 = clrOutline1; 
	m_clrOutline2 = clrOutline2;
	m_nThickness1 = nThickness1; 
	m_nThickness2 = nThickness2;

}

void TextDblGlowStrategy::Init(
	Gdiplus::Brush* pbrushText, 
	Gdiplus::Color clrOutline1, 
	Gdiplus::Color clrOutline2, 
	int nThickness1, 
	int nThickness2)
{
	if(m_pbrushText&&m_pbrushText!=pbrushText)
		delete m_pbrushText;

	m_pbrushText = pbrushText; 
	m_bClrText = false;
	m_clrOutline1 = clrOutline1; 
	m_clrOutline2 = clrOutline2;
	m_nThickness1 = nThickness1; 
	m_nThickness2 = nThickness2;
}

bool TextDblGlowStrategy::DrawString(
	Gdiplus::Graphics* pGraphics,
	Gdiplus::FontFamily* pFontFamily,
	Gdiplus::FontStyle fontStyle,
	int nfontSize,
	const wchar_t*pszText, 
	Gdiplus::Point ptDraw, 
	Gdiplus::StringFormat* pStrFormat)
{
	using namespace Gdiplus;
	GraphicsPath path;
	Status status = path.AddString(pszText,wcslen(pszText),pFontFamily,fontStyle,nfontSize,ptDraw,pStrFormat);
	if(status!=Ok)
		return false;

	for(int i=m_nThickness1; i<=m_nThickness1+m_nThickness2; ++i)
	{
		Pen pen(m_clrOutline2,i);
		pen.SetLineJoin(LineJoinRound);
		pGraphics->DrawPath(&pen, &path);
	}
	Pen pen1(m_clrOutline1,m_nThickness1);
	pen1.SetLineJoin(LineJoinRound);
	pGraphics->DrawPath(&pen1, &path);

	Status status2 = Ok;
	if(m_bClrText)
	{
		SolidBrush brush(m_clrText);
		status2 = pGraphics->FillPath(&brush, &path);
	}
	else
	{
		status2 = pGraphics->FillPath(m_pbrushText, &path);
	}

	return status2 == Ok;
}

bool TextDblGlowStrategy::DrawString(
	Gdiplus::Graphics* pGraphics,
	Gdiplus::FontFamily* pFontFamily,
	Gdiplus::FontStyle fontStyle,
	int nfontSize,
	const wchar_t*pszText, 
	Gdiplus::Rect rtDraw, 
	Gdiplus::StringFormat* pStrFormat)
{
	using namespace Gdiplus;
	GraphicsPath path;
	Status status = path.AddString(pszText,wcslen(pszText),pFontFamily,fontStyle,nfontSize,rtDraw,pStrFormat);
	if(status!=Ok)
		return false;

	for(int i=m_nThickness1; i<=m_nThickness1+m_nThickness2; ++i)
	{
		Pen pen(m_clrOutline2,i);
		pen.SetLineJoin(LineJoinRound);
		pGraphics->DrawPath(&pen, &path);
	}
	Pen pen1(m_clrOutline1,m_nThickness1);
	pen1.SetLineJoin(LineJoinRound);
	pGraphics->DrawPath(&pen1, &path);

	Status status2 = Ok;
	if(m_bClrText)
	{
		SolidBrush brush(m_clrText);
		status2 = pGraphics->FillPath(&brush, &path);
	}
	else
	{
		status2 = pGraphics->FillPath(m_pbrushText, &path);
	}

	return status2 == Ok;
}

bool TextDblGlowStrategy::GdiDrawString(
	Gdiplus::Graphics* pGraphics, 
	LOGFONTW* pLogFont,
	const wchar_t*pszText, 
	Gdiplus::Point ptDraw)
{
	using namespace Gdiplus;
	Gdiplus::GraphicsPath* pPath=NULL;
	bool b = GDIPath::GetStringPath(
		pGraphics, 
		&pPath, 
		pszText, 
		pLogFont,
		ptDraw);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	for(int i=m_nThickness1; i<=m_nThickness1+m_nThickness2; ++i)
	{
		Pen pen(m_clrOutline2,i);
		pen.SetLineJoin(LineJoinRound);
		pGraphics->DrawPath(&pen, pPath);
	}

	Pen pen1(m_clrOutline1,m_nThickness1);
	pen1.SetLineJoin(LineJoinRound);
	pGraphics->DrawPath(&pen1, pPath);

	Status status2 = Ok;
	if(m_bClrText)
	{
		SolidBrush brush(m_clrText);
		status2 = pGraphics->FillPath(&brush, pPath);
	}
	else
	{
		status2 = pGraphics->FillPath(m_pbrushText, pPath);
	}

	if(pPath)
	{
		delete pPath;
		pPath = NULL;
	}

	return status2 == Ok;
}

bool TextDblGlowStrategy::GdiDrawString(
	Gdiplus::Graphics* pGraphics, 
	LOGFONTW* pLogFont,
	const wchar_t*pszText, 
	Gdiplus::Rect rtDraw)
{
	using namespace Gdiplus;
	Gdiplus::GraphicsPath* pPath=NULL;
	bool b = GDIPath::GetStringPath(
		pGraphics, 
		&pPath, 
		pszText, 
		pLogFont,
		rtDraw);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	for(int i=m_nThickness1; i<=m_nThickness1+m_nThickness2; ++i)
	{
		Pen pen(m_clrOutline2,i);
		pen.SetLineJoin(LineJoinRound);
		pGraphics->DrawPath(&pen, pPath);
	}

	Pen pen1(m_clrOutline1,m_nThickness1);
	pen1.SetLineJoin(LineJoinRound);
	pGraphics->DrawPath(&pen1, pPath);

	Status status2 = Ok;
	if(m_bClrText)
	{
		SolidBrush brush(m_clrText);
		status2 = pGraphics->FillPath(&brush, pPath);
	}
	else
	{
		status2 = pGraphics->FillPath(m_pbrushText, pPath);
	}

	if(pPath)
	{
		delete pPath;
		pPath = NULL;
	}
	return status2 == Ok;
}

bool TextDblGlowStrategy::MeasureString(
	Gdiplus::Graphics* pGraphics, 
	Gdiplus::FontFamily* pFontFamily,
	Gdiplus::FontStyle fontStyle,
	int nfontSize,
	const wchar_t*pszText, 
	Gdiplus::Point ptDraw, 
	Gdiplus::StringFormat* pStrFormat,
	float* pfPixelsStartX,
	float* pfPixelsStartY,
	float* pfDestWidth,
	float* pfDestHeight )
{
	using namespace Gdiplus;
	GraphicsPath path;
	Status status = path.AddString(pszText,wcslen(pszText),pFontFamily,fontStyle,nfontSize,ptDraw,pStrFormat);
	if(status!=Ok)
		return false;

	*pfDestWidth= ptDraw.X;
	*pfDestHeight= ptDraw.Y;
	bool b = GDIPath::MeasureGraphicsPath(pGraphics, &path, pfPixelsStartX, pfPixelsStartY, pfDestWidth, pfDestHeight);

	if(false==b)
		return false;

	float pixelThick = 0.0f;
	b = GDIPath::ConvertToPixels(pGraphics,m_nThickness1+m_nThickness2,0.0f,NULL,NULL,&pixelThick,NULL);

	if(false==b)
		return false;

	*pfDestWidth += pixelThick;
	*pfDestHeight += pixelThick;

	return true;
}

bool TextDblGlowStrategy::MeasureString(
	Gdiplus::Graphics* pGraphics, 
	Gdiplus::FontFamily* pFontFamily,
	Gdiplus::FontStyle fontStyle,
	int nfontSize,
	const wchar_t*pszText, 
	Gdiplus::Rect rtDraw,
	Gdiplus::StringFormat* pStrFormat,
	float* pfPixelsStartX,
	float* pfPixelsStartY,
	float* pfDestWidth,
	float* pfDestHeight )
{
	using namespace Gdiplus;
	GraphicsPath path;
	Status status = path.AddString(pszText,wcslen(pszText),pFontFamily,fontStyle,nfontSize,rtDraw,pStrFormat);
	if(status!=Ok)
		return false;

	*pfDestWidth= rtDraw.GetLeft();
	*pfDestHeight= rtDraw.GetTop();
	bool b = GDIPath::MeasureGraphicsPath(pGraphics, &path, pfPixelsStartX, pfPixelsStartY, pfDestWidth, pfDestHeight);

	if(false==b)
		return false;

	float pixelThick = 0.0f;
	b = GDIPath::ConvertToPixels(pGraphics,m_nThickness1+m_nThickness2,0.0f,NULL,NULL,&pixelThick,NULL);

	if(false==b)
		return false;

	*pfDestWidth += pixelThick;
	*pfDestHeight += pixelThick;

	return true;
}

bool TextDblGlowStrategy::GdiMeasureString(
	Gdiplus::Graphics* pGraphics, 
	LOGFONTW* pLogFont,
	const wchar_t*pszText, 
	Gdiplus::Point ptDraw,
	float* pfPixelsStartX,
	float* pfPixelsStartY,
	float* pfDestWidth,
	float* pfDestHeight )
{
	using namespace Gdiplus;
	Gdiplus::GraphicsPath* pPath=NULL;
	bool b = GDIPath::GetStringPath(
		pGraphics, 
		&pPath, 
		pszText, 
		pLogFont,
		ptDraw);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth= ptDraw.X;
	*pfDestHeight= ptDraw.Y;
	b = GDIPath::MeasureGraphicsPath(pGraphics, pPath, pfPixelsStartX, pfPixelsStartY, pfDestWidth, pfDestHeight);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	float pixelThick = 0.0f;
	b = GDIPath::ConvertToPixels(pGraphics,m_nThickness1+m_nThickness2,0.0f,NULL,NULL,&pixelThick,NULL);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth += pixelThick;
	*pfDestHeight += pixelThick;

	if(pPath)
	{
		delete pPath;
		pPath = NULL;
	}
	return true;
}

bool TextDblGlowStrategy::GdiMeasureString(
	Gdiplus::Graphics* pGraphics, 
	LOGFONTW* pLogFont,
	const wchar_t*pszText, 
	Gdiplus::Rect rtDraw,
	float* pfPixelsStartX,
	float* pfPixelsStartY,
	float* pfDestWidth,
	float* pfDestHeight )
{
	using namespace Gdiplus;
	Gdiplus::GraphicsPath* pPath=NULL;
	bool b = GDIPath::GetStringPath(
		pGraphics, 
		&pPath, 
		pszText, 
		pLogFont,
		rtDraw);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth= rtDraw.GetLeft();
	*pfDestHeight= rtDraw.GetTop();
	b = GDIPath::MeasureGraphicsPath(pGraphics, pPath, pfPixelsStartX, pfPixelsStartY, pfDestWidth, pfDestHeight);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	float pixelThick = 0.0f;
	b = GDIPath::ConvertToPixels(pGraphics,m_nThickness1+m_nThickness2,0.0f,NULL,NULL,&pixelThick,NULL);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth += pixelThick;
	*pfDestHeight += pixelThick;

	if(pPath)
	{
		delete pPath;
		pPath = NULL;
	}
	return true;
}

bool TextDblGlowStrategy::GdiMeasureStringRealHeight(
	Gdiplus::Graphics* pGraphics, 
	LOGFONTW* pLogFont,
	const wchar_t*pszText, 
	Gdiplus::Point ptDraw,
	float* pfPixelsStartX,
	float* pfPixelsStartY,
	float* pfDestWidth,
	float* pfDestHeight )
{
	using namespace Gdiplus;
	Gdiplus::GraphicsPath* pPath=NULL;
	bool b = GDIPath::GetStringPath(
		pGraphics, 
		&pPath, 
		pszText, 
		pLogFont,
		ptDraw);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth= ptDraw.X;
	*pfDestHeight= ptDraw.Y;
	b = GDIPath::MeasureGraphicsPathRealHeight(pGraphics, pPath, pfPixelsStartX, pfPixelsStartY, pfDestWidth, pfDestHeight);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	float pixelThick = 0.0f;
	b = GDIPath::ConvertToPixels(pGraphics,m_nThickness1+m_nThickness2,0.0f,NULL,NULL,&pixelThick,NULL);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth += pixelThick;
	*pfDestHeight += pixelThick;

	if(pPath)
	{
		delete pPath;
		pPath = NULL;
	}
	return true;
}

bool TextDblGlowStrategy::GdiMeasureStringRealHeight(
	Gdiplus::Graphics* pGraphics, 
	LOGFONTW* pLogFont,
	const wchar_t*pszText, 
	Gdiplus::Rect rtDraw,
	float* pfPixelsStartX,
	float* pfPixelsStartY,
	float* pfDestWidth,
	float* pfDestHeight )
{
	using namespace Gdiplus;
	Gdiplus::GraphicsPath* pPath=NULL;
	bool b = GDIPath::GetStringPath(
		pGraphics, 
		&pPath, 
		pszText, 
		pLogFont,
		rtDraw);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth= rtDraw.GetLeft();
	*pfDestHeight= rtDraw.GetTop();
	b = GDIPath::MeasureGraphicsPathRealHeight(pGraphics, pPath, pfPixelsStartX, pfPixelsStartY, pfDestWidth, pfDestHeight);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	float pixelThick = 0.0f;
	b = GDIPath::ConvertToPixels(pGraphics,m_nThickness1+m_nThickness2,0.0f,NULL,NULL,&pixelThick,NULL);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth += pixelThick;
	*pfDestHeight += pixelThick;

	if(pPath)
	{
		delete pPath;
		pPath = NULL;
	}
	return true;
}