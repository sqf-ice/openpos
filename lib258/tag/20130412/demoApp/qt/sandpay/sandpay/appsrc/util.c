#include <include.h>
#include <global.h>
#include <xdata.h>
#include <toolslib.h>
#include <osgraph.h>
#include <stdio.h>
#include "new_drv.h"


void UTIL_Beep(void)
{
    char i;
#ifdef GUI_TTS_DATA
    for(i=0;i<7;i++)
	Uart_Printf("\n Os__beep()");
	Uart_Printf("\n");
#else
    for(i=0;i<7;i++)
        Os__beep();
#endif
}

void UTIL_ClearGlobalData(void)
{
	memset((unsigned char *)&NormalTransData,0,sizeof(NormalTransData));
	memset((unsigned char *)&RunData,0,sizeof(RunData));
	memset((unsigned char *)&ExtraTransData,0,sizeof(EXTRATRANS));
	memset((unsigned char *)&ISO8583Data,0,sizeof(ISO8583Data));
	memset((unsigned char *)&DialParam,0,sizeof(DialParam));
	memset((unsigned char *)&InceptTransData,0,sizeof(InceptTransData));
	memset((unsigned char *)&InceptQueryData,0,sizeof(InceptQueryData));
	memset((unsigned char *)&Com1_Info,0,sizeof(Com1_Info));
	memset((unsigned char *)&HeadData,0,sizeof(HeadData));
	memset((unsigned char *)&ReceiveTransData,0,sizeof(ReceiveTransData));
	memset((unsigned char *)&SendTransData,0,sizeof(SendTransData));
	memset((unsigned char *)&PortData,0,sizeof(PortData));
	memset(PORTAUCBUF,0,sizeof(PORTAUCBUF));

	OSMEM_Memset((unsigned char *)&DataSave0,0,sizeof(DATASAVEPAGE0));
	OSMEM_Memset((unsigned char *)&DataSave1,0,sizeof(DATASAVEPAGE1));

	XDATA_Read_Vaild_File(DataSaveConstant);    
	
	XDATA_Read_Vaild_File(DataSaveChange);
	XDATA_Read_Vaild_File(DataSaveTransInfo);
	XDATA_Read_Vaild_File(DataSaveSaveISO8583);
	XDATA_Read_Vaild_File(DataSaveSendISO8583);
	XDATA_Read_Vaild_File(DataSaveReversalISO8583);
	XDATA_Read_Vaild_File(DataSaveBatchFlags);
	XDATA_Read_Vaild_File(DataSaveBlackFeilds);
	XDATA_Read_Vaild_File(DataSaveRedoTrans);
	XDATA_Read_Vaild_File(DataSaveExpInfo);

    Uart_Printf("\n TID = %s\n ", DataSave0.ConstantParamData.aucTerminalID);
    Uart_Printf("\n MID = %s\n ", DataSave0.ConstantParamData.aucMerchantID);


	NORMALTRANS_ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
	memcpy(RunData.aucComVersion,COM_VERSION,5);
	if(DataSave0.ConstantParamData.aucSuperPassword[0]==0x00)
	{
		memcpy(DataSave0.ConstantParamData.aucSuperPassword,"123456",6);
		XDATA_Write_Vaild_File(DataSaveConstant);
	}
}

void UTIL_GetTerminalInfo(void)
{
    
    memset(RUNDATA_aucTerminalID,0,sizeof(RUNDATA_aucTerminalID));
    memcpy(RUNDATA_aucTerminalID,DataSave0.ConstantParamData.aucTerminalID,   PARAM_TERMINALIDLEN);
    memcpy(RUNDATA_aucMerchantID,DataSave0.ConstantParamData.aucMerchantID,   PARAM_MERCHANTIDLEN);       
}

unsigned char  UTIL_GetMenu_Value(unsigned char ucTransType,unsigned short uiIndex,
		unsigned char (*pfnProc)(void),unsigned char *pucFlag,SELMENU *Menu)
{
	if(Menu->ucMenuCnt>MAXMENUDISPLINE)
		return(ERR_NOTPROC);
	if( (pucFlag !=NULL)&&(*pucFlag == 0) )
		return(ERR_NOTPROC);
	Menu->DispMenu[Menu->ucMenuCnt].ucTransType=ucTransType;
	Menu->DispMenu[Menu->ucMenuCnt].uiDispIndex=uiIndex;
	Menu->DispMenu[Menu->ucMenuCnt].pfnProc=pfnProc;
	Menu->ucMenuCnt++;
	return SUCCESS;
}



unsigned char UTIL_Input(unsigned char ucLine,unsigned char ucClrFlag,
                unsigned char ucMin, unsigned char ucMax,
                unsigned char ucType,
                unsigned char *pucBuf,
                unsigned char *pucMask)
{
    const unsigned char aucKeyTab[][7]
    ={  
//      {"0 *\0"},
        {"0 *,-\0"},
        {"1QZ#\0"},
        {"2ABC\0"},
        {"3DEF\0"},
        {"4GHI\0"},
        {"5JKL\0"},
        {"6MNO\0"},
        {"7PRS\0"},
        {"8TUV\0"},
        {"9WXY\0"},
        {"0,.\0"}
    };

    unsigned char   aucDisp[52];    /* Buffer for Display */
    unsigned char   aucInput[52];   /* Buffer for Key Input */
    unsigned char   ucKey;
    unsigned char   ucLastKey;
    unsigned char   ucKeyTabOffset;
    unsigned char   ucEnd;
    unsigned char   ucRedraw;
    unsigned char   ucCount;
    unsigned char   ucOffset;
    DRV_OUT *pdKey;
    static unsigned int uiTimeout;
    static unsigned int uiLastTime;
    memset(&aucDisp[0],0,sizeof(aucDisp));
    memset(&aucInput[0],0,sizeof(aucInput));
    ucLastKey = 0;
    ucEnd = FALSE;
//  ucRedraw = FALSE;
    ucRedraw = TRUE;
    uiLastTime = uiTimeout;
    ucCount = 0;
    
//Uart_Printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
    if( ucClrFlag )
    {
        //Os__clr_display(ucLine);
    }
    if( strlen((char *)pucBuf))
    {
        ucRedraw = TRUE;
        ucCount = strlen((char *)pucBuf);
        if( ucCount > sizeof(aucInput))
            ucCount = sizeof(aucInput);
        memcpy(aucInput,pucBuf,ucCount);            
    }   
//    Os__timer_start(&uiTimeout);
    do
    {
        if( ucRedraw == TRUE)
        {
            memset(&aucDisp[0],0,sizeof(aucDisp));
            if( ucCount > 15)
            {
                if(  (ucType == 'p')
                   ||(ucType == 'P')
                  )
                {
                    memset(&aucDisp[0],'*',15);
                }else
                {
#if 1
Uart_Printf("ucCount=%d\n",ucCount);
                    if(ucCount > ucMax)
                    {
                        if((ucType=='a')||(ucType=='A'))
                        {
                            memcpy(&aucDisp[0],&aucInput[ucCount-15],ucCount-1);
Uart_Printf("aucDisp = %s\n",aucDisp);
                        }
                        else
                            memcpy(&aucDisp[0],&aucInput[ucCount-15],ucCount);
                    }else
#endif
                    memcpy(&aucDisp[0],&aucInput[ucCount-15],15);
                }
                if(ucCount > ucMax)
                {
                    if((ucType=='a')||(ucType=='A'))
                        aucDisp[ucCount-1] = '_';
                    else
                        aucDisp[ucCount] = '_';
                }else
                    aucDisp[ucCount] = '_';
            }else
            {
                if(  (ucType == 'p')||(ucType == 'P') )
                {
                    memset(&aucDisp[0],'*',ucCount);
                }else
                {
#if 1
                    if(ucCount > ucMax)
                    {
                        if((ucType=='a')||(ucType=='A'))
                            memcpy(&aucDisp[0],&aucInput[0],ucCount-1);
                        else
                            memcpy(&aucDisp[0],&aucInput[0],ucCount);
                    }else
#endif
                        memcpy(&aucDisp[0],&aucInput[0],ucCount);
                }
                if(ucCount > ucMax)
                {
                    if((ucType=='a')||(ucType=='A'))
                        aucDisp[ucCount-1] = '_';
                    else
                        aucDisp[ucCount] = '_';
                }else
                    aucDisp[ucCount] = '_';
            }
            //Os__clr_display(ucLine);
            Os__display(ucLine,0,&aucDisp[0]);
            ucRedraw = FALSE;
        }
        uiTimeout = 50*ONESECOND;
        Os__timer_start(&uiTimeout);
        uiLastTime = uiTimeout;
        pdKey = Os__get_key();
        
        do{
        }while(  (pdKey->gen_status==DRV_RUNNING) &&(uiTimeout !=0)  );
        
        if (uiTimeout == 0)
        {
            Os__abort_drv(drv_mmi);
            ucKey = ERR_CANCEL;
            break;
        }else
        {
            ucKey = pdKey->xxdata[1];
        }
        if(  (ucKey >= '0') &&(ucKey <= '9') )
        {
            if( pucMask )
            {
                if( !strchr((char *)pucMask, ucKey ))
                {
                    Uart_Printf("ch not found\n");
                    Os__timer_stop(&uiTimeout);
                    continue;
                }
            }
            switch(ucType)
            {
            case 'h':
            case 'H':
                if( ucCount >= ucMax)
                {
                    Os__beep();
                    break;
                }
                if( ucLastKey != ucKey)
                {
                    ucLastKey = ucKey;
                    ucKeyTabOffset = 0;
                    aucInput[ucCount++] = ucKey;
                    ucRedraw = TRUE;
                }else
                {
                    if(  (uiLastTime-uiTimeout) < 1*ONESECOND)
                    {
                        if( ucCount )
                        {
                            ucOffset = ucCount - 1;
                        }else
                        {
                            ucOffset = 0;
                        }
                        if(  (ucKey >= '2')  &&(ucKey <= '3') )
                        {
                            if( ucKeyTabOffset < strlen((char *)aucKeyTab[ucKey-'0'])-1)
                            {
                                ucKeyTabOffset ++;
                                ucRedraw = TRUE;
                            }else
                            {
                                if( ucKeyTabOffset )
                                {
                                    ucKeyTabOffset = 0;
                                    ucRedraw = TRUE;
                                }
                            }
                            aucInput[ucOffset] = aucKeyTab[ucKey-'0'][ucKeyTabOffset];
                                    }else
                        {
                            ucLastKey = ucKey;
                            ucKeyTabOffset = 0;
                            aucInput[ucCount++] = ucKey;
                            ucRedraw = TRUE;
                        }
                    }
                    else
                    {
                        if( ucCount >= ucMax)
                        {
                            Os__beep();
                            break;
                        }
                        ucLastKey = ucKey;
                        ucKeyTabOffset = 0;
                        aucInput[ucCount++] = ucKey;
                        ucRedraw = TRUE;                            
                    }
                }
                break;
            case 'a':
            case 'A':
Uart_Printf("first ucCount=%d,ucMax=%d\n",ucCount,ucMax);
                                                                          
                if( ucCount > ucMax)
                {
Uart_Printf(" break                              first ucCount=%d,ucMax=%d\n",ucCount,ucMax);
                    Os__beep();
                    break;
                }
 Uart_Printf("bbbb ucCount = %d\n");         
                if(( ucLastKey != ucKey)&&(ucCount < ucMax))
                {
Uart_Printf("ucCount++\n");
                    ucLastKey = ucKey;
                    ucKeyTabOffset = 0;
                    aucInput[ucCount++] = ucKey;
                    ucRedraw = TRUE;        
                }else
                {
                    if((uiLastTime-uiTimeout) < 1*ONESECOND)
                    {
                        if( ucCount )
                        {
                            ucOffset = ucCount - 1;
                        }else
                        {
                            ucOffset = 0;
                        }
                        if( ucKeyTabOffset < strlen((char *)aucKeyTab[ucKey-'0'])-1)
                        {
                            ucKeyTabOffset ++;
                            ucRedraw = TRUE;
                        }else
                        {
                            if( ucKeyTabOffset != 0)
                            {
                                ucKeyTabOffset = 0;
                                ucRedraw = TRUE;
                            }
                        }   
Uart_Printf("in one second ucCount=%d\n",ucCount);                        
                        aucInput[ucOffset] = aucKeyTab[ucKey-'0'][ucKeyTabOffset];
                    }else
                    {
                        if( ucCount >= ucMax)
                        {
                            Os__beep();
                            break;
                        }
                        ucLastKey = ucKey;
                        ucKeyTabOffset = 0;
                        aucInput[ucCount++] = ucKey;
                        ucRedraw = TRUE;        
Uart_Printf("out one second ucCount=%d\n",ucCount);
                    }
                }
Uart_Printf("ucCount2=%d\n",ucCount);
                break;
            default:
                if( ucCount >= ucMax)
                {
                    Os__beep();
                    break;
                }
                aucInput[ucCount++] = ucKey;
                ucRedraw = TRUE;
                break;
            }
        }
        else
        {
            switch(ucKey)
            {
            case KEY_CLEAR:
                if( ucCount )
                {
                    ucCount = 0;
                    ucLastKey = 0;
                    memset(&aucInput[0],0,sizeof(aucInput));
                    memset(&aucDisp[0],0,sizeof(aucDisp));
                    ucRedraw = TRUE;
                }else
                {
                    ucKey = ERR_CANCEL;
                    ucEnd = TRUE;
                }
                break;
            case KEY_00_PT:
//          case 0x00:
                if(  (ucType == 'h')||(ucType == 'H')||(ucType == 'a')||(ucType == 'A') )
                {
//                  if( ucCount < strlen((char *)&aucInput[0]))
//                      ucCount ++;
                    if( ucCount > ucMax)
                    {
                        Os__beep();
                        break;
                    }
                    if( ucLastKey != ucKey)
                    {
                        ucLastKey = ucKey;
                        ucKeyTabOffset = 0;
                        if((ucKey != '0')&&(ucKey != ',')&&(ucKey != '.'))
                            aucInput[ucCount++] = '0';
                        else
                            aucInput[ucCount++] = ucKey;
                        ucRedraw = TRUE;        
                    }else
                    {
                        if(  (uiLastTime-uiTimeout) < 1*ONESECOND)
                        {
                            if( ucCount )
                            {
                                ucOffset = ucCount - 1;
                            }else
                            {
                                ucOffset = 0;
                            }
                            if( ucKeyTabOffset < strlen((char *)aucKeyTab[10])-1)
                            {
                                ucKeyTabOffset ++;
                                ucRedraw = TRUE;
                            }else
                            {
                                if( ucKeyTabOffset != 0)
                                {
                                    ucKeyTabOffset = 0;
                                    ucRedraw = TRUE;
                                }
                            }   
                            aucInput[ucOffset] = aucKeyTab[10][ucKeyTabOffset];
                        }
                        else
                        {
                            if( ucCount >= ucMax)
                            {
                                Os__beep();
                                break;
                            }
                            ucLastKey = ucKey;
                            ucKeyTabOffset = 0;
                            if((ucKey != '0')&&(ucKey != ',')&&(ucKey != '.'))
                                aucInput[ucCount++] = '0';
                            else
                                aucInput[ucCount++] = ucKey;
                            ucRedraw = TRUE;        
                        }
                    }
                }
                else
                {
                    if( ucCount > (ucMax-2))
                    {
                        Os__beep();
                        break;
                    }
//                  aucInput[ucCount] = ucKey;
                    memcpy(&aucInput[ucCount],"00",2);
                    ucCount = ucCount +2;
                    ucRedraw = TRUE;
                    break;
                }
                break;
            case KEY_BCKSP:
                if( ucCount )
                {
                    if(ucCount > ucMax)
                    {
                        if((ucType=='a')||(ucType=='A'))
                            ucCount = ucCount-2;
                        else
                            ucCount = ucCount -1;
                    }else
                        ucCount --;
                    
                    aucInput[ucCount] = 0;
                    ucRedraw = TRUE;
                }
                break;
            case KEY_ENTER:
                if( ucCount < ucMin )
                {
                    Os__beep();
                }else
                {
                    memset((char *)pucBuf,0x00,sizeof(pucBuf));
                    strcpy((char *)pucBuf,(char *)&aucInput[0]);
                    ucEnd = TRUE;
                }
                break;
            default :
                break;
            }
        }
//        uiLastTime = uiTimeout;
        Os__timer_stop(&uiTimeout);
        if( ucEnd == TRUE)
        {
            break;
        }   
    }while(1);
//    Os__timer_stop(&uiTimeout);         
    return(ucKey);
}


