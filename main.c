// Byte forever banner program for Sinclair ZX Spectrum 48K, Adrian Pilkington 2023
// so, unbelieveabley this is a banner program, written in c and using manual "vector" 
// tyle line drawing to accomplish drawing th letters using a font basically I've made up 
// from the top of my head :)


#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <intrinsic.h>
#include <z80.h>
#include <im2.h>
#include <string.h>
#include <input.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sound.h>
#include <unistd.h>

unsigned char* videoRam = (unsigned char*)0x4000;  // Video RAM address

struct sp1_Rect full_screen = {
  0,
  0,
  32,
  24
};

struct sp1_pss pss = 
{
    &
    full_screen, // print confined to this rectangle
    SP1_PSSFLAG_INVALIDATE | SP1_PSSFLAG_XWRAP | SP1_PSSFLAG_YINC | SP1_PSSFLAG_YWRAP,
    0, // current x
    0, // current y
    0, // attribute mask - overwrite underlying colour
    INK_BLACK | PAPER_WHITE,
    0, // sp1_update* must be consistent with x,y
    0 // visit function (set to zero)
};


void printDebug(int debug) 
{
  char buffer[8];
  sprintf(buffer, "%06d", debug);
  sp1_SetPrintPos(&pss, 1, 30);
  sp1_PrintString(&pss, buffer);
}

void printDebug2(int debug1, int debug2) 
{
  char buffer[8];
  sprintf(buffer, "X=%03d", debug1);
  sp1_SetPrintPos(&pss, 3, 1);
  sp1_PrintString(&pss, buffer);
  sprintf(buffer, "Y=%03d", debug2);  
  sp1_SetPrintPos(&pss, 3, 10);
  sp1_PrintString(&pss, buffer);  
}

void printDebug3(int debug1, int debug2) 
{
  char buffer[8];
  sprintf(buffer, "X=%03d", debug1);
  sp1_SetPrintPos(&pss, 4, 1);
  sp1_PrintString(&pss, buffer);
  sprintf(buffer, "Y=%03d", debug2);  
  sp1_SetPrintPos(&pss, 4, 10);
  sp1_PrintString(&pss, buffer);  
}


void plot(unsigned char x, unsigned char y) {
  * zx_pxy2saddr(x, y) |= zx_px2bitmask(x);
}

void clearplot(unsigned char x, unsigned char y) {
  * zx_pxy2saddr(x, y) &= 0;
}


void line(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, char clearIt) {
  unsigned char dx = abs(x1 - x0);
  unsigned char dy = abs(y1 - y0);
  signed char sx = x0 < x1 ? 1 : -1;
  signed char sy = y0 < y1 ? 1 : -1;
  int err = (dx > dy ? dx : -dy) / 2;
  int e2;

  while (1) {
    if (clearIt) 
      clearplot(x0, y0);
    else
      plot(x0, y0);
    if (x0 == x1 && y0 == y1) break;

    e2 = err;
    if (e2 > -dx) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y0 += sy;
    }
  }
}

void printB(char topLeftCoordX, char topLeftCoordY,char clearIt)
{
    char scale = 3;
    // we're drawing letters using straight lines, each one 30 wide 60 high
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX, topLeftCoordY+60/scale,clearIt);  // | left long vertical bit
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX+20/scale, topLeftCoordY,clearIt);    // top line
    line(topLeftCoordX, topLeftCoordY+60/scale, topLeftCoordX+20/scale, topLeftCoordY+60/scale,clearIt);  // bottom line
    line(topLeftCoordX+20/scale, topLeftCoordY, topLeftCoordX+30/scale, topLeftCoordY+5/scale,clearIt);   // top right diag
    line(topLeftCoordX+20/scale, topLeftCoordY+60/scale, topLeftCoordX+30/scale, topLeftCoordY+55/scale,clearIt);    // bottom right diag up
    line(topLeftCoordX, topLeftCoordY+30/scale, topLeftCoordX+20/scale, topLeftCoordY+30/scale,clearIt);
    line(topLeftCoordX+30/scale, topLeftCoordY+5/scale, topLeftCoordX+30/scale, topLeftCoordY+20/scale,clearIt);  // right upper vertical
    line(topLeftCoordX+30/scale, topLeftCoordY+40/scale, topLeftCoordX+30/scale, topLeftCoordY+55/scale,clearIt); // right lower vertical   
    line(topLeftCoordX+30/scale, topLeftCoordY+20/scale, topLeftCoordX+20/scale, topLeftCoordY+30/scale,clearIt); 
    line(topLeftCoordX+30/scale, topLeftCoordY+40/scale, topLeftCoordX+20/scale, topLeftCoordY+30/scale,clearIt);  
}

