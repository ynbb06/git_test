#pragma once
#include "global.h"
#include "CGraph.h"
#include <stack>
#include <vector>
using namespace std;

class CGameLogic
{
public:
    CGameLogic();
    ~CGameLogic();

	int  GetElement(int nRow, int nCol);                // 获取指定位置元素
	void InitMap(int nRows, int nCols, int nPicNum);    // 初始化地图
	bool IsLink(Vertex v1, Vertex v2);                  // 判断两点是否可连接
	void Clear(Vertex v1, Vertex v2);                   // 清除两点（置空）
    bool isBlank();                                     // 判断地图是否为空
    bool GetPrompt(Vertex& v1, Vertex& v2);             // 获取提示
    void ResetMap();                                    // 重置地图
    void ClearMap();                                    // 清空地图
    void ClearVerList();                                // 清空顶点列表
    stack<Vertex> GetVerList();                         // 获取顶点列表

private:
	bool SearchPath(int nV0, int nV1);                  // 搜索路径
	bool IsExsit(int nVi);                              // 判断顶点是否已在路径中
	bool IsCornor(void);                                // 判断最近三个顶点是否构成拐点
    void PushVertex(int nV);                            // 压入顶点
    void PopVertex();                                   // 弹出顶点
    int  GetVexPath(Vertex avPath[MAX_VERTEX_NUM]);     // 获取路径顶点

    void UpdateArc(int nExtRow, int nExtCol);           // 更新扩展矩阵中指定点的边信息
    void InitPath();                                    // 初始化路径状态

    // 原始行列 → 扩展行列/索引 的转换
    int  ToExtIdx(int nRow, int nCol) const;
    int  ToExtRow(int nRow) const { return nRow + 1; }
    int  ToExtCol(int nCol) const { return nCol + 1; }

    CGraph m_graph;                  // 图结构
    int    m_nRows, m_nCols;         // 实际地图行列（不含外围）
    int    m_nExtRows, m_nExtCols;   // 扩展后行列（m_nRows+2, m_nCols+2）
    int    m_nVexNum;                // 当前路径顶点数
    int    m_nCorner;                // 当前路径拐点数
    int    m_anPath[MAX_VERTEX_NUM]; // 路径顶点索引栈（存扩展后的索引）
    Vertex m_avPath[MAX_VERTEX_NUM]; // 最终路径关键点（行列已转回原始坐标，可含负值）
};