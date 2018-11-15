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
	system("mode con cols=150 lines=40");

	while (1)
	{
		MySetCursor(50, 5);
		for (int i = 0; i < 11; i++)
		{
			for (int j = 0; j < 28; j++)
			{
				MySetCursor(50 + j * 2, 5 + i);
				if (title[i][j] == 1)
					printf("¡á");
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
			// µµ¿ò¸»
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
	MySetCursor(0, 22);
	printf("¦£¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¤\n");
	printf("¦¢ È­»ìÅ¸¿ö   - Q 100°ñµå             ÇöÀç °ñµå  - 300 ¦¢\n");
	printf("¦¢ ´ëÆ÷Å¸¿ö   - W 200°ñµå             ½ºÅ×ÀÌÁö   -  1  ¦¢\n");
	printf("¦¢ ½½·Î¿ìÅ¸¿ö - E 200 °ñµå                             ¦¢\n");
	printf("¦¢ ¹Ì»çÀÏÅ¸¿ö - R 300 °ñµå            ÆøÅº       -  A  ¦¢\n");
	printf("¦¦¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¥\n");
	MySetCursor(96, 27);
	printf("¢¾¢¾¢¾¢¾¢¾¢¾¢¾¢¾¢¾¢¾¢¾¢¾");
	MySetCursor(0, 4);
	printf("¡×");
	MySetCursor(2, 4);
	printf("¡×");
	MySetCursor(4, 4);
	printf("¡×");
	MySetCursor(6, 4);
	printf("¡×");
	MySetCursor(8, 4);
	printf("¡×");
	MySetCursor(10, 4);
	printf("¡×");

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
	printf("¼ÒÇÁÆ®¿þ¾î ±âÃÊ¼³°è 2¹Ý 6Á¶\n");
	printf("¹Ú»ó¿ì\nÀÌ´ÙÈÆ\nÀÌÁ¾¿ø\n¹ÚÃ¶¿ì\n");
	printf("===================================\n");
	printf("contact us : psw7205@gmail.com\n");
	printf("https://github.com/psw7205/SWDesign\n");
}

void DrawGameBoard()
{
	for (int i = 0; i < 40; i++)
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
	printf("¡á");
	MySetCursor(curPosX, curPosY - 1);
	printf("¡Ú");
}