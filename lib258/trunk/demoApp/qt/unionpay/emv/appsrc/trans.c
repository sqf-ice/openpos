#include "include.h"
#include "xdata.h"
#include "global.h"
#include "msg.h"
#include "py_input.h"

unsigned char Trans_TestSingal(void)
{
	unsigned char ucResult;

	G_NORMALTRANS_ucTransType = TRANS_ONLINETEST;
	ucResult = SEL_TestOnlineTrans();
	return(ucResult);
}

unsigned char Trans_EnqBalance(void)
{
	unsigned char ucResult;

	ucResult=SUCCESS;
	if(!I_Inquiry)
		ucResult = ERR_TRANSNOTSUP;
	else
	{
		G_NORMALTRANS_ucTransType = TRANS_QUERYBAL;
		ucResult = Trans_Process(0);
	}
	util_Printf("ucResutl=%02x\n",ucResult);
	return(ucResult);
}

unsigned char TRANS_CheckCard(void)
{
        DRV_OUT *pxKey;
        unsigned char ucResult=SUCCESS;
        unsigned char ucKey;

        while(1)
        {
            pxKey = Os__get_key();
            //Os__clr_display(255);
            //Os__GB2312_display(0,0,(uchar *)" (移动支付消费)");
            //Os__GB2312_display(2,2," 请放卡");

            while(((ucResult=DIGITAL_CheckCard())==0x04)
                        &&(pxKey->gen_status != DRV_ENDED));

		if(pxKey->gen_status==DRV_ENDED)
		{
			Os__abort_drv(drv_icc);
			ucKey = pxKey->xxdata[1];
			if(ucKey==KEY_CLEAR) return ERR_CANCEL;
		}
		if (!ucResult)
                {
                    ucResult = MAG_ProUntouchCard();
                    break;
                }
        }
        util_Printf("移动支付读卡返回:%02x\n",ucResult);
        return(ucResult);
}
unsigned char Trans_InputOrder(void)
{
    unsigned char ucResult=SUCCESS;
    unsigned char ucInputFlag,ucLen;
    unsigned char aucOrdersNo[11];

    if ((G_NORMALTRANS_ucTransType == TRANS_PURCHASE
        || G_NORMALTRANS_ucTransType == TRANS_OFFPURCHASE
        || G_NORMALTRANS_ucTransType == TRANS_MOBILEAUTH)
        && !G_RUNDATA_ucOrderFlag)
    {
        if(DataSave0.ConstantParamData.ucCashFlag == 0x31)
            return (ERR_CANCEL);
        if(DataSave0.ConstantParamData.uclujiaomc == '1')
        	ucResult = UTIL_InputMerchant_Cust(NormalTransData.aucCustName);//选择代理客户名
        if(ucResult != SUCCESS)
        	return ucResult;
        while(1)
        {
            MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);
            //Os__GB2312_display(1, 0, (uchar * )"请输入订单编号:");

            ucInputFlag = PY_INPUT_NUMBER | PY_INPUT_UPPER_CASE|PY_INPUT_LOWER_CASE;

            memset(aucOrdersNo,0,sizeof(aucOrdersNo));
            memcpy(aucOrdersNo,G_NORMALTRANS_aucOrdersNo,TRANS_ORDERSNO);
            ucResult=PY_Main(aucOrdersNo,2,4,TRANS_ORDERSNO,ucInputFlag,300);
            if(ucResult != SUCCESS)
            {
                return(ucResult);
            }
            else
            {
				G_RUNDATA_ucOrderFlag=1;
				memcpy(G_NORMALTRANS_aucOrdersNo,aucOrdersNo,sizeof(aucOrdersNo));
				ucLen = strlen((char*)G_NORMALTRANS_aucOrdersNo);
				util_Printf("订单号长度= %d\n",ucLen);
				util_Printf("补[%d]空格\n",(10-ucLen));
				if(ucLen < 10)
				{
					memset(&G_NORMALTRANS_aucOrdersNo[ucLen],' ',(10-ucLen));
				}
				util_Printf("订单号 =%s\n",G_NORMALTRANS_aucOrdersNo);
				break;
            }
        }
    }
    return(SUCCESS);
}

unsigned char Trans_MobileTProcess(void)
{
	unsigned char ucResult=SUCCESS;

	//先输入交易金额
	ucResult=UTIL_EMVInputTransAmount();
	if (ucResult)	return(ucResult);

	//输入预约手机号码
	return(UTIL_InPutMobileNum());
}

unsigned char Trans_InputNumOfStages(void)
{
	unsigned char ucResult,ucLen,ucKey;
	unsigned char aucNumStages[2],aucProjectNum[31];
	unsigned char aucTempBuf[2];
	unsigned int ulStages;

	ucResult=SUCCESS;
	MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);

	if (G_NORMALTRANS_ucTransType==TRANS_CREDITSALE)
	{
		while(1)
		{
			if (G_NORMALTRANS_ucTransType==TRANS_CREDITSALE)
			{
				//Os__GB2312_display(1, 0, (uchar * )"请输入交易期数:");
				//Os__GB2312_display(2, 0, (uchar * )"(2-36):");
				memset(aucNumStages,0,sizeof(aucNumStages));
				if( UTIL_Input(3,true,1,TRANS_NumStages,'N',aucNumStages,NULL) != KEY_ENTER )
				{
					return(ERR_CANCEL);
				}else
				{

					ucLen=strlen((char*)aucNumStages);
					if(ucLen<2)
					{
						memset(&aucTempBuf[0],0x30,sizeof(aucTempBuf));
						memcpy(&aucTempBuf[2-ucLen],aucNumStages,ucLen);
					}else
						memcpy(aucTempBuf,aucNumStages,TRANS_NumStages);
					ulStages=asc_long(aucTempBuf,2);

					util_Printf("校验期合法性:%d\n",ulStages);
					if (ulStages>1&& ulStages<37)
					{
						memcpy(G_NORMALTRANS_aucNumStages,aucTempBuf,TRANS_NumStages);
						util_Printf("\n交易期数:%s",G_NORMALTRANS_aucNumStages);
						break;
					}
					else
					{
						//Os__clr_display(1);
						//Os__clr_display(2);
						//Os__clr_display(3);
						//Os__GB2312_display(2, 0, (uchar * )" 交易期数不合法");
						UTIL_WaitGetKey(5);
						//Os__clr_display(2);
						continue;
					}
					memcpy(G_NORMALTRANS_aucNumStages,aucTempBuf,TRANS_NumStages);

					util_Printf("\n交易期数:%s",G_NORMALTRANS_aucNumStages);
					break;
				}
			}
			break;
		}

		MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);
		//Os__GB2312_display(1, 0, (uchar * )"请输入商品代码:");
		memset(aucProjectNum,0,sizeof(aucProjectNum));

		ucResult=PY_Main(aucProjectNum,2,0,TRANS_NumProject,PY_INPUT_NUMBER|PY_INPUT_UPPER_CASE,300);
		if(ucResult!=SUCCESS)
		{
			return(ERR_CANCEL);
		}
		else
			memcpy(G_NORMALTRANS_aucNumProject,aucProjectNum,strlen((char*)aucProjectNum));
		util_Printf("项目编码:%s\n",G_NORMALTRANS_aucNumProject);

		while(1)
		{
			MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);
			//Os__GB2312_display(1, 0, (uchar * )"手续费支付方式");
			//Os__GB2312_display(2, 0, (uchar * )"1.一次性 2.分期");
			ucKey=UTIL_WaitGetKey(60);
			switch(ucKey)
			{
				case '1':
					G_NORMALTRANS_bInstallments=true;
					break;
				case '2':
					G_NORMALTRANS_bInstallments=false;
					break;
				case 99:
				case KEY_CLEAR:
					return(ERR_CANCEL);
				default:
					continue;
			}
			util_Printf("手续费支付方式:%02x\n",G_NORMALTRANS_bInstallments);
			break;
		}
	}
	return(SUCCESS);
}

unsigned char Trans_Process(unsigned char Flag)
{
	unsigned char	ucResult,ucI,ucResult1,ucCopy;
	unsigned long	ulAmount;
	unsigned long	AuthLen;
	unsigned char	aucBuf[200],DateTimeBuf[13];
	short iLen;
	unsigned char ucInputFlag;
	unsigned char aucRefNoBuf[13];
	unsigned char ucTransType,ucInputType,ucNeedInputPIN;
	unsigned char ucKey;
	unsigned char	 ucPindata[8], ucPinlen;
	EXTRATRANS    ExtraTransOnline;
	DRV_OUT  aucDRVOut;
#ifndef GUI_PROJECT
	Os__light_on();
	if (DataSave0.ConstantParamData.ucSendCollectFlag=='1')
	{
		//Os__clr_display(255);
		//Os__GB2312_display(1,0,(uchar *)"请先上送采集数据");
		UTIL_GetKey(5);
		return(ERR_CANCEL);
	}
	if (G_NORMALTRANS_ucInputMode != TRANS_INPUTMODE_PUTCARD)
	{
		if(UTIL_Check_BasetSet() == SUCCESS)    //检查终端状态
		{
			if(DataSave0.ConstantParamData.ucCashFlag == 0x31)
				return(DataSave0.ChangeParamData.ucResult);
			else
				return SUCCESS;
		}
	}
#endif
util_Printf("\n.0001.Trans_Process...........");
util_Printf("\n.0001.ucTransType...........[%02x]",G_NORMALTRANS_ucTransType);
util_Printf("\n.0001.ucInputMode...........[%02x]",G_NORMALTRANS_ucInputMode);
util_Printf("\n.0001.CardSpecies...........[%02x]",G_NORMALTRANS_euCardSpecies);
util_Printf("\n.0001.TraceNumber...........[%6d]\n",G_NORMALTRANS_ulTraceNumber);	

util_Printf("\n.0099.001.G_EXTRATRANS_ucInputPINLen..........[%02x][%d].\n",ucResult,G_EXTRATRANS_ucInputPINLen);
	ucResult = SUCCESS;
	G_RUNDATA_ucTransType=G_NORMALTRANS_ucTransType;

	if((ucResult = SAV_CheckTransMaxNb()) != SUCCESS)		//检查交易是否已满
		return(ucResult);

    	/*Cashier[柜员]*/
	memcpy(G_NORMALTRANS_aucCashierNo,
		DataSave0.Cashier_SysCashier_Data.aucCashierNo[DataSave0.ChangeParamData.ucCashierLogonIndex],
			CASH_CASHIERNOLEN);

	if(  (G_NORMALTRANS_ucTransType != TRANS_TIPADJUST)
	   &&(G_NORMALTRANS_ucTransType != TRANS_OFFPREAUTHFINISH)
	   &&(G_NORMALTRANS_ucTransType != TRANS_UNDOOFF)
	   &&(G_NORMALTRANS_ucTransType != TRANS_REFUND)
	  )
	{
		COMMS_PreComm();
		ucTransType=G_NORMALTRANS_ucTransType;
		util_Printf("\nDataSave0.ChangeParamData.ucDownLoadFlag=%d",DataSave0.ChangeParamData.ucDownLoadFlag);
		if( DataSave0.ChangeParamData.ucDownLoadFlag==1 )
		{
			G_NORMALTRANS_ucTransType=TRANS_LOADPARAM;
			ucResult=LOGON_Online();
			if( ucResult==SUCCESS )
			{
				DataSave0.ChangeParamData.ucDownLoadFlag=0;
				XDATA_Write_Vaild_File(DataSaveChange);
			}
		}
		G_NORMALTRANS_ucTransType=ucTransType;
	}
util_Printf("\n.0001.001.Trans_Process...........\n");
util_Printf("\n.0099.002.G_EXTRATRANS_ucInputPINLen..........[%02x][%d].\n",ucResult,G_EXTRATRANS_ucInputPINLen);

	/* Input Super Password 主管密码*/
	if( ucResult == SUCCESS )
	{
		switch( G_NORMALTRANS_ucTransType )
		{
		case TRANS_CREDITSVOID:
		case TRANS_VOIDPURCHASE:
			if(DataSave0.ConstantParamData.ucUnManagePwd=='1')
			{
				ucResult = CASH_InputAndChackManagerPass(0);
				if(ucResult != SUCCESS)
					return ucResult;
			}
			break;
		case TRANS_REFUND:
		case TRANS_UNDOOFF:
		case TRANS_CREDITSREFUND:
		case TRANS_VOIDPREAUTH:
		case TRANS_VOIDCUPMOBILE:
		case TRANS_VOIDMOBILEAUTH:
		case TRANS_VOIDPREAUTHFINISH:
			ucResult = CASH_InputAndChackManagerPass(0);
			if(ucResult != SUCCESS)
				return ucResult;
			break;
		default:
			break;
		}
	}
util_Printf("\n.0099.003.G_EXTRATRANS_ucInputPINLen..........[%02x][%d].\n",ucResult,G_EXTRATRANS_ucInputPINLen);

	/* Input Original Trace Number*/
	if( ucResult == SUCCESS && !Flag)
	{
		switch( G_NORMALTRANS_ucTransType )
		{
			case TRANS_UNDOOFF:
			case TRANS_TIPADJUST:
			case TRANS_CREDITSVOID:
			case TRANS_VOIDPURCHASE:
			case TRANS_VOIDCUPMOBILE:
			case TRANS_VOIDMOBILEAUTH:
			case TRANS_VOIDPREAUTHFINISH:
#ifdef GUI_PROJECT
				memcpy(G_RUNDATA_aucOriginalTraceNum,ProUiFace.UiToPro.aucOldTrace,strlen((char*)ProUiFace.UiToPro.aucOldTrace));
#else
				if((DataSave0.ConstantParamData.ucCashFlag == 0x31)&&(G_NORMALTRANS_ucTransType ==TRANS_VOIDPURCHASE ))
				  break;				
				MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);
				//Os__GB2312_display(1, 0, (uchar * )"请输入原凭证号:");
				if( UTIL_Input(2,true,1,TRANS_TRACENUMLEN,'N',G_RUNDATA_aucOriginalTraceNum,NULL) != KEY_ENTER )
				{
					return(ERR_CANCEL);
				}
#endif
				break;
	       	 default:
				break;
		}
	}
	util_Printf("\n.0001.002.Trans_Process...........[%02x]\n",ucResult);

	/* Search old trans or find new trans location */
	if( ucResult == SUCCESS )  //查询存储空间
	{
		switch( G_NORMALTRANS_ucTransType )
		{
		case TRANS_QUERYBAL:
			break;
		case TRANS_UNDOOFF://离线撤消
			ucResult = UTIL_Is_Trans_Empty();
			util_Printf("\nucResult=%02x\n",ucResult);
			if(ucResult != SUCCESS)
				return ucResult;
			ucResult=SAV_SavedTransIndex(3);
			util_Printf("SAV_SavedTransIndex(3)---ucResult=%02x",ucResult);
			break;
		case TRANS_VOIDPURCHASE:
		case TRANS_CREDITSVOID:
		case TRANS_VOIDCUPMOBILE:
		case TRANS_VOIDMOBILEAUTH:
		case TRANS_VOIDPREAUTHFINISH:
			ucResult =  SAV_SavedTransIndex(1);
			if(ucResult == SUCCESS)
			{
				ucResult =  SAV_SavedTransIndex(0);		/*将撤销交易作为单独交易存储*/
			}
			break;
		case TRANS_TIPADJUST:
			ucResult =  SAV_SavedTransIndex(2);
			util_Printf("返回值:%02x\n",ucResult);
			if(ucResult == SUCCESS)
			{
				if(G_NORMALTRANS_ucOldTransType == TRANS_OFFPREAUTHFINISH)
				{
					/*不保留小费额*/
					if(DataSave0.TransInfoData.auiTransIndex[G_RUNDATA_uiOldTransIndex]==TRANS_NIINORMAL)
					{
						G_RUNDATA_ucTIPSaving = 0;		/*离线交易未上送调整，覆盖原交易存放*/
						G_NORMALTRANS_ulTraceNumber = G_NORMALTRANS_ulOldTraceNumber;
					}else
					{
						G_RUNDATA_ucTIPSaving = 1;		/*离线已上送调整，产生新的交易存放*/
						ucResult =  SAV_SavedTransIndex(0);
					}
				}else
			 	{
			 		if(!DataSave0.ConstantParamData.ucXorTipFee)
			 		{
						return ERR_TRANSNOTSUP;
			 		}
					/*保留小费额*/
					G_RUNDATA_ucTIPSaving = 2;		/*其他交易小费，产生新的交易存放*/
					ucResult =  SAV_SavedTransIndex(0);
				}
			}
			break;
		default:
			ucResult =  SAV_SavedTransIndex(0);
			break;
		}
	}
util_Printf("\n.0099.004.G_EXTRATRANS_ucInputPINLen..........[%02x][%d].\n",ucResult,G_EXTRATRANS_ucInputPINLen);

	if (!ucResult && DataSave0.ConstantParamData.ucORDERFLAG == 0x31)//订单号
	{
		ucResult =Trans_InputOrder();
		if (ucResult)
			return(ucResult);
	}

	if(!ucResult && G_NORMALTRANS_ucTransType==TRANS_EC_UNASSIGNLOAD)
	{
		  ucResult=MASAPP_SwipeCard();/*转出卡输入*/
		  if(!ucResult)
		  	ucResult = UTIL_InputEncryptPIN_EC(ucPindata ,&ucPinlen);
	}
	util_Printf("\n.0002.Trans_Process...........[%02x]\n",ucResult);

	//读取片卡信息
	if( (ucResult == SUCCESS)
		&&(G_NORMALTRANS_ucTransType !=TRANS_UNDOOFF)
		&&(G_NORMALTRANS_ucTransType != TRANS_TIPADJUST)
		&&(G_NORMALTRANS_ucTransType != TRANS_CUPMOBILE)
		&&(G_NORMALTRANS_ucTransType != TRANS_VOIDCUPMOBILE)
		&&(G_NORMALTRANS_ucTransType != TRANS_MOBILEAUTH)
		&&(G_NORMALTRANS_ucTransType != TRANS_VOIDMOBILEAUTH)
	  )
	{
		util_Printf("\n.00021.Trans_Process...........\n");

		if( ( (G_NORMALTRANS_ucTransType == TRANS_VOIDPURCHASE)
				&&(DataSave0.ConstantParamData.ucUnSaleSwipe=='0'))
		  	||((G_NORMALTRANS_ucTransType == TRANS_VOIDPREAUTHFINISH)
		  		&&(DataSave0.ConstantParamData.ucUnAuthFinSwipe=='0'))
		  	||((G_NORMALTRANS_ucTransType == DataSave0.ConstantParamData.ucDefaultTransParam)
		  	   	&&(G_RUNDATA_ucDefOperation))
		  )
		{
			util_Printf("\n 1.G_RUNDATA_ucDefOperation=%02x\n",G_RUNDATA_ucDefOperation);
		}
		else
		{
			util_Printf("\n 2.G_RUNDATA_ucDefOperation=%02x\n",G_RUNDATA_ucDefOperation);

			if(((G_NORMALTRANS_ucTransType == TRANS_PURCHASE)
				||(G_NORMALTRANS_ucTransType == TRANS_QUERYBAL))
			&&(ReaderSupport)&&(ReaderType==READER_HONGBAO))
			{
				util_Printf("\n HBReader.....");
				
				if(DataSave0.ConstantParamData.ucCashFlag != 0x31)
				    ucResult=UTIL_EMVInputTransAmount();//非接交易先输金额
				    
				if(ucResult == SUCCESS )
				{
					memset(&aucDRVOut,0,sizeof(DRV_OUT));
					if(G_NORMALTRANS_ucECTrans==TRANS_Q_PURCHASE)
					{
						ucInputType=0x04;
						MAG_DisplaySwipeCardMode(G_NORMALTRANS_ucTransType,0x18);
					}
					else
					{
						ucInputType=0x07;
						MAG_DisplaySwipeCardMode(G_NORMALTRANS_ucTransType,0x15);
					}

					memset((unsigned char *)&InterfaceTransData,0,sizeof(InterfaceTransData));
                		      INTERFACE_TransToReader(G_NORMALTRANS_ulAmount,&InterfaceTransData,&aucDRVOut,&ucInputType);
                			util_Printf("\nRead返回刷卡模式ucInputType= %02x\n",ucInputType);
									
                        	if(((ucInputType== 0x00)||(ucInputType== 0x04)||(ucInputType== 0x07))
                        		&&(InterfaceTransData.unTransResult!=SUCCESS))//IC卡和磁卡不要判断
                        	{
                        		ucResult = InterfaceTransData.unTransResult;
                        		util_Printf("非接卡器返回ucResult= %02x\n",ucResult);
                        	}
                        	else
                        	{
                        		//BIT 0  IC;  BIT 1  mag ; BIT 2 interface
                	        	util_Printf("\nINTERFACE_TransToReader ucInputType=%02x",ucInputType);
                	        	if(ucInputType == 0x01)	     //IC卡
                	        		ucResult =MASAPP_External_Event(&aucDRVOut,0x01,&ucKey);
                	        	else if(ucInputType == 0x02) //磁卡
                	        		ucResult =MASAPP_External_Event(&aucDRVOut,0x02,&ucKey);
                	        	else if(ucInputType == 0x04) //非接卡
                	        	{
                	        		util_Printf("\nInterfaceTransData.bCheckOnlinePin =%d",InterfaceTransData.bCheckOnlinePin);
                	        		if((InterfaceTransData.bCheckOnlinePin == 0)&&(G_NORMALTRANS_ucTransType != TRANS_QUERYBAL))
                	        			ucNeedInputPIN =0;
                	        		else
                	        			ucNeedInputPIN =1;
                	        		if(InterfaceTransData.bForceOnline == 0)
                	        			COMMS_FinComm();
                	        		util_Printf("\nInterfaceTransData.bForceOnline=abc=%d",InterfaceTransData.bForceOnline);
                	        		ucResult = MASAPP_UnTouchCard_Event();
                	        		util_Printf("\nMASAPP_UnTouchCard_Event=%d",ucResult);
            		     		}
            		     		else
            				{
            					ucResult = MASAPP_Event(true ,&ucKey);
            					if(ucResult!=SUCCESS)
            						return(ucResult);
            				}
                	     	}
			    }
			}
#if SANDREADER
			else if((G_NORMALTRANS_ucTransType == TRANS_PURCHASE
				|| G_NORMALTRANS_ucTransType == TRANS_CREDITSALE
				|| G_NORMALTRANS_ucTransType == TRANS_QUERYBAL)
			&&(ReaderSupport)&&(ReaderType==READER_SAND))
			{
				util_Printf("\nSANDReader");
				if(DataSave0.ConstantParamData.ucCashFlag != 0x31)
				    ucResult=UTIL_EMVInputTransAmount();    //非接交易先输金额

				if ((!ucResult)&&(G_NORMALTRANS_ucTransType == TRANS_CREDITSALE))
				{
					ucResult=Trans_InputNumOfStages();
					if (ucResult)
						return(ucResult);
				}
				if(!ucResult)
				{
					util_Printf("\n.00022.Trans_Process...........\n");
					ucResult = MASAPP_Event_SAND(true ,&ucKey);
					util_Printf("\n.00023.Trans_Process..........End.\n");
					util_Printf("\nMASAPP_Event_SAND ucResult=%02x\n",ucResult);
				}
				if(ucResult)
				    return(ucResult);
			}
#endif
			else
			{

		util_Printf("\n.00023.Trans_Process...........\n");
		
				ucResult = MASAPP_Event(true ,&ucKey);
				if(ucResult!=SUCCESS)
					return(ucResult);
			}
		}
	}
