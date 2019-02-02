#include "Global.h"
#include "TextService.h"
#include "EditSession.h"

STDAPI TextService::OnCompositionTerminated(TfEditCookie ecWrite, ITfComposition *pComposition)
{
	DebugLogFile(L"%s\n", L"TextService::OnCompositionTerminated");
	// releae our cached composition
	
	if (_pComposition != NULL)
	{
		_pComposition->Release();
		_pComposition = NULL;
	}
	

	return S_OK;
}

bool TextService::_IsComposing()
{
	return _pComposition != NULL;
}

void TextService::SetComposition(ITfComposition *pComposition)
{
	_pComposition = pComposition;
}

HRESULT TextService::HandleCompositionKey(TfEditCookie ec, ITfContext *pContext, WPARAM wParam) 
{
	ITfRange *pRangeComposition;
	TF_SELECTION tfSelection;
	ULONG cFetched;
	BOOL fCovered;
	HRESULT hr = S_OK;
	WCHAR ch = (WCHAR)wParam;
	
	// Start the new compositon if there is no composition.
	if (!_IsComposing())
	{
		_StartComposition(pContext, ch);
		return S_OK;
	}

	DebugLogFile(L"%s EC: %lu\n", L"TextService::HandleCompositionKey", ec);

	// first, test where a keystroke would go in the document if an insert is done
	if (pContext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &cFetched) != S_OK || cFetched != 1)
	{
		DebugLogFile(L"\t%s\n", L"GetSelection fail...");
		return S_FALSE;
	}
		
	// is the insertion point covered by a composition?
	if (_pComposition->GetRange(&pRangeComposition) == S_OK)
	{
		fCovered = IsRangeCovered(ec, tfSelection.range, pRangeComposition);

		pRangeComposition->Release();

		if (!fCovered)
		{
			DebugLogFile(L"\t%s\n", L"IsRangeCovered fail...");
			tfSelection.range->Release();
			return S_OK;
		}
	}

	BOOL isEmpty;
	tfSelection.range->IsEmpty(ec, &isEmpty);
	DebugLogFile(L"\t%s -> %d\n", L"Is Range Empty?", isEmpty);
	if (isEmpty)
	{
		LONG cch;
		tfSelection.range->ShiftStart(ec, -1, &cch, NULL);
	}

	// insert the text
	// use SetText here instead of InsertTextAtSelection because a composition has already been started
	// Don't allow to the app to adjust the insertion point inside the composition
	//if (tfSelection.range->SetText(ec, 0, &ch, 1) != S_OK)
	if (tfSelection.range->SetText(ec, TF_ST_CORRECTION, &ch, 1) != S_OK)
	{
		DebugLogFile(L"\t%s\n", L"SetText Fail..");
		tfSelection.range->Release();
		return S_OK;
	}

	// update the selection, we'll make it an insertion point just past
	// the inserted text.
	//tfSelection.range->Collapse(ec, TF_ANCHOR_END);
	//pContext->SetSelection(ec, 1, &tfSelection);
	tfSelection.range->Release();

	return S_OK;
}

void TextService::_StartComposition(ITfContext *pContext, WPARAM wParam)
{
	DebugLogFile(L"%s\n", L"TextService::_StartComposition");

	StartCompositionEditSession *pStartCompositionEditSession;
	HRESULT hr;

	if (pStartCompositionEditSession = new StartCompositionEditSession(this, pContext))
	{
		pStartCompositionEditSession->SetPContext(pContext, wParam);
		pContext->RequestEditSession(_ClientId, pStartCompositionEditSession, TF_ES_SYNC | TF_ES_READWRITE, &hr);
		pStartCompositionEditSession->Release();
	}
}

void TextService::_HandleComposition(ITfContext *pContext, WCHAR preedit)
{
	DebugLogFile(L"%s\n", L"TextService::_HandleComposition");

	EditSession* pEditSession;
	HRESULT hr;

	if (pEditSession = new EditSession(this))
	{
		pEditSession->SetPContext(pContext, (WPARAM)preedit);
		pContext->RequestEditSession(_ClientId, pEditSession, TF_ES_SYNC | TF_ES_READWRITE, &hr);
		pEditSession->Release();
	}
}

void TextService::_AppendText(ITfContext *pContext, WCHAR txt)
{
	DebugLogFile(L"%s\n", L"TextService::_AppendText");

	AppendCompositionEditSession* pAppendEditSession;
	HRESULT hr;

	if (pAppendEditSession = new AppendCompositionEditSession(this, pContext))
	{
		pAppendEditSession->SetPContext(pContext, (WPARAM)txt);
		pContext->RequestEditSession(_ClientId, pAppendEditSession, TF_ES_SYNC | TF_ES_READWRITE, &hr);
		pAppendEditSession->Release();
	}
}

void TextService::_EndComposition(ITfContext *pContext)
{
	if (_IsComposing())
	{
		DebugLogFile(L"%s\n", L"TextService::_EndComposition");

		EndCompositionEditSession *pEndEditSession;
		HRESULT hr;

		if (pEndEditSession = new EndCompositionEditSession(this, pContext))
		{
			pEndEditSession->SetPContext(pContext, 0);
			pContext->RequestEditSession(_ClientId, pEndEditSession, TF_ES_SYNC | TF_ES_READWRITE, &hr);
			pEndEditSession->Release();
		}
	}
}

void TextService::TerminateComposition(TfEditCookie ec)
{
	DebugLogFile(L"%s EC:%lu\n", L"TextService::TerminateComposition", ec);

	if (_pComposition != NULL)
	{
		DebugLogFile(L"\t%s\n", L"Call EndComposition");
		if (_pComposition->EndComposition(ec) == S_OK)
		{
			_pComposition->Release();
			_pComposition = NULL;
			DebugLogFile(L"\t-->%s\n", L"EndComposition Done");
		}
		else
		{
			DebugLogFile(L"\t-->%s\n", L"EndComposition Fail");
		}
	}
}

