#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


#define Font_Data "/root/Font.dat"
#define FONT_HEIGHT 16
#define FONT_WIDTH 	16

#define IMX258_SPLC501_IOCTL_BASE	'C'
#define SPLC501_ZI _IOW(IMX258_SPLC501_IOCTL_BASE, 0, char *)
#define SPLC501_ASCII _IOW(IMX258_SPLC501_IOCTL_BASE, 1, char *)
#define SPLC501_BMP _IOW(IMX258_SPLC501_IOCTL_BASE, 2, char *)
#define SPLC501_CLR _IOW(IMX258_SPLC501_IOCTL_BASE, 3, char *)
#define SPLC501_ASCII5x7 _IOW(IMX258_SPLC501_IOCTL_BASE, 4, char *)
#define SPLC501_LCDON _IOW(IMX258_SPLC501_IOCTL_BASE, 5, char *)
#define SPLC501_LCDOFF _IOW(IMX258_SPLC501_IOCTL_BASE, 6, char *)


char asciizi5x7[][6] =
{
/*   */
0x00,0x00,0x00,0x00,0x00,0x00,
/* ! */
0x00,0x00,0xBC,0x00,0x00,0x00,
/* " */
0x00,0x04,0x06,0x02,0x00,0x00,
/* # */
0x28,0xFC,0xFC,0x28,0x00,0x00,
/* $ */
0x00,0xDC,0xFE,0xEC,0x00,0x00,
/* % */
0x1C,0x9C,0xF0,0xFC,0x00,0x00,
/* & */
0xE0,0xBC,0x7C,0x90,0x00,0x00,
/* ' */
0x04,0x02,0x00,0x00,0x00,0x00,
/* ( */
0x00,0x00,0xFC,0x02,0x00,0x00,
/* ) */
0x02,0x84,0x78,0x00,0x00,0x00,
/* * */
0x00,0x28,0x7C,0x28,0x00,0x00,
/* + */
0x10,0x10,0x7C,0x10,0x10,0x00,
/* , */
0x00,0x80,0x00,0x00,0x00,0x00,
/* - */
0x10,0x10,0x10,0x10,0x00,0x00,
/* . */
0x00,0x80,0x00,0x00,0x00,0x00,
/* / */
0x00,0xE0,0x1C,0x02,0x00,0x00,
/* 0 */
0x78,0x84,0x84,0x78,0x00,0x00,
/* 1 */
0x00,0x88,0xF8,0x80,0x00,0x00,
/* 2 */
0x00,0xCC,0xA4,0x9C,0x00,0x00,
/* 3 */
0x00,0xCC,0x94,0xEC,0x00,0x00,
/* 4 */
0x30,0x48,0xFC,0x00,0x00,0x00,
/* 5 */
0x00,0xDC,0x94,0xF4,0x00,0x00,
/* 6 */
0x78,0x94,0x94,0x70,0x00,0x00,
/* 7 */
0x00,0x04,0xF4,0x0C,0x00,0x00,
/* 8 */
0x6C,0x94,0x94,0xEC,0x00,0x00,
/* 9 */
0x38,0xA4,0xA4,0x78,0x00,0x00,
/* : */
0x00,0x00,0x90,0x00,0x00,0x00,
/* ; */
0x00,0x00,0x90,0x00,0x00,0x00,
/* < */
0x00,0x10,0x6C,0x82,0x00,0x00,
/* = */
0x50,0x50,0x50,0x50,0x00,0x00,
/* > */
0x00,0xC6,0x28,0x10,0x00,0x00,
/* ? */
0x08,0xA4,0x14,0x0C,0x00,0x00,
/* @ */
0x78,0xF4,0xAC,0xF8,0x00,0x00,
/* A */
0xC0,0xFC,0xF8,0x80,0x00,0x00,
/* B */
0x84,0xFC,0x94,0xEC,0x00,0x00,
/* C */
0x78,0x84,0x84,0x84,0x00,0x00,
/* D */
0x84,0xFC,0x84,0x78,0x00,0x00,
/* E */
0x84,0xFC,0xBC,0x84,0x00,0x00,
/* F */
0x84,0xFC,0x94,0x3C,0x04,0x00,
/* G */
0x78,0x84,0xA4,0x64,0x20,0x00,
/* H */
0x84,0xFC,0xA4,0xFC,0x84,0x00,
/* I */
0x00,0x84,0xFC,0x84,0x00,0x00,
/* J */
0x00,0x04,0xFC,0x04,0x00,0x00,
/* K */
0x84,0xFC,0xB4,0xEC,0x84,0x00,
/* L */
0x84,0xFC,0x84,0x80,0x00,0x00,
/* M */
0x84,0xFC,0xFF,0xFC,0x84,0x00,
/* N */
0x84,0xFC,0x8C,0xFC,0x04,0x00,
/* O */
0x78,0x84,0x84,0x78,0x00,0x00,
/* P */
0x84,0xFC,0x94,0x1C,0x00,0x00,
/* Q */
0x78,0xC4,0xC4,0x78,0x00,0x00,
/* R */
0x84,0xFC,0x94,0x7C,0x80,0x00,
/* S */
0xDC,0x94,0xA4,0xEC,0x00,0x00,
/* T */
0x04,0x84,0xFC,0x84,0x04,0x00,
/* U */
0xFC,0x84,0x84,0xFC,0x04,0x00,
/* V */
0x04,0x3C,0xC0,0x3C,0x04,0x00,
/* W */
0x1C,0xE4,0x1C,0xF4,0x0C,0x00,
/* X */
0x84,0xFC,0xFC,0x84,0x00,0x00,
/* Y */
0x04,0x9C,0xF0,0x8C,0x04,0x00,
/* Z */
0x84,0xE4,0xBC,0x84,0x00,0x00,
/* [ */
0x00,0x00,0xFE,0x02,0x00,0x00,
/* \ */
0x00,0x1C,0xE0,0x00,0x00,0x00,
/* ] */
0x00,0x02,0xFE,0x00,0x00,0x00,
/* ^ */
0x00,0x04,0x02,0x04,0x00,0x00,
/* _ */
0x00,0x00,0x00,0x00,0x00,0x00,
/* ` */
0x00,0x00,0x02,0x00,0x00,0x00,
/* a */
0xD0,0xB0,0xF0,0x80,0x00,0x00,
/* b */
0x04,0xFC,0x90,0xF0,0x00,0x00,
/* c */
0x00,0xE0,0x90,0x90,0x00,0x00,
/* d */
0x00,0xF0,0x94,0xFC,0x80,0x00,
/* e */
0x00,0xE0,0xB0,0xB0,0x00,0x00,
/* f */
0x00,0x90,0xFC,0x94,0x04,0x00,
/* g */
0x00,0xF0,0xB0,0xB0,0x10,0x00,
/* h */
0x84,0xFC,0x90,0xF0,0x80,0x00,
/* i */
0x00,0x90,0xF4,0x80,0x00,0x00,
/* j */
0x00,0x10,0xF4,0x00,0x00,0x00,
/* k */
0x84,0xFC,0xF0,0x90,0x00,0x00,
/* l */
0x00,0x84,0xFC,0x80,0x00,0x00,
/* m */
0xF0,0x10,0xF0,0x10,0xF0,0x00,
/* n */
0x90,0xF0,0x90,0xF0,0x80,0x00,
/* o */
0x60,0x90,0x90,0x60,0x00,0x00,
/* p */
0x10,0xF0,0x90,0xF0,0x00,0x00,
/* q */
0xF0,0x90,0xF0,0x00,0x00,0x00,
/* r */
0x90,0xF0,0x90,0x10,0x00,0x00,
/* s */
0xB0,0xD0,0xD0,0xD0,0x00,0x00,
/* t */
0x00,0x10,0xF8,0x90,0x00,0x00,
/* u */
0x10,0xF0,0x90,0xF0,0x80,0x00,
/* v */
0x10,0x70,0x80,0x70,0x10,0x00,
/* w */
0x30,0xD0,0x30,0xF0,0x10,0x00,
/* x */
0x90,0xF0,0xF0,0x90,0x00,0x00,
/* y */
0x10,0xB0,0x70,0x10,0x00,0x00,
/* z */
0x00,0xD0,0xF0,0x90,0x00,0x00,
/* { */
0x00,0x00,0x10,0xFE,0x02,0x00,
/* | */
0x00,0x00,0xFF,0x00,0x00,0x00,
/* } */
0x00,0x02,0xFE,0x10,0x00,0x00,
/* ~ */
0x03,0x01,0x02,0x02,0x03,0x00,
/*  */
0x00,0x00,0x00,0x00,0x00,0x00,

};

