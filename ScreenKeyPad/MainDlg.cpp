// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"
#include "KeyDefine.h"
CMainDlg * CMainDlg::instance = nullptr;
BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	instance = this;
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	m_keyWindowList = GetDlgItem(IDC_KEY_WINDOW_LIST);
	m_keyList = GetDlgItem(IDC_KEY_LIST);
	std::vector<UINT> knownKeys;
	FillKnownVirtualKey(knownKeys);
	for ( auto key : knownKeys )
	{
		m_keyList.InsertItem(CBEIF_TEXT | CBEIF_LPARAM, -1, (LPCTSTR)KeyToString(key), 0, 0, 0, 0, (LPARAM)key);
	}

	AddKeyWindow();

	return TRUE;
}

void CMainDlg::AddKeyWindow()
{
	KeyWindow * window = new KeyWindow();
	window->Create(VK_SPACE);
	window->SetMoveResizeMode(m_MoveMode);
	m_AllKeyWindows.push_back(window);

	m_keyWindowList.AddItem((UINT)(CBEIF_TEXT | CBEIF_LPARAM), (LPCTSTR)LPSTR_TEXTCALLBACK, 0, 0, 0, 0, (LPARAM)window);
	m_CurrentSelected = (int)m_AllKeyWindows.size() - 1;
	this->_RefreshCurrentSelected();
	//this->SetActiveWindow();
}

void CMainDlg::RemoveKeyWindow()
{
	if ( m_CurrentSelected >= 0 && m_CurrentSelected < (int)m_AllKeyWindows.size() )
	{
		m_AllKeyWindows[m_CurrentSelected]->Close();
		delete m_AllKeyWindows[m_CurrentSelected];
		m_AllKeyWindows.erase(m_AllKeyWindows.begin() + m_CurrentSelected);
		m_keyWindowList.DeleteItem(m_CurrentSelected);
		this->_RefreshCurrentSelected();
	}
}

void CMainDlg::_RefreshCurrentSelected()
{
	CComboBox winList = GetDlgItem(IDC_KEY_WINDOW_LIST);
	winList.SetCurSel(m_CurrentSelected);
	KeyWindow * window = nullptr;
	if ( m_CurrentSelected >= 0 && m_CurrentSelected < (int)m_AllKeyWindows.size() )
		window = m_AllKeyWindows[m_CurrentSelected];
	if ( window )
	{
		UINT key = window->GetKey();
		int count = m_keyList.GetCount();
		m_keyList.SetCurSel(-1);
		for ( int i = 0; i < count; ++i )
		{
			if ( m_keyList.GetItemData(i) == key )
			{
				m_keyList.SetCurSel(i);
				break;
			}
		}

	}
	for ( int i = 0; i < (int)m_AllKeyWindows.size(); ++i )
	{
		m_AllKeyWindows[i]->SetSelected(i == m_CurrentSelected);
	}
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	for ( auto p : m_AllKeyWindows )
	{
		p->Close();
		delete p;
	}
	m_AllKeyWindows.clear();

	return 0;
}


LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}



void CMainDlg::OnClickMoveMode(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CButton btn = wndCtl;
	//btn.SetCheck(!btn.GetCheck());
	auto status = btn.GetCheck();
	if ( status == BST_CHECKED )
	{
		btn.SetCheck(BST_UNCHECKED);
		m_MoveMode = FALSE;
	}
	else
	{
		btn.SetCheck(BST_CHECKED);
		m_MoveMode = TRUE;
	}
	for ( auto p : m_AllKeyWindows )
	{
		p->SetMoveResizeMode(m_MoveMode);
	}
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}



LRESULT CMainDlg::OnCbenGetdispinfoKeyWindowList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	PNMCOMBOBOXEX pCBEx = reinterpret_cast<PNMCOMBOBOXEX>(pNMHDR);
	// TODO: Add your control notification handler code here
	if ( pCBEx->ceItem.mask & CBEIF_TEXT )
	{
		KeyWindow * window = (KeyWindow*)pCBEx->ceItem.lParam;
		pCBEx->ceItem.pszText = (LPWSTR)window->GetWindowTitle();
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedRemove(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	RemoveKeyWindow();
	return 0;
}


LRESULT CMainDlg::OnBnClickedAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	AddKeyWindow();
	return 0;
}


LRESULT CMainDlg::OnCbnSelchangeKeyList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	KeyWindow * window = nullptr;
	if ( m_CurrentSelected >= 0 && m_CurrentSelected < (int)m_AllKeyWindows.size() )
		window = m_AllKeyWindows[m_CurrentSelected];
	if ( window )
	{
		int index = m_keyList.GetCurSel();
		if ( index >= 0 )
		{
			UINT key = (UINT)m_keyList.GetItemData(index);
			window->SetKey(key);
		}
	}   
	return 0;
}


LRESULT CMainDlg::OnCbnSelchangeKeyWindowList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_CurrentSelected = m_keyWindowList.GetCurSel();
	this->_RefreshCurrentSelected();
	return 0;
}
