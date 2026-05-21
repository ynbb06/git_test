#pragma once
#include "CGameLogic.h"
class CGameControl
{
public:
	Vertex selFirst;		//第一次选中的图片坐标
	Vertex selSecond;		//第二次选中的图片坐标
	Vertex promptFirst;		//提示的第一张图片坐标
	Vertex promptSecond;	//提示的第二张图片坐标

	CGameLogic m_GameLogic;

	void SetFirstSelect(int nRow, int nCol);	//设置第一次选中的图片坐标
	void SetSecondSelect(int nRow, int nCol);	//设置第二次选中的图片坐标

	int GetElement(int nRow, int nCol);			//获取地图上指定位置的图片编号
	bool StartGame(int nRows, int nCols, int nPicNum);	//开始游戏，供CGameDlg调用

	bool Link(stack<Vertex>& verList);			//判断两张图片是否可以连接，相连路径保存在verList中
	bool isWin();								//判断游戏是否胜利

	void SetFirstPrompt(int nRow, int nCol);	//设置提示的第一张图片坐标
	void SetSecondPrompt(int nRow, int nCol);	//设置提示的第二张图片坐标
	//获取提示信息，如果有提示则返回true，并将提示的两张图片坐标保存在promptFirst和promptSecond中，相连路径保存在verList中
	bool GetPrompt(stack<Vertex>& verList);

	void ResetMap();							//重排地图
	void ClearMap();							//清空地图
};

