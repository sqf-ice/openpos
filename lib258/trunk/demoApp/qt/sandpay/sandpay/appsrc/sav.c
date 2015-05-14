#include <include.h>
#include <global.h>
#include <xdata.h>


unsigned char SAV_SavedTransIndex(unsigned char ucFlag)
{
	unsigned short	uiIndex;
	unsigned char	ucResult=SUCCESS,ucTransType,ucHandFlag;
	unsigned long ulOldTraceNumber,ulTraceNumber;
	ucResult=XDATA_Write_Vaild_File(DataSaveTemp);
	Uart_Printf("ucResult(%d) = %02x\n",__LINE__,ucResult);
	XDATA_Clear_Temp_File();
	if(ucResult!=SUCCESS)
		return ucResult;
	switch( ucFlag )
	{
	case 0:
		for(uiIndex=0;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if( DataSave0.TransInfoData.auiTransIndex[uiIndex] == 0x00)
				break;
		}
		if( uiIndex == TRANS_MAXNB)
		{
			return(ERR_TRANSFILEFULL);
		}
		RUNDATA_uiTransIndex = uiIndex;
		break;
	case 1:
		ucTransType = NORMALTRANS_ucTransType;
		ulTraceNumber = NORMALTRANS_ulTraceNumber;
		ulOldTraceNumber=RunData.ulOldTraceNumber;
		ucHandFlag=NormalTransData.ucHandFlag;
		for(uiIndex=0;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if( DataSave0.TransInfoData.auiTransIndex[uiIndex])
			{
				ucResult=XDATA_Read_SaveTrans_File_2Datasave(uiIndex);
				if(ucResult!=SUCCESS)
					return ucResult;
				if(ulOldTraceNumber != DataSave0.SaveTransData.ulTraceNumber)
				{
					continue;
				}
				if(NORMALTRANS_ucTransType!= TRANS_S_VOIDREFAPL)
				{
					if( memcmp(DataSave0.SaveTransData.aucUserCardNo,NormalTransData.aucUserCardNo,10))
						return (ERR_NOTPROC);
				}
				memcpy(&NormalTransData,&DataSave0.SaveTransData,sizeof(NORMALTRANS));
				
				if(NORMALTRANS_ucTransType == TRANS_S_ONLINEPURCHASE)
				{
					if(ucTransType!=TRANS_S_VOID)
						return (ERR_NOTPROC);
				}
				else
				if(NORMALTRANS_ucTransType == TRANS_S_ONLINEPURCHASE_P)
				{
					if(ucTransType!=TRANS_S_VOID_P)
						return (ERR_NOTPROC);
				}
				else
				if(NORMALTRANS_ucTransType == TRANS_S_PREAUTH)
				{
					if(ucTransType!=TRANS_S_PREAUTHVOID)
						return (ERR_NOTPROC);
				}
				else
				if(NORMALTRANS_ucTransType == TRANS_S_PREAUTHFINISH)
				{
					if(ucTransType!=TRANS_S_PREAUTHFINISHVOID)
						return (ERR_NOTPROC);
				}
				else
				if(NORMALTRANS_ucTransType == TRANS_S_REFAPL)
				{
					if(ucTransType!=TRANS_S_VOIDREFAPL)
						return (ERR_NOTPROC);
				}
				else
				if(NORMALTRANS_ucTransType == TRANS_S_BARCODE)
				{
					continue;
				}
				else
				{
					return (ERR_NOTPROC);

				}
				
				RUNDATA_uiOldTransIndex = uiIndex;
				break;
			}
		}
		if(uiIndex == TRANS_MAXNB)
		{
			return(ERR_NOTRANS);
		}
		for(uiIndex=0;uiIndex<TRANS_MAXNB;uiIndex++)
		{
			if( DataSave0.TransInfoData.auiTransIndex[uiIndex] == 0)
				break;
		}
		if( uiIndex == TRANS_MAXNB)
		{
			return(ERR_TRANSFILEFULL);
		}
		RUNDATA_uiTransIndex = uiIndex;

		if(DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiOldTransIndex] == 0xffff)
		{
			return(ERR_ALREADYVOID);
		}	
		NORMALTRANS_ucTransType = ucTransType;
		NORMALTRANS_ulTraceNumber = ulTraceNumber;
		long_asc(NormalTransData.aucOldTrace,6,&ulOldTraceNumber);
		NormalTransData.ucHandFlag=ucHandFlag;
		break;
	default:
		break;
	}
	return SUCCESS;
}
unsigned char SAV_TotalDomain(uchar ucTransType)
{
	int i=0;
	for(i=0;i<50;i++)
	{
		if(DataSave0.TransInfoData.aucTotalIndex[i]==0)
			break;
		if(DataSave0.TransInfoData.aucTotalIndex[i]==ucTransType)
			return SUCCESS;
	}
	DataSave0.TransInfoData.aucTotalIndex[i]=ucTransType;
	return XDATA_Write_Vaild_File(DataSaveTransInfo); 
}

