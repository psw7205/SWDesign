#include <stdio.h>
#include <time.h>

#include "TD.h"
#include "kb_input2.h"
#include "console_draw2.h"
#pragma warning(disable: 4996) 
#define MF_WIDTH  15
#define MF_HEIGHT 7
#define MAP_WIDTH  50
#define MAP_HEIGHT 26
#define MS_START_POINT_X  -3
#define MS_START_POINT_Y  5
#define END_POINT_X  47
#define END_POINT_Y  9

/* ?域變數宣告區 */

clock_t Timer;
clock_t MSMoveTimer;
int EndPointTimer=0;

int MenuFrame[MF_HEIGHT][MF_WIDTH]={0};
int map[MAP_HEIGHT][MAP_WIDTH*2]={0};

int HP;
int money;
int score=0;

int quickMove=1;

/* ?域變數宣告區 Ending */

/******************* struct 集合區 *******************/

/* 選單 */
struct t_menu {
   int x;
   int y;
   int fgcolor;
   int bgcolor;
   int cursor;
   int num_options;
   int state[10];
   char infom[10][20];  /* 塔的資訊 */
   char text[10][40];
	char alt_text[10][40];
};
typedef struct t_menu Menu;

/* 怪物資訊 */
struct t_moster {
	int x;
	int y;
	int color;
	int state; /* 0:移動? 1:?擊中 */
	int kind;
	int direct;
	int hp;
	int beAtk;
	int value;
	int deValue;
	int shape[3][6];
	struct t_moster *nextPtr;
};
typedef struct t_moster Moster;

/* Tower 資訊 */
struct t_toewr {
	int x;
	int y;
	int level;
	int UPMoney;
	int sMoney;
	int kind;
	int atkValue;
	int speed;
	int shape[3][3];
	int atkRage;
	int atking; /* 0:無 1:?擊中 */ 
	clock_t timer;
	struct t_toewr *nextPtr;
};
typedef struct t_toewr Tower;

/* 控制游標元件 */
struct t_choice {
	int x;
	int y;
	int color;
	int shape[2][4];
};
typedef struct t_choice Choice;

/******************* struct 集合區 Ending *******************/

/* 函式宣告區 */

extern int sin_game(void);    /* 遊戲??傳???，告訴?式玩家是?要?刻離?遊戲 */

void setMap(void);
void showMap(int x,int y);

void setExitMenu(Menu *m);
void showExitMenu(Menu *m);

Moster* genMoster(Moster *Fm,int mos_num,int op_x,int op_y,int level);
void showMoster(Moster *m,int map_x,int map_y);
void moveMoster(Moster *m);
Moster* deleteMoster(Moster *Fm,int num);

void Attack(Moster *m,Tower *t);
Moster* ColorAndDead(Moster *m);
int CheckMSAllDead(Moster *m);

Tower* createTower(Tower *FT,int kind,int op_x,int op_y);
void showTower(Tower *FT,int map_x,int map_y);
int TowerCheck(int cu_x,int cu_y,Tower *FT);
int setTowerMenu(Tower *FT,Menu *m,int x,int y);
void showTowerMenu(Menu *m);
int UpGrade(Tower *FT,int x,int y);
Tower* deleteTower(Tower *FT,int x,int y);

void EndPoint(int map_x,int map_y,int state);

/* 函式宣告區 Ending */

