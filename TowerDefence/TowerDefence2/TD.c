#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "TD.h"
#include "kb_input2.h"
#include "console_draw2.h"
#pragma warning(disable: 4996) 

int frame[HEIGHT+2][WIDTH+2] = {0};   /* ?? */
char plate[HEIGHT][WIDTH];  /* 畫? */

struct t_menu {
   int x;
   int y;
   int fgcolor;
   int bgcolor;
   int cursor;
   int num_options;
   int state[10];
   char text[10][40];
	char alt_text[10][40];
};
typedef struct t_menu Menu;

struct t_june {
	int x;
	int y;
	int color;
	int state;
	int shape[3][3];
};
typedef struct t_june June;

int j_shape[2][9] = {
	{0, 5, 0,
	 2, 4, 3,
	 0, 5, 0},
	{0, 5, 0,
	 2, 4, 3,
	 6, 0, 7}};

int num=0;

/* 函式宣告區 */

/* 部分函式已宣告於 TD.h */

void setMainMenu(Menu *m);
void showMainMenu(Menu *m);
void Ending(void);

void genJune(June *j);
void showJune(June *j);
void moveJune(June *j);

/* 函式宣告區 Ending */

int main(void)
{
	int i,j;
	int IsEnding = 0;
	int vk;
	int position=0;
	
	int IsMain=0;
	Menu m;
	June J[6];
	int j_stop=0;
	
	clock_t t_start;
	
	/* 製????*/
   for (i = 0; i < HEIGHT+2; i++) {
      frame[i][0] = 1;
      frame[i][WIDTH+2-1] = 1;
   }
   for (j = 0; j < WIDTH+2; j++) {
      frame[0][j] = 1;
      frame[HEIGHT+2-1][j] = 1;
   }
   /* 顯????*/
 
   for (i = 0; i < HEIGHT+2; i++) {
   	for (j = 0; j < WIDTH+2; j++) {
      	if (frame[i][j] == 1) {
         	putASCII2(j*2 + OFFSET_X - 2, i + OFFSET_Y-1, 0xA1, 7, 0);  
				putASCII2(j*2+1 + OFFSET_X - 2, i + OFFSET_Y-1, 0xBD, 7, 0);
			} else {
				putASCII2(j*2 + OFFSET_X - 2, i + OFFSET_Y-1, ' ', 0, 0);
				putASCII2(j*2+1 + OFFSET_X - 2, i + OFFSET_Y-1, ' ', 0, 0);
			}
		}
	}
	drawCmdWindow();
	
	t_start=clock();  /* ?始計? */
	
	/* 啟動鍵盤控制 */ 
   initializeKeyInput();
	
	while(IsEnding != 1) {
		
		/* 讓主選單初始化 */
		if(IsMain != 1) { 
			setMainMenu(&m);
			IsMain = 1;
			getImage(); /*  ?始讀入預?製?的imageMenu.txt檔 */
			genJune(&J[0]);
		}
		
		/* 如果囧人停下來，而且還沒產生超過 6 ?人的話，就產生新的囧人 */
		if(J[num].state == 2 && num < 5) {
			num++;
			genJune(&J[num]);
		}
		
		/* 鍵盤控制選單 */
		if(waitForKeyDown(0.1)) {
      	vk=getKeyEventVirtual();
      	
			switch (vk)	{
				case VK_UP:
					if(m.cursor != 0) {   /* 如果游標在第??，則不做任?事 */
	            	m.state[m.cursor]=0;
	            	m.cursor--;
   	         	m.state[m.cursor]=1;
					}
					break;
				case VK_DOWN:
   	      	if(m.cursor != m.num_options-1) { /* 如果游標在最後??，則不做任?事 */ 
	            	m.state[m.cursor]=0;
	            	m.cursor++;
   	         	m.state[m.cursor]=1;
					}
					break;
				case VK_RETURN:
					switch (m.cursor) {     /* 在不同?置按下 ENTER 有不同反應 */ 
						case 0:
							if(sin_game() == 1)
								IsEnding = 1;
							num = 0;
							for(i=0;i<num;i++)
								J[i].state = 0;
							IsMain=0;
							break;
						case 2:
							intro();
							num = 0;
							for(i=0;i<num;i++)
								J[i].state = 0;
							IsMain=0;
							break;
						case 3:
							writer();
							num = 0;
							for(i=0;i<num;i++)
								J[i].state = 0;
							IsMain=0;
							break;
						case 4:
							rankList();
							num = 0;
							for(i=0;i<num;i++)
								J[i].state = 0;
							IsMain=0;
							break;
						case 5:
							IsEnding = 1; break;
					}
			}
			
		}/* if(waitForKeyDown(0.1)) 結尾 */
		
		showMainMenu(&m);
		showImage(m.cursor);
		
		/* 除了?在移動中的囧人，停止不動的囧人?要顯? */
		for(i=0;i<=num;i++)
			showJune(&J[i]);
		
		if(IsMain == 1)
			drawCmdWindow();
		
		/* 用計?來控制囧人的移動速度 */
		if((float)(clock()-t_start) >= 0.3*CLOCKS_PER_SEC) {
			/* 如果囧人到了停止?置就不??續移動 */
			if(J[num].x >= (18+num*3))
				moveJune(&J[num]);
			else
				J[num].state = 2; /* 把囧人的狀態?為 2 代表「停止」 */
			t_start = clock();
		}
	} /* while(IsEnding != 1) 結尾 */ 
	
	i=0;
	Ending();
	
	/* 當使用者再次按下 Enter 鍵??真?跳出 */
	while(1) {
		if(waitForKeyDown(1)) {
			vk=getKeyEventVirtual();
			if(vk == VK_RETURN)
				break;
		}
		if(i==0)
			putString(24+OFFSET_X,18+OFFSET_Y,"                      ",12,0);
		else
			putString(24+OFFSET_X,18+OFFSET_Y,"檜瞪 �飛橉虞� Enter...",12,0);
		drawCmdWindow();
		i= !i;
	}
	
	return 0;
}