unsigned char SAV_IssueInfo(uchar aucIssueName[16])
{
	int i=0;
	unsigned char ucResult=SUCCESS,uct;
	if(NormalTransData.ucKYFlag)
		return SUCCESS;
	uct=NormalTransData.ucTransType;
	if(NormalTransData.ucTransType==TRANS_S_VOID)
	{
		NormalTransData.ucTransType=TRANS_S_ONLINEPURCHASE;
	}
	if(NormalTransData.ucTransType==TRANS_S_VOID_P)
	{
		NormalTransData.ucTransType=TRANS_S_ONLINEPURCHASE_P;
	}
	if(NormalTransData.ucTransType==TRANS_S_PREAUTHFINISHVOID)
	{
		NormalTransData.ucTransType=TRANS_S_PREAUTHFINISH;
	}
	if(NormalTransData.ucTransType==TRANS_S_VOIDPAYONLINE)
	{
		NormalTransData.ucTransType=TRANS_S_PAYONLINE;
	}
	while(1)
	{
		memset(&IssueInfo,0,sizeof(ISSUETRANSINFO));
		ucResult=XDATA_Read_IssueInfo_File(i);
		Uart_Printf("SAV_IssueInfo i=%d,ucResult=%02x\n",i,ucResult);
		if(ucResult==SUCCESS)
		{
			if(IssueInfo.ucTranstype==0)
				break;
			if(IssueInfo.ucTranstype==NormalTransData.ucTransType
			&&IssueInfo.ucPtcode==NormalTransData.ucPtCode
			&&memcmp(IssueInfo.aucIssueName,aucIssueName,16)==0)
				break;
		}
		else
			break;
		i++;
	}
	IssueInfo.ucTranstype=NormalTransData.ucTransType;
	memcpy(IssueInfo.aucIssueName,aucIssueName,16);
	IssueInfo.ucPtcode=NormalTransData.ucPtCode;
	NormalTransData.ucTransType=uct;
	if(NormalTransData.ucTransType==TRANS_S_VOID
	||NormalTransData.ucTransType==TRANS_S_VOID_P
	||NormalTransData.ucTransType==TRANS_S_PREAUTHFINISHVOID
	||NormalTransData.ucTransType==TRANS_S_VOIDPAYONLINE
	)
	{
		IssueInfo.uiTransNb--;
		IssueInfo.ulTransAmount-=NormalTransData.ulAmount;

	}
	else
	{
		IssueInfo.uiTransNb++;
		IssueInfo.ulTransAmount+=NormalTransData.ulAmount;
		if(NormalTransData.ulAmount>NormalTransData.ulPrevEDBalance)
			IssueInfo.ulYJAmount=IssueInfo.ulYJAmount+NormalTransData.ulAmount-NormalTransData.ulPrevEDBalance;
	}
	return XDATA_Write_IssueInfo_File(i); 
}

unsigned char SAV_PurchaseSave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	if(NormalTransData.ucReturnflag==true)
		NormalTransData.ulAfterEDBalance=0;
	else
		NormalTransData.ulAfterEDBalance=NormalTransData.ulPrevEDBalance-NormalTransData.ulAmount;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
    Uart_Printf("@@@@@@@@@@@@@@@ucResult===%02x\n",ucResult);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	if(NormalTransData.ucKYFlag==0)
	{
		if(NormalTransData.ucReturnflag==true)
		{
			uiTotalNb=pTransTotal->uiReturnNb+1;
			ulTotalAmount=pTransTotal->ulReturnAmount+NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
			       	       (unsigned char *)&pTransTotal->uiReturnNb,
				       sizeof(unsigned int));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
			       	       (unsigned char *)&pTransTotal->ulReturnAmount,
				       sizeof(unsigned long));    		
			DataSave0.TransInfoData.TransTotal.ulYJAmount=DataSave0.TransInfoData.TransTotal.ulYJAmount+NormalTransData.ulAmount-NormalTransData.ulPrevEDBalance;
			DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiReturnNb++;
			DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulReturnAmount+=NormalTransData.ulAmount;
			DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulYJAmount=DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulYJAmount
				+NormalTransData.ulAmount-NormalTransData.ulPrevEDBalance;

		}
		else
		{
			if(NormalTransData.ucTransType==TRANS_S_TRANSPURCHASE)
			{
				uiTotalNb=pTransTotal->uiTransferPurchaseNb+1;
				ulTotalAmount=pTransTotal->ulTransferPurchaseAmount+NORMALTRANS_ulAmount;
				Os__saved_copy((unsigned char *)&uiTotalNb,
				       	       (unsigned char *)&pTransTotal->uiTransferPurchaseNb,
					       sizeof(unsigned int));
				Os__saved_copy((unsigned char *)&ulTotalAmount,
				       	       (unsigned char *)&pTransTotal->ulTransferPurchaseAmount,
					       sizeof(unsigned long));    		
				DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiTransferPurchaseNb++;
				DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulTransferPurchaseAmount+=NormalTransData.ulAmount;

			}
			else
			{

				uiTotalNb=pTransTotal->uiPurchaseNb+1;
				ulTotalAmount=pTransTotal->ulPurchaseAmount+NORMALTRANS_ulAmount;
				Os__saved_copy((unsigned char *)&uiTotalNb,
				       	       (unsigned char *)&pTransTotal->uiPurchaseNb,
					       sizeof(unsigned int));
				Os__saved_copy((unsigned char *)&ulTotalAmount,
				       	       (unsigned char *)&pTransTotal->ulPurchaseAmount,
					       sizeof(unsigned long));    		
				DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiPurchaseNb++;
				DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulPurchaseAmount+=NormalTransData.ulAmount;
			}

	       }
		Os__saved_copy((unsigned char *)&uiAllTotalNb,
			       	       (unsigned char *)&pTransTotal->uiTotalNb,
				       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulAllTtAmount,
			       	       (unsigned char *)&pTransTotal->ulTotalAmount,
				       sizeof(unsigned long));	
	
	}
    Uart_Printf("&pTransTotal->uiTotalNb******************************%d\n",&pTransTotal->uiTotalNb);
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	DataSave0.RedoTrans.ucRedoFlag=0;
	XDATA_Write_Vaild_File(DataSaveRedoTrans);

	return SUCCESS;
}