/* 單人模式 */
int sin_game(void)
{
	int vk = 0;
	int IsEnding=0;
	int Exit=0;
	int moster_step=0;
	int gen_mos_num=0;
	int wave=1;
	int nextTime=10;
	float MSMoveTime;
	
	int map_x=0;
	int map_y=0;
	
	int i,j;
	int choice_shape[2][4]={
		0x3C, 0x20, 0x20, 0x3E,
		0x3C, 0x20, 0x20, 0x3E };
	int showCre=0;
	int TS[3][6];
	int noMoney=0;
	int UG=0;  /* 檢查升?是?可?的變數 */
	int showTM=0;
	
	char toString[100];  /* 把數字變成字? */
	
	Menu exit,TM;
	Choice CI;
	Moster *First_m; /* 用?標?向第?隻怪物 */
	Tower *First_t;
	
	
	
	/* 製?選單???*/
	for (i = 0; i < MF_HEIGHT; i++) {
		MenuFrame[i][0] = 1;
		MenuFrame[i][MF_WIDTH-1] = 1;
	}
	for (j = 0; j < MF_WIDTH; j++) {
		MenuFrame[0][j] = 1;
		MenuFrame[MF_HEIGHT-1][j] = 1;
	}
   
	/* 初始化選擇用的元件 */
	CI.x = 15;
	CI.y = 10;
	CI.color = 14;
	for(i=0;i<2;i++)
		for(j=0;j<4;j++)
			CI.shape[i][j]=choice_shape[i][j];
	
	/* 初始化 */
	First_m = NULL;
	First_t = NULL;
	HP=100;
	money=600;
	score=0;	
	setMap();	/* 初始化地圖 */
	Timer = clock();
	MSMoveTimer = clock();
	
	clearScreen();	/* ?將畫?清空 */
	while(IsEnding != 1) {
		
		if(waitForKeyDown(0.1))
      	vk=getKeyEventVirtual();
		
		if(Exit == 0) {
			
		/* 主要畫?顯?區 */
			switch (vk) {
      		case VK_ESCAPE:  /* 如果按下 ESC 鍵就跳出離?選單 */ 
					Exit=1;    break;
				case VK_UP:
					if(showTM == 0) {
						if(CI.y > 0)
							CI.y--;
						else if(map_y > 0)
							map_y--;
					} else {
						if(TM.cursor != 0) {
							TM.state[TM.cursor]=0;
							TM.cursor--;
							TM.state[TM.cursor]=1;
						}
					}
					break;
				case VK_DOWN:
					if(showTM == 0) {
						if(CI.y < HEIGHT-5)
							CI.y++;
						else if(map_y < MAP_HEIGHT - HEIGHT)
							map_y++;
					} else {
						if(TM.cursor != 2) {
							TM.state[TM.cursor]=0;
							TM.cursor++;
							TM.state[TM.cursor]=1;
						}
					}
					break;
				case VK_RIGHT:
					if(showTM == 0) {
						if(CI.x < WIDTH-5)
							CI.x++;
						else if(map_x < MAP_WIDTH - WIDTH)
							map_x++;
					}
					break;
				case VK_LEFT:
					if(showTM == 0) {
						if(CI.x > 0)
							CI.x--;
						else if(map_x > 0)
							map_x--;
					}
					break;
				case VK_RETURN:
					if(showTM == 0) {
						if(showCre == 1) {
							 if(money < 150 && noMoney == 0)
								noMoney=1;
							else if (noMoney == 1)
								noMoney=0;
							else if (TowerCheck(CI.x + map_x + 1,CI.y + map_y + 1,First_t)) { 
								First_t = createTower(First_t,1,CI.x + map_x + 1,CI.y + map_y + 1);
								money-=150;
							} 
						} else if(setTowerMenu(First_t,&TM,CI.x + map_x + 1,CI.y + map_y + 1)) {
							showTM = 1;
						}
					} else {
						if(TM.cursor == 0) {
							UG = UpGrade(First_t,CI.x + map_x + 1,CI.y + map_y + 1);
							if( UG == 1 )
								setTowerMenu(First_t,&TM,CI.x + map_x + 1,CI.y + map_y + 1);
							else if ( UG == 0 ){
								if(noMoney == 0)
									noMoney=1;
								else
									noMoney=0;
							}
						} else if (TM.cursor == 1) {
							First_t = deleteTower(First_t,CI.x + map_x + 1,CI.y + map_y + 1);
							showTM = 0;
						} else if (TM.cursor == 2) {
							noMoney = 0;
							showTM = 0;
						}
					} 
					break;
				case VK_Q:
					if(showTM == 0) {
						if(showCre == 0)
							showCre=1;
						else {
							if(noMoney == 1)
								noMoney=0;
							showCre=0;
						}
					}
					break;
				case VK_F:  /* skip 掉等待?間 */
					if(nextTime != 0)
						nextTime=0;
					break;
				case VK_S:
					if(quickMove == 3)
						quickMove = 1;
					else
						quickMove = 3;
					break;
			} /* switch (vk) Ending */
			
		/* 第二種怪物走比較快 */
		if(First_m != NULL && First_m->kind == 2 )
			MSMoveTime=0.3/quickMove;
		else
			MSMoveTime=0.6/quickMove;
		
		/* 每過?段?間，怪物前進?步 */
		if((float)(clock()-MSMoveTimer) > CLOCKS_PER_SEC * MSMoveTime) {
			/* 每走五步且怪物在 10 隻以內產生?隻怪物 */
			if(moster_step%5 == 0 && gen_mos_num < 10 && nextTime == 0) {
				First_m = genMoster(First_m,(wave-1)%3+1,
				MS_START_POINT_X,MS_START_POINT_Y,(wave-1)/3+1); /* 產生怪物與資訊 */
				gen_mos_num++;
			}
			if(nextTime == 0) {
				moveMoster(First_m);  /* 讓怪物「?」往前移動?步 */
				moster_step++;
				if(CheckMSAllDead(First_m)) {
					moster_step=0;
					gen_mos_num=0;
					nextTime=5;  /* 下?波等待?間 */
					wave++;
				}
			}
			MSMoveTimer = clock();
		}
		
		/* 計算遊戲?間 */
		if((float)(clock()-Timer) > CLOCKS_PER_SEC) {
			if(nextTime != 0)
				nextTime--;
			Timer = clock();
		}
		
		Attack(First_m,First_t);
		First_m = ColorAndDead(First_m);
		
		showMap(map_x,map_y); /* 顯?地圖 */
		showMoster(First_m,map_x,map_y);
		showTower(First_t,map_x,map_y);	
		EndPoint(map_x,map_y,0); /* 顯?終點 */
		
		/* 檢驗第?隻是?到達終點( 要?檢查看看 First_m 是?為 NULL ) */
		if( First_m != NULL )
			if( First_m->x > 46 ) {
				First_m = deleteMoster(First_m,1);
				HP-=10;
				EndPoint(map_x,map_y,1);
			}
		
		/* 顯?選擇用游標 */
		for(i=0;i<2;i++) {
			for(j=0;j<2;j++) {
				putASCII2(j*8 + CI.x*2 + OFFSET_X, i*4 + CI.y + OFFSET_Y,
				CI.shape[i][j*2],CI.color, 0);  
				putASCII2(j*8+1 + CI.x*2 + OFFSET_X, i*4 + CI.y + OFFSET_Y,
				CI.shape[i][j*2+1],CI.color, 0);
			}
		}
		if(showCre == 1) {  /* 顯?虛擬 Tower 圖? */
			tower_shape(4,TS);
			for (i=0; i<3; i++)
      		for (j=0; j<3; j++)
					if(CI.x+1+j < 35 && CI.y+1+i < 20 && CI.x+1+j >= 0 && CI.y+1+i >= 0) {
						putASCII2((CI.x+1+j)*2 + OFFSET_X,CI.y+1 + i + OFFSET_Y,
						TS[i][j*2],7,0);
						putASCII2((CI.x+1+j)*2+1 + OFFSET_X,CI.y+1 + i + OFFSET_Y,
						TS[i][j*2+1],7,0);
					}
		} 
		
		/* 資訊顯?區 */
		
		sprintf(toString," Main Tower HP:%3d ",HP);  /* ?量 */
		putString(0 + OFFSET_X,0 + OFFSET_Y,toString,12,7);
		
		sprintf(toString," 埤萄:%5d ",money);  /* ?錢 */
		putString(0 + OFFSET_X,HEIGHT-1 + OFFSET_Y,toString,12,7);
		
		sprintf(toString,"Score:%5d ",score);  /* 分數 */
		putString(WIDTH*2-12 + OFFSET_X,HEIGHT-1 + OFFSET_Y,toString,12,7);
		
		if(nextTime != 0) {
			sprintf(toString," %d蝶纔檜雖 衛濛..%2d ",wave,nextTime);  /* 分數 */
			putString(20 + OFFSET_X,HEIGHT-1 + OFFSET_Y,toString,12,11);
		}
		
		if(noMoney == 1) {  /* 錢不夠?提醒?下 */
			putString(22+OFFSET_X,8+OFFSET_Y,"                    ",14,5);
			putString(22+OFFSET_X,9+OFFSET_Y," 埤萄陛 睡褶м棲棻. ",14,5);
			putString(22+OFFSET_X,10+OFFSET_Y,"                    ",14,5);
		}
		
		if(showTM == 1) {
			showTowerMenu(&TM);
		}
		
		/* 主要畫?顯?區 Ending */
		
		/* 選單區 */
		} else if(Exit == 1) { /* 初始化離?選單 */
			setExitMenu(&exit);
			Exit=2;
		} else if(Exit == 2) { /* 顯?離?選單 */
			showExitMenu(&exit);
			switch (vk)	{
				case VK_UP:
					if(exit.cursor != 0) {   /* 如果游標在第??，則不做任?事 */
	            	exit.state[exit.cursor]=0;
	            	exit.cursor--;
   	         	exit.state[exit.cursor]=1;
					}
					break;
				case VK_DOWN:
   	      	if(exit.cursor != exit.num_options-1) { /* 如果游標在最後??，則不做任?事 */ 
	            	exit.state[exit.cursor]=0;
	            	exit.cursor++;
   	         	exit.state[exit.cursor]=1;
					}
					break;
				case VK_RETURN:
					switch (exit.cursor) {     /* 在不同?置按下 ENTER 有不同反應 */ 
						case 0:
							clearScreen();
							Exit=0; break;
						case 1:
							return 0;
						case 2:
							return 1; /* 若選擇 exit 則直接 return 1 離?遊戲 */
					} /* switch (m.cursor) Ending */
			} /* switch (vk) Ending */
		} /* if(Exit == 2) Ending */
		
		/* 選單區?Ending */
		
		drawCmdWindow();
		
		if( HP <= 0)
			IsEnding = 1;
	}  /* while(IsEnding != 1) 結尾 */
	
	
	putString(24+OFFSET_X,8+OFFSET_Y,"========================",11,5);
	putString(24+OFFSET_X,9+OFFSET_Y,"       Game Over        ",11,5);
	putString(24+OFFSET_X,10+OFFSET_Y,"========================",11,5);
	
	/* 當使用者再次按下 Enter 鍵??真?跳出 */
	while(1) {
		if(waitForKeyDown(1)) {
			vk=getKeyEventVirtual();
			if(vk == VK_RETURN)
				break;
		}
		if(i==0)
			putString(24+OFFSET_X,12+OFFSET_Y,"                  ",0,5);
		else
			putString(24+OFFSET_X,12+OFFSET_Y," Enter...",11,5);
		drawCmdWindow();
		i= !i;
	}
	
	saveScore(score);
	
	return 0;
}