util_Printf("\n.0099.005.G_EXTRATRANS_ucInputPINLen..........[%02x][%d].\n",ucResult,G_EXTRATRANS_ucInputPINLen);

	util_Printf("\n.0003.Trans_Process...........[%02x]\n",ucResult);

	if(ucResult==SUCCESS
		&&G_NORMALTRANS_ucTransType == TRANS_OFFPREAUTHFINISH)
	{
		ucResult=Trans_OFFLineTrans();
		util_Printf("离线结算ucResult2=%02x\n",ucResult);
		if(ucResult!=SUCCESS)
			return(ucResult);
	}
	util_Printf("\n.0003.001.Trans_Process...........[%02x]\n",ucResult);
	//手机预约消费
	if (!ucResult && G_NORMALTRANS_ucTransType==TRANS_MOBILEAUTH)
	{
		ucResult=Trans_MobileTProcess();
		if (ucResult){
			COMMS_FinComm();
			return(ucResult);
		}
	}
	util_Printf("\n.0003.002.Trans_Process...........[%02x]\n",ucResult);
	/* Void trans & tipadjust trans */
	if(ucResult == SUCCESS )									//输入原参考号
	{
		switch( G_NORMALTRANS_ucTransType )
		{
			case TRANS_REFUND:
				if(DataSave0.ConstantParamData.ucCashFlag == 0x31)
					  break;
#ifdef GUI_PROJECT
				memcpy(G_NORMALTRANS_aucOldRefNumber,ProUiFace.UiToPro.aucRefNum,12);
#else				
				MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);
				//Os__GB2312_display(1, 0, (uchar * )"请输入原参考号:");

				ucInputFlag = PY_INPUT_NUMBER|PY_INPUT_UPPER_CASE;
				memset(aucRefNoBuf,0,sizeof(aucRefNoBuf));
				memcpy(aucRefNoBuf,G_NORMALTRANS_aucOldRefNumber,12);
				ucResult=PY_Main(aucRefNoBuf,2,TRANS_REFNUMLEN,TRANS_REFNUMLEN,ucInputFlag,300);
				if(ucResult!=SUCCESS)
				{
					return(ERR_CANCEL);
				}
				else
				{
					memcpy(G_NORMALTRANS_aucOldRefNumber,aucRefNoBuf,12);
					util_Printf("\naucRefNoBuf=%s\n",aucRefNoBuf);
					util_Printf("G_NORMALTRANS_aucOldRefNumber=%s\n",G_NORMALTRANS_aucOldRefNumber);
				}
#endif				
        		break;
			default:
				break;
		}
	}
	util_Printf("\n.0003.003.Trans_Process...........[%02x]\n",ucResult);
util_Printf("\n.0099.006.G_EXTRATRANS_ucInputPINLen..........[%02x][%d].\n",ucResult,G_EXTRATRANS_ucInputPINLen);

	/*Input Original Data*/
	if( ucResult == SUCCESS )								//输入原交易日期
	{
		switch( G_NORMALTRANS_ucTransType )
		{
			case TRANS_REFUND:
			case TRANS_VOIDPREAUTH:
			case TRANS_PREAUTHFINISH:
			case TRANS_PREAUTHSETTLE:
		 	if(DataSave0.ConstantParamData.ucCashFlag != 0x31)
		 	{
				memset(DateTimeBuf ,0 ,sizeof(DateTimeBuf));
#ifdef GUI_PROJECT
				asc_bcd(&G_NORMALTRANS_aucOldDate[2], 2, ProUiFace.UiToPro.aucTransDate, 4);
#else
				MSG_DisplayMsg( true,0,0,MSG_TRANSDATE);
				//Os__GB2312_display(2, 0, (uchar * )"(月月日日):");
				if( UTIL_Input(3,true,TRANS_DATELEN,TRANS_DATELEN,'N',DateTimeBuf,NULL) != KEY_ENTER )
				{
					return(ERR_CANCEL);
				}
				asc_bcd(&G_NORMALTRANS_aucOldDate[2], 2, DateTimeBuf, 4);
#endif				
		 	}
			G_NORMALTRANS_ulOldTraceNumber = G_NORMALTRANS_ulTraceNumber;
			break;
	     default:
			break;
		}
	}
	util_Printf("\n.0003.004.Trans_Process...........[%02x]\n",ucResult);
	/* Input AuthCode */
	if( ucResult == SUCCESS )
	{
		switch( G_NORMALTRANS_ucTransType )
		{
			case TRANS_PREAUTHFINISH:
			case TRANS_PREAUTHSETTLE:
			case TRANS_VOIDPREAUTH:
			case TRANS_PREAUTHADD:
#ifdef GUI_PROJECT
				memcpy(G_NORMALTRANS_aucAuthCode,ProUiFace.UiToPro.aucAuthNo,6);
#else				
        			MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);
				//Os__GB2312_display(1, 0, (uchar * )"请输入原授权码:");
				ucInputFlag = PY_INPUT_NUMBER|PY_INPUT_UPPER_CASE;
				ucResult=PY_Main(G_NORMALTRANS_aucAuthCode,2,2,TRANS_AUTHCODELEN,ucInputFlag,300);
				util_Printf("ucResult=%02x\n",ucResult);
#endif				

				if(ucResult!=SUCCESS)
				{
					return(ERR_CANCEL);
				}else
				{
	        			memcpy(G_NORMALTRANS_aucOldAuthCode,G_NORMALTRANS_aucAuthCode,6);
					util_Printf("G_NORMALTRANS_aucOldAuthCode=%s,\nG_NORMALTRANS_aucAuthCode=%s\n",G_NORMALTRANS_aucOldAuthCode,G_NORMALTRANS_aucAuthCode);
				}
				AuthLen = strlen((char*)G_NORMALTRANS_aucOldAuthCode);
				util_Printf("原授权号长度= %d\n",AuthLen);
				util_Printf("补[%d]空格\n",(6-AuthLen));
				if(AuthLen < 6)
				{
					memset(&G_NORMALTRANS_aucOldAuthCode[AuthLen],' ',(6-AuthLen));
				}
				util_Printf("G_NORMALTRANS_aucOldAuthCode=%s\n",G_NORMALTRANS_aucOldAuthCode);
		        	break;
        	default:
				break;
		}
	}
	util_Printf("\n.0003.005.Trans_Process...........[%02x]\n",ucResult);
util_Printf("\n.0099.007.G_EXTRATRANS_ucInputPINLen..........[%02x][%d].\n",ucResult,G_EXTRATRANS_ucInputPINLen);

	/*输入交易金额*/
	if( (ucResult == SUCCESS)
		&& (G_NORMALTRANS_ucTransType != TRANS_CUPMOBILE )
		&& (G_NORMALTRANS_ucTransType != TRANS_MOBILEAUTH ))
	{
		 if((DataSave0.ConstantParamData.ucCashFlag == 0x31)
		 	&&((G_NORMALTRANS_ucTransType == TRANS_PURCHASE)
			  	||(G_NORMALTRANS_ucTransType == TRANS_REFUND)
				||(G_NORMALTRANS_ucTransType == TRANS_CREDITSREFUND)))
		{
			    ucResult = UTIL_CheckTransAmount();
		}
		if(G_NORMALTRANS_ulAmount==0||G_NORMALTRANS_ucTransType==TRANS_TIPADJUST)
		{
			if (DataSave0.ConstantParamData.ucCashFlag == 0x31)
			{
				return (ERR_NOZEOR);
			}
			ucResult=UTIL_EMVInputTransAmount();
		}
	}
	#if 1
	if((ucResult == SUCCESS)&&(G_NORMALTRANS_ucTransType == TRANS_REFUND))
	{
		util_Printf("Befor refund\n");
		ucTransType = G_NORMALTRANS_ucTransType;
		G_NORMALTRANS_ucTransType = TRANS_LOGONON;
		ucResult = LOGON_CashierProcess(1);
		util_Printf("Befor refund should Logon :[%02x]\n",ucResult);
		if (ucResult != SUCCESS)
		{
			//Os__clr_display(255);
			//Os__GB2312_display(1,2,(unsigned char *)"交易失败");
			//Os__GB2312_display(2,1,(unsigned char *)"请重新签到!");
			UTIL_GetKey(2);
			return (ERR_CANCEL);
		}
		G_NORMALTRANS_ucTransType = ucTransType;
		G_RUNDATA_ucHostConnectFlag = true;

		util_Printf("Precom :%02x\n",G_RUNDATA_ucDialFlag);
	}
	#endif

	util_Printf("\n.0003.006.Trans_Process...........[%02x]\n",ucResult);
	if(  (ucResult == SUCCESS)
		&&(( (G_NORMALTRANS_ucTransType == TRANS_VOIDPURCHASE)
			&&(DataSave0.ConstantParamData.ucUnSaleSwipe=='0'))
	  	||((G_NORMALTRANS_ucTransType == TRANS_VOIDPREAUTHFINISH)
	  		&&(DataSave0.ConstantParamData.ucUnAuthFinSwipe=='0'))
	  ))
	{
		G_NORMALTRANS_ucInputMode = TRANS_INPUTMODE_MANUALINPUT;
	}
		util_Printf("\n.0004.Trans_Process...........[%02x]\n",ucResult);

#if 1
	util_Printf("G_NORMALTRANS_ucInputMode= %02x\n",G_NORMALTRANS_ucInputMode);
	util_Printf("G_NORMALTRANS_euCardSpecies= %02x\n",G_NORMALTRANS_euCardSpecies);
	util_Printf("G_RUNDATA_ucReadCardFlag= %02x\n",G_RUNDATA_ucReadCardFlag);
	util_Printf("G_NORMALTRANS_ucTransType= %02x\n",G_NORMALTRANS_ucTransType);
#endif
util_Printf("\n.0099.008.G_EXTRATRANS_ucInputPINLen..........[%02x][%d].\n",ucResult,G_EXTRATRANS_ucInputPINLen);

	//银联芯片卡处理
	if( ucResult == SUCCESS
		&&G_NORMALTRANS_euCardSpecies==CARDSPECIES_EMV
		&&(G_NORMALTRANS_ucInputMode!=TRANS_INPUTMODE_HB_PUTCARD)
		&&(G_NORMALTRANS_ucInputMode!=TRANS_INPUTMODE_SAND_PUTCARD))//	&&G_NORMALTRANS_ucInputMode==TRANS_INPUTMODE_INSERTCARD
	{
		util_Printf("\nG_NORMALTRANS_euCardSpecies==CARDSPECIES_EMV");
		if(G_RUNDATA_ucReadCardFlag == EMVFIRSTMAG)
		{
			util_Printf("\nG_RUNDATA_ucReadCardFlag == EMVFIRSTMAG");
			memcpy(&ExtraTransOnline ,&ExtraTransData, sizeof(EXTRATRANS));
		}
		if(G_NORMALTRANS_ucTransType == TRANS_QUERYBAL
			||G_NORMALTRANS_ucTransType == TRANS_PURCHASE
			||G_NORMALTRANS_ucTransType == TRANS_PREAUTH
			||G_NORMALTRANS_ucTransType == TRANS_PREAUTHFINISH
			||G_NORMALTRANS_ucTransType == TRANS_EC_ASSIGNLOAD
			||G_NORMALTRANS_ucTransType == TRANS_EC_UNASSIGNLOAD
			||G_NORMALTRANS_ucTransType == TRANS_EC_CASHLOAD
			||G_NORMALTRANS_ucTransType == TRANS_CREDITSALE
			)/*判断交易类型*/
			{
				util_Printf("流水WWW [%d]\n",G_NORMALTRANS_ulTraceNumber);
				ucResult = EMVTransProcess();/*ICC卡EMV处理*/
				util_Printf("EMV=%02x\n",ucResult);
			}
		else
		if(G_NORMALTRANS_ucTransType == TRANS_VOIDPURCHASE
			||G_NORMALTRANS_ucTransType == TRANS_VOIDPREAUTHFINISH
			||G_NORMALTRANS_ucTransType == TRANS_REFUND
			||G_NORMALTRANS_ucTransType == TRANS_VOIDPREAUTH
			||G_NORMALTRANS_ucTransType == TRANS_PREAUTHSETTLE
			||G_NORMALTRANS_ucTransType==TRANS_OFFPREAUTHFINISH
			||G_NORMALTRANS_ucTransType==TRANS_PREAUTHADD
			||G_NORMALTRANS_ucTransType==TRANS_CREDITSREFUND
			||G_NORMALTRANS_ucTransType==TRANS_CREDITSVOID
			)
		{
			if(G_NORMALTRANS_ucInputMode==TRANS_INPUTMODE_INSERTCARD)/**/
				ucResult = EMV_SimpleProcess();
			if(!ucResult)
			{
				switch( G_NORMALTRANS_ucTransType )
				{
					case TRANS_CREDITSVOID:
					case TRANS_VOIDPURCHASE:
					case TRANS_VOIDPREAUTH:
					case TRANS_VOIDPREAUTHFINISH:
						if(DataSave0.ConstantParamData.ucVoidInputPin=='1')
						{
							ucResult = UTIL_InputEncryptPIN(ucPindata ,&ucPinlen);
							if(ucResult==EMVERROR_BYPASS)
								ucResult =SUCCESS;
						}
						break;
					default:
						break;
				}
			}
			if(!ucResult&&G_NORMALTRANS_ucTransType != TRANS_OFFPREAUTHFINISH)
			{
				ucResult = TRANS_OnlineProcess();
			}
		}
		util_Printf("FallBack＝%02x %02x\n",G_NORMALTRANS_ucFallBackFlag,ucResult);
		if(G_NORMALTRANS_ucTransType == TRANS_EC_ASSIGNLOAD
			||G_NORMALTRANS_ucTransType == TRANS_EC_UNASSIGNLOAD
			||G_NORMALTRANS_ucTransType == TRANS_EC_CASHLOAD
			)
			G_NORMALTRANS_ucFallBackFlag =0;
		if(G_NORMALTRANS_ucFallBackFlag)
		{
			if(G_RUNDATA_ucReadCardFlag != EMVFIRSTMAG)
				ucResult=MASAPP_Event(true ,&ucKey);
			else
			{
				memcpy(&ExtraTransData, &ExtraTransOnline ,sizeof(EXTRATRANS));
				G_NORMALTRANS_ucInputMode=TRANS_INPUTMODE_SWIPECARD;
				G_NORMALTRANS_euCardSpecies=CARDSPECIES_MAG;
				ucResult = SUCCESS ;
			}
		}
	}
	util_Printf("\n.0005.Trans_Process...........[%02x]\n",ucResult);
#if 1
	util_Printf("G_NORMALTRANS_ucInputMode= %02x\n",G_NORMALTRANS_ucInputMode);
	util_Printf("G_NORMALTRANS_euCardSpecies= %02x\n",G_NORMALTRANS_euCardSpecies);
	util_Printf("G_RUNDATA_ucReadCardFlag= %02x\n",G_RUNDATA_ucReadCardFlag);
	util_Printf("G_NORMALTRANS_ucTransType= %02x\n",G_NORMALTRANS_ucTransType);
#endif
util_Printf("\n.0099.009.G_EXTRATRANS_ucInputPINLen..........[%02x][%d].\n",ucResult,G_EXTRATRANS_ucInputPINLen);

    //银联非接卡处理
	if(( ucResult == SUCCESS&&G_NORMALTRANS_euCardSpecies==CARDSPECIES_EMV)
		&&(G_NORMALTRANS_ucInputMode==TRANS_INPUTMODE_HB_PUTCARD))
	{
		if(ucNeedInputPIN ==1)
			ucResult = UTIL_InputEncryptPIN(ucPindata ,&ucPinlen);
		if((!ucResult)&&(InterfaceTransData.bForceOnline !=0x00))
			ucResult = TRANS_OnlineProcess();
	}
		util_Printf("\n.0006.Trans_Process...........\n");
util_Printf("\n.0099.010.G_EXTRATRANS_ucInputPINLen..........[%02x][%d].\n",ucResult,G_EXTRATRANS_ucInputPINLen);

	//银联非接卡处理，杉德读卡器(处理信用支付交易)
	if(( ucResult == SUCCESS&&G_NORMALTRANS_euCardSpecies==CARDSPECIES_EMV)
		&&(G_NORMALTRANS_ucInputMode==TRANS_INPUTMODE_SAND_PUTCARD)
		&&(G_NORMALTRANS_ucTransType==TRANS_CREDITSALE
				||G_NORMALTRANS_ucTransType==TRANS_CREDITSVOID))
	{
		ucPinlen=0;
		memset(ucPindata,0,sizeof(ucPindata));
		ucResult = UTIL_InputEncryptPIN(ucPindata ,&ucPinlen);
		if (!ucResult)
			ucResult = TRANS_OnlineProcess();
	}
		util_Printf("\n.0007.Trans_Process...........\n");
util_Printf("\n.0099.011.G_EXTRATRANS_ucInputPINLen..........[%02x][%d].\n",ucResult,G_EXTRATRANS_ucInputPINLen);
	//银联磁条卡处理
	if( (ucResult == SUCCESS )
		&&(G_NORMALTRANS_euCardSpecies!=CARDSPECIES_EMV)
		&&(G_NORMALTRANS_euCardSpecies!=CARDSPECIES_UNTOUCH)
		)
	{
		util_Printf("\n.0007.Trans_Process..........[%02x].\n",G_NORMALTRANS_ucTransType);
			
		switch( G_NORMALTRANS_ucTransType )
		{
		case TRANS_QUERYBAL:
		case TRANS_PURCHASE:
		case TRANS_PREAUTH:
		case TRANS_CREDITSALE:
		case TRANS_MOBILEAUTH:		//手机预约消费
			ucResult = UTIL_InputEncryptPIN(ucPindata ,&ucPinlen);
			if(!ucResult)
			{
				ucResult = TRANS_OnlineProcess();
			}
			break;
		case TRANS_UNDOOFF:
		case TRANS_TIPADJUST:
		case TRANS_OFFPREAUTHFINISH:
			break;
		default:
			if(G_NORMALTRANS_ucOldTransType==TRANS_OFFPURCHASE)
			{
				G_NORMALTRANS_ucTransType=TRANS_VOIDOFFPURCHASE;
			}
			switch( G_NORMALTRANS_ucTransType )
			{
				case TRANS_PURCHASE:
				case TRANS_TIPADJUST:
				case TRANS_OFFPREAUTHFINISH:
				case TRANS_REFUND:
				case TRANS_PREAUTHSETTLE:
					break;
				case TRANS_CREDITSVOID:
				case TRANS_VOIDPURCHASE:
				case TRANS_VOIDPREAUTH:
				case TRANS_VOIDMOBILEAUTH:
				case TRANS_VOIDPREAUTHFINISH:
					if(DataSave0.ConstantParamData.ucVoidInputPin=='1')
						ucResult = UTIL_InputEncryptPIN(ucPindata ,&ucPinlen);
					break;
				case TRANS_PREAUTHFINISH:
					if( DataSave0.ConstantParamData.ucPREAUTHFINISHInputPin=='1')
						ucResult = UTIL_InputEncryptPIN(ucPindata ,&ucPinlen);
					break;
				default:
					ucResult = UTIL_InputEncryptPIN(ucPindata ,&ucPinlen);
					break;
			}
			if(!ucResult)
			{
				ucResult = TRANS_OnlineProcess();
			}
			if(ucResult == SUCCESS)
			{
				memset(aucBuf,0,sizeof(aucBuf));
				ucResult = ISO8583_GetBitValue(4,aucBuf,&iLen,sizeof(aucBuf));
				if( ucResult == SUCCESS )
				{
					ulAmount = asc_long(aucBuf,12);

					if(G_NORMALTRANS_ucOldTransType != TRANS_OFFPREAUTHFINISH)
					{
						if(ulAmount != G_NORMALTRANS_ulAmount)
							ucResult = ERR_AMOUNTERR;
					}
					else
						if(ulAmount != (G_NORMALTRANS_ulFeeAmount + G_NORMALTRANS_ulAmount) )
							ucResult = ERR_AMOUNTERR;
				}
			}
			break;
		}
	}
		util_Printf("\n.0008.Trans_Process...........[%02x]\n",ucResult);