unsigned char SAV_PayNobarSave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	if(NormalTransData.ucReturnflag==true)
		NormalTransData.ulAfterEDBalance=0;
	else
		NormalTransData.ulAfterEDBalance=NormalTransData.ulPrevEDBalance-NormalTransData.ulAmount;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	if(NormalTransData.ucKYFlag==0)
	{
		uiTotalNb=pTransTotal->uiPayNobarNb+1;
		ulTotalAmount=pTransTotal->ulPayNobarAmount+NORMALTRANS_ulAmount;
		Os__saved_copy((unsigned char *)&uiTotalNb,
		       	       (unsigned char *)&pTransTotal->uiPayNobarNb,
			       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulTotalAmount,
		       	       (unsigned char *)&pTransTotal->ulPayNobarAmount,
			       sizeof(unsigned long));    		
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiPayNobarNb++;
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulPayNobarAmount+=NormalTransData.ulAmount;

		Os__saved_copy((unsigned char *)&uiAllTotalNb,
			       	       (unsigned char *)&pTransTotal->uiTotalNb,
				       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulAllTtAmount,
			       	       (unsigned char *)&pTransTotal->ulTotalAmount,
				       sizeof(unsigned long));	
	
	}
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	DataSave0.RedoTrans.ucRedoFlag=0;
	XDATA_Write_Vaild_File(DataSaveRedoTrans);

	return SUCCESS;
}

unsigned char SAV_PurchaseSave_P(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	if(NormalTransData.ucReturnflag==true)
		NormalTransData.ulAfterEDBalance=0;
	else
		NormalTransData.ulAfterEDBalance=NormalTransData.ulPrevEDBalance-NormalTransData.ulAmount;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	if(NormalTransData.ucKYFlag==0)
	{
		if(NormalTransData.ucReturnflag==true)
		{
			uiTotalNb=pTransTotal->uiReturnpNb+1;
			ulTotalAmount=pTransTotal->ulReturnpAmount+NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
			       	       (unsigned char *)&pTransTotal->uiReturnpNb,
				       sizeof(unsigned int));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
			       	       (unsigned char *)&pTransTotal->ulReturnpAmount,
				       sizeof(unsigned long));    		
			DataSave0.TransInfoData.TransTotal.ulYJpAmount=DataSave0.TransInfoData.TransTotal.ulYJpAmount+NormalTransData.ulAmount-NormalTransData.ulPrevEDBalance;
			DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiReturnpNb++;
			DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulReturnpAmount+=NormalTransData.ulAmount;
			DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulYJpAmount=DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulYJpAmount
				+NormalTransData.ulAmount-NormalTransData.ulPrevEDBalance;

		}
		else
		{
			uiTotalNb=pTransTotal->uiPurchasepNb+1;
			ulTotalAmount=pTransTotal->ulPurchasepAmount+NORMALTRANS_ulAmount;
			Os__saved_copy((unsigned char *)&uiTotalNb,
			       	       (unsigned char *)&pTransTotal->uiPurchasepNb,
				       sizeof(unsigned int));
			Os__saved_copy((unsigned char *)&ulTotalAmount,
			       	       (unsigned char *)&pTransTotal->ulPurchasepAmount,
				       sizeof(unsigned long));    		
			DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiPurchasepNb++;
			DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulPurchasepAmount+=NormalTransData.ulAmount;

	       }
		Os__saved_copy((unsigned char *)&uiAllTotalNb,
			       	       (unsigned char *)&pTransTotal->uiTotalNb,
				       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulAllTtAmount,
			       	       (unsigned char *)&pTransTotal->ulTotalAmount,
				       sizeof(unsigned long));	
	
	}
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	DataSave0.RedoTrans.ucRedoFlag=0;
	XDATA_Write_Vaild_File(DataSaveRedoTrans);

	return SUCCESS;
}

unsigned char SAV_PTCPurchaseSave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	if(NormalTransData.ucReturnflag==true)
		NormalTransData.ulAfterEDBalance=0;
	else
		NormalTransData.ulAfterEDBalance=NormalTransData.ulPrevEDBalance-NormalTransData.ulAmount;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	if(NormalTransData.ucKYFlag==0)
	{
		uiTotalNb=pTransTotal->uiPTCPurchaseNb+1;
		ulTotalAmount=pTransTotal->ulPTCPurchaseAmount+NORMALTRANS_ulAmount;
		Os__saved_copy((unsigned char *)&uiTotalNb,
		       	       (unsigned char *)&pTransTotal->uiPTCPurchaseNb,
			       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulTotalAmount,
		       	       (unsigned char *)&pTransTotal->ulPTCPurchaseAmount,
			       sizeof(unsigned long));    		
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiPTCPurchaseNb++;
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulPTCPurchaseAmount+=NormalTransData.ulAmount;

		Os__saved_copy((unsigned char *)&uiAllTotalNb,
			       	       (unsigned char *)&pTransTotal->uiTotalNb,
				       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulAllTtAmount,
			       	       (unsigned char *)&pTransTotal->ulTotalAmount,
				       sizeof(unsigned long));	
	
	}
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	DataSave0.RedoTrans.ucRedoFlag=0;
	XDATA_Write_Vaild_File(DataSaveRedoTrans);

	return SUCCESS;
}


unsigned char SAV_ZshPurchaseSave(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	if(NormalTransData.ucReturnflag==true)
		NormalTransData.ulAfterEDBalance=0;
	else
		NormalTransData.ulAfterEDBalance=NormalTransData.ulPrevEDBalance-NormalTransData.ulAmount;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;

	if(NormalTransData.ucKYFlag==0)
	{
			
		pTransTotal=&DataSave0.TransInfoData.TransTotal;
		uiAllTotalNb=pTransTotal->uiTotalNb;
		uiAllTotalNb++;
		ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
		
		uiTotalNb=pTransTotal->uiZshPurchaseNb+1;
		ulTotalAmount=pTransTotal->ulZshPurchaseAmount+NORMALTRANS_ulAmount;
		Os__saved_copy((unsigned char *)&uiTotalNb,
		       	       (unsigned char *)&pTransTotal->uiZshPurchaseNb,
			       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulTotalAmount,
		       	       (unsigned char *)&pTransTotal->ulZshPurchaseAmount,
			       sizeof(unsigned long));    		
		DataSave0.TransInfoData.TransTotal.ulZshPurchaseRate+=NormalTransData.ulRateAmount;
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiZshPurchaseNb++;
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulZshPurchaseAmount+=NormalTransData.ulAmount;
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulZshPurchaseRate+=NormalTransData.ulRateAmount;


		Os__saved_copy((unsigned char *)&uiAllTotalNb,
			       	       (unsigned char *)&pTransTotal->uiTotalNb,
				       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulAllTtAmount,
			       	       (unsigned char *)&pTransTotal->ulTotalAmount,
				       sizeof(unsigned long));	
	}
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	DataSave0.RedoTrans.ucRedoFlag=0;
	XDATA_Write_Vaild_File(DataSaveRedoTrans);
	
	return ucResult;
}



