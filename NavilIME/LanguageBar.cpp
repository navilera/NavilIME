#include "Global.h"
#include "TextService.h"
#include "LanguageBar.h"
#include "Server.h"
#include "NavilIME.h"

/**********************************************
TextService Functions.. Not LanguageBar function (Notice)
***********************************************/
const WCHAR LangbarImeModeDescription[] = L"Hangul Turn On/Off";
const WCHAR ImeModeDescription[] = L"Hangul/English input (Shift+Space)";

bool TextService::_InitLangBarItemButton()
{
	HRESULT hr = S_OK;
	ITfLangBarItemMgr* pLangBarItemMgr = nullptr;

	if (_pHangulTurnOnOffStatus != nullptr)
	{
		return true;
	}
	else
	{
		_pHangulTurnOnOffStatus = new LanguageBar(GUID_LBI_INPUTMODE, LangbarImeModeDescription, ImeModeDescription, IDI_ICON_HANGUL_ON, IDI_ICON_HANGUL_OFF);
	}

	hr = _pThreadMgr->QueryInterface(IID_ITfLangBarItemMgr, (void **)&pLangBarItemMgr);
	if (SUCCEEDED(hr))
	{
		hr = pLangBarItemMgr->AddItem(_pHangulTurnOnOffStatus);
		pLangBarItemMgr->Release();
	}

	return (hr == S_OK);
}

void TextService::_UnInitLangBarItemButton()
{
	HRESULT hr = S_OK;
	ITfLangBarItemMgr* pLangBarItemMgr = nullptr;

	if (_pHangulTurnOnOffStatus == nullptr)
	{
		return;
	}

	hr = _pThreadMgr->QueryInterface(IID_ITfLangBarItemMgr, (void **)&pLangBarItemMgr);
	if (SUCCEEDED(hr))
	{
		hr = pLangBarItemMgr->RemoveItem(_pHangulTurnOnOffStatus);
		if (SUCCEEDED(hr))
		{
			delete _pHangulTurnOnOffStatus;
			_pHangulTurnOnOffStatus = nullptr;
		}
		pLangBarItemMgr->Release();
	}
}

/*
* START LanguageBar Functions
*/


LanguageBar::LanguageBar(REFGUID guidLangBar, LPCWSTR description, LPCWSTR tooltip, DWORD onIconIndex, DWORD offIconIndex)
{
	DWORD bufLen = 0;

	Server_DllAddRef();

	// initialize TF_LANGBARITEMINFO structure.
	_tfLangBarItemInfo.clsidService = cNavilIME_CLSID;															// This LangBarItem belongs to this TextService.
	_tfLangBarItemInfo.guidItem = guidLangBar;															        // GUID of this LangBarItem.
	_tfLangBarItemInfo.dwStyle = (TF_LBI_STYLE_BTN_BUTTON | TF_LBI_STYLE_SHOWNINTRAY);							// This LangBar is a button type.
	_tfLangBarItemInfo.ulSort = 0;																			    // The position of this LangBar Item is not specified.
	StringCchCopy(_tfLangBarItemInfo.szDescription, ARRAYSIZE(_tfLangBarItemInfo.szDescription), description);  // Set the description of this LangBar Item.

	// Initialize the sink pointer to NULL.
	_pLangBarItemSink = nullptr;

	// Initialize ICON index and file name.
	_onIconIndex = onIconIndex;
	_offIconIndex = offIconIndex;

	/*
	// Initialize compartment.
	_pCompartment = nullptr;
	_pCompartmentEventSink = nullptr;
	*/

	_status = 0;

	_refCount = 1;

	// Initialize Tooltip
	_pTooltipText = nullptr;
	if (tooltip)
	{
		size_t len = 0;
		if (StringCchLength(tooltip, STRSAFE_MAX_CCH, &len) != S_OK)
		{
			len = 0;
		}
		bufLen = static_cast<DWORD>(len) + 1;
		_pTooltipText = (LPCWSTR) new WCHAR[bufLen];
		if (_pTooltipText)
		{
			StringCchCopy((LPWSTR)_pTooltipText, bufLen, tooltip);
		}
	}
}

LanguageBar::~LanguageBar()
{
	Server_DllRelease();

	if (_pTooltipText)
	{
		delete[] _pTooltipText;
		_pTooltipText = nullptr;
	}
}

