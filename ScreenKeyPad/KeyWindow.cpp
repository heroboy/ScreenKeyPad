#include "stdafx.h"
#include "KeyWindow.h"
#include "KeyDefine.h"
#define RESIZE_EDGE 15

KeyWindow::KeyWindow()
{

}

KeyWindow::~KeyWindow()
{

}

BOOL KeyWindow::Create(UINT key)
{
	if ( this->IsWindow() ) return FALSE;
	_SetupKey(key);
	CRect rcDefaultSize(0, 0, 100, 100);
	BOOL ret = __super::Create(NULL, rcDefaultSize, m_DisplayText, WS_VISIBLE | WS_POPUP, WS_EX_TOPMOST | WS_EX_NOACTIVATE) != NULL;
	return ret;
}

void KeyWindow::_SetupKey(UINT key)
{
	m_VirtualKey = key;
	m_DisplayText = KeyToString(key);
}

void KeyWindow::OnPaint(CDCHandle)
{
	CPaintDC dc(m_hWnd);
	Gdiplus::Graphics g(dc.m_hDC);


	CRect rcClient;
	GetClientRect(&rcClient);

	g.Clear(Gdiplus::Color::White);
	_DrawText(g);
	_DrawMoveIndicator(g);
}

UINT KeyWindow::OnNcHitTest(CPoint point)
{
	if ( m_MoveResizeMode )
	{
		this->ScreenToClient(&point);
		CRect rcClient;
		GetClientRect(&rcClient);
		const int WIDTH = rcClient.Width();
		const int HEIGHT = rcClient.Height();
		if ( point.y < RESIZE_EDGE )
		{
			if ( point.x < RESIZE_EDGE )
			{
				return HTTOPLEFT;
			}
			else if ( point.x >= WIDTH - RESIZE_EDGE )
			{
				return HTTOPRIGHT;
			}
			else
			{
				return HTTOP;
			}
		}
		else if ( point.y >= HEIGHT - RESIZE_EDGE )
		{
			if ( point.x < RESIZE_EDGE )
			{
				return HTBOTTOMLEFT;
			}
			else if ( point.x >= WIDTH - RESIZE_EDGE )
			{
				return HTBOTTOMRIGHT;
			}
			else
			{
				return HTBOTTOM;
			}
		}
		else
		{
			if ( point.x < RESIZE_EDGE )
			{
				return HTLEFT;
			}
			else if ( point.x >= WIDTH - RESIZE_EDGE )
			{
				return HTRIGHT;
			}
			else
			{
				return HTCAPTION;
			}
		}
	}
	else
	{
		return HTCLIENT;
	}
	return HTCLIENT;
}

void KeyWindow::OnGetMinMaxInfo(LPMINMAXINFO lpMMI)
{
	lpMMI->ptMinTrackSize = CPoint(70, 70);
}

void KeyWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ( m_MoveResizeMode ) return;
	this->_TriggerKeyDown();
	this->SetCapture();
}

void KeyWindow::OnLButtonUp(UINT nFlags, CPoint point)
{
	this->_TriggerKeyUp();
	if ( GetCapture() == m_hWnd )
	{
		ReleaseCapture();
	}
}

void KeyWindow::OnSize(UINT nType, CSize size)
{
	m_RecalcTextPath = TRUE;
	Invalidate();
}

void KeyWindow::_DrawText(Gdiplus::Graphics& g)
{
	auto TEXT_COLOR = Gdiplus::Color::Black;
	bool DRAW_OUTLINE = m_IsKeyDown ? FALSE : TRUE;
	CRect rcClient;
	GetClientRect(&rcClient);
	const Gdiplus::FontFamily * fontFamily = Gdiplus::FontFamily::GenericSansSerif();
	Gdiplus::Font font(fontFamily, rcClient.Height() * 2 / 3.0f, 0, Gdiplus::UnitPixel);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	Gdiplus::SolidBrush brush(TEXT_COLOR);
	//Gdiplus::RectF rcText((float)rcClient.left, (float)rcClient.top + rcClient.Height() / 6.0f, (float)rcClient.Width(), (float)rcClient.Height());
	if ( m_RecalcTextPath )
	{
		m_RecalcTextPath = FALSE;
		m_CachedTextPath.Reset();
		float EDGE = 10;
		float WIDTH = rcClient.Width() - EDGE * 2.0f;
		float HEIGHT = rcClient.Height() - EDGE * 2.0f;
		float StartFontSize = HEIGHT - 1;
		if ( WIDTH > 0.0f && HEIGHT > 0.0f && StartFontSize > 0.0f )
		{
			while ( true )
			{
				Gdiplus::Font font2(fontFamily, StartFontSize, 0, Gdiplus::UnitPixel);
				Gdiplus::RectF outRect;
				g.MeasureString(m_DisplayText.GetString(), m_DisplayText.GetLength(), &font2, Gdiplus::PointF(), &outRect);
				if ( (outRect.Width <= WIDTH && outRect.Height < HEIGHT) || StartFontSize < 10.0f )
				{
					outRect.X = (WIDTH - outRect.Width) * 0.5f + EDGE;
					outRect.Y = (HEIGHT - outRect.Height) * 0.5f + EDGE;
					m_CachedTextPath.AddString((const WCHAR*)m_DisplayText, m_DisplayText.GetLength(), fontFamily, 0, StartFontSize, outRect, &format);
					//m_CachedTextPath.AddRectangle(outRect);
					break;
				}
				StartFontSize -= 1.0f;
			}
		}
	}
	if ( DRAW_OUTLINE )
	{
		Gdiplus::Pen pp(&brush, 1.0f);
		g.DrawPath(&pp, &m_CachedTextPath);
	}
	else
	{
		//g.DrawString((const WCHAR*)m_DisplayText, m_DisplayText.GetLength(), &font, rcText, &format, &brush);
		g.FillPath(&brush, &m_CachedTextPath);
	}

}