/******************* 地圖讀入與顯? *******************/
/************ 讀入地圖 ************/
void setMap(void)
{
	FILE *in;
	int i,j;
	
	in = fopen("map.txt","r");
	/* 將 ImageMenu.txt 裡的字元存進?列之中 */
	for(i=0;i<MAP_HEIGHT;i++)
		for(j=0;j<MAP_WIDTH*2;j++)
			fscanf(in,"%x,",&map[i][j]);

	fclose(in);
}

/************ 顯?地圖 ************/

void showMap(int x,int y)
{
	int i,j;
	
	for(i=0;i<HEIGHT;i++) {
		for(j=0;j<WIDTH;j++) {
			putASCII2(j*2 + OFFSET_X, i + OFFSET_Y, map[i+y][(j+x)*2],7, 0);  
			putASCII2(j*2+1 + OFFSET_X, i + OFFSET_Y, map[i+y][(j+x)*2+1],7, 0);
		}
	}
}

/******************* 選單?定與顯? *******************/

/************ 初始化結?選單 ************/
void setExitMenu(Menu *m)
{	
	int i;
	
	/* m(選單)的初始化?定 */
	m->x = 24;
   m->y = 7;
   m->fgcolor = 12;
   m->bgcolor = 7;
   m->num_options = 3;  
   m->cursor = 0;
   for (i=0; i < (m->num_options); i++) {
      m->state[i] = 0;
   }
   m->state[0] =  1;
   
   strcpy(m->text[0], " Continue");
   strcpy(m->text[1], " MainMenu");
   strcpy(m->text[2], " Exit");
   strcpy(m->alt_text[0], "->Continue");
   strcpy(m->alt_text[1], "->MainMenu");
   strcpy(m->alt_text[2], "->Exit");
}