void printY(char topLeftCoordX, char topLeftCoordY,char clearIt)
{
      char scale = 3;
    // we're drawing letters using straight lines, each one 30 wide 60 high    
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX+15/scale, topLeftCoordY+30/scale,clearIt);
    line(topLeftCoordX+15/scale, topLeftCoordY+30/scale, topLeftCoordX+30/scale, topLeftCoordY,clearIt);
    line(topLeftCoordX+15/scale, topLeftCoordY+30/scale, topLeftCoordX+15/scale, topLeftCoordY+60/scale,clearIt);    
}

void printT(char topLeftCoordX, char topLeftCoordY,char clearIt)
{
      char scale = 3;
    // we're drawing letters using straight lines, each one 30 wide 60 high    
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX+30/scale, topLeftCoordY,clearIt);
    line(topLeftCoordX+15/scale, topLeftCoordY, topLeftCoordX+15/scale, topLeftCoordY+60/scale,clearIt);
}
void printE(char topLeftCoordX, char topLeftCoordY,char clearIt)
{
      char scale = 3;
    // we're drawing letters using straight lines, each one 30 wide 60 high    
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX+30/scale, topLeftCoordY,clearIt);
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX, topLeftCoordY+60/scale,clearIt);    
    line(topLeftCoordX, topLeftCoordY+30/scale, topLeftCoordX+20/scale, topLeftCoordY+30/scale,clearIt);    
    line(topLeftCoordX, topLeftCoordY+60/scale, topLeftCoordX+30/scale, topLeftCoordY+60/scale,clearIt);    
}

void printF(char topLeftCoordX, char topLeftCoordY,char clearIt)
{
     char scale = 3;
    // we're drawing letters using straight lines, each one 30 wide 60 high    
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX+30/scale, topLeftCoordY,clearIt);
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX, topLeftCoordY+60/scale,clearIt);    
    line(topLeftCoordX, topLeftCoordY+30/scale, topLeftCoordX+20/scale, topLeftCoordY+30/scale,clearIt);        
}

void printO(char topLeftCoordX, char topLeftCoordY,char clearIt)
{
     char scale = 3;
    // we're drawing letters using straight lines, each one 30 wide 60 high    
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX+30/scale, topLeftCoordY,clearIt);
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX, topLeftCoordY+60/scale,clearIt);    
    line(topLeftCoordX+30/scale, topLeftCoordY, topLeftCoordX+30/scale, topLeftCoordY+60/scale,clearIt);    
    line(topLeftCoordX, topLeftCoordY+60/scale, topLeftCoordX+30/scale, topLeftCoordY+60/scale,clearIt);    
}

