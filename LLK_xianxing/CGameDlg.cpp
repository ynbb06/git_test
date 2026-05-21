// CGameDlg.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "afxdialogex.h"
#include "CGameDlg.h"


// CGameDlg 对话框

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::~CGameDlg()
{
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRO_TIME, GameProgress);
}


BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_START, &CGameDlg::OnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CGameDlg::OnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_PROMPT, &CGameDlg::OnClickedBtnPrompt)
	ON_BN_CLICKED(IDC_BTN_RESET, &CGameDlg::OnClickedBtnReset)
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGameDlg 消息处理程序
void CGameDlg::InitBackground(void)
{
	HANDLE Backbmp = ::LoadImageW(NULL, _T("theme\\picture\\fruit_bg.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	CClientDC dc(this);
	m_dcBG.CreateCompatibleDC(&dc);
	m_dcBG.SelectObject(Backbmp);

	m_dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, 800, 600);
	m_dcMem.SelectObject(&bmpMem);

	m_dcMem.BitBlt(0, 0, 800, 600, &m_dcBG, 0, 0, SRCCOPY);

	CRect rtWin;
	CRect rtClient;
	this->GetWindowRect(rtWin);
	this->GetClientRect(rtClient);
	int nSpanWidth = rtWin.Width() - rtClient.Width();
	int nSpanHeight = rtWin.Height() - rtClient.Height();

	MoveWindow(0, 0, 800 + nSpanWidth, 600 + nSpanHeight);
	CenterWindow();
}


BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	this->ModifyStyleEx(0, WS_EX_APPWINDOW);

	//设置背景
	InitBackground();

	//设置模式
	InitMode(this->GameMode);

	//设置游戏窗口按钮的初始状态
	SetButton(TRUE, FALSE, FALSE, FALSE);

	//初始化元素和掩码
	InitElement();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CGameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	dc.BitBlt(0, 0, 800, 600, &m_dcMem, 0, 0, SRCCOPY);
}

void CGameDlg::SetGameMode(int mode)
{
	this->GameMode = mode;
}

void CGameDlg::InitMode(int mode)
{
	//根据ID获取控件
	CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PRO_TIME);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TIME);
	if (mode == 1)
	{
		this->SetWindowTextW(_T("欢乐连连看——基本模式"));
		pEdit->EnableWindow(FALSE); //禁用编辑框
	}
	else if(mode==0)
	{
		this->SetWindowTextW(_T("欢乐连连看——休闲模式"));
		pProgress->ShowWindow(SW_HIDE); //隐藏进度条
		pEdit->ShowWindow(SW_HIDE); //隐藏编辑框
	}
	else
		this->SetWindowTextW(_T("欢乐连连看——关卡模式"));
}

void CGameDlg::OnClickedBtnStart()
{
	//初始化地图，地图用一个二维数组来表示，数组元素的值为图片编号
	bool status = m_GameC.StartGame(ParamSetting::Rows, ParamSetting::Cols, ParamSetting::PicNum);
	if (status)
	{
		//设置状态
		playing = true;
		firstSelect = true;

		SetButton(FALSE, TRUE, TRUE, TRUE);
		UpdateMap(); //展示地图信息

		switch (this->GameMode)
		{
			case 1:
			case 2:
				SetGameProgress(EASY);
				break;
			case 3:
				SetGameProgress(MED);
				break;
			case 4:
				SetGameProgress(HARD);
				break;
			default:
				break;
		}

		//如果背景音乐开关打开，则播放背景音乐
		if (ParamSetting::BGMOn)
			TurnonBGM(ParamSetting::BGMSoundPath);
	}
	else
		AfxMessageBox(L"行列和图片数量不匹配，无法绘制地图！", MB_OKCANCEL);

	InvalidateRect(FALSE);
}

void CGameDlg::OnClickedBtnStop()
{
	if (playing)
	{
		this->GetDlgItem(IDC_BTN_STOP)->SetWindowText(L"重新开始");
		playing = false;
	}
	else
	{
		this->GetDlgItem(IDC_BTN_STOP)->SetWindowText(L"暂停游戏");
		playing = true;
	}
}

void CGameDlg::OnClickedBtnPrompt()
{
	if (!playing)
		return;
	stack<Vertex> verList;
	bool bSuc = m_GameC.GetPrompt(verList);
	if (bSuc)
	{
		m_dcMem.BitBlt(0, 0, 800, 600, &m_dcBG, 0, 0, SRCCOPY);  //重绘背景
		UpdateMap();  //更新地图信息
		DrawTipFrame(m_GameC.promptFirst.row, m_GameC.promptFirst.col);
		DrawTipFrame(m_GameC.promptSecond.row, m_GameC.promptSecond.col);
		DrawTipLine(verList);  //绘制提示线
	}
}