util_Printf("\n.0099.012.G_EXTRATRANS_ucInputPINLen..........[%02x][%d].\n",ucResult,G_EXTRATRANS_ucInputPINLen);
	//银联手机芯片卡处理
	if ((ucResult == SUCCESS)
		&&(G_NORMALTRANS_euCardSpecies == CARDSPECIES_UNTOUCH))
	{
		util_Printf("非接触交易参数:類型:%02x 模式:%02x 卡種:%02x\n",G_NORMALTRANS_ucTransType,G_NORMALTRANS_ucInputMode,G_NORMALTRANS_euCardSpecies);
		switch( G_NORMALTRANS_ucTransType )
		{
			case TRANS_CUPMOBILE:
				ucResult = UTIL_InputMobileAmount();
				if (ucResult == SUCCESS)
				{
					memset(ucPindata,0,sizeof(ucPindata));
					ucResult = UTIL_InputEncryptPIN(ucPindata ,&ucPinlen);
					if (ucResult == SUCCESS)
					{
						ucResult = TRANS_OnlineProcess();
					}
				}
				break;
			case TRANS_VOIDCUPMOBILE:
				ucResult = MAG_ProUntouchCard();
				if (ucResult == SUCCESS)
				{
					memset(ucPindata,0,sizeof(ucPindata));
					ucResult = UTIL_InputEncryptPIN(ucPindata ,&ucPinlen);
					if (ucResult == SUCCESS)
						ucResult = TRANS_OnlineProcess();
				}
				else
				{
					return (ERR_CANCEL);
				}
				break;
			default:
				break;
		}
	}
		util_Printf("\n.0009.Trans_Process..........[%02x]\n",ucResult);

	/*GET the current data and time*/
	if((ucResult == SUCCESS ))
	{
		switch( G_NORMALTRANS_ucTransType )
		{
		case TRANS_UNDOOFF:
		case TRANS_TIPADJUST:
		case TRANS_OFFPREAUTHFINISH:
		case TRANS_OFFPURCHASE:
#ifdef USEUPDATADATATIME
			memset(DateTimeBuf,0,sizeof(DateTimeBuf));
			Os__read_date(DateTimeBuf);
			asc_bcd(aucBuf,3,DateTimeBuf,6);
			G_NORMALTRANS_aucDate[0] = 0x20;
			G_NORMALTRANS_aucDate[1] = aucBuf[2];
			G_NORMALTRANS_aucDate[2] = aucBuf[1];
			G_NORMALTRANS_aucDate[3] = aucBuf[0];
			memset(DateTimeBuf,0,sizeof(DateTimeBuf));
			Os__read_time_sec(DateTimeBuf);
			asc_bcd(aucBuf,3,DateTimeBuf,6);
			aucBuf[3]=0;
			memcpy(G_NORMALTRANS_aucTime,aucBuf,3);
#else
			memset(DateTimeBuf,0,sizeof(DateTimeBuf));
			Os__read_date(aucBuf);
			DateTimeBuf[0] = 0x20;
			asc_bcd(&DateTimeBuf[1],1,&aucBuf[4],2);
			LOGON_Dispatch_DateTime(&DateTimeBuf[2]);
			memcpy(G_NORMALTRANS_aucDate,DateTimeBuf,4);
			Os__read_time_sec(aucBuf);
			memcpy(G_NORMALTRANS_aucTime,&DateTimeBuf[4],2);
			asc_bcd(&G_NORMALTRANS_aucTime[2],1,&aucBuf[4],2);
			util_Printf("\n--2--G_NORMALTRANS_aucDate Time =");
			{
				int i;
				for(i=0;i<13;i++)
					util_Printf("%02x ",DateTimeBuf[i] );
				util_Printf("\n");
			}
#endif
			break;
		default:
			break;
		}
	}
		util_Printf("\n.0010.Trans_Process..........[%02x]\n",ucResult);

	/* Store Trans */
	util_Printf("G_NORMALTRANS_ucTransType 12=%02x\n",G_NORMALTRANS_ucTransType);
	if( ucResult == SUCCESS )
	{
		switch( G_NORMALTRANS_ucTransType )
		{
		case TRANS_QUERYBAL:
			break;
		case TRANS_CREDITSVOID:
		case TRANS_CREDITSALE:
		case TRANS_MOBILEAUTH:
		case TRANS_VOIDMOBILEAUTH:
		case TRANS_REFUND:
		case TRANS_UNDOOFF:
		case TRANS_PREAUTH:
		case TRANS_PURCHASE:
		case TRANS_TIPADJUST:
		case TRANS_CUPMOBILE:
		case TRANS_PREAUTHADD:
		case TRANS_OFFPURCHASE:
		case TRANS_VOIDPREAUTH:
		case TRANS_VOIDPURCHASE:
		case TRANS_VOIDCUPMOBILE:
		case TRANS_PREAUTHFINISH:
		case TRANS_PREAUTHSETTLE:
		case TRANS_VOIDOFFPURCHASE:
		case TRANS_OFFPREAUTHFINISH:
		case TRANS_VOIDPREAUTHFINISH:
		case TRANS_EC_ASSIGNLOAD:
		case TRANS_EC_UNASSIGNLOAD:
		case TRANS_EC_CASHLOAD:
			ucResult = SAV_TransSave();
			break;
		default:
			break;
		}
	}
	if(!DataSave0.ChangeParamData.ucPackgeHeadEve)
		COMMS_FinComm();
	util_Printf("G_NORMALTRANS_ulTraceNumber 1111 [%d]\n",G_NORMALTRANS_ulTraceNumber);
		util_Printf("\n.0011.Trans_Process............[%02x]\n",ucResult);
		
	util_Printf("\n==1===ucResult = %02X\n",ucResult);
	if(( ucResult == SUCCESS )&&(DataSave0.ConstantParamData.ucCashFlag != 0x31))
	{
		switch( G_NORMALTRANS_ucTransType )
		{
		case TRANS_QUERYBAL:
			break;
		case TRANS_CREDITSVOID:
		case TRANS_CREDITSALE:
		case TRANS_MOBILEAUTH:
		case TRANS_VOIDMOBILEAUTH:
		case TRANS_UNDOOFF:
		case TRANS_PURCHASE:
		case TRANS_VOIDPURCHASE:
		case TRANS_PREAUTH:
		case TRANS_VOIDPREAUTH:
		case TRANS_PREAUTHFINISH:
		case TRANS_PREAUTHSETTLE:
		case TRANS_VOIDPREAUTHFINISH:
		case TRANS_OFFPREAUTHFINISH:
		case TRANS_REFUND:
		case TRANS_TIPADJUST:
		case TRANS_VOIDOFFPURCHASE:
		case TRANS_OFFPURCHASE:
		case TRANS_PREAUTHADD:
		case TRANS_CUPMOBILE:
		case TRANS_VOIDCUPMOBILE:
		case TRANS_EC_ASSIGNLOAD:
		case TRANS_EC_UNASSIGNLOAD:
		case TRANS_EC_CASHLOAD:
#ifdef GUI_PROJECT
	    		       PRINT_Ticket(NORMAL_PRINT_FLAG,ucCopy);
#else
			if( G_NORMALTRANS_ucTransType == TRANS_TIPADJUST )
			{
				//Os__clr_display(255);
				//Os__GB2312_display(1,0,(uchar *)"    调整成功");
				//Os__GB2312_display(2,0,(uchar *)"    正在打印");
			}
			else
			{
				if(G_NORMALTRANS_ucInputMode != TRANS_INPUTMODE_INSERTCARD
					&&G_NORMALTRANS_ucInputMode != TRANS_INPUTMODE_PUTCARD)
				{
					//Os__clr_display(255);
					//Os__GB2312_display(1,0,(uchar *)"    交易成功");
					//Os__GB2312_display(2,0,(uchar *)"    正在打印");
				}
			}

			if (G_NORMALTRANS_ucInputMode == TRANS_INPUTMODE_HB_PUTCARD
				||G_NORMALTRANS_ucInputMode == TRANS_INPUTMODE_SAND_PUTCARD)
			{
				//Os__clr_display(255);
				//Os__GB2312_display(1,0,(uchar *)"    交易成功");
				//Os__GB2312_display(2,0,(uchar *)"    正在打印");
				//Os__GB2312_display(3,0,(uchar *)"     请取卡");
			}
			if(G_NORMALTRANS_ucInputMode == TRANS_INPUTMODE_INSERTCARD)
			{
				//Os__clr_display(255);
				//Os__GB2312_display(1,0,(uchar *)"    交易成功");
				//Os__GB2312_display(2,0,(uchar *)"    正在打印");
				//Os__GB2312_display(3,0,(uchar *)"     请拔卡");
			}
			util_Printf("打印次数3333 = %02x,%d\n",PRINTTIMES,ucCopy);
			for(ucCopy=0,ucI=0;ucI<PRINTTIMES;ucI++,ucCopy++)
			{
				if(ucI)
				{
					//Os__clr_display(255);
					//Os__GB2312_display(1,0,(uchar *)"    请撕小票");
					//Os__GB2312_display(2,0,(uchar *)"按任意键继续打印");
					MSG_WaitKey(5);
				}
				if(ucI)
				{
					//Os__clr_display(255);
					//Os__GB2312_display(1,0,(uchar *)" 正在打印....");
				}
	    		       PRINT_Ticket(NORMAL_PRINT_FLAG,ucCopy);
			}
#endif			
			break;
		default:
			break;
		}
	}
	util_Printf("===2==ucResult = %02X\n",ucResult);
	if( (G_NORMALTRANS_ucTransType == TRANS_OFFPREAUTHFINISH)		/*离线未上送调整交易,流水号不变*/
	      &&(G_NORMALTRANS_ucTIPAdjustFlag))
      {
      }
	else
	{
		if (DataSave0.ConstantParamData.ucCashFlag != 0x31)
		{
			UTIL_IncreaseTraceNumber();	/*离线及调整在存储时流水号已加，上送时则不加*/
		}
	}
	util_Printf("流水32323[%d]\n",G_NORMALTRANS_ulTraceNumber);
	
	if (!ucResult
	&&(G_NORMALTRANS_ucTransType != TRANS_TIPADJUST)
	  &&(G_NORMALTRANS_ucTransType != TRANS_OFFPREAUTHFINISH)
	  &&(G_NORMALTRANS_ucTransType != TRANS_OFFPURCHASE))
	{
		ucResult1 = Trans_SendICOfflineTrans();
	}

	if(DataSave0.ConstantParamData.ucCashFlag == 0x31)
	{
		   if(  (G_NORMALTRANS_ucTransType != TRANS_TIPADJUST)
			   &&(G_NORMALTRANS_ucTransType != TRANS_OFFPREAUTHFINISH))
			{
				if(DataSave0.ChangeParamData.ucPackgeHeadEve)
				{
					ucResult1 = EMV_PackgeHeadEveProcess(DataSave0.ChangeParamData.ucPackgeHeadEve);
					if(ucResult1 == SUCCESS)
					{
						Os__saved_set(&DataSave0.ChangeParamData.ucPackgeHeadEve,0,1);
					}
					XDATA_Write_Vaild_File(DataSaveChange);
				}
			}
	}else
	{
	   if(  (G_NORMALTRANS_ucTransType != TRANS_TIPADJUST)
		   &&(G_NORMALTRANS_ucTransType != TRANS_OFFPREAUTHFINISH))
		{
			if(DataSave0.ChangeParamData.ucPackgeHeadEve)
			{
				ucResult1=EMV_PackgeHeadEveProcess(DataSave0.ChangeParamData.ucPackgeHeadEve);
				if(ucResult1 == SUCCESS)
				{
					Os__saved_set(&DataSave0.ChangeParamData.ucPackgeHeadEve,0,1);
				}
				XDATA_Write_Vaild_File(DataSaveChange);
			}
		}
	}
	/*处理报头下发要求*/
	COMMS_FinComm();
	if(ReaderSupport&&(ReaderType==READER_HONGBAO)&&(G_NORMALTRANS_ucInputMode==TRANS_INPUTMODE_HB_PUTCARD))
	{
		util_Printf("\nINTERFACE_RefurbishReaderLCD(4)");
		INTERFACE_RefurbishReaderLCD(4);//交易完成
	}
	util_Printf("==3===ucResult = %02X\n",ucResult);
	if((ucResult != SUCCESS)
		&&(ucResult != ERR_CANCEL)
		&&(ucResult != ERR_END)&&(DataSave0.ConstantParamData.ucCashFlag != 0x31))
	{
		MSG_DisplayErrMsg(ucResult);
		ucResult = ERR_CANCEL;
	}
	util_Printf("流水45455[%d]\n",G_NORMALTRANS_ulTraceNumber);
	return ucResult;
}

unsigned char Trans_VoidCUP(void)
{
	unsigned char ucResult;

	if(DataSave0.ConstantParamData.ucCashFlag == 0x31)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"一体机模式下");
		//Os__GB2312_display(1,0,(uchar *)"该交易不支持!");
		MSG_WaitKey(3);
		ucResult = ERR_CANCEL;
		return(ucResult);
	}
	ucResult = UTIL_Check_BasetSet();
	if (ucResult == SUCCESS)
		return(ERR_CANCEL);

	G_NORMALTRANS_ucTransType = TRANS_VOIDCUPMOBILE;
	G_NORMALTRANS_ucInputMode = TRANS_INPUTMODE_PUTCARD;
	G_NORMALTRANS_euCardSpecies = CARDSPECIES_UNTOUCH;
	ucResult = Trans_Process(0);
	return(ucResult);
}
//#define test_1
unsigned char TRANS_OnlineProcess(void)
{
	NORMALTRANS TransDataOfOnLine;
	EXTRATRANS    ExtraTransOnline;
	RUNDATA	RunDataOnline;
	unsigned char ucResult = SUCCESS,ucReFile=SUCCESS,ucTransType;

	util_Printf("\nG_NORMALTRANS_ucFallBackFlag=2009-10-20 11:14=%d",G_NORMALTRANS_ucFallBackFlag);
#ifndef test_1
	if(DataSave0.ChangeParamData.ucScriptInformValid)//上送脚本
	{
		ucResult=Trans_IssuerScriptInformSend();
		if(ucResult) return ucResult;

	}
	util_Printf("\n判断是否有冲正\n");
	util_Printf("DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid---121--- = %02x\n",DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid);
	if( DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid == DATASAVEVALIDFLAG)
	{
		ucTransType = G_NORMALTRANS_ucTransType;
		G_NORMALTRANS_ucTransType=TRANS_REVERSAL;
		util_Printf("\n冲正.........................................................................开始\n");
		ucResult=Trans_SendReversalTrans();
		util_Printf("----交易类型:%02x\n",G_NORMALTRANS_ucTransType);
		util_Printf("\n冲正..............................%02x..................................结束\n",ucResult);
		if(ucResult != SUCCESS )
		    return ucResult;
		G_NORMALTRANS_ucTransType = ucTransType;
	}
	else
	{
		Os__saved_set(&DataSave0.Trans_8583Data.ReversalISO8583Data.ucAuthFlag,0,1);
		ucReFile = XDATA_Write_Vaild_File(DataSaveTrans8583);
		if (ucReFile != SUCCESS)
		{
			MSG_DisplayErrMsg(ucReFile);
			return(ERR_CANCEL);
		}
	}

	memcpy(&TransDataOfOnLine ,&NormalTransData, sizeof(NORMALTRANS));
	memcpy(&ExtraTransOnline ,&ExtraTransData, sizeof(EXTRATRANS));
	memcpy(&RunDataOnline ,&RunData, sizeof(RUNDATA));

	ucResult = Trans_MagOffLineSettle(true);

	util_Printf("\n Trans_MagOffLineSettle :%02x",ucResult);
	
	memcpy(&NormalTransData, &TransDataOfOnLine ,sizeof(NORMALTRANS));
	memcpy(&ExtraTransData, &ExtraTransOnline ,sizeof(EXTRATRANS));
	memcpy(&RunData, &RunDataOnline ,sizeof(RUNDATA));
#endif

	if(ucResult == SUCCESS)
	{
		ucResult = TRANS_Online();
	}
	return ucResult;
}


unsigned char  Trans_IssuerScriptInformSend(void)
{

	unsigned char aucBuf[500],ucResult=SUCCESS,ucI;

	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char*)"正在发送脚本");
	Os__xdelay(5);
	ucI=0;
	do
	{
		ISO8583_RestoreISO8583Data((unsigned char *)&ISO8583Data,
				(unsigned char *)&DataSave0.Trans_8583Data.ScriptInform8583Data);

		memset(aucBuf,0,sizeof(aucBuf));
		long_asc(aucBuf,6,&G_NORMALTRANS_ulTraceNumber);
		ISO8583_SetBitValue(11,aucBuf,6);
	 	ucResult=COMMS_TransSendReceive();
		ucI++;

	}while( (ucResult!=SUCCESS)
			&&(ucI<DataSave0.ConstantParamData.ucTransRetry));


	 Os__saved_set((unsigned char *)(&DataSave0.ChangeParamData.ucScriptInformValid),
				0, sizeof(unsigned char));

	 UTIL_IncreaseTraceNumber();
	G_NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
	XDATA_Write_Vaild_File(DataSaveChange);

	return ucResult;
}


unsigned char  Trans_SendReversalTrans(void)
{
	unsigned char ucI;
	unsigned char ucR,ucJ;
	unsigned char ucResult;
	unsigned char aucBuf[512];

	util_Printf("准备冲正.........................................................................\n");
	util_Printf("交易类型:%02x\n",G_NORMALTRANS_ucTransType);
	ucI=0;
	do
    {
		//Os__clr_display(255);
		//Os__GB2312_display(1,0,(unsigned char*)"  正在发送冲正");
		Os__xdelay(50);
		ISO8583_RestoreISO8583Data((unsigned char *)&ISO8583Data,
			(unsigned char *)&DataSave0.Trans_8583Data.ReversalISO8583Data.BackupISO8583Data);
		if(DataSave0.Trans_8583Data.ReversalISO8583Data.ucAuthFlag)
		{
			ISO8583_SetBitValue(38,DataSave0.Trans_8583Data.ReversalISO8583Data.aucAuthData,6);
		}
//		if(G_NORMALTRANS_euCardSpecies==CARDSPECIES_EMV)
		{
		if(DataSave0.Trans_8583Data.ReversalISO8583Data.uiICTransInfoLen)
		{
			ISO8583_RemoveBit(55);
			ISO8583_SetBitValue(55,DataSave0.Trans_8583Data.ReversalISO8583Data.aucICTransInfo,DataSave0.Trans_8583Data.ReversalISO8583Data.uiICTransInfoLen);
		}
		}
		ISO8583_SetBitValue(39,DataSave0.Trans_8583Data.ReversalISO8583Data.ucRevealEvent,2);

		util_Printf("是否支持前置转发:%02x\n",DataSave0.ConstantParamData.ucMBFEFlag);
        if (DataSave0.ConstantParamData.ucMBFEFlag==0x31
        && DataSave0.ConstantParamData.ucSendOrder=='1')
        {
            //冲正时，将本次订单号保存在临时变量中
            memset(G_RUNDATA_aucMBFEBuf,0,sizeof(G_RUNDATA_aucMBFEBuf));
            memcpy(G_RUNDATA_aucMBFEBuf,G_NORMALTRANS_aucOrdersNo,TRANS_ORDERSNO);
            util_Printf("冲正前订单号_N:%s\n",G_NORMALTRANS_aucOrdersNo);
            util_Printf("冲正前订单号_R:%s\n",G_RUNDATA_aucMBFEBuf);

            //将需要冲正的数据取出
            memset(G_NORMALTRANS_aucOrdersNo,0,sizeof(G_NORMALTRANS_aucOrdersNo));
            util_Printf("---冲正中订单号_N:%s\n",G_NORMALTRANS_aucOrdersNo);
            util_Printf("冲正中订单号_N:%s\n",xDATA_Change.aucMBFEBuf);
            memcpy(G_NORMALTRANS_aucOrdersNo,xDATA_Change.aucMBFEBuf,TRANS_ORDERSNO);

            util_Printf("冲正中订单号_N:%s\n",G_NORMALTRANS_aucOrdersNo);

            ISO8583_MBFEClear();
            memset(aucBuf,0,sizeof(aucBuf));
            memcpy(aucBuf,G_NORMALTRANS_aucOrdersNo,TRANS_ORDERSNO);
            ISO8583_MBFESetBitValue(2,aucBuf,TRANS_ORDERSNO);
            ucResult = ISO8583_MBFEPackData(ISO8583_MBFE.aucCommBuf,&ISO8583_MBFE.uiCommBufLen,ISO8583_MAXCOMMBUFLEN);
            util_Printf("冲正时前置转发订单号报文:");
            for (ucJ=0;ucJ<ISO8583_MBFE.uiCommBufLen;ucJ++)
                util_Printf("%02x ",ISO8583_MBFE.aucCommBuf[ucJ]);
            util_Printf("\n");
        }
		ucResult = COMMS_TransSendReceive();
		util_Printf("G_RUNDATA_ucErrorExtCode--01---=%d\nComms_ucResult\n",G_RUNDATA_ucErrorExtCode,ucResult);
		if ( (G_RUNDATA_ucErrorExtCode == 25)	/*规定该两种错误码无需冲正*/
		  ||(G_RUNDATA_ucErrorExtCode == 12)
		  )
		  	ucResult = SUCCESS;
		if ((G_RUNDATA_ucErrorExtCode==98)
			||(G_RUNDATA_ucErrorExtCode==96)
			//||(G_RUNDATA_ucErrorExtCode==100)
			)
		{
			Os__saved_copy((unsigned char *)"06",
				(unsigned char *)(&DataSave0.Trans_8583Data.ReversalISO8583Data.ucRevealEvent),2);
			ucR = XDATA_Write_Vaild_File(DataSaveTrans8583);
			if(ucR != SUCCESS)
			{
				return(ucR);
			}
		}else if(G_RUNDATA_ucErrorExtCode==100)
        {
            Os__saved_copy((unsigned char *)"A0",
				(unsigned char *)(&DataSave0.Trans_8583Data.ReversalISO8583Data.ucRevealEvent),2);
			ucR = XDATA_Write_Vaild_File(DataSaveTrans8583);
			if(ucR != SUCCESS)
			{
				return(ucR);
			}
            return(ERR_CHECKKEY);
        }

		ucI++;
		util_Printf("\nDataSave0.ConstantParamData.ucTransRetry %02x",DataSave0.ConstantParamData.ucTransRetry);
	}while( (ucResult!=SUCCESS)
		 &&(ucI<DataSave0.ConstantParamData.ucTransRetry)
		 );
	if(DataSave0.ConstantParamData.ucTransRetry&&ucI > DataSave0.ConstantParamData.ucTransRetry )
	{
		ucResult = SUCCESS;             //如果交易上送次数达到所设上限，则不上送冲正，直接继续下笔交易/*ERR_END*/
	}
	Os__saved_set(&DataSave0.Trans_8583Data.ReversalISO8583Data.ucAuthFlag,0,1);
	ucR = XDATA_Write_Vaild_File(DataSaveTrans8583);
	if(ucR != SUCCESS)
	{
		return(ucR);
	}
	UTIL_IncreaseTraceNumber();

	G_NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;

	Os__saved_set((unsigned char *)(&DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid),
				0, sizeof(unsigned char));
	memset(&DataSave0.Trans_8583Data.ReversalISO8583Data,0x00,sizeof(REVERSALISO8583));

	ucR = XDATA_Write_Vaild_File(DataSaveTrans8583);
	if (ucR != SUCCESS)
	{
		return(ucR);
	}
	if(ucResult != SUCCESS&&ucResult !=ERR_DIAL)
	{
		if(DataSave0.ConstantParamData.ucPrnErrTicket=='1')
		{
			PRINT_Revesal_Ticket(0);		/*打印冲正不成功警告单*/
		}
		ucResult=SUCCESS;
	}
        if (DataSave0.ConstantParamData.ucMBFEFlag==0x31)
        {
            //冲正后，将本次订单号取出
            memset(G_NORMALTRANS_aucOrdersNo,0,sizeof(G_NORMALTRANS_aucOrdersNo));
            memcpy(G_NORMALTRANS_aucOrdersNo,G_RUNDATA_aucMBFEBuf,TRANS_ORDERSNO);
            util_Printf("冲正后订单号_N:%s\n",G_NORMALTRANS_aucOrdersNo);
            util_Printf("冲正后订单号_R:%s\n",G_RUNDATA_aucMBFEBuf);
        }
	return ucResult;
}

unsigned char Trans_MagOffLineSettle(unsigned char ucReTryFlag)
{
	unsigned char	ucResult,ucReFile,ucI;
	unsigned short	uiTransIndex,uiNII;

	util_Printf("\n**************************************************\n");
	util_Printf("\n	   Send MAG Offline Trans");
	util_Printf("\n**************************************************\n");
	ucResult = SUCCESS;
	if( (ucResult == SUCCESS)
	  &&(G_NORMALTRANS_ucTransType != TRANS_TIPADJUST)
	  &&(G_NORMALTRANS_ucTransType != TRANS_OFFPREAUTHFINISH)
	  )
	{
		if((G_NORMALTRANS_ucTransType == TRANS_SETTLE)
		   ||((G_NORMALTRANS_ucTransType != TRANS_SETTLE)
		   	&&(DataSave0.ConstantParamData.ucOFFLineSendType=='1')))
		{
			for(uiTransIndex=0;uiTransIndex<=DataSave0.ChangeParamData.uiLastTransIndex;uiTransIndex++)
	    		{
	                            GetOldTransData(uiTransIndex);
					OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),
								(unsigned char *)&NormalTransData,
								sizeof(NORMALTRANS));
					if( ( (DataSave1.SaveTrans.ucTransType==TRANS_OFFPREAUTHFINISH)
					    ||(DataSave1.SaveTrans.ucTransType==TRANS_TIPADJUST)
					    ||(DataSave1.SaveTrans.ucTransType==TRANS_TIPADJUSTOK)
					    )
					  &&(((DataSave0.TransInfoData.auiTransIndex[uiTransIndex])&TRANS_NIISENDFINISH) != TRANS_NIISENDFINISH)
					  &&((DataSave0.TransInfoData.auiTransIndex[uiTransIndex])!= 0 )
					  )
					{
						G_EXTRATRANS_ucInputPINLen=0;

						ucI = 0;
						do{
							//util_Printf("G_NORMALTRANS_ulTraceNumber --11--55--= %d \n",G_NORMALTRANS_ulTraceNumber);
							ucResult = TRANS_Online();
							//util_Printf("G_NORMALTRANS_ulTraceNumber --11--66--= %d \n",G_NORMALTRANS_ulTraceNumber);
							ucI++;
							util_Printf("上送脫机数据ucResult %02x ucI %02x ",ucResult,ucI);
							if(!ucReTryFlag)
							{
								break;
							}
						}while( (ucResult != SUCCESS)&&((ucResult!=ERR_HOSTCODE)||(G_RUNDATA_ucErrorExtCode == 98))
						      &&(ucI<=DataSave0.ConstantParamData.ucTransRetry)
							);

						uiNII = DataSave0.TransInfoData.auiTransIndex[uiTransIndex]|TRANS_NIISENDFINISH;
						Os__saved_copy((unsigned char *)&uiNII,
							(unsigned char *)&DataSave0.TransInfoData.auiTransIndex[uiTransIndex],
							sizeof(unsigned short));

						if (( !ucReTryFlag&&ucResult)
						   ||((ucI > DataSave0.ConstantParamData.ucTransRetry)&&ucReTryFlag)
						   )
						    G_NORMALTRANS_ucNotSendOkFlag = true;
						else
						    G_NORMALTRANS_ucNotSendOkFlag = false;

						Os__saved_copy((unsigned char *)&NormalTransData,(unsigned char *)&(DataSave1.SaveTrans),sizeof(NORMALTRANS));
						XDATA_Write_SaveTrans_File(uiTransIndex);   //Add at 08-7-21

						util_Printf("G_NORMALTRANS_ucTransType-OffLineSettle-2--=%s\n",G_NORMALTRANS_ucTransType);
						if(ucI > DataSave0.ConstantParamData.ucTransRetry&&ucReTryFlag)
						{
							if(DataSave0.ConstantParamData.ucPrnErrTicket=='1')
							{
								PRINT_OffTrans_Ticket();
							}
						}
						ucResult = SUCCESS;
						ucReFile = XDATA_Write_Vaild_File(DataSaveTransInfo);
						if (ucReFile != SUCCESS)
						{
							MSG_DisplayErrMsg(ucReFile);
							return(ERR_CANCEL);
						}
					}
					Os__saved_swap(DATASAVEPAGE_NORMAL);
			}
		}
	}
	util_Printf("\n	*********	Send MAG Offline Trans   end    *********    ");
	return(ucResult);

}
unsigned char TRANS_SetUntouch55Field(void)
{
	unsigned char ucResult;
	unsigned char ucI;
	unsigned char aucBuf[32],aucDateTimeBuf[16];
	unsigned char aucCardInfor[520],ucCardInforLen;
	unsigned char aucInitBuf[215];
	unsigned short uiInitBufLen,uiLen;



	ucResult = SUCCESS;
	memset(aucBuf,0,sizeof(aucBuf));
	memset(aucDateTimeBuf,0,sizeof(aucDateTimeBuf));
	Os__read_date(aucBuf);
	aucDateTimeBuf[0] = 0x20;
	asc_bcd(&aucDateTimeBuf[1],1,&aucBuf[4],2);
	asc_bcd(&aucDateTimeBuf[2],1,&aucBuf[2],2);
	asc_bcd(&aucDateTimeBuf[3],1,&aucBuf[0],2);

	Os__read_time_sec(aucBuf);
	asc_bcd(&aucDateTimeBuf[4],3,aucBuf,6);

	DrvIn.type = 0x89;
	DrvIn.length = 14;

	DrvIn.xxdata[0] = 0x0D;
	DrvIn.xxdata[1] = 0x80;
	DrvIn.xxdata[2] = 0xF8;
	DrvIn.xxdata[3] = 0x02;
	DrvIn.xxdata[4] = 0x00;
	DrvIn.xxdata[5] = 0x08;

	memcpy(&DrvIn.xxdata[6],aucDateTimeBuf,7);
	memcpy(G_EXTRATRANS_aucReadCardTime,aucDateTimeBuf,7);
	DrvIn.xxdata[13] = 0x80;
#ifdef UnTest
	{
		unsigned char ucI;
		util_Printf("Sand TO PIN Data FOR GBI:");
		for (ucI=0;ucI<13;ucI++)
		{
			util_Printf("%02x ",DrvIn.xxdata[ucI]);
		}
		util_Printf("\n");
	}
#endif
	ucResult = SendReceivePinpad();
#ifdef UnTest
	{
		unsigned char ucI;
		util_Printf("RECE GBI DATA FROM PIN :\n");
		for (ucI=0;ucI<DrvOut.xxdata[1];ucI++)
		{
			util_Printf("%02x ",DrvOut.xxdata[ucI]);
		}
		util_Printf("\n");
	}
	util_Printf("ucResult = %02x\n",ucResult);
#endif
	if (ucResult == SUCCESS)
	{
		if (DrvOut.xxdata[0] == 0x00)
		{
			ucCardInforLen = DrvOut.xxdata[1];
			memcpy(aucCardInfor,&DrvOut.xxdata[2],ucCardInforLen);
			//主账号
			memset(aucInitBuf,0,sizeof(aucInitBuf));
			hex_asc(aucInitBuf,&aucCardInfor[20],20);
#ifdef UnTest
			util_Printf("账号:\n");
			for (ucI=0;ucI<20;ucI++)
				util_Printf("%02x ",aucInitBuf[ucI]);
			util_Printf("\n");
#endif
			for (ucI=0;ucI<20;ucI++)
			{
				if (aucInitBuf[ucI]== 0x46)
				{
					G_NORMALTRANS_ucDispSourceAccLen = ucI;
					memcpy(G_NORMALTRANS_aucSourceAcc,aucInitBuf,G_NORMALTRANS_ucDispSourceAccLen);
					break;
				}
			}
#ifdef UnTest
			util_Printf("主账号:\n");
			for (ucI=0;ucI<G_NORMALTRANS_ucDispSourceAccLen;ucI++)
				util_Printf("%02x ",G_NORMALTRANS_aucSourceAcc[ucI]);
			util_Printf("\n");
#endif
			//二磁道数据
			uiInitBufLen = bcd_long (&aucCardInfor[30],2);
			G_EXTRATRANS_uiISO2Len = uiInitBufLen;
#ifdef UnTest
			util_Printf("uiInitBufLen =%d,%d\n",uiInitBufLen,G_EXTRATRANS_uiISO2Len);
#endif
			if (uiInitBufLen%2)
				uiLen = uiInitBufLen/2+1;
			else
				uiLen = uiInitBufLen/2;
#ifdef UnTest
			util_Printf("uiLen =%d\n",uiLen);
#endif
			memcpy(G_EXTRATRANS_aucISO2,&aucCardInfor[31],uiLen);
#ifdef UnTest
			util_Printf("二磁道Len:%d\n",uiLen);
			for (ucI=0;ucI<uiLen;ucI++)
			{
				util_Printf("%02x ",G_EXTRATRANS_aucISO2[ucI]);
			}
			util_Printf("\n");
#endif
			//三磁道数据
			uiInitBufLen  = bcd_long (&aucCardInfor[50],4);
			G_EXTRATRANS_uiISO3Len = uiInitBufLen;
#ifdef UnTest
			util_Printf("uiInitBufLen 3 = %d,%d\n",uiInitBufLen,G_EXTRATRANS_uiISO3Len);
#endif
			if (uiInitBufLen%2)
				uiLen = uiInitBufLen/2+1;
			else
				uiLen = uiInitBufLen/2;

			memcpy(G_EXTRATRANS_aucISO3,&aucCardInfor[52],uiLen);
#ifdef UnTest
			util_Printf("三磁道数据:\n");
			{
				int i;
				for (i=0;i<uiLen;i++)
					util_Printf("%02x ",G_EXTRATRANS_aucISO3[i]);
				util_Printf("\n");
			}
#endif
			//有效期
			memcpy(G_NORMALTRANS_ucExpiredDate,&aucCardInfor[104],2);
			//过程密钥
			memcpy(G_EXTRATRANS_aucSessionKey,&aucCardInfor[108],8);
#ifdef UnTest
			{
				int i;
				util_Printf("有效期:");
				for (i=0;i<2;i++)
				{
					util_Printf("%02x ",G_NORMALTRANS_ucExpiredDate[i]);
				}
				util_Printf("\n");
				util_Printf("过程密钥数据:");
				for (i=0;i<8;i++)
				{
					util_Printf("%02x ",G_EXTRATRANS_aucSessionKey[i]);
				}
				util_Printf("\n");
			}
#endif
		}
		else
		{
			//Os__clr_display(255);
			//Os__GB2312_display(1,1,(uchar *)" 读GBI文件失败");
			//Os__GB2312_display(2,2,(uchar *)" 请重试");
			UTIL_GetKey(2);
			return (ERR_CANCEL);
		}
	}
	else
	{
		return (ERR_CANCEL);
	}
	return (SUCCESS);
}

