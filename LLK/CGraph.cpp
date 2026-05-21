#include "pch.h"
#include "CGraph.h"

CGraph::CGraph()
{
    InitGraph();
}

void CGraph::InitGraph()
{
    for (int i = 0; i < MAX_VERTEX_NUM; i++)
        m_Vertices[i] = -1;

    for (int i = 0; i < MAX_VERTEX_NUM; i++)
        for (int j = 0; j < MAX_VERTEX_NUM; j++)
            m_AdjMatrix[i][j] = false;

    m_nVexnum = 0;
    m_nArcnum = 0;
}

int CGraph::AddVertex(int nInfo)
{
    if (m_nVexnum >= MAX_VERTEX_NUM)
        return 0;
    m_Vertices[m_nVexnum] = nInfo;
    m_nVexnum++;
    return m_nVexnum;
}

void CGraph::AddArc(int nIndex1, int nIndex2)
{
    m_AdjMatrix[nIndex1][nIndex2] = true;
    m_AdjMatrix[nIndex2][nIndex1] = true;
}

int CGraph::GetVertex(int nIndex)
{
    return m_Vertices[nIndex];
}

bool CGraph::GetArc(int nV1Index, int nV2Index)
{
    return m_AdjMatrix[nV1Index][nV2Index];
}

void CGraph::UpdateVertex(int nIndex, int info)
{
    m_Vertices[nIndex] = info;
}

int CGraph::GetVexnum()
{
    return m_nVexnum;
}

void CGraph::ClearGraph()
{
    InitGraph();
}

void CGraph::ChangeVerex(int nIndex1, int nIndex2)
{
    int Temp = m_Vertices[nIndex1];
    m_Vertices[nIndex1] = m_Vertices[nIndex2];
    m_Vertices[nIndex2] = Temp;
}

void CGraph::ClearArcs()
{
    for (int i = 0; i < MAX_VERTEX_NUM; i++)
        for (int j = 0; j < MAX_VERTEX_NUM; j++)
            m_AdjMatrix[i][j] = false;
}