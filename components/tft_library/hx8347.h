#ifndef __HX8347_H__
#define __HX8347_H__

void hx8347_lcdInit(TFT_t * dev, int width, int height, int offsetx, int offsety);
void hx8347_lcd_write_register(TFT_t * dev, uint8_t addr, uint16_t data);
void hx8347_lcdDrawPixel(TFT_t * dev, uint16_t x, uint16_t y, uint16_t color);
void hx8347_lcdDrawMultiPixels(TFT_t * dev, uint16_t x, uint16_t y, uint16_t size, uint16_t * colors);
void hx8347_lcdDrawFillRect(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void hx8347_lcdDisplayOff(TFT_t * dev);
void hx8347_lcdDisplayOn(TFT_t * dev);
void hx8347_lcdInversionOff(TFT_t * dev);
void hx8347_lcdInversionOn(TFT_t * dev);
void hx8347_lcdBGRFilter(TFT_t * dev);
bool hx8347_lcdEnableScroll(TFT_t * dev);
void hx8347_lcdSetScrollArea(TFT_t * dev, uint16_t tfa, uint16_t vsa, uint16_t bfa);
void hx8347_lcdResetScrollArea(TFT_t * dev, uint16_t vsa);
void hx8347_lcdStartScroll(TFT_t * dev, uint16_t vsp);

#endif