unsigned char UTIL_Input_pp(unsigned char ucLine,unsigned char ucClrFlag,
                unsigned char ucMin, unsigned char ucMax,
                unsigned char ucType,
                unsigned char *pucBuf)
                
{
    unsigned char   aucDisp[40];    /* Buffer for Display */
    unsigned char   aucInput[40];   /* Buffer for Key Input */
    unsigned char   ucKey;
    unsigned char   ucLastKey;
    unsigned char   ucEnd;
    unsigned char   ucRedraw;
    unsigned char   ucCount;
    DRV_OUT *pdKey;
    static unsigned int uiTimeout;
    static unsigned int uiLastTime;
    memset(&aucDisp[0],0,sizeof(aucDisp));
    memset(&aucInput[0],0,sizeof(aucInput));
    ucLastKey = 0;
    ucEnd = FALSE;
    ucRedraw = FALSE;
    uiTimeout = 50*ONESECOND;
    uiLastTime = uiTimeout;
    ucCount = 0;
    Os__light_on_pp();
    if( ucClrFlag )
    {
        //Os__clr_display_pp(ucLine);
    }
    if( strlen((char *)pucBuf))
    {
        ucRedraw = TRUE;
        ucCount = strlen((char *)pucBuf);
        if( ucCount > sizeof(aucInput))
            ucCount = sizeof(aucInput);
        memcpy(aucInput,pucBuf,ucCount);            
    }
    Os__timer_start(&uiTimeout);

    do
    {
        if( ucRedraw == TRUE)
        {
            memset(&aucDisp[0],0,sizeof(aucDisp));
            if( ucCount > 15)
            {
                if(  (ucType == 'p')
                   ||(ucType == 'P')
                  )
                {
                    memset(&aucDisp[0],'*',15);
                }else
                {
                    memcpy(&aucDisp[0],&aucInput[ucCount-15],15);
                }
            }else
            {
                if(  (ucType == 'p')
                   ||(ucType == 'P')
                  )
                {
                    if(ucCount == 0)
                    {
                        memset(aucDisp,' ',sizeof(aucDisp));
                        aucDisp[15] = 0x00;
                    }
                    else
                        memset(&aucDisp[0],'*',ucCount);
                }else
                {
                    memcpy(&aucDisp[0],&aucInput[0],ucCount);
                }
            }
            //Os__clr_display_pp(ucLine);
            Os__display_pp(ucLine,0,&aucDisp[0]);
            ucRedraw = FALSE;
        }
        
        pdKey = Os__get_key_pp();

        do{
        }while(  (pdKey->gen_status==DRV_RUNNING)
               &&(uiTimeout !=0)
               );

        if (uiTimeout == 0)
        {
            Os__abort_drv(drv_pad);
            ucKey = ERR_CANCEL;
            break;
        }else
        {
            ucKey = pdKey->xxdata[1];
        }
        if(  (ucKey >= '0')
           &&(ucKey <= '9')
          )
        {
            if( ucCount >= ucMax)
            {
                Os__beep();
                continue;
            }
            aucInput[ucCount++] = ucKey;
            ucRedraw = TRUE;
        }else
        {
            switch(ucKey)
            {
            case KEY_CLEAR:
                if( ucCount )
                {
                    ucCount = 0;
                    ucLastKey = 0;
                    memset(&aucInput[0],0,sizeof(aucInput));
                    memset(&aucDisp[0],0,sizeof(aucDisp));
                    ucRedraw = TRUE;
                }else
                {
                    ucKey = ERR_CANCEL;
                    ucEnd = TRUE;
                }
                break;
            case KEY_ENTER:
                if( ucCount < ucMin )
                {
                    Os__beep();
                }else
                {
                    memset((char *)pucBuf,0x00,sizeof(pucBuf));
                    strcpy((char *)pucBuf,(char *)&aucInput[0]);
                    ucEnd = TRUE;
                }
                break;
            default :
                break;
            }
        }
        uiLastTime = uiTimeout;
        if( ucEnd == TRUE)
        {
            break;
        }   
    }while(1);

    Os__timer_stop(&uiTimeout);
    for(uiTimeout=0;uiTimeout<20000;uiTimeout++);
    if(ucClrFlag!=1&&ucClrFlag!=2)
    {
	    //Os__clr_display_pp(255);
	    //Os__GB2312_display_pp(0,0,(unsigned char *)"   ڴ");
	    //Os__GB2312_display_pp(1,0,(unsigned char *)"   Ժ.....");
 		}           
    return(ucKey);
}


unsigned char UTIL_DisplayMenu(SELMENU *pMenu)
{
	unsigned char ucI,ucJ;
	unsigned char ucKBKey;
	unsigned char ucLOKey1;
	unsigned char ucLOKey2;
	unsigned char ucCurrLine;
	unsigned char ucCurrPage;
	unsigned char ucMaxPage;
	unsigned char aucLine[9];
	unsigned char aucDispBuf[25];
	DRV_OUT * ptKey;

	if(!pMenu->ucMenuCnt)
		return(ERR_OSFUNC);
	ucMaxPage = pMenu->ucMenuCnt/4;
	if(pMenu->ucMenuCnt%4)
		ucMaxPage++;
	ucCurrPage = 0;
	memset(aucLine,0,sizeof(aucLine));
	do{
		//Os__clr_display(255);
		ucCurrLine =  pMenu->ucMenuCnt-ucCurrPage*4;
		for(ucI=0;ucI<4&&ucI<ucCurrLine;ucI++)
		{
			ucJ = ucI+ucCurrPage*4;
			aucLine[0] = (ucJ+1)+0x30;	/*1ſʼʾ*/
			aucLine[1] = '.';
			Os__display(ucI,0,aucLine);
			memset(aucDispBuf,0,sizeof(aucDispBuf));
			MSG_GetMsg(pMenu->DispMenu[ucJ].uiDispIndex,aucDispBuf,sizeof(aucDispBuf));
			//Os__GB2312_display(ucI,1,aucDispBuf);
		}
		ucKBKey = Os__xget_key();	
		switch(ucKBKey)
		{
			case KEY_ENTER:
				ucCurrPage++;
				if(ucCurrPage>=ucMaxPage)
					ucCurrPage = 0;
				break;	
			case KEY_CLEAR:
				return(ERR_CANCEL);					
			case KEY_F3:
				ucCurrPage++;
				if(ucCurrPage>=ucMaxPage)
					ucCurrPage = 0;
				break;
			case KEY_F4:
				if(ucCurrPage)
					ucCurrPage--;
				else
					ucCurrPage = ucMaxPage-1;	
				break;
			case KEY_PAPER_FEED:
				Os__xlinefeed(6*LINENUM);
				continue;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				ucKBKey = ucKBKey&0x0F;
				NORMALTRANS_ucTransType = pMenu->DispMenu[ucKBKey-1].ucTransType;
				if(ucKBKey<=pMenu->ucMenuCnt)
				{				
					if( pMenu->DispMenu[ucKBKey-1].pfnProc )
					{
						return((*(pMenu->DispMenu[ucKBKey-1].pfnProc))());
					}
				}	
				break;									
			default:
				break;				
		}
	}while(1);		
}





unsigned char UTIL_Input_XY(unsigned char ucLine,unsigned char ucCol,unsigned char ucClrFlag,unsigned char ucMin, 
                unsigned char ucMax,unsigned char ucType,unsigned char *pucBuf,unsigned char *pucMask)
{
    const unsigned char aucKeyTab[][7]
    ={  {"0 *,\0"},{"1#\0"},{"2ABC\0"}, {"3DEF\0"},{"4GHI\0"},  {"5JKL\0"},{"6MNO\0"},  
        {"7PQRS\0"},    {"8TUV\0"},{"9WXYZ\0"},{"0,.\0"}};

    unsigned char   aucDisp[40],aucInput[40],   ucKey,ucLastKey,ucKeyTabOffset, ucEnd,ucRedraw;
    unsigned char   ucCount,ucOffset;
    DRV_OUT *pdKey;
    static unsigned int uiTimeout,uiLastTime;
    
    memset(&aucDisp[0],0,sizeof(aucDisp));
    memset(&aucInput[0],0,sizeof(aucInput));
    ucLastKey = 0;
    ucEnd = FALSE;
    ucRedraw = TRUE;
    uiTimeout = 50*ONESECOND;
    uiLastTime = uiTimeout;
    ucCount = 0;

    if( ucClrFlag )
        //Os__clr_display(ucLine);    
    if( strlen((char *)pucBuf))
    {
        ucRedraw = TRUE;
        ucCount = strlen((char *)pucBuf);
        if( ucCount > sizeof(aucInput))
            ucCount = sizeof(aucInput);
        memcpy(aucInput,pucBuf,ucCount);            
    }   
    Os__timer_start(&uiTimeout);
    do
    {
        if( ucRedraw == TRUE)
        {
            memset(&aucDisp[0],0,sizeof(aucDisp));
            if( ucCount > 15)
            {
                if(  (ucType == 'p')||(ucType == 'P') )     
                    memset(&aucDisp[0],'*',15);
                else        
                    memcpy(&aucDisp[0],&aucInput[ucCount-15],15);
                aucDisp[ucCount] = '_';
            }else
            {
                if(  (ucType == 'p')||(ucType == 'P') )
                    memset(&aucDisp[0],'*',ucCount);
                else
                    memcpy(&aucDisp[0],&aucInput[0],ucCount);               
                aucDisp[ucCount] = '_';
            }
            //Os__clr_display(ucLine);
            Os__display(ucLine,ucCol,&aucDisp[0]);
            ucRedraw = FALSE;
        }       
        pdKey = Os__get_key();      
        do{ }while(  (pdKey->gen_status==DRV_RUNNING) &&(uiTimeout !=0)  );     
        if (uiTimeout == 0)
        {
            Os__abort_drv(drv_mmi);
            ucKey = ERR_CANCEL;
            break;
        }else
            ucKey = pdKey->xxdata[1];       
        if(  (ucKey >= '0') &&(ucKey <= '9') )
        {
            if( pucMask )
                if( !strchr((char *)pucMask, ucKey )) continue;
            switch(ucType)
            {
                    case 'h':
            case 'H':
                if( ucCount > ucMax)
                {
                    Os__beep();
                    break;
                }
                if( ucLastKey != ucKey)
                {
                    ucLastKey = ucKey;
                    ucKeyTabOffset = 0;
                    aucInput[ucCount++] = ucKey;
                    ucRedraw = TRUE;
                }else
                {
                                if(  (uiLastTime-uiTimeout) < 1*ONESECOND)
                    {
                                    if( ucCount )
                            ucOffset = ucCount - 1;
                        else    ucOffset = 0;                       
                                    if(  (ucKey >= '2')  &&(ucKey <= '3') )
                        {
                                        if( ucKeyTabOffset < strlen((char *)aucKeyTab[ucKey-'0'])-1)
                            {
                                ucKeyTabOffset ++;
                                ucRedraw = TRUE;
                            }else
                            {
                                if( ucKeyTabOffset )
                                {
                                    ucKeyTabOffset = 0;
                                    ucRedraw = TRUE;
                                }
                            }
                            aucInput[ucOffset] = aucKeyTab[ucKey-'0'][ucKeyTabOffset];
                                    }else
                        {
                            ucLastKey = ucKey;
                            ucKeyTabOffset = 0;
                            aucInput[ucCount++] = ucKey;
                            ucRedraw = TRUE;
                        }
                                }
                                else
                    {
                        if( ucCount >= ucMax)
                        {
                            Os__beep();
                            break;
                        }
                        ucLastKey = ucKey;
                        ucKeyTabOffset = 0;
                        aucInput[ucCount++] = ucKey;
                        ucRedraw = TRUE;                            
                    }
                        }
                break;
                    case 'a':
                    case 'A':
                if( ucCount > ucMax)
                {
                    Os__beep();
                    break;
                }
                if( ucLastKey != ucKey)
                {
                    ucLastKey = ucKey;
                    ucKeyTabOffset = 0;
                    aucInput[ucCount++] = ucKey;
                    ucRedraw = TRUE;        
                }else
                {
                    if(  (uiLastTime-uiTimeout) < 1*ONESECOND)
                    {
                        if( ucCount )
                            ucOffset = ucCount - 1;
                        else
                            ucOffset = 0;                       
                        if( ucKeyTabOffset < strlen((char *)aucKeyTab[ucKey-'0'])-1)
                        {
                            ucKeyTabOffset ++;
                            ucRedraw = TRUE;
                        }else
                        {
                            if( ucKeyTabOffset != 0)
                            {
                                ucKeyTabOffset = 0;
                                ucRedraw = TRUE;
                            }
                        }   
                        aucInput[ucOffset] = aucKeyTab[ucKey-'0'][ucKeyTabOffset];
                    }else
                    {
                        if( ucCount >= ucMax)
                        {
                            Os__beep();
                            break;
                        }
                        ucLastKey = ucKey;
                        ucKeyTabOffset = 0;
                        aucInput[ucCount++] = ucKey;
                        ucRedraw = TRUE;        
                    }
                }
                break;
                default:
                if( ucCount >= ucMax)
                {
                    Os__beep();
                    break;
                }
                aucInput[ucCount++] = ucKey;
                ucRedraw = TRUE;
                break;
            }
        }
        else
        {
            switch(ucKey)
            {
            case KEY_CLEAR:
                if( ucCount )
                {
                    ucCount = 0;
                    ucLastKey = 0;
                    memset(&aucInput[0],0,sizeof(aucInput));
                    memset(&aucDisp[0],0,sizeof(aucDisp));
                    ucRedraw = TRUE;
                }else
                {
                    ucKey = ERR_CANCEL;
                    ucEnd = TRUE;
                }
                break;
            case KEY_00_PT:
                if(  (ucType == 'h')||(ucType == 'H')||(ucType == 'a')||(ucType == 'A') )
                {
                    if( ucCount > ucMax)
                    {
                        Os__beep();
                        break;
                    }
                    if( ucLastKey != ucKey)
                    {
                        ucLastKey = ucKey;
                        ucKeyTabOffset = 0;
                        if((ucKey != '0')&&(ucKey != ',')&&(ucKey != '.'))
                            aucInput[ucCount++] = '0';
                        else
                            aucInput[ucCount++] = ucKey;
                        ucRedraw = TRUE;        
                    }else
                    {
    
                        if(  (uiLastTime-uiTimeout) < 1*ONESECOND)
                        {
                            if( ucCount )
                                ucOffset = ucCount - 1;
                            else    ucOffset = 0;
                            if( ucKeyTabOffset < strlen((char *)aucKeyTab[10])-1)
                            {
                                ucKeyTabOffset ++;
                                ucRedraw = TRUE;
                            }else
                            {
                                if( ucKeyTabOffset != 0)
                                {
                                    ucKeyTabOffset = 0;
                                    ucRedraw = TRUE;
                                }
                            }   
                            aucInput[ucOffset] = aucKeyTab[10][ucKeyTabOffset];
                        }
                        else
                        {
                            if( ucCount >= ucMax)
                            {
                                Os__beep();
                                break;
                            }
                            ucLastKey = ucKey;
                            ucKeyTabOffset = 0;
                            if((ucKey != '0')&&(ucKey != ',')&&(ucKey != '.'))
                                aucInput[ucCount++] = '0';
                            else
                                aucInput[ucCount++] = ucKey;
                            ucRedraw = TRUE;        
                        }
                    }
                }
                else
                {
                    if( ucCount > (ucMax-2))
                    {
                        Os__beep();
                        break;
                    }
//                  aucInput[ucCount] = ucKey;
                    memcpy(&aucInput[ucCount],"00",2);
                    ucCount = ucCount +2;
                    ucRedraw = TRUE;
                    break;
                }
                break;
            case KEY_BCKSP:
                if( ucCount )
                {
                    ucCount --;
                    aucInput[ucCount] = 0;
                    ucRedraw = TRUE;
                }
                break;
            case KEY_ENTER:
                if( ucCount < ucMin )
                    Os__beep();
                else
                {
                    memset((char *)pucBuf,0x00,sizeof(pucBuf));
                    strcpy((char *)pucBuf,(char *)&aucInput[0]);
                    ucEnd = TRUE;
                }
                break;
            default :
                break;
            }
        }
        uiLastTime = uiTimeout;
        if( ucEnd == TRUE)
            break;          
    }while(1);
    Os__timer_stop(&uiTimeout);         
    return(ucKey);
}





