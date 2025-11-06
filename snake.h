#pragma once
#define WIDE 60//定义坐标范围
#define HIGH 20
#define INFO_WIDTH 20 // 右侧信息栏宽度
#define POINTS_PER_LEVEL 30 // 每升一级需要的分数
struct BODY//1蛇的结构体
{
    int X,Y;
};
struct SNAKE
{
    int size;
    struct BODY body[WIDE*HIGH];
}snake;
int food[2]={0};//food[0]为X，food[1]为Y
void IntFood();//食物的随机数
void Intsnake();
void ShowUI();//显示界面
void Playgame();//开始游戏
int score=0;//得分
int level=1; // 当前关卡
int dx=0,dy=0;
void Intwall();//初始化墙
int lx=0;
int ly=0;
void ShowInfo(); // 显示右侧信息