unsigned char SAV_ExpPurchaseSave(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	if(NormalTransData.ucReturnflag==true)
		NormalTransData.ulAfterEDBalance=0;
	else
		NormalTransData.ulAfterEDBalance=NormalTransData.ulPrevEDBalance-NormalTransData.ulAmount;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;

	if(NormalTransData.ucKYFlag==0)
	{
			
		pTransTotal=&DataSave0.TransInfoData.TransTotal;
		uiAllTotalNb=pTransTotal->uiTotalNb;
		uiAllTotalNb++;
		ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
		
		uiTotalNb=pTransTotal->uiExpPurchaseNb+1;
		ulTotalAmount=pTransTotal->ulExpPurchaseAmount+NORMALTRANS_ulAmount;
		Os__saved_copy((unsigned char *)&uiTotalNb,
		       	       (unsigned char *)&pTransTotal->uiExpPurchaseNb,
			       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulTotalAmount,
		       	       (unsigned char *)&pTransTotal->ulExpPurchaseAmount,
			       sizeof(unsigned long));    		
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiExpPurchaseNb++;
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulExpPurchaseAmount+=NormalTransData.ulAmount;


		Os__saved_copy((unsigned char *)&uiAllTotalNb,
			       	       (unsigned char *)&pTransTotal->uiTotalNb,
				       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulAllTtAmount,
			       	       (unsigned char *)&pTransTotal->ulTotalAmount,
				       sizeof(unsigned long));	
	}
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	DataSave0.RedoTrans.ucRedoFlag=0;
	XDATA_Write_Vaild_File(DataSaveRedoTrans);
	
	return ucResult;
}

unsigned char SAV_OnlinePurchaseSave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiOnlinePurchaseNb+1;
	ulTotalAmount=pTransTotal->ulOnlinePurchaseAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiOnlinePurchaseNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulOnlinePurchaseAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiOnlinePurchaseNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulOnlinePurchaseAmount+=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));				  	    
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName1)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;

	return SUCCESS;
}



unsigned char SAV_OnlinePurchaseSave_P(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiOnlinePurchasepNb+1;
	ulTotalAmount=pTransTotal->ulOnlinePurchasepAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiOnlinePurchasepNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulOnlinePurchasepAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiOnlinePurchasepNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulOnlinePurchasepAmount+=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));				  	    
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName1)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;

	return SUCCESS;
}


unsigned char SAV_ZshOnlinePurchaseSave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiZshOnlinePurchaseNb+1;
	ulTotalAmount=pTransTotal->ulZshOnlinePurchaseAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiZshOnlinePurchaseNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulZshOnlinePurchaseAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.TransTotal.ulZshOnlinePurchaseRate+=NormalTransData.ulRateAmount;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiZshOnlinePurchaseNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulZshOnlinePurchaseAmount+=NormalTransData.ulAmount;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulZshOnlinePurchaseRate+=NormalTransData.ulRateAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));				  	    
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName1)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;

	return SUCCESS;
}


unsigned char SAV_JsZshOnlinePurchaseSave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiJsZshOnlinePurchaseNb+1;
	ulTotalAmount=pTransTotal->ulJsZshOnlinePurchaseAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiJsZshOnlinePurchaseNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulJsZshOnlinePurchaseAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.TransTotal.ulJsZshOnlinePurchaseRate+=NormalTransData.ulRateAmount;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiJsZshOnlinePurchaseNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulJsZshOnlinePurchaseAmount+=NormalTransData.ulAmount;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulJsZshOnlinePurchaseRate+=NormalTransData.ulRateAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));				  	    
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName1)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;

	return SUCCESS;
}

unsigned char SAV_PaySave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned short   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	NormalTransData.ulAfterEDBalance=NormalTransData.ulPrevEDBalance-NormalTransData.ulAmount;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	if(NormalTransData.ucKYFlag==0)
	{
		uiTotalNb=pTransTotal->uiPayNb+1;
		ulTotalAmount=pTransTotal->ulPayAmount+NORMALTRANS_ulAmount;
		Os__saved_copy((unsigned char *)&uiTotalNb,
		       	       (unsigned char *)&pTransTotal->uiPayNb,
			       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulTotalAmount,
		       	       (unsigned char *)&pTransTotal->ulPayAmount,
			       sizeof(unsigned long));    		
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiPayNb++;
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulPayAmount+=NormalTransData.ulAmount;

		Os__saved_copy((unsigned char *)&uiAllTotalNb,
			       	       (unsigned char *)&pTransTotal->uiTotalNb,
				       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulAllTtAmount,
			       	       (unsigned char *)&pTransTotal->ulTotalAmount,
				       sizeof(unsigned long));	
	}
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	DataSave0.RedoTrans.ucRedoFlag=0;
	XDATA_Write_Vaild_File(DataSaveRedoTrans);
	for(index=0;index<TRANS_MAXNB;index++)
	{
		if(DataSave0.TransInfoData.auiTransIndex[index]!=0x01)
			continue;
		XDATA_Read_SaveTrans_File_2Datasave(index);
		if(NormalTransData.ulTraceNumber==DataSave0.SaveTransData.ulTraceNumber
			&&DataSave0.SaveTransData.ucTransType==TRANS_S_BARCODE)
		{
			Uart_Printf("**Enter ucSuccessflag change\n");
			DataSave0.SaveTransData.ucPtCode=NormalTransData.ucPtCode;
			DataSave0.SaveTransData.ucSuccessflag='0';
			ucResult = XDATA_Write_SaveTrans_File(index);
			if(ucResult!=SUCCESS)
				return ucResult;
		}
	}

	return SUCCESS;
}



