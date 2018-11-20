#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "ItemDesign.h"
#pragma warning (disable:4996)

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
COORD MyGetCursor();
void MySetCursor(int x, int y);
void RemoveCursor();
void RunGame();
void StartGame();
void ShowHelp();
void ExitGame();
void DrawGameBoard();
void ShiftRight();
void ShiftLeft();
void ShiftDown();
void ShiftUp();
void KeyInput();
int curPosX, curPosY;
void MakeTower();

int main()
{

	RunGame();
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

void RunGame()
{
	system("mode con cols=150 lines=44");

	while (1)
	{
		MySetCursor(50, 5);
		for (int i = 0; i < 11; i++)
		{
			for (int j = 0; j < 28; j++)
			{
				MySetCursor(50 + j * 2, 5 + i);
				if (title[i][j] == 1)
					printf("■");
			}
			printf("\n");
		}

		MySetCursor(70, 20);
		printf("1 : PLAY GAME");

		MySetCursor(70, 22);
		printf("2 : HELP");

		MySetCursor(70, 24);
		printf("3 : EXIT");

		char selectMenu = getch();

		if (selectMenu == '1')
		{
			StartGame();
			break;
		}
		else if (selectMenu == '2')
		{
			// 도움말
		}
		else if (selectMenu == '3')
		{
			ExitGame();
			break;
		}
		else
		{
			continue;
		}
	}
}

void StartGame()
{
	system("cls");
	DrawGameBoard();
	MySetCursor(0, 26);
	printf("┌─────────────────────────────────────────────────────┐\n");
	printf("│ 화살타워   - Q 100골드             현재 골드  - 300 │\n");
	printf("│ 대포타워   - W 200골드             스테이지   -  1  │\n");
	printf("│ 슬로우타워 - E 200 골드                             │\n");
	printf("│ 미사일타워 - R 300 골드            폭탄       -  A  │\n");
	printf("└─────────────────────────────────────────────────────┘\n");
	MySetCursor(96, 31);
	printf("♥♥♥♥♥♥♥♥♥♥♥♥");
	MySetCursor(0, 4);
	printf("§");
	MySetCursor(2, 4);
	printf("§");
	MySetCursor(4, 5);
	printf("§");
	MySetCursor(6, 5);
	printf("§");
	MySetCursor(8, 4);
	printf("§");
	MySetCursor(10, 4);
	printf("§");

	while (1)
	{
		while (1)
		{
			KeyInput();
		}
	}
}

void ShowHelp()
{
	system("cls");
}

void ExitGame()
{
	system("cls");
	printf("소프트웨어 기초설계 2반 6조\n");
	printf("박상우\n이다훈\n이종원\n박철우\n");
	printf("===================================\n");
	printf("contact us : psw7205@gmail.com\n");
	printf("https://github.com/psw7205/SWDesign\n");
}

void DrawGameBoard()
{
	for (int i = 0; i < 44; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			MySetCursor(j * 2, i);
			if (mapModel[i][j] == 1)
				printf("#");
		}
		printf("\n");
	}
}
void ShiftRight()
{
	curPosX += 2;
	MySetCursor(curPosX, curPosY);
	return;
}

void ShiftLeft()
{
	curPosX -= 2;
	MySetCursor(curPosX, curPosY);
	return;
}

void ShiftDown()
{
	curPosY++;
	MySetCursor(curPosX, curPosY);
	return;
}

void ShiftUp()
{
	curPosY--;
	MySetCursor(curPosX, curPosY);
}

void KeyInput()
{
	int key;

	for (int i = 0; i < 20; i++)
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
				ShiftUp();
				break;
			case DOWN:
				ShiftDown();
				break;
			case 'Q':
				MakeTower();
				break;
			}
		}
		Sleep(10);
	}
}

void MakeTower()
{
	if (mapModel[curPosY + 1][curPosX] == 0)
	{
		printf("★");
	}
}
