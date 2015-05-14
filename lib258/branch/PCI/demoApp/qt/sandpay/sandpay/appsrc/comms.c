/* 

--------------------------------------------------------------------------
    FILE  comms.c                              
--------------------------------------------------------------------------
    INTRODUCTION          
    ============                                             
    Created :       2004-11-30      
    Last modified : 2004-11-30      
    Module :  Shanghai Smart Card Network
    Purpose :
        Source file of communicate function.

    List of routines in file :
                                                                          
    File history :

*/

#include <include.h>
#include <global.h>
#include <xdata.h>
#include <osgraph.h>
#include <new_drv.h>

extern const SINGLEERRMSG NetErrMsg[];

unsigned char COMMS_PreComm(void)
{   
    unsigned char ucResult;
    unsigned char ucI;
    unsigned char ucSBoardLen,aucBuf[64];

    COM_PARAM CommParam =
    {
        1,     /* Stop bit */
        1200,  /* Speed */
        2,     /* MODEM 0 RS232 1 HDLC 2 */
        8,     /* Data bit */
       'N'     /* NO parity */
    };

/*    unsigned char aucModemParam[10]=
    {
        0,                  //0:frequency   1:plus
        0,                  //check dial ring  0:yes,1:no
        8,                  //ժȴʱ    u:100ms,min:4
        6,                  //ߵȴʱ      u:100ms   0~255
        115,                //һ뱣ʱ  u:1ms   70~255
        8,                  //ʱ  u:10ms  7~25
        1,                  //d7-->0:sync   1:async     d6d5-->00:1200b     01:2400b
                            d4d3:-->00:bell     11:ccitt    0x:ccitt
                            d2d1d0:-->000:5s        001:8s      010:11s
                                      011:14s       100:17s     101:20s
                                      110:23s       111:26s 
        3,                  //ѭ  0-->1
        0,                  
        0
    }; 
*/     
    RUNDATA_ucHostConnectFlag = false;

    switch( DataSave0.ConstantParamData.ucCommMode )
    {
    case PARAM_COMMMODE_HDLC:
        for(ucI=0;ucI<10;ucI++)
        {
//            Os__set_modem_parameter(ucI,aucModemParam[ucI]);
              Os__set_modem_parameter(ucI,*(&DataSave0.ConstantParamData.ModemParam.dp+ucI));
        }   
        break;
    default:
        break;
    }
    CommParam.com_nr = DataSave0.ConstantParamData.ucCommMode;
    CommParam.speed = DataSave0.ConstantParamData.uiCommSpeed;
    
	switch( DataSave0.ConstantParamData.ucCommMode )
	{
		case PARAM_COMMMODE_GPRS://xiaobin,2011-05-11,modify
        //	OSGSM_GprsSetAPN((unsigned char*)DataSave0.ConstantParamData.ucAPN);
			break;
		default:
			if(Os__init_com(&CommParam) != OK)
			{
				return(ERR_OSFUNC);
			}
			break;
	}
/************Ƚֻ绰*************************/ 
    memset(aucBuf,0,sizeof(aucBuf));
    memcpy(aucBuf,DataSave0.ConstantParamData.aucSwitchBoard,PARAM_SWITCHLEN);
    ucSBoardLen=strlen((char *)aucBuf);
    memcpy(&DialParam.phone_nr[0],aucBuf,ucSBoardLen);
    
/**************************************************************/    
    switch( DataSave0.ConstantParamData.ucCommMode )
    {
	case PARAM_COMMMODE_MODEM:
	case PARAM_COMMMODE_HDLC:
	    DialParam.prefix[0] = 0;
	    DialParam.auto_man = MANUAL_CALL;
	    if( RUNDATA_ucDialRetry%2 )
	    {
	    	if(NORMALTRANS_ucTransType==TRANS_S_INIONLINE)
	            memcpy(&DialParam.phone_nr[ucSBoardLen],DataSave0.ConstantParamData.aucInitTelNumber2,
	                21);
	         else
	         	memcpy(&DialParam.phone_nr[ucSBoardLen],DataSave0.ConstantParamData.aucHostTelNumber2,
	                21);
	    }else
	    {
	    	if(NORMALTRANS_ucTransType==TRANS_S_INIONLINE)
	            memcpy(&DialParam.phone_nr[ucSBoardLen],DataSave0.ConstantParamData.aucInitTelNumber1,
	                21);
	        else
	        	 memcpy(&DialParam.phone_nr[ucSBoardLen],DataSave0.ConstantParamData.aucHostTelNumber1,
	                21);
	    }
#ifdef TEST
		Uart_Printf("DialParam.phone_nr1=%s\n",DialParam.phone_nr);
#endif                  
		RUNDATA_ucDialRetry ++;
	    if( ( ucResult = Os__backgnd_dial(&DialParam)) != STILL_DIALING )
	    {
#ifdef TEST
			Uart_Printf("Os__backgnd_dial ucResult=%02x\n",ucResult);
#endif          
	        return(ERR_OSFUNC);
	    }
	    break;
	case PARAM_COMMMODE_GPRS:		//xiaobin,2011-05-11,modify
		ucResult = OSGSM_GprsDial();
		Uart_Printf ("\n22222222233 ");
		Uart_Printf ("\nOSGSM_GprsDial %02x",ucResult);
		break;
	case PARAM_COMMMODE_CDMA:		//gaolong,2011-07-19,add
		ucResult = OSGSM_GprsDial();
		Uart_Printf("\nOSGSM_GprsDial %02x",ucResult);
		break;
	default:
		break;
    }
    return(SUCCESS);
}
unsigned char COMMS_PreComm1(void)
{	
	unsigned char ucResult;
	unsigned char ucI;
	unsigned char ucSBoardLen,aucBuf[17];

	COM_PARAM CommParam =
	{
		1,     /* Stop bit */
		9600,  /* Speed */
		1,	   /* MODEM 0 RS232 1 HDLC 2 */
		8,     /* Data bit */
	       'N'     /* NO parity */
	};

	if(Os__init_com(&CommParam) != OK)
	{
		return(ERR_OSFUNC);
	}

	return(SUCCESS);
}

unsigned char COMMS_PreComm2(void)
{	
	unsigned char ucResult = SUCCESS;

	if(Os__init_com3(0x0303, 0x0c00, 0x0c) != OK)
	{
		return(ERR_OSFUNC);
	}
	
	return(SUCCESS);
}


