
// PatchInstallDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "GdiPlus.h"
using namespace Gdiplus;

#define WM_UPDATE_PROGRESS WM_USER+200
#define WM_NOTIFY_PROGRESS_RANGE WM_USER+201

// CPatchInstallDlg �Ի���
class CPatchInstallDlg : public CDialogEx
{
// ����
public:
	CPatchInstallDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CPatchInstallDlg();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATCHINSTALL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnUpdateProgress(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnNotifyProgressRange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButton1();
	CStatic ProcessText;
	CProgressCtrl ProgressBar;
	CStatic TestTotalSizeText;
	CStatic TestDownloadedSizeText;

protected:
	ULONG_PTR  m_GdiToken;
	GdiplusStartupInput m_GdiInput;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
