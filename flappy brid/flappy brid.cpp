#include<conio.h>
#include<stdio.h>
#include<Windows.h>
#include <time.h>

/* 规定向右是x，向下是y，左上角（0, 0） */
/* 游戏界面高度为25宽度21，(0, 1) - (20, 25) */

#define WINDOWS_WIDTH	20			/* 游戏界面宽度 */
#define WINDOWS_HEIGHT	25			/* 游戏界面高度 */
#define BRID_POS_X		6			/* 设置小鸟初始的x坐标 */
#define WALL_PILLAR		"墙"		/* 边界和柱子的表示方法 */
#define END_STRING		"游戏结束！"
#define SCORE			"总得分："	
#define START_STRING	"退出游戏请输入：q，开始游戏请输入：s。请输入："
#define GAP_SIZE		5			/* 柱子缺口大小 */
#define PILLAR_WIDTH	3			/* 柱子宽度 */
#define BRID_DOWN_SIZE	1			/* 小鸟下降高度 */
#define BRID_UP_SIZE	3			/* 小鸟上升高度 */

bool isStart = true;				/* 标记游戏是否结束 */
int myScore = 0;					/* 游戏得分 */

/* 包含柱子的信息 */
typedef struct pillar {
	int posX;						/* 左上角x坐标 */
	int posY = 1;					/* 左上角y坐标，始终为1 */
	int width = PILLAR_WIDTH;		/* 柱子宽度 */
	int gapPos;						/* 柱子缺口位置，取值从1到WINDOWS_HEIGHT - gapSize  */
	int gapSize = GAP_SIZE;			/* 柱子缺口的高度 */
}pillar;

void StartGame();					/* 开始游戏 */
void DrawBrid(int x, int y);		/* 根据左上角位置(x, y)画小鸟 */
void DrawPillar(pillar p);			/* 根据柱子信息画柱子 */
void SetPos(int x, int y);			/* 设置光标的位置 */
void DrawBorder();					/* 画游戏边框 */
void DrawScore(int score, int x, int y);		/* 显示分数 */

int main(int argc, char* argv[]) {
	char input;
	printf(START_STRING);
	while (true) {
		scanf("%c", &input);
		char c;	/* 获取缓存区内容 */
		switch (input) {
		case 'q':
			return 0;
		case 's':
			isStart = true;
			StartGame();
			while ((c = getchar()) != EOF && c != '\n');
			break;
		default:
			getchar();
			while ((c = getchar()) != EOF && c != '\n');
			break;
		}
	}
	// StartGame();
	return 0;
}

/* 开始游戏 */
void StartGame() {
	myScore = 0;
	pillar p;
	p.posX = WINDOWS_WIDTH * 2;		/* 设置柱子起始位置 */
	p.gapPos = 12;					/* 设置柱子缺口位置 */
	int bridY = 13;					/* 小鸟起始的y坐标 */
	char getChr;					/* 接收键盘输入 */

	/* 游戏开始 */
	while (isStart) {
		system("cls");				/* 清屏 */
		DrawBorder();
		DrawScore(myScore, WINDOWS_WIDTH * 2 + 4, 0);		/* 显示分数 */
		DrawBrid(BRID_POS_X, bridY);
		DrawPillar(p);
		if (_kbhit()) {
			getChr = _getch();
			/* 获取键盘指令 */
			if (int(getChr) == 32) {
				bridY -= BRID_UP_SIZE;			/* 小鸟上升的高度 */
			}
		}
		bridY += BRID_DOWN_SIZE;				/* 小鸟下降的高度 */
		/* 柱子右侧不越过小鸟，柱子左移 */
		if (p.posX + p.width > BRID_POS_X / 2)
			p.posX--;
		/* 柱子右侧越过小鸟，柱子再次从有段开始 */
		else {
			myScore += 1;
			p.posX = WINDOWS_WIDTH * 2;
			srand((unsigned)time(NULL));								/* 设置随机种子 */
			p.gapPos = rand() % (WINDOWS_HEIGHT - p.gapSize) + 1;		/* 随机生成柱子缺口的位置 */
		}
		/* 判断游戏结束 */
		/* 当小鸟超出游戏窗口的时候游戏结束 */
		if (bridY <= 0 || bridY >= WINDOWS_HEIGHT + 1) {
			isStart = false;
			system("cls");
			DrawBorder();
			SetPos(int(WINDOWS_WIDTH - (strlen(END_STRING) / 2)), int(WINDOWS_HEIGHT / 2) - 3);		/* 打印游戏结束 */
			printf(END_STRING);
			SetPos(int(WINDOWS_WIDTH - (strlen(END_STRING) / 2)), int(WINDOWS_HEIGHT / 2) - 2);		/* 打印分数 */
			printf(SCORE);
			printf("%d", myScore);
			SetPos(0, int(WINDOWS_HEIGHT / 2));
			printf(START_STRING);
		}
		/* 当小鸟和柱子碰撞游戏结束 */
		if (BRID_POS_X >= p.posX && BRID_POS_X <= p.posX + p.width) {
			if (bridY < p.gapPos || bridY > p.gapPos + p.gapSize) {
				isStart = false;
				system("cls");
				DrawBorder();
				SetPos(int(WINDOWS_WIDTH - (strlen(END_STRING) / 2)), int(WINDOWS_HEIGHT / 2) - 3);		/* 打印游戏结束 */
				printf(END_STRING);
				SetPos(int(WINDOWS_WIDTH - (strlen(END_STRING) / 2)), int(WINDOWS_HEIGHT / 2) - 2);
				printf(SCORE);
				printf("%d", myScore);
				SetPos(0, int(WINDOWS_HEIGHT / 2));
				printf(START_STRING);
			}
		}
		Sleep(50);
	}
}


/* 画游戏边框 */
void DrawBorder() {
	SetPos(0, 0);
	for (int i = 0; i < WINDOWS_WIDTH + 1; i++) {
		printf(WALL_PILLAR);
	}
	SetPos(0, WINDOWS_HEIGHT + 1);
	for (int i = 0; i < WINDOWS_WIDTH + 1; i++) {
		printf(WALL_PILLAR);
	}
}

/* 根据位置画小鸟 */
void DrawBrid(int x, int y) {
	SetPos(x, y);
	printf("鸟");
}

/* 根据位置画柱子 */
void DrawPillar(pillar p) {
	for (int j = 0; j < WINDOWS_HEIGHT; j++) {
		if (j >= p.gapPos && j <= p.gapPos + p.gapSize)
			continue;
		SetPos(p.posX, p.posY + j);
		for (int i = 0; i < p.width; i++) {
			printf(WALL_PILLAR);
		}
	}
}

/* 设置光标的位置 */
void SetPos(int x, int y) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

/* 显示分数 */
void DrawScore(int score, int x, int y) {
	SetPos(x, y + 0);
	printf("**********");
	SetPos(x, y + 1);
	printf(SCORE);
	printf("%d", myScore);
	SetPos(x, y + 2);
	printf("**********");
}
