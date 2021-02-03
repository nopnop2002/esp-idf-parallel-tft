# esp-idf-parallel-tft
8 bit parallel TFT Driver for esp-idf.   
You can use such a TFT-Shield with esp32.   

![TFT-Shield](https://user-images.githubusercontent.com/6020549/104253960-10a71380-54b9-11eb-8789-a12c2c769ab4.JPG)

# Support driver

## Generic Sheild   
- ILI9225   
- ILI9226(Same as ILI9225)   
- ILI9325   
- ILI9327   
- ILI9341   
- ILI9342   
- ILI9481   
- ILI9486   
- ILI9488   
- SPFD5408(Almost the same as ILI9325)   
- R61505(Almost the same as ILI9325)   
- R61509   
- LGDP4532   
- ST7775(Same as ILI9225)   
- ST7781   
- ST7783(Same as ST7781)   
- ST7796(Same as ILI9486)   
- HX8347A(*2)   
- HX8347D(Almost the same as HX8347A)(*2)   
- HX8347G(Same as HX8347D)(*2)   
- HX8347I(Same as HX8347D)(*2)   

## OPEN-SMART Products   
- OPEN-SMART S6D1121(*1)   
- OPEN-SMART ST7775(*1)   
- OPEN-SMART ST7783(*1)   

(*1)   
I2S parallel does not work.   
I don't know why.   
GPIO parallel or REGISTER I/O parallel works.   

(*2)   
Very Slow.   
Most drivers require three commands to display one Pixel.   
This driver require 9 commands to display one Pixel.   
For some reason, the color of 0xFFFF does not appear.   

These are all 2.4 inch, 320x240 TFTs.
![TFT-SHIELD-2](https://user-images.githubusercontent.com/6020549/104244320-873a1600-54a5-11eb-93c0-9fad671fdfed.JPG)

3.95 inches is almost twice as large as 2.4 inches.
![TFT-SHIELD-3](https://user-images.githubusercontent.com/6020549/104244328-8903d980-54a5-11eb-8c9a-d26408e04f92.JPG)

# Installation for ESP32

```
git clone https://github.com/nopnop2002/esp-idf-parallel-tft
cd esp-idf-parallel-tft/
idf.py set-target esp32
idf.py menuconfig
idf.py flash
```

# Installation for ESP32-S2

```
git clone https://github.com/nopnop2002/esp-idf-parallel-tft
cd esp-idf-parallel-tft/
idf.py set-target esp32s2
idf.py menuconfig
idf.py flash
```

You have to set this config value with menuconfig.   
- CONFIG_DRIVER   
 __IMPORTANT__   
 The information provided by sellers on Ebay and AliExpress is largely incorrect.   
 You waste time if you don't choose the right driver.   
 There are many [variations](http://domoticx.com/arduino-shield-2-4-tft-lcd-touch/) of the 2.4 inch shield.   
 You can use [this](https://github.com/prenticedavid/MCUFRIEND_kbv/tree/master/examples/LCD_ID_readreg) to find out the driver.   
 This is for Arduino UNO.   
 Do not use this on the ESP32 as the GPIO on the ESP32 is not 5V tolerant.   
 __Never believe the seller's information.__   
- CONFIG_INTERFACE   
 Most drivers work using I2S parallel.   
 However, some drivers only work using GPIO parallels or REGISTER I/O parallels.   
 I2S parallel is most fast.   
 REGISTER I/O parallel is the next fastest.   
 When using REGISTER I/O parallel, GPIO from D0 to D7 is 0 to 31.   
 GPIO parallel is most slow.   
- CONFIG_WIDTH   
- CONFIG_HEIGHT   
 Specifies the resolution of the TFT.   
- CONFIG_OFFSETX   
- CONFIG_OFFSETY   
 You can specify the GRAM offset, but I've never seen a TFT with an offset.   
- CONFIG_INVERSION   
 For some TFTs, the BGR may be inverted.   
 Specify if the colors are inverted.

![config-menu](https://user-images.githubusercontent.com/6020549/104242485-94093a80-54a2-11eb-934b-90eda9fb7bbe.jpg)

![config-app1](https://user-images.githubusercontent.com/6020549/106704254-27abd200-662f-11eb-8697-743e220c030d.jpg)

![config-app2](https://user-images.githubusercontent.com/6020549/106686442-9d06ab00-660d-11eb-9a53-912e221278ce.jpg)

![config-app4](https://user-images.githubusercontent.com/6020549/106704258-28dcff00-662f-11eb-8a75-2070c3fe746a.jpg)

# Wireing  

|TFT||ESP32|
|:-:|:-:|:-:|
|LDC_RST|--|GPIO32(*1)|
|LDC_CS|--|GPIO33(*1)|
|LDC_RS|--|GPIO15(*1)|
|LDC_WR|--|GPIO4(*1)|
|LDC_RD|--|GPIO2(*1)|
|LDC_D0|--|GPIO12(*1)|
|LDC_D1|--|GPIO13(*1)|
|LDC_D2|--|GPIO26(*1)|
|LDC_D3|--|GPIO25(*1)|
|LDC_D4|--|GPIO17(*1)|
|LDC_D5|--|GPIO16(*1)|
|LDC_D6|--|GPIO27(*1)|
|LDC_D7|--|GPIO14(*1)|
|5V|--|5V(*2)|
|3.3V|--|3.3V(*2)|
|GND|--|GND|

(*1)   
You can change any GPIO using menuconfig.   

![config-app3](https://user-images.githubusercontent.com/6020549/106685441-be669780-660b-11eb-8e8e-790348e65921.jpg)

(*2)   
When a regulator(It's often AMS1117) is mounted on the back, it's operated 5V.   
When a regulator is NOT mounted on the back, it's operated 3.3V.   

![Back](https://user-images.githubusercontent.com/6020549/104248029-1f3afe00-54ac-11eb-913d-0a832fb569b2.JPG)

__Note__   
My R61509V has a regulator on the back.   
Normally, a TFT with a regulator works at 5V, but my R61509V doesn't work unless I supply both 5V and 3.3V.   

__Note__   
ESP32 development board cannot supply too much current.   
It is more stable when supplied from an external power source.   

![All](https://user-images.githubusercontent.com/6020549/104249117-1e0ad080-54ae-11eb-8c25-46a2d8fa5fed.JPG)

![Circuit](https://user-images.githubusercontent.com/6020549/104282887-0efa4180-54f3-11eb-8768-27a48ec38129.jpg)

# Graphic support
![Graphic1](https://user-images.githubusercontent.com/6020549/104248260-848eef00-54ac-11eb-9ab8-4b74a2a7713f.JPG)
![Graphic2](https://user-images.githubusercontent.com/6020549/104248263-85c01c00-54ac-11eb-9d19-6886827cd29d.JPG)
![Graphic3](https://user-images.githubusercontent.com/6020549/104248265-86f14900-54ac-11eb-937b-b2bd5ed4ce69.JPG)
![Graphic4](https://user-images.githubusercontent.com/6020549/104248266-8789df80-54ac-11eb-89e6-bad34a3c035d.JPG)
![Graphic5](https://user-images.githubusercontent.com/6020549/104248267-88227600-54ac-11eb-903f-3c64f4e7cb60.JPG)
![Graphic6](https://user-images.githubusercontent.com/6020549/104248268-88bb0c80-54ac-11eb-827b-d886e0491664.JPG)
![Graphic7](https://user-images.githubusercontent.com/6020549/104248273-8953a300-54ac-11eb-8ed1-ffc38e8d6a7c.JPG)
![Graphic8](https://user-images.githubusercontent.com/6020549/104248276-89ec3980-54ac-11eb-8b0d-0bb90e33473e.JPG)

# Fonts support
![Font1](https://user-images.githubusercontent.com/6020549/104248202-6628f380-54ac-11eb-893d-08fdbfd5ee93.JPG)
It's possible to text rotation and invert.   
![Font2](https://user-images.githubusercontent.com/6020549/104248205-675a2080-54ac-11eb-8268-c7b585061205.JPG)
![Font3](https://user-images.githubusercontent.com/6020549/104248206-67f2b700-54ac-11eb-9b94-2cc37f0360e3.JPG)
![Font4](https://user-images.githubusercontent.com/6020549/104248208-688b4d80-54ac-11eb-8b88-d87b5b084e71.JPG)
It's possible to indicate more than one font at the same time.   
![Font5](https://user-images.githubusercontent.com/6020549/104248209-6923e400-54ac-11eb-8812-a438ca1be724.JPG)

# Image support
BMP file   
![Image1](https://user-images.githubusercontent.com/6020549/104248104-3974dc00-54ac-11eb-9b97-56a062f13e5b.JPG)
JPEG file   
![Image2](https://user-images.githubusercontent.com/6020549/104248108-3aa60900-54ac-11eb-8545-3c8971a344a9.JPG)
PNG file    
![Image3](https://user-images.githubusercontent.com/6020549/104248110-3b3e9f80-54ac-11eb-9487-a0379f40fd35.JPG)

# Font File   
You can add your original font file.   
The format of the font file is the FONTX format.   
Your font file is put in font directory.   
Your font file is uploaded to SPIFFS partition using meke flash.   

Please refer [this](http://elm-chan.org/docs/dosv/fontx_e.html) page about FONTX format.   

```
FontxFile yourFont[2];
InitFontx(yourFont,"/spiffs/your_font_file_name","");
uint8_t ascii[10];
strcpy((char *)ascii, "MyFont");
lcdDrawString(dev, yourFont, x, y, ascii, color);
```

# Application layer

![LibraryLayer](https://user-images.githubusercontent.com/6020549/104282561-972c1700-54f2-11eb-9b0b-732f17e9d41b.jpg)

# Performance comparison using ILI9488(320x480)

|Test|GPIO parallel|REGISTER I/O parallel|I2S parallel|
|:-:|:-:|:-:|:-:|
|AddressTest|1300|430|70|
|FillTest|5320|2810|1680|
|ColorBarTest|1290|460|110|
|ArrowTest|1360|460|210|
|LineTest|3190|1280|3650|
|CircleTest|3030|1230|3310|
|RoundRectTest|3060|1220|3400|
|RectAngleTest|4110|1890|11480|
|TriangleTest|4700|2110|13290|
|DirectionTest|1440|530|340|
|HorizontalTest|1750|640|740|
|VerticalTest|1750|640|730|
|FillRectTest|2340|890|280|
|ColorTest|2630|970|400|
|BMPTest|3350|2190|1370|
|JPEGTest|3870|2990|2630|
|PNGTest|4310|3450|3050|

# Reference about I2S parallel mode
https://github.com/espressif/esp-iot-solution/issues/19