unsigned char COMMS_CheckPreComm(void)
{
    DRV_OUT *pdKey;
    unsigned char ucResult,ucDialDispFlag;
    unsigned int uiTimeout;
    unsigned char ucI;
    NEW_DRV_TYPE  new_drv;
    int ret ;
    ucI = 1;
    MSG_DisplayMsg(true,0,0,MSG_DIALING);
    //pdKey = Os__get_key();
    for(;;)
    {
        uiTimeout = DataSave0.ConstantParamData.uiDialWaitTime*100;
        ucDialDispFlag = true;
        do{
            switch(DataSave0.ConstantParamData.ucCommMode )
            {
            case PARAM_COMMMODE_MODEM:
            case PARAM_COMMMODE_HDLC:
                ucResult = Os__check_backgnd_dial(CFG_CHECKDIALTIMEOUT);
                break;
                Uart_Printf("uiDialWaitTime=%d\n",uiTimeout);
            case PARAM_COMMMODE_GPRS:
                ucResult = OSGSM_GprsCheckDial(10*CFG_CHECKDIALTIMEOUT);
                Uart_Printf("\nOSGSM_GprsCheckDial %02x",ucResult);
                break;
            default:
                return ERR_COMMS_PROTOCOL;
            }
            if((((ucResult == DIAL_OK)
                 ||(ucResult == DIAL_CONNECT))
                &&((DataSave0.ConstantParamData.ucCommMode==PARAM_COMMMODE_MODEM)
                   ||DataSave0.ConstantParamData.ucCommMode==PARAM_COMMMODE_HDLC))
                    ||((ucResult==SUCCESS)&&(DataSave0.ConstantParamData.ucCommMode==PARAM_COMMMODE_GPRS)))
            {
                break;
            }else if(ucDialDispFlag == true)
            {
                Os__abort_drv(drv_mmi);
                Os__xdelay(100);
                MSG_DisplayErrMsgNotWait(ucResult);
                //pdKey = Os__get_key();
                ret = Os_Wait_Event(KEY_DRV, &new_drv, uiTimeout*10);
                ucDialDispFlag = false;
            }
        }while(new_drv.drv_type != DRV_TIMEOUT);

        Uart_Printf("IN DIA***************************ret ================%d\n",ret);

        if(ret==0)
        {
            if(new_drv.drv_data.gen_status == DRV_ENDED)
            {
                if(new_drv.drv_data.xxdata[1]== KEY_CLEAR)
                {
                    return(ERR_CANCEL);
                }else
                {
                    //pdKey = Os__get_key();
                    uiTimeout = DataSave0.ConstantParamData.uiDialWaitTime*100;
                    ret = Os_Wait_Event(KEY_DRV, &new_drv, uiTimeout*10);
                    if(ret==0)
                    {
                        if(new_drv.drv_data.gen_status == DRV_RUNNING)
                            continue;
                    }
                }
            }
        }
        if((((ucResult == DIAL_OK)
             ||(ucResult == DIAL_CONNECT))
            &&((DataSave0.ConstantParamData.ucCommMode==PARAM_COMMMODE_MODEM)
               ||DataSave0.ConstantParamData.ucCommMode==PARAM_COMMMODE_HDLC))
                ||((ucResult==SUCCESS)&&(DataSave0.ConstantParamData.ucCommMode==PARAM_COMMMODE_GPRS)))
        {
            RUNDATA_ucHostConnectFlag = true;
#ifdef TEST
            Uart_Printf("DIAL_CONNECT\n");
#endif
            Os__abort_drv(drv_mmi);
            return(SUCCESS);
        }
        /* Retry Dial */
        COMMS_FinComm();
        if( ucI < DataSave0.ConstantParamData.ucDialRetry)
        {
            do{
                ucResult = COMMS_PreComm();
                ucI ++;
                if( ucResult == SUCCESS )
                {
                    break;
                }else
                {
                    COMMS_FinComm();
                }
            }while( ucI < DataSave0.ConstantParamData.ucDialRetry );
        }else
        {
            Os__abort_drv(drv_mmi);
            return(ucResult);
        }
        if( ucResult == SUCCESS )
        {
            continue;
        }else
        {
            Os__abort_drv(drv_mmi);
            return(ERR_DIAL);
        }
        }
}

unsigned char COMMS_TransOnline(void)
{
    /* restore iso8583data from backup */
    
    ISO8583_RestoreISO8583Data((unsigned char *)&ISO8583Data,
                    (unsigned char *)&DataSave0.SaveISO8583Data);

    return(COMMS_TransSendReceive());
}


unsigned char COMMS_TransSendReceive(void)
{
    unsigned char aucMAC[8];
    unsigned short uiLen;
    unsigned char ucResult;
    Uart_Printf("COMMS_TransSendReceive_____COMMS_TransSendReceiveCOMMS_TransSendReceiveCOMMS_TransSendReceive");
    /* Save package to SendISO8583Data */

    ISO8583_SaveISO8583Data((unsigned char *)&ISO8583Data,
                        (unsigned char *)&DataSave0.SendISO8583Data);
    XDATA_Write_Vaild_File(DataSaveSendISO8583);
                        
    ucResult = ISO8583_PackData(ISO8583Data.aucCommBuf,&ISO8583Data.uiCommBufLen,
                    ISO8583_MAXCOMMBUFLEN);
    //ISO8583_DumpData();
  
    /* Set MAC */
    if(  (ISO8583_CheckBit(64) == SUCCESS)
       ||(ISO8583_CheckBit(128) == SUCCESS)
      )
    {
        uiLen = ISO8583Data.uiCommBufLen-8;

        ucResult = UTIL_CalcMemberMac(ISO8583Data.aucCommBuf,uiLen,
                                    aucMAC);
        /* Copy MAC to commbuf */
        memcpy( &ISO8583Data.aucCommBuf[uiLen],aucMAC,8);
    }
    //ISO8583_DumpData();
    /* SendReceive with host */
    uiLen = sizeof(ISO8583Data.aucCommBuf);
    ISO8583_DumpData();
    //UTIL_PrintHexMsg(ISO8583Data.aucCommBuf,ISO8583Data.uiCommBufLen);
    if(Commun_flag==0x34)
		RunData.ucComType=DataSave0.ConstantParamData.ucCom;	
    if(DataSave0.ConstantParamData.ucYTJFlag!=2)
    {
	    if(RunData.ucComType)
	    {
			ucResult = rs232_SendReceive(ISO8583Data.aucCommBuf,ISO8583Data.uiCommBufLen,ISO8583Data.aucCommBuf,&uiLen);
	    }
	    else
	    {
		    if(Commun_flag==0x31)
		    {
			    	Uart_Printf("\n111111\n");
			    	ucResult = COMMS_SendReceive(ISO8583Data.aucCommBuf,ISO8583Data.uiCommBufLen,
			                    ISO8583Data.aucCommBuf,&uiLen);
		   	}else
		   	{
		   		Uart_Printf("\n222222\n");
				if(DataSave0.ConstantParamData.ucConnectType=='2'
					&&(NormalTransData.ucTransType==TRANS_S_INIONLINE
						||NormalTransData.ucTransType==TRANS_S_LOGONON
						||NormalTransData.ucTransType==TRANS_S_BATCHUP
						||NormalTransData.ucTransType==TRANS_S_SETTLE_ONE
						||NormalTransData.ucTransType==TRANS_S_SETTLE_TWO))
				{
						Uart_Printf("\n22222222222222222222\n");
			    		ucResult = COMMS_SendReceiveNet_Long(ISO8583Data.aucCommBuf,ISO8583Data.uiCommBufLen,
			                    ISO8583Data.aucCommBuf,&uiLen,IPAddress,HostPort);
				}
				else
				{
					Uart_Printf("\nELSEELSEELSEELSEELSE\n");
			   		ucResult = COMMS_SendReceiveNet(ISO8583Data.aucCommBuf,ISO8583Data.uiCommBufLen,
			                    ISO8583Data.aucCommBuf,&uiLen,IPAddress,HostPort);

				}
		       	
                if( ucResult != SUCCESS)
				{
					MSG_DisplaySINGLEERRMSG(NetErrMsg,ucResult);
					ucResult=ERR_CANCEL;
					return ucResult;
				}
		   	}
	   
	        }
		}
		else
		{
		     ucResult = Port_SendReceive(ISO8583Data.aucCommBuf,ISO8583Data.uiCommBufLen,
	                    ISO8583Data.aucCommBuf,&uiLen);
		}
#ifdef TEST
        Uart_Printf("COMMS_SendReceive ucResult=%02x",ucResult);
#endif

    if( ucResult != SUCCESS)
        return(ucResult);
    ISO8583Data.uiCommBufLen = uiLen;
    ISO8583_UnpackData(ISO8583Data.aucCommBuf,ISO8583Data.uiCommBufLen);
//  ISO8583_DumpData();
    /* Check response data valid */
    ucResult = ISO8583_CheckResponseValid();
    if(RUNDATA_ucErrorExtCode == 15)//Ѿ
    {
    	ucResult=SUCCESS;
    }
    if( ucResult != SUCCESS)
    {
        /* If have response return, cancel reversal */      
        return(ucResult);
    }
    if(  (ISO8583_CheckBit(64) == SUCCESS)
       ||(ISO8583_CheckBit(128) == SUCCESS)
      )
    {
    	if(Commun_flag==0x31)
       		uiLen = ISO8583Data.uiCommBufLen-8;
       	else
       		uiLen = ISO8583Data.uiCommBufLen-10;

        ucResult = UTIL_CalcMemberMac(ISO8583Data.aucCommBuf,uiLen,
                                    aucMAC);
#ifdef TEST
{
	unsigned short i;
	Uart_Printf("Data to calculate MAC are \n");
	for(i=0;i<uiLen;i++)
	{
		Uart_Printf("%02x ",ISO8583Data.aucCommBuf[i]);
		if(!((i+1)%20))
			Uart_Printf("\n");
	}
	Uart_Printf("\n");
	Uart_Printf("calculated MAC are \n");
	for(i=0;i<8;i++)
	{
		Uart_Printf("%02x ",aucMAC[i]);

	}
	Uart_Printf("\n");
}
#endif         
         
//        if( memcmp( &ISO8583Data.aucCommBuf[uiLen],aucMAC,4) )
//        {
//            return(ERR_ISO8583_MACERROR);
//        }
    }/*
    else
    {
        if(S_NORMALTRANS_ucTransType != TRANS_REVERSAL)
                return(ERR_COMMS_NOMAC);
    } 
*/  
    return(SUCCESS);
}


