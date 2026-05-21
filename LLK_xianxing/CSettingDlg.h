#pragma once
#include "afxdialogex.h"


// CSettingDlg 对话框

class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSettingDlg();
	CDC m_dcMem;	//内存DC

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void InitBackground(void);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioClose();
	afx_msg void OnBnClickedRadioOpen();

	void SetTheme(int theme);
	void SetMap(int row, int col, int picNum);
	void SetBGM(bool on);
	bool IsNum(CString s);
	bool BGMOn;
};
