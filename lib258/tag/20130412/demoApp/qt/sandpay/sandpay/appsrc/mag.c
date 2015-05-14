#include <oslib.h>
//#include <osdata.h>
#include <osgraph.h>
#include <include.h>
#include <global.h>
#include <xdata.h>

/************** dpnew 031013**************/
extern union seve_in eve_in;

unsigned char MAG_GetTrackInfo(unsigned char *pucTrack)
{
	unsigned short uiI;
	unsigned char  *pucPtr;
	Uart_Printf("Enter MAG_GetTrackInfo\n");
	
	pucPtr = pucTrack;
	pucPtr += 2;
	/* Track2 information */
	EXTRATRANS_ucISO2Status = *(pucPtr);

	pucPtr ++;
	EXTRATRANS_uiISO2Len = *(pucPtr);
	Uart_Printf("\nEXTRATRANS_ucISO2Status=%02x\n",EXTRATRANS_ucISO2Status);	
	Uart_Printf("EXTRATRANS_uiISO2Len=%d\n",EXTRATRANS_uiISO2Len);	
	/***判断二磁道数据是否正确***/
	if(EXTRATRANS_ucISO2Status != SUCCESS_TRACKDATA)
			return ERR_MAG_TRACKDATA;
	if(EXTRATRANS_uiISO2Len > TRANS_ISO2LEN)
		return(ERR_MAG_TRACKDATA);
		
	pucPtr ++;

	for( uiI=0; uiI<EXTRATRANS_uiISO2Len; uiI++,pucPtr ++)
	{
    		EXTRATRANS_aucISO2[uiI] =  *(pucPtr) | 0x30;
	}

	Uart_Printf("%s\n",EXTRATRANS_aucISO2);

    	/* Track3 information */
   	EXTRATRANS_ucISO3Status = *(pucPtr);
	pucPtr ++;
	EXTRATRANS_uiISO3Len = *(pucPtr);

	pucPtr ++;

	Uart_Printf("EXTRATRANS_ucISO3Status=%02x\n",EXTRATRANS_ucISO3Status);	
	Uart_Printf("EXTRATRANS_uiISO3Len=%d\n",EXTRATRANS_uiISO3Len);	

	for( uiI=0; uiI<EXTRATRANS_uiISO3Len; uiI++,pucPtr ++)
	{
    		EXTRATRANS_aucISO3[uiI] =  *(pucPtr) | 0x30;
    	}

	Uart_Printf("%s\n",EXTRATRANS_aucISO3);

	
	/***过滤三磁道状态不正确和磁道数据超长***/
	if((EXTRATRANS_ucISO3Status != SUCCESS_TRACKDATA)
		||(EXTRATRANS_uiISO3Len > TRANS_ISO3LEN))
	{
		EXTRATRANS_uiISO3Len = 0;
		Uart_Printf("EXTRATRANS_uiISO3Len=%d\n",EXTRATRANS_uiISO3Len);	
	}
	if(memcmp(">>>",&EXTRATRANS_aucISO2[20],3)&&memcmp("===",&EXTRATRANS_aucISO2[20],3))
	{
		return(ERR_NOTPROC);
	}
	NormalTransData.ucNewOrOld=1;
	return(SUCCESS);
}