void delay(float sec)
{
   clock_t startc;    
   startc = clock();
	while((float)(clock()-startc)/CLOCKS_PER_SEC <= sec);
} 

void putString(int x, int y, char *p, int fg_color, int bg_color)
{
   int i;
   for(i=0; i<strlen(p); i++)
      putASCII2(x+i, y, p[i], fg_color, bg_color);
}

void clearScreen(void)
{
	int i,j;
	
	for(i=0;i<HEIGHT;i++) {
		for(j=0;j<WIDTH;j++) {
			putASCII2(j*2 + OFFSET_X,i + OFFSET_Y,' ',0,0);
			putASCII2(j*2 +1 + OFFSET_X,i + OFFSET_Y,' ',0,0);
		}
	}
}

/******************* 主選單?定與顯? *******************/

/************ 初始化主選單 ************/
void setMainMenu(Menu *m)
{
	/* 將預?做好的部分?頭畫? MainMenu.txt 存入 plate 中 */
	FILE *in;
	char ch;
	int i,j;
	
	in = fopen("MainMenu.txt","r");
	for(i=0;i<HEIGHT;i++) {
		for(j=0;j<WIDTH;j++) {
			while((ch = fgetc(in)) == '\n' || ch == '|'); /* 排除掉換?字元與 | 字元 */ 
			plate[i][j] = ch;
		}
	}
	fclose(in);
	
	/* m(選單)的初始化?定 */
	m->x = 3;
   m->y = 12;
   m->fgcolor = 10;
   m->bgcolor = 0;
   m->num_options = 6;  
   m->cursor = 0;
   for (i=0; i < (m->num_options); i++) {
      m->state[i] = 0;
   }
   m->state[0] =  1;
   
   strcpy(m->text[0], "啪歜衛濛- GAME PLAY");
   strcpy(m->text[1], "萃檜ж晦- TWO PLAYERS");
   strcpy(m->text[2], "啪歜模偃 INTRODRCTION");
   strcpy(m->text[3], "虜菔檜 ABOUT MAKER");
   strcpy(m->text[4], "牖嬪ル RANKING LIST");
   strcpy(m->text[5], "釭陛晦 EXIT");
   strcpy(m->alt_text[0], "->啪歜衛濛- GAME PLAY");
   strcpy(m->alt_text[1], "->萃檜ж晦- TWO PLAYERS");
   strcpy(m->alt_text[2], "->啪歜模渠 INTRODRCTION");
   strcpy(m->alt_text[3], "->虜菔檜 ABOUT MAKER");
   strcpy(m->alt_text[4], "->牖嬪ル RANKING LIST");
   strcpy(m->alt_text[5], "->釭陛晦 EXIT");
}

