#pragma once

//定义顶点结构体
typedef struct tagVertex
{
	int row;     //行
	int col;     //列
	int info;    //值为BLANK表示当前点没有图片
}Vertex;

#define MAP_WIDTH           800   //游戏地图宽度
#define MAP_HEIGHT          600   //游戏地图高度
#define MAP_TOP             50    //地图距离游戏界面上方的间距
#define MAP_LEFT            20    //地图距离游戏界面左边的间距
#define PIC_HEIGHT          40    //图片高度
#define PIC_WIDTH           40    //图片宽度
//#define Rows				10	  //行数
//#define Cols				16	  //列数
//#define PicNum			8	  //图片种类数
#define BLANK				-1    //判断是否为空
#define EASY 				300   //进度条满值
#define MED					200
#define HARD				100