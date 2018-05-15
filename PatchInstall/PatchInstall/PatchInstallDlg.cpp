
// PatchInstallDlg.cpp : ʵ���ļ�
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


// CPatchInstallDlg �Ի���
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


// CPatchInstallDlg ��Ϣ�������

BOOL CPatchInstallDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	GdiplusStartup(&m_GdiToken, &m_GdiInput, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPatchInstallDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CHttpAssetsConfigFile ConfigFile;
	ConfigFile.CompareAssetConfigFile(this->m_hWnd);
}


HBRUSH CPatchInstallDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	if (nCtlColor == CTLCOLOR_STATIC) 
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		hbr = (HBRUSH) GetStockObject(NULL_BRUSH);
	}
	
	return hbr;
}
