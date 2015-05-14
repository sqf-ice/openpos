#include <osmain.h>
#include <include.h>
#include <global.h>
#include <xdata.h>
unsigned char MenuSys_Appli(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned short uiI,uiJ;
	unsigned char  aucBuf[17],aucZDFunFlag[64];
	unsigned char  ucFlag = 0;
	SELMENU OnlineMenu;
	int i;
	
	memset(&OnlineMenu,0,sizeof(SELMENU));
	for(i=0;i<DataSave0.ChangeParamData.uiZoneCodeNum;i++)
	{
		ucResult=XDATA_Read_ZoneCtrls_File(i);
		if(ucResult==SUCCESS)
		{
			UTIL_BitFlagAnalyze(DataSave1.ZoneCtrl.aucFunction,8,aucZDFunFlag);
			if(aucZDFunFlag[8])
				break;
		}
	}
	
	UTIL_GetMenu_Value(MSG_NULL,SET_DATETIME,CFG_ConstantParamDateTime,NULL,&OnlineMenu);
	UTIL_GetMenu_Value(MSG_NULL,SET_PARAM,CFG_ConstantParamPhoneNo,NULL,&OnlineMenu);
	if(i!=DataSave0.ChangeParamData.uiZoneCodeNum)
		UTIL_GetMenu_Value(MSG_NULL,MSG_MULTSET,UTIL_SetMult,NULL,&OnlineMenu);
	UTIL_GetMenu_Value(MSG_NULL,SET_COMTYPE,STL_ByCom,NULL,&OnlineMenu);
	UTIL_GetMenu_Value(MSG_NULL,MSG_SETCOMMMODE,UTIL_SetCommMode,NULL,&OnlineMenu);
	if(DataSave0.ConstantParamData.ucYTJFlag)
		UTIL_GetMenu_Value(MSG_NULL,MSG_SETCOM,UTIL_SetCOM,NULL,&OnlineMenu);
	UTIL_GetMenu_Value(MSG_NULL,MSG_PRINTTABLES,CFG_PrintTables,NULL,&OnlineMenu);
	//UTIL_GetMenu_Value(NULL,MSG_NETTEST,CFG_NetTest,NULL,&OnlineMenu);
	UTIL_GetMenu_Value(MSG_NULL,MSG_VERSION,CFG_ConstantParamVersion,NULL,&OnlineMenu);
  
	
  ucResult = UTIL_DisplayMenu(&OnlineMenu);
	
	return ucResult;
}

unsigned char MENU_OnlineManage(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned short uiI,uiJ;
	unsigned char  aucBuf[17];
	
	SELMENU OnlineMenu;
	
	memset(&OnlineMenu,0,sizeof(SELMENU));
	
	
	UTIL_GetMenu_Value(TRANS_S_LOGONON,MSG_POSLOGON,LOGON_LogonProcess,NULL,&OnlineMenu);
	//UTIL_GetMenu_Value(TRANS_S_CASHIERLOGONON,  MSG_CASHIERPOSLOGON, CashierLogonProcess,NULL,&OnlineMenu);
	UTIL_GetMenu_Value(MSG_NULL,MSG_POSLOGOFF,LOGON_LogoffProcess,NULL,&OnlineMenu);
	UTIL_GetMenu_Value(MSG_NULL,	MSG_CHANGESUPERPASS, 	CASH_ChangeSuperPasswd1	,NULL,&OnlineMenu);
	UTIL_GetMenu_Value(MSG_NULL,  MSG_CASHIERCHANGEPIN,CashierChangepin,NULL,&OnlineMenu);
	
    ucResult = UTIL_DisplayMenu(&OnlineMenu);
	
	return ucResult; 
}
 