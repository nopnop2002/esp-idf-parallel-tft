#ifndef __LCD_LIB_H__
#define __LCD_LIB_H__

#include "fontx.h"

#define RED				0xf800
#define GREEN			0x07e0
#define BLUE			0x001f
#define BLACK			0x0000
#define WHITE			0xffff
#define GRAY			0x8c51
#define YELLOW			0xFFE0
#define CYAN			0x07FF
#define PURPLE			0xF81F

typedef enum {
  DIRECTION0,
  DIRECTION90,
  DIRECTION180,
  DIRECTION270
} font_direction_t

/*
The version of GCC has changed.
In GCC 10.2.1, I get a "multiple definitions" error, but not in GCC 8.3.0.
There was a change in behaviour about global variables between GCC 8.3.0 and GCC 10.2.1.
In C you are supposed to only define a global variable in one translation unit,
other translation unit that want to access the variable should declare it as "extern".
GCC 8.3.0 has a collaborative definition, so no error occurs.
But GCC 10.2.1 don't have cooperative definition, so error occurs.
*/

#ifndef __TFT_LIB_C__
	#define DECLARE extern
#else
	#define DECLARE
#endif

// Controller-dependent function
DECLARE void (*DrawPixel)(TFT_t * dev, uint16_t x, uint16_t y, uint16_t color);
DECLARE void (*DrawMultiPixels)(TFT_t * dev, uint16_t x, uint16_t y, uint16_t size, uint16_t * colors);
DECLARE void (*DrawFillRect)(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
DECLARE void (*DisplayOff)(TFT_t * dev);
DECLARE void (*DisplayOn)(TFT_t * dev);
DECLARE void (*InversionOff)(TFT_t * dev);
DECLARE void (*InversionOn)(TFT_t * dev);
DECLARE bool (*EnableScroll)(TFT_t * dev);
DECLARE void (*SetScrollArea)(TFT_t * dev, uint16_t tfa, uint16_t vsa, uint16_t bfa);
DECLARE void (*ResetScrollArea)(TFT_t * dev, uint16_t vsa);
DECLARE void (*StartScroll)(TFT_t * dev, uint16_t vsp);

// Global function
void lcdInitDevice(TFT_t * dev, int width, int height, int offsetx, int offsety);
void lcdDrawPixel(TFT_t * dev, uint16_t x, uint16_t y, uint16_t color);
void lcdDrawMultiPixels(TFT_t * dev, uint16_t x, uint16_t y, uint16_t size, uint16_t * colors);
void lcdDrawFillRect(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDisplayOff(TFT_t * dev);
void lcdDisplayOn(TFT_t * dev);
void lcdInversionOff(TFT_t * dev);
void lcdInversionOn(TFT_t * dev);
bool lcdEnableScroll(TFT_t * dev);
void lcdSetScrollArea(TFT_t * dev, uint16_t tfa, uint16_t vsa, uint16_t bfa);
void lcdResetScrollArea(TFT_t * dev, uint16_t vsa);
void lcdStartScroll(TFT_t * dev, uint16_t vsp);

void lcdFillScreen(TFT_t * dev, uint16_t color);
void lcdDrawLine(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDrawRect(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDrawRectAngle(TFT_t * dev, uint16_t xc, uint16_t yc, uint16_t w, uint16_t h, uint16_t angle, uint16_t color);
void lcdDrawTriangle(TFT_t * dev, uint16_t xc, uint16_t yc, uint16_t w, uint16_t h, uint16_t angle, uint16_t color);
void lcdDrawCircle(TFT_t * dev, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void lcdDrawFillCircle(TFT_t * dev, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void lcdDrawRoundRect(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color);
void lcdDrawArrow(TFT_t * dev, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t w, uint16_t color);
void lcdDrawFillArrow(TFT_t * dev, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t w, uint16_t color);
uint16_t rgb565_conv(uint16_t r, uint16_t g, uint16_t b);
int lcdDrawChar(TFT_t * dev, FontxFile *fx, uint16_t x, uint16_t y, uint8_t ascii, uint16_t color);
int lcdDrawString(TFT_t * dev, FontxFile *fx, uint16_t x, uint16_t y, uint8_t * ascii, uint16_t color);
//int lcdDrawSJISChar(TFT_t * dev, FontxFile *fx, uint16_t x, uint16_t y, uint16_t sjis, uint16_t color);
//int lcdDrawUTF8Char(TFT_t * dev, FontxFile *fx, uint16_t x, uint16_t y, uint8_t *utf8, uint16_t color);
//int lcdDrawUTF8String(TFT_t * dev, FontxFile *fx, uint16_t x, uint16_t y, unsigned char *utfs, uint16_t color);
void lcdSetFontDirection(TFT_t * dev, font_direction_t);
void lcdSetFontFill(TFT_t * dev, uint16_t color);
void lcdUnsetFontFill(TFT_t * dev);
void lcdSetFontUnderLine(TFT_t * dev, uint16_t color);
void lcdUnsetFontUnderLine(TFT_t * dev);

#endif