unsigned char Mag_Menu(union seve_in *EveIn)
{
	unsigned char strBuf[17];
	unsigned char ucResult=SUCCESS,flag1=1,flag2=1,flag3=1,flag4=1,flag5=1,flag6=1,flag7=1;
	unsigned char flag8=1,flag9=1,flaga=1,flagb=1,flagc=1,flagd=1,flage=1;
	int i;
	SELMENU ManagementMenu;
	Uart_Printf("Enter magmenu\n");
	if(ucResult==SUCCESS)
	{
		if( MAG_GetTrackInfo(&(EveIn->e_mag_card.mag_card[0])) != SUCCESS)
		{
			return(ERR_NOTPROC);
		}
	}
	if(ucResult==SUCCESS)
	{
		if(memcmp(">>>",&EXTRATRANS_aucISO2[20],3)&&memcmp("===",&EXTRATRANS_aucISO2[20],3))
		{
			return(ERR_NOTPROC);
		}
	}/*
	if(ucResult==SUCCESS)
	{
		if(memcmp("83657868",EXTRATRANS_aucISO3,8))
		{
			return(ERR_NOTPROC);
		}
	}*/
	if(ucResult==SUCCESS)
	{
		ucResult=CFG_CheckFlag();
	}
	if(ucResult==SUCCESS)
	{
		NormalTransData.ucCardType=CARD_MAG;
		asc_bcd(NormalTransData.aucUserCardNo,8,EXTRATRANS_aucISO2,16);
		asc_bcd(&NormalTransData.ucPtCode,1,&EXTRATRANS_aucISO2[23],2);
		asc_bcd(NormalTransData.aucZoneCode,3,&EXTRATRANS_aucISO2[25],6);
		NormalTransData.aucExpiredDate[0]=0x20;
		asc_bcd(&NormalTransData.aucExpiredDate[1],3,&EXTRATRANS_aucISO3[21],6);
		ucResult=CFG_CheckCardValid();
	}
	if(ucResult==SUCCESS)
	{
		memset(&ManagementMenu,0,sizeof(SELMENU));
		Uart_Printf("DataSave1.CardTable.ucCountType=%02x\n",DataSave1.CardTable.ucCountType);
		Uart_Printf("RunData.aucZDFunFlag\n");
		for(i=0;i<64;i++)
			Uart_Printf("%02x ",RunData.aucZDFunFlag[i]);
		Uart_Printf("\n");
		Uart_Printf("RunData.aucFunFlag\n");
		for(i=0;i<32;i++)
			Uart_Printf("%02x ",RunData.aucFunFlag[i]);
		Uart_Printf("\n");
		Uart_Printf("RunData.aucWDFunFlag\n");
		for(i=0;i<16;i++)
			Uart_Printf("%02x ",RunData.aucWDFunFlag[i]);
		Uart_Printf("\n");
		if(RunData.aucZDFunFlag[20]!=1||(DataSave1.CardTable.ucCountType=='1'&&RunData.aucFunFlag[2]==0)
			||DataSave1.CardTable.ucCountType=='3')
			flag4=0;
		if(RunData.aucZDFunFlag[40]!=1||(DataSave1.CardTable.ucCountType=='1'&&RunData.aucFunFlag[2]==0))
			flag3=0;
		if(RunData.aucZDFunFlag[1]!=1&&RunData.aucZDFunFlag[21]!=1
			||DataSave1.CardTable.ucCountType=='3')
			flag6=0;
		if(RunData.aucZDFunFlag[47]!=1||(DataSave1.CardTable.ucCountType=='1'&&RunData.aucFunFlag[2]==0)
			||DataSave1.CardTable.ucCountType=='3')
			flag5=0;/**/
		if(RunData.aucZDFunFlag[26]!=1||DataSave1.CardTable.ucCountType=='3')
			flag7=0;
		if(RunData.aucZDFunFlag[35]!=1||DataSave1.CardTable.ucCountType!='3')
			flag9=0;
		if(RunData.aucZDFunFlag[11]!=1||(DataSave1.CardTable.ucCountType=='1'&&RunData.aucFunFlag[2]==0))
			flag8=0;
		if(RunData.aucZDFunFlag[41]!=1)
			flaga=0;
		UTIL_GetMenu_Value(TRANS_S_ONLINEPURCHASE,MSG_ONLINEPURCHASE,Untouch_OnlinePurchase,&flag4,&ManagementMenu);
		UTIL_GetMenu_Value(TRANS_S_VOID,MSG_VOID,Untouch_Void,&flag4,&ManagementMenu);
		UTIL_GetMenu_Value(TRANS_S_ONLINEPURCHASE_P,MSG_ONLINEPURCHASE_P,Untouch_OnlinePurchase,&flag9,&ManagementMenu);
		UTIL_GetMenu_Value(TRANS_S_VOID_P,MSG_VOID_P,Untouch_Void,&flag9,&ManagementMenu);
		UTIL_GetMenu_Value(MSG_NULL,  MSG_PAYADMIN,Untouch_PayAdmin,&flag6,&ManagementMenu);	
		UTIL_GetMenu_Value(TRANS_S_PREAUTH,  MSG_PREAUTHPROCESS,	  Untouch_PreAuthProcess,&flag5,&ManagementMenu);	
		UTIL_GetMenu_Value(TRANS_S_MOBILECHARGE,  MSG_MOBILEADMIN,	  Untouch_MobileAdmin,&flag7,&ManagementMenu);	
		UTIL_GetMenu_Value(TRANS_S_QUERY,  MSG_QUERY,	  Untouch_Query,&flag3,&ManagementMenu);	
		UTIL_GetMenu_Value(TRANS_S_ONLINEREFUND,  MSG_ONLINEREFUND,	  Untouch_OnlineRefund,&flag8,&ManagementMenu);	
		UTIL_GetMenu_Value(MSG_NULL,  MSG_CHANEPIN,	  Untouch_ChangePin,&flaga,&ManagementMenu);	
		if(flag3==0&&flag4==0&&flag5==0&&flag6==0&&flag7==0&&flag8==0&&flag9==0&&flaga==0)
			return ERR_NOFUNCTION;
	}

	if(ucResult==SUCCESS)
	{
		CFG_SetBatchNumber();
	}	
	if( ucResult == SUCCESS)
	{
		ucResult = UTIL_DisplayMenu(&ManagementMenu);
	}

	if( ucResult != SUCCESS)
	{
		COMMS_FinComm();
		MSG_DisplayErrMsg(ucResult);
	}
		
	return SUCCESS;	
}

 