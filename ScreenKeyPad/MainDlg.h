// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "KeyWindow.h"
#include <vector>
#include "resource.h"
#define WM_NOTIFY_ICON_MESSAGE (WM_USER+1)
class KeyWindow;
class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
	
	int m_CurrentSelected = -1;
	BOOL m_MoveMode = FALSE;
	void _RefreshCurrentSelected();

	CComboBoxEx m_keyWindowList;
	CComboBoxEx m_keyList;

	void _ShowNotifyIcon();
	void _HideNotifyIcon();
	void _Show();
	void _Hide();
	void _ExitMoveMode();
public:
	static CMainDlg * instance;
	std::vector<KeyWindow*> m_AllKeyWindows;

	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_NOTIFY_ICON_MESSAGE,OnNotifyIconMessage)
		MSG_WM_CLOSE(OnClose)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER_EX(IDC_MOVE_MODE,BN_CLICKED,OnClickMoveMode)
		NOTIFY_HANDLER(IDC_KEY_WINDOW_LIST, CBEN_GETDISPINFO, OnCbenGetdispinfoKeyWindowList)
		COMMAND_HANDLER(IDC_REMOVE, BN_CLICKED, OnBnClickedRemove)
		COMMAND_HANDLER(IDC_ADD, BN_CLICKED, OnBnClickedAdd)
		COMMAND_HANDLER(IDC_KEY_LIST, CBN_SELCHANGE, OnCbnSelchangeKeyList)
		COMMAND_HANDLER(IDC_KEY_WINDOW_LIST, CBN_SELCHANGE, OnCbnSelchangeKeyWindowList)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNotifyIconMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void OnClose();
	void OnClickMoveMode(UINT uNotifyCode, int nID, CWindow wndCtl);
	void CloseDialog(int nVal);

	void AddKeyWindow();
	void RemoveKeyWindow();
	LRESULT OnCbenGetdispinfoKeyWindowList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRemove(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCbnSelchangeKeyList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCbnSelchangeKeyWindowList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
