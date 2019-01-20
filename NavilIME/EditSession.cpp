#include "Global.h"
#include "EditSession.h"
#include "TextService.h"

EditSession::EditSession(TextService *pTextService)
{
	_pContext = nullptr;
	_pTextService = pTextService;
	_pTextService->AddRef();

	_cRef = 1;	
}

EditSession::~EditSession()
{
	if (_pContext != nullptr) {
		_pContext->Release();
	}
	_pTextService->Release();
}

// IUnknown
STDMETHODIMP EditSession::QueryInterface(REFIID riid, void **ppvObj) 
{
	if (ppvObj == NULL)
	{
		return E_INVALIDARG;
	}

	*ppvObj = NULL;

	if (IsEqualIID(riid, IID_IUnknown) ||
		IsEqualIID(riid, IID_ITfEditSession))
	{
		*ppvObj = (ITfEditSession *)this;
	}

	if (*ppvObj) {
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) EditSession::AddRef(void) 
{
	return ++_cRef;
}

STDMETHODIMP_(ULONG) EditSession::Release(void) 
{
	LONG cr = --_cRef;

	assert(_cRef >= 0);

	if (_cRef == 0)
	{
		delete this;
	}

	return cr;
}

// ITfEditSession
STDMETHODIMP EditSession::DoEditSession(TfEditCookie ec)
{
	return _pTextService->HandleCompositionKey(ec, _pContext, _wParam);
}

void EditSession::SetPContext(ITfContext *i, WPARAM wparam)
{
	_pContext = i;
	_pContext->AddRef();

	_wParam = wparam;
}


StartCompositionEditSession::StartCompositionEditSession(TextService* pTxtSvc, ITfContext* pCtx) : EditSession(pTxtSvc)
{
	SetPContext(pCtx, 0);
}

// ITfEditSession
STDMETHODIMP StartCompositionEditSession::DoEditSession(TfEditCookie ec)
{
	ITfInsertAtSelection *pInsertAtSelection = NULL;
	ITfRange *pRangeInsert = NULL;
	ITfContextComposition *pContextComposition = NULL;
	ITfComposition *pComposition = NULL;
	HRESULT hr = E_FAIL;

	// insert the text
	WCHAR ch = (WCHAR)_wParam;

	DebugLogFile(L"%s EC:%lu\n", L"StartCompositionEditSession::DoEditSession", ec);

	// A special interface is required to insert text at the selection
	if (_pContext->QueryInterface(IID_ITfInsertAtSelection, (void **)&pInsertAtSelection) != S_OK)
	{
		goto Exit;
	}

	
	if (pInsertAtSelection->InsertTextAtSelection(ec, 0, &ch, 1, &pRangeInsert) != S_OK)
	//if (pInsertAtSelection->InsertTextAtSelection(ec, TF_IAS_QUERYONLY, NULL, 0, &pRangeInsert) != S_OK)
	{
		goto Exit;
	}

	// get an interface on the context to deal with compositions
	if (_pContext->QueryInterface(IID_ITfContextComposition, (void **)&pContextComposition) != S_OK)
	{
		goto Exit;
	}

	// start the new composition
	if ((pContextComposition->StartComposition(ec, pRangeInsert, _pTextService, &pComposition) == S_OK) && (pComposition != NULL))
	{
		DebugLogFile(L"\t->%s\n", L"StartComposition OK");
		// Store the pointer of this new composition object in the instance 
		// of the CTextService class. So this instance of the CTextService 
		// class can know now it is in the composition stage.
		_pTextService->SetComposition(pComposition);

		// 
		//  set selection to the adjusted range
		//
		TF_SELECTION tfSelection;
		tfSelection.range = pRangeInsert;
		tfSelection.style.ase = TF_AE_NONE;
		tfSelection.style.fInterimChar = true;
		_pContext->SetSelection(ec, 1, &tfSelection);
	}

Exit:
	if (pContextComposition != NULL)
		pContextComposition->Release();

	if (pRangeInsert != NULL)
		pRangeInsert->Release();

	if (pInsertAtSelection != NULL)
		pInsertAtSelection->Release();

	return S_OK;
}

EndCompositionEditSession::EndCompositionEditSession(TextService* pTxtSvc, ITfContext* pCtx) : EditSession(pTxtSvc)
{
	SetPContext(pCtx, 0);
}

// ITfEditSession
STDMETHODIMP EndCompositionEditSession::DoEditSession(TfEditCookie ec)
{
	DebugLogFile(L"%s EC:%lu\n", L"EndCompositionEditSession::DoEditSession", ec);

	_pTextService->TerminateComposition(ec);
	return S_OK;
}
