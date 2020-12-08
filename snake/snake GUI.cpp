#include<easyx.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>
#include<stdio.h>

#define WINDOW_HEIGHT	750		/* 背景高度 */
#define WINDOW_WIDTH	1000	/* 背景宽度 */
#define SNAKE_HEIGHT	25		/* 蛇的高度 */
#define SNAKE_WIDTH		25		/* 蛇的宽度 */
#define FOOD_WIDTH		25		/* 食物宽度 */
#define FOOD_HEIGHT		25		/* 食物高度 */
#define SLEEP_TIME		200		/* 休眠时间 */
#define START_POS_X		500		/* 开始时的位置x */
#define START_POS_Y		300		/* 开始时的位置y */
#define FOOD_X			100		/* 食物起始位置x */
#define FOOD_Y			100		/* 食物起始位置y */

/* 存储蛇身体的数据结构，倒叙存储 */
typedef struct snake {
	int x;
	int y;
	snake* next;
}snake, * snakelink;

/* 枚举类型，移动的方向 */
typedef enum direction {
	LEFT, RIGHT, UP, DOWN
};

IMAGE imgBg, imgBody, imgHeadRight, imgHeadLeft, imgHeadUp, imgHeadDown;
bool isStart = true;			/* 标记游戏是否开始 */
snakelink snakeLink = NULL;		/* 存储蛇的链表，逆序存储，即链表头是蛇尾 */
enum direction snakeDir = RIGHT;	/* 标记蛇前进的方向 */
int foodX = FOOD_X;					/* 食物坐标x */
int foodY = FOOD_Y;					/* 食物坐标y */
int myScore = 0;					/* 分数 */
char pChr[20];
WCHAR myScoreStr[20];

void GetMsg();					/* 消息循环 */
void InitGraphics();			/* 加载所有资源 */
void DrawSnake();				/* 绘制蛇 */
void StartGame();				/* 开始游戏 */
void CreateNode();				/* 创建链表结点 */
void EndtoStart();				/* 头结点放到尾结点上 */
void CreateFood();				/* 生成食物 */
bool CheckFood();				/* 碰撞检测蛇和食物 */
bool CheckSelf();				/* 检测和自己碰撞 */

int main(int argc, char* argv[]) {
	StartGame();
	
	return 0;
}

/* 开始游戏 */
void StartGame() {
	myScore = 0;
	InitGraphics();
	snakeLink = (snakelink)malloc(sizeof(snake));	/* 创建蛇头 */
	if (snakeLink == NULL) return;
	snakeLink->next = NULL;
	snakeLink->x = START_POS_X;
	snakeLink->y = START_POS_Y;
	CreateNode();
	CreateNode();
	CreateFood();		/* 生成食物 */
	GetMsg();										/* 消息循环 */
	return;
}

/* 消息循环 */
void GetMsg() {
	isStart = true;						/* 设置游戏开始的标志 */
	snakeDir = RIGHT;					/* 默认向右移动 */
	char getChr;
	while (isStart) {
		if (_kbhit()) {
			getChr = _getch();
			/* w 119 */
			/* a 97 */
			/* s 115 */
			/* d 100 */
			/* 消息分发 */
			switch (int(getChr)) {
			case 119:
				if (snakeDir != DOWN) snakeDir = UP;
				break;
			case 97:
				if (snakeDir != RIGHT) snakeDir = LEFT;
				break;
			case 115:
				if (snakeDir != UP) snakeDir = DOWN;
				break;
			case 100:
				if (snakeDir != LEFT) snakeDir = RIGHT;
				break;
			}
		}
		DrawSnake();		/* 绘制蛇体 */
		if (CheckFood()) {
			CreateNode();
			CreateFood();
		}
		else {
			EndtoStart();		/* 蛇体移动 */
		}
		if (CheckSelf()) {
			isStart = false;
		}
		Sleep(SLEEP_TIME);
	};
}

/* 加载所有资源 */
void InitGraphics() {
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	loadimage(&imgBg, L"images/background.png");
	loadimage(&imgBody, L"images/body.png");
	loadimage(&imgHeadDown, L"images/head-down.png");
	loadimage(&imgHeadLeft, L"images/head-left.png");
	loadimage(&imgHeadRight, L"images/head-right.png");
	loadimage(&imgHeadUp, L"images/head-up.png");
	BeginBatchDraw();										/* 开始绘制图像 */
	return;
}

/* 绘制蛇 */
void DrawSnake() {
	putimage(0, 0, &imgBg);									/* 绘制背景 */
	sprintf(pChr, "score: %d", myScore);
	mbstowcs(myScoreStr, pChr, 10);						/* 字符型转宽字符 */
	outtextxy(10, 10, myScoreStr);						/* 打印分数 */
	putimage(foodX, foodY, &imgBody);						/* 绘制食物 */
	snakelink link = snakeLink;
	while (link) {
		/* 判断是否是链表最后一个 */
		if (link->next == NULL) {
			/* 根据方向绘制蛇头 */
			switch (snakeDir) {
			case RIGHT:
				putimage(link->x, link->y, &imgHeadRight);
				break;
			case LEFT:
				putimage(link->x, link->y, &imgHeadLeft);
				break;
			case UP:
				putimage(link->x, link->y, &imgHeadUp);
				break;
			case DOWN:
				putimage(link->x, link->y, &imgHeadDown);
				break;
			}
		}
		else {
			putimage(link->x, link->y, &imgBody);
		}
		link = link->next;
	}
	FlushBatchDraw();
}