char asciizi[][16] = 
{
/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*--  文字:  !  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,
/*--  文字:  "  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*--  文字:  #  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00,
/*--  文字:  $  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00,
/*--  文字:  %  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00,
/*--  文字:  &  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10,
/*--  文字:  '  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*--  文字:  (  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,
/*--  文字:  )  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,
/*--  文字:  *  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,
/*--  文字:  +  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00,
/*--  文字:  ,  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,
/*--  文字:  -  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
/*--  文字:  .  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,
/*--  文字:  /  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,
/*--  文字:  0  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,
/*--  文字:  1  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
/*--  文字:  2  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,
/*--  文字:  3  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,
/*--  文字:  4  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,
/*--  文字:  5  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,
/*--  文字:  6  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,
/*--  文字:  7  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,
/*--  文字:  8  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,
/*--  文字:  9  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,
/*--  文字:  :  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,
/*--  文字:  ;  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,
/*--  文字:  <  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,
/*--  文字:  =  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,
/*--  文字:  >  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,
/*--  文字:  ?  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00,
/*--  文字:  @  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00,
/*--  文字:  A  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,
/*--  文字:  B  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,
/*--  文字:  C  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,
/*--  文字:  D  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,
/*--  文字:  E  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,
/*--  文字:  F  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,
/*--  文字:  G  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,
/*--  文字:  H  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,
/*--  文字:  I  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
/*--  文字:  J  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,
/*--  文字:  K  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,
/*--  文字:  L  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,
/*--  文字:  M  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,
/*--  文字:  N  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,
/*--  文字:  O  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,
/*--  文字:  P  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,
/*--  文字:  Q  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00,
/*--  文字:  R  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,
/*--  文字:  S  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,
/*--  文字:  T  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,
/*--  文字:  U  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,
/*--  文字:  V  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,
/*--  文字:  W  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,
/*--  文字:  X  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,
/*--  文字:  Y  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,
/*--  文字:  Z  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,
/*--  文字:  [  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,
/*--  文字:  \  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,
/*--  文字:  ]  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,
/*--  文字:  ^  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*--  文字:  _  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
/*--  文字:  `  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*--  文字:  a  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,
/*--  文字:  b  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,
/*--  文字:  c  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,
/*--  文字:  d  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,
/*--  文字:  e  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,
/*--  文字:  f  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
/*--  文字:  g  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,
/*--  文字:  h  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,
/*--  文字:  i  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
/*--  文字:  j  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,
/*--  文字:  k  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00,
/*--  文字:  l  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
/*--  文字:  m  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,
/*--  文字:  n  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,
/*--  文字:  o  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,
/*--  文字:  p  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,
/*--  文字:  q  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,
/*--  文字:  r  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,
/*--  文字:  s  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,
/*--  文字:  t  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,
/*--  文字:  u  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,
/*--  文字:  v  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00,
/*--  文字:  w  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00,
/*--  文字:  x  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00,
/*--  文字:  y  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00,
/*--  文字:  z  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,
/*--  文字:  {  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,
/*--  文字:  |  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,
/*--  文字:  }  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00,
/*--  文字:  ~  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*--  文字:    --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
	
	
static  int get_cnstr(char *dest, char *src, size_t n)
{
  
  unsigned char bytes;
  unsigned long offset;  
  int qm,wm;
  int i,j;  
  int fd;

  if(src == 0)
  {
     return -1;
  }
  
  fd = open(Font_Data, O_RDONLY);
  if(fd < 0)
  {
    printf("Open %s failed!\n", Font_Data);
    return -2;
  }
        memset(dest, 0, n);
        
  qm = *src;
  wm = *(src+1);
  
  
  if ((qm < 0x80)||(wm < 0x40)) 
  {
    return 0;
  }
  
  if ((qm >= 0xB0)&&(wm >= 0xA1))
  {
      qm = qm - 0xB0;
      wm = wm - 0xA1;
      offset = qm * 94 + wm;
  }
  else if (qm <= 0xA0)
  {
      qm = qm - 0x81;
      wm = wm - 0x40;
      offset = qm * 190 + wm + 6768;
      if (wm > 0x3E)
      {
        offset --;
      }
  }
  else if ((qm >= 0xAA)&&(wm <= 0xA0))
  {
      qm = qm - 0xAA;
      wm = wm - 0x40;
      offset = qm * 96 + wm + 12848;    //xu.xb
      if (wm > 0x3E)
      {
        offset --;
      }        
  } 
  else if ((qm >= 0xA1)&&(wm >= 0xA1))
  {
      qm = qm - 0xA1;
      wm = wm - 0xA1;
      offset = qm * 94 + wm + 12848 + 8160;      
  }   
  else if ((qm >= 0xA8)&&(wm <= 0x96))
  {
      qm = qm - 0xA8;
      wm = wm - 0x40;
      offset = qm * 87 + wm + 12848 + 8160 + 846;      
  }     
  
  offset *= ((FONT_HEIGHT * FONT_WIDTH) >> 3);
  
  lseek(fd,offset,SEEK_SET);
  read(fd, dest, ((FONT_HEIGHT * FONT_WIDTH) >> 3));
  
  close(fd);
  
  return 0;

}

void Os__display_ascii5x7_imx(int x, int y, char c)//API
{
    char hzbuf[6 + 2 + 1];
    int fd;
	int i;
	int j;
	
	fd = open("/dev/splc501c", O_RDWR);
	
	if(fd < 0)
		printf("open ldc failed\n");
	
	if(c < 0x20)
		return;
	        
    hzbuf[0] = y;
    hzbuf[1] = x;
    
    memcpy(&(hzbuf[2]), &(asciizi5x7[c - 0x20]), 6);
    
    ioctl(fd,SPLC501_ASCII5x7,hzbuf);  
    close(fd);       
}

void Os__display_ascii_imx(int x, int y, char c)//API
{
    char hzbuf[16 + 2 + 1];
    int fd;
	int i;
	int j;
	
	fd = open("/dev/splc501c", O_RDWR) ;
	
	if(fd < 0)
		printf("open ldc failed\n");
	
	if(c < 0x20)
	        return;
	        
    hzbuf[0] = y;
    hzbuf[1] = x;
    
    memcpy(&(hzbuf[2]), &(asciizi[c - 0x20]), 16);
    
    ioctl(fd,SPLC501_ASCII,hzbuf);  
    close(fd);       
}

unsigned char Os__graph_display_imx(unsigned char line,unsigned char column,unsigned char *Drawing,unsigned char size)//API
{
	char hzbuf[512];
	int fd;
	int m;
	unsigned char temp;
	unsigned char temp2;
	unsigned char rotate;
	int i;
	int j;
	fd = open("/dev/splc501c", O_RDWR) ;
	if(fd < 0)
	printf("open ldc failed\n");

        hzbuf[1] = column/8;
        hzbuf[0] = 3 - line/16;
        for(j = 0; j < 2; j++)
        {
		    for(i = 0; i < 8; i++)
		    {
		    	hzbuf[((8*j + i)) + 2] = ((Drawing[1+j-1]&(0x01<<(7 - i)))>>(7-i)<<0) + ((Drawing[3+j-1]&(0x01<<(7-i)))>>(7-i)<<1)
		    					+ ((Drawing[5+j-1]&(0x01<<(7-i)))>>(7-i)<<2) + ((Drawing[7+j-1]&(0x01<<(7-i)))>>(7-i)<<3)
		    					+ ((Drawing[9+j-1]&(0x01<<(7-i)))>>(7-i)<<4) + ((Drawing[11+j-1]&(0x01<<(7-i)))>>(7-i)<<5)
		    					+ ((Drawing[13+j-1]&(0x01<<(7-i)))>>(7-i)<<6) + ((Drawing[15+j-1]&(0x01<<(7-i)))>>(7-i)<<7);
		    }
		}
		for(j = 2; j < 4; j++)
        {
		    for(i = 0; i < 8; i++)
		    {
		    	hzbuf[((8*j + i)) + 2] = ((Drawing[1+j-1+16-2]&(0x01<<(7-i)))>>(7-i)<<0) + ((Drawing[3+j-1+16-2]&(0x01<<(7-i)))>>(7-i)<<1)
		    					+ ((Drawing[5+j-1+16-2]&(0x01<<(7-i)))>>(7-i)<<2) + ((Drawing[7+j-1+16-2]&(0x01<<(7-i)))>>(7-i)<<3)
		    					+ ((Drawing[9+j-1+16-2]&(0x01<<(7-i)))>>(7-i)<<4) + ((Drawing[11+j-1+16-2]&(0x01<<(7-i)))>>(7-i)<<5)
		    					+ ((Drawing[13+j-1+16-2]&(0x01<<(7-i)))>>(7-i)<<6) + ((Drawing[15+j-1+16-2]&(0x01<<(7-i)))>>(7-i)<<7);
		    }
		}

        ioctl(fd,SPLC501_ZI,hzbuf);       
	
		close(fd);   
}

void Os__display_zi(int x, int y, char *s)
{
    char hzbuf[512];
    int fd;
	int i;
	int j;
	fd = open("/dev/splc501c", O_RDWR) ;
	
	if(fd < 0)
	printf("open ldc failed\n");
	
    hzbuf[0] = y;
    hzbuf[1] = x;
    get_cnstr(hzbuf+2, s, 32);

    ioctl(fd,SPLC501_ZI,hzbuf);       
	 	
	close(fd);        
}

void Os__display_graph_imx(int x, int y, char *s)
{
        char hzbuf[512];
        int fd;
	int i;
	int j;
	fd = open("/dev/splc501c", O_RDWR) ;
	printf("display graph\n");
	if(fd < 0)
	printf("open ldc failed\n");
	
        ioctl(fd,SPLC501_BMP,s);       
	 	
	close(fd);        
}

void _Os__clr_display(unsigned char line)
{
	int i;
	int j;
	int fd;
	line = 7 - line;
	fd = open("/dev/splc501c", O_RDWR) ;

	if(fd < 0)
	printf("open ldc failed\n");
	
    ioctl(fd,SPLC501_CLR,&line);
    	
	close(fd);    
	
}

unsigned char Os__GB2312_display_imx(unsigned char column,unsigned char line,unsigned char *Text)//API
{
	unsigned char len, i;
	unsigned char *p;
	unsigned char zi[2];
	unsigned char x;

	p = Text;
	len = strlen(Text);

	x = line;
	for(i = 0; i < len; i++)
	{
		if((*(p+i)) < 0x80)
		{
			Os__display_ascii_imx(i+line, column, *(p+i));
			x = x + 1;
		}
		else
		{
			zi[0] = *(p+i);
			i++;
			zi[1] = *(p+i);
			Os__display_zi(2*(i/2)+line-(i - 1)%2, 3 - column, zi);
			x = ((i + line)/2)+2;
		}
	}       
}

unsigned char Os__display_imx(unsigned char Line, unsigned char Column, unsigned char *Text)
{
	return Os__GB2312_display_imx(Line, Column, Text);
}

unsigned char OSMMI_GB2312Display_imx(unsigned char ucFont,unsigned char ucRow,unsigned char ucCol, unsigned char *pucPtr)
{
	Os__GB2312_display_imx(ucRow,ucCol,pucPtr);
}

void Os__display5x7_imx(unsigned line, unsigned column, char *Text)
{
	unsigned char len, i;
	unsigned char *p;
	unsigned char zi[2];
	unsigned char x;

	p = Text;
	len = strlen(Text);

	x = line;
	for(i = 0; i < len; i++)
	{
		if((*(p+i)) < 0x80)
		{
			Os__display_ascii5x7_imx(i+line, column, *(p+i));
		}
	}  
}

unsigned char OSMMI_DisplayASCII_imx (unsigned char ucFont,unsigned char ucRow,unsigned char ucCol, unsigned char * Text)//API
{
	if(ucFont == 0x31)
		Os__display_imx(ucRow, ucCol, Text);
	else
	{
		Os__display5x7_imx( ucCol, ucRow,Text);
	}
}

void OSMMI_ClrDisplay_imx(unsigned char ucFont, unsigned char line)//API
{
	int i;
	if(line >= 255)
	{
		for(i = 0; i < 8; i++)
			_Os__clr_display(i);
	}
	if(ucFont == 0x31)
	{
		_Os__clr_display(2*line);
		_Os__clr_display(2*line + 1);
	}
	else
	_Os__clr_display(line);
	
}

unsigned char OSMMI_GraphDisplay_imx(unsigned char ucType, unsigned char ucRow, unsigned char ucCol,unsigned char *pucPtr,unsigned char ucLen)
{
	int i;
	if(ucType == 0)
	{
		for(i = 0; i < 32; i++)
		{
			*(pucPtr + i) = ~(*(pucPtr+i));
		}
	}
		
	Os__graph_display_imx(ucRow,ucCol,pucPtr,ucLen);
}


unsigned char Os__light_on_imx(void)
{
	int fd;
	fd = open("/dev/splc501c", O_RDWR) ;

	if(fd < 0)
	printf("open ldc failed\n");
	
    ioctl(fd,SPLC501_LCDON,NULL);
	 	
	close(fd);
}

unsigned char Os__light_off_imx(void)
{
	int fd;
	fd = open("/dev/splc501c", O_RDWR) ;

	if(fd < 0)
	printf("open ldc failed\n");
	
    ioctl(fd,SPLC501_LCDOFF,NULL);
	 	
	close(fd);   
}

void Os__set_contrast_imx(unsigned char value)
{

}

void Os__clr_display_imx(unsigned char line)//API
{
	OSMMI_ClrDisplay_imx(0x31, line);
}

