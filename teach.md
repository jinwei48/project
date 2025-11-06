# 贪吃蛇游戏增强教程：添加关卡系统和右侧信息显示

## 学习目标

通过本教程，你将学会如何：
- 在现有贪吃蛇游戏中添加关卡系统
- 在游戏界面右侧显示得分和当前关卡
- 随着关卡提升调整游戏难度（速度）
- 完善游戏结束时的统计信息展示

## 前提条件

- 已有的贪吃蛇游戏代码（snake.c 和 snake.h）
- 基本的 C 语言知识
- Windows 环境（因为代码使用了 Windows API）

## 预计完成时间
约 30 分钟

## 最终效果
游戏界面将变为：
- 左侧：游戏区域（蛇和食物）
- 右侧：显示当前得分和关卡信息
- 随着关卡提升，蛇的移动速度会逐渐加快

## 步骤一：修改头文件 (snake.h)

首先，我们需要在头文件中添加关卡相关的变量和常量。

```c
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
```

### 修改说明

1. 添加了 `INFO_WIDTH` 常量定义右侧信息栏的宽度
2. 添加了 `POINTS_PER_LEVEL` 常量定义升级所需的分数
3. 添加了 `level` 变量跟踪当前关卡
4. 添加了 `ShowInfo()` 函数声明，用于显示右侧信息

## 步骤二：修改初始化墙函数 (Intwall)

接下来，修改 `Intwall()` 函数，扩展界面宽度以容纳右侧信息栏。

```c
void Intwall() {//初始化墙
    for(int i=0;i<=HIGH;i++){
        for(int j=0;j<=WIDE+INFO_WIDTH;j++){
            if(j == WIDE) // 游戏区域和信息区域的分隔线
                putchar('|');
            else if(i==HIGH || j==WIDE+INFO_WIDTH)
                putchar('=');
            else if(j > WIDE) // 信息区域背景
                putchar(' ');
            else
                putchar(' ');
        }
        putchar('\n');
    }
}
```

### 修改说明

1. 将内循环的范围扩展到 `WIDE+INFO_WIDTH`
2. 在 `j == WIDE` 的位置添加垂直线作为分隔
3. 确保底部边界扩展到新的宽度
4. 为信息区域预留空白背景

## 步骤三：实现显示信息函数 (ShowInfo)

现在创建 `ShowInfo()` 函数，用于在右侧信息栏显示得分和关卡。

```c
void ShowInfo() {
    COORD coord;
    // 显示得分
    coord.X = WIDE + 2;
    coord.Y = 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("得分: %d", score);
    
    // 显示关卡
    coord.Y = 4;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("关卡: %d", level);
    
    // 显示下一级所需分数
    coord.Y = 6;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    int pointsNeeded = level * POINTS_PER_LEVEL - score;
    if (pointsNeeded <= 0)
        pointsNeeded = POINTS_PER_LEVEL;
    printf("下一级: %d分", pointsNeeded);
    
    // 显示游戏控制提示
    coord.Y = 9;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("控制:");
    coord.Y = 10;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("WASD - 移动");
    coord.Y = 11;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("ESC - 退出");
}
```

### 函数说明

1. 使用 `COORD` 结构和 `SetConsoleCursorPosition` 在指定位置显示文本
2. 显示当前得分、关卡和升级所需分数
3. 添加了简单的游戏控制提示

## 步骤四：修改游戏主循环 (Playgame)

修改 `Playgame()` 函数，添加关卡逻辑和难度调整。