unsigned char UTIL_Input_XY_Exp(unsigned char ucLine,unsigned char ucCol,unsigned char ucClrFlag,unsigned char ucMin, 
                unsigned char ucMax,unsigned char ucType,unsigned char *pucBuf,unsigned char *pucMask)
{
    const unsigned char aucKeyTab[][7]
    ={  {"0 *,\0"},{"1#\0"},{"2ABC\0"}, {"3DEF\0"},{"4GHI\0"},  {"5JKL\0"},{"6MNO\0"},  
        {"7PQRS\0"},    {"8TUV\0"},{"9WXYZ\0"},{"0,.\0"}};

    unsigned char   aucDisp[40],aucInput[40],   ucKey,ucLastKey,ucKeyTabOffset, ucEnd,ucRedraw;
    unsigned char   ucCount,ucOffset;
    DRV_OUT *pdKey;
    static unsigned int uiTimeout,uiLastTime;
    
    memset(&aucDisp[0],0,sizeof(aucDisp));
    memset(&aucInput[0],0,sizeof(aucInput));
    ucLastKey = 0;
    ucEnd = FALSE;
    ucRedraw = TRUE;
    uiTimeout = 50*ONESECOND;
    uiLastTime = uiTimeout;
    ucCount = 0;

    if( ucClrFlag )
        //Os__clr_display(ucLine);    
    if( strlen((char *)pucBuf))
    {
        ucRedraw = TRUE;
        ucCount = strlen((char *)pucBuf);
        if( ucCount > sizeof(aucInput))
            ucCount = sizeof(aucInput);
        memcpy(aucInput,pucBuf,ucCount);            
    }   
    Os__timer_start(&uiTimeout);
    do
    {
        if( ucRedraw == TRUE)
        {
            memset(&aucDisp[0],0,sizeof(aucDisp));
            if( ucCount > 15)
            {
                if(  (ucType == 'p')||(ucType == 'P') )     
                    memset(&aucDisp[0],'*',15);
                else        
                    memcpy(&aucDisp[0],&aucInput[ucCount-15],15);
                aucDisp[ucCount] = '_';
            }else
            {
                if(  (ucType == 'p')||(ucType == 'P') )
                    memset(&aucDisp[0],'*',ucCount);
                else
                    memcpy(&aucDisp[0],&aucInput[0],ucCount);               
                aucDisp[ucCount] = '_';
            }
            //Os__clr_display(ucLine);
            Os__display(ucLine,ucCol,&aucDisp[0]);
            ucRedraw = FALSE;
        }       
        pdKey = Os__get_key();      
        do{ }while(  (pdKey->gen_status==DRV_RUNNING) &&(uiTimeout !=0)  );     
        if (uiTimeout == 0)
        {
            Os__abort_drv(drv_mmi);
            ucKey = ERR_CANCEL;
            break;
        }else
            ucKey = pdKey->xxdata[1];       
        if(  (ucKey >= '0') &&(ucKey <= '9') )
        {
            if( pucMask )
                if( !strchr((char *)pucMask, ucKey )) continue;
            switch(ucType)
            {
                    case 'h':
            case 'H':
                if( ucCount > ucMax)
                {
                    Os__beep();
                    break;
                }
                if( ucLastKey != ucKey)
                {
                    ucLastKey = ucKey;
                    ucKeyTabOffset = 0;
                    aucInput[ucCount++] = ucKey;
                    ucRedraw = TRUE;
                }else
                {
                                if(  (uiLastTime-uiTimeout) < 1*ONESECOND)
                    {
                                    if( ucCount )
                            ucOffset = ucCount - 1;
                        else    ucOffset = 0;                       
                                    if(  (ucKey >= '2')  &&(ucKey <= '3') )
                        {
                                        if( ucKeyTabOffset < strlen((char *)aucKeyTab[ucKey-'0'])-1)
                            {
                                ucKeyTabOffset ++;
                                ucRedraw = TRUE;
                            }else
                            {
                                if( ucKeyTabOffset )
                                {
                                    ucKeyTabOffset = 0;
                                    ucRedraw = TRUE;
                                }
                            }
                            aucInput[ucOffset] = aucKeyTab[ucKey-'0'][ucKeyTabOffset];
                                    }else
                        {
                            ucLastKey = ucKey;
                            ucKeyTabOffset = 0;
                            aucInput[ucCount++] = ucKey;
                            ucRedraw = TRUE;
                        }
                                }
                                else
                    {
                        if( ucCount >= ucMax)
                        {
                            Os__beep();
                            break;
                        }
                        ucLastKey = ucKey;
                        ucKeyTabOffset = 0;
                        aucInput[ucCount++] = ucKey;
                        ucRedraw = TRUE;                            
                    }
                        }
                break;
                    case 'a':
                    case 'A':
                if( ucCount > ucMax)
                {
                    Os__beep();
                    break;
                }
                if( ucLastKey != ucKey)
                {
                    ucLastKey = ucKey;
                    ucKeyTabOffset = 0;
                    aucInput[ucCount++] = ucKey;
                    ucRedraw = TRUE;        
                }else
                {
                    if(  (uiLastTime-uiTimeout) < 1*ONESECOND)
                    {
                        if( ucCount )
                            ucOffset = ucCount - 1;
                        else
                            ucOffset = 0;                       
                        if( ucKeyTabOffset < strlen((char *)aucKeyTab[ucKey-'0'])-1)
                        {
                            ucKeyTabOffset ++;
                            ucRedraw = TRUE;
                        }else
                        {
                            if( ucKeyTabOffset != 0)
                            {
                                ucKeyTabOffset = 0;
                                ucRedraw = TRUE;
                            }
                        }   
                        aucInput[ucOffset] = aucKeyTab[ucKey-'0'][ucKeyTabOffset];
                    }else
                    {
                        if( ucCount >= ucMax)
                        {
                            Os__beep();
                            break;
                        }
                        ucLastKey = ucKey;
                        ucKeyTabOffset = 0;
                        aucInput[ucCount++] = ucKey;
                        ucRedraw = TRUE;        
                    }
                }
                break;
                default:
                if( ucCount >= ucMax)
                {
                    Os__beep();
                    break;
                }
                aucInput[ucCount++] = ucKey;
                ucRedraw = TRUE;
                break;
            }
        }
        else
        {
            switch(ucKey)
            {
            case KEY_CLEAR:
                if( ucCount )
                {
                    ucCount = 0;
                    ucLastKey = 0;
                    memset(&aucInput[0],0,sizeof(aucInput));
                    memset(&aucDisp[0],0,sizeof(aucDisp));
                    ucRedraw = TRUE;
                }else
                {
                    ucKey = ERR_CANCEL;
                    ucEnd = TRUE;
                }
                break;
            case KEY_00_PT:
                if(  (ucType == 'h')||(ucType == 'H')||(ucType == 'a')||(ucType == 'A') )
                {
                    if( ucCount > ucMax)
                    {
                        Os__beep();
                        break;
                    }
                    if( ucLastKey != ucKey)
                    {
                        ucLastKey = ucKey;
                        ucKeyTabOffset = 0;
                        if((ucKey != '0')&&(ucKey != ',')&&(ucKey != '.'))
                            aucInput[ucCount++] = '0';
                        else
                            aucInput[ucCount++] = ucKey;
                        ucRedraw = TRUE;        
                    }else
                    {
    
                        if(  (uiLastTime-uiTimeout) < 1*ONESECOND)
                        {
                            if( ucCount )
                                ucOffset = ucCount - 1;
                            else    ucOffset = 0;
                            if( ucKeyTabOffset < strlen((char *)aucKeyTab[10])-1)
                            {
                                ucKeyTabOffset ++;
                                ucRedraw = TRUE;
                            }else
                            {
                                if( ucKeyTabOffset != 0)
                                {
                                    ucKeyTabOffset = 0;
                                    ucRedraw = TRUE;
                                }
                            }   
                            aucInput[ucOffset] = aucKeyTab[10][ucKeyTabOffset];
                        }
                        else
                        {
                            if( ucCount >= ucMax)
                            {
                                Os__beep();
                                break;
                            }
                            ucLastKey = ucKey;
                            ucKeyTabOffset = 0;
                            if((ucKey != '0')&&(ucKey != ',')&&(ucKey != '.'))
                                aucInput[ucCount++] = '0';
                            else
                                aucInput[ucCount++] = ucKey;
                            ucRedraw = TRUE;        
                        }
                    }
                }
                else
                {
                    if( ucCount > (ucMax-2))
                    {
                        Os__beep();
                        break;
                    }
//                  aucInput[ucCount] = ucKey;
                    memcpy(&aucInput[ucCount],"00",2);
                    ucCount = ucCount +2;
                    ucRedraw = TRUE;
                    break;
                }
                break;
            case KEY_BCKSP:
                if( ucCount )
                {
                    ucCount --;
                    aucInput[ucCount] = 0;
                    ucRedraw = TRUE;
                }
                break;
            case KEY_ENTER:
                if( ucCount < ucMin )
                    Os__beep();
                else
                {
                    memset((char *)pucBuf,0x00,sizeof(pucBuf));
                    strcpy((char *)pucBuf,(char *)&aucInput[0]);
                    ucEnd = TRUE;
                }
                break;
            default :
                break;
            }
        }
        uiLastTime = uiTimeout;
        if( ucEnd == TRUE)
            break;          
    }while(1);
    Os__timer_stop(&uiTimeout);         
    return(ucKey);
}

unsigned char UTIL_GetKey(unsigned char wait_sec)
{  
    unsigned int Timeout;
    DRV_OUT *KBoard;
    NEW_DRV_TYPE  new_drv;
    int ret ;

    Timeout=wait_sec*100;
    /*Os__timer_start(&Timeout);
    Os__xdelay(1);
    KBoard=Os__get_key();
    while ((Timeout!=0)&&(KBoard->gen_status==DRV_RUNNING));
    */
    ret = Os_Wait_Event(KEY_DRV, &new_drv, Timeout*10);
    if(ret == 0)
    {
        if(new_drv.drv_type == DRV_TIMEOUT)
        {
            Os__abort_drv(drv_mmi);
            Os__xdelay(10);
            return  99;
        }
    }
    return new_drv.drv_data.xxdata[1];

    //Os__timer_stop(&Timeout);
    //if(Timeout==0)
    //{
    //Os__abort_drv(drv_mmi);
    //Os__xdelay(10);
    //return  99;
    //}
//    return KBoard->xxdata[1];
}


