#include "Global.h"
#include "Register.h"

#define CLSID_STRLEN 38
#define TEXTSERVICE_LANGID       MAKELANGID(LANG_KOREAN, SUBLANG_KOREAN)

static bool RegServer(void);
static bool RegProfile(void);
static bool RegCategory(void);

static void UnRegServer(void);
static void UnRegProfile(void);
static void UnRegCategory(void);

static void CLSIDToString(REFGUID refGUID, wchar_t *pchA);
static LONG RecurseDeleteKey(_In_ HKEY hParentKey, _In_ LPCTSTR lpszKey);

static const WCHAR RegInfo_Prefix_CLSID[] = L"CLSID\\";
static const WCHAR RegInfo_Key_InProSvr32[] = L"InProcServer32";
static const WCHAR RegInfo_Key_ThreadModel[] = L"ThreadingModel";

static const WCHAR TEXTSERVICE_DESC[] = L"Navil IME";
static const WCHAR TEXTSERVICE_MODEL[] = L"Apartment";

static const GUID SupportCategories[] = {
	GUID_TFCAT_TIP_KEYBOARD,
	GUID_TFCAT_TIPCAP_SYSTRAYSUPPORT,
	/*
	GUID_TFCAT_DISPLAYATTRIBUTEPROVIDER,
	GUID_TFCAT_TIPCAP_UIELEMENTENABLED,
	GUID_TFCAT_TIPCAP_SECUREMODE,
	GUID_TFCAT_TIPCAP_COMLESS,
	GUID_TFCAT_TIPCAP_INPUTMODECOMPARTMENT,
	GUID_TFCAT_TIPCAP_IMMERSIVESUPPORT,
	*/
};

bool RegisterAll(void)
{
	return (RegServer() && RegProfile() && RegCategory());
}

void UnregisterAll(void)
{
	UnRegCategory();
	UnRegProfile();
	UnRegServer();
}


static bool RegServer(void)
{
	DebugLogFile(L"%s\n", L"RegServer");

	DWORD dw;
	HKEY hKey = nullptr;
	HKEY hSubKey = nullptr;
	WCHAR achIMEKey[ARRAYSIZE(RegInfo_Prefix_CLSID) + CLSID_STRLEN];
	WCHAR achFileName[MAX_PATH];

	CLSIDToString(cNavilIME_CLSID, achIMEKey + ARRAYSIZE(RegInfo_Prefix_CLSID) - 1);

	memcpy(achIMEKey, RegInfo_Prefix_CLSID, sizeof(RegInfo_Prefix_CLSID) - sizeof(WCHAR));

	if (RegCreateKeyEx(HKEY_CLASSES_ROOT, achIMEKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dw) == ERROR_SUCCESS)
	{
		if (RegSetValueEx(hKey, NULL, 0, REG_SZ, (const BYTE *)TEXTSERVICE_DESC, (_countof(TEXTSERVICE_DESC)) * sizeof(WCHAR)) != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return false;
		}
		         
		if (RegCreateKeyEx(hKey, RegInfo_Key_InProSvr32, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hSubKey, &dw) == ERROR_SUCCESS)
		{
			dw = GetModuleFileNameW(gHandleDllInstance, achFileName, ARRAYSIZE(achFileName));
			dw = (dw >= (MAX_PATH - 1)) ? MAX_PATH : (++dw);

			if (RegSetValueEx(hSubKey, NULL, 0, REG_SZ, (const BYTE *)achFileName, (dw) * sizeof(WCHAR)) != ERROR_SUCCESS)
			{
				RegCloseKey(hSubKey);
				RegCloseKey(hKey);
				return false;
			}
			if (RegSetValueEx(hSubKey, RegInfo_Key_ThreadModel, 0, REG_SZ, (const BYTE *)TEXTSERVICE_MODEL, (_countof(TEXTSERVICE_MODEL)) * sizeof(WCHAR)) != ERROR_SUCCESS)
			{
				RegCloseKey(hSubKey);
				RegCloseKey(hKey);
				return false;
			}
			RegCloseKey(hSubKey);
		}
		else
		{
			RegCloseKey(hKey);
			return false;
		}
		RegCloseKey(hKey);
	}

	return true;
}

static void UnRegServer(void)
{
	DebugLogFile(L"%s\n", L"UnRegServer");

	WCHAR achIMEKey[ARRAYSIZE(RegInfo_Prefix_CLSID) + CLSID_STRLEN] = { '\0' };

	CLSIDToString(cNavilIME_CLSID, achIMEKey + ARRAYSIZE(RegInfo_Prefix_CLSID) - 1);
	memcpy(achIMEKey, RegInfo_Prefix_CLSID, sizeof(RegInfo_Prefix_CLSID) - sizeof(WCHAR));

	RecurseDeleteKey(HKEY_CLASSES_ROOT, achIMEKey);
}

