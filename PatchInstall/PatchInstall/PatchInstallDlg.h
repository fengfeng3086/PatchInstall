
// PatchInstallDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "GdiPlus.h"
using namespace Gdiplus;

#define WM_UPDATE_PROGRESS WM_USER+200
#define WM_NOTIFY_PROGRESS_RANGE WM_USER+201

// CPatchInstallDlg 对话框
class CPatchInstallDlg : public CDialogEx
{
// 构造
public:
	CPatchInstallDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CPatchInstallDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATCHINSTALL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
