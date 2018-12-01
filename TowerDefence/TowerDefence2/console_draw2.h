#include <windows.h>
#ifndef CONSOLE_DRAW2_
#define CONSOLE_DRAW2_
#define SCREEN_WIDTH 80 
#define SCREEN_HEIGHT 25 
extern void putASCII2( int x, int y, int ch, int fgcolor, int bgcolor );
extern void drawCmdWindow();
#endif
/* 
putASCII2()  最後兩?參數分別是字元前景和背景的顏色
可以參考下?的顏色?對照表來?定你想要的顏色  
   
   0: 黑     1: 暗藍   2: 暗綠   3: 暗青 
   4: 暗?   5: 暗紫   6: 暗黃   7: 淺灰
   8: 暗灰   9: 亮藍  10: 亮綠  11: 亮青
  12: 亮?  13: 亮紫  14: 亮黃  15: 白   
*/ 
