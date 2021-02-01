#include <string.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#if CONFIG_LGDP4532

#include "lcd_com.h"
#include "lcd_lib.h"
#include "lgdp4532.h"

#define TAG "LGDP4532"

void lgdp4532_lcdInit(TFT_t * dev, int width, int height, int offsetx, int offsety)
{
	lcdInitDevice(dev, width, height, offsetx, offsety);

	ESP_LOGI(TAG,"Your TFT is LGDP4532");
	ESP_LOGI(TAG,"Screen width:%d",width);
	ESP_LOGI(TAG,"Screen height:%d",height);

	static const uint16_t LGDP4532_regValues[] = {
		0x0000,0x0001, //Device code read
		0x0010,0x0628, //Power control 1 SAP[2:0] BT[3:0] AP[2:0] DK DSTB SLP
		0x0012,0x0006, //Power control 3 PON VRH[3:0]
		//0x0013,0x0A32, //Power control 4 VCOMG VDV[4:0] VCM[6:0]
		0x0011,0x0040, //Power control 2; DC1[2:0] DC0[2:0] VC[2:0]
		//0x0015,0x0050, //Regulator control RSET RI[2:0] RV[2:0] RCONT[2:0]
		0x0012,0x0016, //Power control 3 PON VRH[3:0]
		TFTLCD_DELAY16,50,
		0x0010,0x5660, //Power control 1 SAP[2:0] BT[3:0] AP[2:0] DK DSTB SLP
		TFTLCD_DELAY16,50,
		//0x0013,0x2A4E, //Power control 4 VCOMG VDV[4:0] VCM[6:0]
		//0x0001,0x0100, //Driver output control SM SS
		//0x0002,0x0300, //LCD Driving Wave Control
		0x0003,0x1030, //Entry mode TRI DFM  BGR  ORG I/D[1:0] AM
		//0x0003,0x1020, //Entry mode TRI DFM  BGR	ORG I/D[1:0] AM
		//0x0003,0x1010, //Entry mode TRI DFM  BGR	ORG I/D[1:0] AM
		//0x0003,0x1000, //Entry mode TRI DFM  BGR	ORG I/D[1:0] AM
		//0x0007,0x0202, //Display Control 1 PTDE[1:0] BASEE GON DTE COL D[1:0]
		TFTLCD_DELAY16,50,
		//0x0008,0x0202, //Display Control 2 FP[3:0] BP[3:0] front and back porch (blank period at begin and end..)
		//0x000A,0x0000, //Test Register 1 (RA0h)
		//Gamma adjustment
		0x0030,0x0000,
		0x0031,0x0402,
		0x0032,0x0106,
		0x0033,0x0700,
		0x0034,0x0104,
		0x0035,0x0301,
		0x0036,0x0707,
		0x0037,0x0305,
		0x0038,0x0208,
		0x0039,0x0F0B,
		TFTLCD_DELAY16,50,
		//some of this stuff in range 41-93 really throws things off....
		//0x0041,0x0002,
		//0x0060,0x2700, //Driver Output Control (R60h)
		0x0060,0xA700, //Driver Output Control (R60h)
		0x0061,0x0001, //Base Image Display Control (R61h)
		//0x0090,0x0119,   //Panel Interface Control 1 (R90h) DIVI[1:0]  RTNI[4:0]
		//0x0092,0x010A,  //Panel Interface Control 2 (R92h)  NOWI[2:0] EQI2[1:0] EQI1[1:0]
		//0x0093,0x0004, //Panel Interface Control 3 (R93h) MCPI[2:0]
		//0x00A0,0x0100, //Test Register 1 (RA0h)
		TFTLCD_DELAY16,50,
		0x0007,0x0133, //Display Control 1 PTDE[1:0] BASEE GON DTE COL D[1:0]
		TFTLCD_DELAY16,50,
		//0x00A0,0x0000, //Test Register 1 (RA0h)
	};
	lcd_write_table16(dev, LGDP4532_regValues, sizeof(LGDP4532_regValues));

	// lgdp4532 custom function
	DrawPixel = ili9325_lcdDrawPixel;
	DrawMultiPixels = ili9325_lcdDrawMultiPixels;
	DrawFillRect = ili9325_lcdDrawFillRect;
	DisplayOff = ili9325_lcdDisplayOff;
	DisplayOn = ili9325_lcdDisplayOn;
	InversionOff = ili9325_lcdInversionOff;
	InversionOn = ili9325_lcdInversionOn;
	EnableScroll = ili9325_lcdEnableScroll;
	SetScrollArea = ili9325_lcdSetScrollArea;
	ResetScrollArea = ili9325_lcdResetScrollArea;
	StartScroll = ili9325_lcdStartScroll;

}

#endif