```c
void Playgame() {
    char key = 'D';
    int delay = 200; // 初始延迟时间（毫秒）
    
    // 初始显示信息
    ShowInfo();
    
    while(snake.body[0].X >= 0 && snake.body[0].X < WIDE && snake.body[0].Y >= 0 && snake.body[0].Y < HIGH) {
        ShowUI();//没撞墙显示界面
        
        // 检查是否升级
        int newLevel = score / POINTS_PER_LEVEL + 1;
        if (newLevel > level) {
            level = newLevel;
            // 随着关卡提升，降低延迟时间（增加速度）
            delay = 200 - (level - 1) * 15;
            if (delay < 50) delay = 50; // 设置最小延迟
            ShowInfo(); // 更新信息显示
            
            // 显示升级提示
            COORD coord;
            coord.X = WIDE / 2 - 5;
            coord.Y = HIGH / 2;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            printf("升级了！");
            Sleep(500);
            // 清除升级提示
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            printf("        ");
        }
        
        // 方向控制，是否与自身碰撞
        while(_kbhit()){
            key=_getch();//有界面输入就接收
            if (key == 27) { // ESC键退出游戏
                printf("\n游戏已退出");
                return;
            }
        }
        
        switch(key) {
            case 'D':
            case 'd':
                dx = 1; dy = 0; break;//右
            case 'W':
            case 'w':
                dx = 0; dy = -1; break;//上
            case 'A':
            case 'a':
                dx = -1; dy = 0; break;//左
            case 'S':
            case 's':
                dx = 0; dy = 1; break;//下
        }
        
        //是否与身体有碰撞
        for(int i = 1; i < snake.size; i++) {
            if(snake.body[0].X == snake.body[i].X && snake.body[0].Y == snake.body[i].Y) {
                COORD coord;
                coord.X = WIDE / 2 - 5;
                coord.Y = HIGH / 2;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                printf("游戏结束！");
                coord.Y++;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                printf("最终得分: %d", score);
                coord.Y++;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                printf("到达关卡: %d", level);
                return;
            }
        }
        
        if(snake.body[0].X == food[0] && snake.body[0].Y == food[1]) {//与食物碰撞
            IntFood();//随机增加食物
            snake.size++;//身体加长
            score += 10;
            ShowInfo(); // 更新得分显示
        }
        
        //蛇更新坐标，从后往前复制
        lx=snake.body[snake.size-1].X;
        ly=snake.body[snake.size-1].Y;
        for(int i=snake.size-1;i>0;i--){  
            snake.body[i].X=snake.body[i-1].X;
            snake.body[i].Y=snake.body[i-1].Y;
        }
        
        //更新蛇头
        snake.body[0].X+=dx;
        snake.body[0].Y+=dy;
        
        Sleep(delay); // 使用随关卡变化的延迟
    }
    
    // 撞到边界的游戏结束处理
    COORD coord;
    coord.X = WIDE / 2 - 5;
    coord.Y = HIGH / 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("游戏结束！");
    coord.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("最终得分: %d", score);
    coord.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("到达关卡: %d", level);
}
```

### 修改说明

1. 添加了 `delay` 变量控制游戏速度
2. 实现了关卡升级逻辑：每获得一定分数自动升级
3. 随着关卡提升，减少延迟时间使游戏速度加快
4. 升级时显示提示信息
5. 添加了 ESC 键退出功能
6. 改进了游戏结束时的提示，显示最终得分和到达的关卡
7. 每次得分后更新信息显示

## 步骤五：修改主函数 (main)

修改 `main()` 函数，确保正确初始化所有需要的变量。

```c
int main() {
    srand((size_t)time(NULL));
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize=sizeof(cci);
    cci.bVisible=FALSE;//去除隐藏控制台光标
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cci);

    // 初始化变量
    score = 0;
    level = 1;
    
    Intwall();//初始化墙
    IntFood();
    Intsnake();
    Playgame();
    return EXIT_SUCCESS;
}
```

### 修改说明

添加了 `score` 和 `level` 的显式初始化，确保每次运行游戏都从第一关开始。

## 完整代码

### snake.h 完整代码

```c
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
```

### snake.c 完整代码