STDAPI LanguageBar::QueryInterface(REFIID riid, _Outptr_ void **ppvObj)
{
	if (ppvObj == nullptr)
	{
		return E_INVALIDARG;
	}

	*ppvObj = nullptr;

	if (IsEqualIID(riid, IID_IUnknown) ||
		IsEqualIID(riid, IID_ITfLangBarItem) ||
		IsEqualIID(riid, IID_ITfLangBarItemButton))
	{
		*ppvObj = (ITfLangBarItemButton *)this;
	}
	else if (IsEqualIID(riid, IID_ITfSource))
	{
		*ppvObj = (ITfSource *)this;
	}

	if (*ppvObj)
	{
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

STDAPI_(ULONG) LanguageBar::AddRef()
{
	return ++_refCount;
}

STDAPI_(ULONG) LanguageBar::Release()
{
	LONG cr = --_refCount;

	assert(_refCount >= 0);

	if (_refCount == 0)
	{
		delete this;
	}

	return cr;
}

STDAPI LanguageBar::GetInfo(_Out_ TF_LANGBARITEMINFO *pInfo)
{
	_tfLangBarItemInfo.dwStyle |= TF_LBI_STYLE_SHOWNINTRAY;
	*pInfo = _tfLangBarItemInfo;
	return S_OK;
}

STDAPI LanguageBar::GetStatus(_Out_ DWORD *pdwStatus)
{
	if (pdwStatus == nullptr)
	{
		E_INVALIDARG;
	}

	*pdwStatus = _status;
	return S_OK;
}

void LanguageBar::SetStatus(DWORD dwStatus, BOOL fSet)
{
	BOOL isChange = FALSE;

	if (fSet)
	{
		if (!(_status & dwStatus))
		{
			_status |= dwStatus;
			isChange = TRUE;
		}
	}
	else
	{
		if (_status & dwStatus)
		{
			_status &= ~dwStatus;
			isChange = TRUE;
		}
	}

	if (isChange && _pLangBarItemSink)
	{
		_pLangBarItemSink->OnUpdate(TF_LBI_STATUS | TF_LBI_ICON);
	}

	return;
}

STDAPI LanguageBar::Show(BOOL fShow)
{
	if (_pLangBarItemSink)
	{
		_pLangBarItemSink->OnUpdate(TF_LBI_STATUS);
	}
	return S_OK;
}

STDAPI LanguageBar::GetTooltipString(_Out_ BSTR *pbstrToolTip)
{
	*pbstrToolTip = SysAllocString(_pTooltipText);

	return (*pbstrToolTip == nullptr) ? E_OUTOFMEMORY : S_OK;
}

STDAPI LanguageBar::OnClick(TfLBIClick click, POINT pt, _In_ const RECT *prcArea)
{
	/*
	BOOL isOn = FALSE;
	_pCompartment->_GetCompartmentBOOL(isOn);
	_pCompartment->_SetCompartmentBOOL(isOn ? FALSE : TRUE);
	*/
	DebugLogFile(L"%s\n", L"LanguageBar::OnClick");

	switch (click)
	{
	case TF_LBI_CLK_RIGHT:
		break;
	case TF_LBI_CLK_LEFT:
		ToggleStatus();
		break;
	default:
		break;
	}

	return S_OK;
}

STDAPI LanguageBar::InitMenu(_In_ ITfMenu *pMenu)
{
	DebugLogFile(L"%s - %p\n", L"LanguageBar::InitMenu", pMenu);

	if (pMenu == nullptr)
	{
		return E_INVALIDARG;
	}

#if 0	//  InitMenu는 왜 인지 모르겠지만 동작하지 않는다. 방법을 모르겠음.
	ImeMenu* pImeMenu = gNavilIME.GetImeMenu();
	for (int i = 0; i < IME_MENUNUM_MAX_NUM; i++)
	{
		if (pImeMenu[i].id == -1) {
			break;
		}

		pMenu->AddMenuItem(pImeMenu[i].id, 
			TF_LBMENUF_CHECKED,
			nullptr, 
			nullptr, 
			pImeMenu[i].text, 
			(ULONG)wcslen(pImeMenu[i].text), 
			nullptr);
	}
	pMenu->AddMenuItem(1001,
		TF_LBMENUF_CHECKED,
		nullptr,
		nullptr,
		L"adsfef",
		(ULONG)wcslen(L"adsfef"),
		nullptr);
	pMenu->AddMenuItem(1002,
		TF_LBMENUF_CHECKED,
		nullptr,
		nullptr,
		L"adsfefkkk",
		(ULONG)wcslen(L"adsfefkkk"),
		nullptr);

#endif
	

	return S_OK;
}

STDAPI LanguageBar::OnMenuSelect(UINT wID)
{
	return S_OK;
}

STDAPI LanguageBar::GetIcon(_Out_ HICON *phIcon)
{
	/*
	BOOL isOn = FALSE;
	if (!_pCompartment)
	{
		return E_FAIL;
	}
	*/
	if (!phIcon)
	{
		return E_FAIL;
	}
	*phIcon = nullptr;

	//_pCompartment->_GetCompartmentBOOL(isOn);

	DWORD status = 0;
	GetStatus(&status);

	// If IME is working on the UAC mode, the size of ICON should be 24 x 24.
	int desiredSize = 24;// 16;
	/*
	if (_isSecureMode) // detect UAC mode
	{
		desiredSize = _isSecureMode ? 24 : 16;
	}
	*/

	//if (isOn && !(status & TF_LBI_STATUS_DISABLED))
	if (!(status & TF_LBI_STATUS_BTN_TOGGLED))
	{
		if (gHandleDllInstance)
		{
			*phIcon = reinterpret_cast<HICON>(LoadImage(gHandleDllInstance, MAKEINTRESOURCE(_onIconIndex), IMAGE_ICON, desiredSize, desiredSize, 0));
			gNavilIME.SetHangulMode(true);
		}
	}
	else
	{
		if (gHandleDllInstance)
		{
			*phIcon = reinterpret_cast<HICON>(LoadImage(gHandleDllInstance, MAKEINTRESOURCE(_offIconIndex), IMAGE_ICON, desiredSize, desiredSize, 0));
			gNavilIME.SetHangulMode(false);
		}
	}

	return (*phIcon != NULL) ? S_OK : E_FAIL;
}

void LanguageBar::ToggleStatus()
{
	DWORD status = 0;
	GetStatus(&status);

	SetStatus(TF_LBI_STATUS_BTN_TOGGLED, ((status & TF_LBI_STATUS_BTN_TOGGLED) ? FALSE : TRUE));
}

STDAPI LanguageBar::GetText(_Out_ BSTR *pbstrText)
{
	*pbstrText = SysAllocString(_tfLangBarItemInfo.szDescription);

	return (*pbstrText == nullptr) ? E_OUTOFMEMORY : S_OK;
}

STDAPI LanguageBar::AdviseSink(__RPC__in REFIID riid, __RPC__in_opt IUnknown *punk, __RPC__out DWORD *pdwCookie)
{
	DebugLogFile(L"%s\n", L"LanguageBar::AdviseSink");

	// We allow only ITfLangBarItemSink interface.
	if (!IsEqualIID(IID_ITfLangBarItemSink, riid))
	{
		return CONNECT_E_CANNOTCONNECT;
	}

	// We support only one sink once.
	if (_pLangBarItemSink != nullptr)
	{
		return CONNECT_E_ADVISELIMIT;
	}

	// Query the ITfLangBarItemSink interface and store it into _pLangBarItemSink.
	if (punk == nullptr)
	{
		return E_INVALIDARG;
	}
	if (punk->QueryInterface(IID_ITfLangBarItemSink, (void **)&_pLangBarItemSink) != S_OK)
	{
		_pLangBarItemSink = nullptr;
		return E_NOINTERFACE;
	}

	// return our cookie.
	*pdwCookie = _cookie;
	return S_OK;
}

STDAPI LanguageBar::UnadviseSink(DWORD dwCookie)
{
	// Check the given cookie.
	if (dwCookie != _cookie)
	{
		return CONNECT_E_NOCONNECTION;
	}

	// If there is nno connected sink, we just fail.
	if (_pLangBarItemSink == nullptr)
	{
		return CONNECT_E_NOCONNECTION;
	}

	_pLangBarItemSink->Release();
	_pLangBarItemSink = nullptr;

	return S_OK;
}