#include "Global.h"
#include "TextService.h"
#include "LanguageBar.h"
#include "EditSession.h"

// {178A04CC-7A3E-47B9-99FE-C62D2F4CA20A}
static const GUID cPreservedKey_GUID_ToggleHangul =
{ 0x178a04cc, 0x7a3e, 0x47b9,{ 0x99, 0xfe, 0xc6, 0x2d, 0x2f, 0x4c, 0xa2, 0xa } };

/* Set Shift+Space as on off key */
static const TF_PRESERVEDKEY cShiftSpace = { VK_SPACE, TF_MOD_SHIFT };
static const TF_PRESERVEDKEY cHangul = { VK_HANGUL, TF_MOD_IGNORE_ALL_MODIFIER };
static const WCHAR ToggleDesc[] = L"Hangul Toggle";

bool TextService::_InitKeyEventSink()
{
	DebugLogFile(L"%s\n", L"TextService::_InitKeyEventSink");

	ITfKeystrokeMgr* pKeystrokeMgr = nullptr;
	HRESULT hr = S_OK;

	if (FAILED(_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr)))
	{
		DebugLogFile(L"\t%s\n", L"KeystrokeMgr fail");
		return false;
	}

	hr = pKeystrokeMgr->AdviseKeyEventSink(_ClientId, (ITfKeyEventSink *)this, TRUE);

	pKeystrokeMgr->Release();

	return (hr == S_OK);
}

void TextService::_UninitKeyEventSink()
{
	DebugLogFile(L"%s\n", L"TextService::_UninitKeyEventSink");

	ITfKeystrokeMgr* pKeystrokeMgr = nullptr;

	if (FAILED(_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr)))
	{
		return;
	}

	pKeystrokeMgr->UnadviseKeyEventSink(_ClientId);

	pKeystrokeMgr->Release();
}

bool TextService::_InitPreservedKey() 
{
	DebugLogFile(L"%s\n", L"TextService::_InitPreservedKey");


	ITfKeystrokeMgr *pKeystrokeMgr;
	HRESULT hr;

	if (_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr) != S_OK) {
		return false;
	}

	// register Hangul
	hr = pKeystrokeMgr->PreserveKey(_ClientId,
		cPreservedKey_GUID_ToggleHangul,
		&cHangul,
		ToggleDesc,
		(ULONG)wcslen(ToggleDesc));

	if (hr == S_OK) {
		// register Shift + Space
		hr = pKeystrokeMgr->PreserveKey(_ClientId,
			cPreservedKey_GUID_ToggleHangul,
			&cShiftSpace,
			ToggleDesc,
			(ULONG)wcslen(ToggleDesc));
	}

	pKeystrokeMgr->Release();

	return (hr == S_OK);
}

void TextService::_UninitPreservedKey() 
{
	DebugLogFile(L"%s\n", L"TextService::_UninitPreservedKey");

	ITfKeystrokeMgr *pKeystrokeMgr;

	if (_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr) != S_OK) {
		return;
	}

	pKeystrokeMgr->UnpreserveKey(cPreservedKey_GUID_ToggleHangul, &cShiftSpace);
	pKeystrokeMgr->UnpreserveKey(cPreservedKey_GUID_ToggleHangul, &cHangul);

	pKeystrokeMgr->Release();
}

WORD TextService::_ConvertVKeyToAscii(UINT code)
{
	//
	// Map virtual key to scan code
	//
	UINT scanCode = 0;
	scanCode = MapVirtualKey(code, 0);

	//
	// Keyboard state
	//
	BYTE abKbdState[256] = { '\0' };
	if (!GetKeyboardState(abKbdState))
	{
		return 0;
	}

	//
	// Map virtual key to character code
	//
	WORD wch = '\0';
	if (ToAscii(code, scanCode, abKbdState, &wch, 0) == 1)
	{
		return wch;
	}

	return 0;
}

void TextService::_Automata(UINT key)
{
	WORD code = _ConvertVKeyToAscii((UINT)key);
	DebugLogFile(L"%s %x\n", L"TextService::_Automata (ascii)", code);
	
	_keyEaten = false;

	if (gNavilIME.GetHangulMode() == false)
	{
		_keyEaten = false;
		return;
	}

	if (key == VK_SHIFT)
	{
		_keyEaten = true;
	}

	if (key == VK_BACK)
	{
		DebugLogFile(L"\t%s\n", L"BACKSPACE");
		_keyEaten = true;
		gNavilIME.HangulBackspace();
		if (gNavilIME.HangulGetPreedit() == 0)
		{
			// 자모를 다 지우고 libhangul 버퍼가 비어 있는 상황
			_keyEaten = false;
		}
		return;
	}

	if (gNavilIME.HangulProcess((int)code))
	{
		DebugLogFile(L"\t%s %x\n", L"proc:", code);

		_keyEaten = true;

		UINT commit = gNavilIME.HangulGetCommit();
		if (commit != 0)
		{
			// 한글을 조합하다가 숫자나 기호를 입력하면 commit에는 한글이 있고 preedit은 0이다.
			UINT preedit = gNavilIME.HangulGetPreedit();
			if (preedit == 0) {
				_keyEaten = false;
			}
			// 숫자나 기호를 중간에 한글 없이 연속해서 입력하면 commit과 code가 값이 같다.
			if (commit == code) {
				_keyEaten = false;
			}
		}
	}
}

