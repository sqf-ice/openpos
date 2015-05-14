#include <include.h>
#include <Global.h>
#include <xdata.h>


#pragma arm section rwdata ,code = "manager"

#if 0
void manager(struct seven *EventInfo)
{
	Uart_Printf("manager\r\n");
	unsigned char ucResult ;
	unsigned char EventType;
	unsigned char EventNum;
	

	EventType = (unsigned char )EventInfo->event_type;
	EventNum = (unsigned char )EventInfo->event_nr;

	
	ucResult = SUCCESS;

	Os__light_on();
	EventInfo->pt_eve_out->treatment_status = OK;
	UTIL_ClearGlobalData();
	
	switch ( EventType+EventNum)
	{
		case EVECIR + EVE_MAG_CARD:
			ucResult = MAG_GetTrackInfo(&(EventInfo->pt_eve_in->e_mag_card.mag_card[0]));
			if(!ucResult)
			{
			      ucResult = Trans_Process();
			}
			if(( ucResult != SUCCESS)&&( ucResult != ERR_CANCEL))	
			{
				MSG_DisplayErrMsg(ucResult);
			}
			Os__light_off();
			break;
	     /*  case EVESEL + '1':
		case EVESEL + '2':
			Mag_Menu(EventNum);
			Os__light_off();
		      break;*/
	       case EVESEL + 0xff:
			/*Os__clr_display(255);
			Os__GB2312_display(0,0,(uchar *)"1.ǩ��  2.����");*/
			Mag_Menu(EventNum);
			Os__light_off();
			break;
		case EVECOM + EVE_POWER_ON:
			OnEve_power_on();
			break;
	       default:
		        break;
	}
}
#endif
#pragma arm section code ,rwdata