unsigned char SAV_OnlinePaySave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiOnlinePayNb+1;
	ulTotalAmount=pTransTotal->ulOnlinePayAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiOnlinePayNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulOnlinePayAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiOnlinePayNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulOnlinePayAmount+=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));				  	    
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
 	if(SAV_IssueInfo(NormalTransData.aucZoneName1)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}

	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;
	for(index=0;index<TRANS_MAXNB;index++)
	{
		if(DataSave0.TransInfoData.auiTransIndex[index]!=0x01)
			continue;
		XDATA_Read_SaveTrans_File_2Datasave(index);
		if(NormalTransData.ulTraceNumber==DataSave0.SaveTransData.ulTraceNumber
			&&DataSave0.SaveTransData.ucTransType==TRANS_S_BARCODE)
		{
			Uart_Printf("**Enter ucSuccessflag change\n");
			DataSave0.SaveTransData.ucPtCode=NormalTransData.ucPtCode;
			DataSave0.SaveTransData.ucSuccessflag='0';
			ucResult = XDATA_Write_SaveTrans_File(index);
			if(ucResult!=SUCCESS)
				return ucResult;
		}
	}

	return SUCCESS;
}
unsigned char SAV_MobileSave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	NormalTransData.ulAfterEDBalance=NormalTransData.ulPrevEDBalance-NormalTransData.ulAmount;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	if(NormalTransData.ucKYFlag==0)
	{
		uiTotalNb=pTransTotal->uiMobileNb+1;
		ulTotalAmount=pTransTotal->ulMobileAmount+NORMALTRANS_ulAmount;
		Os__saved_copy((unsigned char *)&uiTotalNb,
		       	       (unsigned char *)&pTransTotal->uiMobileNb,
			       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulTotalAmount,
		       	       (unsigned char *)&pTransTotal->ulMobileAmount,
			       sizeof(unsigned long));    		
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiMobileNb++;
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulMobileAmount+=NormalTransData.ulAmount;

		Os__saved_copy((unsigned char *)&uiAllTotalNb,
			       	       (unsigned char *)&pTransTotal->uiTotalNb,
				       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulAllTtAmount,
			       	       (unsigned char *)&pTransTotal->ulTotalAmount,
				       sizeof(unsigned long));	
	}
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
 	if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}

 
	DataSave0.RedoTrans.ucRedoFlag=0;
	XDATA_Write_Vaild_File(DataSaveRedoTrans);

	return SUCCESS;
}



unsigned char SAV_OnlineMobileSave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiOnlineMobileNb+1;
	ulTotalAmount=pTransTotal->ulOnlineMobileAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiOnlineMobileNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulOnlineMobileAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiOnlineMobileNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulOnlineMobileAmount+=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));				  	    
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
 	if(SAV_IssueInfo(NormalTransData.aucZoneName1)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}

	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;

	return SUCCESS;
}

unsigned char SAV_VoidSave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiOldTransIndex]=0xffff;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiOnlinePurchaseNb-1;
	ulTotalAmount=pTransTotal->ulOnlinePurchaseAmount-NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiOnlinePurchaseNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulOnlinePurchaseAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiOnlinePurchaseNb--;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulOnlinePurchaseAmount-=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));				  	    
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName1)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
 
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;

	return SUCCESS;
}

unsigned char SAV_VoidSave_P(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiOldTransIndex]=0xffff;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiOnlinePurchasepNb-1;
	ulTotalAmount=pTransTotal->ulOnlinePurchasepAmount-NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiOnlinePurchasepNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulOnlinePurchasepAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiOnlinePurchasepNb--;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulOnlinePurchasepAmount-=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));				  	    
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName1)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
 
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;

	return SUCCESS;
}

unsigned char SAV_LoadSave(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index;
	Uart_Printf("enter SAV_LoadSave\n");
	Uart_Printf("RUNDATA_uiTransIndex=%d\n",RUNDATA_uiTransIndex);
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;

			
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	if(NormalTransData.ucKYFlag==0)
	{
	
		uiTotalNb=pTransTotal->uiLoadNb+1;
		ulTotalAmount=pTransTotal->ulLoadAmount+NORMALTRANS_ulAmount;
		Os__saved_copy((unsigned char *)&uiTotalNb,
		       	       (unsigned char *)&pTransTotal->uiLoadNb,
			       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulTotalAmount,
		       	       (unsigned char *)&pTransTotal->ulLoadAmount,
			       sizeof(unsigned long));    		
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiLoadNb++;
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulLoadAmount+=NormalTransData.ulAmount;

		Os__saved_copy((unsigned char *)&uiAllTotalNb,
			       	       (unsigned char *)&pTransTotal->uiTotalNb,
				       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulAllTtAmount,
			       	       (unsigned char *)&pTransTotal->ulTotalAmount,
				       sizeof(unsigned long));	

	}
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
 	if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	DataSave0.RedoTrans.ucRedoFlag=0;
	XDATA_Write_Vaild_File(DataSaveRedoTrans);
				
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;

	
	return ucResult;
}

