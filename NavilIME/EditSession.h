#pragma once

class TextService;

class EditSession : public ITfEditSession
{
public:
	EditSession(TextService *pTextService);
	virtual ~EditSession();

	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, _Outptr_ void **ppvObj);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	// ITfEditSession
	STDMETHODIMP DoEditSession(TfEditCookie ec);

	void SetPContext(ITfContext *i, WPARAM wparam);

protected:
	TextService* _pTextService;
	ITfContext* _pContext;
	WPARAM		_wParam;

protected:
	LONG _cRef;     // COM ref count
};

class StartCompositionEditSession : public EditSession
{
public:
	StartCompositionEditSession(TextService* pTxtSvc, ITfContext* pCtx);

	// ITfEditSession
	STDMETHODIMP DoEditSession(TfEditCookie ec);
};

class EndCompositionEditSession : public EditSession
{
public:
	EndCompositionEditSession(TextService* pTxtSvc, ITfContext* pCtx);

	// ITfEditSession
	STDMETHODIMP DoEditSession(TfEditCookie ec);
};

