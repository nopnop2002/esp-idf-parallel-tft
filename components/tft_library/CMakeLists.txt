set(component_srcs "fontx.c"
	"i2s_lcd.c"
	"ili9225.c"
	"ili9320.c"
	"ili9325.c"
	"ili9327.c"
	"ili9341.c"
	"ili9342.c"
	"ili9481.c"
	"ili9486.c"
	"ili9488.c"
	"lcd_com.c"
	"lcd_lib.c"
	"lgdp4532.c"
	"r61509.c"
	"st7781.c"
	"s6d1121.c"
	"hx8347.c"
	)


idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS ".")