unsigned char TRANS_Online(void)
{
	unsigned char ucResult,ucReFile;
	unsigned char aucBuf[1024],aucBuf2[10];
	unsigned char aucAmbuf[50],ucI,ucAmoutbuf[15];
	unsigned char aucBufCard[21];
	unsigned char aucTemp[15];
	unsigned char aucDispBuf[15];
	unsigned char date[7];
	unsigned short uiTagLen;
	short iLen;
	unsigned long  ulAmountTemp;
	unsigned int uiI,uiK;
	unsigned long AuthLen;
	unsigned char aucFBuf[128],aucReversal[256],aucISO2[40];
	unsigned short uiReversalLen;

	util_Printf("\n\n TRANS_Online G_NORMALTRANS_ucTransType:[%02x]\n",G_NORMALTRANS_ucTransType);
	
	ISO8583_Clear();

	/* Set Process code bit 3 */
	memset(aucBuf,0,sizeof(aucBuf));
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_QUERYBAL:
			aucBuf[0] = 0x31;
			break;
		case TRANS_MOBILEAUTH:
		case TRANS_PURCHASE:
		case TRANS_CREDITSALE:
		case TRANS_TIPADJUST:
		case TRANS_TIPADJUSTOK:
		case TRANS_OFFPURCHASE:
		case TRANS_PREAUTHFINISH:
		case TRANS_PREAUTHSETTLE:
		case TRANS_OFFPREAUTHFINISH:
			aucBuf[0] = 0x00;
			break;
		case TRANS_PREAUTH:
		case TRANS_PREAUTHADD:
			aucBuf[0] = 0x03;
			break;
		case TRANS_REFUND:
		case TRANS_CREDITSVOID:
		case TRANS_VOIDPREAUTH:
		case TRANS_VOIDMOBILEAUTH:
		case TRANS_VOIDPURCHASE:
		case TRANS_VOIDPREAUTHFINISH:
			aucBuf[0] = 0x20;
        		break;
		case TRANS_CUPMOBILE:
			aucBuf[0] = 0x00;
			break;
		case TRANS_VOIDCUPMOBILE:
			aucBuf[0] = 0x20;
			break;
		case TRANS_EC_ASSIGNLOAD:
			aucBuf[0] =0x60;
			break;
		case TRANS_EC_UNASSIGNLOAD:
			aucBuf[0] =0x62;
			break;
		case TRANS_EC_CASHLOAD:
			aucBuf[0] =0x63;
			break;
		default:
			break;
	}
	aucBuf[1] = aucBuf[1] | (((G_NORMALTRANS_ucSourceAccType - 0x30) << 4 ) & 0xF0);
	hex_asc(G_RUNDATA_RevChack_ucProcessCode, aucBuf, 6);
	ISO8583_SetBitHexValue(3,aucBuf,3);
	util_Printf("\nG_NORMALTRANS_ucTransType=%d,F3=%02x,%02x,%02x",G_NORMALTRANS_ucTransType,aucBuf[0],aucBuf[1],aucBuf[2]);

	/* Set Amount bit 4 */
	memset(aucBuf,0,sizeof(aucBuf));
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_QUERYBAL:
			break;
		case TRANS_REFUND:
		case TRANS_CREDITSVOID:
		case TRANS_CREDITSALE:
		case TRANS_PREAUTH:
		case TRANS_MOBILEAUTH:
		case TRANS_PURCHASE:
		case TRANS_CUPMOBILE:
		case TRANS_PREAUTHADD:
		case TRANS_VOIDPREAUTH:
		case TRANS_OFFPURCHASE:
		case TRANS_VOIDPURCHASE:
		case TRANS_PREAUTHFINISH:
		case TRANS_PREAUTHSETTLE:
		case TRANS_VOIDCUPMOBILE:
		case TRANS_OFFPREAUTHFINISH:
		case TRANS_VOIDPREAUTHFINISH:
		case TRANS_EC_ASSIGNLOAD:
		case TRANS_EC_UNASSIGNLOAD:
		case TRANS_EC_CASHLOAD:
		case TRANS_VOIDMOBILEAUTH:
			long_asc(aucBuf,12,&G_NORMALTRANS_ulAmount);
			ISO8583_SetBitValue(4,aucBuf,12);
			break;
		case TRANS_TIPADJUST:
		case TRANS_TIPADJUSTOK:
			if(G_NORMALTRANS_ucOldTransType != TRANS_OFFPREAUTHFINISH)
				ulAmountTemp = G_NORMALTRANS_ulFeeAmount + G_NORMALTRANS_ulAmount;/*dp*/
			else
				ulAmountTemp = G_NORMALTRANS_ulAmount;/*dp*/
			long_asc(aucBuf,12,&ulAmountTemp);
			ISO8583_SetBitValue(4,aucBuf,12);
			break;
		default:
			break;
	}

	/* Set system trace audit number bit 11 */
	memset(aucBuf,0,sizeof(aucBuf));
	long_asc(aucBuf,6,&G_NORMALTRANS_ulTraceNumber);
	ISO8583_SetBitValue(11,aucBuf,6);

	/* Set expire data bit 14 */
	if(G_RUNDATA_ucTransType != TRANS_SETTLE)
	{
		switch(G_NORMALTRANS_ucTransType)
		{
			case TRANS_CUPMOBILE:
				if (memcmp(G_NORMALTRANS_ucExpiredDate,"\x00\x00",2))
				{
					ISO8583_SetBitHexValue(14,G_NORMALTRANS_ucExpiredDate,2);
				}
				//需要从卡片中读取,在设置55域数据后设置该域值
				break;
			case TRANS_VOIDCUPMOBILE:
				if (memcmp(G_NORMALTRANS_ucExpiredDate,"\x00\x00",2))
				{
					util_Printf("Set Field 4\n");
					ISO8583_SetBitHexValue(14,G_NORMALTRANS_ucExpiredDate,2);
				}
				break;
			default:
				if(memcmp(G_NORMALTRANS_ucExpiredDate,"\x00\x00",2))
				{
					ISO8583_SetBitHexValue(14,G_NORMALTRANS_ucExpiredDate,2);
					memcpy(G_NORMALTRANS_ucBatchSendExpiredDate , G_NORMALTRANS_ucExpiredDate,2 );
				}
				break;
		}
	}
	else
	{
		if(memcmp(G_NORMALTRANS_ucBatchSendExpiredDate,"\x00\x00",2))
		{
			ISO8583_SetBitHexValue(14,G_NORMALTRANS_ucBatchSendExpiredDate,2);
		}
	}

	/* Set Card Sequence Number bit 23 */
	if(G_NORMALTRANS_euCardSpecies==CARDSPECIES_EMV
		&&(G_NORMALTRANS_ucTransType !=TRANS_OFFPREAUTHFINISH)
		&&(G_NORMALTRANS_ucTransType !=TRANS_TIPADJUST)
		&&(G_NORMALTRANS_ucTransType !=TRANS_TIPADJUSTOK)
		)
	{
		if(G_NORMALTRANS_ucPANSeqExist)
			ISO8583_SetBitHexValue(23,&G_NORMALTRANS_ucPANSeq,sizeof(G_NORMALTRANS_ucPANSeq));
	}

	/* Set Point of Service Condition Mode bit 25 */
	memset(aucBuf,0,sizeof(aucBuf));
	memset(aucBuf,'0',sizeof(aucBuf));
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_MOBILEAUTH:
		case TRANS_VOIDMOBILEAUTH:
			memcpy(aucBuf,(uchar*)"67",2);
			break;
		case TRANS_CREDITSALE:
		case TRANS_CREDITSVOID:
			memcpy(aucBuf,(uchar*)"64",2);
			break;
		case TRANS_PREAUTH:
		case TRANS_PREAUTHFINISH:
		case TRANS_PREAUTHSETTLE:
		case TRANS_VOIDPREAUTH:
		case TRANS_VOIDPREAUTHFINISH:
			aucBuf[1] = '6';
		        break;
		case TRANS_PREAUTHADD:
			aucBuf[0] = '6';
			break;
		case TRANS_CUPMOBILE:
		case TRANS_VOIDCUPMOBILE:
			aucBuf[0] = '0';
			break;
		case TRANS_EC_ASSIGNLOAD:
		case TRANS_EC_UNASSIGNLOAD:
		case TRANS_EC_CASHLOAD:
			aucBuf[0] ='9';
			aucBuf[1] ='1';
			break;
		default:
			aucBuf[1] = '0';
			break;
	}
    ISO8583_SetBitValue(25,aucBuf,2);

	/* Set Point of Service PIN Capture Code bit 26 */
	if(G_NORMALTRANS_ucTransType==TRANS_EC_UNASSIGNLOAD)
	{
		if(G_EXTRATRANS_ucInputPINLen_UnAssign != 0)
		{
			memset(aucBuf,0,sizeof(aucBuf));
			memcpy(aucBuf,"12",2);
			ISO8583_SetBitValue(26,aucBuf,2);

			/* Set bit 52 */
			ISO8583_SetBitHexValue(52,G_EXTRATRANS_aucPINData_UnAssign,8);
			memset(aucBuf,0,sizeof(aucBuf));
			switch (G_NORMALTRANS_ucTransType)
			{
				case TRANS_CUPMOBILE:
					aucBuf[0] = '1';
					break;
				default:
					aucBuf[0] = '2';
					break;
			}

			if(DataSave0.ConstantParamData.ENCRYPTTYPEParam==ENCRYPTTYPE_SINGLE)
				aucBuf[1] = '0';
			else if(DataSave0.ConstantParamData.ENCRYPTTYPEParam==ENCRYPTTYPE_DOUBLE)
				aucBuf[1] = '6';
			memcpy(&aucBuf[2],"00000000000000",14);
			ISO8583_SetBitValue(53,aucBuf,16);
		}
	}else if (G_NORMALTRANS_ucTransType == TRANS_MOBILEAUTH
		   ||G_NORMALTRANS_ucTransType == TRANS_VOIDMOBILEAUTH)
	{
		if(G_EXTRATRANS_ucInputPINLen != 0)
		{
			memset(aucBuf,0,sizeof(aucBuf));
			memcpy(aucBuf,"06",2);
			ISO8583_SetBitValue(26,aucBuf,2);

			/* Set bit 52 */
			ISO8583_SetBitHexValue(52,G_EXTRATRANS_aucPINData,8);
			memset(aucBuf,0,sizeof(aucBuf));
			aucBuf[0] = '1';
			if(DataSave0.ConstantParamData.ENCRYPTTYPEParam==ENCRYPTTYPE_SINGLE)
				aucBuf[1] = '0';
			else if(DataSave0.ConstantParamData.ENCRYPTTYPEParam==ENCRYPTTYPE_DOUBLE)
				aucBuf[1] = '6';
			memcpy(&aucBuf[2],"00000000000000",14);
			ISO8583_SetBitValue(53,aucBuf,16);
		}
	}
	else
	{
		if(G_EXTRATRANS_ucInputPINLen != 0)
		{
			memset(aucBuf,0,sizeof(aucBuf));
			memcpy(aucBuf,"12",2);
			ISO8583_SetBitValue(26,aucBuf,2);

			/* Set bit 52 */
			ISO8583_SetBitHexValue(52,G_EXTRATRANS_aucPINData,8);
			memset(aucBuf,0,sizeof(aucBuf));
			switch (G_NORMALTRANS_ucTransType)
			{
				case TRANS_CUPMOBILE:
					aucBuf[0] = '1';
					break;
				default:
					aucBuf[0] = '2';
					break;
			}

			if(DataSave0.ConstantParamData.ENCRYPTTYPEParam==ENCRYPTTYPE_SINGLE)
				aucBuf[1] = '0';
			else if(DataSave0.ConstantParamData.ENCRYPTTYPEParam==ENCRYPTTYPE_DOUBLE)
				aucBuf[1] = '6';
			memcpy(&aucBuf[2],"00000000000000",14);
			ISO8583_SetBitValue(53,aucBuf,16);
		}
	}

    /* set Retrieval Reference Number bit 37 */
	memset(aucBuf,0,sizeof(aucBuf));
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_REFUND:
			ISO8583_SetBitValue(37,G_NORMALTRANS_aucOldRefNumber,12);
			break;
		case TRANS_VOIDCUPMOBILE:
		case TRANS_VOIDPURCHASE:
		case TRANS_VOIDMOBILEAUTH:
		case TRANS_VOIDPREAUTHFINISH:
		case TRANS_TIPADJUST:
		case TRANS_CREDITSVOID:
		case TRANS_TIPADJUSTOK:
			ISO8583_SetBitValue(37,G_NORMALTRANS_aucRefNum,12);
			break;
		default:
			break;
	}

    /* set Authorization code bit 38*/
	memset(aucBuf,0,sizeof(aucBuf));
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_VOIDPREAUTHFINISH:
		case TRANS_PREAUTHFINISH:
			util_Printf("111-38Fildes Len = %d\n",strlen((char*)G_NORMALTRANS_aucAuthCode));
			if(strlen((char*)G_NORMALTRANS_aucOldAuthCode))
				ISO8583_SetBitValue(38,G_NORMALTRANS_aucOldAuthCode,TRANS_AUTHCODELEN);
			break;
		case TRANS_PREAUTHSETTLE:
		case TRANS_VOIDPREAUTH:
		case TRANS_VOIDPURCHASE:
		case TRANS_CREDITSVOID:
		case TRANS_TIPADJUST:
		case TRANS_TIPADJUSTOK:
		case TRANS_PREAUTHADD:
		case TRANS_VOIDCUPMOBILE:
		case TRANS_VOIDMOBILEAUTH:
		case TRANS_OFFPREAUTHFINISH:
			if(G_RUNDATA_ucTransType != TRANS_SETTLE)
			{
				if(strlen((char*)G_NORMALTRANS_aucAuthCode))
				{
					AuthLen = strlen((char*)G_NORMALTRANS_aucAuthCode);
					if(AuthLen < 6)
					{
						memset(&G_NORMALTRANS_aucAuthCode[AuthLen],' ',(6-AuthLen));
					}
					ISO8583_SetBitValue(38,G_NORMALTRANS_aucAuthCode,TRANS_AUTHCODELEN);
					memcpy( G_NORMALTRANS_aaucBatchSendAuthCode, G_NORMALTRANS_aucAuthCode, TRANS_AUTHCODELEN);
				}
			}
			else
			{
				if(strlen((char*)G_NORMALTRANS_aaucBatchSendAuthCode))
					ISO8583_SetBitValue(38,G_NORMALTRANS_aaucBatchSendAuthCode,TRANS_AUTHCODELEN);
			}
			break;
		default:
			break;
	}

	ISO8583_SetBitValue(41,G_RUNDATA_aucTerminalID,8);

	/* Set Card Acceptor Identification Code bit 42 */
	ISO8583_SetBitValue(42,G_RUNDATA_aucMerchantID,15);

	if(G_NORMALTRANS_ulFeeAmount)
	{
		memset(aucBuf,0,sizeof(aucBuf));
		long_asc(aucBuf,12,&G_NORMALTRANS_ulFeeAmount);
		ISO8583_SetBitValue(48,aucBuf,12);
	}
	/* Set bit 49 */
	ISO8583_SetBitValue(49,(uchar *)"156",3);

	/* Set PBOC IC Transaction Information bit 55*/
	if (G_NORMALTRANS_euCardSpecies == CARDSPECIES_UNTOUCH)
	{
		util_Printf("\nRead To Set 55 Field  About Untouch Card\n");
		memset(aucBuf,0,sizeof(aucBuf));
		memset(aucFBuf,0,sizeof(aucFBuf));
		memcpy(aucFBuf,"\xDF\x32",2);

		memcpy(&aucFBuf[2],"\x0A",1);    //银联二代系统

		memcpy(&aucFBuf[3],G_NORMALTRANS_aucUntouchSeq,10); //卡片序列号
		memcpy(&aucFBuf[13],"\xDF\x33",2);
		memcpy(&aucFBuf[15],"\x08",1);
		memcpy(&aucFBuf[16],G_EXTRATRANS_aucSessionKey,8);
		memcpy(&aucFBuf[24],"\xDF\x34",2);
		memcpy(&aucFBuf[26],"\x07",1);
		memcpy(&aucFBuf[27],G_EXTRATRANS_aucReadCardTime,7);

		{
			int i;
			for (i=0;i<strlen((char *)aucFBuf);i++)
			{
				util_Printf("%02x ",aucFBuf[i]);
			}
			util_Printf("55域长:%d\n",strlen((char *)aucFBuf));
			util_Printf("\n");
		}
		ucResult = ISO8583_SetBitValue(55,aucFBuf,34);
	}

	if (G_NORMALTRANS_euCardSpecies == CARDSPECIES_EMV)
	{
		if(G_NORMALTRANS_ucTransType==TRANS_PURCHASE
			||G_NORMALTRANS_ucTransType==TRANS_PREAUTH
			||G_NORMALTRANS_ucTransType==TRANS_QUERYBAL
			||G_NORMALTRANS_ucTransType == TRANS_PREAUTHFINISH
			||G_NORMALTRANS_ucTransType == TRANS_EC_ASSIGNLOAD
			||G_NORMALTRANS_ucTransType == TRANS_EC_UNASSIGNLOAD
			||G_NORMALTRANS_ucTransType == TRANS_EC_CASHLOAD
			||G_NORMALTRANS_ucTransType == TRANS_CREDITSALE)
		{
			memset(aucBuf,0,sizeof(aucBuf));
			uiTagLen=0;
			memset(aucReversal,0,sizeof(aucReversal));
			uiReversalLen =0;
			if(G_NORMALTRANS_ucInputMode==TRANS_INPUTMODE_HB_PUTCARD)
			{
				TRANS_SetUntouchField55(aucBuf ,&uiTagLen);
				TRANS_SetUntouchField55_Reversal(aucReversal,&uiReversalLen);
				util_Printf("\nTRANS_SetUntouchField55 uiTagLen=%d",uiTagLen);
			}
			#if SANDREADER
			else if(G_NORMALTRANS_ucInputMode==TRANS_INPUTMODE_SAND_PUTCARD)
			{
				QTRANS_SetField55(FIRSTGACPHASETAG,Msg_EMV_BatchTrans,aucBuf ,&uiTagLen);
				QTRANS_SetField55_Reversal(FIRSTGACPHASETAG,Msg_EMV_BatchTrans,aucReversal,&uiReversalLen);
				util_Printf("\nTRANS_SetField55  uiTagLen=%d",uiTagLen);
			}
			#endif
			else
			{
				TRANS_SetField55(FIRSTGACPHASETAG,Msg_EMV_BatchTrans,aucBuf ,&uiTagLen);
				TRANS_SetField55_Reversal(FIRSTGACPHASETAG,Msg_EMV_BatchTrans,aucReversal,&uiReversalLen);
				util_Printf("\nTRANS_SetField55  uiTagLen=%d",uiTagLen);
			}
			util_Printf("\nuiReversalLen=%d",uiReversalLen);
			util_Printf("\nucResult==abc=%d",ucResult);
			ucResult=ISO8583_SetBitValue(55,aucBuf,uiTagLen);
		}
		else if(G_NORMALTRANS_ucTransType==TRANS_OFFPURCHASE)
			ucResult=ISO8583_SetBitValue(55,G_NORMALTRANS_ucField55Data,G_NORMALTRANS_uiField55Len);
	}
	/* Set bit 60 */
	memset(aucBuf,0,sizeof(aucBuf));
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_QUERYBAL:
			memcpy(aucBuf,"01",2);
			break;
		case TRANS_PREAUTH:
		case TRANS_PREAUTHADD:
			memcpy(aucBuf,"10",2);
			break;
		case TRANS_VOIDPREAUTH:
			memcpy(aucBuf,"11",2);
			break;
		case TRANS_PREAUTHFINISH:
			memcpy(aucBuf,"20",2);
			break;
		case TRANS_VOIDPREAUTHFINISH:
			memcpy(aucBuf,"21",2);
			break;
		case TRANS_PURCHASE:
		case TRANS_CREDITSALE:
		case TRANS_CUPMOBILE:
			memcpy(aucBuf,"22",2);
			break;
		case TRANS_VOIDPURCHASE:
		case TRANS_VOIDCUPMOBILE:
		case TRANS_CREDITSVOID:
			memcpy(aucBuf,"23",2);
			break;
		case TRANS_REFUND:
			memcpy(aucBuf,"25",2);
			memcpy(&aucBuf[8],"000",3);
			break;
		case TRANS_OFFPREAUTHFINISH:
			memcpy(aucBuf,"30",2);
			break;
		case TRANS_TIPADJUST:
			memcpy(aucBuf,"32",2);
			break;
		case TRANS_TIPADJUSTOK:
			memcpy(aucBuf,"34",2);
			break;
		case TRANS_OFFPURCHASE:
			memcpy(aucBuf,"36",2);
			break;
		case TRANS_PREAUTHSETTLE:
			memcpy(aucBuf,"24",2);
			break;
		case TRANS_EC_ASSIGNLOAD:
			memcpy(aucBuf,"45",2);
			break;
		case TRANS_EC_UNASSIGNLOAD:
			memcpy(aucBuf,"47",2);
			break;
		case TRANS_EC_CASHLOAD:
			memcpy(aucBuf,"46",2);
			break;
		case TRANS_MOBILEAUTH:
			memcpy(aucBuf,"54",2);
			break;
		case TRANS_VOIDMOBILEAUTH:
			memcpy(aucBuf,"53",2);
			break;
		default:
			break;
	}
	long_asc(&aucBuf[2],6,&DataSave0.ChangeParamData.ulBatchNumber);
	memcpy(&aucBuf[8],"000",3);
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_CUPMOBILE:
		case TRANS_VOIDCUPMOBILE:
			aucBuf[11] = '6';
			aucBuf[12] = '0';
			break;
		case TRANS_MOBILEAUTH:
		case TRANS_VOIDMOBILEAUTH:
			memcpy(&aucBuf[11],"00",2);
			break;
		default:
			aucBuf[11]='5';
			aucBuf[12]=G_NORMALTRANS_ucFallBackFlag+'0';
			break;
	}
	if((G_NORMALTRANS_ucInputMode==TRANS_INPUTMODE_HB_PUTCARD)
		||(G_NORMALTRANS_ucInputMode==TRANS_INPUTMODE_SAND_PUTCARD))
	{
		aucBuf[11] = '6';
		aucBuf[12] = '0';
	}else if (G_NORMALTRANS_ucInputMode==TRANS_INPUTMODE_INSERTCARD)
		aucBuf[11] = '5';

	util_Printf("\nField 60:%s\n",aucBuf);
	ISO8583_SetBitValue(60,aucBuf,strlen((char*)aucBuf));

	memset(aucBuf,0,sizeof(aucBuf));
	ucI = 0;
	if( (G_NORMALTRANS_ucTransType != TRANS_REFUND)
	  &&(G_NORMALTRANS_ucTransType!= TRANS_OFFPREAUTHFINISH)
	  &&(G_NORMALTRANS_ucTransType!=TRANS_PREAUTHSETTLE)
	  &&(G_NORMALTRANS_ucTransType!=TRANS_VOIDPREAUTH)
	  &&(G_NORMALTRANS_ucTransType!=TRANS_PREAUTHFINISH)
	  &&(G_NORMALTRANS_ucTransType!=TRANS_PREAUTHADD)
	  &&(G_NORMALTRANS_ucTransType!=TRANS_CREDITSALE)
	  )
	{
		long_asc(&aucBuf[ucI],6,&DataSave0.ChangeParamData.ulBatchNumber);
		ucI+=6;
		long_asc(&aucBuf[ucI],6,&G_NORMALTRANS_ulOldTraceNumber);
		ucI+=6;
	}else
	{
		memcpy(&aucBuf[ucI],"000000000000",12);
		ucI+=12;
	}
	if ((G_NORMALTRANS_ucTransType == TRANS_REFUND)
		||(G_NORMALTRANS_ucTransType == TRANS_VOIDPREAUTH)
		||(G_NORMALTRANS_ucTransType == TRANS_PREAUTHFINISH)
		||(G_NORMALTRANS_ucTransType == TRANS_VOIDPREAUTHFINISH)
		||(G_NORMALTRANS_ucTransType == TRANS_PREAUTHSETTLE)
		)
	{
		util_Printf("G_NORMALTRANS_aucOldDate = %02x %02x %02x %02x\n",G_NORMALTRANS_aucOldDate[0],G_NORMALTRANS_aucOldDate[1],G_NORMALTRANS_aucOldDate[2],G_NORMALTRANS_aucOldDate[3]);
		bcd_asc(&aucBuf[ucI], &G_NORMALTRANS_aucOldDate[2], 4);
	}
	else if ((G_NORMALTRANS_ucTransType == TRANS_TIPADJUST)
		||(G_NORMALTRANS_ucTransType == TRANS_TIPADJUSTOK)
		||(G_NORMALTRANS_ucTransType== TRANS_OFFPREAUTHFINISH))
	{
		if(G_RUNDATA_ucTransType != TRANS_SETTLE&&(G_NORMALTRANS_ucTransType== TRANS_OFFPREAUTHFINISH))
			memcpy(&aucBuf[ucI],"0000",4);
		else
		{
			bcd_asc(&aucBuf[ucI], &G_NORMALTRANS_aucOldDate[2], 4);
		}
	}
	else
	{
		memcpy(&aucBuf[ucI],"0000",4);
	}

	ucI+=4;
	{
		aucBuf[16] = 0x30;
		ucI++;
		aucBuf[17] = G_NORMALTRANS_ucOldAuthType;
		ucI++;
	}
	if(G_NORMALTRANS_ucOldAuthType == OldAuthType_TEL)
	{
		memcpy(&aucBuf[ucI],G_NORMALTRANS_aucAuthUintCode,TRANS_UintCodeLEN);
		ucI+=TRANS_UintCodeLEN;
	}