```c
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<windows.h>
#include<conio.h>
#include"snake.h"

void IntFood() {
    srand((size_t)time(NULL));
    food[0] = rand() % WIDE;//随机值
    food[1] = rand() % HIGH;
}

void Intsnake() {//初始化蛇

    snake.size = 2;
    //初始化蛇的位置
   snake.body[0].X = WIDE / 2;
    snake.body[0].Y = HIGH / 2;
    snake.body[1].X = WIDE / 2 - 1;
    snake.body[1].Y = HIGH / 2;
}

void ShowUI() {
    COORD coord;//显示蛇的位置
    coord.X=lx;
    coord.Y=ly;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    putchar(' ');//清除上一个位置的字符
    for(int i = 0; i < snake.size; i++) {
        coord.X = snake.body[i].X;
        coord.Y = snake.body[i].Y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);//输出光标
        if(i == 0)
            putchar('@');
        else
            putchar('*');
    }
    //显示食物的位置
    coord.X = food[0];
    coord.Y = food[1];
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    putchar('#');
}

void ShowInfo() {
    COORD coord;
    // 显示得分
    coord.X = WIDE + 2;
    coord.Y = 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("得分: %d", score);
    
    // 显示关卡
    coord.Y = 4;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("关卡: %d", level);
    
    // 显示下一级所需分数
    coord.Y = 6;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    int pointsNeeded = level * POINTS_PER_LEVEL - score;
    if (pointsNeeded <= 0)
        pointsNeeded = POINTS_PER_LEVEL;
    printf("下一级: %d分", pointsNeeded);
    
    // 显示游戏控制提示
    coord.Y = 9;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("控制:");
    coord.Y = 10;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("WASD - 移动");
    coord.Y = 11;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("ESC - 退出");
}

//核心函数，游戏循环
void Playgame() {
    char key = 'D';
    int delay = 200; // 初始延迟时间（毫秒）
    
    // 初始显示信息
    ShowInfo();
    
    while(snake.body[0].X >= 0 && snake.body[0].X < WIDE && snake.body[0].Y >= 0 && snake.body[0].Y < HIGH) {
        ShowUI();//没撞墙显示界面
        
        // 检查是否升级
        int newLevel = score / POINTS_PER_LEVEL + 1;
        if (newLevel > level) {
            level = newLevel;
            // 随着关卡提升，降低延迟时间（增加速度）
            delay = 200 - (level - 1) * 15;
            if (delay < 50) delay = 50; // 设置最小延迟
            ShowInfo(); // 更新信息显示
            
            // 显示升级提示
            COORD coord;
            coord.X = WIDE / 2 - 5;
            coord.Y = HIGH / 2;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            printf("升级了！");
            Sleep(500);
            // 清除升级提示
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            printf("        ");
        }
        
        // 方向控制，是否与自身碰撞
        while(_kbhit()){
            key=_getch();//有界面输入就接收
            if (key == 27) { // ESC键退出游戏
                printf("\n游戏已退出");
                return;
            }
        }
        
        switch(key) {
            case 'D':
            case 'd':
                dx = 1; dy = 0; break;//右
            case 'W':
            case 'w':
                dx = 0; dy = -1; break;//上

            case 'A':
            case 'a':
                dx = -1; dy = 0; break;//左

            case 'S':
            case 's':
                dx = 0; dy = 1; break;//下
        }
        
        //是否与身体有碰撞
        for(int i = 1; i < snake.size; i++) {
            if(snake.body[0].X == snake.body[i].X && snake.body[0].Y == snake.body[i].Y) {
                COORD coord;
                coord.X = WIDE / 2 - 5;
                coord.Y = HIGH / 2;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                printf("游戏结束！");
                coord.Y++;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                printf("最终得分: %d", score);
                coord.Y++;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                printf("到达关卡: %d", level);
                return;
            }
        }
        
        if(snake.body[0].X == food[0] && snake.body[0].Y == food[1]) {//与食物碰撞
            IntFood();//随机增加食物
            snake.size++;//身体加长
            score += 10;
            ShowInfo(); // 更新得分显示
        }
        
        //蛇更新坐标，从后往前复制
        lx=snake.body[snake.size-1].X;
        ly=snake.body[snake.size-1].Y;
        for(int i=snake.size-1;i>0;i--){  
            snake.body[i].X=snake.body[i-1].X;
            snake.body[i].Y=snake.body[i-1].Y;
        }
        
        //更新蛇头
        snake.body[0].X+=dx;
        snake.body[0].Y+=dy;
        
        Sleep(delay); // 使用随关卡变化的延迟
    }
    
    // 撞到边界的游戏结束处理
    COORD coord;
    coord.X = WIDE / 2 - 5;
    coord.Y = HIGH / 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("游戏结束！");
    coord.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("最终得分: %d", score);
    coord.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("到达关卡: %d", level);
}

void Intwall() {//初始化墙
    for(int i=0;i<=HIGH;i++){
        for(int j=0;j<=WIDE+INFO_WIDTH;j++){
            if(j == WIDE) // 游戏区域和信息区域的分隔线
                putchar('|');
            else if(i==HIGH || j==WIDE+INFO_WIDTH)
                putchar('=');
            else if(j > WIDE) // 信息区域背景
                putchar(' ');
            else
                putchar(' ');
        }
        putchar('\n');
    }
}

int main() {
    srand((size_t)time(NULL));
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize=sizeof(cci);
    cci.bVisible=FALSE;//去除隐藏控制台光标
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cci);

    // 初始化变量
    score = 0;
    level = 1;
    
    Intwall();//初始化墙
    IntFood();
    Intsnake();
    Playgame();
    return EXIT_SUCCESS;
}
```

## 挑战与扩展

完成基本实现后，你可以尝试以下扩展：

1. **自定义难度设置**：在游戏开始时让玩家选择难度
2. **多食物系统**：不同类型的食物提供不同分数
3. **障碍物**：在较高关卡中添加固定障碍物
4. **生命值系统**：允许玩家碰撞几次后才结束游戏
5. **最高分记录**：将最高分保存到文件中

## 总结

通过本教程，你已经成功地为贪吃蛇游戏添加了：

- 关卡系统，随着得分增加自动升级
- 右侧信息栏，显示得分、关卡和控制提示
- 随关卡提升的难度调整（速度增加）
- 游戏结束时的统计信息
- ESC 键退出功能

这些改进使游戏更具挑战性和可玩性，同时提供了更好的用户体验。你可以继续扩展这个游戏，添加更多功能来增强游戏体验。