#include <stdio.h>

#include "console_draw2.h"
#pragma warning(disable: 4996) 
#define IM_WIDTH     21
#define IM_HEIGHT    9
#define IM_OFFSET_X  33
#define IM_OFFSET_Y  14

char image[10][IM_HEIGHT][IM_WIDTH];

/* 函式宣告區 */

extern void getImage(void);
extern void showImage(int num);

extern void moster_shape(int num,int state,int *shape);
extern void tower_shape(int num,int *shpae);

/* 函式宣告區 Ending*/

/* 圖片?列區 */

int june_shape[2][18] = {
	{ 	0x20, 0x20, 0xa1, 0xfc, 0x20, 0x20,
		0xa1, 0xf8, 0xa1, 0xbd, 0xa1, 0xf2, 
		0x20, 0x20, 0xa1, 0xfc, 0x20, 0x20},
	{ 	0x20, 0x20, 0xa1, 0xfc, 0x20, 0x20,
		0xa1, 0xf8, 0xa1, 0xbd, 0xa1, 0xf2, 
		0xa2, 0x7e, 0x20, 0x20, 0xa2, 0xa1}};
		
int two_shape[2][18] = {
	{ 	0xa5, 0x59, 0xa1, 0xfc, 0xa5, 0x59,
		0xa2, 0x7c, 0xa1, 0xbd, 0xa2, 0x7d, 
		0xa2, 0x49, 0xa2, 0x72, 0xa2, 0x49  },
	{ 	0xa5, 0x59, 0xa1, 0xfc, 0xa5, 0x59,
		0xa2, 0x7c, 0xa1, 0xbd, 0xa2, 0x7d, 
		0xa2, 0x7a, 0xa2, 0x72, 0xa2, 0x7b }};
		
int mon_shape[2][18] = {
	{ 	0xa1, 0xb6, 0xa1, 0xfc, 0xa1, 0xb6,
		0xa1, 0xf2, 0xa1, 0xbd, 0xa1, 0xf2, 
		0xf9, 0xe9, 0xa1, 0xbf, 0xf9, 0xeb, },
	{ 	0xa1, 0xb6, 0xa1, 0xfc, 0xa1, 0xb6,
		0xa1, 0xf2, 0xa1, 0xbd, 0xa1, 0xf2, 
		0xa2, 0x7e, 0xa1, 0xbf, 0xa2, 0xa1  }};
		
		
int level01_shape[1][9] = {
		0,0,0,
		0,3,0,
		0,0,0};
		
int NoColorTS[1][18] = {
		0xa1, 0xbd, 0xa1, 0xbd, 0xa1, 0xbd, 
		0xa1, 0xbd, 0xa1, 0xb7, 0xa1, 0xbd, 
		0xa1, 0xbd, 0xa1, 0xbd, 0xa1, 0xbd };

/* 圖片?列區 Ending*/

/* 這裡?含了?些可以讀取讀取?部檔?的函式 */
/* 然後將這些?部檔?轉成圖片，並且顯?在螢幕上 */

void getImage(void)
{
	FILE *in;
	char ch;
	int num,i,j;
	
	in = fopen("ImageMenu.txt","r");
	/* 將 ImageMenu.txt 裡的字元存進?列之中 */
	for(num=0;num<6;num++) {
		for(i=0;i<IM_HEIGHT;i++) {
			for(j=0;j<IM_WIDTH;j++) {
				/* 排除掉 '\n'、'|'、'-' 字元 */
				while((ch = fgetc(in)) == '\n' || ch == '|' || ch == '-');  
				image[num][i][j] = ch;
			}
		}
	}
	fclose(in);
}

