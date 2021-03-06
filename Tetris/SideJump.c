﻿// 벽은 2, 블럭은 1로 표시
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

#define GBOARD_WIDTH	10
#define GBOARD_HEIGHT	20

#define GBOARD_ORIGIN_X	2
#define GBOARD_ORIGIN_Y	2

int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];
int curPosX, curPosY;
int blockID;

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
int sideCollision(int posX, int posY, int blockModel[4][4]);
int BlockCollision(int posX, int posY, int blockModel[4][4]);

int main()
{
	RemoveCursor();
	DrawGameBoard();
	srand((unsigned int)time(NULL));

	while (1)
	{
		curPosX = 8;
		curPosY = 2;
		MySetCursor(curPosX, curPosY);

		blockID = (rand() % 7) * 4;

		if (!IsGameOver())
			break;

		while (1)
		{
			if (ShiftDown() == 0)
			{
				AddBlockToBoard();
				break;
			}
			KeyInput();
		}
		RemoveFillUpLine();
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
	COORD curPos = MyGetCursor();

	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			MySetCursor(curPos.X + x * 2, curPos.Y + y);

			if (blockInfo[y][x] == 1)
				printf("■");

		}
	}
	MySetCursor(curPos.X, curPos.Y);
}

void DeleteBlock(int blockInfo[4][4])
{
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
		return 0;

	DeleteBlock(blockModel[blockID]);
	curPosY++;
	MySetCursor(curPosX, curPosY);
	ShowBlock(blockModel[blockID]);

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
	int tmp;

	if (!DetectCollision(curPosX - 2, curPosY, blockModel[blockID]))
	{
		if (!sideCollision(curPosX - 2, curPosY, blockModel[blockID]))
		{
			tmp = GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 - 6;

			if (DetectCollision(tmp, curPosY, blockModel[blockID]))
			{
				DeleteBlock(blockModel[blockID]);

				curPosX = tmp;

				while (DetectCollision(curPosX, curPosY, blockModel[blockID]))
				{
					curPosX ++;
				}
				curPosX -= 2;

				MySetCursor(curPosX, curPosY);
				ShowBlock(blockModel[blockID]);
				return 1;
			}
			else
				return 0;
		}
		else
			return 0;
	}

	DeleteBlock(blockModel[blockID]);
	curPosX -= 2;

	MySetCursor(curPosX, curPosY);
	ShowBlock(blockModel[blockID]);

	return 1;
}

int ShiftRight()
{
	int tmp;

	if (!DetectCollision(curPosX + 2, curPosY, blockModel[blockID]))
	{
		if (!sideCollision(curPosX + 2, curPosY, blockModel[blockID]))
		{
			tmp = GBOARD_ORIGIN_X + 2;
			if (DetectCollision(tmp, curPosY, blockModel[blockID]))
			{
				DeleteBlock(blockModel[blockID]);
				curPosX = tmp;

				while (DetectCollision(curPosX, curPosY, blockModel[blockID]))
				{
					curPosX -= 2;
				}
				curPosX += 2;

				MySetCursor(curPosX, curPosY);
				ShowBlock(blockModel[blockID]);
				return 1;
			}
			else
				return 0;
		}
		else
			return 0;
	}

	DeleteBlock(blockModel[blockID]);
	curPosX += 2;

	MySetCursor(curPosX, curPosY);
	ShowBlock(blockModel[blockID]);

	return 1;
}

int RotateBlock()
{
	int base = blockID - blockID % 4;
	int tmpID = base + (blockID + 1) % 4;

	if (!(DetectCollision(curPosX, curPosY, blockModel[tmpID])))
		return 0;

	DeleteBlock(blockModel[blockID]);

	blockID = tmpID;
	ShowBlock(blockModel[blockID]);

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
		if (_kbhit() != 0)
		{
			key = _getch();

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
	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		MySetCursor(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT)
			printf("└");
		else
			printf("│");

		MySetCursor(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT)

			printf("┘");
		else
			printf("│");
	}

	for (x = 1; x < GBOARD_WIDTH + 1; x++)
	{
		MySetCursor(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
		printf("─");
	}

	for (y = 0; y < GBOARD_HEIGHT; y++)
	{
		gameBoardInfo[y][0] = 2;
		gameBoardInfo[y][GBOARD_WIDTH + 1] = 2;
	}

	for (x = 0; x < GBOARD_WIDTH + 2; x++)
	{
		gameBoardInfo[GBOARD_HEIGHT][x] = 2;
	}
}

int DetectCollision(int posX, int posY, int blockModel[4][4])
{
	int x, y;
	int arrX = (posX - GBOARD_ORIGIN_X) / 2;
	int arrY = posY - GBOARD_ORIGIN_Y;

	for (x = 0; x < 4; x++)
	{
		for (y = 0; y < 4; y++)
		{
			if (blockModel[y][x] == 1 && (gameBoardInfo[arrY + y][arrX + x] == 1 || gameBoardInfo[arrY + y][arrX + x] == 2))
				return 0;
		}
	}
	return 1;
}

int sideCollision(int posX, int posY, int blockModel[4][4])
{
	int x, y;
	int arrX = (posX - GBOARD_ORIGIN_X) / 2;
	int arrY = posY - GBOARD_ORIGIN_Y;

	for (x = 0; x < 4; x++)
	{
		for (y = 0; y < 4; y++)
		{
			if (blockModel[y][x] == 1 && gameBoardInfo[arrY + y][arrX + x] == 2)
				return 0;
		}
	}
	return 1;
}

int BlockCollision(int posX, int posY, int blockModel[4][4])
{
	int x, y;
	int arrX = (posX - GBOARD_ORIGIN_X) / 2;
	int arrY = posY - GBOARD_ORIGIN_Y;

	for (x = 0; x < 4; x++)
	{
		for (y = 0; y < 4; y++)
		{
			if (blockModel[y][x] == 1 && gameBoardInfo[arrY + y][arrX + x] == 1)
				return 0;
		}
	}
	return 1;

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

			if (blockModel[blockID][y][x] == 1)
				gameBoardInfo[arrCurY + y][arrCurX + x] = 1;
		}
	}
}

int IsGameOver()
{
	if (!(DetectCollision(curPosX, curPosY, blockModel[blockID])))
	{
		printf("GameOver");
		return 0;
	}
	return 1;
}

void SpaceDown()
{
	while (ShiftDown());
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

			if (gameBoardInfo[y][x] == 1)
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
		{
			if (gameBoardInfo[y][x] != 1)
				break;
		}

		if (x == (GBOARD_WIDTH + 1))
		{
			for (line = 0; y - line > 0; line++)
				memcpy(&gameBoardInfo[y - line][1], &gameBoardInfo[(y - line) - 1][1], GBOARD_WIDTH * sizeof(int));

			y++;
		}
	}
	RedrawBlocks();
}