#ifdef TEST
	{
		unsigned char ucJ;
		util_Printf("G_NORMALTRANS_ucTransType=%02x\n",G_NORMALTRANS_ucTransType);
		for(ucJ=0;ucJ<ucI;ucJ++)
			util_Printf("%02x,",aucBuf[ucJ]);
		util_Printf("\n");
	}
#endif

	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_CREDITSVOID:
		case TRANS_VOIDPURCHASE:
		case TRANS_VOIDCUPMOBILE:
		case TRANS_VOIDMOBILEAUTH:
			ISO8583_SetBitValue(61,aucBuf,12);
			break;
		case TRANS_PREAUTHFINISH:
		case TRANS_PREAUTHADD:
		case TRANS_REFUND:
		case TRANS_PREAUTHSETTLE:
		case TRANS_VOIDPREAUTH:
		case TRANS_VOIDPREAUTHFINISH:
			ISO8583_SetBitValue(61,aucBuf,16);
			break;
		case TRANS_OFFPREAUTHFINISH:
		case TRANS_TIPADJUST:
		case TRANS_TIPADJUSTOK:
			util_Printf("aucBuf = %s\n",aucBuf);
			ISO8583_SetBitValue(61,aucBuf,ucI);
			break;
		default:
			break;
	}

	/* Set bit 62 */
	memset(aucBuf,0,sizeof(aucBuf));
	memset(aucBuf,' ',28);
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_MOBILEAUTH:
			memcpy(aucBuf,"90",2);
			memcpy(&aucBuf[2],G_NORMALTRANS_aucMobileNUM,11);
			memcpy(&aucBuf[13],G_NORMALTRANS_aucMobileCHK,6);
			ISO8583_SetBitValue(62,aucBuf,22);
			break;
		case TRANS_CREDITSALE:
			memcpy(aucBuf,G_NORMALTRANS_aucNumStages,2);
			memcpy(&aucBuf[2],G_NORMALTRANS_aucNumProject,strlen((char*)G_NORMALTRANS_aucNumProject));
			if (G_NORMALTRANS_bInstallments)				//一次性支付手续费
				memcpy(&aucBuf[32],"10",2);
			else
				memcpy(&aucBuf[32],"11",2);						//分期支付手续费
			ucResult=ISO8583_SetBitValue(62,aucBuf,34);
			aucBuf[35]=0;
			util_Printf("\n***Stage***Set 62 Bit:%s\n",aucBuf);
			break;
		default:
			break;
	}

	memset(aucBuf,0,sizeof(aucBuf));
	memcpy(aucBuf,G_NORMALTRANS_aucAuthGroup,TRANS_GROUPLEN);
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_REFUND:
			ISO8583_SetBitValue(63,(uchar *)"000",3);
			break;
		case TRANS_OFFPREAUTHFINISH:
		case TRANS_TIPADJUST:
		case TRANS_TIPADJUSTOK:
		case TRANS_OFFPURCHASE:
			ISO8583_SetBitValue(63,aucBuf,3);
			break;
		default:
			break;
	}

	/* Set conditional data */
	memset(aucBuf,0,sizeof(aucBuf));
	memset(aucBuf2,0,sizeof(aucBuf2));
	memcpy(aucBuf,"0000",4);
	memcpy(aucBuf2,"0000",4);

	G_NORMALTRANS_ucInputMode = ProUiFace.UiToPro.CardInfo.ucInputMode;
	
	util_Printf("\n====Set 2 Bit:[%02x][uiISO2Len=%d] [uiISO2Len=%d] [uiISO3Len=%d] [uiISO3Len=%d]\n",
		G_NORMALTRANS_ucInputMode,G_EXTRATRANS_uiISO2Len,G_NORMALTRANS_ucISO2Len,
		G_EXTRATRANS_uiISO3Len,G_NORMALTRANS_ucISO3Len);
	
	if (G_EXTRATRANS_uiISO2Len>37)	G_EXTRATRANS_uiISO2Len=37;
	if (G_NORMALTRANS_ucISO2Len>37)	G_NORMALTRANS_ucISO2Len=37;
	if (G_EXTRATRANS_uiISO3Len>104)	G_EXTRATRANS_uiISO3Len=104;
	if (G_NORMALTRANS_ucISO3Len>104)	G_NORMALTRANS_ucISO3Len=104;
	
	switch(G_NORMALTRANS_ucInputMode)
	{
	case TRANS_INPUTMODE_MANUALINPUT:
		util_Printf("TRANS_INPUTMODE_MANUALINPUT\n");
		memset(aucBufCard,0,sizeof(aucBufCard));
		if(G_NORMALTRANS_ucSourceAccLen%2)
		{
			bcd_asc(&aucBufCard[0],G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen+1);
		}
		else
		{
			bcd_asc(&aucBufCard[0],G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen);
		}
	    ISO8583_SetBitValue(2,aucBufCard,G_NORMALTRANS_ucSourceAccLen);
	    memcpy(aucBuf,"0000",4);
	    aucBuf[1] = '1';
	    break;
	case TRANS_INPUTMODE_SWIPECARD:
		util_Printf("TRANS_INPUTMODE_SWIPECARD\n");
		if( (G_RUNDATA_ucTransType == TRANS_SETTLE)
		  &&((G_NORMALTRANS_ucTransType == TRANS_REFUND)
		  	||(G_NORMALTRANS_ucTransType == TRANS_PREAUTHSETTLE))
		  )
		{
			G_EXTRATRANS_uiISO2Len = G_NORMALTRANS_ucISO2Len;
			G_EXTRATRANS_uiISO3Len = G_NORMALTRANS_ucISO3Len;
			bcd_asc(G_EXTRATRANS_aucISO2,G_NORMALTRANS_aucISO2,G_EXTRATRANS_uiISO2Len);
			bcd_asc(G_EXTRATRANS_aucISO3,G_NORMALTRANS_aucISO3,G_EXTRATRANS_uiISO3Len);
		}
		if( G_EXTRATRANS_uiISO2Len )
		{
		    ISO8583_SetBitValue(35,G_EXTRATRANS_aucISO2,G_EXTRATRANS_uiISO2Len);
		}
		if( G_EXTRATRANS_uiISO3Len )
		{
		    ISO8583_SetBitValue(36,G_EXTRATRANS_aucISO3,G_EXTRATRANS_uiISO3Len);
		}
      		aucBuf[1] = '2';
	    break;
	case TRANS_INPUTMODE_INSERTCARD:
		switch(G_NORMALTRANS_ucTransType)
		{
			case TRANS_EC_UNASSIGNLOAD:
				/////////////////////////////转出方账户信息///////////////
				if( G_NORMALTRANS_ucUAAISOLen2 )
				{
				    ISO8583_SetBitValue(35,G_NORMALTRANS_aucUnAssignAccountISO2,
			    		G_NORMALTRANS_ucUAAISOLen2);
				}
				if( G_NORMALTRANS_ucUAAISOLen3 )
				{
				    ISO8583_SetBitValue(36,G_NORMALTRANS_aucUnAssignAccountISO3,
			    		G_NORMALTRANS_ucUAAISOLen3);
				}
				ISO8583_SetBitValue(2,G_NORMALTRANS_aucUnAssignAccountISO2,G_NORMALTRANS_ucMainAccountLen);
				//////////////////////////////////////////
				memset(aucBufCard,0,sizeof(aucBufCard));
				if(G_NORMALTRANS_ucSourceAccLen%2)
				{
					bcd_asc(&aucBufCard[0],G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen+1);
				}
				else
				{
					bcd_asc(&aucBufCard[0],G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen);
				}
			  	ISO8583_SetBitValue(62,aucBufCard,G_NORMALTRANS_ucSourceAccLen);
			  	aucBuf[1] = '2';
				memcpy(aucBuf2,"05",2);
				break;
			default:
				memset(aucBufCard,0,sizeof(aucBufCard));
				if(G_NORMALTRANS_ucSourceAccLen%2)
				{
					bcd_asc(&aucBufCard[0],G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen+1);
				}
				else
				{
					bcd_asc(&aucBufCard[0],G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen);
				}
			  	ISO8583_SetBitValue(2,aucBufCard,G_NORMALTRANS_ucSourceAccLen);
					memset(aucBuf,0x00,sizeof(aucBuf));
					{
						uchar ucI;
						util_Printf("\n");
						for(ucI=0;ucI<G_NORMALTRANS_ucISO2Len;ucI++)
						   util_Printf("%02x ",G_NORMALTRANS_aucISO2[ucI]);
						}
					if(G_NORMALTRANS_ucISO2Len%2)
			  		bcd_asc( aucISO2, G_NORMALTRANS_aucISO2,G_NORMALTRANS_ucISO2Len+1);
			  	else
			  		bcd_asc( aucISO2, G_NORMALTRANS_aucISO2,G_NORMALTRANS_ucISO2Len);
			  	util_Printf("\nG_EXTRATRANS_uiISO2Len=%d",G_EXTRATRANS_uiISO2Len);
			 	 util_Printf("\nG_NORMALTRANS_ucISO2Len=%d",G_NORMALTRANS_ucISO2Len);
				if( G_EXTRATRANS_uiISO2Len )
				{
					ISO8583_SetBitValue(35,G_EXTRATRANS_aucISO2,
												G_EXTRATRANS_uiISO2Len);
				}else if( G_NORMALTRANS_ucISO2Len )
		    			ISO8583_SetBitValue(35,aucISO2,G_NORMALTRANS_ucISO2Len);
				memset(aucBuf,0x30,4);
				memcpy(aucBuf,"05",2);
				break;
		}
		break;
	case TRANS_INPUTMODE_PUTCARD:
	       util_Printf("非接触卡交易\n");
		util_Printf("\nLen:%02x\n",G_NORMALTRANS_ucDispSourceAccLen);
		util_Printf("第二域:\n");
		{
			int i;
			for (i=0;i<G_NORMALTRANS_ucDispSourceAccLen;i++)
				util_Printf("%02x ",G_NORMALTRANS_aucSourceAcc[i]);
		}
		util_Printf("\n");
		{
			ISO8583_SetBitValue(2,G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucDispSourceAccLen);
		}
		util_Printf("G_EXTRATRANS_aucISO2:\n");
		{
			int i;
			util_Printf("Len2:%d\n",G_EXTRATRANS_uiISO2Len+1);
			for (i=0;i<G_EXTRATRANS_uiISO2Len+1;i++)
			util_Printf("%02x ",G_EXTRATRANS_aucISO2[i]);
		}
		util_Printf("\n");
		if (G_EXTRATRANS_uiISO2Len)
		{
				ISO8583_SetBitValue(35,G_EXTRATRANS_aucISO2,G_EXTRATRANS_uiISO2Len);
		}
		util_Printf("G_EXTRATRANS_aucISO3:\n");
		{
			int i;
			util_Printf("Len3:%d\n",G_EXTRATRANS_uiISO3Len);
			for (i=0;i<G_EXTRATRANS_uiISO3Len+2;i++)
				util_Printf("%02x ",G_EXTRATRANS_aucISO3[i]);
		}
		util_Printf("\n");
		if (G_EXTRATRANS_uiISO3Len)
		{
			ISO8583_SetBitValue(36,G_EXTRATRANS_aucISO3,G_EXTRATRANS_uiISO3Len);
		}
		memset(aucBuf,0,sizeof(aucBuf));
		memcpy(aucBuf,"96",2);
	       break;
	case TRANS_INPUTMODE_HB_PUTCARD:
	case TRANS_INPUTMODE_SAND_PUTCARD:
		util_Printf("\nG_NORMALTRANSd-9876665_ucInputMode=%d",G_NORMALTRANS_ucInputMode);
		memset(aucBufCard,0,sizeof(aucBufCard));
		memset(aucISO2,0,sizeof(aucISO2));
		if(G_NORMALTRANS_ucSourceAccLen%2)
		{
			bcd_asc(&aucBufCard[0],G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen+1);
		}
		else
		{
			bcd_asc(&aucBufCard[0],G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen);
		}
		util_Printf("\nG_NORMALTRANS_ucSourceAccLen=-df-=%d",G_NORMALTRANS_ucSourceAccLen);
		if(G_NORMALTRANS_ucSourceAccLen)
	  		ISO8583_SetBitValue(2,aucBufCard,G_NORMALTRANS_ucSourceAccLen);
	  	bcd_asc( aucISO2, G_NORMALTRANS_aucISO2,G_NORMALTRANS_ucISO2Len);

		util_Printf("\nG_EXTRATRANS_HB_uiISO2Len=%d",G_EXTRATRANS_uiISO2Len);
	 	util_Printf("\nG_NORMALTRANS_ucISO2Len=%d",G_NORMALTRANS_ucISO2Len);

		if( G_EXTRATRANS_uiISO2Len )
		{
			ISO8583_SetBitValue(35,G_EXTRATRANS_aucISO2,G_EXTRATRANS_uiISO2Len);
		}else if( G_NORMALTRANS_ucISO2Len )
		   	ISO8583_SetBitValue(35,aucISO2,G_NORMALTRANS_ucISO2Len);
		aucBuf[1] = '7';
		break;
	default:
	       aucBuf[1] = '0';
	       aucBuf[2] = '1';
		break;
	}


	if(G_NORMALTRANS_ucTransType==TRANS_EC_UNASSIGNLOAD)
	{
		 if(G_EXTRATRANS_ucInputPINLen_UnAssign)
		    aucBuf[2] = '1';
		 else
		    aucBuf[2] = '2';
		 aucBuf2[2] ='2';
	}
	else
	{
		 if(G_EXTRATRANS_ucInputPINLen)
		    aucBuf[2] = '1';
		 else
		    aucBuf[2] = '2';
	}

	if( G_NORMALTRANS_ucTransType == TRANS_TIPADJUSTOK
		|| G_NORMALTRANS_ucTransType == TRANS_TIPADJUST
		|| G_NORMALTRANS_ucTransType == TRANS_OFFPREAUTHFINISH)
		{
			memcpy( aucBuf,"0120",4 );
		}else if (G_NORMALTRANS_ucTransType == TRANS_MOBILEAUTH
		|| G_NORMALTRANS_ucTransType == TRANS_VOIDMOBILEAUTH)
	{
		memset(aucBuf,0,sizeof(aucBuf));
		if(G_EXTRATRANS_ucInputPINLen != 0)
			memcpy(aucBuf,"9210",4);
		else
			memcpy(aucBuf,"9220",4);
	}
	ISO8583_SetBitValue(22,aucBuf,4);

	if(G_NORMALTRANS_ucTransType == TRANS_EC_UNASSIGNLOAD)
		ISO8583_SetBitValue(48,aucBuf2,4);

	/*  set bit 2*/
	memset(aucBuf,0,sizeof(aucBuf));
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_OFFPREAUTHFINISH:
		case TRANS_TIPADJUST:
		case TRANS_TIPADJUSTOK:
			ISO8583_RemoveBit(35);
			ISO8583_RemoveBit(36);
			if(G_NORMALTRANS_ucSourceAccLen%2)
				bcd_asc(aucBuf,G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen+1);
			else
				bcd_asc(aucBuf,G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen);
			ISO8583_SetBitValue(2,aucBuf,G_NORMALTRANS_ucSourceAccLen);
			break;
		case TRANS_MOBILEAUTH:
			ISO8583_RemoveBit(2);
			break;
		default:
			break;
	}

	/* Set MSGID */
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_PREAUTH:
		case TRANS_PREAUTHADD:
		case TRANS_VOIDPREAUTH:
			ISO8583_SetMsgID(100);
			break;
		case TRANS_REFUND:
		case TRANS_TIPADJUST:
		case TRANS_TIPADJUSTOK:
		case TRANS_OFFPREAUTHFINISH:
		case TRANS_PREAUTHSETTLE://YJ
			ISO8583_SetMsgID(220);
			break;
		case TRANS_CUPMOBILE:
		case TRANS_VOIDCUPMOBILE:
			ISO8583_SetMsgID(200);
			break;
		default:
			ISO8583_SetMsgID(200);
			break;
	}

	if(G_RUNDATA_ucTransType != TRANS_SETTLE)
	{
		ISO8583_SetBitValue(64,(uchar *)"0000000000000000",16);
	}
	else
	{
		ISO8583_SetMsgID(320);
	}

    	/* save to backup first */
	ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
							(unsigned char *)&DataSave0.Trans_8583Data.SaveISO8583Data);
	memset(&DataSave0.Trans_8583Data.ReversalISO8583Data,0x00,sizeof(REVERSALISO8583));
	ucReFile = XDATA_Write_Vaild_File(DataSaveTrans8583);
	if (ucReFile != SUCCESS)
	{
		MSG_DisplayErrMsg(ucReFile);
		return(ERR_CANCEL);
	}
	/* Set reversal trans data */
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_PREAUTHADD:
			ISO8583_RemoveBit(61);
			break;
		default:
			break;
	}
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_UNDOOFF:
		case TRANS_QUERYBAL:
	    	case TRANS_REFUND:
		/*脱机交易和小费交易不做冲正 dp 建行 */
		case TRANS_OFFPREAUTHFINISH:
		case TRANS_TIPADJUST:
		case TRANS_TIPADJUSTOK:
		case TRANS_OFFPURCHASE:
		case TRANS_PREAUTHSETTLE:
			break;
		default:
			ISO8583_SetMsgID(400);
			ISO8583_RemoveBit(26);
			ISO8583_RemoveBit(35);
			ISO8583_RemoveBit(36);
			ISO8583_RemoveBit(37);
	  	    ISO8583_RemoveBit(52);
			ISO8583_RemoveBit(53);
			memset(&DataSave0.Trans_8583Data.ReversalISO8583Data,0x00,sizeof(REVERSALISO8583));
			Os__saved_copy((unsigned char *)"06",(unsigned char *)(&DataSave0.Trans_8583Data.ReversalISO8583Data.ucRevealEvent),2);
			ucReFile = XDATA_Write_Vaild_File(DataSaveTrans8583);
			if (ucReFile != SUCCESS)
			{
				MSG_DisplayErrMsg(ucReFile);
				return(ERR_CANCEL);
			}
			Os__saved_copy((unsigned char *)&NormalTransData,
										(unsigned char *)&DataSave0.Trans_8583Data.ReversalTrans,sizeof(NORMALTRANS));
			if(G_NORMALTRANS_ucTransType==TRANS_PREAUTHFINISH)
				ISO8583_RemoveBit(23);
			if (G_NORMALTRANS_euCardSpecies == CARDSPECIES_UNTOUCH)
			{
				ISO8583_RemoveBit(55);
			}