unsigned char SAV_LoadOnlineSave(void)
{
	unsigned char ucResult = SUCCESS,aucZoneName[20];
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;

			
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	if(NormalTransData.ucKYFlag==0)
	{
	
		uiTotalNb=pTransTotal->uiLoadOnlineNb+1;
		ulTotalAmount=pTransTotal->ulLoadOnlineAmount+NORMALTRANS_ulAmount;
		Os__saved_copy((unsigned char *)&uiTotalNb,
		       	       (unsigned char *)&pTransTotal->uiLoadOnlineNb,
			       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulTotalAmount,
		       	       (unsigned char *)&pTransTotal->ulLoadOnlineAmount,
			       sizeof(unsigned long));    		
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiLoadOnlineNb++;
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulLoadOnlineAmount+=NormalTransData.ulAmount;

		Os__saved_copy((unsigned char *)&uiAllTotalNb,
			       	       (unsigned char *)&pTransTotal->uiTotalNb,
				       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulAllTtAmount,
			       	       (unsigned char *)&pTransTotal->ulTotalAmount,
				       sizeof(unsigned long));	
	}

	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(NormalTransData.ucCardLoadFlag==1)
		memcpy(aucZoneName,NormalTransData.aucZoneName,16);
	else
		memcpy(aucZoneName,NormalTransData.aucZoneName1,16);
 	if(SAV_IssueInfo(aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	DataSave0.RedoTrans.ucRedoFlag=0;
	XDATA_Write_Vaild_File(DataSaveRedoTrans);
				
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;

	
	return ucResult;
}

unsigned char SAV_CashLoadSave(void)
{
	unsigned char ucResult = SUCCESS,aucZoneName[20];
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;

			
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	if(NormalTransData.ucKYFlag==0)
	{
		uiTotalNb=pTransTotal->uiCashLoadNb+1;
		ulTotalAmount=pTransTotal->ulCashLoadAmount+NORMALTRANS_ulAmount;
		Os__saved_copy((unsigned char *)&uiTotalNb,
		       	       (unsigned char *)&pTransTotal->uiCashLoadNb,
			       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulTotalAmount,
		       	       (unsigned char *)&pTransTotal->ulCashLoadAmount,
			       sizeof(unsigned long));    		
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiCashLoadNb++;
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulCashLoadAmount+=NormalTransData.ulAmount;

		Os__saved_copy((unsigned char *)&uiAllTotalNb,
			       	       (unsigned char *)&pTransTotal->uiTotalNb,
				       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulAllTtAmount,
			       	       (unsigned char *)&pTransTotal->ulTotalAmount,
				       sizeof(unsigned long));	

	}
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(NormalTransData.ucCardLoadFlag==1)
		memcpy(aucZoneName,NormalTransData.aucZoneName,16);
	else
		memcpy(aucZoneName,NormalTransData.aucZoneName1,16);
 	if(SAV_IssueInfo(aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	DataSave0.RedoTrans.ucRedoFlag=0;
	XDATA_Write_Vaild_File(DataSaveRedoTrans);
				
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;

	
	return ucResult;
}

unsigned char SAV_RefundSave(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;

			
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	if(NormalTransData.ucKYFlag==0)
	{
		uiTotalNb=pTransTotal->uiRefundNb+1;
		ulTotalAmount=pTransTotal->ulRefundAmount+NORMALTRANS_ulAmount;
		Os__saved_copy((unsigned char *)&uiTotalNb,
		       	       (unsigned char *)&pTransTotal->uiRefundNb,
			       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulTotalAmount,
		       	       (unsigned char *)&pTransTotal->ulRefundAmount,
			       sizeof(unsigned long));    		
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiRefundNb++;
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulRefundAmount+=NormalTransData.ulAmount;

		Os__saved_copy((unsigned char *)&uiAllTotalNb,
			       	       (unsigned char *)&pTransTotal->uiTotalNb,
				       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulAllTtAmount,
			       	       (unsigned char *)&pTransTotal->ulTotalAmount,
				       sizeof(unsigned long));	

	}
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
 	if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	DataSave0.RedoTrans.ucRedoFlag=0;
	XDATA_Write_Vaild_File(DataSaveRedoTrans);
				
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;

	
	return ucResult;
}
unsigned char SAV_BarCode(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount,ulRate;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult=SUCCESS,ucType;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	ucType=NormalTransData.ucTransType;
	NormalTransData.ucTransType=TRANS_S_BARCODE;
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	NormalTransData.ucTransType=ucType;
			
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	uiTotalNb=pTransTotal->ulTotalBarNb;
	uiTotalNb++;
	Os__saved_copy((unsigned char *)&uiTotalNb,
		       	       (unsigned char *)&pTransTotal->ulTotalBarNb,
			       sizeof(unsigned int));
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	return ucResult;
}

unsigned char SAV_SaleSave(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;

			
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiSaleNb+1;
	ulTotalAmount=pTransTotal->ulSaleAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiSaleNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulSaleAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiSaleNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulSaleAmount+=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));	

	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
 	
	return ucResult;
}

unsigned char SAV_SaleOnlineSave(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;

			
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiSaleOnlineNb+1;
	ulTotalAmount=pTransTotal->ulSaleOnlineAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiSaleOnlineNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulSaleOnlineAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiSaleOnlineNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulSaleOnlineAmount+=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));	

	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
 	
	return ucResult;
}

unsigned char SAV_BatchLoadSave(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;

			
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiBatchLoadNb+1;
	ulTotalAmount=pTransTotal->ulBatchLoadAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiBatchLoadNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulBatchLoadAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiBatchLoadNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulBatchLoadAmount+=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));	

	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
 	
	return ucResult;
}

