#include "pch.h"
#include "CGameControl.h"

int CGameControl::GetElement(int nRow, int nCol)
{
	return m_GameLogic.GetElement(nRow, nCol);
}

bool CGameControl::StartGame(int nRows, int nCols, int nPicNum)
{
	if(nRows <= 0 || nCols <= 0 || nPicNum <= 0)
		return false;
	else if ((nRows * nCols) % (nPicNum * 2) != 0)
		return false;
	else
		m_GameLogic.InitMap(nRows, nCols, nPicNum);
	return true;
}

void CGameControl::SetFirstSelect(int nRow, int nCol)
{
	selFirst.row = nRow;
	selFirst.col = nCol;
}

void CGameControl::SetSecondSelect(int nRow, int nCol)
{
	selSecond.row = nRow;
	selSecond.col = nCol;
}

bool CGameControl::Link(stack<Vertex>& verList)
{
	if (m_GameLogic.IsLink(selFirst, selSecond))
	{
		verList = m_GameLogic.GetVerList();
		m_GameLogic.Clear(selFirst, selSecond);
		return true;
	}
	return false;
}

bool CGameControl::isWin()
{
	return m_GameLogic.isBlank();
}

void CGameControl::SetFirstPrompt(int nRow, int nCol)
{
	promptFirst.row = nRow;
	promptFirst.col = nCol;
}

void CGameControl::SetSecondPrompt(int nRow, int nCol)
{
	promptSecond.row = nRow;
	promptSecond.col = nCol;
}

bool CGameControl::GetPrompt(stack<Vertex>& verList)
{
	Vertex v1, v2;
	if (m_GameLogic.GetPrompt(v1, v2))
	{
		SetFirstPrompt(v1.row, v1.col);
		SetSecondPrompt(v2.row, v2.col);
		verList = m_GameLogic.GetVerList();
		return true;
	}
	return false;
}

void CGameControl::ResetMap()
{
	m_GameLogic.ResetMap();
}

void CGameControl::ClearMap()
{
	m_GameLogic.ClearMap();
}