#if	1
			if(G_NORMALTRANS_euCardSpecies==CARDSPECIES_EMV)
			{
				if((G_NORMALTRANS_ucTransType==TRANS_PURCHASE)
					||(G_NORMALTRANS_ucTransType==TRANS_PREAUTH)
					||(G_NORMALTRANS_ucTransType==TRANS_CREDITSALE)
					||(G_NORMALTRANS_ucTransType==TRANS_EC_ASSIGNLOAD)
					||(G_NORMALTRANS_ucTransType==TRANS_EC_UNASSIGNLOAD)
					||(G_NORMALTRANS_ucTransType==TRANS_EC_CASHLOAD))
				{
					//EMV_SaveICTransReversal();
					ISO8583_RemoveBit(55);
					util_Printf("\nuiReversalLendf2222=%d",uiReversalLen);
					ISO8583_SetBitValue(55,aucReversal,uiReversalLen);
				}else
				{
					ISO8583_RemoveBit(55);
				}
			}
		    /* Set reversal bit 61 */
		    if( (G_NORMALTRANS_ucTransType==TRANS_VOIDPURCHASE)
		    	||(G_NORMALTRANS_ucTransType==TRANS_VOIDPREAUTH)
				||(G_NORMALTRANS_ucTransType== TRANS_CREDITSVOID)
		    	||(G_NORMALTRANS_ucTransType==TRANS_VOIDPREAUTHFINISH)
		    	||(G_NORMALTRANS_ucTransType==TRANS_PREAUTHFINISH)
		    	||(G_NORMALTRANS_ucTransType==TRANS_VOIDMOBILEAUTH)
		    	||(G_NORMALTRANS_ucTransType==TRANS_VOIDCUPMOBILE)
		    	)
		    {
				memset( aucBuf,0,sizeof(aucBuf) );
				if( ISO8583_GetBitValue(61, aucBuf, &iLen, sizeof(aucBuf)) != SUCCESS)
				{
					return(ERR_DATAINVALID);
				}
				ISO8583_RemoveBit(61);
				switch( G_NORMALTRANS_ucTransType )
				{
					case TRANS_CREDITSVOID:
					case TRANS_VOIDPURCHASE:
					case TRANS_VOIDCUPMOBILE:
					case TRANS_VOIDMOBILEAUTH:
						long_asc( &aucBuf[6],6,&G_NORMALTRANS_ulTraceNumber);
						ISO8583_SetBitValue(61,aucBuf,12);
						break;
					case TRANS_VOIDPREAUTHFINISH:
						long_asc( &aucBuf[6],6,&G_NORMALTRANS_ulTraceNumber);
						memcpy( &aucBuf[12],"0000",4 );
						ISO8583_SetBitValue(61,aucBuf,16);
						break;
					case TRANS_VOIDPREAUTH:
						long_asc( &aucBuf[0],6,&DataSave0.ChangeParamData.ulBatchNumber);
						long_asc( &aucBuf[6],6,&G_NORMALTRANS_ulTraceNumber);
						memcpy( &aucBuf[12],"0000",4 );
						ISO8583_SetBitValue(61,aucBuf,16);
						break;
					case TRANS_PREAUTHFINISH:
						long_asc( &aucBuf[0],6,&DataSave0.ChangeParamData.ulBatchNumber);
						long_asc( &aucBuf[6],6,&G_NORMALTRANS_ulTraceNumber);
						memcpy( &aucBuf[12],"0000",4 );
						ISO8583_SetBitValue(61,aucBuf,16);
						break;
				}
			}
#endif
			/* Save new reversal */
			G_RUNDATA_ucReversalFlag = 0x31;
			util_Printf("Set冲正标志前=%02x\n",DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid);
			ucResult = ISO8583_SaveReversalISO8583Data();
			util_Printf("Set冲正标志后= %02x\n",DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid);
			if (ucResult)
			{
				return(ucResult);
			}
		      break;
	}
#ifdef TEST
		util_Printf("DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid--666666677--=%02x\n",DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid);
#endif
       if (DataSave0.ConstantParamData.ucMBFEFlag==0x31
        &&(G_NORMALTRANS_ucTransType==TRANS_PURCHASE
            ||G_NORMALTRANS_ucTransType==TRANS_VOIDPURCHASE)
        )
        {
            ISO8583_MBFEClear();
            memset(aucBuf,0,sizeof(aucBuf));
            memcpy(aucBuf,G_NORMALTRANS_aucOrdersNo,TRANS_ORDERSNO);
            ISO8583_MBFESetBitValue(2,aucBuf,TRANS_ORDERSNO);
            ucResult = ISO8583_MBFEPackData(ISO8583_MBFE.aucCommBuf,&ISO8583_MBFE.uiCommBufLen,ISO8583_MAXCOMMBUFLEN);
            /*考虑到交易过程中，可能存在冲正
             *为了正确上送冲正交易数据，现将处
             *理好数据进行保存因为数据小，不建
             *新文件进行保存
            */
            memset(xDATA_Change.aucMBFEBuf,0,sizeof(xDATA_Change.aucMBFEBuf));
            memcpy(xDATA_Change.aucMBFEBuf,&ISO8583_MBFE.aucCommBuf[9],TRANS_ORDERSNO);
            XDATA_Write_Vaild_File(DataSaveChange);
	     util_Printf("前置转发订单号报文:");
	     for (uiI=0;uiI<ISO8583_MBFE.uiCommBufLen;uiI++)
	        util_Printf("%02x ",ISO8583_MBFE.aucCommBuf[uiI]);
	     util_Printf("\n");
	     util_Printf("保存在Chang文件内的数据:");
	     for (uiI=0;uiI<TRANS_ORDERSNO;uiI++)
	        util_Printf("%02x ",xDATA_Change.aucMBFEBuf[uiI]);
	     util_Printf("\n");
        }

	ucResult = COMMS_TransOnline();
	util_Printf("Trans_Online交易通讯返回结果1=%02x\n",ucResult);

	if((ucResult==ERR_DIAL)
		||ucResult==DIAL_BUSY
		||ucResult==DIAL_NO_TONE
		||ucResult==DIAL_NO_CARRIER
		||ucResult==DIAL_LINE_BUSY
		||ucResult==ERR_CANCEL
		)
	{
		memset(&DataSave0.Trans_8583Data.ReversalISO8583Data,0x00,sizeof(REVERSALISO8583));
		ucReFile = XDATA_Write_Vaild_File(DataSaveTrans8583);

		util_Printf("通讯失败后= %02x\n",DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid);
		if (ucReFile != SUCCESS)
		{
			MSG_DisplayErrMsg(ucReFile);
		}
		return ucResult ;
	}
	util_Printf("Trans_Online交易通讯返回结果2=%02x\n",ucResult);
	util_Printf("交易返回-冲正标示= %02x\n",DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid);
	if( ucResult == SUCCESS )
	{
      	       memset(aucBuf,0,sizeof(aucBuf));
		ucResult = ISO8583_GetBitValue(2,aucBuf,&iLen,sizeof(aucBuf));

		if( ucResult == SUCCESS&&G_NORMALTRANS_ucTransType!=TRANS_EC_UNASSIGNLOAD)
		{
			G_NORMALTRANS_ucSourceAccLen = (unsigned char )iLen;
			memset(G_NORMALTRANS_aucSourceAcc,0,sizeof(G_NORMALTRANS_aucSourceAcc));
         		asc_bcd(G_NORMALTRANS_aucSourceAcc,(iLen+1)/2,aucBuf,((iLen+1)/2)*2);
			util_Printf("\n---------------------+++++--------------------------");
			util_Printf("\naucBuf[%d]= %s",iLen,aucBuf);
			{
				int i;
				util_Printf("\nG_NORMALTRANS_aucSourceAcc[%d] = ",G_NORMALTRANS_ucSourceAccLen);
				for(i=0;i<20;i++)
					util_Printf("%02x ",G_NORMALTRANS_aucSourceAcc[i]);
			}
			util_Printf("\n------------------------end----------------------------\n");
		}
		memset(aucBuf,0,sizeof(aucBuf));
		ucResult = ISO8583_GetBitValue(12,aucBuf,&iLen,sizeof(aucBuf));

#ifdef TEST
		util_Printf("ISO8583_GetBitValue12=%02x\n",ucResult);
#endif
		if( ucResult == SUCCESS )
		{
			asc_bcd(G_NORMALTRANS_aucTime,3,aucBuf,6);
		}
		memset(aucBuf,0,sizeof(aucBuf));
		ucResult = ISO8583_GetBitValue(13,aucBuf,&iLen,sizeof(aucBuf));

#ifdef TEST
		util_Printf("ISO8583_GetBitValue13=%02x\n",ucResult);
#endif
		if( ucResult == SUCCESS )
		{
			asc_bcd(&G_NORMALTRANS_aucDate[2],2,aucBuf,4);
			Os__read_date(date);
			asc_bcd(&G_NORMALTRANS_aucDate[1],1,&date[4],2);
			G_NORMALTRANS_aucDate[0] = (G_NORMALTRANS_aucDate[1] > 0x90) ?  0x19 :  0x20 ;
		}
		memset(aucBuf,0,sizeof(aucBuf));
		ucResult = ISO8583_GetBitValue(32,aucBuf,&iLen,sizeof(aucBuf));
#ifdef TEST
		util_Printf("ISO8583_GetBitValue32=%s iLen＝%d\n",aucBuf,iLen);
#endif
		if( ucResult == SUCCESS )
		{
//			asc_hex(G_NORMALTRANS_aucBankID,TRANS_BANKIDLEN,aucBuf,TRANS_BANKIDLEN*2);
			memcpy(G_NORMALTRANS_aucBankID,aucBuf,iLen);
		}
		memset(aucBuf,0,sizeof(aucBuf));
        ucResult = ISO8583_GetBitValue(14,aucBuf,&iLen,sizeof(aucBuf));
#ifdef TEST
		util_Printf("ISO8583_GetBitValue14=%02x\n",ucResult);
#endif
    	if( ucResult == SUCCESS )
		{
			asc_bcd(G_NORMALTRANS_ucExpiredDate,2,aucBuf,4);
		}
		memset(aucBuf,0,sizeof(aucBuf));
        	ucResult = ISO8583_GetBitValue(15,aucBuf,&iLen,sizeof(aucBuf));
#ifdef TEST
		util_Printf("ISO8583_GetBitValue15=%02x\n",ucResult);
#endif
    	if( ucResult == SUCCESS )
		{
			memcpy(G_NORMALTRANS_aucSettleDate,aucBuf,4);
		}
		memset(aucBuf,0,sizeof(aucBuf));
		ucResult = ISO8583_GetBitValue(44,aucBuf,&iLen,sizeof(aucBuf));
#ifdef TEST
		util_Printf("ISO8583_GetBitValue44=%02x\n",ucResult);
#endif
		if(ucResult == SUCCESS&&G_RUNDATA_ucTransType != TRANS_SETTLE)
		{
			memcpy(G_NORMALTRANS_aucIssuerID,aucBuf,ISSUERIDLEN);
			memcpy(G_NORMALTRANS_aucAcquereID,&aucBuf[ISSUERIDLEN],ACQUERELEN);
			memcpy(DataSave0.ChangeParamData.aucAcquereID,G_NORMALTRANS_aucAcquereID,ACQUERELEN);
		}
		memset(aucBuf,0x00,sizeof(aucBuf));
		memcpy(aucBuf,DataSave0.ChangeParamData.aucIssuerID,11);
		util_Printf("Trans IssuserID %s",aucBuf);
		memcpy(aucBuf,DataSave0.ChangeParamData.aucAcquereID,11);
		util_Printf("Trans aucAcquereID %s",aucBuf);
		XDATA_Write_Vaild_File(DataSaveChange);

		memset(aucBuf,0,sizeof(aucBuf));
		ucResult = ISO8583_GetBitValue(37,aucBuf,&iLen,sizeof(aucBuf));
		if( ucResult == SUCCESS )
		{
			util_Printf("\naucBuf=0202=%s",aucBuf);
			memcpy(G_NORMALTRANS_aucRefNum,aucBuf,TRANS_REFNUMLEN);
		}
		memset(aucBuf,0,sizeof(aucBuf));
		memset(G_NORMALTRANS_aucAuthCode,0,sizeof(G_NORMALTRANS_aucAuthCode));
		Os__saved_set(&DataSave0.Trans_8583Data.ReversalISO8583Data.ucAuthFlag,0,1);
		XDATA_Write_Vaild_File(DataSaveTrans8583);
		ucResult = ISO8583_GetBitValue(38,aucBuf,&iLen,sizeof(aucBuf));
		if(ucResult == SUCCESS)
		{
			memcpy(G_NORMALTRANS_aucAuthCode, aucBuf, TRANS_AUTHCODELEN);
			if( (G_NORMALTRANS_ucTransType==TRANS_PREAUTH)
				||(G_NORMALTRANS_ucTransType==TRANS_PURCHASE))
			{
				Os__saved_set(&DataSave0.Trans_8583Data.ReversalISO8583Data.ucAuthFlag,1,1);
				Os__saved_copy(aucBuf,(unsigned char *)(&DataSave0.Trans_8583Data.ReversalISO8583Data.aucAuthData),6);
				XDATA_Write_Vaild_File(DataSaveTrans8583);
			}
		}

		//信用消费处理
		if (G_NORMALTRANS_ucTransType==TRANS_CREDITSALE)
		{
			iLen=0;
			memset(aucBuf,0,sizeof(aucBuf));
			if ((ucResult = ISO8583_GetBitValue(62,aucBuf,&iLen,sizeof(aucBuf)))==SUCCESS)
			{
				util_Printf("信用分期消费返回_数据长度:%d\n",iLen);
				G_NORMALTRANS_ulRepayAmount=asc_long(aucBuf,12);
				memcpy(G_NORMALTRANS_aucCBuf,&aucBuf[12],3);
				G_NORMALTRANS_ulRepayFeeAmt=asc_long(&aucBuf[15],12);

				G_NORMALTRANS_ulStages=asc_long(&aucBuf[27],12);				//奖励积分
				memcpy(&G_NORMALTRANS_ucStagesType,&aucBuf[39],1);			//手续费支付方式
				G_NORMALTRANS_ulFCurrency=asc_long(&aucBuf[40],12);			//首期手续费
				G_NORMALTRANS_ulNCurrency=asc_long(&aucBuf[52],12);		//每期手续费
				util_Printf("手续费支付方5式:%02x\n",G_NORMALTRANS_ucStagesType);
				if (G_NORMALTRANS_ucStagesType=='0')
					G_NORMALTRANS_bInstallments=true;
				else
					G_NORMALTRANS_bInstallments=false;
				util_Printf("手续费支付方式:%02x\n",G_NORMALTRANS_ucStagesType);
				util_Printf("首付:%d\n",G_NORMALTRANS_ulRepayAmount);
				util_Printf("奖励积分:%d\n",G_NORMALTRANS_ulStages);
				util_Printf("手续费:%d\n",G_NORMALTRANS_ulRepayFeeAmt);
				util_Printf("首期手续费:%d\n",G_NORMALTRANS_ulFCurrency);
				util_Printf("每期手续费:%d\n",G_NORMALTRANS_ulNCurrency);
			}
		}

		memset(aucBuf,0,sizeof(aucBuf));
		ucResult = ISO8583_GetBitValue(63,aucBuf,&iLen,sizeof(aucBuf));
		util_Printf("ISO8583_GetBitValue63=%s\n",aucBuf);
		G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_INTERNAL;
		DataSave0.ConstantParamData.ucForeignCard = 0x30;

     if( ucResult == SUCCESS )
	 {
			memcpy(G_NORMALTRANS_aucAuthGroup,aucBuf,TRANS_GROUPLEN);
			memcpy(G_NORMALTRANS_aucCardRemark,&aucBuf[3],TRANS_REMARK);
			util_Printf("63 Filed=%d\n",iLen);
			if(iLen>3)
				G_NORMALTRANS_ucCardRemarkFlag=1;
			else
				G_NORMALTRANS_ucCardRemarkFlag=0;
			switch(G_NORMALTRANS_aucAuthGroup[0])
			{
				case 'C':
					G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_INTERNAL;
					break;
				case 'V':
					DataSave0.ConstantParamData.ucForeignCard = 0x31;
					G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_VISA;
					break;
				case 'M':
					DataSave0.ConstantParamData.ucForeignCard = 0x31;
					G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_MASTER;
					break;
				case 'J':
					DataSave0.ConstantParamData.ucForeignCard = 0x31;
					G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_JCB;
					break;
				case 'D':
					DataSave0.ConstantParamData.ucForeignCard = 0x31;
					G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_DINNER;
					break;
				case 'A':
					DataSave0.ConstantParamData.ucForeignCard = 0x31;
					G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_AE;
					break;
				default:
					G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_INTERNAL;
			}
	}
       if((G_NORMALTRANS_ucTransType == TRANS_QUERYBAL)
	   	||(G_NORMALTRANS_ucTransType == TRANS_PREAUTHADD)
	   )
	{
			memset(aucBuf,0,sizeof(aucBuf));
			memset(aucAmbuf,0,sizeof(aucAmbuf));
			ucResult = ISO8583_GetBitValue(54,aucAmbuf,&iLen,sizeof(aucAmbuf));

			if( ucResult == SUCCESS)
			{
				if(G_NORMALTRANS_ucTransType == TRANS_QUERYBAL)
				{
					memcpy(aucBuf , &aucAmbuf[7] ,13 );
#ifdef TEST
					util_Printf("ISO8583_GetBitValue54=%s\n",aucBuf);
#endif
		   			memset(aucDispBuf,0,sizeof(aucDispBuf));
					memset(ucAmoutbuf,' ',sizeof(ucAmoutbuf));
					memset(aucTemp,0 ,sizeof(aucTemp));
					memcpy(aucTemp,&aucBuf,13);
					//---------金额格式化---------------------------
					uiK=0;
					memcpy(&ucAmoutbuf[1] ,&aucTemp[1] ,10);
					ucAmoutbuf[11] = '.';
					memcpy(&ucAmoutbuf[12] , &aucTemp[11] ,2);
					for(ucI=1;ucI<10;ucI++)
					{
						if(ucAmoutbuf[ucI] == '0')
						{
							ucAmoutbuf[ucI]  = ' ' ;
						}else
						break;
					}
					if(aucTemp[0] !='C' )
					{
						ucAmoutbuf[ucI-1]='-';
					}else
					{
						ucAmoutbuf[ucI-1]=' ';
					}
					//----------END---------------------------------------
					memcpy(aucDispBuf,ucAmoutbuf,14);
#ifdef TEST
					util_Printf("aucDispBuf=%s\n",aucDispBuf);
#endif

#ifdef GUI_PROJECT
					memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
					ProUiFace.ProToUi.uiLines=1;
					memcpy(&ProUiFace.ProToUi.aucLine1[0],(uchar*)"余额(Balance):",14);
					memcpy(&ProUiFace.ProToUi.aucLine1[14],(uchar*)aucDispBuf,strlen((char*)aucDispBuf));
// sleep(1);
    sem_post(&binSem1);
    sem_wait(&binSem2);
#else
					if(DataSave0.ConstantParamData.Pinpadflag ==1)
					{
						#if 1/*新密码键盘*/
						//if((!ReaderSupport)||(ReaderType!=READER_SAND))
						{
							Os__clr_display_pp(255);
							Os__GB2312_display_pp(0,0,(uchar *)"余额(Balance):");
							Os__display_pp(1,0,(unsigned char *)aucDispBuf);
						}
						#else
						Os__clr_display_pp(255);
						Os__GB2312_display_pp(0,0,(uchar *)"余额:");
						Os__display_pp(1,0,(unsigned char *)"Balance:");
						Os__display_pp(2,0,(unsigned char *)aucDispBuf);
						#endif
						//Os__clr_display(255);
						//Os__GB2312_display(1,0,(unsigned char *)"    查询成功");
						//Os__GB2312_display(2,0,(unsigned char *)" 请查看密码键盘");
						COMMS_FinComm();
						{
							UTIL_WaitGetKey(30);
							DispPINPAD_AD();
						}
					}
					else
					{
						//Os__clr_display(255);
						//Os__GB2312_display(0,0,(unsigned char *)"    查询成功");
						//Os__GB2312_display(1,0,(unsigned char *)"可用余额:");
						Os__display(2,0,(unsigned char *)aucDispBuf);						
						COMMS_FinComm();
						UTIL_WaitGetKey(30);
						DispPINPAD_AD();
					}
#endif					
				}else
				if(G_NORMALTRANS_ucTransType == TRANS_PREAUTHADD)
				{
					G_NORMALTRANS_ulAuthAddTotalAmount = asc_long(&aucAmbuf[2],12);
					{
						int i;
						util_Printf("\naucBuf=");
						for(i=0;i<12;i++)
							util_Printf("%02x ",aucAmbuf[i]);
					}
					util_Printf("\n----1---G_NORMALTRANS_ulAuthAddTotalAmount = %d\n",G_NORMALTRANS_ulAuthAddTotalAmount);
				}
			}
		}
		ucResult = SUCCESS;		/*不检查不存在的域*/
	}
	else
	{
		if(ucResult != ERR_COMMS_RECVTIMEOUT)
		{
			memset(aucBuf,0,sizeof(aucBuf));
			if(ISO8583_GetBitValue(38,aucBuf,&iLen,sizeof(aucBuf)) == SUCCESS)
			{
				if( (G_NORMALTRANS_ucTransType==TRANS_PREAUTH)
					||(G_NORMALTRANS_ucTransType==TRANS_PURCHASE))
				{
					Os__saved_set(&DataSave0.Trans_8583Data.ReversalISO8583Data.ucAuthFlag,1,1);
					Os__saved_copy(aucBuf,(unsigned char *)(&DataSave0.Trans_8583Data.ReversalISO8583Data.aucAuthData),6);
					ucReFile = XDATA_Write_Vaild_File(DataSaveTrans8583);
					if (ucReFile != SUCCESS)
					{
						MSG_DisplayErrMsg(ucReFile);
					}
				}
			}
			else
			{
				Os__saved_set(&DataSave0.Trans_8583Data.ReversalISO8583Data.ucAuthFlag,0,1);
				ucReFile = XDATA_Write_Vaild_File(DataSaveTrans8583);
				if (ucReFile != SUCCESS)
				{
					MSG_DisplayErrMsg(ucReFile);
				}
			}

		}
		util_Printf("\n------------------------ucResult = %02x-------11122233--------\n",ucResult);

		if( ucResult == ERR_HOSTCODE )
		{
			if(G_NORMALTRANS_ucInputMode == TRANS_INPUTMODE_INSERTCARD)
				DataSave0.Trans_8583Data.ReversalISO8583Data.ucValid = 0;
			else
				memset(&DataSave0.Trans_8583Data.ReversalISO8583Data,0x00,sizeof(REVERSALISO8583));
			ucReFile = XDATA_Write_Vaild_File(DataSaveTrans8583);
			if (ucReFile != SUCCESS)
			{
				MSG_DisplayErrMsg(ucReFile);
			}
		}
	}

	return(ucResult);
}