/* 创建链表结点 */
void CreateNode() {
	snakelink link = snakeLink;
	while (link->next) {
		link = link->next;
	}
	link->next = (snakelink)malloc(sizeof(snake));
	if (link->next == NULL) return;
	link->next->next = NULL;		/* 设置新的结点的next为空 */
	switch (snakeDir) {
	case RIGHT:
		link->next->x = link->x + SNAKE_WIDTH;
		link->next->y = link->y;
		if (link->next->x >= WINDOW_WIDTH) link->next->x = link->next->x - WINDOW_WIDTH;
		break;
	case LEFT:
		link->next->x = link->x - SNAKE_WIDTH;
		link->next->y = link->y;
		if (link->next->x < 0) link->next->x = link->next->x + WINDOW_WIDTH;
		break;
	case UP:
		link->next->x = link->x;
		link->next->y = link->y - SNAKE_HEIGHT;
		if (link->next->y < 0) link->next->y = link->next->y + WINDOW_HEIGHT;
		break;
	case DOWN:
		link->next->x = link->x;
		link->next->y = link->y + SNAKE_HEIGHT;
		if (link->next->y >= WINDOW_HEIGHT) link->next->y = link->next->y - WINDOW_HEIGHT;
		break;
	}
	return;
}

/* 头结点放到尾结点上，模拟移动 */
void EndtoStart() {
	if (snakeLink->next == NULL) {
		/* 设置蛇移动 */
		switch (snakeDir) {
		case RIGHT:
			snakeLink->x = snakeLink->x + SNAKE_WIDTH;
			if (snakeLink->x >= WINDOW_WIDTH) snakeLink->x = snakeLink->x - WINDOW_WIDTH;
			break;
		case LEFT:
			snakeLink->x = snakeLink->x - SNAKE_WIDTH;
			if (snakeLink->x < 0) snakeLink->x = snakeLink->x + WINDOW_WIDTH;
			break;
		case UP:
			snakeLink->y = snakeLink->y - SNAKE_HEIGHT;
			if (snakeLink->y < 0) snakeLink->y = snakeLink->y + WINDOW_HEIGHT;
			break;
		case DOWN:
			snakeLink->y = snakeLink->y + SNAKE_HEIGHT;
			if (snakeLink->y >= WINDOW_HEIGHT) snakeLink->y = snakeLink->y - WINDOW_HEIGHT;
			break;
		}
	}
	else {
		snakelink link = snakeLink;					/* 遍历链表的结点 */
		while (link->next) {
			link = link->next;
		}
		switch (snakeDir) {
		case RIGHT:
			snakeLink->x = link->x + SNAKE_WIDTH;
			snakeLink->y = link->y;
			if (snakeLink->x >= WINDOW_WIDTH) snakeLink->x = snakeLink->x - WINDOW_WIDTH;
			break;
		case LEFT:
			snakeLink->x = link->x - SNAKE_WIDTH;
			snakeLink->y = link->y;
			if (snakeLink->x < 0) snakeLink->x = snakeLink->x + WINDOW_WIDTH;
			break;
		case UP:
			snakeLink->x = link->x;
			snakeLink->y = link->y - SNAKE_HEIGHT;
			if (snakeLink->y < 0) snakeLink->y = snakeLink->y + WINDOW_HEIGHT;
			break;
		case DOWN:
			snakeLink->x = link->x;
			snakeLink->y = link->y + SNAKE_HEIGHT;
			if (snakeLink->y >= WINDOW_HEIGHT) snakeLink->y = snakeLink->y - WINDOW_HEIGHT;
			break;
		}
		link->next = snakeLink;
		snakeLink = snakeLink->next;
		link->next->next = NULL;
	}

}

/* 生成食物 */
void CreateFood() {
	srand((unsigned)time(NULL));								/* 设置随机种子 */
	foodX = rand() % WINDOW_WIDTH;
	foodX = foodX - (foodX % FOOD_WIDTH);
	foodY = rand() % WINDOW_HEIGHT;
	foodY = foodY - (foodY % FOOD_WIDTH);
	while (CheckFood()) {
		foodX = rand() % WINDOW_WIDTH;
		foodX = foodX - (foodX % FOOD_WIDTH);
		foodY = rand() % WINDOW_HEIGHT;
		foodY = foodY - (foodY % FOOD_WIDTH);
	}
	return;
}

/* 碰撞检测蛇和食物 */
bool CheckFood() {
	snakelink link = snakeLink;
	while (link) {
		if ((foodX + FOOD_WIDTH) > link->x && foodX < (link->x + SNAKE_WIDTH)) {
			if (foodY < (link->y + SNAKE_HEIGHT) && (foodY + FOOD_HEIGHT) > link->y) {
				myScore++;
				return true;		/* 碰撞 */
			}
		}
		link = link->next;
	}
	return false;					/* 无碰撞 */
}

/* 检测和自己碰撞 */
bool CheckSelf() {
	snakelink link = snakeLink;
	snakelink p = snakeLink;
	/* link存储蛇头 */
	while (link->next) {
		link = link->next;
	}
	while (p) {
		if (p == link) break;
		if(link->x < (p->x + SNAKE_WIDTH) && (link->x + SNAKE_WIDTH) > p->x) {
			if (link->y < (p->y + SNAKE_HEIGHT) && (link->y + SNAKE_HEIGHT) > p->y) {
				return true;
			}
		}
		p = p->next;
	}
	return false;
}