unsigned char UTIL_SetPrinter(void)
{
    unsigned char ch,ucResult=SUCCESS;
PRN:    //Os__clr_display(255);
        //Os__GB2312_display(0,0,(unsigned char *)"ǰ");
        //Os__GB2312_display(1,0,(unsigned char *)"1. 2.");
        //Os__GB2312_display(2,0,(unsigned char *)"3.״");
        //Os__GB2312_display(3,0,(unsigned char *)"Ϊ:");
      	
        //if(DataSave0.ConstantParamData.prnflagL)
        	//Os__GB2312_display(0,6,(unsigned char *)"״");	
     //  else if(DataSave0.ConstantParamData.prnflag)
            //Os__GB2312_display(0,6,(unsigned char *)"");
      //  else
            //Os__GB2312_display(0,6,(unsigned char *)"");                
        ch=UTIL_GetKey(15);
        if(ch=='1') 
        {
            //Os__GB2312_display(3,6,(unsigned char *)"");
            ch=UTIL_GetKey(15);
            if(ch==KEY_ENTER) 
            {
				if(!DataSave0.ConstantParamData.prnflag)
					RunData.ucWVFlag=1;
                Os__saved_set(&DataSave0.ConstantParamData.prnflag,1,1);
                Os__saved_set(&DataSave0.ConstantParamData.prnflagS,0,1);
                Os__saved_set(&DataSave0.ConstantParamData.prnflagL,0,1);
                Os__saved_set(&DataSave0.ConstantParamData.linenum,0x16,1);
                Os__saved_set(&DataSave0.ConstantParamData.font,0x1E,1);
                Os__saved_set(&DataSave0.ConstantParamData.printtimes,2,1);             
                Os__saved_set(&Printer_flag,1,1);
                ucResult= XDATA_Write_Vaild_File(DataSaveConstant);
		  if(ucResult!=SUCCESS)
			  return ucResult;
                //Os__clr_display(255);
                //Os__GB2312_display(0,0,(unsigned char *)"ӡ:");
                //Os__GB2312_display(1,0,(unsigned char *)"Ϊ ");                     
                //Os__GB2312_display(2,0,(unsigned char *)"[1]ӡ1");                     
                //Os__GB2312_display(3,0,(unsigned char *)"[]ӡ2");                     
                ch=UTIL_GetKey(15);
		  if(ch=='1')
	  	  {
	                Os__saved_set(&DataSave0.ConstantParamData.printtimes,1,1);             
	                ucResult= XDATA_Write_Vaild_File(DataSaveConstant);
			  if(ucResult!=SUCCESS)
				  return ucResult;
		  }
                return SUCCESS;
            }else
                return ERR_CANCEL;//goto PRN;                   
        }
        else if(ch=='2') 
        {
            //Os__GB2312_display(3,6,(unsigned char *)"");
            ch=UTIL_GetKey(15);
            if(ch==KEY_ENTER) 
            {
				if(DataSave0.ConstantParamData.prnflag||DataSave0.ConstantParamData.prnflagL)
					RunData.ucWVFlag=1;
                Os__saved_set(&DataSave0.ConstantParamData.prnflag,0,1);
                Os__saved_set(&DataSave0.ConstantParamData.prnflagS,0,1);
                Os__saved_set(&DataSave0.ConstantParamData.prnflagL,0,1);                
                Os__saved_set(&DataSave0.ConstantParamData.linenum,0x01,1);
                Os__saved_set(&DataSave0.ConstantParamData.font,0,1);
                Os__saved_set(&DataSave0.ConstantParamData.printtimes,1,1);
                Os__saved_set(&Printer_flag,1,1);
                ucResult=XDATA_Write_Vaild_File(DataSaveConstant);
		  if(ucResult!=SUCCESS)
			  return ucResult;
                //Os__clr_display(255);
                //Os__GB2312_display(0,0,(unsigned char *)"ӡ:");
                //Os__GB2312_display(1,0,(unsigned char *)"Ϊ ");                         
                ch=UTIL_GetKey(15);                                     
                return SUCCESS;
            }
            else   
            	return ERR_CANCEL;//goto PRN;                       
        }
        else if(ch=='3') 
        {
        	//Os__GB2312_display(3,6,(unsigned char *)"״");
        	ch=UTIL_GetKey(15);
  
            if(ch==KEY_ENTER) 
            {
				if(DataSave0.ConstantParamData.prnflag||!DataSave0.ConstantParamData.prnflagL)
					RunData.ucWVFlag=1;
                Os__saved_set(&DataSave0.ConstantParamData.prnflag,0,1);	            	
                Os__saved_set(&DataSave0.ConstantParamData.prnflagS,0,1);
                Os__saved_set(&DataSave0.ConstantParamData.prnflagL,1,1);
                Os__saved_set(&DataSave0.ConstantParamData.linenum,0x08,1);
                Os__saved_set(&DataSave0.ConstantParamData.font,0,1);
                Os__saved_set(&DataSave0.ConstantParamData.printtimes,1,1);             
                Os__saved_set(&Printer_flag,1,1);
		ucResult=XDATA_Write_Vaild_File(DataSaveConstant);//chenzx add 2005-9-5 12:00
		if(ucResult!=SUCCESS)
			return ucResult;
                //Os__clr_display(255);
                //Os__GB2312_display(0,0,(unsigned char *)"ӡ:");
                //Os__GB2312_display(1,0,(unsigned char *)"Ϊ ״");                     
                ch=UTIL_GetKey(15);                                     
                return SUCCESS;
            }else
                return ERR_CANCEL;//goto PRN;                     
        }        
        else if(ch==KEY_CLEAR)  
        	return ERR_CANCEL;
        else 
        	goto PRN;
}



unsigned char UTIL_IncreaseTraceNumber(unsigned char aucMode)
{
    ULONG_C51 ulTraceNumber;
    switch(aucMode)
    {
        case 0:
            ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
            ulTraceNumber ++;
            if( ulTraceNumber > 999999 )
            {
                ulTraceNumber = 1;
            }
            break;
    /*ڽľֽظ,Կǳָôʱ,ˮŷŴ*/
        case 1: 
            ulTraceNumber = DataSave0.ChangeParamData.ulTraceNumber;
            ulTraceNumber += 50;
            if( ulTraceNumber > 999999 )
            {
                ulTraceNumber = 1;
            }
            break;
    /*ˮΪ1ٳ*/
        case 2:
            ulTraceNumber=1;
            break;
        default:
            break;
    }               
    Os__saved_copy( (unsigned char *)&ulTraceNumber,
            (unsigned char *)&DataSave0.ChangeParamData.ulTraceNumber,
            sizeof(ULONG_C51));
    return XDATA_Write_Vaild_File(DataSaveChange);
}


void UTIL_Form_Montant(unsigned char *Mnt_Fmt,unsigned long Montant,unsigned char Pos_Virgule)
{
    uchar   i;
    uchar   j;
	
    long_str(Mnt_Fmt,10,&Montant);
    j = Pos_Virgule;
    for (i=9; j ; i--, j--)
        Mnt_Fmt[i+1] = Mnt_Fmt[i];
    /* end modify */

    
    if (Pos_Virgule)
    {
        Mnt_Fmt[i+1] ='.';
        Mnt_Fmt[i+1+Pos_Virgule+1] = 0;
    }
    for(j=0 ; ((j < i) && (Mnt_Fmt[j]=='0')) ; Mnt_Fmt[j++]=' ');
}




unsigned char UTIL_InputAmount(unsigned char ucLine,
				unsigned long *pulAmount,
				unsigned long ulMinAmount, unsigned long ulMaxAmount)
{
	unsigned char	aucDisp[CFG_MAXDISPCHAR+1];//16		/* Buffer for Display */
	unsigned char	aucInput[CFG_MAXAMOUNTLEN+1];//10	/* Buffer for Key Input */
	unsigned char	ucKey;
	unsigned char	ucRedraw;
	unsigned char	ucCount;
	unsigned char	ucI;
	unsigned char	ucJ;
	int i;
	DRV_OUT *pdKey;
	unsigned int	uiTimeout;
	unsigned long	ulAmount;
	memset(&aucDisp[0],0,sizeof(aucDisp));
	memset(&aucDisp[0],' ',CFG_MAXDISPCHAR);
	aucDisp[CFG_MAXDISPCHAR-1] = '0';
	aucDisp[CFG_MAXDISPCHAR-2] = '0';
	aucDisp[CFG_MAXDISPCHAR-3] = '.';
	aucDisp[CFG_MAXDISPCHAR-4] = '0';
	memset(&aucInput[0],0,sizeof(aucInput));
	ucRedraw = TRUE;
	uiTimeout = 50*ONESECOND;
	ucCount = DataSave0.ConstantParamData.ucAmoutInput;
	Os__timer_start(&uiTimeout);
	do
	{
//add by wanHZia
		if( ucRedraw == TRUE)
		{
			//Os__clr_display(ucLine);
			Os__display(ucLine,0,&aucDisp[0]);
			ucRedraw = FALSE;
		}		
		pdKey = Os__get_key();
		
		do{			
		  }while(  (pdKey->gen_status==DRV_RUNNING) &&(uiTimeout !=0)  );

		if (uiTimeout == 0)
		{
//
			Os__abort_drv(drv_mmi);
			Os__timer_stop(&uiTimeout);
			return(ERR_CANCEL);
		}else
		{
			ucKey = pdKey->xxdata[1];
		}
		Os__abort_drv(drv_mmi);
//
		switch(ucKey)
		{
			case KEY_CLEAR:
Uart_Printf("ucCount=%d\n",ucCount);
					if( ucCount > DataSave0.ConstantParamData.ucAmoutInput)
					{
						memset(&aucInput[0],'0',2);
						ucCount = DataSave0.ConstantParamData.ucAmoutInput;
						memset(&aucDisp[0],0,sizeof(aucDisp));
						memset(&aucDisp[0],' ',CFG_MAXDISPCHAR);
						aucDisp[CFG_MAXDISPCHAR-1] = '0';
						aucDisp[CFG_MAXDISPCHAR-2] = '0';
						aucDisp[CFG_MAXDISPCHAR-3] = '.';
						aucDisp[CFG_MAXDISPCHAR-4] = '0';
						ucRedraw = TRUE;
					}else
					{
						Os__timer_stop(&uiTimeout);
						return(ERR_CANCEL);
					}
					break;				
			case KEY_BCKSP:
					if( ucCount > DataSave0.ConstantParamData.ucAmoutInput)
					{
						ucCount --;
						aucInput[ucCount-DataSave0.ConstantParamData.ucAmoutInput] = '0';
						ucRedraw = TRUE;
					}
					break;					
			case KEY_ENTER:
					ulAmount = asc_long(aucInput,ucCount);
					//if(ulAmount == 0)
					//	break;
					if( ulAmount < ulMinAmount )
					{
						Os__beep();
					}else
					{
						if( ulAmount > ulMaxAmount )
						{
Uart_Printf("UTIL_InputAmount if( ulAmount =%d> ulMaxAmount=%d )\n",ulAmount,ulMaxAmount);
							*pulAmount = ulAmount;
							Os__beep();	
							Os__timer_stop(&uiTimeout);
							return(ERR_VOIDDEPOSITAMOUNT);
						}else
						{
							*pulAmount = ulAmount;
							Os__timer_stop(&uiTimeout);
							return(SUCCESS);
						}
					}
					break;
		        case KEY_00_PT:
						ulAmount = asc_long(aucInput,ucCount);
						if(ulAmount == 0)
							break;
				            if( ucCount > (CFG_MAXAMOUNTLEN-2))
				            {
				                Os__beep();
				                break;
				            }
				            memcpy(&aucInput[ucCount],"00",2);
				            ucCount = ucCount +2;
				            ucRedraw = TRUE;
				            break;
			default :
					if(  (ucKey <= '9')  &&(ucKey >= '0') )
					{
						ulAmount = asc_long(aucInput,ucCount);
						if((ulAmount == 0)&&(ucKey == '0') )
							break;
						if( ucCount < CFG_MAXAMOUNTLEN)		//10
						{
							aucInput[ucCount-DataSave0.ConstantParamData.ucAmoutInput] = ucKey;
							if(DataSave0.ConstantParamData.ucAmoutInput==1)
							{
								aucInput[ucCount]='0';
							}
							if(DataSave0.ConstantParamData.ucAmoutInput==2)
							{
								aucInput[ucCount-1]='0';
								aucInput[ucCount]='0';
							}
							ucCount ++;
							ucRedraw = TRUE;
						}
					}
					Uart_Printf("aucinput:\n");
					for(i=0;i<16;i++)
						Uart_Printf("%02x ",aucInput[i]);
					Uart_Printf("\n");
					break;
		}
		/* Copy data from Input buffer to Display buffer */
		memset(&aucDisp[0],0,sizeof(aucDisp));
		memset(&aucDisp[0],' ',CFG_MAXDISPCHAR); 		//16
		aucDisp[CFG_MAXDISPCHAR-1] = '0';
		aucDisp[CFG_MAXDISPCHAR-2] = '0';
		aucDisp[CFG_MAXDISPCHAR-3] = '.';
		aucDisp[CFG_MAXDISPCHAR-4] = '0';			
		if( ucCount )
		{
			for(ucI=0,ucJ=0;ucI<ucCount;ucI++,ucJ++)
			{
				if( ucJ == 2)
				{
					ucJ ++;
				}
				Uart_Printf("ucCount=%d,ucI=%d,ucJ=%d\n",ucCount,ucI,ucJ);
				Uart_Printf("aucInput=%02x\n",aucInput[ucCount-ucI-1]);
				aucDisp[CFG_MAXDISPCHAR-ucJ-1] = aucInput[ucCount-ucI-1];
				Uart_Printf("aucDisp=%02x\n",aucDisp[15-ucJ]);
			}
		}
		Uart_Printf("aucDisp\n");
		for(i=0;i<16;i++)
			Uart_Printf("%02x ",aucDisp[i]);
		Uart_Printf("\n");
	}while(1);
}


void UTIL_Print_PRJ_VER(void)
{
	  unsigned char  aucPrintBuf[25];
	  unsigned char  i;
	  i = sizeof(CURRENT_PRJ);
	  memset(aucPrintBuf,' ',sizeof(aucPrintBuf));
	  aucPrintBuf[24]='\0';
	  memcpy(aucPrintBuf,CURRENT_PRJ,sizeof(CURRENT_PRJ)-1);
	  memcpy(&aucPrintBuf[i],CURRENT_VER,sizeof(CURRENT_VER));
	  Os__xprint((unsigned char *)aucPrintBuf);
}


unsigned char OSUTIL_CheckIPAddrField(
                unsigned char *pucIP,
                unsigned char ucIPLen,
                unsigned char *pucIPField)
{
    unsigned short uiIPField;

    if(ucIPLen > 4 )
    {
        return(ERR_CANCEL);    //return(OSERR_BADIP);  cj
    }
    if( strlen((char *)pucIP) > ucIPLen )
    {
        return(ERR_CANCEL);    //return(OSERR_BADIP);  cj
    }
    uiIPField = CONV_StrLong(pucIP);
    if( uiIPField > 255 )
    {
        return(ERR_CANCEL);    //return(OSERR_BADIP);  cj
    }
    *pucIPField = uiIPField;

    return(SUCCESS);    //return(OSERR_SUCCESS);   cj
}





