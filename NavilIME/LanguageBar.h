#pragma once
class LanguageBar : public ITfLangBarItemButton,
	public ITfSource
{
public:
	LanguageBar(REFGUID guidLangBar, LPCWSTR description, LPCWSTR tooltip, DWORD onIconIndex, DWORD offIconIndex);
	~LanguageBar();

	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, _Outptr_ void **ppvObj);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	// ITfLangBarItem
	STDMETHODIMP GetInfo(_Out_ TF_LANGBARITEMINFO *pInfo);
	STDMETHODIMP GetStatus(_Out_ DWORD *pdwStatus);
	STDMETHODIMP Show(BOOL fShow);
	STDMETHODIMP GetTooltipString(_Out_ BSTR *pbstrToolTip);

	// ITfLangBarItemButton
	STDMETHODIMP OnClick(TfLBIClick click, POINT pt, _In_ const RECT *prcArea);
	STDMETHODIMP InitMenu(_In_ ITfMenu *pMenu);
	STDMETHODIMP OnMenuSelect(UINT wID);
	STDMETHODIMP GetIcon(_Out_ HICON *phIcon);
	STDMETHODIMP GetText(_Out_ BSTR *pbstrText);

	// ITfSource
	STDMETHODIMP AdviseSink(__RPC__in REFIID riid, __RPC__in_opt IUnknown *punk, __RPC__out DWORD *pdwCookie);
	STDMETHODIMP UnadviseSink(DWORD dwCookie);
	
public:
	void SetStatus(DWORD dwStatus, BOOL fSet);
	void ToggleStatus();

private:
	ITfLangBarItemSink* _pLangBarItemSink;
	TF_LANGBARITEMINFO _tfLangBarItemInfo;

	LPCWSTR _pTooltipText;
	DWORD _onIconIndex;
	DWORD _offIconIndex;

	DWORD _status;

	// The cookie for the sink to CLangBarItemButton.
	static const DWORD _cookie = 0xD0C0FFEE;

	LONG _refCount;

	/*
	CCompartment* _pCompartment;
	CCompartmentEventSink* _pCompartmentEventSink;
	static HRESULT _CompartmentCallback(_In_ void *pv, REFGUID guidCompartment);
	*/
};

