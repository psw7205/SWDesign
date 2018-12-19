/*
몬스터 : 불, 물, 나무, 보스(노멀)
타워 : 불, 물, 나무, 노멀

상성표: 불->나무, 물->불, 나무->물 (데미지 150%)
		나무->불, 불->물, 물->나무 (데미지 50%)
		그 외 (100%)

스테이지1: 불5물5 (체력100)
스테이지2: 불5물5 (체력150)
스테이지3: 불4물4나무2 (체력150)
스테이지4: 불4물4나무2 (체력200)
스테이지5: 불3물3나무3보스1 (체력200)(보스체력300(모든공격50%))
각각 상성나쁨(150%), 상성좋음(50%), 보통(100%)
*/

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <mmsystem.h>

#include "ItemDesign.h"
#include "Monsters.h"
#include "Tower.h"

#pragma comment(lib,"winmm.lib")
#pragma warning (disable:4996)
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27

#define SPEED 10
#define TIME 150

COORD MyGetCursor();
void MySetCursor(int x, int y);
void RemoveCursor();
void StartMenu();
void HelpGame();
void RunGame();
void StartGame();
void ExitGame();
void DrawGameBoard();
void ShiftRight();
void ShiftLeft();
void ShiftDown();
void ShiftUp();
void PrintCurPos();
void KeyInput(NPC *mon);
void PrintHelpMenu();
void PrintLife();
void SelectTower(int type);
int isGameOver();
void GameOver();
void ShowMonster(char monsterInfo[2][2], int shape, int hp, int max_hp, int mx, int my);
int curPosX, curPosY;
int start_flag = 0;
int DetectCollision(int posX, int posY);
int MoveMonster(NPC *mon);
void DC_chk(NPC *mon);
void InitMonster(NPC *mon, int i);
void DeleteTower();
void bomb(NPC *mon);
void DeleteMonster(char monsterInfo[2][2], int mx, int my);
void printfhelp();
void GameWin();
void gamePause();
COORD start, end, first_corner;

int gold;
int life;
int Time;
int Stage;

int main() {
	RunGame();
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
void StartMenu()
{
	system("cls");
	MySetCursor(16, 5);
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 28; j++) {
			MySetCursor(16 + j * 2, 5 + i);
			if (title[i][j] == 1)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
				printf("■");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
		}
		printf("\n");
	}//show title
	MySetCursor(36, 20);
	printf("1 : PLAY GAME");

	MySetCursor(36, 22);
	printf("2 : HELP");

	MySetCursor(36, 24);
	printf("3 : EXIT");
}

