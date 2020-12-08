#include<easyx.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>
#include<stdio.h>

#define WINDOW_HEIGHT	750		/* �����߶� */
#define WINDOW_WIDTH	1000	/* ������� */
#define SNAKE_HEIGHT	25		/* �ߵĸ߶� */
#define SNAKE_WIDTH		25		/* �ߵĿ�� */
#define FOOD_WIDTH		25		/* ʳ���� */
#define FOOD_HEIGHT		25		/* ʳ��߶� */
#define SLEEP_TIME		200		/* ����ʱ�� */
#define START_POS_X		500		/* ��ʼʱ��λ��x */
#define START_POS_Y		300		/* ��ʼʱ��λ��y */
#define FOOD_X			100		/* ʳ����ʼλ��x */
#define FOOD_Y			100		/* ʳ����ʼλ��y */

/* �洢����������ݽṹ������洢 */
typedef struct snake {
	int x;
	int y;
	snake* next;
}snake, * snakelink;

/* ö�����ͣ��ƶ��ķ��� */
typedef enum direction {
	LEFT, RIGHT, UP, DOWN
};

IMAGE imgBg, imgBody, imgHeadRight, imgHeadLeft, imgHeadUp, imgHeadDown;
bool isStart = true;			/* �����Ϸ�Ƿ�ʼ */
snakelink snakeLink = NULL;		/* �洢�ߵ���������洢��������ͷ����β */
enum direction snakeDir = RIGHT;	/* �����ǰ���ķ��� */
int foodX = FOOD_X;					/* ʳ������x */
int foodY = FOOD_Y;					/* ʳ������y */
int myScore = 0;					/* ���� */
char pChr[20];
WCHAR myScoreStr[20];

void GetMsg();					/* ��Ϣѭ�� */
void InitGraphics();			/* ����������Դ */
void DrawSnake();				/* ������ */
void StartGame();				/* ��ʼ��Ϸ */
void CreateNode();				/* ���������� */
void EndtoStart();				/* ͷ���ŵ�β����� */
void CreateFood();				/* ����ʳ�� */
bool CheckFood();				/* ��ײ����ߺ�ʳ�� */
bool CheckSelf();				/* �����Լ���ײ */

int main(int argc, char* argv[]) {
	StartGame();
	
	return 0;
}

/* ��ʼ��Ϸ */
void StartGame() {
	myScore = 0;
	InitGraphics();
	snakeLink = (snakelink)malloc(sizeof(snake));	/* ������ͷ */
	if (snakeLink == NULL) return;
	snakeLink->next = NULL;
	snakeLink->x = START_POS_X;
	snakeLink->y = START_POS_Y;
	CreateNode();
	CreateNode();
	CreateFood();		/* ����ʳ�� */
	GetMsg();										/* ��Ϣѭ�� */
	return;
}

/* ��Ϣѭ�� */
void GetMsg() {
	isStart = true;						/* ������Ϸ��ʼ�ı�־ */
	snakeDir = RIGHT;					/* Ĭ�������ƶ� */
	char getChr;
	while (isStart) {
		if (_kbhit()) {
			getChr = _getch();
			/* w 119 */
			/* a 97 */
			/* s 115 */
			/* d 100 */
			/* ��Ϣ�ַ� */
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
		DrawSnake();		/* �������� */
		if (CheckFood()) {
			CreateNode();
			CreateFood();
		}
		else {
			EndtoStart();		/* �����ƶ� */
		}
		if (CheckSelf()) {
			isStart = false;
		}
		Sleep(SLEEP_TIME);
	};
}

/* ����������Դ */
void InitGraphics() {
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	loadimage(&imgBg, L"images/background.png");
	loadimage(&imgBody, L"images/body.png");
	loadimage(&imgHeadDown, L"images/head-down.png");
	loadimage(&imgHeadLeft, L"images/head-left.png");
	loadimage(&imgHeadRight, L"images/head-right.png");
	loadimage(&imgHeadUp, L"images/head-up.png");
	BeginBatchDraw();										/* ��ʼ����ͼ�� */
	return;
}

/* ������ */
void DrawSnake() {
	putimage(0, 0, &imgBg);									/* ���Ʊ��� */
	sprintf(pChr, "score: %d", myScore);
	mbstowcs(myScoreStr, pChr, 10);						/* �ַ���ת���ַ� */
	outtextxy(10, 10, myScoreStr);						/* ��ӡ���� */
	putimage(foodX, foodY, &imgBody);						/* ����ʳ�� */
	snakelink link = snakeLink;
	while (link) {
		/* �ж��Ƿ����������һ�� */
		if (link->next == NULL) {
			/* ���ݷ��������ͷ */
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

/* ���������� */
void CreateNode() {
	snakelink link = snakeLink;
	while (link->next) {
		link = link->next;
	}
	link->next = (snakelink)malloc(sizeof(snake));
	if (link->next == NULL) return;
	link->next->next = NULL;		/* �����µĽ���nextΪ�� */
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

/* ͷ���ŵ�β����ϣ�ģ���ƶ� */
void EndtoStart() {
	if (snakeLink->next == NULL) {
		/* �������ƶ� */
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
		snakelink link = snakeLink;					/* ��������Ľ�� */
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

/* ����ʳ�� */
void CreateFood() {
	srand((unsigned)time(NULL));								/* ����������� */
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

/* ��ײ����ߺ�ʳ�� */
bool CheckFood() {
	snakelink link = snakeLink;
	while (link) {
		if ((foodX + FOOD_WIDTH) > link->x && foodX < (link->x + SNAKE_WIDTH)) {
			if (foodY < (link->y + SNAKE_HEIGHT) && (foodY + FOOD_HEIGHT) > link->y) {
				myScore++;
				return true;		/* ��ײ */
			}
		}
		link = link->next;
	}
	return false;					/* ����ײ */
}

/* �����Լ���ײ */
bool CheckSelf() {
	snakelink link = snakeLink;
	snakelink p = snakeLink;
	/* link�洢��ͷ */
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

