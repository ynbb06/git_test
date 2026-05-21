#include "pch.h"
#include "CGameLogic.h"

CGameLogic::~CGameLogic()
{
	for (int i = 0;i < rows;i++)
		delete[] GameMap[i];
	delete[] GameMap;
}

int CGameLogic::GetElement(int nRow, int nCol)
{
	return GameMap[nRow][nCol];
}

void CGameLogic::InitMap(int nRows, int nCols, int nPicNum)
{
	//为二维数组分配内存
	this->GameMap = new int* [nRows];
	for (int i = 0;i < nRows;i++) {
		this->GameMap[i] = new int[nCols];
		memset(this->GameMap[i], NULL, sizeof(int) * nCols);
	}

	this->rows = nRows;
	this->cols = nCols;
		
	//用图片编号填充地图
	int nRepeatNum = nRows * nCols / nPicNum;  //每种图片的数量
	int nCount = 0;  //当前图片的数量
	for (int i = 0;i < nPicNum;i++)
	{
		for (int j = 0;j < nRepeatNum;j++)
		{
			GameMap[nCount / nCols][nCount % nCols] = i;  //从左到右、从上到下
			nCount++;
		}
	}

	//将图片随机打乱
	srand((int)time(NULL));
	int nVertexNum = nRows * nCols;
	for (int i = 0;i < nVertexNum;i++)
	{
		int nIndex1 = rand() % nVertexNum;
		int nIndex2 = rand() % nVertexNum;
		//交换这两个坐标位置的图片编号
		swap(GameMap[nIndex1 / nCols][nIndex1 % nCols], GameMap[nIndex2 / nCols][nIndex2 % nCols]);
	}
}

bool CGameLogic::LinkX(int Row, int Col1, int Col2)
{
	if (Col1 > Col2)
		swap(Col1, Col2);
	for (int i = Col1 + 1;i <= Col2;i++)
	{
		if (i == Col2)
			return true;
		if (GameMap[Row][i] != BLANK)
			return false;
	}
	return false;
}

bool CGameLogic::LinkY(int Row1, int Row2, int Col)
{
	if (Row1 > Row2)
		swap(Row1, Row2);
	for (int i = Row1 + 1;i <= Row2;i++)
	{
		if (i == Row2)
			return true;
		if (GameMap[i][Col] != BLANK)
			return false;
	}
	return false;
}

bool CGameLogic::LinkInRow(Vertex v1, Vertex v2)
{
	int Row = v1.row;
	int Col1 = v1.col;
	int Col2 = v2.col;
	return LinkX(Row, Col1, Col2);
}

bool CGameLogic::LinkInCol(Vertex v1, Vertex v2)
{
	int Col = v1.col;
	int Row1 = v1.row;
	int Row2 = v2.row;
	return LinkY(Row1, Row2, Col);
}

bool CGameLogic::LinkOneCorner(Vertex v1, Vertex v2)
{
	if (GameMap[v1.row][v2.col] == BLANK)
	{
		if (LinkX(v1.row, v1.col, v2.col) && LinkY(v1.row, v2.row, v2.col))
		{
			Vertex v = { v1.row, v2.col, BLANK };
			verList.push(v);
			return true;
		}
	}
	if (GameMap[v2.row][v1.col] == BLANK)
	{
		if (LinkY(v1.row, v2.row, v1.col) && LinkX(v2.row, v1.col, v2.col))
		{
			Vertex v = { v2.row, v1.col, BLANK };
			verList.push(v);
			return true;
		}
	}
	return false;
}

bool CGameLogic::LinkTwoCorner(Vertex v1, Vertex v2)
{
	for (int i = 0;i < rows;i++)
	{
		if (GameMap[i][v1.col] == BLANK && GameMap[i][v2.col] == BLANK)
		{
			if (LinkY(v1.row, i, v1.col) && LinkX(i, v1.col, v2.col) && LinkY(i, v2.row, v2.col))
			{
				Vertex vx1 = { i, v1.col, BLANK };
				Vertex vx2 = { i, v2.col, BLANK };
				verList.push(vx1);
				verList.push(vx2);
				return true;
			}
		}
	}
	for (int j = 0;j < cols;j++)
	{
		if (GameMap[v1.row][j] == BLANK && GameMap[v2.row][j] == BLANK)
		{
			if (LinkX(v1.row, v1.col, j) && LinkY(v1.row, v2.row, j) && LinkX(v2.row, j, v2.col))
			{
				Vertex vx1 = { v1.row, j, BLANK };
				Vertex vx2 = { v2.row, j, BLANK };
				verList.push(vx1);
				verList.push(vx2);
				return true;
			}
		}
	}
	return false;
}

