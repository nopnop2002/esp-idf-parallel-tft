#ifndef __S6D1121_H__
#define __S6D1121_H__

void s6d1121_lcdInit(TFT_t * dev, int width, int height, int offsetx, int offsety);
void s6d1121_lcdDrawPixel(TFT_t * dev, uint16_t x, uint16_t y, uint16_t color);
void s6d1121_lcdDrawMultiPixels(TFT_t * dev, uint16_t x, uint16_t y, uint16_t size, uint16_t * colors);
void s6d1121_lcdDrawFillRect(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void s6d1121_lcdDisplayOff(TFT_t * dev);
void s6d1121_lcdDisplayOn(TFT_t * dev);
void s6d1121_lcdInversionOff(TFT_t * dev);
void s6d1121_lcdInversionOn(TFT_t * dev);
void s6d1121_lcdBGRFilter(TFT_t * dev);
bool s6d1121_lcdEnableScroll(TFT_t * dev);
void s6d1121_lcdSetScrollArea(TFT_t * dev, uint16_t tfa, uint16_t vsa, uint16_t bfa);
void s6d1121_lcdResetScrollArea(TFT_t * dev, uint16_t vsa);
void s6d1121_lcdStartScroll(TFT_t * dev, uint16_t vsp);

#endif

