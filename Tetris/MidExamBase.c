
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "block.h"
#pragma warning(disable:4996)

#define LEFT	75
#define RIGHT	77
#define UP		72
#define DOWN	80
#define SPACE	32
#define SPEED	10

#define GBOARD_WIDTH	10 // 보드 크기
#define GBOARD_HEIGHT	20

#define GBOARD_ORIGIN_X	2 // 보드 오프셋
#define GBOARD_ORIGIN_Y	2

int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2]; // 게임 보드 크기 
int curPosX, curPosY; // 커서 위치를 전역으로 관리, 어차피 하나만 필요하므로 전역이여도 됨
int blockID; // 블록 아이디, 역시 블록은 항상 1개 뿐이므로 전역으로 관리

void ShowBlock(int blockInfo[4][4]);
void DeleteBlock(int blockInfo[4][4]);
void RemoveCursor();
void MySetCursor(int x, int y);
COORD MyGetCursor();
int ShiftDown();
int ShiftLeft();
int ShiftRight();
int RotateBlock();
int RRotateBlock();
void ShiftUp();
void KeyInput();
void DrawGameBoard();
int DetectCollision(int posX, int posY, int blockModel[4][4]);
void AddBlockToBoard();
int IsGameOver();
void SpaceDown();
void RedrawBlocks();
void RemoveFillUpLine();

int main()
{
	RemoveCursor(); // 커서가 안보이게 지움
	DrawGameBoard(); // 게임판 생성

	while (1)
	{
		curPosX = 10; // 블럭 생성위치
		curPosY = 2;
		MySetCursor(curPosX, curPosY); // 커서 이동

		srand((unsigned int)time(NULL)); // 랜덤 시드
		blockID = (rand() % 7) * 4; // 랜덤 블럭 생성

		if (!IsGameOver()) // 게임오버 함수가 호출되면 게임 종료
			break;

		while (1)
		{
			if (ShiftDown() == 0) // 블록이 내려가다가 멈추면
			{
				AddBlockToBoard(); // 블록을 게임보드에 추가함
				break; // 반복문 탈출
			}

			KeyInput(); // 키보드 입력
		}

		RemoveFillUpLine(); // 한줄이 꽉 찾는지 검사, 제거
	}

	getchar();

	return 0;
}


void RemoveCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void MySetCursor(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD MyGetCursor()
{
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}

void ShowBlock(int blockInfo[4][4])
{
	int y, x;
	COORD curPos = MyGetCursor(); // 현재 위치를 받아오고

	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			MySetCursor(curPos.X + x * 2, curPos.Y + y); 
			// 사각형은 가로를 2칸 차지하므로 2개씩 증가
			// 4x4 배열을 돌며 1이 있는 경우 출력함

			if (blockInfo[y][x] == 1)
				printf("■");

		}
	}

	MySetCursor(curPos.X, curPos.Y); // 커서 위치 초기화
}

void DeleteBlock(int blockInfo[4][4])
{	// 쇼블럭과 동일, 다만 사각형을 공백으로 출력
	int y, x;
	COORD curPos = MyGetCursor();

	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			MySetCursor(curPos.X + x * 2, curPos.Y + y);

			if (blockInfo[y][x] == 1)
				printf("  ");
		}
	}
	MySetCursor(curPos.X, curPos.Y);
}

int ShiftDown()
{
	if (!(DetectCollision(curPosX, curPosY + 1, blockModel[blockID])))
		return 0; // 아래칸에서 충돌이 일어나는지 검사

	DeleteBlock(blockModel[blockID]);	// 기존 블럭을 지우고
	curPosY++;							// 한칸 아래로 커서 이동
	MySetCursor(curPosX, curPosY);		// 커서 설정
	ShowBlock(blockModel[blockID]);		// 다시 그리기

	return 1;
}

void ShiftUp()
{
	DeleteBlock(blockModel[blockID]);
	curPosY--;
	MySetCursor(curPosX, curPosY);
	ShowBlock(blockModel[blockID]);
}

int ShiftLeft()
{
	if (!(DetectCollision(curPosX - 2, curPosY, blockModel[blockID])))
		return 0;

	DeleteBlock(blockModel[blockID]);
	curPosX -= 2;

	MySetCursor(curPosX, curPosY);
	ShowBlock(blockModel[blockID]);

	return 1;
}

int ShiftRight()
{
	if (!(DetectCollision(curPosX + 2, curPosY, blockModel[blockID])))
		return 0;

	DeleteBlock(blockModel[blockID]);
	curPosX += 2;

	MySetCursor(curPosX, curPosY);
	ShowBlock(blockModel[blockID]);

	return 1;
}

int RotateBlock()
{
	int base = blockID - blockID % 4;		// 베이스 블럭 찾기
	int tmpID = base + (blockID + 1) % 4;	// 다음 ID 계산

	if (!(DetectCollision(curPosX, curPosY, blockModel[tmpID])))
		return 0; // 지금 블럭이 충돌이 일어나는지 검사

	DeleteBlock(blockModel[blockID]); // 지금 블럭을 지우고

	blockID = tmpID; // 계산한 블럭 ID로 바꿈
	ShowBlock(blockModel[blockID]); // 회전한 블록 그리기

	return 1;
}