/************ 顯?主選單 ************/
void showMainMenu(Menu *m)
{
	int i,j;
	
	/* 依不同數字顯?不同字元與不同顏色的方塊 */
	for (i = 0; i < HEIGHT; i++) {
   	for (j = 0; j < WIDTH; j++) {
			if(plate[i][j] == '1') {
				putASCII2(j*2 + OFFSET_X, i + OFFSET_Y,0xA1, 12, 0);
				putASCII2(j*2+1 + OFFSET_X, i + OFFSET_Y,0xBD, 12, 0);
			} else if(plate[i][j] == '2') {
				putASCII2(j*2 + OFFSET_X, i + OFFSET_Y,0xA1, 14, 0);
				putASCII2(j*2+1 + OFFSET_X, i + OFFSET_Y,0xBD, 14, 0);
			} else if(plate[i][j] == '3') {
				putASCII2(j*2 + OFFSET_X, i + OFFSET_Y,0xF9, 15, 0);
				putASCII2(j*2+1 + OFFSET_X, i + OFFSET_Y,0xF9, 15, 0);
			} else if(plate[i][j] == '4') {
				putASCII2(j*2 + OFFSET_X, i + OFFSET_Y,0xF9, 15, 0);
				putASCII2(j*2+1 + OFFSET_X, i + OFFSET_Y,0xDE, 15, 0);
			} else if(plate[i][j] == '5') {
				putASCII2(j*2 + OFFSET_X, i + OFFSET_Y,0xF9, 15, 0);
				putASCII2(j*2+1 + OFFSET_X, i + OFFSET_Y,0xF8, 15, 0);
			} else if(plate[i][j] == '6') {
				putASCII2(j*2 + OFFSET_X, i + OFFSET_Y,0xA1, 15, 0);
				putASCII2(j*2+1 + OFFSET_X, i + OFFSET_Y,0xD7, 15, 0);
			} else {
				putASCII2(j*2 + OFFSET_X, i + OFFSET_Y,' ', 0, 0);
				putASCII2(j*2+1 + OFFSET_X, i + OFFSET_Y,' ', 0, 0);
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

/************ 結?畫? ************/

void Ending(void)
{	
	int i,j;
	int times=0;
	int people=0;
	int state=0;
	int color=14;
	June J[10];
	char endImage[HEIGHT][WIDTH];
	
	FILE *in;
	char ch;
	
	in = fopen("Ending.txt","r");
	for(i=0;i<HEIGHT;i++) {
		for(j=0;j<WIDTH;j++) {
			while((ch = fgetc(in)) == '\n' || ch == '|'); /* 排除掉換?字元與 | 字元 */ 
			endImage[i][j] = ch;
		}
	}
	fclose(in);
	
	while(1) {
		clearScreen(); /* 把畫?清空 */
		
		/* 初始化囧人 */
		if(times%3 == 0 && people<7) {
			J[people].x = 35;
			J[people].y = 0 + people*3;
			J[people].color = 15-people;
			J[people].state = 0;
			people++;
		}
		
		/* 把每?囧人顯?出來並移動 */
		for(i=0;i<=people;i++) {
	      moveJune(&J[i]);   	
   	   showJune(&J[i]);
		}
		
		for(i=0;i<HEIGHT;i++)
			for(j=WIDTH-1;j>=WIDTH-1-(times-3)+(i/3)*3;j--) {
				if(j>=0) {
					color=0;
					if(endImage[i][j] == '1')
						color=4;
					else if(endImage[i][j] == '2')
						color=8;
					else if(endImage[i][j] == '3')
						color=1;
					else if(endImage[i][j] == '4')
						color=14;
					else if(endImage[i][j] == '5')
						color=12;
					else if(endImage[i][j] == '6')
						color=2;
					else if(endImage[i][j] == '7')
						color=9;
					putASCII2(j*2 + OFFSET_X, i + OFFSET_Y,0xA1, color, 0);
					putASCII2(j*2+1 + OFFSET_X, i + OFFSET_Y,0xBD, color, 0);
				}
			}
		
		drawCmdWindow();
      
      times++;
      
      if(times > 55)
      	break;
      
		delay(0.1);
	} /* while(1) Ending */
	
}

/******************* 怪物產生與顯? *******************/

/************ 產生?始囧人 ************/
void genJune(June *j)
{
	int i,k;
	
	j->x = 35;
	j->y = 3;
	j->color = num + 9;
	j->state = 0;
	
	for (i=0; i<3; i++)
      for (k=0; k<3; k++)
         j->shape[i][k] = j_shape[0][i*3+k];
}

/************ 顯??始囧人 ************/
void showJune(June *j)
{
	int i,k;
	
	for (i=0; i<3; i++) {
      for (k=0; k<3; k++) {
			if((j->x+k < 35) && (j->x+k >= 0) && (j->y+i < 20))     /* 防止囧人超出?? */
	         switch (j->shape[i][k]) {
   	         case 1:
      	         putASCII2((j->x+ k)*2 + OFFSET_X, 
         	      j->y + i + OFFSET_Y, 0xCA, j->color, 0);  
            	   putASCII2((j->x+ k)*2+1 + OFFSET_X, 
               	j->y + i + OFFSET_Y, 0xA8, j->color, 0);             
	               break;
   	         case 2:
      	         putASCII2((j->x+ k)*2 + OFFSET_X, 
         	      j->y + i + OFFSET_Y, 0xA1, j->color, 0);  
            	   putASCII2((j->x+ k)*2+1 + OFFSET_X, 
               	j->y + i + OFFSET_Y, 0xF8, j->color, 0);             
               	break;
	            case 3:
   	            putASCII2((j->x+ k)*2 + OFFSET_X, 
      	         j->y + i + OFFSET_Y, 0xA1, j->color, 0);  
         	      putASCII2((j->x+ k)*2+1 + OFFSET_X, 
            	   j->y + i + OFFSET_Y, 0xF2, j->color, 0);             
               	break;
	            case 4:
   	            putASCII2((j->x+ k)*2 + OFFSET_X, 
      	         j->y + i + OFFSET_Y, 0xA1, j->color, 0);  
         	      putASCII2((j->x+ k)*2+1 + OFFSET_X, 
            	   j->y + i + OFFSET_Y, 0xBD, j->color, 0);             
	               	break;
   	         case 5:
      	         putASCII2((j->x+ k)*2 + OFFSET_X, 
         	      j->y + i + OFFSET_Y, 0xA1, j->color, 0);  
            	   putASCII2((j->x+ k)*2+1 + OFFSET_X, 
               	j->y + i + OFFSET_Y, 0xFC, j->color, 0);             
	               break;
   	         case 6:
      	         putASCII2((j->x+ k)*2 + OFFSET_X, 
         	      j->y + i + OFFSET_Y, 0xA2, j->color, 0);  
            	   putASCII2((j->x+ k)*2+1 + OFFSET_X, 
               	j->y + i + OFFSET_Y, 0x7E, j->color, 0);             
	               break;
   	         case 7:
      	         putASCII2((j->x+ k)*2 + OFFSET_X, 
         	      j->y + i + OFFSET_Y, 0xA2, j->color, 0);  
            	   putASCII2((j->x+ k)*2+1 + OFFSET_X, 
               	j->y + i + OFFSET_Y, 0xA1, j->color, 0);             
	               break;
				} /* switch() Ending */
		} /* for(k=0....) Ending */ 
	}
}

/************ 移動?始囧人 ************/
void moveJune(June *j)
{
	int i,k;
	
	(j->x)--;   /* 囧人往左邊移動 */ 
	j->state = !(j->state);  /* 每移動?步就改變姿勢 */ 
	for (i=0; i<3; i++)
      for (k=0; k<3; k++)
         j->shape[i][k] = j_shape[j->state][i*3+k];
}