void RunGame() {
	system("mode con cols=86 lines=42");
	PlaySound(TEXT("gameStart.wav"), NULL, SND_ASYNC);
	while (1) {
		StartMenu();
		char selectMenu = getch();
		//show menu and receive key input
		if (selectMenu == '1') {
			StartGame();
			break;
		}
		else if (selectMenu == '2') {
			HelpGame();
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
	NPC *mon;
	mon = MakeMonster();
	Stage = 1;
	life = 10;
	gold = 500;
	Time = TIME;
	DrawGameBoard();//draw the road of game screen
	MySetCursor(60, 35);
	PrintLife();
	MySetCursor(0, 32);
	PrintHelpMenu();

	PlaySound(TEXT("stage.wav"), NULL, SND_ASYNC | SND_LOOP);

	while (1)
	{
		if (isGameOver())
		{
			GameOver();
			break;
		}

		KeyInput(mon);

		if (Time <= 0)
		{
			start_flag = 1;
			Time = TIME;
		}

		if (start_flag == 1)
		{
			start_flag = MoveMonster(mon);
		}
		else
		{
			// Time
			MySetCursor(68, 33);
			printf("Time Limit:%3ds", Time / 10);
			Time--;
			MySetCursor(curPosX, curPosY);
		}
	}

}

int isGameOver()
{
	if (life <= 0)
		return 1;

	return 0;
}

void ExitGame() {
	system("cls");
	printf("소프트웨어 기초설계 2반 6조\n");
	printf("박상우\n이다훈\n이종원\n박철우\n");
	printf("===================================\n");
	printf("contact us : psw7205@gmail.com\n");
	printf("https://github.com/psw7205/SWDesign\n");

	Sleep(4 * 1000);
	exit(0);
}

void DrawGameBoard() {
	for (int i = 0; i < 33; i++) {
		for (int j = 0; j < 43; j++) {
			MySetCursor(j * 2, i);
			if (mapModel[i][j] == 1)
				printf("■");
		}
		printf("\n");
	}
} //draw the road of game screen

void ShiftRight() {
	curPosX += 2;
	if (curPosX > 82)
	{
		curPosX -= 2;
	}
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
	if (curPosY > 30)
	{
		curPosY--;
	}
	MySetCursor(curPosX, curPosY);
	return;
}

void ShiftUp() {
	curPosY--;
	if (curPosY < 0)
	{
		curPosY++;
	}
	MySetCursor(curPosX, curPosY);
}

void KeyInput(NPC *mon) {
	int key;
	for (int i = 0; i < 10; i++) {
		if (_kbhit() != 0) {
			key = _getch();
			switch (key) {
			case LEFT:
				if (start_flag == 0)
				{
					ShiftLeft();
				}
				break;
			case RIGHT:
				if (start_flag == 0)
				{
					ShiftRight();
				}
				break;
			case UP:
				if (start_flag == 0)
				{
					ShiftUp();
				}
				break;
			case DOWN:
				if (start_flag == 0)
				{
					ShiftDown();
				}
				break;
			case 'q':
			case 'w':
			case 'e':
			case 'r':
				if (start_flag == 0)
				{
					SelectTower(key);
				}
				break;
			case 'd':
				if (start_flag == 0)
				{
					DeleteTower();
				}
				break;
			case 's':
				if (life == 0)
				{
					life = 10;
				}
				MySetCursor(60, 35);
				PrintLife();
				start_flag = 1;
				Time = TIME;
				break;
			case 'a':
				if (start_flag == 1)
				{
					bomb(mon);
				}
				break;
			case ESC:
				gamePause();
				break;
			}
		}
		Sleep(SPEED);
	}
	PrintCurPos();
}

void gamePause()
{
	system("pause>null");
}


///// 몬스터 부분.
void ShowMonster(char monsterInfo[2][2], int shape, int hp, int max_hp, int mx, int my) { // 1로 되어있는 곳을 보여준다.
	int x, y;

	for (y = 0; y < 2; y++) {
		for (x = 0; x < 2; x++) {
			MySetCursor(mx + (x * 2), my + y);
			switch (shape)
			{
			case 2:
				if (monsterInfo[y][x] == 1) {
					if ((hp <= ((max_hp / 10.0) * 6)) && (hp > ((max_hp / 10.0) * 3)))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
						printf("火");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
					else if (hp <= ((max_hp / 10.0) * 6))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
						printf("火");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
					else
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
						printf("火");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				break;
			case 3:
				if (monsterInfo[y][x] == 1) {
					if ((hp <= ((max_hp / 10.0) * 6)) && (hp > ((max_hp / 10.0) * 3)))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
						printf("水");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
					else if (hp <= ((max_hp / 10.0) * 6))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
						printf("水");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
					else
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
						printf("水");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				break;
			case 4:
				if (monsterInfo[y][x] == 1) {
					if ((hp <= ((max_hp / 10.0) * 6)) && (hp > ((max_hp / 10.0) * 3)))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
						printf("木");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
					else if (hp <= ((max_hp / 10.0) * 6))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
						printf("木");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
					else
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
						printf("木");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				break;
			case 5:
				if (monsterInfo[y][x] == 1) {
					if ((hp <= ((max_hp / 10.0) * 6)) && (hp > ((max_hp / 10.0) * 3)))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
						printf("王");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
					else if (hp <= ((max_hp / 10.0) * 6))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
						printf("王");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
					else
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
						printf("王");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
				}
				break;
			}
		}
	}
	MySetCursor(mx, my);
}

void bomb(NPC*mon) {
	int i;
	int bombprice = 300;
	if (gold < bombprice) return;
	gold -= bombprice;

	for (int i = 0; i < 33; i++) {
		for (int j = 0; j < 43; j++) {
			MySetCursor(j * 2, i);
			if (mapModel[i][j] == 9)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				printf("★");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
		}
		printf("\n");
	}
	Sleep(100);
	for (int i = 0; i < 33; i++) {
		for (int j = 0; j < 43; j++) {
			MySetCursor(j * 2, i);
			if (mapModel[i][j] == 9)
				printf(" ");
		}
		printf("\n");
	}

	for (i = 0; i < 10; i++) {
		if (mon[i].curx > 0) // 맵에 있는 몬스터만 폭탄
		{
			mon[i].hp -= 200; // 폭탄 데미지
		}
		if (mon[i].hp <= 0) // 몬스터 hp0가 일경우
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			InitMonster(mon, i);// set up the in
		}
	}

	MySetCursor(0, 32);
	PrintHelpMenu();
}

void DeleteMonster(char monsterInfo[2][2], int mx, int my) { // 1로 되어있는 곳을 지운다.
	int x, y;

	for (y = 0; y < 2; y++) {
		for (x = 0; x < 2; x++) {
			MySetCursor(mx + (x * 2), my + y);

			if (monsterInfo[y][x] == 1) {
				printf(" ");
			}
		}
	}
	MySetCursor(mx, my);
}

void InitMonster(NPC *mon, int i)
{
	if (Stage == 1) // 스테이지 2에서는 기존(체력업)
	{
		mon[i].move_flag = 0;
		mon[i].curx = (-16 * i);
		mon[i].cury = 3;
		mon[i].hp = 150;
		mon[i].max_hp = 150;
	}
	else if (Stage == 2) // 스테이지 3에서는 4,4,2(new속성)
	{
		mon[i].move_flag = 0;
		mon[i].curx = (-16 * i);
		mon[i].cury = 3;
		mon[i].hp = 150;
		mon[i].max_hp = 150;
		if (i < 4)
		{
			mon[i].shape = 2;
		}
		else if (i < 8)
		{
			mon[i].shape = 3;
		}
		else
		{
			mon[i].shape = 4;
		}
	}
	else if (Stage == 3) // 스테이지 5에서는 4,4,2(new속성) (체력업)
	{
		mon[i].move_flag = 0;
		mon[i].curx = (-16 * i);
		mon[i].cury = 3;
		mon[i].hp = 200;
		mon[i].max_hp = 200;
		if (i < 4)
		{
			mon[i].shape = 2;
		}
		else if (i < 8)
		{
			mon[i].shape = 3;
		}
		else
		{
			mon[i].shape = 4;
		}
	}
	else if (Stage == 4) // 스테이지 5에서는 3,3,3,1(보스추가)
	{
		mon[i].move_flag = 0;
		mon[i].curx = (-16 * i);
		mon[i].cury = 3;
		mon[i].hp = 200;
		mon[i].max_hp = 200;
		if (i < 3)
		{
			mon[i].shape = 2;
		}
		else if (i < 6)
		{
			mon[i].shape = 3;
		}
		else if (i < 9)
		{
			mon[i].shape = 4;
		}
		else
		{
			mon[i].shape = 5;
			mon[i].hp = 300;
			mon[i].max_hp = 300;
		}
	}

}

void DC_chk(NPC *mon)
{
	int bullet;
	int type;
	int i;

	for (i = 0; i < 2; i++) // 길 양쪽 체크
	{
		if (mon->move_flag % 2 == 0) // 세로 길
		{
			bullet = DetectCollision(mon->curx + (i * 2), mon->cury);
		}
		else // 가로 길
		{
			bullet = DetectCollision(mon->curx, mon->cury + i);
		}
		type = bullet / 10;
		type %= 10; // bullet의 10의자리는 몬스터 종류(Shape)
		if (bullet != 0)
		{
			if ( // 50%의 데미지
				(mon->shape == 2 && type == 4) ||
				(mon->shape == 3 && type == 2) ||
				(mon->shape == 4 && type == 3) ||
				(type == 5)
				)
			{
				mon->hp -= ((bullet / 100) / 2);
				if (mon->hp <= 0) {
					DeleteMonster(monsterModel[0], mon->curx, mon->cury);
					gold += 20;
					mon->move_flag = 0;

					MySetCursor(0, 32);
					PrintHelpMenu();
				}
			}
			else if ( // 150%의 데미지
				(mon->shape == 4 && type == 2) ||
				(mon->shape == 2 && type == 3) ||
				(mon->shape == 3 && type == 4) ||
				(mon->shape == 5)
				)
			{
				mon->hp -= ((bullet / 100) + ((bullet / 100) / 2));

				if (mon->hp <= 0) {
					DeleteMonster(monsterModel[0], mon->curx, mon->cury);
					gold += 20;
					mon->move_flag = 0;

					MySetCursor(0, 32);
					PrintHelpMenu();
				}
			}
			else if ( // 100%의 데미지
				(mon->shape == 4 && type == 4) ||
				(mon->shape == 3 && type == 3) ||
				(mon->shape == 2 && type == 2)
				)
			{
				mon->hp -= (bullet / 100);
				if (mon->hp <= 0) {
					DeleteMonster(monsterModel[0], mon->curx, mon->cury);
					gold += 20;
					mon->move_flag = 0;

					MySetCursor(0, 32);
					PrintHelpMenu();
				}
			}

			//printf("%d", mon->hp);
		}
	}
}

int MoveMonster(NPC *mon) {
	int i = 0;
	int bullet = 0;

	for (i = 0; i < 10; i++)
	{
		if (mon[i].move_flag == 1)
		{
			if (mon[i].curx >= 0)
			{
				DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			}
			mon[i].curx = mon[i].curx + 2;
			MySetCursor(mon[i].curx, mon[i].cury);
			if (mon[i].curx >= 0)
			{
				ShowMonster(monsterModel[0], mon[i].shape, mon[i].hp, mon[i].max_hp, mon[i].curx, mon[i].cury);
				DC_chk(&mon[i]);
			}
			if (mon[i].curx == 76) { mon[i].move_flag = 2; }
		}
		if (mon[i].move_flag == 2)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].cury = mon[i].cury + 1;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].shape, mon[i].hp, mon[i].max_hp, mon[i].curx, mon[i].cury);
			DC_chk(&mon[i]);
			if (mon[i].cury == 19) { mon[i].move_flag = 3; }
		}
		if (mon[i].move_flag == 3)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].curx = mon[i].curx - 2;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].shape, mon[i].hp, mon[i].max_hp, mon[i].curx, mon[i].cury);
			DC_chk(&mon[i]);
			if (mon[i].curx == 8) { mon[i].move_flag = 4; }
		}
		if (mon[i].move_flag == 4)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].cury = mon[i].cury - 1;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].shape, mon[i].hp, mon[i].max_hp, mon[i].curx, mon[i].cury);
			DC_chk(&mon[i]);
			if (mon[i].cury == 11) { mon[i].move_flag = 5; }
		}
		if (mon[i].move_flag == 5)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].curx = mon[i].curx + 2;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].shape, mon[i].hp, mon[i].max_hp, mon[i].curx, mon[i].cury);
			DC_chk(&mon[i]);
			if (mon[i].curx == 42) { mon[i].move_flag = 6; }
		}
		if (mon[i].move_flag == 6)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].cury = mon[i].cury + 1;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].shape, mon[i].hp, mon[i].max_hp, mon[i].curx, mon[i].cury);
			DC_chk(&mon[i]);
			if (mon[i].cury == 27) { mon[i].move_flag = 7; }
		}
		if (mon[i].move_flag == 7)
		{
			DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
			mon[i].curx = mon[i].curx + 2;
			MySetCursor(mon[i].curx, mon[i].cury);
			ShowMonster(monsterModel[0], mon[i].shape, mon[i].hp, mon[i].max_hp, mon[i].curx, mon[i].cury);
			DC_chk(&mon[i]);
			if (mon[i].curx >= 80) // 몬스터가 끝까지 갔을 경우.
			{
				DeleteMonster(monsterModel[0], mon[i].curx, mon[i].cury);
				InitMonster(mon, i);
				start_flag = 0;

				life--;
				MySetCursor(60, 35);
				PrintLife();
			}
		}
	}
	if (mon[9].move_flag == 0
		&& mon[0].move_flag == 0
		&& mon[1].move_flag == 0
		&& mon[2].move_flag == 0
		&& mon[3].move_flag == 0
		&& mon[4].move_flag == 0
		&& mon[5].move_flag == 0
		&& mon[6].move_flag == 0
		&& mon[7].move_flag == 0
		&& mon[8].move_flag == 0
		)
	{
		for (i = 0; i < 10; i++)
		{
			InitMonster(mon, i);
			mon[i].move_flag = 1;
		}

		Stage++;
		if (Stage == 6 && life >= 0)
			GameWin();

		MySetCursor(0, 32);
		PrintHelpMenu();
		return 0;
	}

	return 1;
}
///// 몬스터 부분.

