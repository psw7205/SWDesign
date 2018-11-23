#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "ItemDesign.h"
#include "Monsters.h"
#include "Tower.h"
#include "Bullet.h"

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
void PrintCurPos();
void KeyInput();
void MakeTower(int type);
void PrintHelpMenu();
void DrawMob(int type);
void GenerateMob(int type); //make mobs according to the parameter 'num' and link them to the head of the mob type(parameter 'type')
void MoveMob(int type);
void Hit(int bx, int by, int ex, int ey, int id, int *hp, int damage);

int curPosX, curPosY; //these variables are coordinates of the 'mapModel' array, not the coordinates of console screen
int mob1num, mob2num, mob3num, mob4num; //number of each monsters per wave
int stt = 0;
COORD start, end, first_corner;
NPC *head1;
NPC *head2;
NPC *head3;
NPC *head4;
Tower *TowerList;
Bullet *BulletList;
int gold = 9999;
int life = 10;

int main() {
	start.X = 0; start.Y = 5;
	end.X = 118; end.Y = 28;
	first_corner.X = 110; first_corner.Y = 5;

	head1 = MakeMob(1, 1, 1, 'a');
	head2 = MakeMob(1, 1, 1, 'a');
	head3 = MakeMob(1, 1, 1, 'a');
	head4 = MakeMob(1, 1, 1, 'a');

	RunGame();
	getchar();
	return 0;
}

void RemoveCursor() {
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void MySetCursor(int x, int y) {
	COORD pos = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD MyGetCursor() {
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}
void RunGame() {
	system("mode con cols=150 lines=44");
	while (1) {
		MySetCursor(50, 5);
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 28; j++) {
				MySetCursor(50 + j * 2, 5 + i);
				if (title[i][j] == 1)
					printf("■");
			}
			printf("\n");
		}//show title
		MySetCursor(70, 20);
		printf("1 : PLAY GAME");

		MySetCursor(70, 22);
		printf("2 : HELP");

		MySetCursor(70, 24);
		printf("3 : EXIT");

		char selectMenu = getch();
		//show menu and receive key input
		if (selectMenu == '1') {
			StartGame();
			break;
		}
		else if (selectMenu == '2') {
			// 도움말
		}
		else if (selectMenu == '3') {
			ExitGame();
			break;
		}
		else {
			continue;
		}
	}
}
void StartGame() {
	system("cls");
	DrawGameBoard();//draw the road of game screen
	MySetCursor(0, 26);
	PrintHelpMenu();
	MySetCursor(96, 31);
	for (int i = 0; i < life; i++)
		printf("♥");
	//draw game screen info and road
	mob1num = 30;//temporary number
	GenerateMob(1);
	while (1) {
		while (1) {
			KeyInput();
			if (stt == 1) break;
		}//when a wave is ended and player is installing the turret (break time)

		DrawMob(1);
		MoveMob(1);
		//Sleep(4);
	}
}

void ShowHelp() {
	system("cls");
}

void ExitGame() {
	system("cls");
	printf("소프트웨어 기초설계 2반 6조\n");
	printf("박상우\n이다훈\n이종원\n박철우\n");
	printf("===================================\n");
	printf("contact us : psw7205@gmail.com\n");
	printf("https://github.com/psw7205/SWDesign\n");
}
void DrawGameBoard() {
	for (int i = 0; i < 44; i++) {
		for (int j = 0; j < 60; j++) {
			MySetCursor(j * 2, i);
			if (mapModel[i][j] == 1)
				printf("#");
		}
		printf("\n");
	}
} //draw the road of game screen

void ShiftRight() {
	curPosX += 2;
	MySetCursor(curPosX, curPosY);
	return;
}

void ShiftLeft() {
	if (!(curPosX - 2 < 0))curPosX -= 2;
	MySetCursor(curPosX, curPosY);
	return;
}

void ShiftDown() {
	curPosY++;
	MySetCursor(curPosX, curPosY);
	return;
}

void ShiftUp() {
	curPosY--;
	MySetCursor(curPosX, curPosY);
}

void KeyInput() {
	int key;
	for (int i = 0; i < 20; i++) {
		if (_kbhit() != 0) {
			key = _getch();
			switch (key) {
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
			case 'q':
			case 'w':
			case 'e':
			case 'r':
				MakeTower(key);
				break;
			case 'd':
				if (stt == 0)
					stt = 1;
				break;
			}
		}
		Sleep(1);
	}
	PrintCurPos();
}

void GenerateMob(int type) {
	if (type == 1) {
		for (int i = 0; i < mob1num; i++) {
			NPC* make = MakeMob(1, start.X + i * 2, start.Y, 'a');//this paramter will be changed according to the mob design
			if (head1->next == NULL) head1->next = make;
			else {
				make->next = head1->next;
				head1->next = make;
			}
		}
	}
}//this method shlud be called when there is no node which is linked to head