/************ 顯?結?選單 ************/
void showExitMenu(Menu *m)
{
	int i,j;
	
	/* 顯?選單??部分?*/
   for (i = 0; i < MF_HEIGHT; i++) {
   	for (j = 0; j < MF_WIDTH; j++) {
      	if (MenuFrame[i][j] == 1) {
         	putASCII2(j*2 + m->x + OFFSET_X-4, i + m->y + OFFSET_Y-2, 0xA1, 12, 0);  
				putASCII2(j*2+1 + m->x + OFFSET_X-4, i + m->y + OFFSET_Y-2, 0xBD, 12, 0);
			} else {
				putASCII2(j*2 + m->x + OFFSET_X-4, i + m->y + OFFSET_Y-2, ' ', 0, 7);
				putASCII2(j*2+1 + m->x + OFFSET_X-4, i + m->y + OFFSET_Y-2, ' ', 0, 7);
			}
		}
	}
	
	/* 顯?選單部分 */
	for(i=0;i<(m->num_options);i++) {
		if(m->state[i] == 0)
			putString(m->x + OFFSET_X,m->y + i + OFFSET_Y,m->text[i],
			m->fgcolor,m->bgcolor);
		else
			putString(m->x + OFFSET_X,m->y + i + OFFSET_Y,m->alt_text[i],
			m->bgcolor,m->fgcolor);
	} 
}

/******************* 怪物產生與顯? *******************/

/************ 產生怪物 ************/
Moster* genMoster(Moster *Fm,int mos_num,int op_x,int op_y,int level)
{
	int i,k;
	int *p;  /* 用?標來記?形狀?列的?置 */
	Moster *newPtr,*curPtr,*prePtr;
	
	newPtr = (Moster*)malloc(sizeof(Moster));  /* ?空出?? Moster型別 的記憶體?置 */
	if (newPtr != NULL) { 
		
		newPtr->x = op_x;
		newPtr->y = op_y;
		newPtr->color = 10;
		newPtr->state = 0;
		newPtr->kind = mos_num;
		newPtr->direct = 1; /* 0:向上 1:向右 2:向下 */
		newPtr->hp = 100;
		newPtr->beAtk = 0;
		if(mos_num == 3)
			newPtr->deValue = level*2 + 2;
		else
			newPtr->deValue = level*2;
		newPtr->value = level*2 + mos_num - 2;  /* 怪物價?為 level*2 ?上種類減二 */
		newPtr->nextPtr = NULL;
		moster_shape(newPtr->kind,newPtr->state,newPtr->shape); /* 取得形狀 */
		
		/* 如果沒有怪物，就直接把 Fm ?向新怪物 */
		if(Fm == NULL) {
			Fm = newPtr;
			return Fm;
		} else {
			prePtr = NULL;
			curPtr = Fm;
		
			/* 尋找最後?隻怪物的?置 */
			while (curPtr != NULL) {
				prePtr = curPtr;
				curPtr = curPtr->nextPtr;   /* 抓出下???標，然後再檢查 */
			}
			prePtr->nextPtr = newPtr;
			
			return Fm;
		}
	}
	else {
		return NULL;
	}
}

