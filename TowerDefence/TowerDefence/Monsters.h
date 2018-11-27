#pragma once

typedef struct NPC {
	int hp;
	int curx;
	int cury;
	int move_flag;
	char shape; // 몬스터 종류 플래그.
}NPC;
char monsterModel[][2][2] = {
	{
		{ 1, 1 },
		{ 1, 1 }
	},
	{
		{ 1, 1 },
		{ 1, 1 }
	},
	{
		{ 1, 1 },
		{ 1, 1 }
	},
	{
		{ 1, 1 },
		{ 1, 1 }
	}
}; // 몬스터 종류 만들기.