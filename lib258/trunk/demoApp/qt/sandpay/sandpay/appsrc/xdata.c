#include <include.h>
#include <global.h>

#define DATASAVE
#include <xdata.h>
#undef DATASAVE

FILEINFO const FileInfo[DataSaveMaxIndex]=
{
	{"constant"},
	{"change"},
	{"transinf"},
	{"saveiso"},
	{"sendiso"},
	{"reveriso"},
	{"savetran"},
  {"batchfla"},
  {"blackfei"},
  {"zonectrl"},
  {"cardtabl"},
  {"redo"},
  {"rate"},
  {"temp"},
  {"issinfo"},
  {"lstinfo"},
  {"expinfo"},
	{0}
};

unsigned char XDATA_Display_WriteFileErrMsg(unsigned char ucChar)
{
		 
        switch(ucChar)
        {
                case 0x00: 
			return SUCCESS;
                case 0x01: 
		        //Os__clr_display(255);
			//Os__GB2312_display(1,0,(uchar *)"ļ");
			break;
		  case 0x02:
		        //Os__clr_display(255);
		  	//Os__GB2312_display(1,0,(uchar *)"ļ");
			break;
                case 0x03: 
		        //Os__clr_display(255);
			//Os__GB2312_display(1,0,(uchar *)"ļʧ");
			break;
		  case 0x04:
		        //Os__clr_display(255);
		  	//Os__GB2312_display(1,0,(uchar *)"ļϵͳ");
			break;
		  case 0x05:
		        //Os__clr_display(255);
		  	//Os__GB2312_display(1,0,(uchar *)"ڴռ䲻");
			break;
		  case 0x0A:
		        //Os__clr_display(255);
		  	//Os__GB2312_display(1,0,(uchar *)"дļʧ");
			break;	
		  case 0x0B:
		        //Os__clr_display(255);
		  	//Os__GB2312_display(1,0,(uchar *)"ļ");
			break;
		  case 0x0C:
		        //Os__clr_display(255);
		  	//Os__GB2312_display(1,0,(uchar *)"ļδ");
			break;	
          case 0x0D:
		        //Os__clr_display(255);
            //Os__GB2312_display(1,0,(uchar *)"文件偏移错误");
			break;
		  case 0x0E:
		        //Os__clr_display(255);
		  	//Os__GB2312_display(1,0,(uchar *)"ļԴ");
			break;	
		  case 0x0F:
		        //Os__clr_display(255);
		  	//Os__GB2312_display(1,0,(uchar *)"ļ");
			break;
		  default :	
		        //Os__clr_display(255);
		       //Os__GB2312_display(1,0,(uchar *)"ļϵͳ");
			break;
        }

	Uart_Printf("\n  XDATA_Display_WriteFileErrMsg:%02X-----------\n",ucChar);
	//MSG_WaitKey(3);
	return ERR_CANCEL;
}

unsigned char XDATA_ClearAllFile(void)
{
	int iFileHandel;
	int iFileErrNo;
	int i;
	unsigned char ucResult;
	
	for(i=0;i<DataSaveMaxIndex;i++)
	{
		if( FileInfo[i].aucFileName != NULL )
		{
			if(OSAPP_FileSize((char *)FileInfo[i].aucFileName)>0)
			{
				iFileHandel = OSAPP_OpenFile((char *)FileInfo[i].aucFileName,O_READ);
				OSAPP_FileTrunc(iFileHandel,0);
				OSAPP_FileClose(iFileHandel);
			}
		}
		else
		{
			break;
		}
	}	
	ucResult = OSAPP_FileGetLastError();
	return(ucResult);
}