/************ 顯?怪物 ************/
void showMoster(Moster *m,int map_x,int map_y)
{
	int i,j;
	int show_x,show_y;
	
	while(m != NULL) {
		show_x = m->x - map_x;  /* 怪物顯??的座標必須用怪物座標減去地圖顯?座標 */
		show_y = m->y - map_y;
		for (i=0; i<3; i++)
      	for (j=0; j<3; j++)
				if(show_x+j < WIDTH && show_y+i < HEIGHT && show_x+j >= 0 && show_y+i >= 0) {
					putASCII2((show_x+j)*2 + OFFSET_X,show_y + i + OFFSET_Y,
					m->shape[i][j*2],m->color,0);
					putASCII2((show_x+j)*2+1 + OFFSET_X,show_y + i + OFFSET_Y,
					m->shape[i][j*2+1],m->color,0);
					if(m->beAtk == 1 && i == 0 && j == 1) {
						putASCII2((show_x+1)*2 + OFFSET_X,show_y + OFFSET_Y,0xA1,12,0);
						putASCII2((show_x+1)*2+1 + OFFSET_X,show_y + OFFSET_Y,0xB9,12,0);
						m->beAtk = 0;
					}
				}
		m = m->nextPtr;
	} /* while(m != NULL) Ending */
}

/************ 移動怪物 ************/
void moveMoster(Moster *m)
{
	int *p;
	int i,k;
	int head_x[3]={0,1,0};
	int head_y[3]={-1,0,1};
	int next_x,next_y;
	
	while(m != NULL) {
		/* 用前方物體來判?下??方向 */
		next_x = m->x+1 + head_x[m->direct]*2;
		next_y = m->y+1 + head_y[m->direct]*2;
		if(map[next_y][next_x*2] == 0xa1 && m->direct == 1) { /* 向右遇到牆壁的話 */
			next_x = m->x+1 + head_x[0]*2; 
			next_y = m->y+1 + head_y[0]*2;
			if(map[next_y][next_x*2] != 0xa1) /* 看看左邊是?是牆壁 */ 
				m->direct--; /* 左轉 */ 
			next_x = m->x+1 + head_x[2]*2;
			next_y = m->y+1 + head_y[2]*2;
			if(map[next_y][next_x*2] != 0xa1) /* 看看右邊是?是牆壁 */ 
				m->direct++; /* 右轉 */ 
		} else if(map[next_y][next_x*2] == 0xa1 && m->direct == 0) { /* 向上遇到牆壁的話 */
			m->direct++; /* 左轉 */ 
		} else if(map[next_y][next_x*2] == 0xa1 && m->direct == 2) { /* 向下遇到牆壁的話 */
			m->direct--; /* 右轉 */ 
		}
		m->x = m->x + head_x[m->direct];
		m->y = m->y + head_y[m->direct];
	
		m->state = !(m->state);  /* 每移動?步就改變姿勢 */
		moster_shape(m->kind,m->state,m->shape); /* 取得形狀 */
		
		m = m->nextPtr; /* 取得下?隻的?置 */
	}
}


/************ 刪除怪物 ************/
Moster* deleteMoster(Moster *Fm,int num)
{
	int i;
	Moster *prePtr, *curPtr, *tempPtr;
	
	/* ?檢查有沒有資? */
	if(num == 1) {
		tempPtr = Fm;
		Fm = Fm->nextPtr;
		free( tempPtr );
	} else {
		/* 找出第num?怪物?置 */
		prePtr = NULL;
		curPtr = Fm;
		for(i=0;i<num-1;i++) {
			if(curPtr != NULL) {
				prePtr = curPtr;
				curPtr = prePtr->nextPtr;
			} else { 
				return Fm;
			} 
		}
		tempPtr = curPtr; /* 為了等?下釋放記憶體，要?記下?點的?置 */
		prePtr->nextPtr = curPtr->nextPtr;
		free( tempPtr );   /* 把要刪除的?點記憶體釋放掉 */ 
	}
	return Fm;
}

/******************* Tower 與 怪物 的互動函式*******************/

