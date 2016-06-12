#include "stdafx.h"
#include "KeyDefine.h"

struct _Vk_To_String
{
	WORD key;
	const WCHAR * str;
};
#define DEFINE_VK(key,str) {key,str},
static _Vk_To_String _default_mappings[] = {
	DEFINE_VK(VK_NUMPAD0,L"0")
	DEFINE_VK(VK_NUMPAD1,L"1")
	DEFINE_VK(VK_NUMPAD2,L"2")
	DEFINE_VK(VK_NUMPAD3,L"3")
	DEFINE_VK(VK_NUMPAD4,L"4")
	DEFINE_VK(VK_NUMPAD5,L"5")
	DEFINE_VK(VK_NUMPAD6,L"6")
	DEFINE_VK(VK_NUMPAD7,L"7")
	DEFINE_VK(VK_NUMPAD8,L"8")
	DEFINE_VK(VK_NUMPAD9,L"9")
	DEFINE_VK(VK_MULTIPLY     ,L"*")
	DEFINE_VK(VK_ADD		  ,L"+")
	DEFINE_VK(VK_SUBTRACT	  ,L"-")
	DEFINE_VK(VK_DIVIDE		  ,L"/")

	DEFINE_VK(VK_F1   ,L"F1")
	DEFINE_VK(VK_F2	  ,L"F2")
	DEFINE_VK(VK_F3	  ,L"F3")
	DEFINE_VK(VK_F4	  ,L"F4")
	DEFINE_VK(VK_F5	  ,L"F5")
	DEFINE_VK(VK_F6	  ,L"F6")
	DEFINE_VK(VK_F7	  ,L"F7")
	DEFINE_VK(VK_F8	  ,L"F8")
	DEFINE_VK(VK_F9	  ,L"F9")
	DEFINE_VK(VK_F10  ,L"F10")
	DEFINE_VK(VK_F11  ,L"F11")
	DEFINE_VK(VK_F12  ,L"F12")

	DEFINE_VK(VK_LSHIFT   ,L"[L]SHIFT")
	DEFINE_VK(VK_RSHIFT	  ,L"[R]SHIFT")
	DEFINE_VK(VK_LCONTROL ,L"[L]CTRL")
	DEFINE_VK(VK_RCONTROL ,L"[R]CTRL")
	DEFINE_VK(VK_LMENU	  ,L"[L]ALT")
	DEFINE_VK(VK_RMENU 	  ,L"[R]ALT")

	DEFINE_VK(VK_SPACE,L"SPACE")
	DEFINE_VK(VK_PRIOR,L".")
	DEFINE_VK(VK_END,L"END")
	DEFINE_VK(VK_HOME,L"HOME")
	DEFINE_VK(VK_LEFT,L"LEFT")
	DEFINE_VK(VK_UP,L"UP")
	DEFINE_VK(VK_RIGHT,L"RIGHT")
	DEFINE_VK(VK_DOWN,L"DOWN")
	DEFINE_VK(VK_ESCAPE,L"ESC")
	DEFINE_VK(VK_RETURN,L"RETURN")

	DEFINE_VK(VK_LBUTTON,L"LMB")
	DEFINE_VK(VK_RBUTTON,L"RMB")
	DEFINE_VK(VK_MBUTTON,L"MMB")

};

const WCHAR * KeyToString(UINT vk)
{
	static CString str;
	if ( vk >= 'A' && vk <= 'Z' )
	{
		str.Format(L"%c", L'A' + (vk - 'A'));
	}
	else
	{
		for ( auto&& item : _default_mappings )
		{
			if ( item.key == vk )
			{
				return item.str;
			}
		}
		str.Format(L"0x02X", vk);
	}

	return str;
}