unsigned char OSUTIL_InputIPV4Addr(
                unsigned int uiTimeout,
                unsigned char ucFont,
                unsigned char ucRow,
                unsigned char *pucIPV4Addr)
{
    unsigned char ucResult;
    unsigned char aucDisp[20];
    unsigned char aucIPV4Addr[4];
    unsigned char ucIPV4AddrLen;
    unsigned char aucBuf[5];
    unsigned char ucBufLen;
    unsigned char ucCursorRow;
    unsigned char ucCursorCol;
    unsigned char ucRedraw;



    DRV_OUT *pdKey;   //DRVOUT *pdKey;       cj
    unsigned char ucKey;
    unsigned char ucEnd;

    ucResult = SUCCESS;       //OSERR_SUCCESS;    cj
    memset(aucDisp,0,sizeof(aucDisp));
    memset(aucIPV4Addr,0,sizeof(aucIPV4Addr));
    ucIPV4AddrLen = 0;
    memset(aucBuf,0,sizeof(aucBuf));
    ucBufLen = 0;
    Uart_Printf("\n=====aucIPV4Addr[0]======\n",&aucIPV4Addr);
    if( uiTimeout )
    {
        Os__timer_start(&uiTimeout );  //OSTIMER_Start(&uiTimeout);  cj
    }else
    {
        uiTimeout = 1;
    }
    ucRedraw = FALSE;
    ucEnd = FALSE;
    if( pucIPV4Addr )
    {
        memcpy(aucIPV4Addr,pucIPV4Addr,4);
        ucRedraw = TRUE;

    }
    ucResult = SUCCESS;     //OSERR_SUCCESS;   cj
    do
    {
        if( ucRedraw == TRUE)
        {
            memset(&aucDisp[0],0,sizeof(aucDisp));
            sprintf((char *)aucDisp,"%3d.%3d.%3d.%3d",
                aucIPV4Addr[0],aucIPV4Addr[1],aucIPV4Addr[2],aucIPV4Addr[3]);
               Os__display(1,0,&aucDisp[0]);    //OSMMI_DisplayASCII(ucFont,ucRow,0,&aucDisp[0]);  //// cj
            if( ucIPV4AddrLen < 4 )
            {
                ucCursorCol = (ucIPV4AddrLen*4+2)*6;
            }else
            {
                ucCursorCol = (3*4+2)*6;
            }
            ucCursorRow = ucRow*8+7;
     // Uart_Printf("=====chen00======\n");
         //   HALLCD_Display8DotInRow(0x00,ucCursorRow,ucCursorCol,0xFC);     //cj
     // Uart_Printf("=====chen01======\n");
            ucRedraw = FALSE;
        }
        pdKey = Os__get_key();       // OSMMI_GetKey();      cj

        do{
        }while(  (pdKey->gen_status == DRV_RUNNING)
               &&(uiTimeout)
               );

        if (uiTimeout == 0)
        {
            Os__abort_drv(drv_mmi);  //OSDRV_Abort(HALDRVID_MMI);
            ucResult = ERR_CANCEL;             //OSERR_CANCEL;   cj
            break;
        }else
        {
            ucKey = pdKey->xxdata[1];
        }
        Uart_Printf("\n ucKey %02x",ucKey);
        switch(ucKey)
        {
        case KEY_CLEAR:
            if( ucIPV4AddrLen )
            {
                ucIPV4AddrLen = 0;
                memset(aucIPV4Addr,0,sizeof(aucIPV4Addr));
                memset(aucBuf,0,sizeof(aucBuf));
                ucBufLen = 0;
                ucRedraw = TRUE;
            }else
            {
                ucEnd = TRUE;
                ucResult = ERR_CANCEL;   //OSERR_CANCEL;   cj
            }
            break;
        case KEY_00_PT:
            if( ucIPV4AddrLen < 4)
            {
                if( !OSUTIL_CheckIPAddrField(aucBuf,ucBufLen,&aucIPV4Addr[ucIPV4AddrLen]) )
                {
                    ucIPV4AddrLen ++;
                    if( ucIPV4AddrLen >= 4 )
                    {
                        ucIPV4AddrLen = 3;
                    }
                    ucBufLen = 0;
                    memset(aucBuf,0,sizeof(aucBuf));
                    ucRedraw = TRUE;
                }
            }else
            {
                Os__beep();
            }
            break;
        case KEY_BCKSP:
            if( ucBufLen )
            {
                ucBufLen --;
                aucBuf[ucBufLen] = 0;
                if( !OSUTIL_CheckIPAddrField(aucBuf,ucBufLen,&aucIPV4Addr[ucIPV4AddrLen]) )
                {
                    ucRedraw = TRUE;
                }
            }else
            {
                if( ucIPV4AddrLen )
                {
                    aucIPV4Addr[ucIPV4AddrLen] = 0;
                    ucIPV4AddrLen --;
                    sprintf((char *)aucBuf,"%d",aucIPV4Addr[ucIPV4AddrLen]);
                    ucBufLen = strlen((char *)aucBuf);
                    ucRedraw = TRUE;
                }else
                {
                    aucIPV4Addr[ucIPV4AddrLen] = 0;
                    Os__beep();
                }
            }
            break;
        case KEY_ENTER:
            ucEnd = TRUE;
            ucResult=KEY_ENTER;
            break;
        default :
            if(  (ucKey >= '0')
               &&(ucKey <= '9')
              )
            {
                aucBuf[ucBufLen++] = ucKey;
                if( ucIPV4AddrLen < 4 )
                {
                    if( !OSUTIL_CheckIPAddrField(aucBuf,ucBufLen,&aucIPV4Addr[ucIPV4AddrLen]) )
                    {
                        ucRedraw = TRUE;
                    }
                }else
                {
                    OSMMI_Beep();
                }
                ucRedraw = TRUE;
                if( ucBufLen >= 3 )
                {
                    memset(aucBuf,0,sizeof(aucBuf));
                    ucBufLen = 0;
                    if( ucIPV4AddrLen < 3 )
                    {
                        ucIPV4AddrLen ++;
                    }
                }
            }
            break;
        }
        if( ucEnd == TRUE)
            break;
    }while(1);


    if( ucResult==KEY_ENTER )
    {
        if( pucIPV4Addr )
        {

            memcpy(pucIPV4Addr,aucIPV4Addr,4);
Uart_Printf("----2----aucIPV4Addr=\n",&aucIPV4Addr);
            Uart_Printf("ucResult1 %02x",ucResult);
        }
    }
    return(ucResult);
}

void UTIL_GetDateTime(void)
{
/*	unsigned char aucBuf[10];
	
	memset(aucBuf,0,sizeof(aucBuf));
	Os__read_time_sec(aucBuf);//hhmmss
	asc_bcd(RUNDATA_aucTime,3,aucBuf,6);
	RUNDATA_aucDate[0]=0x20;
	
	memset(aucBuf,0,sizeof(aucBuf));
	Os__read_date(aucBuf);	//\DDMMYY;
	asc_bcd(&RUNDATA_aucDate[1],1,&aucBuf[4],2);
	asc_bcd(&RUNDATA_aucDate[2],1,&aucBuf[2],2);
	asc_bcd(&RUNDATA_aucDate[3],1,&aucBuf[0],2);	
	*/
}

unsigned char UTIL_SetPinpad(void)
{
    unsigned char ch,ucResult=SUCCESS;
MIMA:   //Os__clr_display(255);
        //Os__GB2312_display(0,0,(unsigned char *)"ǰ");
        //Os__GB2312_display(1,0,(unsigned char *)"1. 2.");
        //Os__GB2312_display(2,0,(unsigned char *)"Ϊ:");
       // if (DataSave0.ConstantParamData.Pinpadflag)
            //Os__GB2312_display(0,6,(unsigned char *)"");
       // else
            //Os__GB2312_display(0,6,(unsigned char *)"");              
        ch=UTIL_GetKey(15);
        if(ch=='1') 
        {
            //Os__GB2312_display(2,6,(unsigned char *)"");
            ch=UTIL_GetKey(15);
                if(ch==KEY_ENTER) 
            {
                Os__saved_set(&DataSave0.ConstantParamData.Pinpadflag,1,1);
                Os__saved_set(&Pinpad_flag,1,1);
                ucResult=XDATA_Write_Vaild_File(DataSaveConstant);
		  if(ucResult!=SUCCESS)
			  return ucResult;
                //Os__clr_display(255);
                //Os__GB2312_display(0,0,(unsigned char *)"");
                //Os__GB2312_display(1,0,(unsigned char *)"Ϊ:   ");  
                //Os__GB2312_display(2,0,(unsigned char *)"PINPAD");  
                //Os__clr_display_pp(255);
                //Os__GB2312_display_pp(0,0,( unsigned char *)     "ѡ");
                Os__display_pp(0,6,(unsigned char *)"PINPAD");
                //Os__GB2312_display_pp(1,0,( unsigned char *) " 밴ȷ");
                if(UTIL_GetKey_pp(20) != 99)
                {
                    //Os__clr_display(255);
                    //Os__GB2312_display(0,0,( unsigned char *) "PINPAD ȷ");   
                    ch = UTIL_GetKey(10);                   
                    return SUCCESS;
                }else   
                {
                    //Os__clr_display(255);
                    //Os__GB2312_display(0,0,( unsigned char *) "PINPAD Դ");   
                    ch = UTIL_GetKey(15);                   
                    return SUCCESS;
                }
               }else   return ERR_CANCEL;// goto MIMA;                  
        }
        else if(ch=='2') 
            {
                //Os__GB2312_display(2,6,(unsigned char *)"");
                    ch=UTIL_GetKey(15);
                    if(ch==KEY_ENTER) 
                {
                    Os__saved_set(&DataSave0.ConstantParamData.Pinpadflag,0,1);
                    Os__saved_set(&Pinpad_flag,1,1);
                    ucResult=XDATA_Write_Vaild_File(DataSaveConstant);
       		  if(ucResult!=SUCCESS)
       			  return ucResult;
                    //Os__clr_display(255);
                    //Os__GB2312_display(0,0,(unsigned char *)"");
                    //Os__GB2312_display(1,0,(unsigned char *)"Ϊ:   ");                            
                    ch=UTIL_GetKey(15);  
                    return SUCCESS;
                }else   return ERR_CANCEL;//goto MIMA;                      
                }else if(ch==KEY_CLEAR)  return ERR_CANCEL;
                    else  goto MIMA;    
Uart_Printf("CFG_ConstantParamPinpad of pinpadflag=%d\n",DataSave0.ConstantParamData.Pinpadflag);
}





unsigned char UTIL_CalcMemberMac(
				unsigned char *pucInData,
				unsigned short uiInLen,
				unsigned char *pucOutMAC)
{
	unsigned char aucHexData[8],ucBackup;
	unsigned char *pucPtr,ucResult=SUCCESS;
	unsigned short uiI,uiJ,uiTTLen;
	unsigned short uiLen;
	Uart_Printf("\nEnter SMC_UTIL_CalcMemberMac");
	
	uiTTLen=uiInLen;
	pucPtr = pucInData;	
	//ڴıռѾ8Բ
	//Ժ޸עⲻҪ

	//ݺӦ0x80ǵһֽ
	ucBackup=*(pucPtr+uiInLen);
	//memcpy(pucPtr+uiInLen,"\x80",1);
	//uiTTLen +=1;
	
#if  0			
	memset(aucHexData,0,sizeof(aucHexData));
	uiLen = 8;
	for(uiJ=0;uiJ<uiLen;uiJ++)
	{
		aucHexData[uiJ] ^= *(pucPtr+uiJ);
	}
	pucPtr += 8;
	
	for(uiI=8;uiI<uiTTLen;uiI += 8)
	{
		uiLen = min(8,pucInData+uiTTLen-pucPtr);
		for(uiJ=0;uiJ<uiLen;uiJ++)
		{
			aucHexData[uiJ] ^= *(pucPtr+uiJ);
		}
		
		ucResult=PINPAD_47_Encrypt8ByteSingleKey(KEYARRAY_WKCARD1,
											KEYINDEX_WKCARD1_MACKEY,
											aucHexData,
											aucHexData);
		if( ucResult != SUCCESS)
        	return(ucResult);
		pucPtr += uiLen;
	}
#else
       memset(aucHexData,0,sizeof(aucHexData));
	uiLen = 8;
	for(uiJ=0;uiJ<uiLen;uiJ++)
	{
		aucHexData[uiJ] ^= *(pucPtr+uiJ);
	}
	pucPtr += 8;
	
	for(uiI=8;uiI<uiTTLen;uiI += 8)
	{
		uiLen = min(8,pucInData+uiTTLen-pucPtr);
		for(uiJ=0;uiJ<uiLen;uiJ++)
		{
			aucHexData[uiJ] ^= *(pucPtr+uiJ);
		}

{
int i;
Uart_Printf("\n**********Ľ**********\n");

for(i=0;i<8;i++)
{
Uart_Printf("%02x ",aucHexData[i]);
}
Uart_Printf("\n");
}
		pucPtr += uiLen;
	}
{
int i;
Uart_Printf("\n**********Ľ**********\n");
for(i=0;i<8;i++)
Uart_Printf("%02x ",aucHexData[i]);
Uart_Printf("\n");
}

/*	ucResult=PINPAD_47_Encrypt8ByteSingleKey(KEYARRAY_MACKEYZHU,
											KEYINDEX_WKCARD1_MACKEY,
											aucHexData,
											aucHexData);
*/
	des(aucHexData,aucHexData,DataSave0.ConstantParamData.MacKey);
{
int i;
Uart_Printf("\n**********Ľ**********\n");
for(i=0;i<8;i++)
Uart_Printf("%02x ",aucHexData[i]);
Uart_Printf("\n");
}

	if( ucResult != SUCCESS)
        	return(ucResult);

#endif

	memcpy(pucInData+uiInLen,&ucBackup,1);
	memcpy(pucOutMAC,aucHexData,8);

	return(SUCCESS);
}


unsigned char UTIL_CalcSandLoadTAC(unsigned char *pucInData,
									         unsigned short  uiInLen,
									         unsigned char  *pucOutData)
{
	unsigned char ucResult;
	unsigned char aucData[8];
	unsigned char aucUseKey[8];
	unsigned char *pucPtr;
	unsigned short uiI,uiJ,uiLen;
	unsigned char ucI;
	
	pucPtr = pucInData;
	memset(aucData,0,sizeof(aucData));
	for(uiI=0;uiI<uiInLen;uiI += 8)
	{
		uiLen=min(8,pucInData+uiInLen-pucPtr);
		for(uiJ=0;uiJ<uiLen;uiJ++)
			aucData[uiJ] ^= *(pucPtr+uiJ);
		pucPtr +=8;


	}

	memset(aucUseKey,0,sizeof(aucUseKey));
	memcpy(aucUseKey,RunData.aucLoadKey,6);
	des(aucData,aucData,aucUseKey);


	//ֻȡǰ4λ
    memcpy(pucOutData,aucData,4);
    return(SUCCESS);       
}



unsigned char UTIL_CalcSandTAC(unsigned char *pucInData,
									         unsigned short  uiInLen,
									         unsigned char  *pucOutData)
{
	unsigned char ucResult,ucPKeyIndex,ucKeyArray;
	unsigned char aucData[8];
	unsigned char aucUseKey[8];
	unsigned char *pucPtr;
	unsigned short uiI,uiJ,uiLen;
	unsigned char ucI;
	
	pucPtr = pucInData;
	memset(aucData,0,sizeof(aucData));
	for(uiI=0;uiI<uiInLen;uiI += 8)
	{
		uiLen=min(8,pucInData+uiInLen-pucPtr);
		for(uiJ=0;uiJ<uiLen;uiJ++)
			aucData[uiJ] ^= *(pucPtr+uiJ);
		pucPtr +=8;


	}
	if(NormalTransData.ucNewOrOld==0)
	{
		switch(NormalTransData.ucPtCode)
		{
			case 0x01:ucKeyArray=KEYARRAY_PURSECARD2;
				ucPKeyIndex=KEYINDEX_WKCARD1_PURCHASEKEY;
				break;
			case 0x02:ucKeyArray=KEYARRAY_PURSECARD3;
				ucPKeyIndex=KEYINDEX_PURSECARD3_AMOUNTCHECK;
				break;
			default:break;
		}
	}
	else
	{
		switch(NormalTransData.ucPtCode)
		{
			case 0x01:
				if(RunData.ucKeyArrey)
					ucKeyArray=KEYARRAY_PURSECARD5+RunData.ucKeyArrey;
				else
					ucKeyArray=KEYARRAY_PURSECARD1;
				break;
			case 0x02:ucKeyArray=KEYARRAY_PURSECARD4;break;
			case 0x03:ucKeyArray=KEYARRAY_PURSECARD5+RunData.ucKeyArrey;break;
			default:break;
		}
		
		ucPKeyIndex=KEYINDEX_PURSECARD1_PURCHASEKEY;
	}

	ucResult = PINPAD_47_Encrypt8ByteSingleKey(
	        ucKeyArray,
	        ucPKeyIndex,   
	        NormalTransData.aucUserCardNo,
	        aucUseKey);

	if(ucResult!=SUCCESS)
		return ucResult;
     
	ucResult = PINPAD_47_Crypt8ByteSingleKey(
	        ucKeyArray,
	        ucPKeyIndex+1,   
	        aucUseKey,
	        aucUseKey);
	if(ucResult!=SUCCESS)
		return ucResult;
			
	ucResult = PINPAD_47_Encrypt8ByteSingleKey(
	        ucKeyArray,
	        ucPKeyIndex,   
	        aucUseKey,
	       aucUseKey);
	if(ucResult!=SUCCESS)
		return ucResult;
	memset(&aucUseKey[6],0,2);
	des(aucData,aucData,aucUseKey);


	//ֻȡǰ4λ
    memcpy(pucOutData,aucData,4);
    return(SUCCESS);       
}