/************ ?擊 ************/
void Attack(Moster *m,Tower *t)
{
	Moster *curMP;
	int dis_x,dis_y;  /* 計算怪物與塔的距離差 */
	
	curMP = m;
	while( t != NULL ) {
		while( curMP != NULL ) {
			dis_x = curMP->x - t->x;
			dis_y = curMP->y - t->y;
			
			if((dis_x*dis_x) + (dis_y*dis_y) <= (t->atkRage*t->atkRage) && 
			(float)clock() - t->timer > CLOCKS_PER_SEC/quickMove ) {
				/* 利用商高定理檢驗敵人是?在範圍內 ， 以及距離上次?擊使?達到必要?間 */
				/* 如果在範圍內，塔就?始?擊 */
				
				if((t->atkValue - m->deValue) <= 0) {/* 確??擊是?小於怪物防禦 */
					if(curMP->hp < (4 - m->kind)*1 )
						curMP->hp = 0;
					else
						curMP->hp -= (4 - m->kind)*1;
				}
				else if(curMP->hp > (t->atkValue - m->deValue)) /* 確??量大於減少的? */
					curMP->hp -= t->atkValue - m->deValue;
				else
					curMP->hp = 0;
				
				/* ?些怪物與 Tower 狀態的?改 */
				curMP->beAtk = 1;
				t->atking = 1;
				t->timer = clock();
				
				break;
			} else {
				curMP = curMP->nextPtr;
			}
		}
		t = t->nextPtr; /* ?續檢查下?座塔 */
		curMP = m; /* ??怪物檢查?標 */
	}
}

/************ ?量顏色與怪物死?檢查 ************/
Moster* ColorAndDead(Moster *Fm)
{
	int num=1;
	Moster *m;
	
	m = Fm;
	while( m != NULL) {
		/* 檢查怪物?量以改變顏色 */
		if(m->hp <= 75 && m->hp > 50)
			m->color = 11;
		else if(m->hp <= 50 && m->hp > 25)
			m->color = 13;
		else if(m->hp <= 25 && m->hp > 0)
			m->color = 12;
		
		if(m->hp == 0) {
			Fm = deleteMoster(Fm,num);
			m = Fm;
			if (m == NULL)
				return;
			num--; /* ?為刪掉?隻怪物? num ?減少，這樣 num 的數量就不?確了 */
			money+=(10*m->value);
			score+=(5*m->value);
		}
		
		num++;
		m = m->nextPtr;
	}
	return Fm;
}

/************ 確?怪物都死? ************/
int CheckMSAllDead(Moster *m)
{
	int i=0;
	
	while(m != NULL) {
		m = m->nextPtr;
		i++;
	}
	if(i == 0)
		return 1;
	else
		return 0;
} 

/******************* Tower 產生與顯? *******************/

/************ 產生 Tower ************/
Tower* createTower(Tower *FT,int kind,int op_x,int op_y)
{
	Tower *newPtr, *prePtr, *curPtr;
	
	newPtr = (Tower*)malloc(sizeof(Tower));  /* ?空出?? Tower型別 的記憶體?置 */
	if (newPtr != NULL) {  /* 確?真的有「確實空出」?塊?用的記憶體 */
		/* 初始化新塔的資訊 */
		newPtr->x = op_x;
		newPtr->y = op_y;
		newPtr->kind = kind;
		newPtr->atkRage = 5;
		newPtr->atking = 0;
		newPtr->level = 1;
		newPtr->UPMoney = 100;
		newPtr->sMoney = 75;
		newPtr->speed = 1;
		newPtr->atkValue = 15;
		newPtr->timer = clock();
		newPtr->nextPtr = NULL;   /* ?標則?暫?不讓它?向任?東? */
		tower_shape(newPtr->kind,newPtr->shape); /* 存入形狀 */
		
		prePtr = NULL;
		curPtr = FT;     /* 把最初的?標 np 存進 curPtr */
		
		/* ?檢查看看目前的?標是不是空的 */
		while (curPtr != NULL) {
			prePtr = curPtr;
			curPtr = curPtr->nextPtr;   /* 抓出下???標，然後再檢查 */
		}
		
		if (prePtr == NULL) {
			newPtr->nextPtr = FT;
			FT = newPtr;
		} else {  
			prePtr->nextPtr = newPtr;
			newPtr->nextPtr = curPtr;
		}
		return FT;
	}
	else {
		return NULL;
	}
}