unsigned char COMMS_SendReceive(unsigned char *pucInData,unsigned short uiInLen,
                    unsigned char *pucOutData,unsigned short *puiOutLen)
{
    unsigned char ucResult;
    unsigned char aucBuf[4];
    unsigned char aucComVersion[9];
    unsigned char ucLRC;
    unsigned short uiI;
    unsigned short uiLen;
    unsigned short uiTimeout;
    unsigned char ucByte;
    unsigned char *pucData;
    unsigned char ucFirstChar;
    //unsigned char aucTPDU[]={ 0x60,0x00,0x03,0x80,0x00,0xBB,0xBB,0x32};
    unsigned char aucTPDU[]={ 0x60,0x00,0x03,0x80,0x00,0xAA,0xBB,0x31,0x31 };
	
    if( RUNDATA_ucHostConnectFlag == false )
    {
        if(  (ucResult = COMMS_CheckPreComm()) != SUCCESS)
        {               
            return(ucResult);
        }   
    }
	
    MSG_DisplayMsg( true,0,0,MSG_SENDING);
    for(uiTimeout = 0;uiTimeout<20000;uiTimeout++);
    /* Clear communication buffer first */
Uart_Printf(" send 1 \n");
    Os__com_flush2();
    /* Send Data */
Uart_Printf(" send 2 \n");
	
    //uiLen = uiInLen+8;  /* include TPDU */
    uiLen = uiInLen+9;  /* include TPDU */

    memset(aucComVersion,0,sizeof(aucComVersion));	
    memcpy(aucComVersion,RunData.aucComVersion,5);
    uiLen+=9;

    switch( DataSave0.ConstantParamData.ucCommMode )
    {
    case PARAM_COMMMODE_MODEM:
    case PARAM_COMMMODE_RS232:
    
        aucBuf[0] = CHAR_STX;
		
#ifdef TEST
Uart_Printf("uiLen=%d\n",uiLen);
#endif

        short_bcd(&aucBuf[1],2,&uiLen);
        aucBuf[3] = CHAR_ETX;

        for(ucLRC=0,uiI=1;uiI<3;uiI++)
        {
            ucLRC ^= aucBuf[uiI];
        }
        for(uiI=0;uiI<3;uiI++)
        {
            if(  Os__txcar(aucBuf[uiI]) != OK )
            {
                return(ERR_COMMS_SENDCHAR);
            }
        }
        break;
    default:
        break;
    }
	
    /* Send TPDU */
    for(uiI=0;uiI<9;uiI++)
    {
        if(  Os__txcar(aucTPDU[uiI]) != OK )
        {
            return(ERR_COMMS_SENDCHAR);
        }
        ucLRC ^= aucTPDU[uiI];
    }
    /* Send ComVersion*/
    for(uiI=0;uiI<9;uiI++)
    {
        if(  Os__txcar(aucComVersion[uiI]) != OK )
        {
            return(ERR_COMMS_SENDCHAR);
        }
        ucLRC ^= aucComVersion[uiI];
    }
    
    Uart_Printf(" send 3 \n");
#ifdef TEST
	Uart_Printf(" Send Data \n");	
#endif
    for(uiI=0;uiI<uiInLen;uiI++)
    {
        ucByte = *(pucInData+uiI);
        ucLRC ^= ucByte;
#ifdef TEST
		Uart_Printf("%02x ",ucByte);
		if(!((uiI+1)%20))
			Uart_Printf("\n");
#endif
        if ( Os__txcar(ucByte) != OK )
        {
            return(ERR_COMMS_SENDCHAR);
        }
    }
Uart_Printf("\n");
Uart_Printf(" send 4 \n");
    switch( DataSave0.ConstantParamData.ucCommMode )
    {
    case PARAM_COMMMODE_MODEM:
    case PARAM_COMMMODE_RS232:
        if( Os__txcar(aucBuf[3]) != OK)
        {
            return(ERR_COMMS_SENDCHAR);
        }
        if( Os__txcar(ucLRC) != OK)
        {
            return(ERR_COMMS_SENDCHAR);
        }
    default:
        break;
    }

Uart_Printf(" send 5 \n");
    //Os__clr_display(255);
Uart_Printf(" send 6 \n");

    //Os__GB2312_display(0,0,(unsigned char *)"ڽ...");
    /* Receive Data */
    uiTimeout = DataSave0.ConstantParamData.uiReceiveTimeout*100;
    pucData = pucOutData;
    switch( DataSave0.ConstantParamData.ucCommMode )
    {
    case PARAM_COMMMODE_MODEM:
    case PARAM_COMMMODE_RS232:
        ucLRC = 0x00;
        ucResult = COMMS_ReceiveByte(&ucByte,&uiTimeout);
        if( ucResult != OK)
        {
            return(ucResult);
        }

        if( ucByte != CHAR_STX )
        {
            return(ERR_COMMS_PROTOCOL);
        }
        memset(aucBuf,0,sizeof(aucBuf));
        for(uiI=0;uiI<2;uiI++)
        {
            ucResult = COMMS_ReceiveByte(&ucByte,&uiTimeout);
            if( ucResult != OK)
            {
                return(ucResult);
            }
            aucBuf[uiI] = ucByte;
            ucLRC ^= ucByte;
        }
        uiLen = (unsigned short)bcd_long(aucBuf,4);
        if( uiLen > *puiOutLen )
        {
            return(ERR_COMMS_RECVBUFFOVERLIMIT);
        }
        uiTimeout = COMMS_RECVTIMEOUT;
        for( uiI=0;uiI<uiLen;uiI++)
        {
            ucResult = COMMS_ReceiveByte(&ucByte,&uiTimeout);

            if( ucResult != OK)
            {
                return(ucResult);
            }
            ucLRC ^= ucByte;
            if( uiI >=18 )       /* skip TPDU */
            {
                *pucData = ucByte;
                pucData ++;
            }else
            {
                if( uiI == 0)   /* check valid tpdu header */
                {
                    if( ucByte != 0x60)
                    {
                        return(ERR_COMMS_ERRTPDU);
                    }
                }
            }
        }

        ucResult = COMMS_ReceiveByte(&ucByte,&uiTimeout);
        if( ucResult != OK)
        {
            return(ucResult);
        }
        if( ucByte != CHAR_ETX )
        {
            return(ERR_COMMS_PROTOCOL);
        }
        ucLRC ^= ucByte;
        ucResult = COMMS_ReceiveByte(&ucByte,&uiTimeout);
        if( ucResult != OK)
        {
            return(ucResult);
        }
        if( ucByte != ucLRC )
        {
            return(ERR_COMMS_LRC);
        }
        break;
    default:
        ucFirstChar = true;
        uiI = 0;
#ifdef TEST
        Uart_Printf("Received Data\n");
#endif
        do{
            ucResult = COMMS_ReceiveByte(&ucByte,&uiTimeout);
#ifdef TEST
			Uart_Printf("%02x ",ucByte);
			if(!((uiI+1)%20))
				Uart_Printf("\n");
#endif
            switch(ucResult)
            {
            case OK:
                if( (pucData-pucOutData) > *puiOutLen )
                {
                    return(ERR_COMMS_RECVBUFFOVERLIMIT);
                }
                if( ucFirstChar == true )
                {
                    if( ucByte != 0x60) /* check valid tpdu header */
                    {
                        return(ERR_COMMS_ERRTPDU);
                    }
                    ucFirstChar = false;
                }
                if( uiI >= 18)   /* skip tpdu */
                {
                    *pucData = ucByte;
                    pucData ++;
                }
                uiI ++;
                uiTimeout = COMMS_RECVTIMEOUT;
                continue;
            case ERR_COMMS_RECVTIMEOUT:
                if( ucFirstChar == false )
                {
                    *puiOutLen = uiI-5;
                    break;
                }else
                {
                    return(ucResult);
                }
            default:
                return(ucResult);
            }
            break;
        }while(1);
        Uart_Printf("\n");
        break;
    }
	
    return(SUCCESS);
}


