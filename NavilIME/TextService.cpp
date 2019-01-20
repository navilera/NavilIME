#include "Global.h"
#include "TextService.h"
#include "Server.h"
#include "EditSession.h"

HRESULT TextService::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObj)
{
	DebugLogFile(L"%s\n", L"TextService::CreateInstance");

	TextService *pTextService;
	HRESULT hr;

	if (ppvObj == nullptr) {
		return E_INVALIDARG;
	}

	*ppvObj = nullptr;

	if (pUnkOuter != nullptr) {
		return CLASS_E_NOAGGREGATION;
	}

	pTextService = new TextService();
	if (pTextService == nullptr) {
		return E_OUTOFMEMORY;
	}

	hr = pTextService->QueryInterface(riid, ppvObj);

	pTextService->Release();

	return hr;
}

TextService::TextService()
{
	DebugLogFile(L"%s\n", L"TextService::Constructor");

	Server_DllAddRef();

	_RefCount = 1;
	_pThreadMgr = nullptr;
	_ClientId = 0;
	_ActivateFlags = 0;
	_threadMgrEventSinkCookie = TF_INVALID_COOKIE;
	_pTextEditSinkContext = 0;
	_textEditSinkCookie = TF_INVALID_COOKIE;
	_pHangulTurnOnOffStatus = nullptr;
	_keyEaten = false;
	_testKeyHappened = false;
	_pComposition = NULL;
}


TextService::~TextService()
{
	DebugLogFile(L"%s\n", L"TextService::Destructor");

	Server_DllRelease();
}

/**********************************************
* IUNknown Interface
**********************************************/

