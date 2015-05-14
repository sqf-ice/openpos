#ifndef _CASHFILE_H
#define _CASHFILE_H

#include <TypeDef.h>
#include <include.h>
#include <interfaceparam.h>
#ifdef _cplusplus
extern "C" {
#endif

#define SALEERR_SUCCESS         0
#define SALEERR_SEEKFILE		1
#define	SALEERR_READFILE        2
#define SALEERR_OPENFILE        4
#define SALEERR_WRITEFILE		5
#define SALEERR_DELETERECORD    6

//2009-9-6 15:53 cbf
unsigned char File_CheckExistFile(unsigned char* pszFileName,unsigned short uiFileLen);
unsigned char File_ReadBasicConfigFile(BASICCONFIG * pBasicConfigParam);
unsigned char File_WriteBasicConfigFile(BASICCONFIG * pBasicConfigParam);
unsigned char File_ReadInterfaceKeyFile(INTERFACEKEYFILE * pInterfacekeyFile);
unsigned char File_WriteInterfaceKeyFile(INTERFACEKEYFILE * pInterfacekeyFile);
void OnEve_INTERFACE_Init(void); 
//End


UINT	FILE_InsertRecordByFileName(UCHAR* pucFileName,UCHAR *pucRecord,USHORT uiRecordSize);
UINT 	FILE_SearchRecordByFileName(UCHAR* pucFileName,USHORT uiRecordSize,USHORT uiRecordItemAddrOffset,
							  UCHAR *pucItemValue,USHORT uiItemValueLen,UCHAR *pRecordBuff);
UCHAR 	FILE_DeleteRecordByFileName(UCHAR* pucFileName,USHORT uiRecordLen,UINT uiStartNum,UINT uiDeleteNums);
UCHAR	FILE_ReadRecord(int iHandle,UINT uiRecordNum,UCHAR* pRecordBuff,USHORT uiRecordLen);
UCHAR	FILE_ReadRecordNums(int iHandle,UINT* puiRecordNum);
UCHAR	FILE_WriteRecord(int iHandle,UINT uiRecordNum,UCHAR* pRecordBuff,USHORT uiRecordLen);
UCHAR	FILE_WriteRecordNums(int iHandle,UINT uiRecordNum);
UCHAR	FILE_ReadRecordByFileName(UCHAR* pucFileName,UINT uiRecordNum,UCHAR* pRecordBuff,USHORT uiRecordLen);
UCHAR	FILE_ReadRecordNumByFileName(UCHAR* pucFileName,UINT *puiRecordNum);

#ifdef _cplusplus
}
#endif
#endif
