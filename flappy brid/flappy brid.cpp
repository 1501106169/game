#include<conio.h>
#include<stdio.h>
#include<Windows.h>
#include <time.h>

/* �涨������x��������y�����Ͻǣ�0, 0�� */
/* ��Ϸ����߶�Ϊ25���21��(0, 1) - (20, 25) */

#define WINDOWS_WIDTH	20			/* ��Ϸ������ */
#define WINDOWS_HEIGHT	25			/* ��Ϸ����߶� */
#define BRID_POS_X		6			/* ����С���ʼ��x���� */
#define WALL_PILLAR		"ǽ"		/* �߽�����ӵı�ʾ���� */
#define END_STRING		"��Ϸ������"
#define SCORE			"�ܵ÷֣�"	
#define START_STRING	"�˳���Ϸ�����룺q����ʼ��Ϸ�����룺s�������룺"
#define GAP_SIZE		5			/* ����ȱ�ڴ�С */
#define PILLAR_WIDTH	3			/* ���ӿ�� */
#define BRID_DOWN_SIZE	1			/* С���½��߶� */
#define BRID_UP_SIZE	3			/* С�������߶� */

bool isStart = true;				/* �����Ϸ�Ƿ���� */
int myScore = 0;					/* ��Ϸ�÷� */

/* �������ӵ���Ϣ */
typedef struct pillar {
	int posX;						/* ���Ͻ�x���� */
	int posY = 1;					/* ���Ͻ�y���꣬ʼ��Ϊ1 */
	int width = PILLAR_WIDTH;		/* ���ӿ�� */
	int gapPos;						/* ����ȱ��λ�ã�ȡֵ��1��WINDOWS_HEIGHT - gapSize  */
	int gapSize = GAP_SIZE;			/* ����ȱ�ڵĸ߶� */
}pillar;

void StartGame();					/* ��ʼ��Ϸ */
void DrawBrid(int x, int y);		/* �������Ͻ�λ��(x, y)��С�� */
void DrawPillar(pillar p);			/* ����������Ϣ������ */
void SetPos(int x, int y);			/* ���ù���λ�� */
void DrawBorder();					/* ����Ϸ�߿� */
void DrawScore(int score, int x, int y);		/* ��ʾ���� */

int main(int argc, char* argv[]) {
	char input;
	printf(START_STRING);
	while (true) {
		scanf("%c", &input);
		char c;	/* ��ȡ���������� */
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

/* ��ʼ��Ϸ */
void StartGame() {
	myScore = 0;
	pillar p;
	p.posX = WINDOWS_WIDTH * 2;		/* ����������ʼλ�� */
	p.gapPos = 12;					/* ��������ȱ��λ�� */
	int bridY = 13;					/* С����ʼ��y���� */
	char getChr;					/* ���ռ������� */

	/* ��Ϸ��ʼ */
	while (isStart) {
		system("cls");				/* ���� */
		DrawBorder();
		DrawScore(myScore, WINDOWS_WIDTH * 2 + 4, 0);		/* ��ʾ���� */
		DrawBrid(BRID_POS_X, bridY);
		DrawPillar(p);
		if (_kbhit()) {
			getChr = _getch();
			/* ��ȡ����ָ�� */
			if (int(getChr) == 32) {
				bridY -= BRID_UP_SIZE;			/* С�������ĸ߶� */
			}
		}
		bridY += BRID_DOWN_SIZE;				/* С���½��ĸ߶� */
		/* �����Ҳ಻Խ��С���������� */
		if (p.posX + p.width > BRID_POS_X / 2)
			p.posX--;
		/* �����Ҳ�Խ��С�������ٴδ��жο�ʼ */
		else {
			myScore += 1;
			p.posX = WINDOWS_WIDTH * 2;
			srand((unsigned)time(NULL));								/* ����������� */
			p.gapPos = rand() % (WINDOWS_HEIGHT - p.gapSize) + 1;		/* �����������ȱ�ڵ�λ�� */
		}
		/* �ж���Ϸ���� */
		/* ��С�񳬳���Ϸ���ڵ�ʱ����Ϸ���� */
		if (bridY <= 0 || bridY >= WINDOWS_HEIGHT + 1) {
			isStart = false;
			system("cls");
			DrawBorder();
			SetPos(int(WINDOWS_WIDTH - (strlen(END_STRING) / 2)), int(WINDOWS_HEIGHT / 2) - 3);		/* ��ӡ��Ϸ���� */
			printf(END_STRING);
			SetPos(int(WINDOWS_WIDTH - (strlen(END_STRING) / 2)), int(WINDOWS_HEIGHT / 2) - 2);		/* ��ӡ���� */
			printf(SCORE);
			printf("%d", myScore);
			SetPos(0, int(WINDOWS_HEIGHT / 2));
			printf(START_STRING);
		}
		/* ��С���������ײ��Ϸ���� */
		if (BRID_POS_X >= p.posX && BRID_POS_X <= p.posX + p.width) {
			if (bridY < p.gapPos || bridY > p.gapPos + p.gapSize) {
				isStart = false;
				system("cls");
				DrawBorder();
				SetPos(int(WINDOWS_WIDTH - (strlen(END_STRING) / 2)), int(WINDOWS_HEIGHT / 2) - 3);		/* ��ӡ��Ϸ���� */
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


/* ����Ϸ�߿� */
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

/* ����λ�û�С�� */
void DrawBrid(int x, int y) {
	SetPos(x, y);
	printf("��");
}

/* ����λ�û����� */
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

/* ���ù���λ�� */
void SetPos(int x, int y) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

/* ��ʾ���� */
void DrawScore(int score, int x, int y) {
	SetPos(x, y + 0);
	printf("**********");
	SetPos(x, y + 1);
	printf(SCORE);
	printf("%d", myScore);
	SetPos(x, y + 2);
	printf("**********");
}
