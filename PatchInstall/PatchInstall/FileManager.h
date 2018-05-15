#pragma once
#include <iostream>
#include <afxinet.h>
#include <Urlmon.h>
#include "PatchInstallDlg.h"
struct AssetItem
{
	CString AssetName;
	CString Md5Code;

	AssetItem() {}

	AssetItem(const CString AN,const CString MD5)
		:AssetName(AN), Md5Code(MD5)
	{
	
	}

	const bool operator==(const AssetItem& other) 
	{
		return  (this->AssetName == other.AssetName) && (this->Md5Code == other.Md5Code);
	}


	const bool operator!=(const AssetItem& other)
	{
		return  (this->AssetName != other.AssetName) || (this->Md5Code != other.Md5Code);
	}
};


struct FileSizeInfo 
{
	LONGLONG  TotalSize;
	LONGLONG  DownloadedSize;
	FileSizeInfo() 
	{
		TotalSize = 0;
		DownloadedSize = 0;
	}

};




class FileManager
{
public:
	FileManager();
	~FileManager();
};


class CHttpAssetsConfigFile 
{
public:
	CHttpAssetsConfigFile();
	~CHttpAssetsConfigFile();

public:
	INT DownloadAssetConfigFile(const CString ConfigFileUrl, std::string& ConfigFileContent);
	bool CompareAssetConfigFile(HWND Wnd);

protected:
	void CompareFileWithAssetConfig();
	CString m_ConfigFilePath;

};

class CHttpAssetDownload 
{

public:
	CHttpAssetDownload();

	CHttpAssetDownload(const CString& AssetSaveDiskD,const CString& AssetURL,const CString& AssetN);
	~CHttpAssetDownload();

	bool DownLoad(HWND Wnd);
	static UINT ThreadProc(LPVOID pPram);

	CString GetAssetsUrlDir() { return AssetsUrlDir; }
	CString GetAssetsAssetsName() { return AssetsName; }
	CString GetAssetSaveDiskDir(){ return AssetSaveDiskDir; }
	HWND    GetWnd() { return CallbackWnd; }
protected:
	CString AssetsUrlDir;
	CString AssetsName;
	CString AssetSaveDiskDir;
	HWND    CallbackWnd;
};

class DownloadProgress : public IBindStatusCallback 
{
public:
	HRESULT __stdcall QueryInterface(const IID &, void **)
	{
		return E_NOINTERFACE;
	}
	ULONG STDMETHODCALLTYPE AddRef(void) 
	{
		return 1;
	}
	ULONG STDMETHODCALLTYPE Release(void)
	{
		return 1;
	}
	HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved, IBinding *pib) 
	{
		return E_NOTIMPL;
	}
	virtual HRESULT STDMETHODCALLTYPE GetPriority(LONG *pnPriority)
	{
		return E_NOTIMPL;
	}
	virtual HRESULT STDMETHODCALLTYPE OnLowResource(DWORD reserved)
	{
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, LPCWSTR szError)
	{
		if (AssetDownloader) 
		{
			delete AssetDownloader;
			AssetDownloader = NULL;
		}
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD *grfBINDF, BINDINFO *pbindinfo) 
	{
		return E_NOTIMPL;
	}
	virtual HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed)
	{
		return E_NOTIMPL;
	}
	virtual HRESULT STDMETHODCALLTYPE OnObjectAvailable(REFIID riid, IUnknown *punk) 
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnProgress)(ULONG ulProgress,ULONG ulProgressMax,ULONG ulStatusCode,LPCWSTR wszStatusText)

	//virtual HRESULT __stdcall OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText)
	{
		if (ulProgressMax != 0)
		{
			//double *percentage = new double(ulProgress * 1.0 / ulProgressMax * 100);
			//将percentage 发送给显示
			::PostMessage(wnd, WM_UPDATE_PROGRESS, (WPARAM)ulProgress, (LPARAM)ulProgressMax);
		//	delete percentage;
		}
		return S_OK;
	}

public :
	HWND wnd;
	CHttpAssetDownload* AssetDownloader;
};