STDAPI TextService::QueryInterface(REFIID riid, void **ppvObj)
{
	DebugLogFile(L"%s\n", L"TextService::QueriyInterface");

	if (ppvObj == nullptr)
	{
		return E_INVALIDARG;
	}

	*ppvObj = nullptr;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfTextInputProcessor)) {
		DebugLogFile(L"\t%s\n", L"TextService::QueriyInterface - Request ITfTextInputProcessor");
		*ppvObj = (ITfTextInputProcessor *)this;
	}
	else if (IsEqualIID(riid, IID_ITfThreadMgrEventSink)) {
		DebugLogFile(L"\t%s\n", L"TextService::QueriyInterface - Request ITfThreadMgrEventSink");
		*ppvObj = (ITfThreadMgrEventSink *)this;
	}
	else if (IsEqualIID(riid, IID_ITfTextEditSink)) {
		DebugLogFile(L"\t%s\n", L"TextService::QueriyInterface - Request ITfTextEditSink");
		*ppvObj = (ITfTextEditSink *)this;
	}
	else if (IsEqualIID(riid, IID_ITfKeyEventSink)) {
		DebugLogFile(L"\t%s\n", L"TextService::QueriyInterface - Request ITfKeyEventSink");
		*ppvObj = (ITfKeyEventSink *)this;
	}
	else if (IsEqualIID(riid, IID_ITfCompositionSink)) {
		DebugLogFile(L"\t%s\n", L"TextService::QueriyInterface - Request ITfCompositionSink");
		*ppvObj = (ITfCompositionSink *)this;
	}
	else if (IsEqualIID(riid, IID_ITfDisplayAttributeProvider)) {
		DebugLogFile(L"\t%s\n", L"TextService::QueriyInterface - Request ITfDisplayAttributeProvider");
		*ppvObj = (ITfDisplayAttributeProvider *)this;
	}
	else if (IsEqualIID(riid, IID_ITfTextInputProcessorEx)) {
		DebugLogFile(L"\t%s\n", L"TextService::QueriyInterface - Request ITfTextInputProcessorEx");
		*ppvObj = (ITfTextInputProcessorEx *)this;
	}
	else if (IsEqualIID(riid, IID_ITfActiveLanguageProfileNotifySink)) {
		DebugLogFile(L"\t%s (Not Imple)\n", L"TextService::QueriyInterface - Request ITfActiveLanguageProfileNotifySink");
		//*ppvObj = (ITfActiveLanguageProfileNotifySink *)this;
	} else if (IsEqualIID(riid, IID_ITfThreadFocusSink)) {
		DebugLogFile(L"\t%s (Not Imple)\n", L"TextService::QueriyInterface - Request ITfThreadFocusSink");
		//*ppvObj = (ITfThreadFocusSink *)this;
	} else if (IsEqualIID(riid, IID_ITfFunctionProvider)) {
		DebugLogFile(L"\t%s (Not Imple)\n", L"TextService::QueriyInterface - Request ITfFunctionProvider");
		//*ppvObj = (ITfFunctionProvider *)this;
	} else if (IsEqualIID(riid, IID_ITfFunction)) {
		DebugLogFile(L"\t%s (Not Imple)\n", L"TextService::QueriyInterface - Request ITfFunction");
		//*ppvObj = (ITfFunction *)this;
	} else if (IsEqualIID(riid, IID_ITfFnGetPreferredTouchKeyboardLayout)) {
		DebugLogFile(L"\t%s (Not Imple)\n", L"TextService::QueriyInterface - Request ITfFnGetPreferredTouchKeyboardLayout");
		//*ppvObj = (ITfFnGetPreferredTouchKeyboardLayout *)this;
	}
	else {
		DebugLogFile(L"\t%s\n", L"TextService::QueriyInterface - Requested GUID is NOT found");
	}

	if (*ppvObj) {
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

STDAPI_(ULONG) TextService::AddRef()
{
	DebugLogFile(L"%s\n", L"TextService::AddRef");

	_RefCount++;
	return _RefCount;
}

STDAPI_(ULONG) TextService::Release()
{
	_RefCount--;
	LONG cr = _RefCount;

	DebugLogFile(L"%s [count : %d]\n", L"TextService::Release", _RefCount);

	assert(_RefCount >= 0);

	if (_RefCount == 0)
	{
		delete this;
	}

	return cr;
}

/**********************************************
* ITfTextInputProcessor Interface
**********************************************/

STDAPI TextService::Activate(ITfThreadMgr *pThreadMgr, TfClientId tfClientId)
{
	DebugLogFile(L"%s\n", L"TextService::Activate");
	return ActivateEx(pThreadMgr, tfClientId, 0);
}

STDAPI TextService::Deactivate()
{
	DebugLogFile(L"%s\n", L"TextService::Deactivate");

	/*
	if (_pCompositionProcessorEngine)
	{
	delete _pCompositionProcessorEngine;
	_pCompositionProcessorEngine = nullptr;
	}

	ITfContext* pContext = _pContext;
	if (_pContext)
	{
	pContext->AddRef();
	_EndComposition(_pContext);
	}

	if (_pCandidateListUIPresenter)
	{
	delete _pCandidateListUIPresenter;
	_pCandidateListUIPresenter = nullptr;

	if (pContext)
	{
	pContext->Release();
	}

	_candidateMode = CANDIDATE_NONE;
	_isCandidateWithWildcard = FALSE;
	}

	_UninitFunctionProviderSink();

	_UninitThreadFocusSink();

	_UninitActiveLanguageProfileNotifySink();
	*/

	_InitTextEditSink(nullptr);

	_UninitThreadMgrEventSink();
	_UninitKeyEventSink();
	_UninitPreservedKey();
	_UnInitLangBarItemButton();

	/*
	CCompartment CompartmentKeyboardOpen(_pThreadMgr, _tfClientId, GUID_COMPARTMENT_KEYBOARD_OPENCLOSE);
	CompartmentKeyboardOpen._ClearCompartment();

	CCompartment CompartmentDoubleSingleByte(_pThreadMgr, _tfClientId, Global::SampleIMEGuidCompartmentDoubleSingleByte);
	CompartmentDoubleSingleByte._ClearCompartment();

	CCompartment CompartmentPunctuation(_pThreadMgr, _tfClientId, Global::SampleIMEGuidCompartmentPunctuation);
	CompartmentDoubleSingleByte._ClearCompartment();
	*/

	if (_pThreadMgr != nullptr)
	{
		_pThreadMgr->Release();
	}

	_ClientId = TF_CLIENTID_NULL;

	/*
	if (_pDocMgrLastFocused)
	{
	_pDocMgrLastFocused->Release();
	_pDocMgrLastFocused = nullptr;
	}
	*/

	return S_OK;
}

/**********************************************
* ITfTextInputProcessorEx Interface
**********************************************/

STDAPI TextService::ActivateEx(ITfThreadMgr *pThreadMgr, TfClientId tfClientId, DWORD dwFlags)
{
	DebugLogFile(L"%s [%p]\n", L"TextService::ActivateEx", pThreadMgr);

	_pThreadMgr = pThreadMgr;
	_pThreadMgr->AddRef();

	_ClientId = tfClientId;
	_ActivateFlags = dwFlags;

	if (!_InitThreadMgrEventSink())
	{
		Deactivate();
		return E_FAIL;
	}

	ITfDocumentMgr* pDocMgrFocus = nullptr;
	if (SUCCEEDED(_pThreadMgr->GetFocus(&pDocMgrFocus)) && (pDocMgrFocus != nullptr))
	{
		DebugLogFile(L"\t%s\n", L"ThreadMgr->GetFocus OK");
		_InitTextEditSink(pDocMgrFocus);
		pDocMgrFocus->Release();
	}

	
	if (!_InitKeyEventSink())
	{
		Deactivate();
		return E_FAIL;
	}

	if (!_InitPreservedKey())
	{
		Deactivate();
		return E_FAIL;
	}
	
	if (!_InitLangBarItemButton())
	{
		Deactivate();
		return E_FAIL;
	}
	

	/*
	if (!_InitActiveLanguageProfileNotifySink())
	{
	Deactivate();
	return E_FAIL;
	}

	if (!_InitThreadFocusSink())
	{
	Deactivate();
	return E_FAIL;
	}

	if (!_InitDisplayAttributeGuidAtom())
	{
	Deactivate();
	return E_FAIL;
	}

	if (!_InitFunctionProviderSink())
	{
	Deactivate();
	return E_FAIL;
	}

	if (!_AddTextProcessorEngine())
	{
	Deactivate();
	return E_FAIL;
	}
	*/

	return S_OK;
}