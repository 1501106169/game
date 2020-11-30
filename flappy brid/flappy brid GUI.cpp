#include<conio.h>
#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<graphics.h>
#include<easyx.h>

#pragma comment(lib, "Winmm.lib")

#define WINDOWS_WIDTH	350			/* ��Ϸ������ */
#define WINDOWS_HEIGHT	600			/* ��Ϸ����߶� */
#define PILLAR_WIDTH	140			/* ���ӿ�� */
#define GAP_SIZE		100			/* ����ȱ�ڴ�С */
#define PILLAR_DEFAULT_X	300		/* ����Ĭ�ϳ�ʼλ�� */
#define PILLAR_SPEED	5			/* �����ƶ��ٶ� */
#define RAND_SIZE		100			/* ����ȱ��λ�ò� */
#define BRID_POS_X		36			/* ����С���ʼ��x���� */
#define BRID_WIDTH		34			/* С���� */
#define BRID_HEIGHT		24			/* С��߶� */
#define BRID_UP_SIZE	30			/* С�������߶� */
#define BRID_DOWN_SIZE	5			/* С���½��߶� */

IMAGE imgBg, imgBridBg, imgBrid, imgPillarDownBg, imgPillarDown, imgPillarUpBg, imgPillarUp;
bool isStart = true;				/* �����Ϸ�Ƿ���� */
int myScore = 0;					/* ��Ϸ�÷� */
int pillarX = PILLAR_DEFAULT_X;		/* ����x���� */
int gapPos = 300;					/* ����ȱ�ڳ�ʼy���� */
int bridY = 300;					/* С���ʼy���� */

void StartGame();					/* ��ʼ��Ϸ */
void InitGraphics();				/* ��ʼ��ͼ�ν��� */
void DrawGraphics();				/* ����ͼ�� */

wchar_t myScoreStr[10];
char pChar[10];

int main(int argc, char* argv[]) {
	StartGame();
	getchar();
	return 0;
}

/* ��ʼ��ͼ�ν��� */
void InitGraphics() {
	initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT);
	loadimage(&imgBg, L"images/background.jpg");
	loadimage(&imgBridBg, L"images/bird1.jpg");
	loadimage(&imgBrid, L"images/bird2.jpg");
	loadimage(&imgPillarDownBg, L"images/bar_down1.gif");
	loadimage(&imgPillarDown, L"images/bar_down2.gif");
	loadimage(&imgPillarUpBg, L"images/bar_up1.gif");
	loadimage(&imgPillarUp, L"images/bar_up2.gif");
	BeginBatchDraw();																/* ��ʼ������ͼ */
	// FlushBatchDraw();															/* ��֮ǰ��ͼȫ������ */
	 mciSendString(L"open audios/background.mp3 alias bkmusic", NULL, 0, NULL);		/* �򿪱������� */
	 mciSendString(L"play bkmusic repeat", NULL, 0, NULL);							/* ѭ������ */
}

/* ����ͼ�� */
void DrawGraphics() {
	putimage(0, 0, &imgBg);
	putimage(pillarX, gapPos - WINDOWS_HEIGHT, &imgPillarUpBg, NOTSRCERASE);
	putimage(pillarX, gapPos - WINDOWS_HEIGHT, &imgPillarUp, SRCINVERT);
	putimage(pillarX, gapPos + GAP_SIZE, &imgPillarDownBg, NOTSRCERASE);
	putimage(pillarX, gapPos + GAP_SIZE, &imgPillarDown, SRCINVERT);
	putimage(BRID_POS_X, bridY, &imgBridBg, NOTSRCERASE);
	putimage(BRID_POS_X, bridY, &imgBrid, SRCINVERT);
	sprintf(pChar, "score: %d", myScore);
	mbstowcs(myScoreStr, pChar, 10);					/* �ַ���ת���ַ� */
	outtextxy(10, 10, myScoreStr);						/* ��ӡ���� */
	FlushBatchDraw();
}

/* ��ʼ��Ϸ */
void StartGame() {
	InitGraphics();
	char getChr;
	while (isStart) {
		DrawGraphics();
		/* С��Ծ����������Ϸ���� */
		if (bridY < 0 || bridY + BRID_WIDTH > WINDOWS_HEIGHT) {
			isStart = false;
		}
		/* С��λ�����ӵķ�Χ�� */
		if (BRID_POS_X + BRID_WIDTH >= pillarX && BRID_POS_X <= pillarX + PILLAR_WIDTH) {
			/* С����ȱ���� */
			if (bridY <= gapPos || bridY + BRID_HEIGHT >= gapPos + GAP_SIZE) {
				isStart = false;
			}
		}
		pillarX -= PILLAR_SPEED;
		bridY += BRID_DOWN_SIZE;
		if (_kbhit()) {
			getChr = _getch();
			/* ��ȡ����ָ�� */
			if (int(getChr) == 32) {
				bridY -= BRID_UP_SIZE;			/* С�������ĸ߶� */
				// mciSendString(L"close jpmusic", NULL, 0, NULL);							/* �Ȱ�ǰ��һ�ε����ֹر� */
				// mciSendString(L"open audios/Jump.mp3 alias jpmusic", NULL, 0, NULL);		/* ���������� */
				// mciSendString(L"play jpmusic", NULL, 0, NULL);								/* ������һ�� */
			}
		}
		/* �������ɵ����ӵ�ȱ�ڵ�λ�� */
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