unsigned char COMMS_ReceiveByte(unsigned char *pucByte,unsigned short *puiTimeout)
{
    unsigned short uiResult;

    uiResult = Os__rxcar(*puiTimeout);

    switch  ( uiResult / 256 )
    {
    case OK :
        *pucByte = uiResult % 256;
        return(OK);
    case COM_ERR_TO:
        return(ERR_COMMS_RECVTIMEOUT);
    default :
        return(ERR_COMMS_RECVCHAR);
    }
}

unsigned char COMMS_FinComm(void)
{
    switch( DataSave0.ConstantParamData.ucCommMode )
    {
    case PARAM_COMMMODE_MODEM:
    case PARAM_COMMMODE_HDLC:
Uart_Printf("hang up step 1\n");
        Os__hang_up();
Uart_Printf("hang up step 2\n");
        break;
   case PARAM_COMMMODE_GPRS:
   case PARAM_COMMMODE_CDMA:
        Uart_Printf("\n Before OSGSM_GprsHangupDial");
        //OSGSM_GprsHangupDial();
	    Uart_Printf("\n End OSGSM_GprsHangupDial");
        break;
    default:
        break;
    }
    RUNDATA_ucHostConnectFlag = false;
    return(SUCCESS);

}

unsigned char COMMS_SendReceiveNet(unsigned char *pucInData,unsigned short uiInLen,
					unsigned char *pucOutData,unsigned short *puiOutLen,
					unsigned long ulHostIPAddress,unsigned int uiHostPort)
{
    unsigned char ucResult,ucIPAddress[5],ucCurrentIP,ucPreIP;
	unsigned short uiTimeout;
	int iTCPHandle,uiI;
	unsigned char ucBuf[20],aucComVersion[9];
	
	unsigned char 	aucTPDU[]={ 0x60,0x00,0x03,0x80,0x00,0xAA,0xBB,0x31,0x31};
	unsigned int 	uiLen=0,uiPort;
	unsigned char 	aucCommuBuf[512];
	unsigned short 	OutLen=0;
	unsigned char   i;

    Uart_Printf("\n\n\n.0.COMMS_SendReceiveNet ..........\n\n\n");

    if( RUNDATA_ucHostConnectFlag == false &&  (Commun_flag==0x33||Commun_flag==0x35))
	{
        ucResult = COMMS_CheckPreComm();
        Uart_Printf("***************COMMS_CheckPreComm===ucResult========== %02x*************\n");
        if(  ucResult!= SUCCESS)
        {
			return(ucResult);
		}	
	}

    memset(aucComVersion,0,sizeof(aucComVersion));	
    memcpy(aucComVersion,RunData.aucComVersion,5);

    ucResult = COMMSENCRYPT_SetVersion("1020");
    if( ucResult != SUCCESS )
    {
    	Uart_Printf("COMMSENCRYPT_SetVersion ucResult = %02x\n",ucResult);
    	COMMSENCRYPT_ErrMsg(ucResult);
    	return ERR_CANCEL;
    }
    ucResult=TCP_Open(&iTCPHandle);
    Uart_Printf("\n End TCP_Open ucResult:[%02x]",ucResult);
    if(ucResult!=SUCCESS) return ucResult;

#ifdef GUI_PROJECT
    memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
    ProUiFace.ProToUi.uiLines=1;
    memcpy(ProUiFace.ProToUi.aucLine1,"连接中...",18);
#else
    //Os__clr_display(255);
    //Os__GB2312_display(0,0,(uchar *)"连接中...");
#endif

	ucPreIP=DataSave0.ChangeParamData.ucCurrentIPFlag;
    //由于外网的不稳定性，最大连接三次
	for(i=0;i<3;i++)
	{
		if(NormalTransData.ucTransType==TRANS_S_INIONLINE)
		{
			memset(ucIPAddress,0x00,sizeof(ucIPAddress));
			memcpy(ucIPAddress,&ulHostIPAddress,sizeof(unsigned long));
			Uart_Printf("\n ucIPAddress = %03d.%03d.%03d.%03d \n ",
							ucIPAddress[0],ucIPAddress[1],ucIPAddress[2],ucIPAddress[3]);
			Uart_Printf("\n uiHostPort = %04d",uiHostPort);
			ucResult = TCP_Connect(iTCPHandle,tab_long(ucIPAddress,4),uiHostPort);
		}
		else
		{
			if(i%2==0)
			{
				memset(ucIPAddress,0x00,sizeof(ucIPAddress));
				if(RunData.uc1stIpTye==1||DataSave0.ChangeParamData.ucCurrentIPFlag==0)
				{
					memcpy(ucIPAddress,&DataSave0.ConstantParamData.ulHostIPAdd1,sizeof(unsigned long));
					uiPort=DataSave0.ConstantParamData.uiHostPort1;
					ucCurrentIP=0;
				}
				else
				{
					memcpy(ucIPAddress,&DataSave0.ConstantParamData.ulHostIPAdd2,sizeof(unsigned long));
					uiPort=DataSave0.ConstantParamData.uiHostPort2;
					ucCurrentIP=1;
				}
				Uart_Printf("\n ucIPAddress = %03d.%03d.%03d.%03d \n ",
								ucIPAddress[0],ucIPAddress[1],ucIPAddress[2],ucIPAddress[3]);
				Uart_Printf("\n uiHostPort = %04d",uiPort);
				ucResult = TCP_Connect(iTCPHandle,tab_long(ucIPAddress,4),uiPort);

			}
			else
			{
				memset(ucIPAddress,0x00,sizeof(ucIPAddress));
				memcpy(ucIPAddress,&DataSave0.ConstantParamData.ulHostIPAdd2,sizeof(unsigned long));
				if(RunData.uc1stIpTye!=1&&DataSave0.ChangeParamData.ucCurrentIPFlag==1)
				{
					memcpy(ucIPAddress,&DataSave0.ConstantParamData.ulHostIPAdd1,sizeof(unsigned long));
					uiPort=DataSave0.ConstantParamData.uiHostPort1;
					ucCurrentIP=0;
				}
				else
				{
					memcpy(ucIPAddress,&DataSave0.ConstantParamData.ulHostIPAdd2,sizeof(unsigned long));
					uiPort=DataSave0.ConstantParamData.uiHostPort2;
					ucCurrentIP=1;
				}
				Uart_Printf("\n ucIPAddress = %03d.%03d.%03d.%03d \n ",
								ucIPAddress[0],ucIPAddress[1],ucIPAddress[2],ucIPAddress[3]);
				Uart_Printf("\n uiHostPort = %04d",uiPort);
				ucResult = TCP_Connect(iTCPHandle,tab_long(ucIPAddress,4),uiPort);
			}
		}
		if(!ucResult)
			break;
	}
	
	Uart_Printf("\n Tcp_Connect Out ucResult: [%02x]  \n",ucResult);
    if( !ucResult )
    {	
		if(ucPreIP!=ucCurrentIP)
			DataSave0.ChangeParamData.ucSwitchIPFlag=1;
		DataSave0.ChangeParamData.ucCurrentIPFlag=ucCurrentIP;
		XDATA_Write_Vaild_File(DataSaveChange);

#ifdef GUI_PROJECT
        memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
        ProUiFace.ProToUi.uiLines=1;
        memcpy(ProUiFace.ProToUi.aucLine1,"发送中...",18);
#else
        MSG_DisplayMsg( true,0,0,MSG_SENDING);
#endif
		
        //后台要求送tpdu跟整个包长(两个字节的bcd码)
		memset(aucCommuBuf,0,sizeof(aucCommuBuf));
		memset(ucBuf,0,sizeof(ucBuf));
		uiLen=uiInLen+9+9;
		Uart_Printf("uiLen=%d\n",uiLen);
		int_bcd(&aucCommuBuf[0],2,&uiLen);
		memcpy(&aucCommuBuf[2],aucTPDU,9);//8
		memcpy(&aucCommuBuf[11],aucComVersion,9);	
		memcpy(&aucCommuBuf[20],pucInData,uiInLen);//10

#ifdef TEST
    	Uart_Printf("\nConnect Server OK");

		Uart_Printf("\n*******Send  Packet Data*******\n");
		for(uiI=0;uiI<uiLen;uiI++)
		{
			if((uiI+1)%20==0) Uart_Printf("\n");

			Uart_Printf("%02x ",aucCommuBuf[uiI]);
		}
#endif

		Uart_Printf("\nBegin Send Data");
		ucResult=DataEncrypt(aucCommuBuf,(uiInLen+20),DataSave0.ConstantParamData.aucTerminalID,
										aucCommuBuf,&OutLen);
        /*
        memset(aucEncryptData,0,sizeof(aucEncryptData));
		memcpy(aucEncryptData,pucInData,uiInLen);
		ucResult=DataEncrypt(aucEncryptData,uiInLen,DataSave0.ConstantParamData.aucTerminalID,
										aucEncryptData,&uiOutLen);
		if(ucResult!=SUCCESS)
            return ucResult;
        */
		if(ucResult!=SUCCESS)
		{
			COMMSENCRYPT_ErrMsg(ucResult);
			ucResult = ERR_CANCEL;
		}

#ifdef TEST
        Uart_Printf("\nOutLen = %dData after Encrypt.\n",OutLen);
		for(uiI=0;uiI<OutLen;uiI++)
		{
			if((uiI+1)%20==0) Uart_Printf("\n");

			Uart_Printf("%02x ",aucCommuBuf[uiI]);
		}
#endif

		ucResult=TCP_Send(iTCPHandle,aucCommuBuf,OutLen);
		//ucResult=TCP_Send(iTCPHandle,aucCommuBuf,(uiInLen+10));
        Uart_Printf("\nTCP_Send ucResult=%02x\n",ucResult);

#if 1
        memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
        ProUiFace.ProToUi.uiLines=1;
        memcpy(ProUiFace.ProToUi.aucLine1,"接收中...",18);
        Uart_Printf("ProUiFace.ProToUi.aucLine1 ===================== %s\n",ProUiFace.ProToUi.aucLine1);
#else
		MSG_DisplayMsg( true,0,0,MSG_RECEIVING);
#endif
		memset(ucBuf,0x00,sizeof(ucBuf));
		memset(aucCommuBuf,0,sizeof(aucCommuBuf));
		memcpy(ucBuf,"ʹ    ",14);
		if( !ucResult )
		{
            Uart_Printf("\nBegin Receive Data");
            //for(uiI=1;uiI<=GUI_COMM_TIMEOUT;uiI++)
            for(uiI=1;uiI<=30;uiI++)
            {
                ucResult = TCP_Recv(iTCPHandle,aucCommuBuf,&OutLen, 60);

                Uart_Printf("\n[--> %02x]  ",ucResult);
                //int_asc(ucBuf+10,2,&uiI);
                //Os__GB2312_display(1,0,ucBuf);
                if(!ucResult) break;
                //if(OutLen) break;
            }
            Uart_Printf("\n");
#ifdef TEST
            Uart_Printf("\n*********Receive Packet Data***********\n");
            Uart_Printf("\nReceive Data Len befor Decrypt= %d,ucResult=%02x\n",OutLen,ucResult);
            for(uiI=0;uiI<OutLen;uiI++)
            {
                if((uiI+1)%20==0) Uart_Printf("\n");
                Uart_Printf("%02x ",aucCommuBuf[uiI]);
            }
#endif

            ucResult = DataDecrypt(aucCommuBuf,aucCommuBuf,&OutLen);
            if(OutLen>17)
                *puiOutLen=OutLen-17;
            if( !ucResult )
            {
                if(*puiOutLen>NETWORK_MAXDATALEN)
                    ucResult=ERR_NETCOMMS_OVERBUFLIMIT;
            }
#ifdef TEST
            Uart_Printf("OutLen=%d\n",OutLen);
            Uart_Printf("\nReceive Data End ucResult:[%02x]",ucResult);
#endif
            if( !ucResult )
            {
                memcpy(pucOutData,&aucCommuBuf[21],*puiOutLen);
#ifdef TEST
                Uart_Printf("\nReceive Data Len = %d\n",*puiOutLen);


                for(uiI=0;uiI<*puiOutLen;uiI++)
                {
                    if((uiI+1)%20==0) Uart_Printf("\n");
                    Uart_Printf("%02x ",pucOutData[uiI]);
                }
                Uart_Printf("\n");
#endif
            }else
            {
                COMMSENCRYPT_ErrMsg(ucResult);
                ucResult = ERR_CANCEL;
            }
		}
	}
    else OSGSM_GprsReset();

    //Os__clr_display(255);
    //Os__GB2312_display(0,0,(unsigned char *)"连接关闭中");
#ifdef GUI_PROJECT
	memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
	ProUiFace.ProToUi.uiLines=1;	
	if(!ucResult)
	{
        memcpy(ProUiFace.ProToUi.aucLine1,"连接成功",16);
	}else
        memcpy(ProUiFace.ProToUi.aucLine1,"连接失败",16);
#else
    //Os__clr_display(255);
    //Os__GB2312_display(0,0,(unsigned char *)"连接成功");
#endif
	TCP_Close(iTCPHandle);	
	Os__xdelay(GUI_DELAY_TIMEOUT);

    Uart_Printf("\n End TCP_Close\n\n");

	return(ucResult);
}