static bool RegProfile(void)
{
	DebugLogFile(L"%s\n", L"RegProfile");

	ITfInputProcessorProfileMgr *pITfInputProcessorProfileMgr = nullptr;
	HRESULT hr = S_FALSE;
	hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, 
		NULL, CLSCTX_INPROC_SERVER, IID_ITfInputProcessorProfileMgr, (void**)&pITfInputProcessorProfileMgr);
	if (FAILED(hr))
	{
		return FALSE;
	}

	WCHAR achIconFile[MAX_PATH] = { '\0' };
	DWORD cchA = 0;
	cchA = GetModuleFileNameW(gHandleDllInstance, achIconFile, MAX_PATH);
	cchA = cchA >= MAX_PATH ? (MAX_PATH - 1) : cchA;
	achIconFile[cchA] = '\0';

	size_t lenOfDesc = 0;
	hr = StringCchLength(TEXTSERVICE_DESC, STRSAFE_MAX_CCH, &lenOfDesc);
	if (hr != S_OK)
	{
		pITfInputProcessorProfileMgr->Release();
		return false;
	}

	hr = pITfInputProcessorProfileMgr->RegisterProfile(cNavilIME_CLSID,
		TEXTSERVICE_LANGID,
		cNavilIME_GUID_Profile,
		TEXTSERVICE_DESC,
		static_cast<ULONG>(lenOfDesc),
		achIconFile,
		cchA,
		(UINT)(-IDI_ICON_IME), NULL, 0, TRUE, 0);

	if (FAILED(hr))
	{
		pITfInputProcessorProfileMgr->Release();
		return false;
	}

	pITfInputProcessorProfileMgr->Release();

	return true;
}

static void UnRegProfile(void)
{
	DebugLogFile(L"%s\n", L"UnRegProfile");

	ITfInputProcessorProfiles *pInputProcessProfiles;
	HRESULT hr;

	hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, NULL, 
		CLSCTX_INPROC_SERVER, IID_ITfInputProcessorProfiles, (void**)&pInputProcessProfiles);

	if (hr != S_OK) {
		return;
	}

	pInputProcessProfiles->Unregister(cNavilIME_CLSID);
	pInputProcessProfiles->Release();
}

static bool RegCategory(void)
{
	DebugLogFile(L"%s\n", L"RegCategory");

	ITfCategoryMgr* pCategoryMgr = nullptr;
	HRESULT hr = S_OK;

	hr = CoCreateInstance(CLSID_TF_CategoryMgr, NULL, 
		CLSCTX_INPROC_SERVER, IID_ITfCategoryMgr, (void**)&pCategoryMgr);

	if (FAILED(hr))	{
		return false;
	}

	for (GUID guid : SupportCategories)
	{
		hr = pCategoryMgr->RegisterCategory(cNavilIME_CLSID, guid, cNavilIME_CLSID);
	}

	pCategoryMgr->Release();

	return (hr == S_OK);
}

static void UnRegCategory(void)
{
	DebugLogFile(L"%s\n", L"UnRegCategory");

	ITfCategoryMgr* pCategoryMgr = S_OK;
	HRESULT hr = S_OK;

	hr = CoCreateInstance(CLSID_TF_CategoryMgr, NULL, 
		CLSCTX_INPROC_SERVER, IID_ITfCategoryMgr, (void**)&pCategoryMgr);

	if (FAILED(hr))
	{
		return;
	}

	for (GUID guid : SupportCategories)
	{
		pCategoryMgr->UnregisterCategory(cNavilIME_CLSID, guid, cNavilIME_CLSID);
	}

	pCategoryMgr->Release();

	return;
}

static void CLSIDToString(REFGUID refGUID, wchar_t *pchA)
{
	static const BYTE GuidMap[] = { 3, 2, 1, 0, '-', 5, 4, '-', 7, 6, '-',
		8, 9, '-', 10, 11, 12, 13, 14, 15 };

	static const wchar_t szDigits[] = L"0123456789ABCDEF";

	int i;
	wchar_t *p = pchA;

	const BYTE * pBytes = (const BYTE *)&refGUID;

	*p++ = '{';
	for (i = 0; i < sizeof(GuidMap); i++) {
		if (GuidMap[i] == '-') {
			*p++ = L'-';
		}
		else {
			*p++ = szDigits[(pBytes[GuidMap[i]] & 0xF0) >> 4];
			*p++ = szDigits[(pBytes[GuidMap[i]] & 0x0F)];
		}
	}

	*p++ = L'}';
	*p = L'\0';
}

static LONG RecurseDeleteKey(_In_ HKEY hParentKey, _In_ LPCTSTR lpszKey)
{
	HKEY regKeyHandle = nullptr;
	LONG res = 0;
	FILETIME time;
	WCHAR stringBuffer[256] = { '\0' };
	DWORD size = ARRAYSIZE(stringBuffer);

	if (RegOpenKey(hParentKey, lpszKey, &regKeyHandle) != ERROR_SUCCESS)
	{
		return ERROR_SUCCESS;
	}

	res = ERROR_SUCCESS;
	while (RegEnumKeyEx(regKeyHandle, 0, stringBuffer, &size, NULL, NULL, NULL, &time) == ERROR_SUCCESS)
	{
		stringBuffer[ARRAYSIZE(stringBuffer) - 1] = '\0';
		res = RecurseDeleteKey(regKeyHandle, stringBuffer);
		if (res != ERROR_SUCCESS)
		{
			break;
		}
		size = ARRAYSIZE(stringBuffer);
	}
	RegCloseKey(regKeyHandle);

	return res == ERROR_SUCCESS ? RegDeleteKey(hParentKey, lpszKey) : res;
}