/************ 顯? Tower ************/
void showTower(Tower *FT,int map_x,int map_y)
{
	int i,j;
	int show_x,show_y;
	int ch1,ch2,color,LVcolor;
	
	while(FT != NULL) {
		show_x = FT->x - map_x;  /* 怪物顯??的座標必須用怪物座標減去地圖顯?座標 */
		show_y = FT->y - map_y;
		/* 不同等?顯?不同顏色的磚塊 */
		switch (FT->level) {
         case 1:
            LVcolor=8; break;
         case 2:
            LVcolor=7; break;
         case 3:
            LVcolor=9; break;
         case 4:
            LVcolor=10; break;
         case 5:
            LVcolor=11; break;
         case 6:
            LVcolor=12; break;
      }
		
		for (i=0; i<3; i++)
      	    for (j=0; j<3; j++) {
      		    switch(FT->shape[i][j]) {
					case 0:
						ch1=0xA1;  ch2=0xBD;  color=LVcolor;  break;
					case 1:
						ch1=0xA2;  ch2=0xA8;  color=LVcolor;  break;
					case 2:
						ch1=0xA2;  ch2=0xA9;  color=LVcolor;  break;
					case 3:
						ch1=0xA1;  ch2=0xB7;  color=14;  break;
					case 4:
						ch1=0xA1;  ch2=0xBD;  color=4;  break;
				}
				if(FT->atking == 1 && i == 1 && j == 1) {
					ch1=0xA1;  ch2=0xB9;  color=14;  FT->atking=0;
				}
				if(show_x+j < WIDTH && show_y+i < HEIGHT && show_x+j >= 0 && show_y+i >= 0) {
					putASCII2((show_x+j)*2 + OFFSET_X,show_y + i + OFFSET_Y,ch1,color,0);
					putASCII2((show_x+j)*2+1 + OFFSET_X,show_y + i + OFFSET_Y,ch2,color,0);
				}
			}
		FT = FT->nextPtr;
	} 
}

/************ 檢查即將建造的 Tower 是?於合法?置 ************/
int TowerCheck(int cu_x,int cu_y,Tower *FT)
{
	int i,j;
	
	/* 檢查即將建造的 Tower 是?與已存在的?? */	
	while( FT != NULL ) {
		if((cu_x >= FT->x) && (cu_x < FT->x+3) && (cu_y >= FT->y) && (cu_y < FT->y+3))
			return 0;  /* 檢查左上角的座標 */
		if((cu_x+2 >= FT->x) && (cu_x+2 < FT->x+3) && (cu_y >= FT->y) && (cu_y < FT->y+3))
			return 0;  /* 檢查右上角的座標 */
		if((cu_x >= FT->x) && (cu_x < FT->x+3) && (cu_y+2 >= FT->y) && (cu_y+2 < FT->y+3))
			return 0;  /* 檢查左下角的座標 */
		if((cu_x+2 >= FT->x) && (cu_x+2 < FT->x+3) && (cu_y+2 >= FT->y) && (cu_y+2 < FT->y+3))
			return 0;  /* 檢查右下角的座標 */
		FT = FT->nextPtr;
	}
	/* 檢查即將建造的 Tower 地點是?為空白 */
	for(i=0;i<3;i++)
		for(j=0;j<6;j++)
			if(map[i+cu_y][j+cu_x*2] != ' ')
				return 0;
	
	return 1;
}

/************ 初始化 Tower 資訊選單 ************/
int setTowerMenu(Tower *FT,Menu *m,int x,int y)
{
	int i,j;
	
	/* m(選單)的初始化?定 */
	m->x = WIDTH*2 - 20;
   m->y = 0;
   m->fgcolor = 15;
   m->bgcolor = 0;
   m->num_options = 3;  
   m->cursor = 2;   
   for (i=0; i < (m->num_options); i++) {
      m->state[i] = 0;
   }
   m->state[m->cursor] =  1;
   
   strcpy(m->text[0], "  Upgrade");
   strcpy(m->text[1], "  Sale");
   strcpy(m->text[2], "  Close");
   strcpy(m->alt_text[0], "-> Upgrade");
   strcpy(m->alt_text[1], "-> Sale");
   strcpy(m->alt_text[2], "-> Close");
   
   while(FT != NULL) {
		if(FT->x == x && FT->y == y)
			break;
		FT = FT->nextPtr;
	}
	
	if(FT != NULL) {
		if(FT->kind == 1)
			sprintf(m->infom[0],"Kind: �香嚃蛾�");
		else if(FT->kind == 1)
			sprintf(m->infom[0],"Kind: 渠ん顫錶");
		else if(FT->kind == 1)
			sprintf(m->infom[0],"Kind: 嘐餌橾顫錶");
			
		if(FT->level < 6) {
			sprintf(m->infom[1],"ATK:  %2d -> %2d",FT->atkValue,FT->atkValue+2);
			sprintf(m->infom[2],"Rage: %2d -> %2d",FT->atkRage,FT->atkRage+1);
			sprintf(m->infom[3],"Speed:%2d -> %2d",FT->speed,FT->speed);
			sprintf(m->infom[5],"UpGrade $ %4d",FT->UPMoney);
		} else {
			sprintf(m->infom[1],"ATK:  %2d -> X",FT->atkValue);
			sprintf(m->infom[2],"Rage: %2d -> X",FT->atkRage);
			sprintf(m->infom[3],"Speed:%2d -> X",FT->speed);
			sprintf(m->infom[5],"UpGrade $ X");
		}
		sprintf(m->infom[4],"LV:   %2d",FT->level);
		sprintf(m->infom[6],"Sale $ %4d",FT->sMoney);
		
		
		return 1;
	}
	return 0;
}

