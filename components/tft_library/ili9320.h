#ifndef __ILI9320_H__
#define __ILI9320_H__

void ili9320_lcdInit(TFT_t * dev, int width, int height, int offsetx, int offsety);
void ili9325_lcdDrawPixel(TFT_t * dev, uint16_t x, uint16_t y, uint16_t color);
void ili9325_lcdDrawMultiPixels(TFT_t * dev, uint16_t x, uint16_t y, uint16_t size, uint16_t * colors);
void ili9325_lcdDrawFillRect(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ili9325_lcdDisplayOff(TFT_t * dev);
void ili9325_lcdDisplayOn(TFT_t * dev);
void ili9325_lcdInversionOff(TFT_t * dev);
void ili9325_lcdInversionOn(TFT_t * dev);
void ili9325_lcdBGRFilter(TFT_t * dev);
bool ili9325_lcdEnableScroll(TFT_t * dev);
void ili9325_lcdSetScrollArea(TFT_t * dev, uint16_t tfa, uint16_t vsa, uint16_t bfa);
void ili9325_lcdResetScrollArea(TFT_t * dev, uint16_t vsa);
void ili9325_lcdStartScroll(TFT_t * dev, uint16_t vsp);

#endif