bool CGameLogic::IsLink(Vertex v1, Vertex v2)
{
	if (v1.row == v2.row && v1.col == v2.col)  //同一个点
		return false;
	if (GetElement(v1.row, v1.col) != GetElement(v2.row, v2.col))  //图片编号不同
		return false;

	ClearVerList();  //清空路径列表
	verList.push(v1);

	if (v1.row == v2.row)  //在同一行
	{
		if (LinkInRow(v1, v2))
		{
			verList.push(v2);
			return true;
		}
		else if (v1.row == 0) //边界情况
		{
			verList.push({ -1, v1.col, BLANK });
			verList.push({ -1, v2.col, BLANK });
			verList.push(v2);
			return true;
		}
		else if (v1.row == rows - 1)
		{
			verList.push({ rows, v1.col, BLANK });
			verList.push({ rows, v2.col, BLANK });
			verList.push(v2);
			return true;
		}
	}

	if (v1.col == v2.col)  //在同一列
	{
		if (LinkInCol(v1, v2))
		{
			verList.push(v2);
			return true;
		}
		else if (v1.col == 0) //边界情况
		{
			verList.push({ v1.row, -1, BLANK });
			verList.push({ v2.row, -1, BLANK });
			verList.push(v2);
			return true;
		}
		else if (v1.col == cols - 1)
		{
			verList.push({ v1.row, cols, BLANK });
			verList.push({ v2.row, cols, BLANK });
			verList.push(v2);
			return true;
		}
	}

	if (LinkOneCorner(v1, v2))
	{
		verList.push(v2);
		return true;
	}

	if (LinkTwoCorner(v1, v2))
	{
		verList.push(v2);
		return true;
	}

	return false;
}

void CGameLogic::Clear(Vertex v1, Vertex v2)
{
	GameMap[v1.row][v1.col] = BLANK;
	GameMap[v2.row][v2.col] = BLANK;
}

bool CGameLogic::isBlank()
{
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			if (this->GameMap[i][j] != BLANK)
				return false;
		}
	}
	return true;
}

stack<Vertex> CGameLogic::GetVerList()
{
	stack<Vertex> copyList = stack<Vertex>(verList);
	return copyList;
}

void CGameLogic::ClearVerList()
{
	stack<Vertex>().swap(verList);
}

bool CGameLogic::GetPrompt(Vertex& v1, Vertex& v2)
{
	for (int row1 = 0; row1 < rows; row1++)
	{
		for (int col1 = 0; col1 < cols; col1++)
		{
			int nFirstElem = GetElement(row1, col1);
			if (nFirstElem == BLANK)
				continue;
			v1.row = row1;
			v1.col = col1;
			v1.info = nFirstElem;
			for (int row2 = row1; row2 < rows; row2++)
			{
				int col = row2 == row1 ? col1 + 1 : 0;  //如果在同一行，第二张图片的列数要从第一张图片的下一列开始
				for (int col2 = col; col2 < cols; col2++)
				{
					int nSecondElem = GetElement(row2, col2);
					if (nSecondElem == BLANK)
						continue;
					v2.row = row2;
					v2.col = col2;
					v2.info = nSecondElem;
					if (IsLink(v1, v2))
						return true;
				}
			}
		}
	}
	return false;
}

void CGameLogic::ResetMap()
{
	//将地图上的图片打乱
	srand((int)time(NULL));
	int nVertexNum = rows * cols;
	for (int i = 0;i < nVertexNum;i++)
	{
		int nIndex1 = rand() % nVertexNum;
		int nIndex2 = rand() % nVertexNum;
		//交换这两个坐标位置的图片编号
		swap(GameMap[nIndex1 / cols][nIndex1 % cols], GameMap[nIndex2 / cols][nIndex2 % cols]);
	}
}

void CGameLogic::ClearMap()
{
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			GameMap[i][j] = BLANK;
		}
	}
}