void showImage(int num)
{
	int i,j;
	int ch1,ch2,fg_color;
	
	/* ?據不同的字元顯?出對應的?形字元 */
	
	for(i=0;i<IM_HEIGHT;i++) {
		for(j=0;j<IM_WIDTH;j++) {
			if(image[num][i][j] == 'U') {
				ch1=0xA1;  ch2=0xB6;  fg_color=4;
			} else if(image[num][i][j] == 'H') {
				ch1=0xA1;  ch2=0xBD;  fg_color=4;
			} else if(image[num][i][j] == 'G') {
				ch1=0xA2;  ch2=0xAB;  fg_color=4;
			} else if(image[num][i][j] == '(') {
				ch1=0xA2;  ch2=0xA8;  fg_color=4;
			} else if(image[num][i][j] == ')') {
				ch1=0xA2;  ch2=0xA9;  fg_color=4;
			} else if(image[num][i][j] == 'E') {
				ch1=0xA1;  ch2=0xBD;  fg_color=5;
			} else if(image[num][i][j] == 'O') {
				ch1=0xA1;  ch2=0xFC4;  fg_color=14;
			} else if(image[num][i][j] == 'e') {
				ch1=0xA1;  ch2=0xB6;  fg_color=12;
			} else if(image[num][i][j] == 'v') {
				ch1=0xA1;  ch2=0xB6;  fg_color=12;
			} else if(image[num][i][j] == 'Q') {
				ch1=0xA1;  ch2=0xFC;  fg_color=11;
			} else if(image[num][i][j] == 'I') {
				ch1=0xA1;  ch2=0xBD;  fg_color=11;
			} else if(image[num][i][j] == 'L') {
				ch1=0xA1;  ch2=0xF8;  fg_color=11;
			} else if(image[num][i][j] == 'R') {
				ch1=0xA1;  ch2=0xF2;  fg_color=11;
			} else if(image[num][i][j] == 'l') {
				ch1=0xA2;  ch2=0x7E;  fg_color=11;
			} else if(image[num][i][j] == 'r') {
				ch1=0xA2;  ch2=0xA1;  fg_color=11;
			} else if(image[num][i][j] == 'a') {
				ch1=0xA1;  ch2=0xBD;  fg_color=14;
			} else if(image[num][i][j] == 'b') {
				ch1=0xA1;  ch2=0xBD;  fg_color=12;
			} else if(image[num][i][j] == 'c') {
				ch1=0xA2;  ch2=0xA9;  fg_color=14;
			} else if(image[num][i][j] == 'd') {
				ch1=0xA2;  ch2=0xA8;  fg_color=14;
			} else if(image[num][i][j] == 'f') {
				ch1=0xA1;  ch2=0xB6;  fg_color=14;
			} else if(image[num][i][j] == 'g') {
				ch1=0xA2;  ch2=0xAB;  fg_color=14;
			} else if(image[num][i][j] == 'S') {
				ch1=0xA1;  ch2=0xB9;  fg_color=14;
			} else if(image[num][i][j] == '1') {
				ch1=0xA1;  ch2=0xBD;  fg_color=11;
			} else if(image[num][i][j] == '2') {
				ch1=0xA2;  ch2=0xA8;  fg_color=11;
			} else if(image[num][i][j] == '3') {
				ch1=0xA2;  ch2=0xA9;  fg_color=11;
			} else if(image[num][i][j] == '4') {
				ch1=0xA2;  ch2=0xAA;  fg_color=11;
			} else if(image[num][i][j] == '5') {
				ch1=0xA2;  ch2=0xAB;  fg_color=11;
			} else if(image[num][i][j] == '6') {
				ch1=0xA1;  ch2=0xFC;  fg_color=10;
			} else if(image[num][i][j] == '=') {
				ch1=0xA1;  ch2=0xD7;  fg_color=11;
			} else {
				ch1=' ';   ch2=' ';  fg_color=0;
			}
			putASCII2(j*2 + IM_OFFSET_X, i + IM_OFFSET_Y, ch1, fg_color, 0);  
			putASCII2(j*2+1 + IM_OFFSET_X, i + IM_OFFSET_Y, ch2, fg_color, 0);
		}
		
	}
}

void moster_shape(int num,int state,int *shape)
{
	int *p=NULL;
	int i,k;
	
	switch (num) {
		case 1:
			if(state == 0)
				p = june_shape[0];
			else
				p = june_shape[1];
			break;
		case 2:
			if(state == 0)
				p = two_shape[0];
			else
				p = two_shape[1];
			break;
		case 3:
			if(state == 0)
				p = mon_shape[0];
			else
				p = mon_shape[1];
			break;
	}
	
	for (i=0; i<3; i++)
      for (k=0; k<6; k++)
         *(shape+i*6+k) = *(p+i*6+k);
}

extern void tower_shape(int num,int *shape)
{
	int *p=NULL;
	int i,k;
	int color=1;
	
	switch (num) {
		case 1:
			p = level01_shape[0];
			break;
		case 4:
			p = NoColorTS[0];
			color=0;
			break;
	}
	
	if(color == 1)
		for (i=0; i<3; i++)
   	   for (k=0; k<3; k++)
      	   *(shape+i*3+k) = *(p+i*3+k);
	else
		for (i=0; i<3; i++)
   	   for (k=0; k<6; k++)
      	   *(shape+i*6+k) = *(p+i*6+k);
}