unsigned char SAV_OnlineRefundSave(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;

			
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiOnlineRefundNb+1;
	ulTotalAmount=pTransTotal->ulOnlineRefundAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiOnlineRefundNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulOnlineRefundAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiOnlineRefundNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulOnlineRefundAmount+=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));	

	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
 	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	return ucResult;
}
unsigned char SAV_TransferSave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned int   uiIndex;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiTransferNb+1;
	ulTotalAmount=pTransTotal->ulTransferAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiTransferNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulTransferAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiTransferNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulTransferAmount+=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));				  	    
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName1)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	XDATA_Write_Vaild_File(DataSaveReversalISO8583);

	for(uiIndex=0;uiIndex<TRANS_MAXNB;uiIndex++)
	{
		if( DataSave0.TransInfoData.auiTransIndex[uiIndex])
		{
			XDATA_Read_SaveTrans_File_2Datasave(uiIndex);
			if(DataSave0.SaveTransData.ulTraceNumber==RunData.ulOldTraceNumber
				&&DataSave0.SaveTransData.ucTransType==TRANS_S_TRANSPURCHASE)
			{
				DataSave0.SaveTransData.ucSuccessflag='0';
				XDATA_Write_SaveTrans_File(uiIndex);
				break;
			}
		}
	}

	return SUCCESS;
}
unsigned char SAV_KyTran(uchar ucKYFlag)
{
	NORMALTRANS tnormal;
	unsigned char ucResult=SUCCESS;
	unsigned char ucStatus,ucLen,aucOutData[30],aucBuf[20];
	Uart_Printf("enter SAV_KyTran\n");
	memcpy(&tnormal,&NormalTransData,sizeof(NORMALTRANS));
	memcpy(&NormalTransData,&DataSave0.RedoTrans.Tran,sizeof(NORMALTRANS));
	ucResult=SAV_SavedTransIndex(0);
	if(ucResult==SUCCESS)
	{
		if(NormalTransData.ucCardType!=CARD_CPU&&tnormal.ucCardType!=CARD_CPU)
			MIFARE_GetTAC();
		else
			PBOC_ISOGetTac(0x06,&NormalTransData.aucSamTace[2]);
	}
	if(ucResult==SUCCESS)
	{
		if(NormalTransData.ucCardType!=CARD_CPU)
			memcpy(NormalTransData.aucTraceTac,&MifareCommandData.aucCommandOut[1],4);
		else
			memcpy(NormalTransData.aucTraceTac,Iso7816Out.aucData,4);
		NormalTransData.ucKYFlag=ucKYFlag;
		switch(NormalTransData.ucTransType)
		{
			case TRANS_S_PURCHASE:
			case TRANS_S_TRANSPURCHASE:
			case TRANS_S_RETURNCARD:
				ucResult=SAV_PurchaseSave();
				break;
			case TRANS_S_PURCHASE_P:
			case TRANS_S_RETURNCARD_P:
				ucResult=SAV_PurchaseSave_P();
				break;
			case TRANS_S_PRELOAD:
				ucResult=SAV_LoadSave();
				break;
			case TRANS_S_LOADONLINE:
				ucResult=SAV_LoadOnlineSave();
				break;
			case TRANS_S_CASHLOAD:
				ucResult=SAV_CashLoadSave();
				break;
			case TRANS_S_REFUND:
				ucResult=SAV_RefundSave();
				break;
			case TRANS_S_PAY:
				ucResult=SAV_PaySave();
				break;
			case TRANS_S_PAYNOBAR:
				ucResult=SAV_PayNobarSave();
				break;
			case TRANS_S_MOBILECHARGE:
				ucResult=SAV_MobileSave();
				break;
			case TRANS_S_MOBILECHARGEONLINE:
				ucResult=SAV_OnlineMobileSave();
				break;
			case TRANS_S_ZSHPURCHASE:
				ucResult=SAV_ZshPurchaseSave();
				break;
			case TRANS_S_CHANGEEXP:
				ucResult=SAV_ChangeExpSave();
				break;
			case TRANS_S_CHANGECD:
				ucResult=SAV_ChangeCDSave();
				break;
			case TRANS_S_EXPPURCHASE:
				ucResult=SAV_ExpPurchaseSave();
				break;
			case TRANS_S_PTCPURCHASE:
				ucResult=SAV_PTCPurchaseSave();
				break;
			default:
				DataSave0.RedoTrans.ucRedoFlag=0;
				XDATA_Write_Vaild_File(DataSaveRedoTrans);
				return ERR_END;
		}
	}
	if(ucKYFlag!=0)
	{
		memcpy(&NormalTransData,&tnormal,sizeof(NORMALTRANS));
		RunData.uckeyi=1;
	}
	return ucResult;
}
unsigned char SAV_PreAuthSave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiPreAuthNb+1;
	ulTotalAmount=pTransTotal->ulPreAuthAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiPreAuthNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulPreAuthAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiPreAuthNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulPreAuthAmount+=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));				  	    
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName1)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}

	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;

	return SUCCESS;
}

unsigned char SAV_PreAuthVoidSave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	//DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiOldTransIndex]=0xffff;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiPreAuthVoidNb+1;
	ulTotalAmount=pTransTotal->ulPreAuthVoidAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiPreAuthVoidNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulPreAuthVoidAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiPreAuthVoidNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulPreAuthVoidAmount+=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));				  	    
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	if(SAV_IssueInfo(NormalTransData.aucZoneName1)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	

	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;

	return SUCCESS;
}
unsigned char SAV_PreAuthFinishSave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiPreAuthFinishNb+1;
	ulTotalAmount=pTransTotal->ulPreAuthFinishAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiPreAuthFinishNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulPreAuthFinishAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiPreAuthFinishNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulPreAuthFinishAmount+=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));				  	    
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
 	if(SAV_IssueInfo(NormalTransData.aucZoneName1)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}

	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;

	return SUCCESS;
}

unsigned char SAV_PreAuthFinishVoidSave(void)
{
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index,ucResult;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiOldTransIndex]=0xffff;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;
	
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiPreAuthFinishNb-1;
	ulTotalAmount=pTransTotal->ulPreAuthFinishAmount-NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiPreAuthFinishNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulPreAuthFinishAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiPreAuthFinishNb--;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulPreAuthFinishAmount-=NormalTransData.ulAmount;

	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));				  	    
	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
 	if(SAV_IssueInfo(NormalTransData.aucZoneName1)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}

	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;
	return SUCCESS;
}

