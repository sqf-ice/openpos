#include <include.h>
#include <global.h>
#include <xdata.h>
#include <toolslib.h>
#include <osgraph.h>




unsigned char FILE_CheckConstantParamData(void)
{
	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveConstant].aucFileName))<0)
	{  		
      //  iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveConstant].aucFileName,O_CREATE|O_RDWR);
       iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveConstant].aucFileName,O_CREATE);

		if(iFileHandel>=0)
		{				
			OSMEM_Memset(&xDATA_Constant,0,xDATA_ConstantSize);
			memcpy(DataSave0.ConstantParamData.ucAPN,"CMNET",5);
			iFileErrNo = OSAPP_FileSeek(iFileHandel,0,SEEK_SET);
			if(!iFileErrNo)
				iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&xDATA_Constant,xDATA_ConstantSize);
			OSAPP_FileClose(iFileHandel);
		}
	}
	else
	{		
		if(iFileSize==xDATA_ConstantSize)
		{	
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveConstant].aucFileName,O_READ);
			iFileErrNo = OSAPP_FileSeek(iFileHandel,0,SEEK_SET);
			if(!iFileErrNo)		
				iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_Constant,xDATA_ConstantSize);
					
		}
		else if(iFileSize)
		{
            iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveConstant].aucFileName,O_READ);
			if( iFileHandel >= 0 )
		    {
		        OSAPP_FileTrunc(iFileHandel,0);
		    }
		}		
		OSAPP_FileClose(iFileHandel);
	}	
	return(OSAPP_FileGetLastError());
}

unsigned char FILE_CheckRedo(void)
{

	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveRedoTrans].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveRedoTrans].aucFileName,O_CREATE);
		
		if(iFileHandel>=0)
		{
			OSMEM_Memset(&DataSave0.RedoTrans,0,xDATA_RedoTransSize);
			//xDATA_Change.uiStartNumber = 0;
			iFileErrNo = OSAPP_FileSeek(iFileHandel,0,SEEK_SET);
			if(!iFileErrNo)
				iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&DataSave0.RedoTrans,xDATA_RedoTransSize);
			OSAPP_FileClose(iFileHandel);
		}
	}else
	{
		if(iFileSize==xDATA_RedoTransSize)
		{
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveRedoTrans].aucFileName,O_READ);
			iFileErrNo = OSAPP_FileSeek(iFileHandel,0,SEEK_SET);
			if(!iFileErrNo)
				iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&DataSave0.RedoTrans,xDATA_RedoTransSize);
			
		}
		else if(iFileSize)
		{
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveRedoTrans].aucFileName,O_READ);	
			OSAPP_FileTrunc(iFileHandel,0);
		}
		OSAPP_FileClose(iFileHandel);	
	}
	return(OSAPP_FileGetLastError());	
}


unsigned char FILE_CheckChangeParamData(void)
{

	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveChange].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveChange].aucFileName,O_CREATE);
		
		if(iFileHandel>=0)
		{
			OSMEM_Memset(&xDATA_Change,0,xDATA_ChangeSize);
			//xDATA_Change.uiStartNumber = 0;
			iFileErrNo = OSAPP_FileSeek(iFileHandel,0,SEEK_SET);
			if(!iFileErrNo)
				iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&xDATA_Change,xDATA_ChangeSize);
			OSAPP_FileClose(iFileHandel);
		}
	}else
	{
		if(iFileSize==xDATA_ChangeSize)
		{
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveChange].aucFileName,O_READ);
			iFileErrNo = OSAPP_FileSeek(iFileHandel,0,SEEK_SET);
			if(!iFileErrNo)
				iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_Change,xDATA_ChangeSize);
			
		}
		else if(iFileSize)
		{
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveChange].aucFileName,O_READ);	
			OSAPP_FileTrunc(iFileHandel,0);
		}
		OSAPP_FileClose(iFileHandel);	
	}
	return(OSAPP_FileGetLastError());	
}

