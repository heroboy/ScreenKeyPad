#pragma once

class KeyWindow :public CWindowImpl<KeyWindow>
{
	CString m_DisplayText;
	UINT m_VirtualKey = 'A';
	BOOL m_MoveResizeMode = FALSE;
	BOOL m_IsKeyDown = FALSE;
	BOOL m_RecalcTextPath = TRUE;
	Gdiplus::GraphicsPath m_CachedTextPath;
	void _SetupKey(UINT key);
	void _DrawText(Gdiplus::Graphics & g);
	void _DrawMoveIndicator(Gdiplus::Graphics & g);
public:
	DECLARE_WND_CLASS_EX(NULL, CS_VREDRAW | CS_HREDRAW, WHITE_BRUSH)

	BEGIN_MSG_MAP_EX(KeyWindow)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCHITTEST(OnNcHitTest)
		MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_SIZE(OnSize)
	END_MSG_MAP()

	KeyWindow();
	~KeyWindow();

	BOOL Create(UINT key = 'A');
	void Close();
	void SetMoveResizeMode(BOOL val);

	void OnPaint(CDCHandle dc);
	UINT OnNcHitTest(CPoint point);
	void OnGetMinMaxInfo(LPMINMAXINFO lpMMI);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnSize(UINT nType, CSize size);

	void _TriggerKeyDown();
	void _TriggerKeyUp();
};