#include "stdafx.h"
#include <afxinet.h>
#include "json_incude/json.h"
#include "FileManager.h"
#include <fstream>
#include <shlwapi.h>
#include <vector>

#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"urlmon.lib")

FileManager::FileManager()
{
}


FileManager::~FileManager()
{
}

CHttpAssetsConfigFile::CHttpAssetsConfigFile()
{
	GetModuleFileName(NULL, m_ConfigFilePath.GetBuffer(256), 256);
	m_ConfigFilePath.ReleaseBuffer();
	int nPos = m_ConfigFilePath.ReverseFind(('\\'));
	m_ConfigFilePath = m_ConfigFilePath.Left(nPos + 1);
}

CHttpAssetsConfigFile::~CHttpAssetsConfigFile()
{
}

INT CHttpAssetsConfigFile::DownloadAssetConfigFile(const CString ConfigFileUrl, std::string& ConfigFileContent)
{
	ASSERT(ConfigFileUrl != "");
	//ASSERT(SavePath != "");
	//if (!PathFileExists(SavePath)) 
	//{
	//	if (0 == CreateDirectory(SavePath, NULL)) 
	//	{
	//		return -9; //文件路劲错误
	//	}
	//}
	
	unsigned short  nPort;
	CString ServerAddr, FileObjectName;
	DWORD  dwServiceType, dwRet;

	if (!AfxParseURL(ConfigFileUrl, dwServiceType, ServerAddr, FileObjectName, nPort)) 
	{
		return -1; //解析地址错误
	}

	CInternetSession Session;
	CHttpFile*       pHttpFile = NULL;

	CHttpConnection* pHttpConn = Session.GetHttpConnection(ServerAddr, nPort);

	if (NULL == pHttpConn) 
	{
		Session.Close();
		return -2; //建立连接错误
	}

	pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_GET, FileObjectName, 0, 1, 0, 0, INTERNET_FLAG_RELOAD);

	if (NULL == pHttpFile) 
	{
		Session.Close();
		return -3; //发送GET请求失败
	}

	pHttpFile->SendRequest();
	pHttpFile->QueryInfoStatusCode(dwRet);

	if (HTTP_STATUS_OK != dwRet) 
	{
		Session.Close();
		delete pHttpConn;
		pHttpConn = NULL;
		delete pHttpFile;
		pHttpFile = NULL;
		return -4;// 服务器不接受请求 
	}

	UINT nFileSzie = (UINT)pHttpFile->GetLength();
	DWORD dwRead = 1;
	CHAR* szBuffer = new CHAR[nFileSzie + 1];
	CHAR* pFileContent = new CHAR[nFileSzie + 1];
	memset(pFileContent, 0, (size_t)(nFileSzie + 1));
	TRY
	{
		/*CString FileStr = SavePath+_T("/")+ FileObjectName;
		CFile AssetConfigFile(FileStr,CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive);*/
		while (dwRead > 0) 
		{
			
			memset(szBuffer, 0, (size_t)(nFileSzie + 1));
			dwRead = pHttpFile->Read(szBuffer, nFileSzie);
			//pHttpFile->
			//AssetConfigFile.Write(szBuffer, dwRead);
			strncat_s(pFileContent, (rsize_t)(nFileSzie+1),szBuffer, (rsize_t)(dwRead+1));
			
		}
		ConfigFileContent = std::string(pFileContent);
		
		
		//AssetConfigFile.Close();

		delete[]szBuffer;
		szBuffer = NULL;

		delete[]pFileContent;
		pFileContent = NULL;


		delete pHttpConn;
		pHttpConn = NULL;
		delete pHttpFile;
		pHttpFile = NULL;

		Session.Close();

	}
	CATCH(CFileException, e) 
	{
		delete[]szBuffer;
		szBuffer = NULL;

		delete pHttpConn;
		pHttpConn = NULL;
		delete pHttpFile;
		pHttpFile = NULL;

		Session.Close();

		return -7;//读写文件异常
	}
	END_CATCH
	return 0;
}

