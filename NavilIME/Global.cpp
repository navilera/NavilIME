#include "Global.h"

#include <iostream>

HINSTANCE gHandleDllInstance;
CRITICAL_SECTION gCriticalSection;
LONG gDllReferCount = -1; // -1 /w no refs, for win95 InterlockedIncrement/Decrement compat

// {1A48E583-7738-412E-8F7A-EDAE42833B91}
const CLSID cNavilIME_CLSID = { 
	0x1a48e583, 
	0x7738, 
	0x412e,
	{ 0x8f, 0x7a, 0xed, 0xae, 0x42, 0x83, 0x3b, 0x91 } 
};

// {40F65843-95C6-40EE-9797-4972FA606EC9}
const GUID cNavilIME_GUID_Profile = {
	0x40f65843, 
	0x95c6, 
	0x40ee,
	{ 0x97, 0x97, 0x49, 0x72, 0xfa, 0x60, 0x6e, 0xc9 } 
};

BOOL IsRangeCovered(TfEditCookie ec, ITfRange *pRangeTest, ITfRange *pRangeCover)
{
	LONG lResult;

	if (pRangeCover->CompareStart(ec, pRangeTest, TF_ANCHOR_START, &lResult) != S_OK ||
		lResult > 0)
	{
		return FALSE;
	}

	if (pRangeCover->CompareEnd(ec, pRangeTest, TF_ANCHOR_END, &lResult) != S_OK ||
		lResult < 0)
	{
		return FALSE;
	}

	return TRUE;
}

#ifdef _DEBUG
void DebugLog(const wchar_t *format, ...) 
{
	FILE *fp;

	if ((fp = _wfopen(DEBUG_LOGFILE, L"a")) == NULL) {
		fp = _wfopen(DEBUG_LOGFILE, L"w");
	}

	if (fp != NULL) {
		va_list args;
		va_start(args, format);
		vfwprintf(fp, format, args);
		va_end(args);

		fclose(fp);
	}
}
#endif
