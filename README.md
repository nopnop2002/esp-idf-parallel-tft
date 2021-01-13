# esp-idf-parallel-tft
8 bit parallel TFT Driver for esp-idf.   
You can use such a TFT-Shield with esp32.   

![TFT-Shield](https://user-images.githubusercontent.com/6020549/104253960-10a71380-54b9-11eb-8789-a12c2c769ab4.JPG)

# Support driver
- ILI9325   
- ILI9327   
- ILI9341   
- ILI9342   
- ILI9481   
- ILI9486   
- SPFD5408(Almost the same as ILI9325)   
- R61505(Almost the same as ILI9325)   
- R61509   
- LGDP4532   
- ST7781   
- ST7783(Same as ST7781)   
- ST7796(Same as ILI9486)   

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
 The information provided by sellers on Ebay and AliExpress is largely incorrect.   
 You waste time if you don't choose the right driver.   
 There are many [variations](http://domoticx.com/arduino-shield-2-4-tft-lcd-touch/) of the 2.4 inch shield.   
 You can use [this](https://github.com/prenticedavid/MCUFRIEND_kbv/tree/master/examples/LCD_ID_readreg) to find out the driver.   
 Do not use this on the ESP32 as the GPIO on the ESP32 is not 5V tolerant.   
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

![config-app1](https://user-images.githubusercontent.com/6020549/104252600-c2444580-54b5-11eb-9ec8-92f2fcbbbdae.jpg)

![config-app2](https://user-images.githubusercontent.com/6020549/104242561-b1d69f80-54a2-11eb-9113-9c0ebf7fa268.jpg)

# Wireing  

|TFT||ESP32|
|:-:|:-:|:-:|
|LDC_RST|--|GPIO15(*1)|
|LDC_CS|--|GPIO14(*1)|
|LDC_RS|--|GPIO4(*1)|
|LDC_WR|--|GPIO13(*1)|
|LDC_RD|--|GPIO2(*1)|
|LDC_D0|--|GPIO19(*1)|
|LDC_D1|--|GPIO21(*1)|
|LDC_D2|--|GPIO25(*1)|
|LDC_D3|--|GPIO22(*1)|
|LDC_D4|--|GPIO23(*1)|
|LDC_D5|--|GPIO33(*1)|
|LDC_D6|--|GPIO32(*1)|
|LDC_D7|--|GPIO27(*1)|
|5V|--|5V(*2)|
|3.3V|--|3.3V(*2)|
|GND|--|GND|

(*1)   
You can change any GPIO using menuconfig.   

![config-app3](https://user-images.githubusercontent.com/6020549/104252602-c3757280-54b5-11eb-9a0f-fed0f9825ca0.jpg)

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

# Reference
https://github.com/espressif/esp-iot-solution/issues/19