int RRotateBlock()
{
	int base = blockID - blockID % 4;
	int tmpID = base + (blockID + 3) % 4;

	if (!(DetectCollision(curPosX, curPosY, blockModel[tmpID])))
		return 0;

	DeleteBlock(blockModel[blockID]);

	blockID = tmpID;
	ShowBlock(blockModel[blockID]);

	return 1;
}

void KeyInput()
{
	int i;
	int key;

	for (i = 0; i < 20; i++)
	{
		if (_kbhit() != 0) // 키보드가 입력되면
		{
			key = _getch(); // 키 저장

			switch (key)
			{
			case LEFT:
				ShiftLeft();
				break;

			case RIGHT:
				ShiftRight();
				break;

			case UP:
				RotateBlock();
				break;

			case SPACE:
				SpaceDown();
				break;

			case 'r':
				RRotateBlock();
				break;
			default:
				break;
			}
		}
		Sleep(SPEED);
	}
}

void DrawGameBoard()
{
	int x, y;
	for (y = 0; y <= GBOARD_HEIGHT; y++) // 게임보드 높이만큼 증가
	{
		MySetCursor(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT) // 마지막에 도달하면 꺽쇠 출력
			printf("└");
		else
			printf("│");

		MySetCursor(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT) // 동시에 오른쪽도 그림

			printf("┘");
		else
			printf("│");
	}

	for (x = 1; x < GBOARD_WIDTH + 1; x++) // 마지막 줄
	{
		MySetCursor(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
		printf("─");
	}

	// 게임보드 벽에 해당하는 부분을 1로 설정
	for (y = 0; y < GBOARD_HEIGHT; y++)
	{
		gameBoardInfo[y][0] = 1;
		gameBoardInfo[y][GBOARD_WIDTH + 1] = 1;
	}

	for (x = 0; x < GBOARD_WIDTH + 2; x++)
	{
		gameBoardInfo[GBOARD_HEIGHT][x] = 1;
	}
}

int DetectCollision(int posX, int posY, int blockModel[4][4])
{
	int x, y;
	int arrX = (posX - GBOARD_ORIGIN_X) / 2; // 커서의 위치를 배열 위치로 변환
	int arrY = posY - GBOARD_ORIGIN_Y;

	// 다 검사할 필요 없이 블럭 4x4 위치만 검사
	for (x = 0; x < 4; x++)
	{
		for (y = 0; y < 4; y++)
		{
			if (blockModel[y][x] == 1 && gameBoardInfo[arrY + y][arrX + x] == 1)
				return 0; // 충돌이 일어나면 0
		}
	}
	return 1; // 아니면 1
}

void AddBlockToBoard()
{
	int x, y;
	int arrCurX, arrCurY;

	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			arrCurX = (curPosX - GBOARD_ORIGIN_X) / 2;
			arrCurY = curPosY - GBOARD_ORIGIN_Y;

			if (blockModel[blockID][y][x] == 1) // 블럭 위치를
				gameBoardInfo[arrCurY + y][arrCurX + x] = 1; // 게임보드 판에 갱신함
		}
	}
}

int IsGameOver()
{
	if (!(DetectCollision(curPosX, curPosY, blockModel[blockID])))
	{	//무한반복문 맨 위에서 검사, 이 때 시작위치에서 충돌이 발생하면 게임 오버
		printf("GameOver");
		return 0;
	}
	return 1;
}

void SpaceDown()
{
	while (ShiftDown()); // 아래로 이동이 충돌이 일어날 때 까지 반복
}

void RedrawBlocks()
{
	int x, y;
	int cursX, cursY;

	for (y = 0; y < GBOARD_HEIGHT; y++)
	{
		for (x = 1; x < GBOARD_WIDTH + 1; x++)
		{
			cursX = x * 2 + GBOARD_ORIGIN_X;
			cursY = y + GBOARD_ORIGIN_Y;
			MySetCursor(cursX, cursY);

			if (gameBoardInfo[y][x] == 1) // 
				printf("■");
			else
				printf("  ");
		}
	}
}

void RemoveFillUpLine()
{
	int line, x, y;

	for (y = GBOARD_HEIGHT - 1; y > 0; y--)
	{
		for (x = 1; x < GBOARD_WIDTH + 1; x++)
		{	// 완성된 라인이 있는지 검사
			if (gameBoardInfo[y][x] != 1)
				break;
		}

		if (x == (GBOARD_WIDTH + 1))
		{	// 한 줄이 완성되었다면 x가 넓이 + 1만큼 증가
			for (line = 0; y - line > 0; line++)
				memcpy(&gameBoardInfo[y - line][1], &gameBoardInfo[(y - line) - 1][1], GBOARD_WIDTH * sizeof(int));
				// memcpy()함수로 위 라인을 통째로 한 칸 아래로 복사
			y++; // 한 줄 내렸으므로 y값 조정을 통해 다시 같은 줄 검사
		}
	}
	RedrawBlocks(); // 재구성한 배열 출력
}
