#ifndef TEMPDRAWFONT_H
#define TEMPDRAWFONT_H

#include<stdint.h>

void InitFont();
void KillFont();
void drawDigit(int num, int x, int y, int size=5, uint32_t color=0x00ffffff);
void drawNumber(int num, int x, int y, int size=5, uint32_t color=0x00ffffff);

#endif
