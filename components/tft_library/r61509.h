#ifndef __R61509_H__
#define __R61509_H__

void r61509_lcdInit(TFT_t * dev, int width, int height, int offsetx, int offsety);
void r61509_lcdDrawPixel(TFT_t * dev, uint16_t x, uint16_t y, uint16_t color);
void r61509_lcdDrawMultiPixels(TFT_t * dev, uint16_t x, uint16_t y, uint16_t size, uint16_t * colors);
void r61509_lcdDrawFillRect(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void r61509_lcdDisplayOff(TFT_t * dev);
void r61509_lcdDisplayOn(TFT_t * dev);
void r61509_lcdInversionOff(TFT_t * dev);
void r61509_lcdInversionOn(TFT_t * dev);
void r61509_lcdBGRFilter(TFT_t * dev);
bool r61509_lcdEnableScroll(TFT_t * dev);
void r61509_lcdSetScrollArea(TFT_t * dev, uint16_t tfa, uint16_t vsa, uint16_t bfa);
void r61509_lcdResetScrollArea(TFT_t * dev, uint16_t vsa);
void r61509_lcdStartScroll(TFT_t * dev, uint16_t vsp);

#endif