unsigned char XDATA_Read_Vaild_File(unsigned char ucFileIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[ucFileIndex].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName,O_READ);
		iFileErrNo = OSAPP_FileSeek(iFileHandel,0,SEEK_SET);
		if(!iFileErrNo)
		{
			switch(ucFileIndex)
			{
				case DataSaveConstant:
					iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_Constant,xDATA_ConstantSize);
					break;
				case DataSaveChange:
					iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_Change,xDATA_ChangeSize);
					break;
				case DataSaveTransInfo:
					iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_TransInfo,xDATA_TransInfoSize);
					break;
				case DataSaveSaveISO8583:
					iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_SaveISO8583,xDATA_BackupISO8583Size);
					break; 
				case DataSaveSendISO8583:
					iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_SendISO8583,xDATA_BackupISO8583Size);
					break; 
				case DataSaveReversalISO8583:
					iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_ReversalISO8583,xDATA_ReversalISO8583Size);
					break; 
				case DataSaveBatchFlags:
					iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)DataSave1.ucBatchFlag,xDATA_BatchFlagsSize);
					break;	
				case DataSaveRedoTrans:
					iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&DataSave0.RedoTrans,xDATA_RedoTransSize);
					break;	
				case DataSaveExpInfo:
					iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&ExpTransData,xDATA_ExpSize);
					break;	
				default:
				        OSAPP_FileClose(iFileHandel);
					return(FILE_NOEXIST);
					break;
			}
		}	    
		OSAPP_FileClose(iFileHandel);
	}
	ucResult = OSAPP_FileGetLastError();    
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}


unsigned char XDATA_Write_Vaild_File(unsigned char ucFileIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;
	unsigned char *Ptr;

	int iFileErrNoBak;
    Uart_Printf("aaaaaaaa111111111111\n");

	iFileHandel = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName,O_WRITE);
	Uart_Printf("iFileHandel = %d\n",iFileHandel);
	iFileErrNo = OSAPP_FileSeek(iFileHandel,0,SEEK_SET);
	Uart_Printf("iFileErrNo = %d\n",iFileErrNo);
    Uart_Printf("&&&&&&&& = %d\n",ucFileIndex);


	if(!iFileErrNo)
	{
		switch(ucFileIndex)
		{
			case DataSaveConstant:
				iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&xDATA_Constant,xDATA_ConstantSize);
				break;
			case DataSaveChange:
				iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&xDATA_Change,xDATA_ChangeSize);
				break;
			case DataSaveTransInfo:
				iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&xDATA_TransInfo,xDATA_TransInfoSize);
				break;
			case DataSaveSaveISO8583:
				iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&xDATA_SaveISO8583,xDATA_BackupISO8583Size);
				break;	
			case DataSaveSendISO8583:
			        iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&xDATA_SendISO8583,xDATA_BackupISO8583Size);
				break; 
			case DataSaveReversalISO8583:
			        iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&xDATA_ReversalISO8583,xDATA_ReversalISO8583Size);
				break; 	
		  case DataSaveBatchFlags:
			        iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)DataSave1.ucBatchFlag,xDATA_BatchFlagsSize);
				break; 	
			case DataSaveRedoTrans:
				iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&DataSave0.RedoTrans,xDATA_RedoTransSize);
				break;
			case DataSaveTemp:
				Ptr = (uchar *)malloc(4000);
				memset(Ptr,0x31,1024);
				iFileErrNo = OSAPP_FileWrite(iFileHandel,Ptr,xDATA_TempSize);
				free(Ptr);
				Uart_Printf("iFileErrNo = %d\n",iFileErrNo);
				break;
			case DataSaveExpInfo:
				iFileErrNo = OSAPP_FileWrite(iFileHandel,&ExpTransData.ExpFeeData[0].ucPt,xDATA_ExpSize);
				break;
			default:
			        OSAPP_FileClose(iFileHandel);
				return(FILE_NOEXIST);
				break;
		}
	}	    
	iFileErrNoBak = OSAPP_FileClose(iFileHandel);
	Uart_Printf("OSAPP_FileClose(iFileHandel) = %d\n",iFileErrNoBak);
	ucResult = OSAPP_FileGetLastError();
	Uart_Printf("ucResult = %02x\n",ucResult);
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}

unsigned char XDATA_Write_SaveTrans_File(unsigned short uiIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;
	iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveSaveTrans].aucFileName,O_WRITE);
	iFileErrNo = OSAPP_FileSeek(iFileHandel,uiIndex*xDATA_SaveTransSize,SEEK_SET);	
	if(!iFileErrNo)
	{
		iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&xDATA_SaveTrans,xDATA_SaveTransSize);
	}
	OSAPP_FileClose(iFileHandel);
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));

}

