#pragma once

class LanguageBar;
class EditSession;

class TextService : public ITfTextInputProcessorEx,
	public ITfThreadMgrEventSink,
	public ITfTextEditSink,
	public ITfKeyEventSink,
	public ITfCompositionSink
{
public:
	TextService();
	~TextService();

	// ComClassFactory has function pointer as call back in Server.cpp
	static HRESULT CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObj);

	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void **ppvObj);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	// ITfTextInputProcessor
	STDMETHODIMP Activate(ITfThreadMgr *pThreadMgr, TfClientId tfClientId);
	STDMETHODIMP Deactivate();

	// ITfTextInputProcessorEx
	STDMETHODIMP ActivateEx(ITfThreadMgr *pThreadMgr, TfClientId tfClientId, DWORD dwFlags);

	// ITfThreadMgrEventSink
	STDMETHODIMP OnInitDocumentMgr(_In_ ITfDocumentMgr *pDocMgr);
	STDMETHODIMP OnUninitDocumentMgr(_In_ ITfDocumentMgr *pDocMgr);
	STDMETHODIMP OnSetFocus(_In_ ITfDocumentMgr *pDocMgrFocus, _In_ ITfDocumentMgr *pDocMgrPrevFocus);
	STDMETHODIMP OnPushContext(_In_ ITfContext *pContext);
	STDMETHODIMP OnPopContext(_In_ ITfContext *pContext);

	// ITfTextEditSink
	STDMETHODIMP OnEndEdit(__RPC__in_opt ITfContext *pContext, TfEditCookie ecReadOnly, __RPC__in_opt ITfEditRecord *pEditRecord);

	// ITfKeyEventSink
	STDMETHODIMP OnSetFocus(BOOL fForeground);
	STDMETHODIMP OnTestKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pIsEaten);
	STDMETHODIMP OnKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pIsEaten);
	STDMETHODIMP OnTestKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pIsEaten);
	STDMETHODIMP OnKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pIsEaten);
	STDMETHODIMP OnPreservedKey(ITfContext *pContext, REFGUID rguid, BOOL *pIsEaten);

	// ITfCompositionSink
	STDMETHODIMP OnCompositionTerminated(TfEditCookie ecWrite, ITfComposition *pComposition);

public:
	/* Composition */
	HRESULT HandleCompositionKey(TfEditCookie ec, ITfContext *pContext, WPARAM wParam);
	void SetComposition(ITfComposition *pComposition);
	void TerminateComposition(TfEditCookie ec);

private:
	/* ThreadMgrEventSink */
	bool _InitThreadMgrEventSink();
	void _UninitThreadMgrEventSink();

	/* TextEditSink */
	bool _InitTextEditSink(_In_ ITfDocumentMgr *pDocMgr);

	/* KeyEventSink */
	bool _InitKeyEventSink();
	void _UninitKeyEventSink();
	bool _InitPreservedKey();
	void _UninitPreservedKey();
	WORD _ConvertVKeyToAscii(UINT code);
	void _Automata(UINT key);

	/* LangBarItemButton */
	bool _InitLangBarItemButton();
	void _UnInitLangBarItemButton();

	/* Composition */
	bool _IsComposing();
	void _StartComposition(ITfContext *pContext, WPARAM wParam);
	void _HandleComposition(ITfContext *pContext, WCHAR preedit);
	void _EndComposition(ITfContext *pContext);

private:
	/* For use IUnknown */
	LONG _RefCount;

	/* Assigned by ActivateEx */
	ITfThreadMgr* _pThreadMgr;
	TfClientId _ClientId;
	DWORD _ActivateFlags;

	/* The cookie of ThreadMgrEventSink */
	DWORD _threadMgrEventSinkCookie;

	/* The cookie of TextEditSink */
	ITfContext* _pTextEditSinkContext;
	DWORD _textEditSinkCookie;

	/* KeyEventSink */
	bool _keyEaten;
	bool _testKeyHappened;

	/* LangBarItemButton */
	LanguageBar* _pHangulTurnOnOffStatus;

	/* EditSession */
	//EditSession* _pEditSession;

	// the current composition object.
	ITfComposition* _pComposition;
};

