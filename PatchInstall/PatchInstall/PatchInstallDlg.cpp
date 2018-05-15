
// PatchInstallDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PatchInstall.h"
#include "PatchInstallDlg.h"
#include "afxdialogex.h"
#include "FileManager.h"



#pragma comment(lib,"gdiplus.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPatchInstallDlg 对话框
CPatchInstallDlg::CPatchInstallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PATCHINSTALL_DIALOG, pParent)
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPatchInstallDlg::~CPatchInstallDlg()
{
	if (m_GdiToken) 
	{
		GdiplusShutdown(m_GdiToken);
	}
}

void CPatchInstallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_ProcessValue, ProcessText);
	DDX_Control(pDX, IDC_PROGRESS1, ProgressBar);
	DDX_Control(pDX, IDC_STATIC_Test2, TestTotalSizeText);
	DDX_Control(pDX, IDC_STATIC_Test1, TestDownloadedSizeText);
}

BEGIN_MESSAGE_MAP(CPatchInstallDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(WM_UPDATE_PROGRESS,OnUpdateProgress)
	ON_BN_CLICKED(IDC_BUTTON1, &CPatchInstallDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_NOTIFY_PROGRESS_RANGE,OnNotifyProgressRange)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPatchInstallDlg 消息处理程序

BOOL CPatchInstallDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	GdiplusStartup(&m_GdiToken, &m_GdiInput, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPatchInstallDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		
		CRect Rc;
		GetWindowRect(&Rc);

		Image BackgroudImg(_T("E://MFCProject//PatchInstall//Debug//background.png"));
		

		Graphics graphic(GetDC()->GetSafeHdc());
		graphic.DrawImage(&BackgroudImg, Rect(0, 0, Rc.Width(), Rc.Height()));
		
		
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPatchInstallDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CPatchInstallDlg::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
{
	FileSizeInfo* pFileSizeInfo = (FileSizeInfo*)(wParam);
	if (NULL == pFileSizeInfo)
	{
		return LRESULT();
	}

	if (1 == lParam) 
	{
		delete pFileSizeInfo;
		pFileSizeInfo = NULL;
		return  LRESULT();
	}

	
	double percentage = (double)(pFileSizeInfo->DownloadedSize * 1.0 / pFileSizeInfo->TotalSize * 100);
	CString text;
	text.Format(_T("%0.2f"), percentage);
	text.Append(_T("%"));
	//ProgressText.SetWindowTextW(text);
	
	CString TotalSize,DownloadedSize;
	
	ProcessText.SetWindowTextW(text);
	
	ProgressBar.SetPos((int)percentage);
	return LRESULT();
}

LRESULT CPatchInstallDlg::OnNotifyProgressRange(WPARAM wParam, LPARAM lParam)
{
	ProgressBar.SetRange(0, 100);
	ProgressBar.SetStep(1);
	ProgressBar.SetPos(0);
	return LRESULT();
}



void CPatchInstallDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CHttpAssetsConfigFile ConfigFile;
	ConfigFile.CompareAssetConfigFile(this->m_hWnd);
}


HBRUSH CPatchInstallDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if (nCtlColor == CTLCOLOR_STATIC) 
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		hbr = (HBRUSH) GetStockObject(NULL_BRUSH);
	}
	
	return hbr;
}
