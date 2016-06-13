#include "stdafx.h"
#include "SnapTool.h"
#include <algorithm>
void SnapTool::EnterSizeMove(HWND hWnd)
{
	m_hWnd = hWnd;
	GetWindowRect(m_hWnd, &m_StartRect);
	m_PrevRect = m_StartRect;
	m_CurrentRect = m_StartRect;
}

void SnapTool::ExitSizeMove()
{
	m_TopEdges.clear();
	m_BottomEdges.clear();
	m_LeftEdges.clear();
	m_RightEdges.clear();
}

void SnapTool::OnMoving(LPRECT pRect)
{
	_UpdateRect(pRect);
	
	{
		bool isTop, isBottom;
		int top, bottom, topDist = 0xffff, bottomDist = 0xffff;
		isTop = _SnapTop(&top, &topDist);
		isBottom = _SnapBottom(&bottom, &bottomDist);
		if ( isTop && (!isBottom || topDist <= bottomDist) )
		{
			//AtlTrace("snap top dist:%d\n", top - pRect->top);
			pRect->bottom += top - pRect->top;
			pRect->top = top;
		}
		else if ( isBottom )
		{
			//AtlTrace("snap bottom dist:%d\n", bottom - pRect->bottom);
			pRect->top += bottom - pRect->bottom;
			pRect->bottom = bottom;
		}
	}
	{
		bool isLeft, isRight;
		int left, right, leftDist = 0xffff, rightDist = 0xffff;
		isLeft = _SnapLeft(&left, &leftDist);
		isRight = _SnapRight(&right, &rightDist);
		if ( isLeft && (!isRight || leftDist <= rightDist) )
		{
			pRect->right += left - pRect->left;
			pRect->left = left;
		}
		else if ( isRight )
		{
			pRect->left += right - pRect->right;
			pRect->right = right;
		}
	}
	m_PrevRect = *pRect;
}

void SnapTool::OnSizing(UINT fwSide, LPRECT pRect)
{
	_UpdateRect(pRect);
	int dist;
	if ( fwSide == WMSZ_TOP || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_TOPRIGHT )
	{
		int top;
		if ( _SnapTop(&top, &dist) )
		{
			pRect->top = top;
		}
	}
	if ( fwSide == WMSZ_BOTTOM || fwSide == WMSZ_BOTTOMLEFT || fwSide == WMSZ_BOTTOMRIGHT )
	{
		int bottom;
		if ( _SnapBottom(&bottom, &dist) )
		{
			pRect->bottom = bottom;
		}
	}
	if ( fwSide == WMSZ_LEFT || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_BOTTOMLEFT )
	{
		int left;
		if ( _SnapLeft(&left, &dist) )
		{
			pRect->left = left;
		}
	}
	if ( fwSide == WMSZ_RIGHT || fwSide == WMSZ_TOPRIGHT || fwSide == WMSZ_BOTTOMRIGHT )
	{
		int right;
		if ( _SnapRight(&right, &dist) )
		{
			pRect->right = right;
		}
	}
	m_PrevRect = *pRect;
}

void SnapTool::_UpdateRect(LPCRECT pRect)
{
	CRect rect(*pRect);
	m_CurrentRect.left += rect.left - m_PrevRect.left;
	m_CurrentRect.right += rect.right - m_PrevRect.right;
	m_CurrentRect.top += rect.top - m_PrevRect.top;
	m_CurrentRect.bottom += rect.bottom - m_PrevRect.bottom;
	m_PrevRect = rect;
}

bool SnapTool::_SnapLeft(int * newVal, int * dist)
{
	return _Snap(m_RightEdges, m_CurrentRect.left, newVal, dist);
}

bool SnapTool::_SnapRight(int * newVal, int * dist)
{
	return _Snap(m_LeftEdges, m_CurrentRect.right, newVal, dist);
}

bool SnapTool::_SnapTop(int * newVal, int * dist)
{
	return _Snap(m_BottomEdges, m_CurrentRect.top, newVal, dist);
}               

bool SnapTool::_SnapBottom(int * newVal, int * dist)
{
	return _Snap(m_TopEdges, m_CurrentRect.bottom, newVal, dist);
}

bool SnapTool::_Snap(const std::vector<int>& edges, int val, int * newVal, int * dist)
{
	bool ret = false;
	for ( auto edge : edges )
	{
		int d = std::abs(val - edge);
		if ( d < SNAP_PIXELS && (!ret || d < *dist) )
		{
			*dist = d;
			*newVal = edge;
			ret = true;
		}
	}
	return ret;
}
void SnapTool::AddSnapRect(RECT rect)
{
	m_LeftEdges.push_back(rect.left);
	m_RightEdges.push_back(rect.right);
	m_TopEdges.push_back(rect.top);
	m_BottomEdges.push_back(rect.bottom);
	m_LeftEdges.push_back(rect.right);
	m_RightEdges.push_back(rect.left);
	m_TopEdges.push_back(rect.bottom);
	m_BottomEdges.push_back(rect.top);
}