// CGameLevelDlg.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "afxdialogex.h"
#include "CGameLevelDlg.h"
#include "CGameDlg.h"


// CGameLevelDlg 对话框

IMPLEMENT_DYNAMIC(CGameLevelDlg, CDialogEx)

CGameLevelDlg::CGameLevelDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LEVEL_DIALOG, pParent)
{

}

CGameLevelDlg::~CGameLevelDlg()
{
}

void CGameLevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameLevelDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_Easy, &CGameLevelDlg::OnBnClickedBtnEasy)
	ON_BN_CLICKED(IDC_BTN_Med, &CGameLevelDlg::OnBnClickedBtnMed)
	ON_BN_CLICKED(IDC_BTN_Hard, &CGameLevelDlg::OnBnClickedBtnHard)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CGameLevelDlg::InitBackground(void)
{
	CBitmap bmpMain;
	bmpMain.LoadBitmapW(IDB_LEVEL);

	CClientDC dc(this);
	m_dcMem.CreateCompatibleDC(&dc);
	m_dcMem.SelectObject(bmpMain);

	CRect rtWin;
	CRect rtClient;
	this->GetWindowRect(rtWin);
	this->GetClientRect(rtClient);
	int nSpanWidth = rtWin.Width() - rtClient.Width();
	int nSpanHeight = rtWin.Height() - rtClient.Height();

	MoveWindow(0, 0, 800 + nSpanWidth, 508 + nSpanHeight);
	CenterWindow();
}

void CGameLevelDlg::SetLevel(int level)
{
	this->ShowWindow(SW_HIDE);
	CGameDlg dlg;
	dlg.SetGameMode(level);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

// CGameLevelDlg 消息处理程序

void CGameLevelDlg::OnBnClickedBtnEasy()
{
	SetLevel(2);
}

void CGameLevelDlg::OnBnClickedBtnMed()
{
	SetLevel(3);
}

void CGameLevelDlg::OnBnClickedBtnHard()
{
	SetLevel(4);
}

BOOL CGameLevelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitBackground();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CGameLevelDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	dc.BitBlt(0, 0, 800, 508, &m_dcMem, 0, 0, SRCCOPY);
}