unsigned char Trans_OFFLineTrans(void)
{
	unsigned char ucResult;
	unsigned char aucBuf[16];
	unsigned long uLAuthLen;
	unsigned char ucChar,ucLen;
    unsigned char ucInputFlag,ucKey=NULL;
/*
	if(UTIL_Check_BasetSet() == SUCCESS)
		return(SUCCESS);

	if((ucResult = SAV_CheckTransMaxNb()) != SUCCESS)
		return(ucResult);

	ucResult = SEL_GetCardMsg(ucKey);
*/
	ucResult = SUCCESS;
	if(ucResult == SUCCESS)
	{
			//Os__clr_display(255);

			//Os__GB2312_display(0,0,(unsigned char*)"    离线结算");
			//Os__GB2312_display(1,0,(unsigned char*)"选择授权方式：");
			//Os__GB2312_display(2,0,(unsigned char*)"1 POS     2 电话");
			//Os__GB2312_display(3,0,(unsigned char*)"3 小额代授权");

			memset(aucBuf,0,sizeof(aucBuf));
			while(1)
			{
				aucBuf[0] = UTIL_GetKey(10) ;
				if((ucResult == SUCCESS)
					&&((aucBuf[0] == '1')
						||(aucBuf[0] == '2')
						||(aucBuf[0] == '3')
						||(aucBuf[0] == KEY_ENTER))
				  )
				{
					if(aucBuf[0] == KEY_ENTER)
					{
						break;
					}
					else
					{
						//Os__GB2312_display(1,7,(uchar *)aucBuf);
						ucLen = strlen((char *)aucBuf);
						if(ucLen==0)
							aucBuf[0] = '0';
						else
						{
							if(aucBuf[0] == '1')
								aucBuf[0] =OldAuthType_POS;
							if(aucBuf[0] == '2')
								aucBuf[0] =OldAuthType_TEL;
							if(aucBuf[0] == '3')
								aucBuf[0] =OldAuthType_OTHER;
						}
						ucChar = aucBuf[0];
						G_NORMALTRANS_ucOldAuthType = ucChar;
					}
				}else
				{
					if ((aucBuf[0]  == 99) ||(aucBuf[0]  == KEY_CLEAR ))
					{
						ucResult = ERR_CANCEL;
						break;
					}
				}
		}
	}
	if(ucResult == SUCCESS)
	{
		if(ucChar == OldAuthType_TEL)//电话
		{
			//Os__clr_display(255);
			//Os__GB2312_display(0,0,(unsigned char*)"  (离线结算)");
			//Os__GB2312_display(1,0,(unsigned char*)"输入授权机构代码");
			memset(aucBuf,0,sizeof(aucBuf));
			if (UTIL_Input(2,false,TRANS_UintCodeLEN,TRANS_UintCodeLEN,'N',aucBuf,0) != KEY_ENTER )
			{
				ucResult = ERR_CANCEL;
			}
			memcpy(G_NORMALTRANS_aucAuthUintCode,aucBuf,TRANS_UintCodeLEN);
		}
	}
	if(ucResult == SUCCESS)
	{
		if(ucChar != OldAuthType_OTHER)//小额代授权
		{
			//Os__clr_display(255);
			//Os__GB2312_display(0,0,(unsigned char*)"  (离线结算)");
			//Os__GB2312_display(1,0,(unsigned char*)"请输入原授权码");
			memset(aucBuf,0,sizeof(aucBuf));
			ucInputFlag =PY_INPUT_NUMBER
							|PY_INPUT_UPPER_CASE;
			ucResult=PY_Main(aucBuf,2,2,TRANS_AUTHCODELEN,ucInputFlag,300);
			if(ucResult!=SUCCESS)
				return(ucResult);
			else
				memcpy(G_NORMALTRANS_aucAuthCode,aucBuf,TRANS_AUTHCODELEN);
			uLAuthLen = strlen((char*)G_NORMALTRANS_aucAuthCode);
			util_Printf("授权码长度 = %d\n需补[%d]个空格\n",uLAuthLen,(6 - uLAuthLen));
			if(uLAuthLen < 6)
			{
				memset(&G_NORMALTRANS_aucAuthCode[uLAuthLen],' ',(6 - uLAuthLen));
			}
			util_Printf("原授权码 = %s\n",G_NORMALTRANS_aucAuthCode);
/*
			if (OSUTIL_Input(0,2,2,TRANS_AUTHCODELEN,'A',aucBuf,NULL) != SUCCESS )
			{
				ucResult = ERR_CANCEL;
			}
			memcpy(G_NORMALTRANS_aucAuthCode,aucBuf,TRANS_AUTHCODELEN);
*/
			util_Printf("G_NORMALTRANS_aucAuthCode=%s\n",G_NORMALTRANS_aucAuthCode);
		}
	}

	if(ucResult == SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(unsigned char*)"  (离线结算)");
		//Os__GB2312_display(1,0,(unsigned char*)"输入国际组织代码:");
		OSMMI_DisplayASCII(0x30,4,0, (unsigned char*)"1.CUP 			2.VIS ");
		OSMMI_DisplayASCII(0x30,5,0, (unsigned char*)"3.MCC 			4.MAE ");
		OSMMI_DisplayASCII(0x30,6,0,(unsigned char*) "5.JCB 			6.DCC ");
		OSMMI_DisplayASCII(0x30,7,0, (unsigned char*)"7.AMX");

			while(1)
			{
				while(1)
				{

					ucKey = UTIL_GetKey(10) ;
					if ((ucKey  == 99) ||(ucKey  == KEY_CLEAR ))
					{
						return(ERR_CANCEL);
					}
					if((ucResult == SUCCESS)
						&&((ucKey == '1')
							||(ucKey == '2')
							||(ucKey == '3')
							||(ucKey == '4')
							||(ucKey == '5')
							||(ucKey == '6')
							||(ucKey == '7')
							||(ucKey == KEY_ENTER)
							)
					  )
					{
						if(ucKey == KEY_ENTER)
							break;
						else
						{
							memset( aucBuf, 0, sizeof(aucBuf));
							aucBuf[0] = ucKey;
							Os__display(2,14,(uchar *)aucBuf);
						}
					}
				}
				ucLen = strlen((char *)aucBuf);
				if(ucLen == 0)
				{
					memcpy(G_NORMALTRANS_aucAuthGroup,"CUP",TRANS_GROUPLEN);
					break;
				}
				ucChar = aucBuf[0];
				switch(ucChar)
				{
					case '1':
					case KEY_ENTER:
						memcpy(G_NORMALTRANS_aucAuthGroup,"CUP",TRANS_GROUPLEN);
						break;
					case '2':
						memcpy(G_NORMALTRANS_aucAuthGroup,"VIS",TRANS_GROUPLEN);
						break;
					case '3':
						memcpy(G_NORMALTRANS_aucAuthGroup,"MCC",TRANS_GROUPLEN);
						break;
					case '4':
						memcpy(G_NORMALTRANS_aucAuthGroup,"MAE",TRANS_GROUPLEN);
						break;
					case '5':
						memcpy(G_NORMALTRANS_aucAuthGroup,"JCB",TRANS_GROUPLEN);
						break;
					case '6':
						memcpy(G_NORMALTRANS_aucAuthGroup,"DCC",TRANS_GROUPLEN);
						break;
					case '7':
						memcpy(G_NORMALTRANS_aucAuthGroup,"AMX",TRANS_GROUPLEN);
						break;
					default:
						break;
				}
				break;
			}
			switch(G_NORMALTRANS_aucAuthGroup[0])
			{
				case 'V':
					G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_VISA;
					break;
				case 'M':
					G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_MASTER;
					break;
				case 'J':
					G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_JCB;
					break;
				case 'D':
					G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_DINNER;
					break;
				case 'A':
					G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_AE;
					break;
				case 'C':
					G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_INTERNAL;
					break;
				default:
					ucResult = ERR_INTERNOTTIP;
					break;
			}
	}
	G_NORMALTRANS_ucSourceAccType = '0';

	memset(G_NORMALTRANS_aucRefNum,'0',TRANS_REFNUMLEN);
	G_NORMALTRANS_ulOldTraceNumber = G_NORMALTRANS_ulTraceNumber;

	if(ucResult == SUCCESS)
	{
		//ucResult = Trans_Process(0);
	}
	return(ucResult);
}

 /*****************************
 Funcation: To Query trans data

 *****************************/
unsigned char Trans_Query(void)
{
	unsigned char ucResult;
	unsigned short uiIndex,uitransnb;
	
	uitransnb = 0;
	ucResult = UTIL_Is_Trans_Empty();
	if(ucResult != SUCCESS)
	{
   		MSG_DisplayMsgKey( true,1,2,MSG_NOTRANS,3);
		return(SUCCESS);
	}
#ifdef GUI_PROJECT
	//for(uiIndex = 0; uiIndex < TRANS_MAXNB; uiIndex++)
	{
		uiIndex=ProUiFace.UiToPro.uiIndex;
		
util_Printf("\n0000001.Trans_Query uiIndex:=[%03d]...........\n", uiIndex);
	
		if( DataSave0.TransInfoData.auiTransIndex[uiIndex])
		{
			GetOldTransData(uiIndex);
			OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),(unsigned char *)&NormalTransData,sizeof(NORMALTRANS));
			if((DataSave0.TransInfoData.auiTransIndex[uiIndex]&TRANS_NIIVOID) == TRANS_NIIVOID
				||((G_NORMALTRANS_ucTransType == TRANS_PURCHASE)
   		 	  	&&(G_NORMALTRANS_ucTIPAdjustFlag)))   /*void trans */
				SERV_TransList(1);
			else
				SERV_TransList(0);
			//uitransnb ++;			
			//if(uitransnb >=(DataSave0.TransInfoData.ForeignTransTotal.uiTotalNb+DataSave0.TransInfoData.TransTotal.uiTotalNb))
			//	break;
		}
	}
#else
	for(uiIndex = 0; uiIndex < TRANS_MAXNB; uiIndex++)
	{
		if( DataSave0.TransInfoData.auiTransIndex[uiIndex])
		{
			GetOldTransData(uiIndex);
			OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),
							(unsigned char *)&NormalTransData,
							sizeof(NORMALTRANS));
			if((DataSave0.TransInfoData.auiTransIndex[uiIndex]&TRANS_NIIVOID) == TRANS_NIIVOID
				||((G_NORMALTRANS_ucTransType == TRANS_PURCHASE)
   		 	  	&&(G_NORMALTRANS_ucTIPAdjustFlag)))   /*void trans */
				SERV_TransList(1);
			else
				SERV_TransList(0);
			uitransnb ++;
			switch(Os__xget_key())
			{
				case KEY_CLEAR:
					return(SUCCESS);
				case KEY_ENTER:
					break;
				default:
					break;
			}
			if(uitransnb >=(DataSave0.TransInfoData.ForeignTransTotal.uiTotalNb+
		  			    DataSave0.TransInfoData.TransTotal.uiTotalNb))
				break;
		}
	}
#endif	
	return(SUCCESS);
}


unsigned char EMV_PackgeHeadEveProcess(unsigned char ucPackgeHeadEveID)
{
	unsigned char ucResult;

	util_Printf("[飞客]ucPackgeHeadEveID ---= [%02x]\n",ucPackgeHeadEveID);
	switch(ucPackgeHeadEveID)
	{
		case 1:
			G_NORMALTRANS_ucTransType = TRANS_LOADPARAM;
			ucResult = SEL_TestOnlineTrans();
			break;
		case 2:
			G_NORMALTRANS_ucTransType = TRANS_SENDSTATS;
			ucResult = SEL_TestOnlineTrans();
			break;
		case 3:
			G_NORMALTRANS_ucTransType = TRANS_LOGONON;
			ucResult = LOGON_CashierProcess(0);
			break;
		case 4:
			ucResult=EMV_LoadCAPK();
			break;
		case 5:
			ucResult=EMV_LoadAIDParam();
			break;
		default:
			break;
	}
	memset(ISO8583Data.aucCommBuf , 0 , ISO8583_MAXCOMMBUFLEN);

	return ucResult;

}

unsigned char TRANS_SetField55(TAGPHASE enTagPhase,unsigned char ucLogType , unsigned char * SendBuf ,unsigned short * SendBufLen)
{

	UCHAR ucResult,ComPackSend[1024];
	USHORT ComPackSendLen,uiLen,uiBuffsize;


	memset(ComPackSend,0,sizeof(ComPackSend));
	ComPackSendLen=0;

	uiBuffsize = sizeof(ComPackSend);

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(enTagPhase,(UCHAR*)"\x9F\x26",ComPackSend+ComPackSendLen,&uiLen);  //ARQC--应用密码
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(enTagPhase,(UCHAR*)"\x9F\x27",ComPackSend+ComPackSendLen,&uiLen);  //CID--密码信息数据
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x37",ComPackSend+ComPackSendLen,&uiLen);  //Unpredictable Number
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(enTagPhase,(UCHAR*)"\x9F\x36",ComPackSend+ComPackSendLen,&uiLen);//ATC
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x95",ComPackSend+ComPackSendLen,&uiLen);  //Terminal Verification Results
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x10",ComPackSend+ComPackSendLen,&uiLen);  //Issuer Application Data
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9A",ComPackSend+ComPackSendLen,&uiLen);  //Terminal Transaction Date
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9C",ComPackSend+ComPackSendLen,&uiLen); 	//Transaction Type
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x02",ComPackSend+ComPackSendLen,&uiLen);  //Amount, Authorised
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x5F\x2A",ComPackSend+ComPackSendLen,&uiLen); 	//Transaction Currency Code
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x82",ComPackSend+ComPackSendLen,&uiLen);//AIP
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x1A",ComPackSend+ComPackSendLen,&uiLen);  //Terminal Country Code
	ComPackSendLen += uiLen;

//	if(TransReqInfo.enTransType ==CASHBACK)
	{
		uiLen = uiBuffsize - ComPackSendLen;
		EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x03",ComPackSend+ComPackSendLen,&uiLen);  //Amount, Other
		ComPackSendLen += uiLen;
	}

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x33",ComPackSend+ComPackSendLen,&uiLen);  //Terminal Capabilities
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x34",ComPackSend+ComPackSendLen,&uiLen);  //CVM RESULT
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x35",ComPackSend+ComPackSendLen,&uiLen);  //Terminal Type
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x1E",ComPackSend+ComPackSendLen,&uiLen);  //IFD
	ComPackSendLen += uiLen;

#ifdef CCB

	//Terminal Application Version Number
	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x09",ComPackSend+ComPackSendLen,&uiLen);  //
	ComPackSendLen += uiLen;
	//Transaction Sequence Counter
	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x41",ComPackSend+ComPackSendLen,&uiLen);  //
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x84",ComPackSend+ComPackSendLen,&uiLen);  //
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x53",ComPackSend+ComPackSendLen,&uiLen);  //
	ComPackSendLen += uiLen;
#else

	//Transaction Sequence Counter
	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x41",ComPackSend+ComPackSendLen,&uiLen);  //
	ComPackSendLen += uiLen;
#endif
	//小额支付应用且能获取时，必须上送该tag
	if (G_NORMALTRANS_ucTransType == TRANS_OFFPURCHASE)
	{
            uiLen = uiBuffsize - ComPackSendLen;
            EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x74",ComPackSend+ComPackSendLen,&uiLen);  //
            ComPackSendLen += uiLen;
            util_Printf("\nuiLen 9F74=%d",uiLen);
        }
	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\xDF\x31",ComPackSend+ComPackSendLen,&uiLen);  //IFD
	ComPackSendLen += uiLen;
	util_Printf("\nuiLen ==%d",uiLen);

	util_Printf("ucLogType= %02x\n",ucLogType);
	if(ucLogType==Msg_EMV_BatchAdvice||G_NORMALTRANS_ucTransType==TRANS_OFFPURCHASE)
	{
		util_Printf("-----in--------\n");
		uiLen = uiBuffsize - ComPackSendLen;
		EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x8A",ComPackSend+ComPackSendLen,&uiLen);  //
		ComPackSendLen += uiLen;
		util_Printf("0x8A = ");
	}
	util_Printf("--55field Len = %d\n",ComPackSendLen);
	memcpy(SendBuf , ComPackSend , ComPackSendLen);

	*SendBufLen = ComPackSendLen;

	return ucResult;
}
#if SANDREADER
unsigned char QTRANS_SetField55(TAGPHASE enTagPhase,unsigned char ucLogType , unsigned char * SendBuf ,unsigned short * SendBufLen)
{

	UCHAR ucResult,ComPackSend[1024];
	USHORT ComPackSendLen,uiLen,uiBuffsize;


	memset(ComPackSend,0,sizeof(ComPackSend));
	ComPackSendLen=0;

	uiBuffsize = sizeof(ComPackSend);

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x26",ComPackSend+ComPackSendLen,&uiLen);  //ARQC--应用密码
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x27",ComPackSend+ComPackSendLen,&uiLen);  //CID--密码信息数据
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x37",ComPackSend+ComPackSendLen,&uiLen);  //Unpredictable Number
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x36",ComPackSend+ComPackSendLen,&uiLen);//ATC
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x95",ComPackSend+ComPackSendLen,&uiLen);  //Terminal Verification Results
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x10",ComPackSend+ComPackSendLen,&uiLen);  //Issuer Application Data
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9A",ComPackSend+ComPackSendLen,&uiLen);  //Terminal Transaction Date
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9C",ComPackSend+ComPackSendLen,&uiLen); 	//Transaction Type
	ComPackSendLen += uiLen;
	if(G_NORMALTRANS_ucTransType==TRANS_QUERYBAL)
	{
		uiLen = uiBuffsize - ComPackSendLen;
		memcpy(ComPackSend+ComPackSendLen,(uchar *)"\x9F\x02\x06\x00\x00\x00\x00\x00\x00",9);
		//QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x02",ComPackSend+ComPackSendLen,&uiLen);  //Amount, Authorised
		ComPackSendLen += 9;
	}
	else
	{
		uiLen = uiBuffsize - ComPackSendLen;
		QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x02",ComPackSend+ComPackSendLen,&uiLen);  //Amount, Authorised
		ComPackSendLen += uiLen;
	}
	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x5F\x2A",ComPackSend+ComPackSendLen,&uiLen); 	//Transaction Currency Code
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x82",ComPackSend+ComPackSendLen,&uiLen);//AIP
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x1A",ComPackSend+ComPackSendLen,&uiLen);  //Terminal Country Code
	ComPackSendLen += uiLen;


	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x03",ComPackSend+ComPackSendLen,&uiLen);  //Amount, Other
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x33",ComPackSend+ComPackSendLen,&uiLen);  //Terminal Capabilities
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x34",ComPackSend+ComPackSendLen,&uiLen);  //CVM RESULT
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x35",ComPackSend+ComPackSendLen,&uiLen);  //Terminal Type
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x1E",ComPackSend+ComPackSendLen,&uiLen);  //IFD
	ComPackSendLen += uiLen;

	//Transaction Sequence Counter
	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x41",ComPackSend+ComPackSendLen,&uiLen);  //
	ComPackSendLen += uiLen;
	//小额支付应用且能获取时，必须上送该tag
	if (G_NORMALTRANS_ucTransType == TRANS_OFFPURCHASE)
	{
		uiLen = uiBuffsize - ComPackSendLen;
		QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x74",ComPackSend+ComPackSendLen,&uiLen);  //
		ComPackSendLen += uiLen;
		util_Printf("\nuiLen 9F74=%d",uiLen);
	}

	util_Printf("ucLogType= %02x\n",ucLogType);
	if(ucLogType==Msg_EMV_BatchAdvice||G_NORMALTRANS_ucTransType==TRANS_OFFPURCHASE)
	{
		util_Printf("-----in--------\n");
		uiLen = uiBuffsize - ComPackSendLen;
		QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x8A",ComPackSend+ComPackSendLen,&uiLen);  //
		ComPackSendLen += uiLen;
		util_Printf("0x8A = ");
	}
	util_Printf("--55field Len = %d\n",ComPackSendLen);
	memcpy(SendBuf , ComPackSend , ComPackSendLen);

	*SendBufLen = ComPackSendLen;

	return ucResult;
}
unsigned char QTRANS_SetField55_Reversal(TAGPHASE enTagPhase,unsigned char ucLogType , unsigned char * SendBuf ,unsigned short * SendBufLen)
{

	UCHAR ucResult,ComPackSend[1024];
	USHORT ComPackSendLen,uiLen,uiBuffsize;


	memset(ComPackSend,0,sizeof(ComPackSend));
	ComPackSendLen=0;

	uiBuffsize = sizeof(ComPackSend);

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x36",ComPackSend+ComPackSendLen,&uiLen);//ATC
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x95",ComPackSend+ComPackSendLen,&uiLen);  //Terminal Verification Results
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x10",ComPackSend+ComPackSendLen,&uiLen);  //Issuer Application Data
	ComPackSendLen += uiLen;


	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x1E",ComPackSend+ComPackSendLen,&uiLen);  //IFD
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	QDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\xDF\x31",ComPackSend+ComPackSendLen,&uiLen);  //IFD
	ComPackSendLen += uiLen;
	util_Printf("--55field Len = %d\n",ComPackSendLen);
	memcpy(SendBuf , ComPackSend , ComPackSendLen);

	*SendBufLen = ComPackSendLen;

	return ucResult;
}
#endif
unsigned char TRANS_SetField55_Reversal(TAGPHASE enTagPhase,unsigned char ucLogType , unsigned char * SendBuf ,unsigned short * SendBufLen)
{

	UCHAR ucResult,ComPackSend[1024];
	USHORT ComPackSendLen,uiLen,uiBuffsize;


	memset(ComPackSend,0,sizeof(ComPackSend));
	ComPackSendLen=0;

	uiBuffsize = sizeof(ComPackSend);

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(enTagPhase,(UCHAR*)"\x9F\x36",ComPackSend+ComPackSendLen,&uiLen);//ATC
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x95",ComPackSend+ComPackSendLen,&uiLen);  //Terminal Verification Results
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x10",ComPackSend+ComPackSendLen,&uiLen);  //Issuer Application Data
	ComPackSendLen += uiLen;


	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x1E",ComPackSend+ComPackSendLen,&uiLen);  //IFD
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\xDF\x31",ComPackSend+ComPackSendLen,&uiLen);  //IFD
	ComPackSendLen += uiLen;
	util_Printf("\nuiLen ==%d",uiLen);
	util_Printf("--55field Len = %d\n",ComPackSendLen);
	memcpy(SendBuf , ComPackSend , ComPackSendLen);

	*SendBufLen = ComPackSendLen;

	return ucResult;
}

