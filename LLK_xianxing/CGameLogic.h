#pragma once
#include <stack>
#include "global.h"
using namespace std;
class CGameLogic
{
public:
	int** GameMap;	// 游戏地图，保存每个位置的图片编号
	int rows;		// 行数
	int cols;		// 列数
	int picNum;		// 图片种类数

	~CGameLogic();
	int GetElement(int nRow, int nCol);  //获取指定位置图片的ID
	void InitMap(int nRows, int nCols, int nPicNum);  //初始化地图

	bool IsLink(Vertex v1, Vertex v2);  //判断两张图片是否可以连接
	void Clear(Vertex v1, Vertex v2);   //消除两张图片
	void ClearVerList();				//清空路径列表
	stack<Vertex> GetVerList();			//获取路径列表
	bool isBlank();						//判断地图上是否还有图片

	bool GetPrompt(Vertex& v1, Vertex& v2);	//获取提示信息
	void ResetMap();						//重排地图
	void ClearMap();						//清空地图
private:
	stack<Vertex> verList;  //用于保存两个图片相连的路径

	bool LinkInRow(Vertex v1, Vertex v2);  //判断两张图片是否在同一行且中间没有其他图片
	bool LinkInCol(Vertex v1, Vertex v2);  //判断两张图片是否在同一列且中间没有其他图片
	bool LinkOneCorner(Vertex v1, Vertex v2);  //判断两张图片是否可以通过一个转角连接
	bool LinkTwoCorner(Vertex v1, Vertex v2);  //判断两张图片是否可以通过两个转角连接
	bool LinkY(int Row1, int Row2, int Col);  //判断两行之间的某一列是否没有图片
	bool LinkX(int Row, int Col1, int Col2);  //判断两列之间的某一行是否没有图片
};

