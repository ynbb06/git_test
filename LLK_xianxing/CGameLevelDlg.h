#pragma once
#include "afxdialogex.h"


// CGameLevelDlg 对话框

class CGameLevelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameLevelDlg)

public:
	CGameLevelDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameLevelDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEVEL_DIALOG };
#endif

protected:
	CDC m_dcMem;	//内存DC

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void SetLevel(int level);
	void InitBackground(void);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnEasy();
	afx_msg void OnBnClickedBtnMed();
	afx_msg void OnBnClickedBtnHard();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