unsigned char UTIL_GetKey_pp(unsigned char wait_sec)
{  
    unsigned int Timeout;
    DRV_OUT *KBoard;

    Timeout=wait_sec*100;
    Os__timer_start(&Timeout);
    Os__xdelay(1);   
        KBoard=Os__get_key_pp();
        while ((Timeout!=0)&&(KBoard->gen_status==DRV_RUNNING));
        Os__timer_stop(&Timeout); 
        if(Timeout==0)
        { 
            Os__abort_drv(drv_mmi);
        Os__xdelay(10);
            return  99;
        }
        return KBoard->xxdata[1];
}



unsigned char UTIL_DisplayTrans(void)
{
	unsigned char	aucBuf[30];
	
	memset(aucBuf,0,sizeof(aucBuf));
	bcd_asc(aucBuf,NormalTransData.aucUserCardNo,16);
	aucBuf[16]=0x00;
	//Os__clr_display(255);
    //Os__GB2312_display(0,0,(uchar *)"卡号: ");
    //Os__display(0,6,aucBuf);
	if(NormalTransData.ucKYFlag)
	{
        memcpy(&aucBuf[16],(uchar *)"[可疑交易]",10);
	}
	//Os__GB2312_display(1,0,&aucBuf[10]);
	
	memset(aucBuf,' ',sizeof(aucBuf));
	aucBuf[16]=0x00;
    switch(NORMALTRANS_ucTransType)
	{
	case TRANS_S_PURCHASE:
		memcpy(aucBuf,(unsigned char*)"",4);
		break;
	case TRANS_S_RETURNCARD:
		memcpy(aucBuf,(unsigned char*)"",8);
		break;
	case TRANS_S_ONLINEPURCHASE:
		if(NormalTransData.ucReturnflag==true)
			memcpy(aucBuf,(unsigned char*)"",8);
		else
			memcpy(aucBuf,(unsigned char*)"",8);
		break;
	case TRANS_S_PURCHASE_P:
		memcpy(aucBuf,(unsigned char*)"",8);
		break;
	case TRANS_S_RETURNCARD_P:
		memcpy(aucBuf,(unsigned char*)"",8);
		break;
	case TRANS_S_ONLINEPURCHASE_P:
		memcpy(aucBuf,(unsigned char*)"",8);
		break;
	case TRANS_S_REFUND:
		memcpy(aucBuf,(unsigned char*)"˻",4);
		break;
	case TRANS_S_VOID:
		memcpy(aucBuf,(unsigned char*)"",4);
		break;
	case TRANS_S_EDLOAD:
        memcpy(aucBuf,(unsigned char*)"ճֵ",8);
		break;
	case TRANS_S_RESETCARD:
		memcpy(aucBuf,(unsigned char*)"",6);
		break;
	case TRANS_S_PRELOAD: 
		memcpy(aucBuf,(unsigned char*)"Ԥֵ",6);
		break;
	case TRANS_S_SALE: 
		memcpy(aucBuf,(unsigned char*)"",6);
		break;	
	case TRANS_S_SETTLE_ONE:
		memcpy(aucBuf,(unsigned char*)"",4);
		break;
	case TRANS_S_CHANGEEXP: 
		memcpy(aucBuf,(unsigned char*)"",4);
		break;
	case TRANS_S_PAY:
		memcpy(aucBuf,(unsigned char*)"ɷ",4);
		break;
	case TRANS_S_PAYONLINE: 
		memcpy(aucBuf,(unsigned char*)"ɷ",8);
		break;
	case TRANS_S_MOBILECHARGE: 
		memcpy(aucBuf,(unsigned char*)"ѻۿ",8);
		break;	
	case TRANS_S_MOBILECHARGEONLINE:
		memcpy(aucBuf,(unsigned char*)"ۿ",8);
		break;
	case TRANS_S_BARCODE: 
		memcpy(aucBuf,(unsigned char*)"",4);
		break;
	case TRANS_S_PREAUTH: 
		memcpy(aucBuf,(unsigned char*)"ԤȨ",6);
		break;
	case TRANS_S_PREAUTHVOID: 
		memcpy(aucBuf,(unsigned char*)"Ȩ",8);
		break;	
	case TRANS_S_PREAUTHFINISH:
		memcpy(aucBuf,(unsigned char*)"Ȩ",8);
		break;
	case TRANS_S_PREAUTHFINISHVOID: 
		memcpy(aucBuf,(unsigned char*)"ɳ",8);
		break;
	case TRANS_S_VOIDPAYONLINE: 
		memcpy(aucBuf,(unsigned char*)"ɷѳ",8);
		break;	
	case TRANS_S_LOADONLINE: 
		memcpy(aucBuf,(unsigned char*)"ֵ",8);
		break;
	case TRANS_S_TRANSPURCHASE:
		memcpy(aucBuf,(unsigned char*)"ʿۿ",8);
		break;
	case TRANS_S_TRANSFER: 
		memcpy(aucBuf,(unsigned char*)"ʳֵ",8);
		break;
	case TRANS_S_EXPPURCHASE: 
		memcpy(aucBuf,(unsigned char*)"",8);
		break;
	case TRANS_S_REFAPL:
		memcpy(aucBuf,(unsigned char*)"˻",8);
		break;
	case TRANS_S_VOIDREFAPL:
		memcpy(aucBuf,(unsigned char*)"",8);
		break;
	case TRANS_S_CASHLOAD: 
		memcpy(aucBuf,(unsigned char*)"ֵֽ",8);
		break;
	case TRANS_S_ZSHPURCHASE: 
		memcpy(aucBuf,(unsigned char*)"Ϳۿ",8);
		break;
	case TRANS_S_PTCPURCHASE: 
		memcpy(aucBuf,(unsigned char*)"ۿ",8);
		break;
	default:
		break;
	}

	long_asc(&aucBuf[10],6,&NormalTransData.ulTraceNumber);
	//Os__GB2312_display(2,0,aucBuf);

    UTIL_DisplayAmount(3,NormalTransData.ulAmount);

	if(NORMALTRANS_ucTransType == TRANS_ADDEXPIRE)
	{
        memset(aucBuf,' ',sizeof(aucBuf));
        aucBuf[16]=0x00;
		memcpy(aucBuf,"Ч: ",8);
		bcd_asc(&aucBuf[8],&NormalTransData.aucNewExpiredDate[1],2);
		aucBuf[10]='/';
		bcd_asc(&aucBuf[11],&NormalTransData.aucNewExpiredDate[2],2);
		aucBuf[13]='/';
		bcd_asc(&aucBuf[14],&NormalTransData.aucNewExpiredDate[3],2);
		//Os__GB2312_display(3,0,aucBuf);
	}

	return SUCCESS;
}


void UTIL_DisplayAmount(unsigned char line,unsigned long amount)
{
    unsigned char aucDisplay[17];
    unsigned char aucBuf[17];
    unsigned long alAmount;
    
    memset(aucDisplay,0,sizeof(aucDisplay));
    memset(aucBuf,0,sizeof(aucBuf));
    alAmount = amount;
    UTIL_Form_Montant(aucBuf,alAmount,2);
    memcpy(aucDisplay,":",5);
    aucDisplay[16] = 0;

    //Os__GB2312_display(line,0,aucDisplay);
    Os__display(line,5,aucBuf);
    
    return;
}



unsigned char UTIL_InputEncryptPIN(void)
{
    unsigned char ucResult = SUCCESS;
    unsigned char aucPIN[10];
    unsigned char aucPINBuf[16];
    unsigned char ucInputPINLen;
	
    memset(aucPIN,0,sizeof(aucPIN));
    //Os__clr_display(255);
    //Os__GB2312_display(0,0,(unsigned char *)"ȴû");    
    //Os__GB2312_display(3,0,(unsigned char *)"밴ȷ");    
	//Os__clr_display_pp(255);
    //Os__GB2312_display_pp(0,0,(unsigned char *)"룺");
    
    ucResult = UTIL_Input(1,true,0,6,'P',aucPIN,NULL);    
    if(ucResult == KEY_ENTER)
    {
		ucResult = SUCCESS;
        ucInputPINLen = strlen((char *)aucPIN);
        if(ucInputPINLen != 0)
        {
            memset(aucPINBuf,0x20,sizeof(aucPINBuf));
            memcpy(&aucPINBuf[0],aucPIN,ucInputPINLen);
            memcpy(aucPIN,aucPINBuf,8);
        }
        else
        {
        	memset(aucPIN,0x20,sizeof(aucPIN));
        }
        /******************ʱʹܷʽfante**************/
        ucResult = PINPAD_47_Encrypt8ByteSingleKey(KEYARRAY_PURSECARD2,
                KEYINDEX_PURSECARD2_PINKEY,aucPIN,RunData.aucPin);
    }
    return(ucResult);
}




unsigned char UTIL_InputCashierPIN(void)
{
    unsigned char ucResult = SUCCESS;
    unsigned char aucPIN[10];
    unsigned char aucPINBuf[16];
    unsigned char ucInputPINLen;
	
    memset(aucPIN,0,sizeof(aucPIN));
    //Os__clr_display(255);
    //Os__GB2312_display(0,0,(unsigned char *)"룺");
    
    ucResult = UTIL_Input(1,true,0,6,'P',aucPIN,NULL);    
    if(ucResult == KEY_ENTER)
    {
    	 ucResult = SUCCESS;
        ucInputPINLen = strlen((char *)aucPIN);
        if(ucInputPINLen != 0)
        {
            memset(aucPINBuf,0x20,sizeof(aucPINBuf));
            memcpy(&aucPINBuf[0],aucPIN,ucInputPINLen);
            memcpy(aucPIN,aucPINBuf,8);
        }
        else
        {
        		memset(aucPIN,0x20,sizeof(aucPIN));
        }
        /******************ʱʹܷʽfante**************/
       des(aucPIN,aucPIN,DataSave0.ConstantParamData.MacMasterKey[0]);
       desm1(aucPIN,aucPIN,DataSave0.ConstantParamData.MacMasterKey[1]);
       des(aucPIN,RunData.aucPin,DataSave0.ConstantParamData.MacMasterKey[0]);
    }
    return(ucResult);
}

unsigned char UTIL_ChangeEncryptPIN(void)
{
	//const unsigned char aucInitPAN[17] = "0000000000000000";
		         int  i;
	unsigned char ucResult;
	unsigned char aucPIN[10];
	unsigned char aucPIN1[10];
	unsigned char aucPIN2[10];
	unsigned char aucPINBuf[16];
	unsigned char ucInputPINLen;//dudu reg
	
	
	
	memset(aucPIN,0,sizeof(aucPIN));
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"ȴû");
	//Os__clr_display_pp(255);
       //Os__GB2312_display_pp(0,0,(unsigned char *)"ԭ룺");

       ucResult = UTIL_Input_pp(1,true,0,6,'P',aucPIN);    
	
Uart_Printf("aucpin:%s\n",aucPIN);
	if(ucResult ==KEY_ENTER)
	{
		ucInputPINLen = strlen((char *)aucPIN);
		ucResult = SUCCESS;
		if(ucInputPINLen != 0)
		{
			memset(aucPINBuf,0x20,sizeof(aucPINBuf));
			//aucPINBuf[0] = '0';
			//aucPINBuf[1] = ucInputPINLen+'0';
			memcpy(&aucPINBuf[0],aucPIN,ucInputPINLen);
			memcpy(aucPIN,aucPINBuf,8);
			//asc_hex(aucPIN,8,aucPINBuf,16);
			
Uart_Printf("aucpin:%s\n",aucPIN);
Uart_Printf("aucPINBuf:%s\n",aucPINBuf);
		}
		else
		{
		       memset(aucPIN,0x20,sizeof(aucPIN));
		}
		/******************ʱʹܷʽfante**************/
	        ucResult = PINPAD_47_Encrypt8ByteSingleKey(KEYARRAY_PURSECARD2,
	                KEYINDEX_PURSECARD2_PINKEY,aucPIN,RunData.aucPin);
		
for(i=0;i<8;i++)
Uart_Printf("%02x ",RunData.aucPin[i]);
Uart_Printf("\n");
	}
	else
	{
	       return ucResult;
	}
	
	memset(aucPIN1,0,sizeof(aucPIN1));
	//Os__clr_display_pp(255);
       //Os__GB2312_display_pp(0,0,(unsigned char *)"룺");

       ucResult = UTIL_Input_pp(1,true,0,6,'P',aucPIN1);    
	
	if(ucResult ==KEY_ENTER)
	{
		memset(aucPIN2,0,sizeof(aucPIN2));
		//Os__clr_display_pp(255);
		//Os__GB2312_display_pp(0,0,(unsigned char *)"ٴ룺");

		ucResult = UTIL_Input_pp(1,true,0,6,'P',aucPIN2);    

		if(ucResult ==KEY_ENTER)
		{			  
			if(memcmp(aucPIN1,aucPIN2,8))
			{
				//Os__clr_display(255);
				//Os__GB2312_display(0,0,(unsigned char *)"2벻һ");
				//Os__clr_display_pp(255);
				//Os__GB2312_display_pp(0,0,(unsigned char *)"2벻һ");
				MSG_WaitKey(5);
				return ERR_CANCEL;

			}
			
			ucInputPINLen = strlen((char *)aucPIN1);
			ucResult = SUCCESS;
			if(ucInputPINLen != 0)
			{
				//memset(aucPINBuf,'F',sizeof(aucPINBuf));
				memset(aucPINBuf,0x20,sizeof(aucPINBuf));
				//aucPINBuf[0] = '0';
				//aucPINBuf[1] = ucInputPINLen+'0';
				memcpy(&aucPINBuf[0],aucPIN1,ucInputPINLen);
				memcpy(aucPIN1,aucPINBuf,8);
				//asc_hex(aucPIN1,8,aucPINBuf,16);
			}
			else
			{
			       memset(aucPIN1,0x20,sizeof(aucPIN1));
			}
			/******************ʱʹܷʽfante**************/
		        ucResult = PINPAD_47_Encrypt8ByteSingleKey(KEYARRAY_PURSECARD2,
		                KEYINDEX_PURSECARD2_PINKEY,aucPIN1,RunData.aucNewPin);
			
for(i=0;i<8;i++)
Uart_Printf("%02x ",RunData.aucPin[i]);
Uart_Printf("\n");
for(i=0;i<8;i++)
Uart_Printf("%02x ",RunData.aucNewPin[i]);
Uart_Printf("\n");

		}
	}
	
    return(ucResult);
}

