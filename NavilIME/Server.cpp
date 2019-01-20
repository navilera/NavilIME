#include "Global.h"
#include "Register.h"
#include "TextService.h"
#include "Server.h"

static ComClassFactory* spComClassFactoryInstance = nullptr;

void Server_DllAddRef(void)
{
	InterlockedIncrement(&gDllReferCount);
}

void Server_DllRelease(void)
{
	if (InterlockedDecrement(&gDllReferCount) < 0) // g_cRefDll == -1 with zero refs
	{
		EnterCriticalSection(&gCriticalSection);

		// need to check ref again after grabbing mutex
		if (spComClassFactoryInstance != nullptr)
		{
			delete spComClassFactoryInstance;
			spComClassFactoryInstance = NULL;
		}
		assert(gDllReferCount == -1);

		LeaveCriticalSection(&gCriticalSection);
	}
}

STDAPI ComClassFactory::QueryInterface(REFIID riid, void **ppvObj) 
{
	DebugLogFile(L"%s\n", L"ComClassFactory::QueryInterface");
	if (IsEqualIID(riid, IID_IClassFactory) || IsEqualIID(riid, IID_IUnknown)) {
		*ppvObj = this;
		Server_DllAddRef();
		return NOERROR;
	}
	*ppvObj = NULL;
	return E_NOINTERFACE;
}

STDAPI_(ULONG) ComClassFactory::AddRef() 
{
	DebugLogFile(L"%s\n", L"ComClassFactory::AddRef");
	Server_DllAddRef();
	return (gDllReferCount + 1); // -1 w/ no refs
}

//	CClassFactory::Release
STDAPI_(ULONG) ComClassFactory::Release() 
{
	DebugLogFile(L"%s\n", L"ComClassFactory::Release");

	Server_DllRelease();
	return (gDllReferCount + 1); // -1 w/ no refs
}

STDAPI ComClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObj) 
{
	DebugLogFile(L"%s\n", L"ComClassFactory::CreateInstance");
	return _pfnCreateInstance(pUnkOuter, riid, ppvObj);
}

STDAPI ComClassFactory::LockServer(BOOL fLock) 
{
	DebugLogFile(L"%s\n", L"ComClassFactory::LockServer");

	if (fLock) {
		Server_DllAddRef();
	}
	else {
		Server_DllRelease();
	}

	return S_OK;
}

//	DllGetClassObject
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppvObj) 
{
	DebugLogFile(L"%s\n", L"DllGetClassObject");

	if (spComClassFactoryInstance == nullptr) {
		EnterCriticalSection(&gCriticalSection);

		// need to check ref again after grabbing mutex
		if (spComClassFactoryInstance == nullptr) {
			spComClassFactoryInstance = new ComClassFactory(cNavilIME_CLSID, TextService::CreateInstance);
		}

		LeaveCriticalSection(&gCriticalSection);
	}

	if (IsEqualIID(riid, IID_IClassFactory) || IsEqualIID(riid, IID_IUnknown))
	{
		if (spComClassFactoryInstance != nullptr && IsEqualGUID(rclsid, spComClassFactoryInstance->_rclsid))
		{
			*ppvObj = (void *)spComClassFactoryInstance;
			Server_DllAddRef();
			DebugLogFile(L"\t%s\n", L"DllGetClassObject.. Done");
			return NOERROR;
		}
	}

	*ppvObj = NULL;
	DebugLogFile(L"\t%s\n", L"DllGetClassObject.. Fail");
	return CLASS_E_CLASSNOTAVAILABLE;
}

//	DllCanUnloadNow
STDAPI DllCanUnloadNow(void) 
{
	DebugLogFile(L"%s\n", L"DllCanUnloadNow");

	if (gDllReferCount >= 0) { // -1 with no refs
		return S_FALSE;
	}

	return S_OK;
}

//	DllUnregisterServer
STDAPI DllUnregisterServer(void) 
{
	DebugLogFile(L"%s\n", L"DllUnregisterServer");

	UnregisterAll();

	return S_OK;
}

//	DllRegisterServer
STDAPI DllRegisterServer(void) 
{
	DebugLogFile(L"%s\n", L"DllRegisterServer");

	// register this service's profile with the tsf
	if (RegisterAll() == false)
	{
		DebugLogFile(L"\t%s\n", L"DllRegisterServer.. Fail");
		DllUnregisterServer(); // cleanup any loose ends
		return E_FAIL;
	}
	DebugLogFile(L"\t%s\n", L"DllRegisterServer.. Done");

	return S_OK;
}