unsigned char SAV_ChangeCDSave(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;

			
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	if(NormalTransData.ucKYFlag==0)
	{
		uiTotalNb=pTransTotal->uiChangeCardNb+1;
		ulTotalAmount=pTransTotal->ulChangeCardAmount+NORMALTRANS_ulAmount;
		Os__saved_copy((unsigned char *)&uiTotalNb,
		       	       (unsigned char *)&pTransTotal->uiChangeCardNb,
			       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulTotalAmount,
		       	       (unsigned char *)&pTransTotal->ulChangeCardAmount,
			       sizeof(unsigned long));    		
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiChangeCardNb++;
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulChangeCardAmount+=NormalTransData.ulAmount;

		Os__saved_copy((unsigned char *)&uiAllTotalNb,
			       	       (unsigned char *)&pTransTotal->uiTotalNb,
				       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulAllTtAmount,
			       	       (unsigned char *)&pTransTotal->ulTotalAmount,
				       sizeof(unsigned long));	

		if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
		{
			//Os__GB2312_display(1,0,"ۼʧ");
			MSG_WaitKey(2);
		}
		if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
		{
			//Os__GB2312_display(1,0,"ۼʧ");
			MSG_WaitKey(2);
		}
	}
	DataSave0.RedoTrans.ucRedoFlag=0;
	XDATA_Write_Vaild_File(DataSaveRedoTrans);
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
						0, sizeof(unsigned char));	
	ucResult=XDATA_Write_Vaild_File(DataSaveReversalISO8583);
	if(ucResult!=SUCCESS)
		return ucResult;
 	
	return ucResult;
}

unsigned char SAV_ChangeExpSave(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;

			
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	if(NormalTransData.ucKYFlag==0)
	{
		uiTotalNb=pTransTotal->uiChangeExpNb+1;
		ulTotalAmount=pTransTotal->ulChangeExpAmount+NORMALTRANS_ulAmount;
		Os__saved_copy((unsigned char *)&uiTotalNb,
		       	       (unsigned char *)&pTransTotal->uiChangeExpNb,
			       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulTotalAmount,
		       	       (unsigned char *)&pTransTotal->ulChangeExpAmount,
			       sizeof(unsigned long));    		
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiChangeExpNb++;
		DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulChangeExpAmount+=NormalTransData.ulAmount;
		Os__saved_copy((unsigned char *)&uiAllTotalNb,
			       	       (unsigned char *)&pTransTotal->uiTotalNb,
				       sizeof(unsigned int));
		Os__saved_copy((unsigned char *)&ulAllTtAmount,
			       	       (unsigned char *)&pTransTotal->ulTotalAmount,
				       sizeof(unsigned long));	

		if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
		{
			//Os__GB2312_display(1,0,"ۼʧ");
			MSG_WaitKey(2);
		}
		if(SAV_IssueInfo(NormalTransData.aucZoneName)!=SUCCESS)
		{
			//Os__GB2312_display(1,0,"ۼʧ");
			MSG_WaitKey(2);
		}
	}
	DataSave0.RedoTrans.ucRedoFlag=0;
	XDATA_Write_Vaild_File(DataSaveRedoTrans);
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
					0,sizeof(unsigned char));
	XDATA_Write_Vaild_File(DataSaveReversalISO8583);
 	
	return ucResult;
}

unsigned char SAV_RefAplSave(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;

			
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiRefAplNb+1;
	ulTotalAmount=pTransTotal->ulRefAplAmount+NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiRefAplNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulRefAplAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiRefAplNb++;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulRefAplAmount+=NormalTransData.ulAmount;
	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));	

	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
					0,sizeof(unsigned char));
	XDATA_Write_Vaild_File(DataSaveReversalISO8583);
 	
	return ucResult;
}

unsigned char SAV_VoidRefAplSave(void)
{
	unsigned char ucResult = SUCCESS;
	unsigned int	uiTotalNb,uiAllTotalNb;
	unsigned long	ulTotalAmount,ulAllTtAmount;
	TRANSTOTAL	*pTransTotal;	
	unsigned char   index;
	
	DataSave0.TransInfoData.auiTransIndex[RUNDATA_uiTransIndex]=0x01;
	
	OSMEM_Memcpy((unsigned char *)&xDATA_SaveTrans,&NormalTransData,sizeof(NORMALTRANS));
	ucResult=XDATA_Write_SaveTrans_File(RUNDATA_uiTransIndex);
	if(ucResult!=SUCCESS)
		return ucResult;

			
	pTransTotal=&DataSave0.TransInfoData.TransTotal;
	uiAllTotalNb=pTransTotal->uiTotalNb;
	uiAllTotalNb++;
	ulAllTtAmount=pTransTotal->ulTotalAmount+NORMALTRANS_ulAmount;
	
	uiTotalNb=pTransTotal->uiRefAplNb-1;
	ulTotalAmount=pTransTotal->ulRefAplAmount-NORMALTRANS_ulAmount;
	Os__saved_copy((unsigned char *)&uiTotalNb,
	       	       (unsigned char *)&pTransTotal->uiRefAplNb,
		       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulTotalAmount,
	       	       (unsigned char *)&pTransTotal->ulRefAplAmount,
		       sizeof(unsigned long));    		
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].uiRefAplNb--;
	DataSave0.TransInfoData.PtInfoData[NormalTransData.ucPtCode-1].ulRefAplAmount-=NormalTransData.ulAmount;
	Os__saved_copy((unsigned char *)&uiAllTotalNb,
		       	       (unsigned char *)&pTransTotal->uiTotalNb,
			       sizeof(unsigned int));
	Os__saved_copy((unsigned char *)&ulAllTtAmount,
		       	       (unsigned char *)&pTransTotal->ulTotalAmount,
			       sizeof(unsigned long));	

	if(XDATA_Write_Vaild_File(DataSaveTransInfo)!=SUCCESS)
	{
		//Os__GB2312_display(1,0,"ۼʧ");
		MSG_WaitKey(2);
	}
	Os__saved_set((unsigned char *)(&DataSave0.ReversalISO8583Data.ucValid),
					0,sizeof(unsigned char));
	XDATA_Write_Vaild_File(DataSaveReversalISO8583);
 	
	return ucResult;
}