unsigned char FILE_CheckTransInfoData(void)
{

	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveTransInfo].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveTransInfo].aucFileName,O_CREATE);		
		if(iFileHandel>=0)
		{
			OSMEM_Memset(&xDATA_TransInfo,0,xDATA_TransInfoSize);
			OSAPP_FileClose(iFileHandel);
		}
	}else
	{
		if(iFileSize==xDATA_TransInfoSize)
		{
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveTransInfo].aucFileName,O_READ);
			iFileErrNo = OSAPP_FileSeek(iFileHandel,0,SEEK_SET);
			if(!iFileErrNo)
				iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_TransInfo,xDATA_TransInfoSize);
			
		}
		else if(iFileSize)
		{
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveTransInfo].aucFileName,O_READ);			
			OSAPP_FileTrunc(iFileHandel,0);
		}
		OSAPP_FileClose(iFileHandel);	
	}
	return(OSAPP_FileGetLastError());	
}

unsigned char FILE_CheckSaveISO8583(void)
{

	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveSaveISO8583].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveSaveISO8583].aucFileName,O_CREATE);		
		if(iFileHandel>=0)
		{
			OSMEM_Memset(&xDATA_SaveISO8583,0,xDATA_BackupISO8583Size);
			OSAPP_FileClose(iFileHandel);
		}
	}else
	{
		if(iFileSize==xDATA_BackupISO8583Size)
		{
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveSaveISO8583].aucFileName,O_READ);
			iFileErrNo = OSAPP_FileSeek(iFileHandel,0,SEEK_SET);
			if(!iFileErrNo)
				iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_SaveISO8583,xDATA_BackupISO8583Size);
			
		}
		else if(iFileSize)
		{
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveSaveISO8583].aucFileName,O_READ);			
			OSAPP_FileTrunc(iFileHandel,0);
		}
		OSAPP_FileClose(iFileHandel);	
	}
	return(OSAPP_FileGetLastError());	
}

unsigned char FILE_CheckSendISO8583(void)
{

	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveSendISO8583].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveSendISO8583].aucFileName,O_CREATE);		
		if(iFileHandel>=0)
		{
			OSMEM_Memset(&xDATA_SendISO8583,0,xDATA_BackupISO8583Size);
			OSAPP_FileClose(iFileHandel);
		}
	}else
	{
		if(iFileSize==xDATA_BackupISO8583Size)
		{
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveSendISO8583].aucFileName,O_READ);
			iFileErrNo = OSAPP_FileSeek(iFileHandel,0,SEEK_SET);
			if(!iFileErrNo)
				iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_SendISO8583,xDATA_BackupISO8583Size);
			
		}
		else if(iFileSize)
		{
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveSendISO8583].aucFileName,O_READ);			
			OSAPP_FileTrunc(iFileHandel,0);
		}
		OSAPP_FileClose(iFileHandel);	
	}
	return(OSAPP_FileGetLastError());	
}

unsigned char FILE_CheckReversalISO8583(void)
{

	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveReversalISO8583].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveReversalISO8583].aucFileName,O_CREATE);		
		if(iFileHandel>=0)
		{
			OSMEM_Memset(&xDATA_ReversalISO8583,0,xDATA_ReversalISO8583Size);
			OSAPP_FileClose(iFileHandel);
		}
	}else
	{
		if(iFileSize==xDATA_ReversalISO8583Size)
		{
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveReversalISO8583].aucFileName,O_READ);
			iFileErrNo = OSAPP_FileSeek(iFileHandel,0,SEEK_SET);
			if(!iFileErrNo)
				iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_ReversalISO8583,xDATA_ReversalISO8583Size);
			
		}
		else if(iFileSize)
		{
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveReversalISO8583].aucFileName,O_READ);			
			OSAPP_FileTrunc(iFileHandel,0);
		}
		OSAPP_FileClose(iFileHandel);	
	}
	return(OSAPP_FileGetLastError());	
}

