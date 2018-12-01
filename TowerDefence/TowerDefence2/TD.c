#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "TD.h"
#include "kb_input2.h"
#include "console_draw2.h"
#pragma warning(disable: 4996) 

int frame[HEIGHT+2][WIDTH+2] = {0};   /* ?? */
char plate[HEIGHT][WIDTH];  /* �e? */

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

/* �禡�ŧi�� */

/* �����禡�w�ŧi�� TD.h */

void setMainMenu(Menu *m);
void showMainMenu(Menu *m);
void Ending(void);

void genJune(June *j);
void showJune(June *j);
void moveJune(June *j);

/* �禡�ŧi�� Ending */

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
	
	/* �s????*/
   for (i = 0; i < HEIGHT+2; i++) {
      frame[i][0] = 1;
      frame[i][WIDTH+2-1] = 1;
   }
   for (j = 0; j < WIDTH+2; j++) {
      frame[0][j] = 1;
      frame[HEIGHT+2-1][j] = 1;
   }
   /* ��????*/
 
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
	
	t_start=clock();  /* ?�l�p? */
	
	/* �Ұ���L���� */ 
   initializeKeyInput();
	
	while(IsEnding != 1) {
		
		/* ���D����l�� */
		if(IsMain != 1) { 
			setMainMenu(&m);
			IsMain = 1;
			getImage(); /*  ?�lŪ�J�w?�s?��imageMenu.txt�� */
			genJune(&J[0]);
		}
		
		/* �p�Gʨ�H���U�ӡA�ӥB�٨S���ͶW�L 6 ?�H���ܡA�N���ͷs��ʨ�H */
		if(J[num].state == 2 && num < 5) {
			num++;
			genJune(&J[num]);
		}
		
		/* ��L������ */
		if(waitForKeyDown(0.1)) {
      	vk=getKeyEventVirtual();
      	
			switch (vk)	{
				case VK_UP:
					if(m.cursor != 0) {   /* �p�G��Цb��??�A�h������?�� */
	            	m.state[m.cursor]=0;
	            	m.cursor--;
   	         	m.state[m.cursor]=1;
					}
					break;
				case VK_DOWN:
   	      	if(m.cursor != m.num_options-1) { /* �p�G��Цb�̫�??�A�h������?�� */ 
	            	m.state[m.cursor]=0;
	            	m.cursor++;
   	         	m.state[m.cursor]=1;
					}
					break;
				case VK_RETURN:
					switch (m.cursor) {     /* �b���P?�m���U ENTER �����P���� */ 
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
			
		}/* if(waitForKeyDown(0.1)) ���� */
		
		showMainMenu(&m);
		showImage(m.cursor);
		
		/* ���F?�b���ʤ���ʨ�H�A����ʪ�ʨ�H?�n��? */
		for(i=0;i<=num;i++)
			showJune(&J[i]);
		
		if(IsMain == 1)
			drawCmdWindow();
		
		/* �έp?�ӱ���ʨ�H�����ʳt�� */
		if((float)(clock()-t_start) >= 0.3*CLOCKS_PER_SEC) {
			/* �p�Gʨ�H��F����?�m�N��??�򲾰� */
			if(J[num].x >= (18+num*3))
				moveJune(&J[num]);
			else
				J[num].state = 2; /* ��ʨ�H�����A?�� 2 �N��u����v */
			t_start = clock();
		}
	} /* while(IsEnding != 1) ���� */ 
	
	i=0;
	Ending();
	
	/* ��ϥΪ̦A�����U Enter ��??�u?���X */
	while(1) {
		if(waitForKeyDown(1)) {
			vk=getKeyEventVirtual();
			if(vk == VK_RETURN)
				break;
		}
		if(i==0)
			putString(24+OFFSET_X,18+OFFSET_Y,"                      ",12,0);
		else
			putString(24+OFFSET_X,18+OFFSET_Y,"���� ȭ������ Enter...",12,0);
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

/******************* �D���?�w�P��? *******************/

/************ ��l�ƥD��� ************/
void setMainMenu(Menu *m)
{
	/* �N�w?���n������?�Y�e? MainMenu.txt �s�J plate �� */
	FILE *in;
	char ch;
	int i,j;
	
	in = fopen("MainMenu.txt","r");
	for(i=0;i<HEIGHT;i++) {
		for(j=0;j<WIDTH;j++) {
			while((ch = fgetc(in)) == '\n' || ch == '|'); /* �ư�����?�r���P | �r�� */ 
			plate[i][j] = ch;
		}
	}
	fclose(in);
	
	/* m(���)����l��?�w */
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
   
   strcpy(m->text[0], "���ӽ���- GAME PLAY");
   strcpy(m->text[1], "�����ϱ�- TWO PLAYERS");
   strcpy(m->text[2], "���ӼҰ� INTRODRCTION");
   strcpy(m->text[3], "������ ABOUT MAKER");
   strcpy(m->text[4], "����ǥ RANKING LIST");
   strcpy(m->text[5], "������ EXIT");
   strcpy(m->alt_text[0], "->���ӽ���- GAME PLAY");
   strcpy(m->alt_text[1], "->�����ϱ�- TWO PLAYERS");
   strcpy(m->alt_text[2], "->���ӼҴ� INTRODRCTION");
   strcpy(m->alt_text[3], "->������ ABOUT MAKER");
   strcpy(m->alt_text[4], "->����ǥ RANKING LIST");
   strcpy(m->alt_text[5], "->������ EXIT");
}

/************ ��?�D��� ************/
void showMainMenu(Menu *m)
{
	int i,j;
	
	/* �̤��P�Ʀr��?���P�r���P���P�C�⪺��� */
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
	
	/* ��?��泡�� */
	for(i=0;i<(m->num_options);i++) {
		if(m->state[i] == 0)
			putString(m->x + OFFSET_X,m->y + i + OFFSET_Y,m->text[i],
			m->fgcolor,m->bgcolor);
		else
			putString(m->x + OFFSET_X,m->y + i + OFFSET_Y,m->alt_text[i],
			m->bgcolor,m->fgcolor);
	} 
}

/************ ��?�e? ************/

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
			while((ch = fgetc(in)) == '\n' || ch == '|'); /* �ư�����?�r���P | �r�� */ 
			endImage[i][j] = ch;
		}
	}
	fclose(in);
	
	while(1) {
		clearScreen(); /* ��e?�M�� */
		
		/* ��l��ʨ�H */
		if(times%3 == 0 && people<7) {
			J[people].x = 35;
			J[people].y = 0 + people*3;
			J[people].color = 15-people;
			J[people].state = 0;
			people++;
		}
		
		/* ��C?ʨ�H��?�X�Өò��� */
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

/******************* �Ǫ����ͻP��? *******************/

/************ ����?�lʨ�H ************/
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

/************ ��??�lʨ�H ************/
void showJune(June *j)
{
	int i,k;
	
	for (i=0; i<3; i++) {
      for (k=0; k<3; k++) {
			if((j->x+k < 35) && (j->x+k >= 0) && (j->y+i < 20))     /* ����ʨ�H�W�X?? */
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

/************ ����?�lʨ�H ************/
void moveJune(June *j)
{
	int i,k;
	
	(j->x)--;   /* ʨ�H�����䲾�� */ 
	j->state = !(j->state);  /* �C����?�B�N���ܫ��� */ 
	for (i=0; i<3; i++)
      for (k=0; k<3; k++)
         j->shape[i][k] = j_shape[j->state][i*3+k];
}
