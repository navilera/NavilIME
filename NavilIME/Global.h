#pragma once

#include "stdafx.h"
#include "resource.h"

#include <windows.h>
#include <ole2.h>
#include <olectl.h>
#include <assert.h>
#include "msctf.h"
#include "sal.h"
#include <combaseapi.h>
#include <strsafe.h>
#include <intsafe.h>
#include "initguid.h"
#include "ctffunc.h"
#include "NavilIME.h"


#ifdef _DEBUG
#define DEBUG_LOGFILE L"C:\\Users\\yiman\\Documents\\Projects\\Ime\\TheNew\\NavilIME\\navilime.log"
void DebugLog(const wchar_t *format, ...);
#define DebugLogFile(fmt, ...) DebugLog(fmt, __VA_ARGS__)
#else
#define DebugLogFile(fmt, ...) 
#endif


BOOL IsRangeCovered(TfEditCookie ec, ITfRange *pRangeTest, ITfRange *pRangeCover);

extern HINSTANCE gHandleDllInstance;
extern CRITICAL_SECTION gCriticalSection;
extern LONG gDllReferCount;

extern const CLSID cNavilIME_CLSID;
extern const GUID cNavilIME_GUID_Profile;

class NavilIME;
extern NavilIME gNavilIME;
