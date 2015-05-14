
#ifndef		_APPDEF_H
#define		_APPDEF_H

#define HALGSM_APNMAXLEN			0x20
#define	HALGSM_USERNAMELEN			40
#define	HALGSM_PASSWORDLEN			20

typedef struct
{
	unsigned char aucAPN[HALGSM_APNMAXLEN];
    unsigned char aucUserName[HALGSM_USERNAMELEN];
    unsigned char aucPassword[HALGSM_PASSWORDLEN];
}WIRELESS_PARAM;

typedef struct
{
	unsigned char aucSerialNo[16];
    unsigned char aucPADSerialNo[16];
	unsigned char ucETHExist;
	unsigned char aucMACAddr[6];
	unsigned char aucIPAddr[4];
    unsigned char aucMask[4];
    unsigned char aucGatewayAddr[4];
    unsigned char aucDNSAddr[4];
	unsigned char ucBoardCast;
    unsigned char ucGSMExist;
	unsigned char aucAPN[HALGSM_APNMAXLEN];

}CONFIG_PARAM;

#endif
