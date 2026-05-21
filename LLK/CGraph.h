#pragma once
#include "global.h"

class CGraph
{
public:
    CGraph(void);
	void InitGraph();                              // 初始化图
	int  AddVertex(int nInfo);                     // 添加顶点
    void AddArc(int nIndex1, int nIndex2);         // 添加边
    int  GetVertex(int nIndex);                    // 获取顶点信息
    bool GetArc(int nV1Index, int nV2Index);       // 获取边信息
    void UpdateVertex(int nIndex, int info);       // 更新顶点信息
    int  GetVexnum();                              // 获取顶点数量
	void ClearGraph();                             // 更新图信息
    void ChangeVerex(int nIndex1, int nIndex2);    // 交换顶点

    void ClearArcs();                              // 清空边

	typedef int  Vertices[MAX_VERTEX_NUM];                  // 顶点数组
	typedef bool AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; // 邻接矩阵
protected:
	Vertices  m_Vertices;  // 顶点数组
	int       m_nVexnum;   // 顶点数量
	AdjMatrix m_AdjMatrix; // 邻接矩阵
	int       m_nArcnum;   // 边数量
};