unsigned char UTIL_ChangeCashierPIN(void)
{
	//const unsigned char aucInitPAN[17] = "0000000000000000";
		         int  i;
	unsigned char ucResult;
	unsigned char aucPIN[10];
	unsigned char aucPIN1[10];
	unsigned char aucPIN2[10];
	unsigned char aucPINBuf[16];
	unsigned char ucInputPINLen;//dudu reg
	
	
	
	memset(aucPIN,0,sizeof(aucPIN));
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"ԭ룺");

       ucResult = UTIL_Input(1,1,6,6,'P',aucPIN,NULL);    
	
Uart_Printf("aucpin:%s\n",aucPIN);
	if(ucResult ==KEY_ENTER)
	{
		ucInputPINLen = strlen((char *)aucPIN);
		ucResult = SUCCESS;
		if(ucInputPINLen != 0)
		{
			memset(aucPINBuf,0x20,sizeof(aucPINBuf));
			//aucPINBuf[0] = '0';
			//aucPINBuf[1] = ucInputPINLen+'0';
			memcpy(&aucPINBuf[0],aucPIN,ucInputPINLen);
			memcpy(aucPIN,aucPINBuf,8);
			//asc_hex(aucPIN,8,aucPINBuf,16);
			
Uart_Printf("aucpin:%s\n",aucPIN);
Uart_Printf("aucPINBuf:%s\n",aucPINBuf);
		}
		else
		{
		       memset(aucPIN,0x20,sizeof(aucPIN));
		}
		/******************ʱʹܷʽfante**************/
	       des(aucPIN,aucPIN,DataSave0.ConstantParamData.MacMasterKey[0]);
	       desm1(aucPIN,aucPIN,DataSave0.ConstantParamData.MacMasterKey[1]);
	       des(aucPIN,RunData.aucPin,DataSave0.ConstantParamData.MacMasterKey[0]);
		
for(i=0;i<8;i++)
Uart_Printf("%02x ",RunData.aucPin[i]);
Uart_Printf("\n");
	}
	else
	{
	       return ucResult;
	}
	
	memset(aucPIN1,0,sizeof(aucPIN1));
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"룺");

       ucResult = UTIL_Input(1,1,6,6,'P',aucPIN1,NULL);    
	
	if(ucResult ==KEY_ENTER)
	{
		memset(aucPIN2,0,sizeof(aucPIN2));
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(unsigned char *)"ٴ룺");

		ucResult = UTIL_Input(1,1,6,6,'P',aucPIN2,NULL);    

		if(ucResult ==KEY_ENTER)
		{			  
			if(memcmp(aucPIN1,aucPIN2,8))
			{
				//Os__clr_display(255);
				//Os__GB2312_display(0,0,(unsigned char *)"2벻һ");
				MSG_WaitKey(5);
				return ERR_CANCEL;

			}
			
			ucInputPINLen = strlen((char *)aucPIN1);
			ucResult = SUCCESS;
			if(ucInputPINLen != 0)
			{
				//memset(aucPINBuf,'F',sizeof(aucPINBuf));
				memset(aucPINBuf,0x20,sizeof(aucPINBuf));
				//aucPINBuf[0] = '0';
				//aucPINBuf[1] = ucInputPINLen+'0';
				memcpy(&aucPINBuf[0],aucPIN1,ucInputPINLen);
				memcpy(aucPIN1,aucPINBuf,8);
				//asc_hex(aucPIN1,8,aucPINBuf,16);
			}
			else
			{
			       memset(aucPIN1,0x20,sizeof(aucPIN1));
			}
			/******************ʱʹܷʽfante**************/
	       des(aucPIN1,aucPIN,DataSave0.ConstantParamData.MacMasterKey[0]);
	       desm1(aucPIN,aucPIN,DataSave0.ConstantParamData.MacMasterKey[1]);
	       des(aucPIN,RunData.aucNewPin,DataSave0.ConstantParamData.MacMasterKey[0]);
			
for(i=0;i<8;i++)
Uart_Printf("%02x ",RunData.aucPin[i]);
Uart_Printf("\n");
for(i=0;i<8;i++)
Uart_Printf("%02x ",RunData.aucNewPin[i]);
Uart_Printf("\n");

		}
	}
	
    return(ucResult);
}

unsigned char UITL_displayCardNum(void)
{
	unsigned char ucResult ;
	unsigned char tmpBuf[20];
	unsigned char dispBuf[17];
	unsigned short cardNumLen; 
	
	while(1)
	{
		//Os__clr_display(255);
		memset(tmpBuf,0x00,sizeof(tmpBuf));
	    bcd_asc(tmpBuf,NormalTransData.aucUserCardNo,16);
		memset(dispBuf,0x00,sizeof(dispBuf));
		memcpy(dispBuf,tmpBuf,16);
		Os__display(0,0,dispBuf);
		cardNumLen=strlen((char *)tmpBuf);
		if(cardNumLen>16)
		{
			memset(dispBuf,0x00,sizeof(dispBuf));
			memcpy(dispBuf,&tmpBuf[16],cardNumLen-16);
			Os__display(1,0,dispBuf);
		}
		UTIL_Form_Montant(tmpBuf,NORMALTRANS_ulAmount,2);
		//Os__GB2312_display(2,0,(unsigned char *)":");
		Os__display(2,5,tmpBuf);
	
		//Os__GB2312_display(3,4,(unsigned char *)"ȷ");
		switch(Os__xget_key())
		{
			case KEY_ENTER:
				ucResult = SUCCESS;
				break;
			case KEY_CLEAR:
				ucResult = ERR_CANCEL;
				break;
			default:
				continue;
		}
		break;
	}
	return ucResult;
}


void UTIL_TEST(void)
{
	   unsigned char aucBuf[392];
	   unsigned char aucBuf1[7601];
	   unsigned int uiTableNum;
	   int iFileHandle;
	int iFileSize;
	int iReadSize;
	int iWriteSize;
	int iI;
	int iRecordPerTime;
	int iMinNum;
	int iFileErrNo;
	unsigned char ucResult;
	
	iFileHandle = 0;
	iFileSize = 0;
	iReadSize = 0;
	iWriteSize = 0;
	
	iI = 0;
	iRecordPerTime = 0;
	
	iMinNum = 0;
	   
	   memset(aucBuf,0,sizeof(aucBuf));
	   aucBuf[0] = 0x10;
	   memcpy(&aucBuf[1],"000000000000010000000000000000000000000",39);
	   memcpy(&aucBuf[40],"000000000000020000000000000000000000000",39);
	   memcpy(&aucBuf[79],"000000000000030000000000000000000000000",39);
	   memcpy(&aucBuf[118],"000000000000040000000000000000000000000",39);
	   memcpy(&aucBuf[157],"000000000000050000000000000000000000000",39);
	   memcpy(&aucBuf[196],"666666000000060000000000000000000000000",39);
	   memcpy(&aucBuf[235],"777777000000070000000000000000000000001",39);
	   memcpy(&aucBuf[274],"000000000000080000000000000000000000000",39);
	   memcpy(&aucBuf[313],"000000000000090000000000000000000000001",39);
	   memcpy(&aucBuf[352],"000000000000100000000000000000000000000",39);
	   
	   
	   uiTableNum = 10;
	                     
	//   XDATA_AppendRecords(DataSaveCardTables,aucBuf,sizeof(CARDTABLE),uiTableNum);
	   
	   
	   iFileHandle = OSAPP_OpenFile((char *)FileInfo[DataSaveCardTables].aucFileName,O_READ);
	
	if( iFileHandle < 0 )
	{
		OSAPP_FileClose(iFileHandle);
		OSAPP_FileGetLastError();
		return;
	}
	
	 memset(aucBuf1,0,sizeof(aucBuf1));
	iFileSize = OSAPP_FileSize((char *)FileInfo[DataSaveCardTables].aucFileName);
	if( iFileSize >= 0 )
	{
	
		if( iFileSize > 0 )
		{
			if(OSAPP_FileSeek(iFileHandle,0,SEEK_SET)==0)
				iReadSize = OSAPP_FileRead(iFileHandle,aucBuf1,iFileSize);
		}
	}
	Uart_Printf("\niReadSize=%d",iReadSize);
	Uart_Printf("\n");
	for(iI=0;iI<iReadSize;iI++)
	{
		 Uart_Printf("%02x  ",aucBuf1[iI]);
		 if(iI%20 == 0)
		 	Uart_Printf("\n");
	}
	
}



void UTIL_TEST1(void)
{
	   unsigned char aucBuf[392];
	   unsigned char aucBuf1[7601];
	   unsigned int uiTableNum;
	   int iFileHandle;
	int iFileSize;
	int iReadSize;
	int iWriteSize;
	int iI;
	int iRecordPerTime;
	int iMinNum;
	int iFileErrNo;
	unsigned char ucResult;
	
	iFileHandle = 0;
	iFileSize = 0;
	iReadSize = 0;
	iWriteSize = 0;
	
	iI = 0;
	iRecordPerTime = 0;
	
	iMinNum = 0;
	   
	   memset(aucBuf,0,sizeof(aucBuf));
	   aucBuf[0] = 0x10;
	   memcpy(&aucBuf[1],"00000000000001000000000000000000000001",38);
	   memcpy(&aucBuf[39],"00100000000002000000000000000000000001",38);
	   memcpy(&aucBuf[77],"00200000000003000000000000000000000001",38);
	   memcpy(&aucBuf[115],"00300000000004000000000000000000000001",38);
	   memcpy(&aucBuf[153],"00400000000005000000000000000000000001",38);
	   memcpy(&aucBuf[191],"66666600000006000000000000000000000001",38);
	   memcpy(&aucBuf[229],"77777700000007000000000000000000000001",38);
	   memcpy(&aucBuf[267],"00800000000008000000000000000000000001",38);
	   memcpy(&aucBuf[305],"00900000000009000000000000000000000001",38);
	   memcpy(&aucBuf[343],"01000000000010000000000000000000000001",38);
	   
	   
	   uiTableNum = 10;
	                     
	//   XDATA_AppendRecords(DataSaveZoneCtrls,aucBuf,sizeof(CARDZONE),uiTableNum);
	   
	   
	   iFileHandle = OSAPP_OpenFile((char *)FileInfo[DataSaveZoneCtrls].aucFileName,O_READ);
	
	if( iFileHandle < 0 )
	{
		OSAPP_FileClose(iFileHandle);
		OSAPP_FileGetLastError();
		return;
	}
	
	 memset(aucBuf1,0,sizeof(aucBuf1));
	iFileSize = OSAPP_FileSize((char *)FileInfo[DataSaveZoneCtrls].aucFileName);
	if( iFileSize >= 0 )
	{
	
		if( iFileSize > 0 )
		{
			if(OSAPP_FileSeek(iFileHandle,0,SEEK_SET)==0)
				iReadSize = OSAPP_FileRead(iFileHandle,aucBuf1,iFileSize);
		}
	}
	Uart_Printf("\niReadSize=%d",iReadSize);
	Uart_Printf("\n");
	for(iI=0;iI<iReadSize;iI++)
	{
		 Uart_Printf("%02x  ",aucBuf1[iI]);
		 if(iI%20 == 0)
		 	Uart_Printf("\n");
	}
	
}

unsigned char UTIL_IPAnalyze(uchar *aucInputIP,ulong *ulIp)
{
	int i,j,k,uiIp;
	unsigned char aucIp[4];
	j=0;k=0;
 	Uart_Printf("\n");
	for(i=0;i<16;i++)
	{
		if(aucInputIP[i]>'9'||aucInputIP[i]<'0')
		{
			uiIp=asc_long(&aucInputIP[i-j],j);
			aucIp[k]=uiIp;
			Uart_Printf("aucIp[%d]=%d,",k,aucIp[k]);
			j=0;k++;
			if(k>3)break;
		}
		else
			j++;
	}
 	Uart_Printf("\n");
	memcpy(ulIp,aucIp,sizeof(unsigned long));
	return SUCCESS;
}
unsigned char UTIL_BitFlagAnalyze(uchar *aucBitFlag,int iLen,uchar *aucAscFlag)
{
	int i,j;
	unsigned char ucBf,*ucAf,ucT;
	ucAf=aucAscFlag;
	for(i=0;i<iLen;i++)
	{
		ucBf=aucBitFlag[i];
		ucT=0x80;
		for(j=0;j<8;j++)
		{
			if(ucBf&ucT)
				*ucAf=1;
			else
				*ucAf=0;
			ucAf++;
			ucT=ucT>>1;
		}
	}
	return SUCCESS;
}

unsigned char UTIL_DisPayFeeInfo(void)
{
	unsigned char ch,aucBuf[17];

	//Os__clr_display(255);
	memset(aucBuf,0,sizeof(aucBuf));
	UTIL_Form_Montant(&aucBuf[5],NormalTransData.ulRateAmount,2);	
	if(RunData.ucRateType2=='0')
		memcpy(aucBuf,(unsigned char *)":",7);
	else
		memcpy(aucBuf,(unsigned char *)":",5);
	//Os__GB2312_display(0,0,aucBuf);
	
	memset(aucBuf,0,sizeof(aucBuf));
	if(RunData.ucRateType2=='1')
	{
		NormalTransData.ucBonusflag=1;
		memcpy(aucBuf,(unsigned char *)"ʵ:",5);
		UTIL_Form_Montant(&aucBuf[5],NORMALTRANS_ulAmount,2);	
	}
	else if(RunData.ucRateType1=='0')
	{
		memcpy(aucBuf,(unsigned char *)"ܿ:",5);
		UTIL_Form_Montant(&aucBuf[5],NORMALTRANS_ulAmount,2);	
	}
	else
	{
		memcpy(aucBuf,(unsigned char *)":",5);
		UTIL_Form_Montant(&aucBuf[5],NORMALTRANS_ulAmount-NormalTransData.ulRateAmount,2);	
	}
	//Os__GB2312_display(1,0,aucBuf);
	//Os__GB2312_display(2,0,(unsigned char *)"[ȷ]--");
	//Os__GB2312_display(3,0,(unsigned char *)"[ȡ]--˳");

	ch=0;
	while(ch!=KEY_ENTER&&ch!=KEY_CLEAR)
	{
		ch=Os__xget_key();
	}
	if(ch==KEY_ENTER)
		return SUCCESS;
	else
		return ERR_CANCEL;
	
}

