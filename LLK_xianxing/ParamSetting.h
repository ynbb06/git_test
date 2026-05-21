#pragma once
#include "afxdialogex.h"
class ParamSetting
{
public:
	static CString BGPath;			//背景图片路径
	static CString ElementPath;		//元素图片路径
	static CString MaskPath;		//掩码图片路径
	static CString ClickSoundPath;	//点击音效路径
	static CString ClearSoundPath;	//消除音效路径
	static CString BGMSoundPath;	//背景音乐路径
	static int Rows;				//行数
	static int Cols;				//列数
	static int PicNum;				//图片数量
	static bool BGMOn;				//背景音乐开关
	static int ThemeNo;				//主题编号
};