unsigned char FILE_CheckBatchFlags(void)
{

	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveBatchFlags].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveBatchFlags].aucFileName,O_CREATE);		
		if(iFileHandel>=0)
		{
			OSMEM_Memset(DataSave1.ucBatchFlag,0,xDATA_BatchFlagsSize);
			OSAPP_FileClose(iFileHandel);
		}
	}else
	{
		if(iFileSize==xDATA_BatchFlagsSize)
		{
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveBatchFlags].aucFileName,O_READ);
			iFileErrNo = OSAPP_FileSeek(iFileHandel,0,SEEK_SET);
			if(!iFileErrNo)
				iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)DataSave1.ucBatchFlag,xDATA_BatchFlagsSize);
			
		}
		else if(iFileSize)
		{
			iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveBatchFlags].aucFileName,O_READ);			
			OSAPP_FileTrunc(iFileHandel,0);
		}
		OSAPP_FileClose(iFileHandel);	
	}
	return(OSAPP_FileGetLastError());	
}

unsigned char FILE_CheckBlackField(void)
{
	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveBlackFeilds].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveBlackFeilds].aucFileName,O_CREATE);
		if(iFileHandel>=0)
		{
			OSAPP_FileClose(iFileHandel);
		}
	}	
	return(OSAPP_FileGetLastError());	
}

unsigned char FILE_CheckSaveTrans(void)
{
	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveSaveTrans].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveSaveTrans].aucFileName,O_CREATE);
		if(iFileHandel>=0)
		{
			OSAPP_FileClose(iFileHandel);
		}
	}	
	return(OSAPP_FileGetLastError());	
}

unsigned char FILE_CheckZoneCtrls(void)
{
	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveZoneCtrls].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveZoneCtrls].aucFileName,O_CREATE);
		if(iFileHandel>=0)
		{
			OSAPP_FileClose(iFileHandel);
		}
	}	
	return(OSAPP_FileGetLastError());	
}

unsigned char FILE_CheckCardTables(void)
{
	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveCardTables].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveCardTables].aucFileName,O_CREATE);
		if(iFileHandel>=0)
		{
			OSAPP_FileClose(iFileHandel);
		}
	}	
	return(OSAPP_FileGetLastError());	
}
unsigned char FILE_CheckRateTables(void)
{
	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveRate].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveRate].aucFileName,O_CREATE);
		if(iFileHandel>=0)
		{
			OSAPP_FileClose(iFileHandel);
		}
	}	
	return(OSAPP_FileGetLastError());	
}
unsigned char FILE_CheckTemp(void)
{
	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveTemp].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveTemp].aucFileName,O_CREATE);
		if(iFileHandel>=0)
		{
			OSAPP_FileClose(iFileHandel);
		}
	}	
	return(OSAPP_FileGetLastError());	
}
unsigned char FILE_CheckIssueInfo(void)
{
	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveIssueInfo].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveIssueInfo].aucFileName,O_CREATE);
		if(iFileHandel>=0)
		{
			OSAPP_FileClose(iFileHandel);
		}
	}	
	return(OSAPP_FileGetLastError());	
}
unsigned char FILE_CheckLastIssueInfo(void)
{
	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveLastIssueInfo].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveLastIssueInfo].aucFileName,O_CREATE);
		if(iFileHandel>=0)
		{
			OSAPP_FileClose(iFileHandel);
		}
	}	
	return(OSAPP_FileGetLastError());	
}
unsigned char FILE_CheckExpInfo(void)
{
	int iFileSize;
	int iFileHandel;
	int iFileErrNo;
	if((iFileSize=OSAPP_FileSize((char *)FileInfo[DataSaveExpInfo].aucFileName))<0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveExpInfo].aucFileName,O_CREATE);
		if(iFileHandel>=0)
		{
			OSAPP_FileClose(iFileHandel);
		}
	}	
	return(OSAPP_FileGetLastError());	
}

