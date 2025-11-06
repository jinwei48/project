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

// 设置控制台文本颜色
void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ShowUI() {
    COORD coord;//显示蛇的位置
    coord.X=lx;
    coord.Y=ly;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    putchar(' ');//清除上一个位置的字符
    
    // 显示蛇身
    for(int i = 0; i < snake.size; i++) {
        coord.X = snake.body[i].X;
        coord.Y = snake.body[i].Y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        
        if(i == 0) {
            // 根据移动方向显示不同的蛇头
            SetColor(12); // 红色蛇头
            if(dx == 1) putchar('>'); // 向右
            else if(dx == -1) putchar('<'); // 向左
            else if(dy == -1) putchar('^'); // 向上
            else if(dy == 1) putchar('v'); // 向下
            else putchar('O'); // 默认蛇头
        } else if(i == snake.size - 1) {
            // 蛇尾
            SetColor(10); // 绿色蛇尾
            // 根据方向显示不同的尾巴形状
            int tailX = snake.body[i].X - snake.body[i-1].X;
            int tailY = snake.body[i].Y - snake.body[i-1].Y;
            if(tailX == 1) putchar('<');
            else if(tailX == -1) putchar('>');
            else if(tailY == 1) putchar('^');
            else if(tailY == -1) putchar('v');
            else putchar('o');
        } else {
            // 蛇身，根据相邻部分的方向决定显示什么字符
            SetColor(14); // 黄色蛇身
            int prevX = snake.body[i-1].X - snake.body[i].X;
            int prevY = snake.body[i-1].Y - snake.body[i].Y;
            int nextX = snake.body[i+1].X - snake.body[i].X;
            int nextY = snake.body[i+1].Y - snake.body[i].Y;
            
            // 判断是直线还是转弯
            if((prevX == nextX) || (prevY == nextY)) {
                // 直线部分
                if(prevX != 0) putchar('='); // 水平
                else putchar('|'); // 垂直
            } else {
                // 转弯部分
                putchar('+');
            }
        }
    }
    
    //显示食物的位置
    coord.X = food[0];
    coord.Y = food[1];
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    SetColor(13); // 紫色食物
    putchar('$');
    
    // 恢复默认颜色
    SetColor(7);
}

void ShowInfo() {
    COORD coord;
    // 显示标题
    SetColor(14); // 黄色文字
    coord.X = WIDE + 3;
    coord.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("SNAKE GAME");
    
    // 显示得分
    SetColor(10); // 绿色文字
    coord.X = WIDE + 2;
    coord.Y = 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("Score: %d", score);
    
    // 显示关卡
    SetColor(12); // 红色文字
    coord.Y = 4;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("Level: %d", level);
    
    // 显示下一级所需分数
    SetColor(13); // 紫色文字
    coord.Y = 6;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    int pointsNeeded = level * POINTS_PER_LEVEL - score;
    if (pointsNeeded <= 0)
        pointsNeeded = POINTS_PER_LEVEL;
    printf("Next Level: %d points", pointsNeeded);

    
    // 显示游戏控制提示
    SetColor(11); // 青色文字
    coord.Y = 9;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("Control:");

    coord.Y = 10;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("WASD - Move");

    coord.Y = 11;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("ESC - Exit");
    
    // 恢复默认颜色
    SetColor(7);
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
            SetColor(14); // 黄色文字
            printf("LEVEL UP!");

            Sleep(500);
            // 清除升级提示
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            printf("        ");
            SetColor(7); // 恢复默认颜色
        }
        
        // 方向控制，是否与自身碰撞
        while(_kbhit()){
            key=_getch();//有界面输入就接收
            if (key == 27) { // ESC键退出游戏
                SetColor(12); // 红色文字
                printf("\nGame Exited.");
                SetColor(7); // 恢复默认颜色
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
                SetColor(12); // 红色文字
                printf("GAME OVER!");

                coord.Y++;//切换到下一行
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                SetColor(10); // 绿色文字
                printf("Final Score: %d", score);

                coord.Y++;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                SetColor(14); // 黄色文字
                printf("Level Reached: %d", level);
                SetColor(7); // 恢复默认颜色

                return;
            }
        }
        
        if(snake.body[0].X == food[0] && snake.body[0].Y == food[1]) {//与食物碰撞
            IntFood();//随机增加食物
            snake.size++;//身体加长
            score += 5;
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
    SetColor(12); // 红色文字
    printf("GAME OVER!");

    coord.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    SetColor(10); // 绿色文字
    printf("Final Score: %d", score);

    coord.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    SetColor(14); // 黄色文字
    printf("Level Reached: %d", level);
    SetColor(7); // 恢复默认颜色

}

void Intwall() {//初始化墙
    SetColor(11); // 青色墙壁
    for(int i=0;i<=HIGH;i++){
        for(int j=0;j<=WIDE+INFO_WIDTH;j++){
            if(j == WIDE) // 游戏区域和信息区域的分隔线
                putchar('|');
            else if(i==0 && j==0) // 左上角
                putchar('+');
            else if(i==0 && j==WIDE+INFO_WIDTH) // 右上角
                putchar('+');
            else if(i==HIGH && j==0) // 左下角
                putchar('+');
            else if(i==HIGH && j==WIDE+INFO_WIDTH) // 右下角
                putchar('+');
            else if(i==0 || i==HIGH) // 上下边框
                putchar('-');
            else if(j==0 || j==WIDE+INFO_WIDTH) // 左右边框
                putchar('|');
            else if(j > WIDE) // 信息区域背景
                putchar(' ');
            else
                putchar(' ');
        }
        putchar('\n');
    }
    SetColor(7); // 恢复默认颜色
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
