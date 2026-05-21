// CHelpDlg.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "afxdialogex.h"
#include "CHelpDlg.h"


// CHelpDlg 对话框

IMPLEMENT_DYNAMIC(CHelpDlg, CDialogEx)

CHelpDlg::CHelpDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HELP_DIALOG, pParent)
{

}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHelpDlg 消息处理程序

BOOL CHelpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitBackground();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CHelpDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	dc.BitBlt(0, 0, 500, 372, &m_dcMem, 0, 0, SRCCOPY);
}

void CHelpDlg::InitBackground(void)
{
	CBitmap bmpMain;
	bmpMain.LoadBitmapW(IDB_SETTING);

	CClientDC dc(this);
	m_dcMem.CreateCompatibleDC(&dc);
	m_dcMem.SelectObject(bmpMain);

	CRect rtWin;
	CRect rtClient;
	this->GetWindowRect(rtWin);
	this->GetClientRect(rtClient);
	int nSpanWidth = rtWin.Width() - rtClient.Width();
	int nSpanHeight = rtWin.Height() - rtClient.Height();

	MoveWindow(0, 0, 500 + nSpanWidth, 372 + nSpanHeight);
	CenterWindow();
}