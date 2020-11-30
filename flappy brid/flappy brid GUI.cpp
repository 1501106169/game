#include<conio.h>
#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<graphics.h>
#include<easyx.h>

#pragma comment(lib, "Winmm.lib")

#define WINDOWS_WIDTH	350			/* 游戏界面宽度 */
#define WINDOWS_HEIGHT	600			/* 游戏界面高度 */
#define PILLAR_WIDTH	140			/* 柱子宽度 */
#define GAP_SIZE		100			/* 柱子缺口大小 */
#define PILLAR_DEFAULT_X	300		/* 柱子默认初始位置 */
#define PILLAR_SPEED	5			/* 柱子移动速度 */
#define RAND_SIZE		100			/* 柱子缺口位置差 */
#define BRID_POS_X		36			/* 设置小鸟初始的x坐标 */
#define BRID_WIDTH		34			/* 小鸟宽度 */
#define BRID_HEIGHT		24			/* 小鸟高度 */
#define BRID_UP_SIZE	30			/* 小鸟上升高度 */
#define BRID_DOWN_SIZE	5			/* 小鸟下降高度 */

IMAGE imgBg, imgBridBg, imgBrid, imgPillarDownBg, imgPillarDown, imgPillarUpBg, imgPillarUp;
bool isStart = true;				/* 标记游戏是否结束 */
int myScore = 0;					/* 游戏得分 */
int pillarX = PILLAR_DEFAULT_X;		/* 柱子x坐标 */
int gapPos = 300;					/* 柱子缺口初始y坐标 */
int bridY = 300;					/* 小鸟初始y坐标 */

void StartGame();					/* 开始游戏 */
void InitGraphics();				/* 初始化图形界面 */
void DrawGraphics();				/* 绘制图形 */

wchar_t myScoreStr[10];
char pChar[10];

int main(int argc, char* argv[]) {
	StartGame();
	getchar();
	return 0;
}

/* 初始化图形界面 */
void InitGraphics() {
	initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT);
	loadimage(&imgBg, L"images/background.jpg");
	loadimage(&imgBridBg, L"images/bird1.jpg");
	loadimage(&imgBrid, L"images/bird2.jpg");
	loadimage(&imgPillarDownBg, L"images/bar_down1.gif");
	loadimage(&imgPillarDown, L"images/bar_down2.gif");
	loadimage(&imgPillarUpBg, L"images/bar_up1.gif");
	loadimage(&imgPillarUp, L"images/bar_up2.gif");
	BeginBatchDraw();																/* 开始批量绘图 */
	// FlushBatchDraw();															/* 将之前绘图全部绘制 */
	 mciSendString(L"open audios/background.mp3 alias bkmusic", NULL, 0, NULL);		/* 打开背景音乐 */
	 mciSendString(L"play bkmusic repeat", NULL, 0, NULL);							/* 循环播放 */
}

/* 绘制图形 */
void DrawGraphics() {
	putimage(0, 0, &imgBg);
	putimage(pillarX, gapPos - WINDOWS_HEIGHT, &imgPillarUpBg, NOTSRCERASE);
	putimage(pillarX, gapPos - WINDOWS_HEIGHT, &imgPillarUp, SRCINVERT);
	putimage(pillarX, gapPos + GAP_SIZE, &imgPillarDownBg, NOTSRCERASE);
	putimage(pillarX, gapPos + GAP_SIZE, &imgPillarDown, SRCINVERT);
	putimage(BRID_POS_X, bridY, &imgBridBg, NOTSRCERASE);
	putimage(BRID_POS_X, bridY, &imgBrid, SRCINVERT);
	sprintf(pChar, "score: %d", myScore);
	mbstowcs(myScoreStr, pChar, 10);					/* 字符型转宽字符 */
	outtextxy(10, 10, myScoreStr);						/* 打印分数 */
	FlushBatchDraw();
}

/* 开始游戏 */
void StartGame() {
	InitGraphics();
	char getChr;
	while (isStart) {
		DrawGraphics();
		/* 小鸟跃出窗口外游戏结束 */
		if (bridY < 0 || bridY + BRID_WIDTH > WINDOWS_HEIGHT) {
			isStart = false;
		}
		/* 小鸟位于柱子的范围内 */
		if (BRID_POS_X + BRID_WIDTH >= pillarX && BRID_POS_X <= pillarX + PILLAR_WIDTH) {
			/* 小鸟在缺口外 */
			if (bridY <= gapPos || bridY + BRID_HEIGHT >= gapPos + GAP_SIZE) {
				isStart = false;
			}
		}
		pillarX -= PILLAR_SPEED;
		bridY += BRID_DOWN_SIZE;
		if (_kbhit()) {
			getChr = _getch();
			/* 获取键盘指令 */
			if (int(getChr) == 32) {
				bridY -= BRID_UP_SIZE;			/* 小鸟上升的高度 */
				// mciSendString(L"close jpmusic", NULL, 0, NULL);							/* 先把前面一次的音乐关闭 */
				// mciSendString(L"open audios/Jump.mp3 alias jpmusic", NULL, 0, NULL);		/* 打开跳动音乐 */
				// mciSendString(L"play jpmusic", NULL, 0, NULL);								/* 仅播放一次 */
			}
		}
		/* 设置生成的柱子的缺口的位置 */
		if (pillarX + PILLAR_WIDTH + PILLAR_SPEED < BRID_POS_X) {
			pillarX = PILLAR_DEFAULT_X;
			myScore += 1;
			if (gapPos < GAP_SIZE) {
				gapPos += rand() % RAND_SIZE;
			}
			else if (gapPos > WINDOWS_HEIGHT - GAP_SIZE) {
				gapPos -= rand() % RAND_SIZE;
			}
			else {
				srand((unsigned)time(NULL));
				gapPos += rand() % (RAND_SIZE * 2) - RAND_SIZE;
			}
		}
		Sleep(50);
	}
}