void DrawMob(int type) {
	if (type == 1) {
		NPC* cur = head1->next;
		for (int i = 0; i < mob1num; i++) {
			MySetCursor(cur->prevx, cur->prevy);
			printf(" ");
			MySetCursor(cur->curx, cur->cury);
			printf("%c", cur->shape);
			cur = cur->next;
		}
	}
	MySetCursor(curPosX, curPosY);
}

void MoveMob(int type) {
	NPC*cur = head1->next;
	for (int i = 0; i < mob1num; i++) {
		if (cur->cury == 5 && cur->curx < 112) {
			cur->prevx = cur->curx; cur->prevy = cur->cury;
			(cur->curx) += 2;
			cur = cur->next;
		}//first corner
		else if (cur->cury < 20 && cur->curx == 112) {
			cur->prevx = cur->curx; cur->prevy = cur->cury;
			(cur->cury) += 1;
			cur = cur->next;
		}//second corner
		else if (cur->cury == 20 && 8 < cur->curx) {
			cur->prevx = cur->curx; cur->prevy = cur->cury;
			if (cur->curx == 62 && cur->cury == 20 && cur->corner == -1) {
				(cur->curx) -= 2;
				cur->corner = 1;
			}
			else if (cur->curx == 62 && cur->cury == 20 && cur->corner == 1)(cur->cury) += 1;
			else (cur->curx) -= 2;
			cur = cur->next;
		}//third corner
		else if (11 < cur->cury && 8 == cur->curx) {
			cur->prevx = cur->curx; cur->prevy = cur->cury;
			(cur->cury) -= 1;
			cur = cur->next;
		}//fourth corner
		else if (11 == cur->cury && cur->curx < 62) {
			cur->prevx = cur->curx; cur->prevy = cur->cury;
			(cur->curx) += 2;
			cur = cur->next;
		}//fifth corner
		else if (cur->cury < 28 && cur->curx == 62) {
			cur->prevx = cur->curx; cur->prevy = cur->cury;
			(cur->cury) += 1;
			cur = cur->next;
		}//sixth corner
		else if (cur->cury == 28 && cur->curx < 118) {
			cur->prevx = cur->curx; cur->prevy = cur->cury;
			(cur->curx) += 2;
			cur = cur->next;
		}//goal
		else if (cur->curx == end.X && cur->cury == end.Y) {
			MySetCursor(cur->curx, cur->cury);
			printf(" ");
			RemoveNum(head1, i);
			mob1num--;
		}
	}
}

void PrintCurPos() {
	MySetCursor(0, 40);
	printf("%d %d", curPosX, curPosY);
	MySetCursor(curPosX, curPosY);
}

void MakeTower(int type) {

	if (mapModel[curPosY + 2][curPosX / 2] == 0
		&& mapModel[curPosY + 1][curPosX / 2] == 0
		&& mapModel[curPosY + 1][curPosX / 2 + 1] == 0
		&& mapModel[curPosY + 2][curPosX / 2 + 1] == 0) {
		switch (type)
		{
		case 'q':
			if (gold < 100)
				break;
			printf("q★");
			MySetCursor(curPosX, curPosY+1);
			printf("■■");
			AddTowerMap('q', curPosY, curPosX);
			gold -= 100;
			addTower(&TowerList, 'q', curPosY + 1, curPosX / 2);
			break;
		case 'w':
			if (gold < 200)
				break;
			printf("w☆");
			MySetCursor(curPosX, curPosY + 1);
			printf("■■");
			AddTowerMap('w', curPosY, curPosX);
			addTower(&TowerList, 'w', curPosY + 1, curPosX / 2);
			gold -= 200;
			break;
		case 'e':
			if (gold < 200)
				break;
			printf("e◎");
			MySetCursor(curPosX, curPosY + 1);
			printf("■■");
			AddTowerMap('e', curPosY, curPosX);
			addTower(&TowerList, 'e', curPosY + 1, curPosX / 2);
			gold -= 200;
			break;
		case 'r':
			if (gold < 300)
				break;
			printf("r◈");
			MySetCursor(curPosX, curPosY + 1);
			printf("■■");
			AddTowerMap('r', curPosY, curPosX);
			addTower(&TowerList, 'r', curPosY + 1, curPosX / 2);
			gold -= 300;
			break;
		}
		MySetCursor(0, 26);
		PrintHelpMenu();
	}
}

void PrintHelpMenu()
{
	printf("┌─────────────────────────────────────────────────────┐\n");
	printf("│ 화살타워   - Q 100골드             현재 골드  - %4d│\n", gold);
	printf("│ 대포타워   - W 200골드             스테이지   -  1  │\n");
	printf("│ 슬로우타워 - E 200 골드                             │\n");
	printf("│ 미사일타워 - R 300 골드            폭탄       -  A  │\n");
	printf("└─────────────────────────────────────────────────────┘\n");
}

void Hit(int bx, int by, int ex, int ey, int id, int *hp, int damage)
{

}