
// PatchInstall.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPatchInstallApp: 
// �йش����ʵ�֣������ PatchInstall.cpp
//

class CPatchInstallApp : public CWinApp
{
public:
	CPatchInstallApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPatchInstallApp theApp;