unsigned char TRANS_SetUntouchField55(unsigned char * SendBuf ,unsigned short * SendBufLen)
{
	UCHAR ucResult,ComPackSend[1024],aucTemp[16];
	USHORT ComPackSendLen,uiLen,uiBuffsize;

	memset(ComPackSend,0,sizeof(ComPackSend));
	ComPackSendLen=0;

	uiBuffsize = sizeof(ComPackSend);
	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x5D",ComPackSend+ComPackSendLen,&uiLen);  //ARQC应用密文
	util_Printf("\nucResult=======9F5D===%d",ucResult);
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F5D ComPackSendLen = %d\n",uiLen);
	}


	uiBuffsize = sizeof(ComPackSend);
	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x26",ComPackSend+ComPackSendLen,&uiLen);  //ARQC应用密文
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F26 ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x27",ComPackSend+ComPackSendLen,&uiLen);  //CID密文信息数据
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F27 ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x10",ComPackSend+ComPackSendLen,&uiLen); //Issuer Application Data
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F10 ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x37",ComPackSend+ComPackSendLen,&uiLen); //Unpredictable Number
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F37 ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x36",ComPackSend+ComPackSendLen,&uiLen);  //ATC
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn TagF36 ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x95",ComPackSend+ComPackSendLen,&uiLen);  	  //Terminal Verification Results
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag95 ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9A",ComPackSend+ComPackSendLen,&uiLen);     //Terminal Transaction Date
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9A ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9C",ComPackSend+ComPackSendLen,&uiLen);     //Transaction Type
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9c ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x02",ComPackSend+ComPackSendLen,&uiLen); //Amount, Authorised
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F02 ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x5F\x2A",ComPackSend+ComPackSendLen,&uiLen); //Transaction Currency Code
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag5F2A ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x82",ComPackSend+ComPackSendLen,&uiLen);     //AIP
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag82 ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x1A",ComPackSend+ComPackSendLen,&uiLen); //Terminal Country Code
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F1A ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x03",ComPackSend+ComPackSendLen,&uiLen); //Amount, Other
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F03 ComPackSendLen = %d\n",uiLen);
	}

	memset(aucTemp,0,sizeof(aucTemp));
	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x33",aucTemp,&uiLen); //Terminal Capabilities
	if(ucResult==SUCCESS)
	{
		util_Printf("\n====9F33===:%02x,%02x,%02x",aucTemp[0],aucTemp[1],aucTemp[2]);
//		ucTemp =aucTemp[3];
//		aucTemp[3]=aucTemp[5];
//		aucTemp[5] =ucTemp;
		memcpy(ComPackSend+ComPackSendLen,aucTemp,uiLen);
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F33 ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x34",ComPackSend+ComPackSendLen,&uiLen); //CVM RESULT
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F34 ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x35",ComPackSend+ComPackSendLen,&uiLen); //Terminal Type
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F35 ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x1E",ComPackSend+ComPackSendLen,&uiLen); //IFD
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F1E ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x41",ComPackSend+ComPackSendLen,&uiLen); //
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F41 ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x6C",ComPackSend+ComPackSendLen,&uiLen); //
	util_Printf("\nTag9F6C=%d",ucResult);
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F6C ComPackSendLen = %d\n",uiLen);
	}
	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x74",ComPackSend+ComPackSendLen,&uiLen); //
	util_Printf("\nTag9F74=%d",ucResult);
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F74 ComPackSendLen = %d\n",uiLen);
	}
	if(G_NORMALTRANS_ucTransType==TRANS_OFFPURCHASE)
	{
		uiLen = uiBuffsize - ComPackSendLen;
		ucResult=INTERFACE_GetTagValue((PUCHAR)"\x8A",ComPackSend+ComPackSendLen,&uiLen); //
		if(ucResult==SUCCESS)
		{
			ComPackSendLen += uiLen;
			util_Printf("WangAn Tag8A ComPackSendLen = %d\n",uiLen);
		}
	}
	memcpy(SendBuf , ComPackSend , ComPackSendLen);
	*SendBufLen = ComPackSendLen;

 	util_Printf("ComPackSendLen = %d\n",ComPackSendLen);
 	UTIL_TestDispStr(ComPackSend,(uint)ComPackSendLen ,(uchar *)"ComPackSend[] ;TRANS_SetUntouchField55 = ",false);
	return ucResult;
}
unsigned char TRANS_SetUntouchField55_Reversal(unsigned char * SendBuf ,unsigned short * SendBufLen)
{
	UCHAR ucResult,ComPackSend[1024];
	USHORT ComPackSendLen,uiLen,uiBuffsize;

	memset(ComPackSend,0,sizeof(ComPackSend));
	ComPackSendLen=0;

	uiBuffsize = sizeof(ComPackSend);


	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x10",ComPackSend+ComPackSendLen,&uiLen); //Issuer Application Data
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag9F10 ComPackSendLen = %d\n",uiLen);
	}


	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x9F\x36",ComPackSend+ComPackSendLen,&uiLen);  //ATC
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn TagF36 ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\x95",ComPackSend+ComPackSendLen,&uiLen);  	  //Terminal Verification Results
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag95 ComPackSendLen = %d\n",uiLen);
	}

	uiLen = uiBuffsize - ComPackSendLen;
	ucResult=INTERFACE_GetTagValue((PUCHAR)"\xDF\x31",ComPackSend+ComPackSendLen,&uiLen);  	  //Terminal Verification Results
	if(ucResult==SUCCESS)
	{
		ComPackSendLen += uiLen;
		util_Printf("WangAn Tag95 ComPackSendLen = %d\n",uiLen);
	}

	memcpy(SendBuf , ComPackSend , ComPackSendLen);
	*SendBufLen = ComPackSendLen;

 	util_Printf("ComPackSendLen = %d\n",ComPackSendLen);
 	UTIL_TestDispStr(ComPackSend,(uint)ComPackSendLen ,(uchar *)"ComPackSend[] ;TRANS_SetUntouchField55 = ",false);
	return ucResult;
}
unsigned char Trans_SendICOfflineTrans(void)
{
	unsigned char ucResult,ucKey;
	unsigned char aucDispIndex[17],aucTemp[3];
	unsigned short  uiTransIndex,uiNII,uiTotalNb,uiIndex;
	unsigned char ucLogonFlag;

	util_Printf("\n**************************************************\n");
	util_Printf("\n				Send IC Offline Trans");
	util_Printf("\n**************************************************\n");
	ucResult=SUCCESS;

//	//Os__clr_display(255);
//	//Os__GB2312_display(0,0,(unsigned char*)"上送IC卡离线交易");

    ucLogonFlag =0;
	uiIndex=0;
	uiTotalNb=DataSave0.TransInfoData.ForeignTransTotal.uiTotalNb+DataSave0.TransInfoData.TransTotal.uiTotalNb;
	for(uiTransIndex=0;uiTransIndex<=uiTotalNb;uiTransIndex++)
    	{
    		GetOldTransData(uiTransIndex);
    		OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),(unsigned char *)&NormalTransData,sizeof(NORMALTRANS));
		if((DataSave1.SaveTrans.ucTransType == TRANS_OFFPURCHASE)
		  &&(((DataSave0.TransInfoData.auiTransIndex[uiTransIndex])&TRANS_NIISENDFINISH) != TRANS_NIISENDFINISH)
		  &&((DataSave0.TransInfoData.auiTransIndex[uiTransIndex])!= 0 )
		  )
		{
	      	//Os__clr_display(255);
			//Os__GB2312_display(0,0,(unsigned char*)"正在上送离线交易");
			//Os__GB2312_display(2,0,(unsigned char*)"     请稍候");
			uiIndex++;
			if(G_NORMALTRANS_euCardSpecies==CARDSPECIES_MAG) continue;
			if(!ucLogonFlag)
			{
				G_NORMALTRANS_ucTransType = TRANS_LOGONON;
				G_NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
				G_RUNDATA_ucDialFlag = 1;
				ucResult = LOGON_CashierProcess(1);
				if(!ucResult)
				{
					ucLogonFlag =1;
					//G_RUNDATA_ucHostConnectFlag = true;
					util_Printf("\nuiTransIndex ==1==%d",uiTransIndex);
					GetOldTransData(uiTransIndex);
					OSMEM_Memcpy((unsigned char *)&(DataSave1.SaveTrans),(unsigned char *)&NormalTransData,sizeof(NORMALTRANS));
				}
			}
			if (!ucResult)
			{
				//Os__clr_display(255);
				//Os__GB2312_display(0,0,(unsigned char*)"正在上送离线交易");
				//Os__GB2312_display(2,0,(unsigned char*)"     请稍候");
				//Os__GB2312_display(3,0,(unsigned char*)"按[取消]键退出");

				memset(aucDispIndex , 0, sizeof(aucDispIndex));
				memcpy(aucDispIndex , "    第[   ]笔", 13 );
				memset(aucTemp , '0',sizeof(aucTemp));
				int_asc(aucTemp, 2, (unsigned int *)(&uiIndex));
				memcpy(&aucDispIndex[7] , aucTemp ,2);
				//Os__GB2312_display(1,0,(unsigned char*)aucDispIndex);
				ucKey = UTIL_WaitGetKey(3);

				if (ucKey==KEY_CLEAR)
					return(ERR_CANCEL);
			    G_EXTRATRANS_ucInputPINLen=0;
			    ucResult = TRANS_Online();
			}
			if( ucResult == SUCCESS||ucResult==ERR_HOSTCODE)
			{
			    if (ucResult==ERR_HOSTCODE
			        && DataSave0.ConstantParamData.ucBatchErrTicket=='1')
			        {
                                PRINT_OffTrans_Ticket();
			        }
				uiNII = DataSave0.TransInfoData.auiTransIndex[uiTransIndex]|TRANS_NIISENDFINISH;
				Os__saved_copy((unsigned char *)&uiNII,
					(unsigned char *)&DataSave0.TransInfoData.auiTransIndex[uiTransIndex],
					sizeof(unsigned short));
				Os__saved_copy((unsigned char *)&NormalTransData,
								(unsigned char *)&(DataSave1.SaveTrans),
								sizeof(NORMALTRANS));
				XDATA_Write_Vaild_File(DataSaveTransInfo);
			}
			else
				break;
		}
	}
	util_Printf("\n	*********	Send IC Offline Trans   end    *********    ");
	return ucResult;

}

unsigned char Trans_SpecInfoSend(unsigned char ucSettleFlag,unsigned int *pucSpecTransNum)
{
	unsigned char ucResult,aucBuf[100];
	unsigned short uiNII;
	unsigned short uiI;
	TRANSRECORD EMVICTransLog;
    unsigned char  ucSpecTransNum;

	util_Printf("\n**************************************************\n");
	util_Printf("\n        Send Offline Error or ARPC Error");
	util_Printf("\n**************************************************\n");
	ucSpecTransNum=0;
	ucResult = SUCCESS;
	ucResult =FILE_ReadRecordNumByFileName(ERRICCTRANS, &ucSpecTransNum);

	util_Printf("------11---ucSpecTransNum=%d ; ucResult = %02x\n",ucSpecTransNum,ucResult);

	for(uiI=0;uiI<ucSpecTransNum;uiI++)
	{
		util_Printf("------1---uiI=%d\n",uiI);
		ucResult = FILE_ReadRecordByFileName(ERRICCTRANS, uiI+1, &EMVICTransLog, sizeof(TRANSRECORD));
		util_Printf("------1---ucResult=%02x\n",ucResult);
		util_Printf("------1---pEMVICTransLog->ucTransFlag=%02x\n",EMVICTransLog.ucTransFlag);
		util_Printf("------1---pEMVICTransLog->ucTransType=%02x\n",EMVICTransLog.ucTransType);

		if(ucResult) return SUCCESS;
		if(EMVICTransLog.ucTransFlag != 0x02) continue;
		if(EMVICTransLog.ucTransType==Msg_EMV_BatchTrans) continue;

		ISO8583_Clear();
		ISO8583_SetMsgID(320);
		memset(aucBuf,0,sizeof(aucBuf));
		if(EMVICTransLog.ucPANLen<20)
		{
			if(EMVICTransLog.ucPANLen%2)
				bcd_asc(aucBuf,EMVICTransLog.aucPAN,EMVICTransLog.ucPANLen+1);
			else
				bcd_asc(aucBuf,EMVICTransLog.aucPAN,EMVICTransLog.ucPANLen);
			ISO8583_SetBitValue(2,aucBuf,EMVICTransLog.ucPANLen);
		}
		memset(aucBuf,0,sizeof(aucBuf));
		long_asc(aucBuf,6,&EMVICTransLog.ulTraceNum);
		ISO8583_SetBitValue(11,aucBuf,6);

		memset(aucBuf,0,sizeof(aucBuf));
		long_asc(aucBuf,12,&EMVICTransLog.ulAmount);
		ISO8583_SetBitValue(4,aucBuf,12);

		memset(aucBuf,0,sizeof(aucBuf));
		memcpy(aucBuf,"0500",4);
		ISO8583_SetBitValue(22,aucBuf,4);

 		if(EMVICTransLog.ucPANSeq)
 		{
			ISO8583_SetBitHexValue(23,&EMVICTransLog.ucPANSeq,1);
 		}

		ISO8583_SetBitValue(41,G_RUNDATA_aucTerminalID,8);
		ISO8583_SetBitValue(42,G_RUNDATA_aucMerchantID,15);
		ISO8583_SetBitValue(55,EMVICTransLog.aucLogBuff,EMVICTransLog.uiLogBuffLen);

		memset(aucBuf,0x00,sizeof(aucBuf));
		memcpy(aucBuf,"00",2);
		long_asc(&aucBuf[2],6,&DataSave0.ChangeParamData.ulBatchNumber);
		if(ucSettleFlag)
			memcpy(&aucBuf[8],"204",3);
		else
			memcpy(&aucBuf[8],"206",3);

		aucBuf[11]='5';
		aucBuf[12]='0';
	       ISO8583_SetBitValue(60,aucBuf,13);

		memset(aucBuf,0x00,sizeof(aucBuf));
		aucBuf[0]=7;
		aucBuf[1]=1;

		if(!memcmp(EMVICTransLog.aucCurrencyCode,TermConfig.aucTermCountryCode,TERMCOUNTRYCODELEN))
			memcpy(&aucBuf[2],"00",2);
		else
			memcpy(&aucBuf[2],"01",2);
		if(!memcmp(EMVICTransLog.aucAuthRespCode,"Z1",2))
			memcpy(&aucBuf[4],"11",2);
		else if(!memcmp(EMVICTransLog.aucAuthRespCode,"Z3",2))
			memcpy(&aucBuf[4],"13",2);
		else if(!memcmp(EMVICTransLog.aucAuthRespCode,"00",2))
			memcpy(&aucBuf[4],"05",2);

		long_asc(&aucBuf[6],12,&G_NORMALTRANS_ulAmount);
		memcpy(&aucBuf[18],"156",3);

		if(!memcmp(EMVICTransLog.aucAuthRespCode,"Z1",2)
		   ||!memcmp(EMVICTransLog.aucAuthRespCode,"Z3",2))
			memcpy(&aucBuf[21],"10",2);
		else
			memcpy(&aucBuf[21],"22",2);

		ISO8583_SetBitValue(62,aucBuf,23);

		ucResult = COMMS_GenSendReceive();
		if( ucResult == SUCCESS)
		{
			ucBatchSandTransNum++;
			uiNII = EMVICTransLog.uiSendFlag|TRANS_BATCHSEND;
			EMVICTransLog.uiSendFlag = uiNII;

			FILE_UpdateRecordByFileName(ERRICCTRANS ,uiI+1 ,&EMVICTransLog ,sizeof(TRANSRECORD));

		}

		if(ucResult) break;

	}

	*pucSpecTransNum=ucSpecTransNum;
	//ucBatchSandTransNum+=ucSpecTransNum;
	util_Printf("\n*******Send Offline Error or ARPC Error*****END*******\n");

	return(SUCCESS);
}

unsigned char TRANS_SetScriptProcessField55(unsigned char ucLogType , unsigned char * SendBuf ,unsigned short * SendBufLen)
{
	UCHAR ucResult,ComPackSend[1024];
	USHORT ComPackSendLen,uiLen,uiBuffsize;

	memset(ComPackSend,0,sizeof(ComPackSend));
	ComPackSendLen=0;

	uiBuffsize = sizeof(ComPackSend);

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x33",ComPackSend+ComPackSendLen,&uiLen);
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x95",ComPackSend+ComPackSendLen,&uiLen);
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x37",ComPackSend+ComPackSendLen,&uiLen);
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x1E",ComPackSend+ComPackSendLen,&uiLen);
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x10",ComPackSend+ComPackSendLen,&uiLen);
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x26",ComPackSend+ComPackSendLen,&uiLen);
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x36",ComPackSend+ComPackSendLen,&uiLen);
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x82",ComPackSend+ComPackSendLen,&uiLen);
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\xDF\x31",ComPackSend+ComPackSendLen,&uiLen);
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9F\x1A",ComPackSend+ComPackSendLen,&uiLen);
	ComPackSendLen += uiLen;

	uiLen = uiBuffsize - ComPackSendLen;
	EMVDATA_TagPackProcess(ALLPHASETAG,(UCHAR*)"\x9A",ComPackSend+ComPackSendLen,&uiLen);
	ComPackSendLen += uiLen;

	memcpy(SendBuf , ComPackSend , ComPackSendLen);

	*SendBufLen = ComPackSendLen;


	return ucResult;
}


unsigned char Trans_EMVProcessScript(void)
{

	unsigned char 	ucResult,ucI;
	unsigned char   aucBuf[512];
	unsigned short uiTagLen;


	memset( &ISO8583Data,0 , sizeof(ISO8583));

	G_NORMALTRANS_ulOldTraceNumber=G_NORMALTRANS_ulTraceNumber;
//	UTIL_IncreaseTraceNumber();
//	G_NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;

	ISO8583_Clear();

	ISO8583_SetMsgID(620);

	/* Set bit 2 */
//	if(G_NORMALTRANS_ucTransType==TRANS_EC_UNASSIGNLOAD)
//	{
//		ISO8583_SetBitValue(2,G_NORMALTRANS_aucUnAssignAccountISO2,G_NORMALTRANS_ucMainAccountLen);
//	}
//	else
	{
		if(G_NORMALTRANS_ucSourceAccLen)
		{
			if(G_NORMALTRANS_ucSourceAccLen%2)
				bcd_asc(aucBuf,G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen+1);
			else
				bcd_asc(aucBuf,G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen);
			ISO8583_SetBitValue(2,aucBuf,G_NORMALTRANS_ucSourceAccLen);
		}
	}


	/* Set bit 3 */
	memset(aucBuf,0,sizeof(aucBuf));
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_QUERYBAL:
			aucBuf[0] = 0x31;
			break;
		case TRANS_PURCHASE:
		case TRANS_TIPADJUST:
		case TRANS_TIPADJUSTOK:
		case TRANS_OFFPURCHASE:
		case TRANS_PREAUTHFINISH:
		case TRANS_PREAUTHSETTLE:
		case TRANS_OFFPREAUTHFINISH:
			aucBuf[0] = 0x00;
			break;
		case TRANS_PREAUTH:
		case TRANS_PREAUTHADD:
			aucBuf[0] = 0x03;
			break;
		case TRANS_REFUND:
		case TRANS_VOIDPREAUTH:
		case TRANS_VOIDPURCHASE:
		case TRANS_VOIDPREAUTHFINISH:
			aucBuf[0] = 0x20;
        		break;
		case TRANS_CUPMOBILE:
			aucBuf[0] = 0x00;
			break;
		case TRANS_VOIDCUPMOBILE:
			aucBuf[0] = 0x20;
			break;
		case TRANS_EC_ASSIGNLOAD:
			aucBuf[0] =0x60;
			break;
		case TRANS_EC_UNASSIGNLOAD:
			aucBuf[0] =0x62;
			break;
		case TRANS_EC_CASHLOAD:
			aucBuf[0] =0x63;
			break;
		default:
			break;
	}
	aucBuf[1] = aucBuf[1] | (((G_NORMALTRANS_ucSourceAccType - 0x30) << 4 ) & 0xF0);
	hex_asc(G_RUNDATA_RevChack_ucProcessCode, aucBuf, 6);
	ISO8583_SetBitHexValue(3,aucBuf,3);
#if 0
	memset(aucBuf,0,sizeof(aucBuf));
	switch(G_NORMALTRANS_ucTransType)
	{
	case TRANS_QUERYBAL:
		aucBuf[0] = 0x31;
		break;
	case TRANS_PURCHASE:
		aucBuf[0] = 0x00;
		break;
	case TRANS_PREAUTH:
		aucBuf[0] = 0x03;
		break;
	default:
		break;
	}
	aucBuf[1] = aucBuf[1] | (((G_NORMALTRANS_ucSourceAccType - 0x30) << 4 ) & 0xF0);
       ISO8583_SetBitHexValue(3,aucBuf,3);
	/* Set bit 12 */
	memset(aucBuf,0,sizeof(aucBuf));
	bcd_asc(aucBuf,G_NORMALTRANS_aucTime,6);
	ISO8583_SetBitValue(12,aucBuf,6);

	/* Set data bit 13 */
	memset(aucBuf,0,sizeof(aucBuf));
	bcd_asc(aucBuf,G_NORMALTRANS_aucDate+2,4);
	ISO8583_SetBitValue(13,aucBuf,4);
#endif
	/* Set Amount bit 4 */
	memset(aucBuf,0,sizeof(aucBuf));
	switch(G_NORMALTRANS_ucTransType)
	{
	case TRANS_QUERYBAL:
		break;
	case TRANS_PURCHASE:
	case TRANS_PREAUTH:
	case TRANS_EC_ASSIGNLOAD:
	case TRANS_EC_UNASSIGNLOAD:
	case TRANS_EC_CASHLOAD:
		long_asc(aucBuf,12,&G_NORMALTRANS_ulAmount);
	    ISO8583_SetBitValue(4,aucBuf,12);
		break;
	default:
		break;
	}

	memset(aucBuf,0,sizeof(aucBuf));
	long_asc(aucBuf,6,&G_NORMALTRANS_ulTraceNumber);
	ISO8583_SetBitValue(11,aucBuf,6);

	/* Set conditional data */
	memset(aucBuf,0,sizeof(aucBuf));
//	if(G_NORMALTRANS_ucTransType==TRANS_EC_UNASSIGNLOAD)
//	{
//		memcpy(aucBuf,"02",2);
//		if(G_EXTRATRANS_ucInputPINLen_UnAssign)
//	    		aucBuf[2] = '1';
//	    	else
//	  aucBuf[2] = '2';
//		ISO8583_SetBitValue(22,aucBuf,4);
//	}
//	else
	{
		memcpy(aucBuf,"05",2);
		if(G_EXTRATRANS_ucInputPINLen)
	    		aucBuf[2] = '1';
	    	else
	    		aucBuf[2] = '2';
		ISO8583_SetBitValue(22,aucBuf,4);
	}
	if(G_NORMALTRANS_ucPANSeqExist)
		ISO8583_SetBitHexValue(23,&G_NORMALTRANS_ucPANSeq,sizeof(G_NORMALTRANS_ucPANSeq));

	ISO8583_SetBitValue(32,G_NORMALTRANS_aucBankID,strlen((char*)G_NORMALTRANS_aucBankID));

	 /* set Retrieval Reference Number bit 37*/
	 util_Printf("\nG_NORMALTRANS_aucRefNum=0202=%s",G_NORMALTRANS_aucRefNum);
    ISO8583_SetBitValue(37,G_NORMALTRANS_aucRefNum,12);

	ISO8583_SetBitValue(41,G_RUNDATA_aucTerminalID,8);

	/* set Authorization code bit 38*/
	if(strlen((char*)G_NORMALTRANS_aucAuthCode))
		ISO8583_SetBitValue(38,G_NORMALTRANS_aucAuthCode,TRANS_AUTHCODELEN);

	/* Set Card Acceptor Identification Code bit 42 */
	ISO8583_SetBitValue(42,G_RUNDATA_aucMerchantID,15);
	/* Set bit 49 */
	ISO8583_SetBitValue(49,(uchar *)"156",3);

	/* Set bit 55 */
	memset(aucBuf,0,sizeof(aucBuf));
	uiTagLen=0;
	TRANS_SetScriptProcessField55(Msg_EMV_BatchTrans,aucBuf ,&uiTagLen);
	ucResult=ISO8583_SetBitValue(55,aucBuf,uiTagLen);

	/* Set bit 60 */
	memset(aucBuf,0,sizeof(aucBuf));
	switch(G_NORMALTRANS_ucTransType)
	{
		case TRANS_QUERYBAL:
			memcpy(aucBuf,"01",2);
			break;
		case TRANS_PREAUTH:
		case TRANS_PREAUTHADD:
			memcpy(aucBuf,"10",2);
			break;
		case TRANS_VOIDPREAUTH:
			memcpy(aucBuf,"11",2);
			break;
		case TRANS_PREAUTHFINISH:
			memcpy(aucBuf,"20",2);
			break;
		case TRANS_VOIDPREAUTHFINISH:
			memcpy(aucBuf,"21",2);
			break;
		case TRANS_PURCHASE:
		case TRANS_CUPMOBILE:
			memcpy(aucBuf,"22",2);
			break;
		case TRANS_VOIDPURCHASE:
		case TRANS_VOIDCUPMOBILE:
			memcpy(aucBuf,"23",2);
			break;
		case TRANS_REFUND:
			memcpy(aucBuf,"25",2);
			memcpy(&aucBuf[8],"000",3);
			break;
		case TRANS_OFFPREAUTHFINISH:
			memcpy(aucBuf,"30",2);
			break;
		case TRANS_TIPADJUST:
			memcpy(aucBuf,"32",2);
			break;
		case TRANS_TIPADJUSTOK:
			memcpy(aucBuf,"34",2);
			break;
		case TRANS_OFFPURCHASE:
			memcpy(aucBuf,"36",2);
			break;
		case TRANS_PREAUTHSETTLE:
			memcpy(aucBuf,"24",2);
			break;
		case TRANS_EC_ASSIGNLOAD:
			memcpy(aucBuf,"00",2);              //45
			break;
		case TRANS_EC_UNASSIGNLOAD:
			memcpy(aucBuf,"00",2);              //47
			break;
		case TRANS_EC_CASHLOAD:
			memcpy(aucBuf,"00",2);              //46
			break;
		default:
			break;
	}
#if 0
	switch(G_NORMALTRANS_ucTransType)
	{
    	case TRANS_QUERYBAL:
	case TRANS_PREAUTH:
	case TRANS_PURCHASE:
		memcpy(aucBuf,"00",2);
    		break;
	default:
		break;
	}
#endif
	long_asc(&aucBuf[2],6,&DataSave0.ChangeParamData.ulBatchNumber);
	strcpy((char*)&aucBuf[8],"951");
	aucBuf[11]='5';
	aucBuf[12]=G_NORMALTRANS_ucFallBackFlag+'0';
    	ISO8583_SetBitValue(60,aucBuf,strlen((char*)aucBuf));

	 /* Set bit 61 */
	memset(aucBuf,0,sizeof(aucBuf));
	ucI=0;
	long_asc(&aucBuf[ucI],6,&DataSave0.ChangeParamData.ulBatchNumber);
	ucI+=6;
	long_asc(&aucBuf[ucI],6,&G_NORMALTRANS_ulOldTraceNumber);
	ucI+=6;
	bcd_asc(&aucBuf[ucI],&G_NORMALTRANS_aucDate[2],4);
	ISO8583_SetBitValue(61,aucBuf,strlen((char*)aucBuf));

	ISO8583_SetBitValue(64,(uchar *)"0000000000000000",16);

	ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
					(unsigned char *)&DataSave0.Trans_8583Data.ScriptInform8583Data);
	Os__saved_set((unsigned char *)(&DataSave0.ChangeParamData.ucScriptInformValid),
					DATASAVEVALIDFLAG,
					sizeof(unsigned char));

	XDATA_Write_Vaild_File(DataSaveTrans8583);
	XDATA_Write_Vaild_File(DataSaveChange);

	return ucResult;
}

unsigned char Trans_SendStats(void)
{
	unsigned char 	ucResult;

	G_NORMALTRANS_ucTransType = TRANS_SENDSTATS;
	DataSave0.ChangeParamData.ucPackgeHeadEve = 1;
	ucResult = SEL_TestOnlineTrans();

	return ucResult;
}

