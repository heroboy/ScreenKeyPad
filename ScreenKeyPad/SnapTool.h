#pragma once
#define SNAP_PIXELS 10
class SnapTool
{
	void _UpdateRect(LPCRECT pRect);
	bool _SnapLeft(int * newVal, int * dist);
	bool _SnapRight(int * newVal, int * dist);
	bool _SnapTop(int * newVal, int * dist);
	bool _SnapBottom(int * newVal, int * dist);
	bool _Snap(const std::vector<int>& edges, int val, int * newVal, int * dist);
public:
	
	HWND m_hWnd;
	std::vector<int> m_LeftEdges, m_RightEdges, m_TopEdges, m_BottomEdges;
	CRect m_StartRect;
	CRect m_PrevRect;
	CRect m_CurrentRect;
	void AddSnapRect(RECT rect);
	void EnterSizeMove(HWND hWnd);
	void ExitSizeMove();
	void OnMoving(LPRECT pRect);
	void OnSizing(UINT fwSide, LPRECT pRect);
};