void PrintCurPos() {
	MySetCursor(78, 37);
	printf("(%2d, %2d)", curPosX, curPosY);
	MySetCursor(curPosX, curPosY);
}

int DetectCollision(int posX, int posY) { // 충돌체크.
	int dX, dY;
	int tmp;
	dX = posX / 2;
	dY = posY;
	tmp = TowerModel[dY][dX];
	if (tmp != 1) {
		return tmp;
	}
	return 0;
}

void SelectTower(int type) {

	if (TowerModel[curPosY + 1][curPosX / 2] % 10 != 1
		&& TowerModel[curPosY][curPosX / 2] % 10 != 1
		&& TowerModel[curPosY][curPosX / 2 + 1] % 10 != 1
		&& TowerModel[curPosY + 1][curPosX / 2 + 1] % 10 != 1
		&& TowerModel[curPosY + 1][curPosX / 2] % 10 != 2
		&& TowerModel[curPosY][curPosX / 2] % 10 != 2
		&& TowerModel[curPosY][curPosX / 2 + 1] % 10 != 2
		&& TowerModel[curPosY + 1][curPosX / 2 + 1] % 10 != 2
		&& TowerModel[curPosY + 1][curPosX / 2] % 10 != 3
		&& TowerModel[curPosY][curPosX / 2] % 10 != 3
		&& TowerModel[curPosY][curPosX / 2 + 1] % 10 != 3
		&& TowerModel[curPosY + 1][curPosX / 2 + 1] % 10 != 3
		&& TowerModel[curPosY + 1][curPosX / 2] % 10 != 4
		&& TowerModel[curPosY][curPosX / 2] % 10 != 4
		&& TowerModel[curPosY][curPosX / 2 + 1] % 10 != 4
		&& TowerModel[curPosY + 1][curPosX / 2 + 1] % 10 != 4
		&& TowerModel[curPosY + 1][curPosX / 2] % 10 != 5
		&& TowerModel[curPosY][curPosX / 2] % 10 != 5
		&& TowerModel[curPosY][curPosX / 2 + 1] % 10 != 5
		&& TowerModel[curPosY + 1][curPosX / 2 + 1] % 10 != 5)
	{
		switch (type)
		{
		case 'q':
			if (gold < 100)
				break;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			BuildTower(type, curPosX, curPosY);
			printf(" ◆");
			MySetCursor(curPosX, curPosY + 1);
			printf("◆◆");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			gold -= 100;
			break;
		case 'w':
			if (gold < 100)
				break;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
			BuildTower(type, curPosX, curPosY);
			printf(" ◆");
			MySetCursor(curPosX, curPosY + 1);
			printf("◆◆");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			gold -= 100;
			break;
		case 'e':
			if (gold < 100)
				break;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			BuildTower(type, curPosX, curPosY);
			printf(" ◆");
			MySetCursor(curPosX, curPosY + 1);
			printf("◆◆");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			gold -= 100;
			break;
		case 'r':
			if (gold < 200)
				break;
			BuildTower(type, curPosX, curPosY);
			printf(" ◈");
			MySetCursor(curPosX, curPosY + 1);
			printf("◈◈");
			gold -= 200;
			break;
		}
		MySetCursor(0, 32);
		PrintHelpMenu();
	}
}
void DeleteTower()
{
	int tx, ty;
	tx = curPosX / 2;
	ty = curPosY;
	if (TowerModel[ty][tx] % 10 != 1 &&
		TowerModel[ty][tx + 1] % 10 != 1 &&
		TowerModel[ty + 1][tx] % 10 != 1 &&
		TowerModel[ty + 1][tx + 1] % 10 != 1 &&
		TowerModel[ty][tx] % 10 != 0 &&
		TowerModel[ty][tx + 1] % 10 != 0 &&
		TowerModel[ty + 1][tx] % 10 != 0 &&
		TowerModel[ty + 1][tx + 1] % 10 != 0)
	{
		printf("    ");
		MySetCursor(curPosX, curPosY + 1);
		printf("    ");
		if (((TowerModel[ty + 2][tx]) / 10) % 10 == 5)
			gold += 150;
		else
			gold += 50;
		DeleteBullet(curPosX, curPosY);
		MySetCursor(0, 32);
		PrintHelpMenu();
	}
}
void PrintHelpMenu()
{
	printf("┌─────────────────────────────────────────────────────┐\n");
	printf("│ 火 타워   - Q  100 골드           현재 골드  - %4d │\n", gold);
	printf("│ 水 타워   - W  100 골드           스테이지   - %1d    │\n", Stage);
	printf("│ 木 타워   - E  100 골드                             │\n");
	printf("│ 올 타워   - R  200 골드                             │\n");
	printf("│ 타워 삭제 - D +50%% 골드                             │\n");
	printf("│ 폭탄      - A  300 골드                             │\n");
	printf("└─────────────────────────────────────────────────────┘\n");
}