unsigned char XDATA_Read_SaveTrans_File(unsigned short uiIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;

//    unsigned char *p;

    Uart_Printf("&&&&&&&&&&&&&&uiIndex =======%d \n\n",uiIndex);
	if(OSAPP_FileSize((char *)FileInfo[DataSaveSaveTrans].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveSaveTrans].aucFileName,O_READ);
		iFileErrNo = OSAPP_FileSeek(iFileHandel,uiIndex*xDATA_SaveTransSize,SEEK_SET);

        Uart_Printf("***************iFileHandel =======%d  iFileErrNo ====%d\n\n",iFileHandel,iFileErrNo);

        if(!iFileErrNo){
			iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&NormalTransData,xDATA_SaveTransSize);
/*
            Uart_Printf("iFileErrNo =======%d \n\n",iFileErrNo);
            p = (unsigned char *)&NormalTransData;
            int i = 0;
            for(i= 0;i<iFileErrNo;i++)
            {
                Uart_Printf("%02X ", p[i]);
                if(((i+1) % 16) == 0)
                {
                    Uart_Printf("\n");
                }
            }
            Uart_Printf("\nNormalTransData end\n\n");
*/
        }

		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}

unsigned char XDATA_Read_SaveTrans_File_2Datasave(unsigned short uiIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveSaveTrans].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveSaveTrans].aucFileName,O_READ);
		iFileErrNo = OSAPP_FileSeek(iFileHandel,uiIndex*xDATA_SaveTransSize,SEEK_SET);
		if(!iFileErrNo)
			iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&DataSave0.SaveTransData,xDATA_SaveTransSize);
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}

unsigned char XDATA_Clear_SaveTrans_File(void)
{
	int iFileHandel;
	int iFileSize;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveSaveTrans].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveSaveTrans].aucFileName,O_WRITE);
		OSAPP_FileTrunc(iFileHandel,0);
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}


unsigned char XDATA_Write_ZoneCtrls_File(unsigned short uiIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;
	iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveZoneCtrls].aucFileName,O_WRITE);
	iFileErrNo = OSAPP_FileSeek(iFileHandel,uiIndex*xDATA_ZoneCtrlsSize,SEEK_SET);	
	if(!iFileErrNo)
	{
		iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&xDATA_ZoneCtrls,xDATA_ZoneCtrlsSize);
	}
	OSAPP_FileClose(iFileHandel);
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));

}

unsigned char XDATA_Read_ZoneCtrls_File(unsigned short uiIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveZoneCtrls].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveZoneCtrls].aucFileName,O_READ);
		iFileErrNo = OSAPP_FileSeek(iFileHandel,uiIndex*xDATA_ZoneCtrlsSize,SEEK_SET);
		if(!iFileErrNo)
			iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_ZoneCtrls,xDATA_ZoneCtrlsSize);
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}

unsigned char XDATA_Clear_ZoneCtrls_File(void)
{
	int iFileHandel;
	int iFileSize;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveZoneCtrls].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveZoneCtrls].aucFileName,O_WRITE);
		OSAPP_FileTrunc(iFileHandel,0);
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}

unsigned char XDATA_Write_CardTables_File(unsigned short uiIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;
	iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveCardTables].aucFileName,O_WRITE);
	iFileErrNo = OSAPP_FileSeek(iFileHandel,uiIndex*xDATA_CardTablesSize,SEEK_SET);	
	if(!iFileErrNo)
	{
		iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&xDATA_CardTables,xDATA_CardTablesSize);
	}
	OSAPP_FileClose(iFileHandel);
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}

unsigned char XDATA_Read_CardTables_File(unsigned short uiIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveCardTables].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveCardTables].aucFileName,O_READ);
		iFileErrNo = OSAPP_FileSeek(iFileHandel,uiIndex*xDATA_CardTablesSize,SEEK_SET);
		if(!iFileErrNo)
			iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_CardTables,xDATA_CardTablesSize);
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}

unsigned char XDATA_Clear_CardTables_File(void)
{
	int iFileHandel;
	int iFileSize;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveCardTables].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveCardTables].aucFileName,O_WRITE);
		OSAPP_FileTrunc(iFileHandel,0);
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}
unsigned char XDATA_Read_Rate_File(unsigned short uiIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveRate].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveRate].aucFileName,O_READ);
		iFileErrNo = OSAPP_FileSeek(iFileHandel,uiIndex*xDATA_RateSize,SEEK_SET);
		if(!iFileErrNo)
			iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_Rate,xDATA_RateSize);
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}

