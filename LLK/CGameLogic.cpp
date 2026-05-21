#include "pch.h"
#include "CGameLogic.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <stack>

CGameLogic::CGameLogic()
{
    m_nRows = m_nCols = 0;
    m_nExtRows = m_nExtCols = 0;
    m_nVexNum = 0;
    m_nCorner = 0;
}

CGameLogic::~CGameLogic() {}

int CGameLogic::ToExtIdx(int nRow, int nCol) const
{
    return (nRow + 1) * m_nExtCols + (nCol + 1);
}

int CGameLogic::GetElement(int nRow, int nCol)
{
    return m_graph.GetVertex(ToExtIdx(nRow, nCol));
}

void CGameLogic::InitMap(int nRows, int nCols, int nPicNum)
{
    int N = nRows * nCols;
    int extRows = nRows + 2;
    int extCols = nCols + 2;
    int extN = extRows * extCols;
    if (extN > MAX_VERTEX_NUM || N <= 0)
        return;

    m_nRows = nRows;
    m_nCols = nCols;
    m_nExtRows = extRows;
    m_nExtCols = extCols;

    m_graph.ClearGraph();

    // 生成实际图片序列
    vector<int> anTemp(N);
    int nRepeat = N / nPicNum;
    int idx = 0;
    for (int i = 0; i < nPicNum; i++)
        for (int j = 0; j < nRepeat; j++)
            anTemp[idx++] = i;

    srand((unsigned int)time(nullptr));
    for (int i = 0; i < 100; i++)
    {
        int i1 = rand() % N;
        int i2 = rand() % N;
        swap(anTemp[i1], anTemp[i2]);
    }

    // 添加所有顶点（包含外围空白）
    for (int i = 0; i < extRows; i++)
    {
        for (int j = 0; j < extCols; j++)
        {
            if (i == 0 || i == extRows - 1 || j == 0 || j == extCols - 1)
                m_graph.AddVertex(BLANK);          // 外围置空
            else
                m_graph.AddVertex(anTemp[(i - 1) * nCols + (j - 1)]); // 内部图片
        }
    }

    // 建立所有边
    for (int i = 0; i < extRows; i++)
        for (int j = 0; j < extCols; j++)
            UpdateArc(i, j);
}

bool CGameLogic::IsLink(Vertex v1, Vertex v2)
{
    int nIndex1 = ToExtIdx(v1.row, v1.col);
    int nIndex2 = ToExtIdx(v2.row, v2.col);

	// 同一位置或任一位置为空格不可连接
    if (nIndex1 == nIndex2) return false;       
    if (m_graph.GetVertex(nIndex1) == BLANK || m_graph.GetVertex(nIndex2) == BLANK) 
        return false;
	// 图片不同不可连接
    if (m_graph.GetVertex(nIndex1) != m_graph.GetVertex(nIndex2)) 
        return false;

    InitPath();
    PushVertex(nIndex1);

    if (SearchPath(nIndex1, nIndex2))
    {
        PushVertex(nIndex2);
        return true;
    }

    PopVertex();
    return false;
}

void CGameLogic::Clear(Vertex v1, Vertex v2)
{
    int idx1 = ToExtIdx(v1.row, v1.col);
    int idx2 = ToExtIdx(v2.row, v2.col);

    m_graph.UpdateVertex(idx1, BLANK);
    m_graph.UpdateVertex(idx2, BLANK);

    // 更新这两个点四周的边（扩展坐标）
    UpdateArc(v1.row + 1, v1.col + 1);
    UpdateArc(v2.row + 1, v2.col + 1);
}

bool CGameLogic::isBlank()
{
    int extN = m_nExtRows * m_nExtCols;
    for (int i = 0; i < extN; i++)
    {
        int row = i / m_nExtCols;
        int col = i % m_nExtCols;
        // 只检查内部地图，外围恒为 BLANK 无需检查
        if (row > 0 && row < m_nExtRows - 1 && col > 0 && col < m_nExtCols - 1)
            if (m_graph.GetVertex(i) != BLANK)
                return false;
    }
    return true;
}

bool CGameLogic::GetPrompt(Vertex& v1, Vertex& v2)
{
    int extN = m_nExtRows * m_nExtCols;
    // 只遍历内部图片顶点
    for (int i = 0; i < m_nRows; i++)
    {
        for (int j = 0; j < m_nCols; j++)
        {
            int idx1 = ToExtIdx(i, j);
            if (m_graph.GetVertex(idx1) == BLANK) continue;

            for (int p = i; p < m_nRows; p++)
            {
                int startQ = (p == i) ? j + 1 : 0;
                for (int q = startQ; q < m_nCols; q++)
                {
                    int idx2 = ToExtIdx(p, q);
                    if (m_graph.GetVertex(idx2) == BLANK) continue;
                    if (m_graph.GetVertex(idx1) != m_graph.GetVertex(idx2)) continue;

                    InitPath();
                    PushVertex(idx1);
					// 只要找到一对可连接的点就返回提示
                    if (SearchPath(idx1, idx2))
                    {
                        v1.row = i; v1.col = j;
                        v2.row = p; v2.col = q;
                        return true;
                    }
                    PopVertex();
                }
            }
        }
    }
    return false;
}