/************ 顯? Tower 資訊選單 ************/
void showTowerMenu(Menu *m)
{
	int i,j;
	
	for(i=0;i<14;i++)
		for(j=0;j<20;j++) {
			if(j == 0)
				putASCII2(m->x + j + OFFSET_X,m->y + i + OFFSET_Y,'|',11,0);
			else if(i == 13)
				putASCII2(m->x + j + OFFSET_X,m->y + i + OFFSET_Y,'-',11,0);
			else
				putASCII2(m->x + j + OFFSET_X,m->y + i + OFFSET_Y,' ',11,0);
		}
	
	/* 顯?資訊部分 */
	for(i=0;i<7;i++) {
			putString(m->x + 4 + OFFSET_X,m->y + i + 1 + OFFSET_Y,m->infom[i],
			m->fgcolor,m->bgcolor);
	} 
	
	/* 顯?選單部分 */
	for(i=0;i<(m->num_options);i++) {
		if(m->state[i] == 0)
			putString(m->x + 4 + OFFSET_X,m->y + i + 10 + OFFSET_Y,
			m->text[i],m->fgcolor,m->bgcolor);
		else
			putString(m->x + 4 + OFFSET_X,m->y + i + 10 + OFFSET_Y,
			m->alt_text[i],m->bgcolor,m->fgcolor);
	} 
}

/************ Tower 升? ************/
int UpGrade(Tower *FT,int x,int y) {
	while(FT != NULL) {
		if(FT->x == x && FT->y == y)
			/* 錢足夠且等?小於等於六??可升? */
			if( FT->level == 6 )
				return 2;
			else if( money >= FT->UPMoney ) { 
				money -= FT->UPMoney;
				FT->atkValue+=2;
				FT->atkRage++;
				FT->UPMoney*=2;
				FT->level++;
				FT->sMoney = FT->UPMoney/2 + 75;
				return 1;
			} else
				return 0;
		FT = FT->nextPtr;
	}
	return 1;
}

/************ 刪除 Tower ************/
Tower* deleteTower(Tower *FT,int x,int y)
{
	int i;
	Tower *prePtr, *curPtr, *tempPtr;
	
	prePtr = FT;
	curPtr = FT->nextPtr;
	
	if(FT->x == x && FT->y == y) {
		money+= FT->sMoney;  /* 刪除塔的??賣錢 */
		tempPtr = FT;
		FT = FT->nextPtr;
		free( tempPtr );
	} else {
		while(curPtr != NULL) {
			if(curPtr->x == x && curPtr->y == y) {
				money+= curPtr->sMoney;  /* 刪除塔的??賣錢 */ 
				tempPtr = curPtr; /* 為了等?下釋放記憶體，要?記下?點的?置 */
				prePtr->nextPtr = curPtr->nextPtr;
				free( tempPtr );   /* 把要刪除的?點記憶體釋放掉 */
				return FT;
			}
			prePtr = curPtr;
			curPtr = curPtr->nextPtr;
		}
	}
	return FT;
} 

/******************* 其他與地圖相關 *******************/
/************ 顯?終點 ************/
void EndPoint(int map_x,int map_y,int state)
{
	int i,j;
	int color;
	int show_x,show_y;
	int End[3][6] = {
		0x20, 0x20, 0xa1, 0xbb, 0x20, 0x20,
		0xa1, 0xf6, 0xa1, 0xb8, 0xa1, 0xf7, 
		0xa2, 0xa8, 0xa1, 0xb5, 0xa2, 0xa9   };
	
	if(EndPointTimer == 14) /* 每換兩次(7秒?次)顏色該?歸零 */
		EndPointTimer=0;
	
	if(state == 0)
		color = EndPointTimer/7 + 10;
	else
		color = 12;
	
	show_x = END_POINT_X - map_x;  /* 出生點顯??的座標必須用怪物座標減去地圖顯?座標 */
	show_y = END_POINT_Y - map_y;
	
	for (i=0; i<3; i++)
      for (j=0; j<3; j++)
			if(show_x+j < 35 && show_y+i < 20 && show_x+j >= 0 && show_y+i >= 0) {
				putASCII2((show_x+j)*2 + OFFSET_X,show_y + i + OFFSET_Y,
				End[i][j*2],color,0);
				putASCII2((show_x+j)*2+1 + OFFSET_X,show_y + i + OFFSET_Y,
				End[i][j*2+1],color,0);
			}
	EndPointTimer++;
}
