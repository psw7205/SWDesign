int flag; // 전역변수
void KeyInput(); // flag에 따라 회전 방향 변경
void RemoveFillUpLine(); // 한줄 삭제 때 마다 flag변경

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
				if (flag == 0)
					RotateBlock();
				if (flag == 1)
					RRotateBlock();
				break;

			case SPACE:
				SpaceDown();
				break;

			default:
				break;
			}
		}
		Sleep(SPEED);
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
			if (flag == 0) // 한줄 삭제 시 flag 변경
				flag = 1;
			else
				flag = 0;
		}
	}
	RedrawBlocks();
}
