#pragma once
#include "SnapTool.h"
#include "MainDlg.h"
class KeyWindow :public CWindowImpl<KeyWindow>
{
	CString m_DisplayText;
	float m_DpiScale = 1.0f;
	UINT m_VirtualKey = 'A';
	BOOL m_MoveResizeMode = FALSE;
	BOOL m_IsKeyDown = FALSE;
	BOOL m_RecalcTextPath = TRUE;
	BOOL m_IsSelected = FALSE;
	Gdiplus::GraphicsPath m_CachedTextPath;
	SnapTool m_SnapTool;
	BYTE m_WindowAlpha = 0xff;
	void _SetupKey(UINT key);
	void _DrawText(Gdiplus::Graphics & g);
	void _DrawMoveIndicator(Gdiplus::Graphics & g);
	void _SetWindowAlpha(BYTE alpha);
public:
	DECLARE_WND_CLASS_EX(NULL, CS_VREDRAW | CS_HREDRAW, NULL_BRUSH)

	BEGIN_MSG_MAP_EX(KeyWindow)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCHITTEST(OnNcHitTest)
		MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_NCLBUTTONDOWN(OnNcLButtonDown)
		MSG_WM_NCRBUTTONUP(OnNcRButtonUp)
		MSG_WM_SIZE(OnSize)
		MSG_WM_ENTERSIZEMOVE(OnEnterSizeMove)
		MSG_WM_EXITSIZEMOVE(OnExitSizeMove)
		MSG_WM_MOVING(OnMoving)
		MSG_WM_SIZING(OnSizing)
		MSG_WM_CONTEXTMENU(OnContextMenu)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
	END_MSG_MAP()

	KeyWindow();
	~KeyWindow();

	BOOL Create(UINT key = 'A');
	void Close();
	void SetMoveResizeMode(BOOL val);

	void OnPaint(CDCHandle dc);
	BOOL OnEraseBkgnd(CDCHandle dc);
	UINT OnNcHitTest(CPoint point);
	void OnGetMinMaxInfo(LPMINMAXINFO lpMMI);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnSize(UINT nType, CSize size);

	void _TriggerKeyDown();
	void _TriggerKeyUp();

	const WCHAR * GetWindowTitle() { return m_DisplayText; }
	UINT GetKey() { return m_VirtualKey; }
	void SetKey(UINT key);
	void SetSelected(BOOL val)
	{
		if ( val != m_IsSelected )
		{
			m_IsSelected = val;
			Invalidate(TRUE);
		}
	}
	BYTE GetAlpha() { return m_WindowAlpha; }
	void SetAlpha(BYTE alpha);
	void OnMoving(UINT fwSide, LPRECT pRect);
	void OnSizing(UINT fwSide, LPRECT pRect);
	void OnEnterSizeMove();
	void OnExitSizeMove();

	void OnContextMenu(CWindow wnd, CPoint point);
	void OnNcLButtonDown(UINT nHitTest, CPoint point);
	void OnNcRButtonUp(UINT nHitTest, CPoint point);
};