unsigned char UTIL_DispRecycleInfo(unsigned long ulInAmount)
{
	unsigned long ulPreBalance;
	unsigned char ch,aucBuf[17],ucAbort;
	unsigned int    uiTimesOut;

	DRV_OUT *pxKey,*pxIcc;
	
	memset(aucBuf,0,sizeof(aucBuf));
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"۳:");
	UTIL_Form_Montant(aucBuf,ulInAmount,2);	
	Os__display(1,5,aucBuf);
	//Os__GB2312_display(2,0,(unsigned char *)"ʹѺ:");
	memset(aucBuf,0,sizeof(aucBuf));
	ulPreBalance=NORMALTRANS_ulAmount-ulInAmount;
	UTIL_Form_Montant(aucBuf,ulPreBalance,2);
	Os__display(3,5,aucBuf);

	while(1)
	{
		pxKey=Os__get_key();
		uiTimesOut = 30*100;
		Os__timer_start(&uiTimesOut);
		while((uiTimesOut)&&(pxKey->gen_status != DRV_ENDED));
		Os__timer_stop(&uiTimesOut);

		if(!uiTimesOut)
		{
                     Os__abort_drv(drv_pad);
			Os__abort_drv(drv_mmi);
			//ucAbort = true;
			uiTimesOut = 10;
			Os__timer_start(&uiTimesOut);
			while(uiTimesOut);
			Os__timer_stop(&uiTimesOut);
			//ucResult = ERR_CANCEL;
			return ERR_CANCEL;		
		}
		else if(pxKey->gen_status == DRV_ENDED)
		{				
			ch = pxKey->xxdata[1];
			if(ch==KEY_ENTER)
				break;
			else
				return ERR_CANCEL;
		}
	 }
	
	memset(aucBuf,0,sizeof(aucBuf));
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"ʣѺ:");
	ulPreBalance=NormalTransData.ulYaAmount-ulPreBalance;
	UTIL_Form_Montant(aucBuf,ulPreBalance,2);
	Os__display(1,5,aucBuf);
	//Os__GB2312_display(2,0,(unsigned char *)"1,˿");
	//Os__GB2312_display(3,0,(unsigned char *)"2,ȡ");

	while(1)
	{
		pxKey=Os__get_key();
		uiTimesOut = 30*100;
		Os__timer_start(&uiTimesOut);
		while((uiTimesOut)&&(pxKey->gen_status != DRV_ENDED));
		Os__timer_stop(&uiTimesOut);

		if(!uiTimesOut)
		{
                     Os__abort_drv(drv_pad);
			Os__abort_drv(drv_mmi);
			//ucAbort = true;
			uiTimesOut = 10;
			Os__timer_start(&uiTimesOut);
			while(uiTimesOut);
			Os__timer_stop(&uiTimesOut);
			//ucResult = ERR_CANCEL;
			return ERR_CANCEL;		
		}
		else if(pxKey->gen_status == DRV_ENDED)
		{				
			ch = pxKey->xxdata[1];
			if(ch=='1')	
			{     
				break;
			}
			else if(ch=='2')	
				return ERR_CANCEL;
			else
				continue;
		}
	 }

	NormalTransData.ucReturnflag=true;
	if(NormalTransData.ucTransType==TRANS_S_PURCHASE)
		NormalTransData.ucTransType=TRANS_S_RETURNCARD;
	if(NormalTransData.ucTransType==TRANS_S_PURCHASE_P)
		NormalTransData.ucTransType=TRANS_S_RETURNCARD_P;
	return SUCCESS;
}
unsigned char UTIL_PinVersionAnalyze(uchar *aucIndata,int iLen,uchar *aucOutData)
{
	int i,j;
	unsigned char ucBf,*ucAf,ucT;
	Uart_Printf("aucIndata:\n");
	for(i=0;i<iLen;i++)
		Uart_Printf("%02x ",*(aucIndata+i));
	for(i=0;i<iLen;i++)
	{
		switch(aucIndata[i])
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				aucOutData[i]=aucIndata[i]+'0';
				break;
			case 0x0A:
				aucOutData[i]='A';
				break;
			case 0x0B:
				aucOutData[i]='B';
				break;
			case 0x0C:
				aucOutData[i]='C';
				break;
			case 0x0D:
				aucOutData[i]='D';
				break;
			case 0x0E:
				aucOutData[i]='E';
				break;
			case 0x0F:
				aucOutData[i]='F';
				break;
			default:
				aucOutData[i]=aucIndata[i];
				break;
		}
	}
	Uart_Printf("aucOutData:\n");
	for(i=0;i<iLen;i++)
		Uart_Printf("%02x ",*(aucOutData+i));
	return SUCCESS;
}


unsigned char UTIL_SetRateType(void)
{
    unsigned char ch,ucResult=SUCCESS;
MIMA:   //Os__clr_display(255);
        //Os__GB2312_display(0,0,(unsigned char *)"ǰ");
        //Os__GB2312_display(1,0,(unsigned char *)"1.ֽ 2.Ƭ");
        //Os__GB2312_display(2,0,(unsigned char *)"Ϊ:");
       // if (!DataSave0.ConstantParamData.Rate_flag)
            //Os__GB2312_display(0,5,(unsigned char *)"ֽ");
      //  else
            //Os__GB2312_display(0,5,(unsigned char *)"Ƭ");              
        ch=UTIL_GetKey(15);
        if(ch=='1') 
        {
            //Os__GB2312_display(2,6,(unsigned char *)"ֽ");
            ch=UTIL_GetKey(15);
                if(ch==KEY_ENTER) 
            {
                Os__saved_set(&DataSave0.ConstantParamData.Rate_flag,0,1);
                ucResult=XDATA_Write_Vaild_File(DataSaveConstant);
		  if(ucResult!=SUCCESS)
			  return ucResult;
                    return SUCCESS;
               }else   return ERR_CANCEL;// goto MIMA;                  
        }
        else if(ch=='2') 
            {
                //Os__GB2312_display(2,6,(unsigned char *)"Ƭ");
                    ch=UTIL_GetKey(15);
                    if(ch==KEY_ENTER) 
                {
                    Os__saved_set(&DataSave0.ConstantParamData.Rate_flag,1,1);
                    ucResult=XDATA_Write_Vaild_File(DataSaveConstant);
       		  if(ucResult!=SUCCESS)
       			  return ucResult;
                    return SUCCESS;
                }else   return ERR_CANCEL;//goto MIMA;                      
                }else if(ch==KEY_CLEAR)  return ERR_CANCEL;
                    else  goto MIMA;    
}
unsigned char UTIL_SetFixAmount(void)
{
	unsigned char ch=0,ucResult=SUCCESS,aucBuf[20];
	while(1)
	{
		ch=0;
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(unsigned char *)"");
	        //Os__GB2312_display(1,0,(unsigned char *)"1.޸Ľ");
	        //Os__GB2312_display(2,0,(unsigned char *)"2.ر");
	        //Os__GB2312_display(3,0,(unsigned char *)"ǰ:ر");
		if(DataSave0.ChangeParamData.ulFixAmount)
		{
			memset(aucBuf,0,sizeof(aucBuf));
			UTIL_Form_Montant(aucBuf,DataSave0.ChangeParamData.ulFixAmount,2);
			Os__display(3,5,aucBuf);
		}
		while(ch!='1'&&ch!='2'&&ch!=KEY_CLEAR&&ch!=KEY_ENTER)
			ch=MSG_WaitKey(60);
		if(ch=='1')
		{
			//Os__clr_display(255);
			//Os__GB2312_display(0,0,(unsigned char *)"䶨ѽ");
			ucResult=UTIL_InputAmount(1,&DataSave0.ChangeParamData.ulFixAmount,1,DataSave0.ConstantParamData.ulMaxXiaoFeiAmount);
			if(ucResult==SUCCESS)
				ucResult=XDATA_Write_Vaild_File(DataSaveChange);
		}
		else if(ch=='2')
		{
			DataSave0.ChangeParamData.ulFixAmount=0;
			ucResult=XDATA_Write_Vaild_File(DataSaveChange);
		}
		else
		{
			return SUCCESS;
		}
	}
}
unsigned char UTIL_GetNewExp(void)
{
	unsigned char ucResult=SUCCESS;
	int i,j;
	unsigned long ulyear;
	Uart_Printf("UTIL_GetNewExp:\n");
	for(i=0;i<20;i++)
	{
		if(ExpTransData.ExpFeeData[i].ucPt)
		{
			for(j=0;j<8;j++)
				Uart_Printf("%02x ",*(&ExpTransData.ExpFeeData[i].ucPt+j));
			Uart_Printf("\n");
		}
	}
	for(i=0;i<30;i++)
	{
		if(ExpTransData.ExpParamData[i].ucPt)
		{
			for(j=0;j<11;j++)
				Uart_Printf("%02x ",*(&ExpTransData.ExpParamData[i].ucPt+j));
			Uart_Printf("\n");
		}
	}
	for(i=0;i<20;i++)
	{
		if(ExpTransData.ExpFeeData[i].ucPt==NormalTransData.ucPtCode)
		{
			if(ExpTransData.ExpFeeData[i].ucPt==1||ExpTransData.ExpFeeData[i].ucPt==2
				||(ExpTransData.ExpFeeData[i].ucPt==3&&!memcmp(ExpTransData.ExpFeeData[i].aucZone,NormalTransData.aucZoneCode,2)))
			{
				NormalTransData.ulAmount=bcd_long(ExpTransData.ExpFeeData[i].aucFee,8);
				break;
			}
		}
	}
	for(i=0;i<30;i++)
	{
		if(ExpTransData.ExpParamData[i].ucPt==NormalTransData.ucPtCode
			&&memcmp(&NormalTransData.aucDateTime[2],ExpTransData.ExpParamData[i].aucBegin,2)>=0
			&&memcmp(&NormalTransData.aucDateTime[2],ExpTransData.ExpParamData[i].aucEnd,2)<=0
			)
		{
			if(ExpTransData.ExpParamData[i].ucPt==1||ExpTransData.ExpParamData[i].ucPt==2
			||(ExpTransData.ExpParamData[i].ucPt==3
			&&!memcmp(ExpTransData.ExpParamData[i].aucZone,NormalTransData.aucZoneCode,2)
			))
			{
				if(ExpTransData.ExpParamData[i].ucYear)
				{
					ulyear=bcd_long(NormalTransData.aucDateTime,4)+1;
					long_bcd(NormalTransData.aucNewExpiredDate,2,&ulyear);
				}
				else
					memcpy(NormalTransData.aucNewExpiredDate,NormalTransData.aucDateTime,2);
				memcpy(&NormalTransData.aucNewExpiredDate[2],ExpTransData.ExpParamData[i].aucNew,2);
				break;
			}
		}
	}
//	if(i==30)
//		return ERR_EXPNOPARAM;
	return SUCCESS;
}
unsigned char UTIL_SetMult(void)
{
	unsigned char ch=0,ucResult=SUCCESS,aucBuf[20];
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(unsigned char *)"ÿۿ");
	//Os__GB2312_display(1,0,(unsigned char *)"Ϊı?");
	ucResult=UTIL_InputAmount(2,&DataSave0.ConstantParamData.ulMult,1,DataSave0.ConstantParamData.ulMaxXiaoFeiAmount);
	if(ucResult==SUCCESS)
		ucResult=XDATA_Write_Vaild_File(DataSaveConstant);
	return ucResult;
}
unsigned char UTIL_SetCommMode(void)
{
	unsigned char ch=0,ucResult=SUCCESS,aucBuf[20];
	//Os__clr_display(255);
	//Os__GB2312_display(1,0,(uchar *)"1.  2.");
	//Os__GB2312_display(2,0,(uchar *)"3.GPRS  4.");
	//Os__GB2312_display(3,0,(uchar *)"5.CDMA");
	switch(Commun_flag)
	{
		case 0x31:
			//Os__GB2312_display(0,0,(unsigned char *)"ǰģʽ:");
			break;
		case 0x32:
			//Os__GB2312_display(0,0,(unsigned char *)"ǰģʽ:");
			break;
		case 0x33:
			//Os__GB2312_display(0,0,(unsigned char *)"ǰģʽ:GPRS");
			break;
		case 0x34:
			//Os__GB2312_display(0,0,(unsigned char *)"ǰģʽ:");
			break;
		case 0x35:
			//Os__GB2312_display(0,0,(unsigned char *)"ǰģʽ:CDMA");
			break;
		default:
			//Os__GB2312_display(0,0,(unsigned char *)"ǰģʽ:δ֪");
			break;
	}
	while(1)
	{
		ch=Os__xget_key();
		if(ch=='1')
		{
			Commun_flag=0x31;
			if(DataSave0.ConstantParamData.ucYTJFlag==2)
				DataSave0.ConstantParamData.ucYTJFlag=1;
		}
		else if(ch=='2')
		{
			Commun_flag=0x32;
			if(DataSave0.ConstantParamData.ucYTJFlag==2)
				DataSave0.ConstantParamData.ucYTJFlag=1;
		}
		else if(ch=='3')
		{
			Commun_flag=0x33;
			if(DataSave0.ConstantParamData.ucYTJFlag==2)
				DataSave0.ConstantParamData.ucYTJFlag=1;
		}
		else if(ch=='4')
		{
			Commun_flag=0x34;
			if(DataSave0.ConstantParamData.ucYTJFlag==1)
				DataSave0.ConstantParamData.ucYTJFlag=2;
		}
		else if(ch=='5')
		{
			Commun_flag=0x35;
			if(DataSave0.ConstantParamData.ucYTJFlag==2)
				DataSave0.ConstantParamData.ucYTJFlag=1;
		}
		else if(ch==KEY_CLEAR)
			return SUCCESS;
		else
			continue;
		break;
	}
	ucResult=XDATA_Write_Vaild_File(DataSaveConstant);
	if(ucResult==SUCCESS)
	{
		//Os__clr_display(255);
		//Os__GB2312_display(0,0,(unsigned char *)"޸ĳɹ");
		switch(Commun_flag)
		{
			case 0x31:
				//Os__GB2312_display(1,0,(unsigned char *)"ǰģʽ:");
				break;
			case 0x32:
				//Os__GB2312_display(1,0,(unsigned char *)"ǰģʽ:");
				break;
			case 0x33:
				//Os__GB2312_display(1,0,(unsigned char *)"ǰģʽ:GPRS");
				break;
			case 0x34:
				//Os__GB2312_display(1,0,(unsigned char *)"ǰģʽ:");
				break;
			case 0x35:
				//Os__GB2312_display(1,0,(unsigned char *)"ǰģʽ:CDMA");
				break;
			default:
				//Os__GB2312_display(1,0,(unsigned char *)"ǰģʽ:δ֪");
				break;
		}
		MSG_WaitKey(5);
	}
	return ucResult;
}
unsigned char UTIL_SetCOM(void)
{
	unsigned char ucInput,ucResult=SUCCESS,aucBuf[20];
	//Os__clr_display(255);
	//Os__GB2312_display(0,0,(uchar *)"POS");
	memset(aucBuf,0,sizeof(aucBuf));
	memcpy(aucBuf,"ǰ:",9);
	if(DataSave0.ConstantParamData.ucCom==1)
		memcpy(&aucBuf[9],"һ",6);
	if(DataSave0.ConstantParamData.ucCom==2)
		memcpy(&aucBuf[9],"ڶ",6);
	//Os__GB2312_display(1,0,aucBuf);
	//Os__GB2312_display(2,0,(uchar *)"1.һ");
	//Os__GB2312_display(3,0,(uchar *)"2.ڶ");
	ucInput=Os__xget_key();
	if(ucInput==KEY_CLEAR)
	{
		return ERR_CANCEL;
	}
	if(ucInput=='2')
		DataSave0.ConstantParamData.ucCom=2;
	if(ucInput=='1')
		DataSave0.ConstantParamData.ucCom=1;
	ucResult=XDATA_Write_Vaild_File(DataSaveConstant);
	return ucResult;
}