void printR(char topLeftCoordX, char topLeftCoordY,char clearIt)
{
    char scale = 3;
    // we're drawing letters using straight lines, each one 30 wide 60 high
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX, topLeftCoordY+60/scale,clearIt);  // | left long vertical bit
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX+20/scale, topLeftCoordY,clearIt);    // top line    
    line(topLeftCoordX+20/scale, topLeftCoordY, topLeftCoordX+30/scale, topLeftCoordY+5/scale,clearIt);   // top right diag
    line(topLeftCoordX, topLeftCoordY+30/scale, topLeftCoordX+20/scale, topLeftCoordY+30/scale,clearIt);
    line(topLeftCoordX+30/scale, topLeftCoordY+5/scale, topLeftCoordX+30/scale, topLeftCoordY+20/scale,clearIt);  // right upper vertical
    line(topLeftCoordX+30/scale, topLeftCoordY+20/scale, topLeftCoordX+20/scale, topLeftCoordY+30/scale,clearIt); 
    line(topLeftCoordX+30/scale, topLeftCoordY+60/scale, topLeftCoordX+20/scale, topLeftCoordY+30/scale,clearIt);  
}

void printV(char topLeftCoordX, char topLeftCoordY,char clearIt)
{
    char scale = 3;
    // we're drawing letters using straight lines, each one 30 wide 60 high
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX+15/scale, topLeftCoordY+60/scale,clearIt);
    line(topLeftCoordX+30/scale, topLeftCoordY, topLeftCoordX+15/scale, topLeftCoordY+60/scale,clearIt);  
}


int main() {
  const char sepration = 2;
  const char widthOfLetter = 10;
  char topleftPosX = 5;
  char topleftPosY = 5;
  char clearIt = 0;
  char moveCursorOn = widthOfLetter+sepration;  
  char directionX = 3;
  char directionY = 3;
  char col = 1;
  zx_border(INK_YELLOW);
  zx_cls(col | PAPER_BLACK);
  
  
  while (1) // main game loop
  {
    //for (int i = 0; i < 2; i++)
    {
      int copyOfPosX = topleftPosX;
      int copyOfPosY = topleftPosY;
      printB(copyOfPosX,copyOfPosY, clearIt);
      copyOfPosX+= moveCursorOn; 
      printY(copyOfPosX, copyOfPosY,clearIt);
      copyOfPosX+=moveCursorOn;
      printT(copyOfPosX, copyOfPosY,clearIt);
      copyOfPosX+=moveCursorOn;
      printE(copyOfPosX, copyOfPosY,clearIt);                       
      copyOfPosX+=moveCursorOn+moveCursorOn; // new word!
      printF(copyOfPosX, copyOfPosY,clearIt);  
      copyOfPosX+=moveCursorOn;
      printO(copyOfPosX, copyOfPosY,clearIt);  
      copyOfPosX+=moveCursorOn;
      printR(copyOfPosX, copyOfPosY,clearIt);        
      copyOfPosX+=moveCursorOn;
      printE(copyOfPosX, copyOfPosY,clearIt);      
      copyOfPosX+=moveCursorOn;
      printV(copyOfPosX, copyOfPosY,clearIt);      
      copyOfPosX+=moveCursorOn;
      printE(copyOfPosX, copyOfPosY,clearIt);            
      copyOfPosX+=moveCursorOn;
      printR(copyOfPosX, copyOfPosY,clearIt);            
     // clearIt = 1 - clearIt;
      // Set the entire screen to white
      //for (int i = 0; i < 6912; i++)
     // {
      //   videoRam[i + 1] = col;  // Set the ink color to white (attribute byte + 1)
     // }
      
    }
    topleftPosX+=directionX;
    topleftPosY+=directionY;
    if (topleftPosX > 100) 
    {
      directionX = -directionX;
      zx_cls(col | PAPER_BLACK);
      col += 1;
      if (col > 15) col = 0;
    }
    if (topleftPosY > 160)  
    {
      directionY = -directionY;   
      zx_cls(col | PAPER_BLACK);
      col += 1;
      if (col > 15) col = 0;
    }    
    if (topleftPosX < 5) 
    {
      directionX = -directionX;
      zx_cls(col | PAPER_BLACK);
      col += 1;
      if (col > 8) col = 0;
    }   
    if (topleftPosY < 5) 
    {
      directionY = -directionY;   
      zx_cls(col | PAPER_BLACK);
      col += 1;
      if (col > 8) col = 1;
    }   
  }
}