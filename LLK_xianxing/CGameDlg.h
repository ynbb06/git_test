#pragma once
#include "afxdialogex.h"
#include "CGameControl.h"
#include "ParamSetting.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// CGameDlg 对话框

class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)

public:
	CGameDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif
private:
	int GameMode;			//游戏模式
	CPoint m_GameRegionTop;	//窗体地图左上角坐标
	CSize m_sizeElem;		//每个图片的大小
	CRect m_GameRegion;		//地图所在的矩形框
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;	//系统图标

	CDC m_dcMem;	//内存DC
	CDC m_dcBG;		//背景
	CDC m_dcEle;	//元素
	CDC m_dcMask;	//掩码

	bool playing;	//游戏状态
	bool firstSelect;	//是否第一次选中图片

	CProgressCtrl GameProgress;	//游戏进度条
	int TimeCounter;			//游戏时间计数器
	
	CGameControl m_GameC;
	DECLARE_MESSAGE_MAP()
public:
	void SetGameMode(int mode);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClickedBtnStart();
	afx_msg void OnClickedBtnStop();
	afx_msg void OnClickedBtnPrompt();
	afx_msg void OnClickedBtnReset();

private:
	void InitMode(int mode);
	void InitBackground(void);
	void SetButton(bool start, bool stop, bool prompt, bool reset);
	void InitElement(void);
	void UpdateMap(void);
	void DrawTipFrame(int nRow, int nCol);
	void DrawTipLine(stack<Vertex>& verList);
	void SetGameProgress(int range);

	void TurnonBGM(CString path);
	void TurnoffBGM();
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	//鼠标左键抬起事件
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
};