void KeyWindow::_DrawMoveIndicator(Gdiplus::Graphics & g)
{
	if ( !m_MoveResizeMode ) return;

	CRect rcClient;
	GetClientRect(&rcClient);
	const float HEIGHT = (float)rcClient.Height();
	const float WIDTH = (float)rcClient.Width();
	const FLOAT EDGE = (float)RESIZE_EDGE;
	const FLOAT EDGEEDGE = EDGE * 0.25f;
	//draw edge
	{
		Gdiplus::SolidBrush brush(Gdiplus::Color::MakeARGB(127, 0, 0xff, 0));
		g.FillRectangle(&brush, Gdiplus::RectF(0, 0, WIDTH, EDGE));
		g.FillRectangle(&brush, Gdiplus::RectF(0, HEIGHT - EDGE, WIDTH, EDGE));
		g.FillRectangle(&brush, Gdiplus::RectF(0, EDGE, EDGE, HEIGHT - EDGE - EDGE));
		g.FillRectangle(&brush, Gdiplus::RectF(WIDTH - EDGE, EDGE, EDGE, HEIGHT - EDGE - EDGE));
	}
	//Draw Arrow

	float TRI_HEIGHT = (EDGE - EDGEEDGE) * 1.474f;
	Gdiplus::PointF pts[3];
	Gdiplus::SolidBrush brush(Gdiplus::Color::Black);
	//left
	pts[0].X = EDGEEDGE;
	pts[0].Y = HEIGHT * 0.5f;
	pts[1].X = EDGE;
	pts[1].Y = HEIGHT * 0.5f - TRI_HEIGHT;
	pts[2].X = EDGE;
	pts[2].Y = HEIGHT * 0.5f + TRI_HEIGHT;
	g.FillPolygon(&brush, pts, 3);
	//right
	pts[0].X = WIDTH - EDGEEDGE;
	pts[0].Y = HEIGHT * 0.5f;
	pts[1].X = WIDTH - EDGE;
	pts[1].Y = HEIGHT * 0.5f - TRI_HEIGHT;
	pts[2].X = WIDTH - EDGE;
	pts[2].Y = HEIGHT * 0.5f + TRI_HEIGHT;
	g.FillPolygon(&brush, pts, 3);
	//top
	pts[0].X = WIDTH * 0.5f;
	pts[0].Y = EDGEEDGE;
	pts[1].X = WIDTH * 0.5f - TRI_HEIGHT;
	pts[1].Y = EDGE;
	pts[2].X = WIDTH * 0.5f + TRI_HEIGHT;
	pts[2].Y = EDGE;
	g.FillPolygon(&brush, pts, 3);
	//bottom
	pts[0].X = WIDTH * 0.5f;
	pts[0].Y = HEIGHT - EDGEEDGE;
	pts[1].X = WIDTH * 0.5f - TRI_HEIGHT;
	pts[1].Y = HEIGHT - EDGE;
	pts[2].X = WIDTH * 0.5f + TRI_HEIGHT;
	pts[2].Y = HEIGHT - EDGE;
	g.FillPolygon(&brush, pts, 3);
	//top left
	pts[0].X = EDGEEDGE;
	pts[0].Y = EDGEEDGE;
	pts[1].X = EDGE;
	pts[1].Y = EDGEEDGE;
	pts[2].X = EDGEEDGE;
	pts[2].Y = EDGE;
	g.FillPolygon(&brush, pts, 3);
	//top right
	pts[0].X = WIDTH - EDGEEDGE;
	pts[0].Y = EDGEEDGE;
	pts[1].X = WIDTH - EDGE;
	pts[1].Y = EDGEEDGE;
	pts[2].X = WIDTH - EDGEEDGE;
	pts[2].Y = EDGE;
	g.FillPolygon(&brush, pts, 3);
	//bottom left
	pts[0].X = EDGEEDGE;
	pts[0].Y = HEIGHT - EDGEEDGE;
	pts[1].X = EDGE;
	pts[1].Y = HEIGHT - EDGEEDGE;
	pts[2].X = EDGEEDGE;
	pts[2].Y = HEIGHT - EDGE;
	g.FillPolygon(&brush, pts, 3);
	//bottom right
	pts[0].X = WIDTH - EDGEEDGE;
	pts[0].Y = HEIGHT - EDGEEDGE;
	pts[1].X = WIDTH - EDGE;
	pts[1].Y = HEIGHT - EDGEEDGE;
	pts[2].X = WIDTH - EDGEEDGE;
	pts[2].Y = HEIGHT - EDGE;
	g.FillPolygon(&brush, pts, 3);
}

void KeyWindow::Close()
{
	this->_TriggerKeyUp();
	this->DestroyWindow();
}

void KeyWindow::SetMoveResizeMode(BOOL val)
{
	m_MoveResizeMode = val;
	if ( val )
	{
		this->_TriggerKeyUp();
	}
	Invalidate();
}

void KeyWindow::_TriggerKeyDown()
{
	if ( !m_IsKeyDown )
	{
		m_IsKeyDown = TRUE;
		INPUT input;
		ZeroMemory(&input, sizeof(input));
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = (WORD)m_VirtualKey;
		input.ki.dwFlags = 0;
		SendInput(1, &input, sizeof(input));
		Invalidate();
	}
}

void KeyWindow::_TriggerKeyUp()
{
	if ( m_IsKeyDown )
	{
		m_IsKeyDown = FALSE;
		INPUT input;
		ZeroMemory(&input, sizeof(input));
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = (WORD)m_VirtualKey;
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(input));
		Invalidate();
	}
}