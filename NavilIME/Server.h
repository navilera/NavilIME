#pragma once

class ComClassFactory : public IClassFactory {
public:
	// IUnknown methods
	STDMETHODIMP QueryInterface(REFIID riid, void **ppvObj);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	// IClassFactory methods
	STDMETHODIMP CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObj);
	STDMETHODIMP LockServer(BOOL fLock);

	// Constructor
	ComClassFactory(REFCLSID rclsid, HRESULT(*pfnCreateInstance)(IUnknown *pUnkOuter, REFIID riid, void **ppvObj))
		: _rclsid(rclsid)
	{
		_pfnCreateInstance = pfnCreateInstance;
	}

	// Destructure
	~ComClassFactory() {}

public:
	REFCLSID _rclsid;
	HRESULT(*_pfnCreateInstance)(IUnknown *pUnkOuter, REFIID riid, void **ppvObj);
};

void Server_DllAddRef(void);
void Server_DllRelease(void);

