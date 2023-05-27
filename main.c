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


void line(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1) {
  unsigned char dx = abs(x1 - x0);
  unsigned char dy = abs(y1 - y0);
  signed char sx = x0 < x1 ? 1 : -1;
  signed char sy = y0 < y1 ? 1 : -1;
  int err = (dx > dy ? dx : -dy) / 2;
  int e2;

  while (1) {
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

void vertline(unsigned char x0, unsigned char y0, unsigned char y1) {
  unsigned char i;
  for (i = y1; i < y0; i++) {
    plot(x0, i);
  }
}

void clearVertline(unsigned char x0, unsigned char y0, unsigned char y1) {
  unsigned char i;
  for (i = y1; i < y0; i++) {
    clearplot(x0, i);
  }
}

void printB(short topLeftCoordX, short topLeftCoordY)
{
    // we're drawing letters using straight lines, each one 30 wide 60 high
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX, topLeftCoordY+60);
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX+20, topLeftCoordY);
    line(topLeftCoordX, topLeftCoordY+60, topLeftCoordX+20, topLeftCoordY+60);
    line(topLeftCoordX+20, topLeftCoordY, topLeftCoordX+25, topLeftCoordY+5);
    line(topLeftCoordX+20, topLeftCoordY+60, topLeftCoordX+25, topLeftCoordY+55);    
    line(topLeftCoordX, topLeftCoordY+30, topLeftCoordX+20, topLeftCoordY+30);
    line(topLeftCoordX+25, topLeftCoordY+5, topLeftCoordX+25, topLeftCoordY+20);
    line(topLeftCoordX+25, topLeftCoordY+40, topLeftCoordX+25, topLeftCoordY+55);    
    line(topLeftCoordX+25, topLeftCoordY+20, topLeftCoordX+20, topLeftCoordY+30);
    line(topLeftCoordX+25, topLeftCoordY+40, topLeftCoordX+20, topLeftCoordY+30);  
}

void printY(short topLeftCoordX, short topLeftCoordY)
{
    // we're drawing letters using straight lines, each one 30 wide 60 high    
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX+15, topLeftCoordY+30);
    line(topLeftCoordX+15, topLeftCoordY+30, topLeftCoordX+30, topLeftCoordY);
    line(topLeftCoordX+15, topLeftCoordY+30, topLeftCoordX+15, topLeftCoordY+60);    
}

void printT(short topLeftCoordX, short topLeftCoordY)
{
    // we're drawing letters using straight lines, each one 30 wide 60 high    
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX+30, topLeftCoordY);
    line(topLeftCoordX+15, topLeftCoordY, topLeftCoordX+15, topLeftCoordY+60);
}
void printE(short topLeftCoordX, short topLeftCoordY)
{
    // we're drawing letters using straight lines, each one 30 wide 60 high    
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX+30, topLeftCoordY);
    line(topLeftCoordX, topLeftCoordY, topLeftCoordX, topLeftCoordY+60);    
    line(topLeftCoordX, topLeftCoordY+30, topLeftCoordX+20, topLeftCoordY+30);    
    line(topLeftCoordX, topLeftCoordY+60, topLeftCoordX+30, topLeftCoordY+60);    
}


int main() {
  const short sepration = 4;
  const short widthOfLetter = 30;
  
  zx_border(INK_YELLOW);
  
  short int topleftPosX = 50;
  short int topleftPosY = 50;

  //while (1) // main game loop
  //{
  printB(topleftPosX,topleftPosY);
  topleftPosX+=widthOfLetter; // The B to Y requires less seperation to look right
  printY(topleftPosX, topleftPosY);
  topleftPosX+=widthOfLetter+sepration;
  printT(topleftPosX, topleftPosY);
  topleftPosX+=widthOfLetter+sepration;
  printE(topleftPosX, topleftPosY);
  //}
  while(1) {}
}