void CGameDlg::OnClickedBtnReset()
{
	if (!playing)
		return;
	m_GameC.ResetMap();
	firstSelect = true;
	m_dcMem.BitBlt(0, 0, 800, 600, &m_dcBG, 0, 0, SRCCOPY);  //重绘背景
	UpdateMap();  //更新地图信息
	InvalidateRect(FALSE);
}

void CGameDlg::SetButton(bool start, bool stop, bool prompt, bool reset)
{
	this->GetDlgItem(IDC_BTN_START)->EnableWindow(start);
	this->GetDlgItem(IDC_BTN_STOP)->EnableWindow(stop);
	this->GetDlgItem(IDC_BTN_PROMPT)->EnableWindow(prompt);
	this->GetDlgItem(IDC_BTN_RESET)->EnableWindow(reset);
}

void CGameDlg::InitElement(void)
{
	CClientDC dc(this);
	HANDLE Elebmp = ::LoadImageW(NULL, ParamSetting::ElementPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_dcEle.CreateCompatibleDC(&m_dcMem);
	m_dcEle.SelectObject(Elebmp);

	HANDLE Maskbmp = ::LoadImageW(NULL, ParamSetting::MaskPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_dcMask.CreateCompatibleDC(&m_dcMem);
	m_dcMask.SelectObject(Maskbmp);
}

void CGameDlg::UpdateMap(void)
{
	int nTop = MAP_TOP;
	int nLeft = MAP_LEFT;
	int nElemW = PIC_WIDTH;
	int nElemH = PIC_HEIGHT;
	for (int i = 0;i < ParamSetting::Rows;i++) 
	{
		for (int j = 0;j <ParamSetting::Cols;j++)
		{
			int nElemID = m_GameC.GetElement(i, j);
			m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH, &m_dcMask, 0, nElemID * nElemH, SRCPAINT);
			m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH, &m_dcEle, 0, nElemID * nElemH, SRCAND);
		}
	}
}
void CGameDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(!playing)
		return;
	//判断鼠标点击位置是否在地图区域内
	if (point.y < m_GameRegion.top || point.y > m_GameRegion.bottom || point.x < m_GameRegion.left || point.x > m_GameRegion.right)
		return CDialogEx::OnLButtonUp(nFlags, point);

	int nRow = (point.y - m_GameRegionTop.y) / m_sizeElem.cy;
	int nCol = (point.x - m_GameRegionTop.x) / m_sizeElem.cx;
	//判断点击位置是否有图片
	if (nRow > ParamSetting::Rows - 1 || nCol > ParamSetting::Cols - 1)
		return CDialogEx::OnLButtonUp(nFlags, point);
	//如果点击位置没有图片，则返回
	if (m_GameC.GetElement(nRow, nCol) == BLANK)
		return CDialogEx::OnLButtonUp(nFlags, point);

	if (firstSelect)  //第一次选中图片
	{
		m_GameC.SetFirstSelect(nRow, nCol);
		DrawTipFrame(nRow, nCol);  //绘制边框
		PlaySound(ParamSetting::ClickSoundPath, NULL, SND_FILENAME | SND_ASYNC);  //播放点击音效
	}
	else  //第二次选中图片
	{
		m_GameC.SetSecondSelect(nRow, nCol);
		DrawTipFrame(nRow, nCol);  //绘制边框

		//判断两张图片是否可以连接
		stack<Vertex> verList;
		bool bSuc = m_GameC.Link(verList);
		if (bSuc)  //如果两张图片可以连接
		{
			DrawTipLine(verList);  //绘制连接线
			PlaySound(ParamSetting::ClearSoundPath, NULL, SND_FILENAME | SND_ASYNC);  //播放消除音效
			m_dcMem.BitBlt(0, 0, 800, 600, &m_dcBG, 0, 0, SRCCOPY);  //重绘背景
			UpdateMap();  //更新地图信息
		}
		Sleep(200);  //暂停200毫秒，等待玩家看到连接线
		InvalidateRect(FALSE);  //刷新界面
		if (bSuc && m_GameC.isWin())
		{
			MessageBox(TEXT("恭喜你！获胜！"));
			//设置按钮
			SetButton(TRUE, FALSE, FALSE, FALSE);
			playing = false;
		}
	}
	firstSelect = !firstSelect;  //切换选中状态
	CDialogEx::OnLButtonUp(nFlags, point);
}

