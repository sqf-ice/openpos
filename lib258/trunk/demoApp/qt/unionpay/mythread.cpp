#include "mythread.h"
#include "QDebug"

extern "C"{
#include "include.h"
#include "global.h"
#include "masapp.h"
}

void  MyThread::run()
{
    qDebug("%d^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^",ProUiFace.UiToPro.ucTransType);
    ProgressFlag=0;
    g_ThreadFlag = 1;
	unsigned char ucResult=0;
	seven *EventInfo;
	//FACEUITOPRO exPro;
	//memset((uchar*)&exPro,0,sizeof(FACEUITOPRO));
	//memcpy((uchar*)&exPro,(uchar*)&ProUiFace.UiToPro,sizeof(FACEUITOPRO));
	qDebug("	1.MyThread::ucTransType...[%02x]", ProUiFace.UiToPro.ucTransType);	
	G_NORMALTRANS_ucTransType = ProUiFace.UiToPro.ucTransType;	
	G_NORMALTRANS_ucInputMode = ProUiFace.UiToPro.CardInfo.ucInputMode;	
	
	switch(G_NORMALTRANS_ucTransType)
	{
	case TRANS_LOGONON:
		qDebug("	2.MyThread::TRANS_LOGONON...");    		
		LOGON_CashierProcess(0);
		break;
	case TRANS_LOGONOFF:
		qDebug("	2.MyThread::TRANS_LOGONOFF...");
		LOGON_CashierProcess(0);
		break;
	case TRANS_QUERYBAL:
		qDebug("	2.MyThread::TRANS_QUERYBAL...");
		ucResult = Trans_Process(0);
		break;		
	case TRANS_PURCHASE:
		qDebug("	2.MyThread::TRANS_PURCHASE...");
		switch(ProUiFace.UiToPro.CardInfo.ucInputMode)
		{
		case TRANS_INPUTMODE_SWIPECARD: 
			//memcpy(EventInfo->pt_eve_in->e_mag_card.mag_card, ProUiFace.UiToPro.aucCardAcc, strlen((char*)ProUiFace.UiToPro.aucCardAcc));    	    	
			EXTrans_ProHead_Mag(EventInfo); 
			break;
		case TRANS_INPUTMODE_INSERTCARD: 
			EXTrans_Process_IC(EventInfo); 
			break;
		case TRANS_INPUTMODE_SAND_PUTCARD: 
			ucResult = Trans_Process(0);
			break;
		}
		break;
	case TRANS_VOIDPURCHASE:
		qDebug("	2.MyThread::TRANS_QUERYBAL...");
		ucResult = Trans_Process(0);		
		break;
	case TRANS_REFUND:
		qDebug("	2.MyThread::TRANS_QUERYBAL...");
		ucResult = Trans_Process(0);		
		break;
	case TRANS_PREAUTH:
		ucResult = Trans_Process(0);
		break;
	case TRANS_PREAUTHFINISH:
		ucResult = Trans_Process(0);
		break;		
	case TRANS_VOIDPREAUTH:
		ucResult = Trans_Process(0);
		break;
	case TRANS_VOIDPREAUTHFINISH:
		ucResult = Trans_Process(0);
		break;
	case TRANS_PREAUTHSETTLE:
		ucResult = Trans_Process(0);
		break;		
	case TRANS_SETTLE:
		SERV_Settle(true);
		break;
		
		
		
	case TRANS_AID_PARAM: // show AID  Info
		EMV_DisplayAIDParam();
		break;
	case TRANS_RID_PARAM: // show RID Info
		EMV_DisplayPKInfo();
		break;

	case TRANS_PRINT_BYTRACE: // Print Every Trans
		SERV_ReprintByTrace();
		break;
	case TRANS_PRINT_LASTTRANS:// Reprint Last Trans
		SERV_Reprint();
		break;
	case TRANS_PRINT_LASTSETTLE: // Print Last Settle
		PRINT_RePrint_List();
		break;
	case TRANS_PRINT_TRANSDETAIL: // Print Trans Detail
		PRINT_Detail_Menu();
		break;
	case TRANS_PRINT_STATIC: // Print Trans Static
		SERV_ReprintSettle();
		break;

	case TRANS_SHOW_STATIC: // SHOW Trans Static
		SERV_DispTotal();
		break;
	case TRANS_SHOW_VERSION: // SHOW version
		CFG_ConstantParamVersion();
		break;

	case TRANS_ONLINETEST: // Roll Back
		ucResult = Trans_TestSingal();
		COMMS_FinComm();
		break;
	case TRANS_SHOW_TRNSDETAIL: // SHOW Trans Static
		Trans_Query();
		break;
		
	case TRANS_PRINT_TTS:
		PRINT_tts();
		break;
	}

	qDebug("	3.MyThread::run...1.");   

    sem_post(&binSem1);
    sem_wait(&binSem2);

    memset((uchar*)&ProUiFace.OweDrv,0x00,sizeof(ProUiFace.OweDrv));
    memset((uchar*)&ProUiFace.ProToUi,0x00,sizeof(ProUiFace.ProToUi));
    memset((uchar*)&ProUiFace.UiToPro,0x00,sizeof(ProUiFace.UiToPro));

    qDebug("	4.MyThread::run...End.");
    ProgressFlag=1;
    g_ThreadFlag = 0;
    
}

}