bool CHttpAssetsConfigFile::CompareAssetConfigFile(HWND Wnd)
{
	CString AssetConfigFileLastDirName = m_ConfigFilePath + _T("DownloadFile");

	std::string ConfigContent;
	INT iRet = DownloadAssetConfigFile(_T("http://192.168.0.108:80/AssetsDownloadConfig.json"), ConfigContent);
	
	if (0 == iRet) 
	{
		std::vector<AssetItem> NeedDownloadAssetArray;

		Json::Value  NewRoot;
		Json::CharReaderBuilder NewReader;
		std::string  ErrorStr;
		std::vector<AssetItem> NewAssetArray;
		if (NewReader.newCharReader()->parse(ConfigContent.c_str(), (ConfigContent.c_str() + ConfigContent.length()), &NewRoot, &ErrorStr))
		{
			size_t nAssetSize = NewRoot["Assets"].size();
			for (size_t Index = 0; Index < nAssetSize; ++Index)
			{
				Json::Value  AssetItemValue = NewRoot["Assets"][Index];
			
				CString AssetName(AssetItemValue["Name"].asCString());
				CString AssetMd5(AssetItemValue["md5"].asCString());
				NewAssetArray.push_back(AssetItem(AssetName, AssetMd5));
			}
		
		}

		CString OldAssetConfigFilePath = m_ConfigFilePath + _T("/DownloadFile/AssetsDownloadConfig.json");
		if (PathFileExists(OldAssetConfigFilePath))
		{
			std::ifstream is;
			is.open(OldAssetConfigFilePath, std::ios::binary);

			Json::Value  OldRoot;
			Json::CharReaderBuilder OldReader;
			std::vector<AssetItem> OldAssetArray;
			if (Json::parseFromStream(OldReader, is, &OldRoot, &ErrorStr))
			{
				size_t nAssetSize = OldRoot["Assets"].size();
				for (size_t Index = 0; Index < nAssetSize; ++Index)
				{
					Json::Value  AssetItemValue = OldRoot["Assets"][Index];
					CString AssetName(AssetItemValue["Name"].asCString());
					CString AssetMd5(AssetItemValue["md5"].asCString());
					OldAssetArray.push_back(AssetItem(AssetName, AssetMd5));
				}

			}

			std::vector<AssetItem> NeedDownloadAssetArray;
			for (auto NewItem : NewAssetArray) 
			{
				bool bFind = false;
				for (auto OldItem : OldAssetArray) 
				{
					if (NewItem == OldItem) 
					{
						bFind = true;
						break;
					}
				}

				if (!bFind) 
				{
					NeedDownloadAssetArray.push_back(NewItem);
					
				}
			}


		}
		else
		{
		
			NeedDownloadAssetArray = NewAssetArray;
		}

		TRY
		{
			
			CString OldAssetConfigFileSaveDir = m_ConfigFilePath + _T("/DownloadFile");
			if (!PathFileExists(OldAssetConfigFileSaveDir))
			{
				if (0 == CreateDirectory(OldAssetConfigFileSaveDir, NULL))
				{
					return false; //文件路劲错误
				}
			}

			//  覆盖原配置文件
			std::ofstream of;
			of.open(OldAssetConfigFilePath, std::ios_base::out | std::ios_base::trunc);
			
			of << ConfigContent;
			of.close();
		}
		CATCH(CFileException, e)
		{

		}
		END_CATCH

		const CString  SaveDiskDir = m_ConfigFilePath + _T("/DownloadAssets");
		if (!PathFileExists(SaveDiskDir))
		{
			if (0 == CreateDirectory(SaveDiskDir, NULL))
			{
				return false; //文件路劲错误
			}
		}

		for (auto Asset : NeedDownloadAssetArray) 
		{
			CHttpAssetDownload* DLDAsset = new CHttpAssetDownload(SaveDiskDir, _T("http://192.168.0.108:80/Assets"), Asset.AssetName);
			DLDAsset->DownLoad(Wnd);
		//	delete DLDAsset;
			//DLDAsset = NULL;
		}

		//MessageBox(NULL, _T("Download Completed"),_T(""),0);
	}
	else 
	{
	
	
	}

	

	return false;
}

void CHttpAssetsConfigFile::CompareFileWithAssetConfig()
{
}

CHttpAssetDownload::CHttpAssetDownload()
{

}

CHttpAssetDownload::CHttpAssetDownload(const CString& AssetSaveDiskD,const CString& AssetURL,const CString & AssetN)
	:AssetSaveDiskDir(AssetSaveDiskD),AssetsUrlDir(AssetURL),AssetsName(AssetN)
{
}

CHttpAssetDownload::~CHttpAssetDownload()
{
}

bool CHttpAssetDownload::DownLoad(HWND wnd)
{

	if (!PathFileExists(AssetSaveDiskDir))
	{
		if (0 == CreateDirectory(AssetSaveDiskDir, NULL))
		{
			return false; //文件路劲错误
		}
	}

	CallbackWnd = wnd;

	AfxBeginThread((AFX_THREADPROC)ThreadProc, this);


	return true;
}