CGameDlg::CGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAME_DIALOG, pParent)
{
	m_GameRegionTop.x = MAP_LEFT;
	m_GameRegionTop.y = MAP_TOP;
	m_sizeElem.cx = PIC_WIDTH;
	m_sizeElem.cy = PIC_HEIGHT;

	m_GameRegion.top = MAP_TOP;
	m_GameRegion.left = MAP_LEFT;
	m_GameRegion.right = MAP_LEFT + PIC_WIDTH * ParamSetting::Cols;
	m_GameRegion.bottom = MAP_TOP + PIC_HEIGHT * ParamSetting::Rows;

	playing = false;
	firstSelect = false;
	GameMode = 0;
}

void CGameDlg::DrawTipFrame(int nRow, int nCol)
{
	CClientDC dc(this);
	CBrush brush(RGB(233, 43, 43));
	CRect rtTipFrame;
	rtTipFrame.top = m_GameRegionTop.y + nRow * m_sizeElem.cy;
	rtTipFrame.left = m_GameRegionTop.x + nCol * m_sizeElem.cx;
	rtTipFrame.right = rtTipFrame.left + m_sizeElem.cx;
	rtTipFrame.bottom = rtTipFrame.top + m_sizeElem.cy;
	dc.FrameRect(&rtTipFrame, &brush);
}

void CGameDlg::DrawTipLine(stack<Vertex>& verList)
{
	CClientDC dc(this);
	CPen penLine(PS_SOLID, 2, RGB(0, 255, 0));
	CPen* pOldPen = dc.SelectObject(&penLine);
	Vertex vTop;
	CPoint cp;
	if (!verList.empty())
	{
		vTop = verList.top();
		verList.pop();
		cp.x = m_GameRegionTop.x + vTop.col * m_sizeElem.cx + m_sizeElem.cx / 2;
		cp.y = m_GameRegionTop.y + vTop.row * m_sizeElem.cy + m_sizeElem.cy / 2;
		dc.MoveTo(cp);
		while (!verList.empty())
		{
			vTop = verList.top();
			verList.pop();
			cp.x = m_GameRegionTop.x + vTop.col * m_sizeElem.cx + m_sizeElem.cx / 2;
			cp.y = m_GameRegionTop.y + vTop.row * m_sizeElem.cy + m_sizeElem.cy / 2;
			dc.LineTo(cp);
		}
	}
	dc.SelectObject(pOldPen);
}

void CGameDlg::SetGameProgress(int range)
{
	GameProgress.SetRange(0, range);
	GameProgress.SetStep(-1);
	GameProgress.SetPos(range);
	TimeCounter = range;
	this->SetTimer(1, 1000, NULL);  //每隔1秒钟触发一次定时器事件
}

void CGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (playing) 
	{
		CString str;
		str.Format(_T("%d"), TimeCounter - 1);
		SetDlgItemText(IDC_EDIT_TIME, str);
		UpdateData(FALSE);

		TimeCounter--;
		GameProgress.StepIt();

		if (TimeCounter == 0)
		{
			KillTimer(1);  //停止定时器
			m_GameC.ClearMap();  //清空地图
			m_dcMem.BitBlt(0, 0, 800, 600, &m_dcBG, 0, 0, SRCCOPY);
			UpdateMap();
			InvalidateRect(FALSE);  //刷新界面
			MessageBox(TEXT("时间到！游戏失败！"));
			SetButton(TRUE, FALSE, FALSE, FALSE);
			playing = false;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CGameDlg::TurnonBGM(CString path)
{
	// 使用 MCI 命令播放背景音乐，路径用引号包裹以支持包含空格
	CString cmd;
	cmd.Format(_T("open \"%s\" alias bgm"), path);

	MCIERROR err = mciSendString(cmd, NULL, 0, NULL);
	if (err != 0)
		ASSERT(0);

	mciSendString(_T("play bgm repeat"), NULL, 0, NULL); // 循环播放背景音乐
}

void CGameDlg::TurnoffBGM()
{
	mciSendString(_T("stop bgm"), NULL, 0, NULL);	//停止播放背景音乐
	mciSendString(_T("close bgm"), NULL, 0, NULL);	//关闭背景音乐文件
}

void CGameDlg::OnClose()
{
	TurnoffBGM();	//关闭背景音乐
	CDialogEx::OnClose();
}