unsigned char COMMS_SendReceiveNet_Long(unsigned char *pucInData,unsigned short uiInLen,
					unsigned char *pucOutData,unsigned short *puiOutLen,
					unsigned long ulHostIPAddress,unsigned int uiHostPort)
{


     unsigned char ucResult,ucIPAddress[5];
	unsigned short uiTimeout;
	int iTCPHandle,uiI;
	unsigned char ucBuf[20],aucComVersion[9];
	
	unsigned char 	aucTPDU[]={ 0x60,0x00,0x03,0x80,0x00,0xAA,0xBB,0x31,0x31};
	unsigned int 	uiLen=0;
	unsigned char 	aucCommuBuf[512];
	unsigned short 	OutLen=0;
	unsigned char   i;

Uart_Printf("\n\n\n.1.COMMS_SendReceiveNet_Long uiInLen :%d.............\n\n\n",uiInLen);
#ifdef GUI_PROJECT			
memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
ProUiFace.ProToUi.uiLines=1;
memcpy(ProUiFace.ProToUi.aucLine1,"...",11);
sleep(1);
#endif
	if( RUNDATA_ucHostConnectFlag == false &&  (Commun_flag==0x33||Commun_flag==0x35))
	{
		if(  (ucResult = COMMS_CheckPreComm()) != SUCCESS)
		{				
			return(ucResult);
		}	
	}
	memset(aucComVersion,0,sizeof(aucComVersion));	
	memcpy(aucComVersion,RunData.aucComVersion,5);
	aucComVersion[5]='L';
	Uart_Printf("\n uiInLen :%02x",uiInLen);
	ucResult = COMMSENCRYPT_SetVersion("1020");
	if( ucResult != SUCCESS )
	{
		Uart_Printf("COMMSENCRYPT_SetVersion ucResult = %02x\n",ucResult);
		COMMSENCRYPT_ErrMsg(ucResult);
		return ERR_CANCEL;
	}
	Uart_Printf("\n RunData.ucTCPConnectFlag :%02x",RunData.ucTCPConnectFlag);
	if(RunData.ucTCPConnectFlag== false)
	{
		ucResult=TCP_Open(&iTCPHandle);
		RunData.iTCPHandle=iTCPHandle;
		Uart_Printf("\n End TCP_Open ucResult:[%02x]",ucResult);
		if(ucResult!=SUCCESS) return ucResult;
        //Os__clr_display(255);
		//Os__GB2312_display(0,0,(uchar *)"...");
		//Ĳȶԣ	
		for(i=0;i<3;i++)
		{
			if(NormalTransData.ucTransType==TRANS_S_INIONLINE)
			{
				memset(ucIPAddress,0x00,sizeof(ucIPAddress));
				memcpy(ucIPAddress,&ulHostIPAddress,sizeof(unsigned long));
				Uart_Printf("\n ucIPAddress = %03d.%03d.%03d.%03d \n ",
								ucIPAddress[0],ucIPAddress[1],ucIPAddress[2],ucIPAddress[3]);
				Uart_Printf("\n uiHostPort = %04d",uiHostPort);
				ucResult = TCP_Connect(iTCPHandle,tab_long(ucIPAddress,4),uiHostPort);
			}
			else
			{
				if(i%2==0)
				{
					memset(ucIPAddress,0x00,sizeof(ucIPAddress));
					memcpy(ucIPAddress,&DataSave0.ConstantParamData.ulHostIPAdd1,sizeof(unsigned long));
					Uart_Printf("\n ucIPAddress = %03d.%03d.%03d.%03d \n ",
									ucIPAddress[0],ucIPAddress[1],ucIPAddress[2],ucIPAddress[3]);
					Uart_Printf("\n uiHostPort = %04d",DataSave0.ConstantParamData.uiHostPort1);
					ucResult = TCP_Connect(iTCPHandle,tab_long(ucIPAddress,4),DataSave0.ConstantParamData.uiHostPort1);

				}
				else
				{
					memset(ucIPAddress,0x00,sizeof(ucIPAddress));
					memcpy(ucIPAddress,&DataSave0.ConstantParamData.ulHostIPAdd2,sizeof(unsigned long));
					Uart_Printf("\n ucIPAddress = %03d.%03d.%03d.%03d \n ",
									ucIPAddress[0],ucIPAddress[1],ucIPAddress[2],ucIPAddress[3]);
					Uart_Printf("\n uiHostPort = %04d",DataSave0.ConstantParamData.uiHostPort2);
					ucResult = TCP_Connect(iTCPHandle,tab_long(ucIPAddress,4),DataSave0.ConstantParamData.uiHostPort2);
				}
			}
			if(!ucResult)
				break;
		}
		Uart_Printf("\n Tcp_Connect Out ucResult: [%02x]  \n",ucResult);
	}
	if( !ucResult )
	{
		RunData.ucTCPConnectFlag=true;
		iTCPHandle=RunData.iTCPHandle;
		MSG_DisplayMsg( true,0,0,MSG_SENDING);


		//̨Ҫtpdu(ֽڵbcd)
		memset(aucCommuBuf,0,sizeof(aucCommuBuf));
		memset(ucBuf,0,sizeof(ucBuf));
		uiLen=uiInLen+9+9;
		Uart_Printf("uiLen=%d\n",uiLen);
		int_bcd(&aucCommuBuf[0],2,&uiLen);
		memcpy(&aucCommuBuf[2],aucTPDU,9);//8
		memcpy(&aucCommuBuf[11],aucComVersion,9);

		memcpy(&aucCommuBuf[20],pucInData,uiInLen);//10

#ifdef TEST
		Uart_Printf("\nConnect Server OK");

		Uart_Printf("\n*******Send  Packet Data*******\n");
		for(uiI=0;uiI<uiLen;uiI++)
		{
			if((uiI+1)%20==0) Uart_Printf("\n");

			Uart_Printf("%02x ",aucCommuBuf[uiI]);
		}
#endif

		Uart_Printf("\nBegin Send Data");
		ucResult=DataEncrypt(aucCommuBuf,(uiInLen+20),DataSave0.ConstantParamData.aucTerminalID,
										aucCommuBuf,&OutLen);
		/*		memset(aucEncryptData,0,sizeof(aucEncryptData));
		memcpy(aucEncryptData,pucInData,uiInLen);
		ucResult=DataEncrypt(aucEncryptData,uiInLen,DataSave0.ConstantParamData.aucTerminalID,
										aucEncryptData,&uiOutLen);
		if(ucResult!=SUCCESS)
			return ucResult;*/
		Uart_Printf("\nDataEncrypt ucResult=%02x\n",ucResult);
		if(ucResult!=SUCCESS)
		{
			COMMSENCRYPT_ErrMsg(ucResult);
			ucResult = ERR_CANCEL;
		}
		Uart_Printf("\nBegin Real Send Data");
		ucResult=TCP_Send(iTCPHandle,aucCommuBuf,OutLen);
		//ucResult=TCP_Send(iTCPHandle,aucCommuBuf,(uiInLen+10));
		Uart_Printf("\nTCP_Send ucResult=%02x\n",ucResult);


		MSG_DisplayMsg( true,0,0,MSG_RECEIVING);
		memset(ucBuf,0x00,sizeof(ucBuf));
		memset(aucCommuBuf,0,sizeof(aucCommuBuf));
		memcpy(ucBuf,"ʹ    ",14);
		if( !ucResult )
		{
		//	Uart_Printf("\nBegin Receive Data");
			for(uiI=1;uiI<=GUI_COMM_TIMEOUT;uiI++)
			{
#ifdef GUI_PROJECT			
memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
ProUiFace.ProToUi.uiLines=1;
int_asc(ProUiFace.ProToUi.aucLine1,2,&uiI);
ProUiFace.uiProgress=uiI;
sleep(1);
#endif
				ucResult = TCP_Recv(iTCPHandle,aucCommuBuf,&OutLen,1);
				int_asc(ucBuf+10,2,&uiI);
				//Os__GB2312_display(1,0,ucBuf);
				if(!ucResult) break;

			}

			ucResult = DataDecrypt(aucCommuBuf,aucCommuBuf,&OutLen);
			if(OutLen>17)
				*puiOutLen=OutLen-17;
			if( !ucResult )
			{
				if(*puiOutLen>NETWORK_MAXDATALEN)
					ucResult=ERR_NETCOMMS_OVERBUFLIMIT;
			}
#ifdef TEST
			Uart_Printf("OutLen=%d\n",OutLen);
			Uart_Printf("\nReceive Data End ucResult:[%02x]",ucResult);
#endif
			if( !ucResult )
			{

				memcpy(pucOutData,&aucCommuBuf[21],*puiOutLen);

#ifdef TEST
				Uart_Printf("\nReceive Data Len = %d\n",*puiOutLen);


				for(uiI=0;uiI<*puiOutLen;uiI++)
				{
					if((uiI+1)%20==0) Uart_Printf("\n");
					Uart_Printf("%02x ",pucOutData[uiI]);
				}
				Uart_Printf("\n");
#endif

			}else
			{
				COMMSENCRYPT_ErrMsg(ucResult);
				ucResult = ERR_CANCEL;
			}
		}


	}
	   else OSGSM_GprsReset();

#ifdef GUI_PROJECT
	memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
	ProUiFace.ProToUi.uiLines=1;	
	if(!ucResult)
	{
		memcpy(ProUiFace.ProToUi.aucLine1,"ͨѶɹ",8);		
	}else
		memcpy(ProUiFace.ProToUi.aucLine1,"ͨѶʧ",8);
sleep(1);
#endif

	TCP_Close(iTCPHandle);	
	
    ////Os__clr_display(255);
	////Os__GB2312_display(0,0,(unsigned char *)"ӹر");
	return(ucResult);
}

