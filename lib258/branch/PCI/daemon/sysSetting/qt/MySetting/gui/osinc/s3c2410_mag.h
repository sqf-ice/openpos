/*
*/
#ifndef __S3C2410_MAG_H__
#define __S3C2410_MAG_H__
#include <linux/ioctl.h>

#include "osdriver.h"

#define DEVICE_NAME "s3c2410_mag"
#define DRIVER_VERSION "v0.1"

#define S3C2410_MAG_IOCTL_BASE	'M'

#define MAX_TK_BUFLen	140			//定义每磁道数据最大长度
#define MGHW_SUCCESS 0
#define MGHW_ODDERR 4
#define MGHW_LRCERR 5


unsigned char TrackOneData[] =
{
    0x40,0x01,0x02,0x43,0x04,0x45,0x46,0x07,0x08,0x49,0x4A,0x0b,0x4c,0x0d,0x0e,0x4f,
    0x10,0x51,0x52,0x13,0x54,0x15,0x16,0x57,0x58,0x19,0x1a,0x5b,0x1c,0x5d,0x5e,0x1f,
    0x20,0x61,0x62,0x23,0x64,0x25,0x26,0x67,0x68,0x29,0x2a,0x6b,0x2c,0x6d,0x6e,0x2f,
    0x70,0x31,0x32,0x73,0x34,0x75,0x76,0x37,0x38,0x79,0x7a,0x3b,0x7c,0x3d,0x3e,0x7f
};

unsigned char TrackOneASCII[] =
{
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f
};


unsigned char BitMask[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

typedef struct
{
    int pin;
    int data;
}PIN_INFO;

#define MAX_TK_BITLen		704
#define MAX_TK_BUFLen		140
typedef struct
{
    unsigned char ucErr;
    unsigned char ucTKLen;
    unsigned char aucTKData[MAX_TK_BUFLen];
//    unsigned char aucTKData[140];
}TK_DATA;


//磁卡配置参数
typedef struct
{
    unsigned char ucMagDebug;
    unsigned char ucMagVer[5+1];
}OSMAGCFG;

OSMAGCFG MagCfg;
#define MAG_RESET		_IOW(S3C2410_MAG_IOCTL_BASE, 0, int)
#define MAG_READ		_IOR(S3C2410_MAG_IOCTL_BASE, 1, int)
#define MAG_READ_STATUS _IOWR(S3C2410_MAG_IOCTL_BASE, 2, int) //读取刷卡状态
#define MAG_SET_CFG _IOWR(S3C2410_MAG_IOCTL_BASE, 3, OSMAGCFG )



#endif

