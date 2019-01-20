#include "Global.h"
#include "TextService.h"

bool TextService::_InitTextEditSink(_In_ ITfDocumentMgr *pDocMgr)
{
	DebugLogFile(L"%s - %p\n", L"TextService::_InitTextEditSink", pDocMgr);

	ITfSource* pSource = nullptr;
	bool ret = true;

	// clear out any previous sink first
	if (_textEditSinkCookie != TF_INVALID_COOKIE && _pTextEditSinkContext != nullptr)
	{
		if (SUCCEEDED(_pTextEditSinkContext->QueryInterface(IID_ITfSource, (void **)&pSource)))
		{
			pSource->UnadviseSink(_textEditSinkCookie);
			pSource->Release();
		}

		_pTextEditSinkContext->Release();
		_pTextEditSinkContext = nullptr;
		_textEditSinkCookie = TF_INVALID_COOKIE;
	}

	if (pDocMgr == nullptr)
	{
		return true; // caller just wanted to clear the previous sink
	}

	if (FAILED(pDocMgr->GetTop(&_pTextEditSinkContext)))
	{
		return false;
	}

	if (_pTextEditSinkContext == nullptr)
	{
		return true; // empty document, no sink possible
	}

	ret = false;
	if (SUCCEEDED(_pTextEditSinkContext->QueryInterface(IID_ITfSource, (void **)&pSource)))
	{
		if (SUCCEEDED(pSource->AdviseSink(IID_ITfTextEditSink, (ITfTextEditSink *)this, &_textEditSinkCookie)))
		{
			ret = true;
		}
		else
		{
			_textEditSinkCookie = TF_INVALID_COOKIE;
		}
		pSource->Release();
	}

	if (ret == false)
	{
		_pTextEditSinkContext->Release();
		_pTextEditSinkContext = nullptr;
	}

	return ret;
}

/*
ITfTextEditSink interface functions
*/
// write-access document lock를 해제 할 때 마다 호출된다.
STDMETHODIMP TextService::OnEndEdit(__RPC__in_opt ITfContext *pContext, TfEditCookie ecReadOnly, __RPC__in_opt ITfEditRecord *pEditRecord)
{
	DebugLogFile(L"%s EC(Ro):%lu\n", L"TextService::OnEndEdit", ecReadOnly);
	
#if 0
	BOOL fSelectionChanged;
	IEnumTfRanges *pEnumTextChanges;
	ITfRange *pRange;

	//
	// did the selection change?
	// The selection change includes the movement of caret as well. 
	// The caret position is represent as the empty selection range when
	// there is no selection.
	//
	if (pEditRecord->GetSelectionStatus(&fSelectionChanged) == S_OK &&
		fSelectionChanged)
	{
		// If the selection is moved to out side of the current composition,
		// terminate the composition. This TextService supports only one
		// composition in one context object.
		if (_IsComposing())
		{
			TF_SELECTION tfSelection;
			ULONG cFetched;

			if (pContext->GetSelection(ecReadOnly, TF_DEFAULT_SELECTION, 1, &tfSelection, &cFetched) == S_OK && cFetched == 1)
			{
				ITfRange *pRangeComposition;
				// is the insertion point covered by a composition?
				if (_pComposition->GetRange(&pRangeComposition) == S_OK)
				{
					if (!IsRangeCovered(ecReadOnly, tfSelection.range, pRangeComposition))
					{
						_EndComposition(pContext);
					}

					pRangeComposition->Release();
				}
			}
		}
	}

	// text modification?
	if (pEditRecord->GetTextAndPropertyUpdates(TF_GTP_INCL_TEXT, NULL, 0, &pEnumTextChanges) == S_OK)
	{
		if (pEnumTextChanges->Next(1, &pRange, NULL) == S_OK)
		{
			//
			// pRange is the updated range.
			//

			pRange->Release();
		}

		pEnumTextChanges->Release();
	}
#endif
	return S_OK;
}