/*
ITfKeyEventSink interface functions
*/
// 키 입력 포커스를 얻었을 때 호출된다.
STDMETHODIMP TextService::OnSetFocus(BOOL fForeground)
{
	DebugLogFile(L"%s\n", L"TextService::OnSetFocus");
	return S_OK;
}

// Notepad는 TestKeyDown/Up이 먼저 호출되고 pIsEaten이 true 일 때만 KeyDown/Up이 호출된다.
// 그런데 Wordpad는 TestKeyDown/Up이 호출되지 않고 바로 KeyDown/Up이 호출된다. 난장판이구만..
STDMETHODIMP TextService::OnTestKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pIsEaten)
{
	DebugLogFile(L"%s\n", L"TextService::OnTestKeyDown");
	
	_Automata((UINT)wParam);
	*pIsEaten = _keyEaten;
	_testKeyHappened = true;

	if (*pIsEaten == false)
	{
		_EndComposition(pContext);
		gNavilIME.HangulFlush();
	}

	return S_OK;
}

// 키가 눌렸을 때
STDMETHODIMP TextService::OnKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pIsEaten)
{
	DebugLogFile(L"%s\n", L"TextService::OnKeyDown");

	if (_testKeyHappened == false)
	{
		OnTestKeyDown(pContext, wParam, lParam, pIsEaten);
		_testKeyHappened = false;

		if (*pIsEaten == false)
		{
			return S_OK;
		}
	}

	*pIsEaten = _keyEaten;

	UINT commit = gNavilIME.HangulGetCommit();
	if (commit != 0)
	{
		DebugLogFile(L"\t%s -> %x\n", L"commit", commit);
		_EndComposition(pContext);
		return S_OK;
	}
	UINT preedit = gNavilIME.HangulGetPreedit();
	if (preedit != 0) {
		DebugLogFile(L"\t%s -> %x\n", L"preedit", preedit);
		_HandleComposition(pContext, (WCHAR)preedit);
	}

	return S_OK;
}

STDMETHODIMP TextService::OnTestKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pIsEaten)
{
	DebugLogFile(L"%s\n", L"TextService::OnTestKeyUp");

	*pIsEaten = _keyEaten;

	return S_OK;
}

// 키가 올라올 때
STDMETHODIMP TextService::OnKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pIsEaten)
{
	DebugLogFile(L"%s\n", L"TextService::OnKeyUp");

	*pIsEaten = _keyEaten;

	if (*pIsEaten)
	{
		UINT commit = gNavilIME.HangulGetCommit();
		if (commit != 0)
		{
			UINT preedit = gNavilIME.HangulGetPreedit();
			if (preedit != 0) {
				DebugLogFile(L"\t%s -> %x\n", L"preedit (in KeyUp)", preedit);
				_HandleComposition(pContext, (WCHAR)preedit);
			}
		}
	}

	return S_OK;
}

// _InitPreservedKey에서 등록한 핫 키가 눌리면 호출된다
STDMETHODIMP TextService::OnPreservedKey(ITfContext *pContext, REFGUID rguid, BOOL *pIsEaten)
{
	DebugLogFile(L"%s\n", L"TextService::OnPreservedKey");

	if (IsEqualGUID(rguid, cPreservedKey_GUID_ToggleHangul)) {
		DebugLogFile(L"\t%s\n", L"-> toggle Hangul");
		_pHangulTurnOnOffStatus->ToggleStatus();

		/*
		BOOL fOpen = _IsKeyboardOpen();
		if (fOpen) {
			_pCandidateWindow->_DrawString(TEXT("off"));
			_SetKeyboardOpen(FALSE);
		}
		else {
			_pCandidateWindow->_DrawString(TEXT("on"));
			_SetKeyboardOpen(TRUE);
		}
		*/
		*pIsEaten = TRUE;
	}
	else {
		*pIsEaten = FALSE;
	}

	return S_OK;
}

