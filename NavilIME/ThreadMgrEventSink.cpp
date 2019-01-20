#include "Global.h"
#include "TextService.h"

bool TextService::_InitThreadMgrEventSink()
{
	DebugLogFile(L"%s\n", L"TextService::_InitThreadMgrEventSink");

	ITfSource* pSource = nullptr;

	if (FAILED(_pThreadMgr->QueryInterface(IID_ITfSource, (void **)&pSource)))
	{
		return false;
	}

	if (FAILED(pSource->AdviseSink(IID_ITfThreadMgrEventSink, (ITfThreadMgrEventSink *)this, &_threadMgrEventSinkCookie)))
	{
		_threadMgrEventSinkCookie = TF_INVALID_COOKIE;
		pSource->Release();
		return false;
	}

	pSource->Release();
	return true;
}

void TextService::_UninitThreadMgrEventSink()
{
	DebugLogFile(L"%s\n", L"TextService::_UninitThreadMgrEventSink");

	ITfSource* pSource = nullptr;

	if (_threadMgrEventSinkCookie == TF_INVALID_COOKIE)
	{
		return;
	}

	if (SUCCEEDED(_pThreadMgr->QueryInterface(IID_ITfSource, (void **)&pSource)))
	{
		pSource->UnadviseSink(_threadMgrEventSinkCookie);
		pSource->Release();
	}

	_threadMgrEventSinkCookie = TF_INVALID_COOKIE;
}

/*
ITfThreadMgrEventSink interface functions
 */
// 첫 번째 입력기 컨택스트가 도큐먼트(아마 입력 대상이 되는 text editor 형태의 모든 컨트롤 일듯)로 푸시될 때 호출된다.
STDMETHODIMP TextService::OnInitDocumentMgr(_In_ ITfDocumentMgr *pDocMgr)
{
	DebugLogFile(L"%s\n", L"TextService::OnInitDocumentMgr");
	return E_NOTIMPL;
}

// 마지막 입력기 컨택스트가 도큐먼트에서 팝될 때 호출된다.
STDMETHODIMP TextService::OnUninitDocumentMgr(_In_ ITfDocumentMgr *pDocMgr)
{
	DebugLogFile(L"%s\n", L"TextService::OnUninitDocumentMgr");
	return E_NOTIMPL;
}

// 포커스가 어떤 도큐먼트에서 다른 도큐먼트로 옮겨 갈 때 호출된다.
// 두 도큐먼트는 NULL일 수도 있다. 무슨 말이냐면 이전 도큐먼트가 없거나
// 이전 도큐먼트에서 입력 포커스를 받지 않았다는 말이다.
STDMETHODIMP TextService::OnSetFocus(_In_ ITfDocumentMgr *pDocMgrFocus, _In_ ITfDocumentMgr *pDocMgrPrevFocus)
{
	DebugLogFile(L"%s\n", L"TextService::OnSetFocus");

	_InitTextEditSink(pDocMgrFocus);

	/*
	_UpdateLanguageBarOnSetFocus(pDocMgrFocus);

	//
	// We have to hide/unhide candidate list depending on whether they are 
	// associated with pDocMgrFocus.
	//
	if (_pCandidateListUIPresenter)
	{
		ITfDocumentMgr* pCandidateListDocumentMgr = nullptr;
		ITfContext* pTfContext = _pCandidateListUIPresenter->_GetContextDocument();
		if ((nullptr != pTfContext) && SUCCEEDED(pTfContext->GetDocumentMgr(&pCandidateListDocumentMgr)))
		{
			if (pCandidateListDocumentMgr != pDocMgrFocus)
			{
				_pCandidateListUIPresenter->OnKillThreadFocus();
			}
			else
			{
				_pCandidateListUIPresenter->OnSetThreadFocus();
			}

			pCandidateListDocumentMgr->Release();
		}
	}

	if (_pDocMgrLastFocused)
	{
		_pDocMgrLastFocused->Release();
		_pDocMgrLastFocused = nullptr;
	}

	_pDocMgrLastFocused = pDocMgrFocus;

	if (_pDocMgrLastFocused)
	{
		_pDocMgrLastFocused->AddRef();
	}
	*/
	return S_OK;
}

// 컨택스트가 푸시될 때 호출된다.
STDMETHODIMP TextService::OnPushContext(_In_ ITfContext *pContext)
{
	DebugLogFile(L"%s\n", L"TextService::OnPushContext");
	return E_NOTIMPL;
}

// 컨택스가 팝될 때 호출된다.
STDMETHODIMP TextService::OnPopContext(_In_ ITfContext *pContext)
{
	DebugLogFile(L"%s\n", L"TextService::OnPopContext");
	return E_NOTIMPL;
}