void CGameLogic::ResetMap()
{
    int extN = m_nExtRows * m_nExtCols;
    // 收集内部顶点索引
    vector<int> innerIdx;
    for (int i = 0; i < m_nRows; i++)
        for (int j = 0; j < m_nCols; j++)
            innerIdx.push_back(ToExtIdx(i, j));

    srand((unsigned int)time(nullptr));
    for (int k = 0; k < 200; k++)
    {
        int a = rand() % innerIdx.size();
        int b = rand() % innerIdx.size();
        m_graph.ChangeVerex(innerIdx[a], innerIdx[b]);
    }

    // 外围保持 BLANK 不变，重建所有边
    m_graph.ClearArcs();
    for (int i = 0; i < m_nExtRows; i++)
        for (int j = 0; j < m_nExtCols; j++)
            UpdateArc(i, j);
}

void CGameLogic::ClearMap()
{
    int extN = m_nExtRows * m_nExtCols;
    for (int i = 0; i < extN; i++)
    {
        int row = i / m_nExtCols;
        int col = i % m_nExtCols;
        if (row == 0 || row == m_nExtRows - 1 || col == 0 || col == m_nExtCols - 1)
            continue; // 外围始终为 BLANK
        m_graph.UpdateVertex(i, BLANK);
    }
    // 重建边（所有内部变为 BLANK 后，与外围的连接仍有效）
    m_graph.ClearArcs();
    for (int i = 0; i < m_nExtRows; i++)
        for (int j = 0; j < m_nExtCols; j++)
            UpdateArc(i, j);
}

void CGameLogic::ClearVerList()
{
    InitPath();
}

stack<Vertex> CGameLogic::GetVerList()
{
    stack<Vertex> s;
    Vertex avPath[MAX_VERTEX_NUM];
    int n = GetVexPath(avPath);
    for (int i = 0; i < n; i++)
        s.push(avPath[i]);
    return s;
}

void CGameLogic::UpdateArc(int nExtRow, int nExtCol)
{
    int idx = nExtRow * m_nExtCols + nExtCol;
    int v1 = m_graph.GetVertex(idx);

    // 左
    if (nExtCol > 0)
    {
        int idx2 = idx - 1;
        int v2 = m_graph.GetVertex(idx2);
        if (v1 == v2 || v1 == BLANK || v2 == BLANK)
            m_graph.AddArc(idx, idx2);
    }
    // 右
    if (nExtCol < m_nExtCols - 1)
    {
        int idx2 = idx + 1;
        int v2 = m_graph.GetVertex(idx2);
        if (v1 == v2 || v1 == BLANK || v2 == BLANK)
            m_graph.AddArc(idx, idx2);
    }
    // 上
    if (nExtRow > 0)
    {
        int idx2 = idx - m_nExtCols;
        int v2 = m_graph.GetVertex(idx2);
        if (v1 == v2 || v1 == BLANK || v2 == BLANK)
            m_graph.AddArc(idx, idx2);
    }
    // 下
    if (nExtRow < m_nExtRows - 1)
    {
        int idx2 = idx + m_nExtCols;
        int v2 = m_graph.GetVertex(idx2);
        if (v1 == v2 || v1 == BLANK || v2 == BLANK)
            m_graph.AddArc(idx, idx2);
    }
}

bool CGameLogic::SearchPath(int nV0, int nV1)
{
	int nVi;
    for (int i = 0;i < 4;++i)
    {
        if(i==0)
            nVi = nV0 - 1; // 左
        else if(i==1)
            nVi = nV0 + 1; // 右
        else if(i==2)
            nVi = nV0 - m_nExtCols; // 上
        else
			nVi = nV0 + m_nExtCols; // 下
        if (m_graph.GetArc(nV0, nVi) && !IsExsit(nVi))
        {
            PushVertex(nVi);
            if (m_nCorner > 2)
            {
                PopVertex();
                continue;
            }

            if (nVi != nV1)
            {
                // 中间点必须为空格（外围虚拟点恒为 BLANK，自动满足）
                if (m_graph.GetVertex(nVi) != BLANK)
                {
                    PopVertex();
                    continue;
                }
                if (SearchPath(nVi, nV1))
                    return true;
            }
            else
            {
                return true;   // 到达目标
            }

            PopVertex();
        }
    }
    return false;
}

bool CGameLogic::IsExsit(int nVi)
{
    for (int i = 0; i < m_nVexNum; i++)
        if (m_anPath[i] == nVi)
            return true;
    return false;
}

bool CGameLogic::IsCornor(void)
{
    if (m_nVexNum >= 3)
    {
        int a = m_anPath[m_nVexNum - 3];
        int b = m_anPath[m_nVexNum - 2];
        int c = m_anPath[m_nVexNum - 1];
        if ((a + c) / 2 != b)
            return true;
    }
    return false;
}

void CGameLogic::PushVertex(int nV)
{
    m_anPath[m_nVexNum] = nV;
    m_nVexNum++;
    if (IsCornor())
        m_nCorner++;
}

void CGameLogic::PopVertex()
{
    if (m_nVexNum > 0)
    {
        if (IsCornor())
            m_nCorner--;
        m_nVexNum--;
    }
}

int CGameLogic::GetVexPath(Vertex avPath[MAX_VERTEX_NUM])
{
    for (int i = 0; i < m_nVexNum; i++)
    {
        int extIdx = m_anPath[i];
        int extRow = extIdx / m_nExtCols;
        int extCol = extIdx % m_nExtCols;
        // 转换回原始坐标
        avPath[i].row = extRow - 1;
        avPath[i].col = extCol - 1;
    }
    return m_nVexNum;
}

void CGameLogic::InitPath()
{
    m_nVexNum = 0;
    m_nCorner = 0;
}