unsigned char COMMS_SendReceiveNetTest(unsigned char *pucInData,unsigned short uiInLen,
					unsigned char *pucOutData,unsigned short *puiOutLen,
					unsigned long ulHostIPAddress,unsigned int uiHostPort)
{


     unsigned char ucResult,ucIPAddress[5],ucCurrentIP,ucPreIP;
	unsigned short uiTimeout;
	int iTCPHandle,uiI;
	unsigned char ucBuf[20],aucComVersion[9];
	
	unsigned char 	aucTPDU[]={ 0x60,0x00,0x03,0x80,0x00,0xAA,0xBB,0x31,0x31};
	unsigned int 	uiLen=0,uiPort;
	unsigned char 	aucCommuBuf[512];
	unsigned short 	OutLen=0;
	unsigned char   i;

	Uart_Printf("\n\n\n.1.COMMS_SendReceiveNetTest uiInLen :%d.............\n\n\n",uiInLen);
//---
memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
ProUiFace.ProToUi.uiLines=1;
memcpy(ProUiFace.ProToUi.aucLine1,"...",11);
Os__xdelay(GUI_DELAY_TIMEOUT);

    ucResult=TCP_Open(&iTCPHandle);
    Uart_Printf("\n End TCP_Open ucResult:[%02x]",ucResult);
    if(ucResult!=SUCCESS) return ucResult;
    //Os__clr_display(255);
	//Os__GB2312_display(0,0,(uchar *)"...");
	for(i=0;i<3;i++)
	{
		if(NormalTransData.ucTransType==TRANS_S_INIONLINE)
		{
			memset(ucIPAddress,0x00,sizeof(ucIPAddress));
			memcpy(ucIPAddress,&ulHostIPAddress,sizeof(unsigned long));
			Uart_Printf("\n ucIPAddress = %03d.%03d.%03d.%03d \n ",
							ucIPAddress[0],ucIPAddress[1],ucIPAddress[2],ucIPAddress[3]);
			Uart_Printf("\n uiHostPort = %04d",uiHostPort);
			ucResult = TCP_Connect(iTCPHandle,tab_long(ucIPAddress,4),uiHostPort);
		}
		else
		{
			if(i%2==0)
			{
				memset(ucIPAddress,0x00,sizeof(ucIPAddress));
				if(RunData.uc1stIpTye==1||DataSave0.ChangeParamData.ucCurrentIPFlag==0)
				{
					memcpy(ucIPAddress,&DataSave0.ConstantParamData.ulHostIPAdd1,sizeof(unsigned long));
					uiPort=DataSave0.ConstantParamData.uiHostPort1;
					ucCurrentIP=0;
				}
				else
				{
					memcpy(ucIPAddress,&DataSave0.ConstantParamData.ulHostIPAdd2,sizeof(unsigned long));
					uiPort=DataSave0.ConstantParamData.uiHostPort2;
					ucCurrentIP=1;
				}
				Uart_Printf("\n ucIPAddress = %03d.%03d.%03d.%03d \n ",
								ucIPAddress[0],ucIPAddress[1],ucIPAddress[2],ucIPAddress[3]);
				Uart_Printf("\n uiHostPort = %04d",uiPort);
				ucResult = TCP_Connect(iTCPHandle,tab_long(ucIPAddress,4),uiPort);

			}
			else
			{
				memset(ucIPAddress,0x00,sizeof(ucIPAddress));
				memcpy(ucIPAddress,&DataSave0.ConstantParamData.ulHostIPAdd2,sizeof(unsigned long));
				if(RunData.uc1stIpTye!=1&&DataSave0.ChangeParamData.ucCurrentIPFlag==1)
				{
					memcpy(ucIPAddress,&DataSave0.ConstantParamData.ulHostIPAdd1,sizeof(unsigned long));
					uiPort=DataSave0.ConstantParamData.uiHostPort1;
					ucCurrentIP=0;
				}
				else
				{
					memcpy(ucIPAddress,&DataSave0.ConstantParamData.ulHostIPAdd2,sizeof(unsigned long));
					uiPort=DataSave0.ConstantParamData.uiHostPort2;
					ucCurrentIP=1;
				}
				Uart_Printf("\n ucIPAddress = %03d.%03d.%03d.%03d \n ",
								ucIPAddress[0],ucIPAddress[1],ucIPAddress[2],ucIPAddress[3]);
				Uart_Printf("\n uiHostPort = %04d",uiPort);
				ucResult = TCP_Connect(iTCPHandle,tab_long(ucIPAddress,4),uiPort);
			}
		}
		if(!ucResult)
			break;
	}
	
	Uart_Printf("\n Tcp_Connect Out ucResult: [%02x]  \n",ucResult);
    if( !ucResult )
    {
		
		if(ucPreIP!=ucCurrentIP)
			DataSave0.ChangeParamData.ucSwitchIPFlag=1;
		DataSave0.ChangeParamData.ucCurrentIPFlag=ucCurrentIP;
		XDATA_Write_Vaild_File(DataSaveChange);




		MSG_DisplayMsg( true,0,0,MSG_SENDING);

		

#ifdef TEST
    	Uart_Printf("\nConnect Server OK");

		Uart_Printf("\n*******Send  Packet Data*******\n");
		for(uiI=0;uiI<uiInLen;uiI++)
		{
			if((uiI+1)%20==0) Uart_Printf("\n");

			Uart_Printf("%02x ",pucInData[uiI]);
		}
#endif
		ucResult=TCP_Send(iTCPHandle,pucInData,uiInLen);

		MSG_DisplayMsg( true,0,0,MSG_RECEIVING);
		memset(ucBuf,0x00,sizeof(ucBuf));
		memset(aucCommuBuf,0,sizeof(aucCommuBuf));
		memcpy(ucBuf,"ʹ    ",14);
		if( !ucResult )
		{
			//	Uart_Printf("\nBegin Receive Data");
				for(uiI=1;uiI<=GUI_COMM_TIMEOUT;uiI++)
				{
//--		
memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
ProUiFace.ProToUi.uiLines=1;
int_asc(ProUiFace.ProToUi.aucLine1,2,&uiI);
ProUiFace.uiProgress=uiI;
Os__xdelay(GUI_DELAY_TIMEOUT);

					ucResult = TCP_Recv(iTCPHandle,aucCommuBuf,&OutLen,1);
					int_asc(ucBuf+10,2,&uiI);
					//Os__GB2312_display(1,0,ucBuf);
					if(!ucResult) break;
				}
				if( !ucResult )
				{

					memcpy(pucOutData,aucCommuBuf,*puiOutLen);

#ifdef TEST
					Uart_Printf("\nReceive Data Len = %d\n",*puiOutLen);


					for(uiI=0;uiI<*puiOutLen;uiI++)
					{
						if((uiI+1)%20==0) Uart_Printf("\n");
						Uart_Printf("%02x ",pucOutData[uiI]);
					}
					Uart_Printf("\n");
#endif

				}
		}


	}
       else OSGSM_GprsReset();

    ////Os__clr_display(255);
	////Os__GB2312_display(0,0,(unsigned char *)"ӹر");
	if(!ucResult)
	{
        //Os__clr_display(255);
		//Os__GB2312_display(0,0,(unsigned char *)"ӳɹ");
	}
	
	memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
	ProUiFace.ProToUi.uiLines=1;	
	if(!ucResult)
        memcpy(ProUiFace.ProToUi.aucLine1,"连接成功",16);
	else
        memcpy(ProUiFace.ProToUi.aucLine1,"连接失败",16);

	Os__xdelay(GUI_DELAY_TIMEOUT);
	
	TCP_Close(iTCPHandle);
    Uart_Printf("\n End TCP_Close\n");
	return(ucResult);
}