unsigned char XDATA_Clear_Rate_File(void)
{
	int iFileHandel;
	int iFileSize;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveRate].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveRate].aucFileName,O_WRITE);
		OSAPP_FileTrunc(iFileHandel,0);
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}

unsigned char XDATA_Read_Black_File(unsigned short uiIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveBlackFeilds].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveBlackFeilds].aucFileName,O_READ);
		iFileErrNo = OSAPP_FileSeek(iFileHandel,uiIndex*xDATA_BlackFeildsSize,SEEK_SET);
		if(!iFileErrNo)
			iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&xDATA_Black,xDATA_BlackFeildsSize);
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}

unsigned char XDATA_Clear_Black_File(void)
{
	int iFileHandel;
	int iFileSize;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveBlackFeilds].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveBlackFeilds].aucFileName,O_WRITE);
		OSAPP_FileTrunc(iFileHandel,0);
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}
unsigned char XDATA_Clear_Temp_File(void)
{
	int iFileHandel;
	int iFileSize;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveTemp].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveTemp].aucFileName,O_WRITE);
		OSAPP_FileTrunc(iFileHandel,0);
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult=OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}
unsigned char XDATA_Write_IssueInfo_File(unsigned short uiIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;
	iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveIssueInfo].aucFileName,O_WRITE);
	iFileErrNo = OSAPP_FileSeek(iFileHandel,uiIndex*sizeof(ISSUETRANSINFO),SEEK_SET);	
	if(!iFileErrNo)
	{
		iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&IssueInfo,sizeof(ISSUETRANSINFO));
	}
	OSAPP_FileClose(iFileHandel);
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));

}

unsigned char XDATA_Read_IssueInfo_File(unsigned short uiIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveIssueInfo].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveIssueInfo].aucFileName,O_READ);
		iFileErrNo = OSAPP_FileSeek(iFileHandel,uiIndex*sizeof(ISSUETRANSINFO),SEEK_SET);
		if(!iFileErrNo)
			iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&IssueInfo,sizeof(ISSUETRANSINFO));
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}

unsigned char XDATA_Clear_IssueInfo_File(void)
{
	int iFileHandel;
	int iFileSize;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveIssueInfo].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveIssueInfo].aucFileName,O_WRITE);
		OSAPP_FileTrunc(iFileHandel,0);
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return ucResult;
}
unsigned char XDATA_Write_LastIssueInfo_File(unsigned short uiIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;
	iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveLastIssueInfo].aucFileName,O_WRITE);
	iFileErrNo = OSAPP_FileSeek(iFileHandel,uiIndex*sizeof(ISSUETRANSINFO),SEEK_SET);	
	if(!iFileErrNo)
	{
		iFileErrNo = OSAPP_FileWrite(iFileHandel,(unsigned char *)&IssueInfo,sizeof(ISSUETRANSINFO));
	}
	OSAPP_FileClose(iFileHandel);
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));

}
unsigned char XDATA_Read_LastIssueInfo_File(unsigned short uiIndex)
{
	int iFileHandel;
	int iFileErrNo;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveLastIssueInfo].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveLastIssueInfo].aucFileName,O_READ);
		iFileErrNo = OSAPP_FileSeek(iFileHandel,uiIndex*sizeof(ISSUETRANSINFO),SEEK_SET);
		if(!iFileErrNo)
			iFileErrNo = OSAPP_FileRead(iFileHandel,(unsigned char *)&IssueInfo,sizeof(ISSUETRANSINFO));
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return(XDATA_Display_WriteFileErrMsg(ucResult));
}

unsigned char XDATA_Clear_LastIssueInfo_File(void)
{
	int iFileHandel;
	int iFileSize;
	unsigned char ucResult;
	if(OSAPP_FileSize((char *)FileInfo[DataSaveLastIssueInfo].aucFileName)>0)
	{
		iFileHandel = OSAPP_OpenFile((char *)FileInfo[DataSaveLastIssueInfo].aucFileName,O_WRITE);
		OSAPP_FileTrunc(iFileHandel,0);
		OSAPP_FileClose(iFileHandel);
	}	
	ucResult = OSAPP_FileGetLastError();
	return ucResult;
}