UINT CHttpAssetDownload::ThreadProc(LPVOID pPram)
{
	CHttpAssetDownload* Asset = (CHttpAssetDownload*)pPram;
	if (NULL == Asset) 
	{
		return 0;
	}

	CString AssetUrl = Asset->GetAssetsUrlDir() + _T("/") + Asset->GetAssetsAssetsName();
	CString SavePath = Asset->GetAssetSaveDiskDir() + _T("/") + Asset->GetAssetsAssetsName();

	/*
	DeleteUrlCacheEntry(AssetUrl);
	DownloadProgress  progress;
	progress.wnd = Asset->GetWnd();
	progress.AssetDownloader = Asset;
	URLDownloadToFile(NULL, AssetUrl, SavePath, NULL, dynamic_cast<IBindStatusCallback*>(&progress));
	*/
	
	unsigned short  nPort;
	CString ServerAddr, FileObjectName;
	DWORD  dwServiceType, dwRet;

	
	if (!AfxParseURL(AssetUrl, dwServiceType, ServerAddr, FileObjectName, nPort))
	{
		return -1; //解析地址错误
	}

	CInternetSession Session;
	CHttpFile*       pHttpFile = NULL;
	const UINT    nTimeOut = 5000;
	Session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
	Session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 10);
	CHttpConnection* pHttpConn = Session.GetHttpConnection(ServerAddr, nPort);

	if (NULL == pHttpConn)
	{
		Session.Close();
		return -1; //建立连接错误
	}

	CHttpFile* HeadHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_HEAD, FileObjectName, 0, 1, 0, 0, INTERNET_FLAG_RELOAD);
	
	if (NULL == HeadHttpFile) 
	{
		Session.Close();
		return -1;
	}

	HeadHttpFile->AddRequestHeaders(_T("Content-Type: application/x-www-form-urlencoded"));
	HeadHttpFile->AddRequestHeaders(_T("Accept:*/*"));

	HeadHttpFile->SendRequest();


	CString FileTotalSize;
	
	FileSizeInfo* pFileSizeInfo = new FileSizeInfo();
	
	if (HeadHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH , FileTotalSize))
	{
		::PostMessage(Asset->GetWnd(), WM_NOTIFY_PROGRESS_RANGE, (WPARAM)0, (LPARAM)0);
		pFileSizeInfo->TotalSize = _ttoll(FileTotalSize);
		
	}
	else
	{
		DWORD ErrorCode = GetLastError();
		return -1;
	}




	pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_GET, FileObjectName, 0, 1, 0, 0, INTERNET_FLAG_RELOAD);

	if (NULL == pHttpFile)
	{
		Session.Close();
		return -1; //发送GET请求失败
	}


	pHttpFile->SendRequest();
	pHttpFile->QueryInfoStatusCode(dwRet);

	if (HTTP_STATUS_OK != dwRet)
	{
		Session.Close();
		delete pHttpConn;
		pHttpConn = NULL;
		delete pHttpFile;
		pHttpFile = NULL;
		return -1;// 服务器不接受请求 
	}

	
	
	UINT nFileSzie = (UINT)pHttpFile->GetLength();
	UINT dwRead = 1;

	CHAR* szBuffer = new CHAR[nFileSzie + 1];

	
	
	TRY
	{
		
		CFile AssetConfigFile(SavePath,CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive);
		//std::ofstream of;
		//of.open(SavePath, std::ios_base::app|std::ios_base::out|std::ios_base::binary);
		LONGLONG ReadTotalSize = 0;
		
		while (dwRead > 0)
		{

			memset(szBuffer, 0, (size_t)(nFileSzie + 1));
			dwRead = pHttpFile->Read(szBuffer, nFileSzie);
			//pHttpFile->
			AssetConfigFile.Write(szBuffer, dwRead);
			//of << szBuffer;
			
			ReadTotalSize += dwRead;

			pFileSizeInfo->DownloadedSize = ReadTotalSize;

			::PostMessage(Asset->GetWnd(), WM_UPDATE_PROGRESS, (WPARAM)(pFileSizeInfo), 0);

		}
		//of.close();
		AssetConfigFile.Close();

		::PostMessage(Asset->GetWnd(), WM_UPDATE_PROGRESS, (WPARAM)(pFileSizeInfo), 1);

		delete[]szBuffer;


		szBuffer = NULL;

		delete pHttpConn;
		pHttpConn = NULL;
		delete pHttpFile;
		pHttpFile = NULL;

		Session.Close();

	}
		CATCH(CFileException, e)
	{
		delete[]szBuffer;
		szBuffer = NULL;

		delete pHttpConn;
		pHttpConn = NULL;
		delete pHttpFile;
		pHttpFile = NULL;

		Session.Close();

		return -1;//读写文件异常
	}
	END_CATCH

	if (Asset) delete Asset; Asset = NULL;
	return  0;

}
