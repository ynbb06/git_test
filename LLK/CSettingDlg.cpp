// CSettingDlg.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "afxdialogex.h"
#include "CSettingDlg.h"
#include "ParamSetting.h"


// CSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTING_DIALOG, pParent)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_CLOSE, &CSettingDlg::OnBnClickedRadioClose)
	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_OPEN, &CSettingDlg::OnBnClickedRadioOpen)
END_MESSAGE_MAP()

void CSettingDlg::InitBackground(void)
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

BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitBackground();

	//增加下拉框数据的初始化
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_THEME);
	pCombo->InsertString(0, _T("水果乐园主题"));
	pCombo->SetCurSel(ParamSetting::ThemeNo);	//设置当前选中项

	CEdit* editRow = (CEdit*)GetDlgItem(IDC_EDIT_ROW);
	CEdit* editCol = (CEdit*)GetDlgItem(IDC_EDIT_COL);
	CEdit* editPic = (CEdit*)GetDlgItem(IDC_EDIT_PIC);

	//将行数、列数和图片数量显示在编辑框中
	CString rows, cols, pics;
	rows.Format(_T("%d"), ParamSetting::Rows);
	cols.Format(_T("%d"), ParamSetting::Cols);
	pics.Format(_T("%d"), ParamSetting::PicNum);

	editRow->SetWindowText(rows);
	editCol->SetWindowText(cols);
	editPic->SetWindowText(pics);

	CButton* radioOpen = (CButton*)GetDlgItem(IDC_RADIO_OPEN);
	CButton* radioClose = (CButton*)GetDlgItem(IDC_RADIO_CLOSE);
	//根据背景音乐开关状态设置单选按钮的选中状态
	if (ParamSetting::BGMOn)
		radioOpen->SetCheck(BST_CHECKED);
	else
		radioClose->SetCheck(BST_CHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSettingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	dc.BitBlt(0, 0, 500, 372, &m_dcMem, 0, 0, SRCCOPY);
}

void CSettingDlg::OnBnClickedOk()
{
	//提取主题信息
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_THEME);
	int theme = pCombo->GetCurSel();
	SetTheme(theme);

	//提取地图信息
	CEdit* editRow = (CEdit*)GetDlgItem(IDC_EDIT_ROW);
	CEdit* editCol = (CEdit*)GetDlgItem(IDC_EDIT_COL);
	CEdit* editPic = (CEdit*)GetDlgItem(IDC_EDIT_PIC);

	CString row, col, pic;
	int rows, cols, pics;

	editRow->GetWindowText(row);
	if (!IsNum(row))
	{
		MessageBox(_T("请输入有效的行数！"));
		return;
	}
	rows = _ttoi(row);
	if (rows < 1 || rows>10)
	{
		MessageBox(_T("行数必须在1到10之间！"));
		return;
	}
	
	editCol->GetWindowText(col);
	if (!IsNum(col))
	{
		MessageBox(_T("请输入有效的列数！"));
		return;
	}
	cols = _ttoi(col);
	if (cols < 1 || cols>16)
	{
		MessageBox(_T("列数必须在1到16之间！"));
		return;
	}

	editPic->GetWindowText(pic);
	if (!IsNum(pic))
	{
		MessageBox(_T("请输入有效的图片数量！"));
		return;
	}
	pics = _ttoi(pic);
	if (pics < 1 || pics>10)
	{
		MessageBox(_T("图片数量必须在1到10之间！"));
		return;
	}

	if ((rows * cols) % (pics * 2) != 0)
	{
		MessageBox(_T("行数×列数必须是图片数量的2倍的整数倍！"));
		return;
	}

	SetMap(rows, cols, pics);

	SetBGM(BGMOn);

	CDialogEx::OnOK();
}

void CSettingDlg::OnBnClickedRadioOpen()
{
	BGMOn = true;
}

void CSettingDlg::OnBnClickedRadioClose()
{
	BGMOn = false;
}

void CSettingDlg::SetTheme(int theme)
{
	ParamSetting::ThemeNo = theme;

	switch (theme)
	{
	case 0:
		ParamSetting::BGPath = _T("theme\\picture\\fruit_bg.bmp");
		ParamSetting::ElementPath = _T("theme\\picture\\fruit_element.bmp");
		ParamSetting::MaskPath = _T("theme\\picture\\fruit_mask.bmp");
		ParamSetting::ClickSoundPath = _T("theme\\music\\fruit_click.wav");
		ParamSetting::ClearSoundPath = _T("theme\\music\\fruit_clear.wav");
		break;
	}
}

void CSettingDlg::SetMap(int row, int col, int picNum)
{
	ParamSetting::Rows = row;
	ParamSetting::Cols = col;
	ParamSetting::PicNum = picNum;
}

void CSettingDlg::SetBGM(bool on)
{
	ParamSetting::BGMOn = on;
}

bool CSettingDlg::IsNum(CString s)
{
	for (int i = 0; i < s.GetLength(); i++)
	{
		if (s[i] < '0' || s[i] > '9')
			return false;
	}
	return true;
}