void GameOver()
{
	system("cls");
	PlaySound(TEXT("gameOver.wav"), NULL, SND_ASYNC | SND_LOOP);
	MySetCursor(16, 5);
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 28; j++) {
			MySetCursor(16 + j * 2, 5 + i);
			if (ending[i][j] == 1)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				printf("■");
			}
		}
		printf("\n");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	printf("\n\n");
	printf("\t\t 소프트웨어 기초설계 2반 6조\n");
	printf("\t\t 박상우 이다훈 이종원 박철우\n");
	printf("\t\t ===================================\n");
	printf("\t\t contact us : psw7205@gmail.com\n");
	printf("\t\t https://github.com/psw7205/SWDesign\n");
	printf("\n");

	Sleep(7 * 1000);
	exit(0);
}

void HelpGame()
{
	system("cls");
	int key;

	printfhelp();
	while (1)
	{
		if (_kbhit() != 0)
		{
			key = _getch();
			if (key == ESC)
			{
				StartMenu();
				break;
			}
		}
	}
}

void PrintLife()
{
	int i;
	MySetCursor(60, 35);
	if (life == 10)
	{
		for (i = 0; i < life; i++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			printf("♥");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
	else
	{
		for (i = 0; i < 10 - life; i++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			printf("♡");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
}
void printfhelp() {

	MySetCursor(15, 5);
	printf("┌──────────────────────────────────────────────────────┐\n");
	MySetCursor(15, 6);
	printf("│                                                      │\n");
	MySetCursor(15, 7);
	printf("│                                                      │\n");
	MySetCursor(15, 8);
	printf("│                                                      │\n");
	MySetCursor(15, 9);
	printf("│       ↑                          Q = 火타워 건설    │\n");
	MySetCursor(15, 10);
	printf("│     ←  → 플레이어 이동          W = 水타워 건설    │\n");
	MySetCursor(15, 11);
	printf("│       ↓                          E = 木타워 건설    │\n");
	MySetCursor(15, 12);
	printf("│                                   R = 노말 타워 건설 │\n");
	MySetCursor(15, 13);
	printf("│                                                      │\n");
	MySetCursor(15, 14);
	printf("│                                                      │\n");
	MySetCursor(15, 15);
	printf("│                                                      │\n");
	MySetCursor(15, 16);
	printf("│                      A = 폭탄                        │\n");
	MySetCursor(15, 17);
	printf("│                      S = 대기 시간 SKIP              │\n");
	MySetCursor(15, 18);
	printf("│                      D = 타워 삭제                   │\n");
	MySetCursor(15, 19);
	printf("│                                                      │\n");
	MySetCursor(15, 20);
	printf("│                                                      │\n");
	MySetCursor(15, 21);
	printf("│                                                      │\n");
	MySetCursor(15, 22);
	printf("│         ESC키를 누르면 메인화면으로 이동합니다.      │\n");
	MySetCursor(15, 23);
	printf("│                                                      │\n");
	MySetCursor(15, 24);
	printf("│                                                      │\n");
	MySetCursor(15, 25);
	printf("└──────────────────────────────────────────────────────┘\n");
}

void GameWin()
{
	system("cls");
	PlaySound(TEXT("gameEnd.wav"), NULL, SND_ASYNC | SND_LOOP);

	MySetCursor(16, 5);
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 19; j++) {
			MySetCursor(16 + j * 2, 5 + i);
			if (winMessage[i][j] == 1)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				printf("■");
			}
		}
		printf("\n");
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	printf("\n");
	printf("\t\t 소프트웨어 기초설계 2반 6조\n");
	printf("\t\t 박상우 이다훈 이종원 박철우\n");
	printf("\t\t ===================================\n");
	printf("\t\t contact us : psw7205@gmail.com\n");
	printf("\t\t https://github.com/psw7205/SWDesign\n");
	printf("\n");

	Sleep(11 * 1000);
	exit(0); // 다시게임하기 구현 실패, 맵 배열들 초기화 필요
}