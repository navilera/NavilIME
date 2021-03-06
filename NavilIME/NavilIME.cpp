// NavilIME.cpp : Defines the exported functions for the DLL application.
//

#include "Global.h"
#include "NavilIME.h"
#include "Pathcch.h"

extern "C"
{
#include "libhangul/hangul.h"
}

NavilIME gNavilIME;

static HangulInputContext* _hangulHnd;

NavilIME::NavilIME()
{
	_IsHangulModeOn = false;
	_hangulHnd = NULL;
}

void NavilIME::HangulNew()
{
	WCHAR achFileName[MAX_PATH];

	GetModuleFileNameW(NULL, achFileName, ARRAYSIZE(achFileName));
	DebugLogFile(L"Handler File Name: %s\n", achFileName);

	GetModuleFileNameW(gHandleDllInstance, achFileName, ARRAYSIZE(achFileName));
	DebugLogFile(L"DLL File Name: %s\n", achFileName);

	WCHAR keyboardConfigFileName[] = L"\\Keyboard.dat";
	PathCchRemoveFileSpec((PWSTR)achFileName, MAX_PATH);
	wcsncat(achFileName, keyboardConfigFileName, lstrlenW(keyboardConfigFileName));
	DebugLogFile(L"Keyboard configuration file: %s\n", achFileName);

	char buff[256] = { 0 };
	FILE* fp = _wfopen(achFileName, L"r");
	if (fp != nullptr) {
		fread(buff, 256, 1, fp);
		fclose(fp);
	}
	else {
		DebugLogFile(L"%s\n", L"Config file read fail");
		sprintf(buff, "%s", "318Na");
	}

	
	wchar_t text_wchar[256];
	size_t num;
	mbstowcs_s(&num, text_wchar, buff, 256);
	DebugLogFile(L"Keyboard Type: %s\n", text_wchar);
	
	for (int i = 0; i < strlen(buff); i++)
	{
		if (buff[i] == 0x0D) { buff[i] = 0x00; }
		if (buff[i] == 0x0A) { buff[i] = 0x00; }
	}

	const char* hangulType = buff;// "318Na";
	_hangulHnd = hangul_ic_new(hangulType);
	hangul_ic_set_option(_hangulHnd, HANGUL_IC_OPTION_AUTO_REORDER, true);
}

NavilIME::~NavilIME()
{
	DebugLogFile(L"%s\n", L"Destruct Navil IME");
	hangul_ic_delete(_hangulHnd);
}

bool NavilIME::HangulProcess(int ascii)
{
	return hangul_ic_process(_hangulHnd, ascii);
}

bool NavilIME::HangulBackspace()
{
	return hangul_ic_backspace(_hangulHnd);
}

UINT NavilIME::HangulGetPreedit(UINT idx)
{
	const uint32_t* ret = hangul_ic_get_preedit_string(_hangulHnd);
	return ret[idx];
}

UINT NavilIME::HangulGetCommit(UINT idx)
{
	const uint32_t* ret = hangul_ic_get_commit_string(_hangulHnd);
	return ret[idx];
}

UINT NavilIME::HangulFlush()
{
	const uint32_t* ret = hangul_ic_flush(_hangulHnd);
	return ret[0];
}

bool NavilIME::GetHangulMode()
{
	return _IsHangulModeOn;
}

bool NavilIME::SetHangulMode(bool mode)
{
	_IsHangulModeOn = mode;
	return GetHangulMode();
}

void NavilIME::ToggleHangulMode()
{
	SetHangulMode(GetHangulMode() ? false : true);
}