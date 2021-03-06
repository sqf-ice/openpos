/*
*osicc.h
* Sand ICCard Driver for Linux OS
*/

#ifndef __OSICC_H__
#define __OSICC_H__

#include "libicc.h"

extern struct ic_device ICDEV;
extern int fd_icc;

extern unsigned char ucCardReader;
//extern unsigned char ucSW1;
//extern unsigned char ucSW2;
extern unsigned char ucCardRecvLen;
extern unsigned char ucCardRecvStatus;
extern unsigned char aucCardRecvBuf[256];//recv from IC card

extern unsigned char ucCardSendLen;
extern unsigned char aucCardSendBuf[256];


extern unsigned char uiRecvLen;
extern unsigned char uiRecvedLen;
extern unsigned char ucAPDUCase;
extern unsigned char ucAPDUOneByteFlag;
extern unsigned char uiAPDUSendLen;
extern unsigned char uiAPDUSentLen;
extern unsigned char uiAPDURecvLen;
extern unsigned char uiAPDURecvedLen;
extern volatile unsigned char ucATRLen;

extern volatile unsigned short uiT1FrameLastSendLen;
extern volatile unsigned short uiT1FrameSendLen;
extern volatile unsigned short uiT1FrameSentLen;



#define HALICC_RESET_COLD 0x00	//冷复位
#define HALICC_RESET_WARM 0x01	//热复位

#define ICCMETHOD_ASYNC					0x00
#define ICCMETHOD_SYNC					0x01


// ICC and SIM reader numbers :
#define ICC1            0
#define SIM1            1
#define SIM2            2
#define SIM3            3
#define SIM4            4

// commands to execute in the ICC or SAM reader
#define POWER_ON        	0
#define POWER_OFF       	1
#define SYNC_ORDER      	2
#define ASYNC_ORDER     	3
#define ASYNC_PROTOCOL  	4
#define WARM_RESET      	5
#define NEW_SYNC_ORDER  	6
#define NEW_POWER_ON    	7
#define NEW_WARM_RESET		8
#define CHANGE_PROTOCOL 	9
#define SET_ICC_STANDARD 	10
#define EXCHANGE_BYTES  	100

#define ISO7816         0
#define EMV             1
#define MPEV5           2
#define VISA            3
#define MONDEX          4
#define CLA_IDEN        0
#define CLA_00          1
#define PROT_T0         0
#define PROT_T1         1
#define PROT_DFT        0xff

// execution status from ICC and SIM driver (drv_status)
#define OK              0
#define KO              1
#define NOICC           1
#define NOREADER        2
#define OVERRUN         6
#define DRVUNKNOWN      7
#define WRONGREADER     10
#define WRONGORDER      11
#define NULLPTOUT       12

#define ASY_OK          0x30      /* treatment ok */
#define ICC_REMOVED     0x31      /* smart card removed during the treatment */
#define VCC_ERROR       0x32      /* Vcc default */
#define VPP_ERROR       0x33      /* Vpp default */
#define ICC_DIALOG_ERR  0x34      /* error of dialog with the smart card */
#define ICC_ASY         0x35      /* asynchronous smart card detected */
#define ICC_ERR         0x36      /* driver unable to read the asynchronous smart card */
#define ICC_SYN         0x37      /* synchronous smart card detected */
#define ICC_MUTE        0x38      /* smart card dumb */
#define ICC_RUNNING     0xFF      /* treatment running */

// order for an ASYNCHRONOUS chip
typedef struct
{
    unsigned char   Lc;
    unsigned char * ptin;
}type0;
typedef struct
{
    unsigned char   CLA;
    unsigned char   INS;
    unsigned char   P1;
    unsigned char   P2;
}type1;
typedef struct
{
    unsigned char   CLA;
    unsigned char   INS;
    unsigned char   P1;
    unsigned char   P2;
    unsigned char   Le;
}type2;
typedef struct
{
    unsigned char   CLA;
    unsigned char   INS;
    unsigned char   P1;
    unsigned char   P2;
    unsigned char   Lc;
    unsigned char * ptin;
}type3;
typedef struct
{
    unsigned char   CLA;
    unsigned char   INS;
    unsigned char   P1;
    unsigned char   P2;
    unsigned char   Lc;
    unsigned char * ptin;
    unsigned char   Le;
}type4;
typedef struct
{
    unsigned short   Lc;
    unsigned char * ptin;
}type6;
typedef struct
{
    unsigned char   pcb;
    unsigned char   Lc;
    unsigned char * ptin;
}type7;
union AS_ORDER_TYPE
{
    type0   order_type0;
    type1   order_type1;
    type2   order_type2;
    type3   order_type3;
    type4   order_type4;
    type3   order_type5;
    type6   order_type6;
    type7   order_type7;
};
typedef struct
{
    unsigned char   order_type;
    union AS_ORDER_TYPE as_order;       // => according to 'order_type' value
    unsigned char   NAD;
}ASYNC_ORDER_TYPE;

// order for a SYNCHRONOUS chip managed by the BIOS
typedef struct
{
    unsigned char   card_type;  // chip ID
    unsigned char   INS;
    unsigned char   ADDH;
    unsigned char   ADDL;
    unsigned char   Len;  // data length in BITS
    unsigned char * ptin; // data transmitted
}SYNC_ORDER_TYPE;

// new function (execute a list of basic orders) for a SYNCHRONOUS chip
typedef struct
{
    unsigned char   Len;    // length of the list of orders
    unsigned char * ptin;   // list of orders to execute
}NEW_SYNC_ORDER_TYPE;

// cold or warm reset with parameters for card standards and preferred protocol
typedef struct
{
    unsigned char card_standards;
    unsigned char GR_class_byte_00;
    unsigned char preferred_protocol;
}NEW_POWER_TYPE;

// send/receive exchange for PARAVANT card
typedef struct
{
    unsigned short  send_len;        // number of bytes to send to ICC
    unsigned char   *ptin ;          // points to data to send
    unsigned short  rec_len;         // number of bytes to receive
    unsigned char   TC1;             // Extra Guard Time to be used for the sending
    unsigned char   check_status;    // check or not the first 2 received bytes (0x90/0x00)
}EXCHANGE_TYPE;

// order to be transmitted to the ICC or SIM
union ORDER_IN_TYPE
{
    NEW_SYNC_ORDER_TYPE new_sync_order; // new functions for synchronous chip
    SYNC_ORDER_TYPE     sync_order;     // order to a synchronous chip
    ASYNC_ORDER_TYPE    async_order;    // order to an asynchronous chip
    unsigned char       async_protocol; // T=0 or T=1 protocol selection
    NEW_POWER_TYPE      new_power;      // power on with parameters
    EXCHANGE_TYPE       exchange;       // SEND/RECEIVE exchange (PARAVANT)
    unsigned char       icc_standard;   // ISO7816, EMV, MPEV5, VISA or MONDEX
};

// command to execute in the ICC or SIM reader
typedef struct
{
    unsigned char   order;                      // order number to execute
    union ORDER_IN_TYPE * pt_order_in;  // order to be transmitted
    unsigned char * ptout;                      // output buffer
}ICC_ORDER;

// execution report
typedef struct
{
    unsigned char   drv_status;
    unsigned char   card_status;
    unsigned short  Len;
}ICC_ANSWER;




#if 0
/*****copy from libicc.h**********************************************************************/
typedef struct
{
    volatile unsigned char ucStatus;
    volatile unsigned char ucSession;
    volatile unsigned char ucSessionStep;
    volatile unsigned char ucLastError;
    volatile unsigned char ucPowerType;
    volatile unsigned char ucResetType;
    volatile unsigned char ucProtocol;
    unsigned char ucFi;
    unsigned char ucDi;
    unsigned char ucRate;
    unsigned char ucGTR;
    unsigned long ulWWT;
    unsigned short uiCWT;
    unsigned char ucWTX;
    unsigned long ulBWT;
    unsigned char ucT1IFSReqFlag;
    unsigned char ucT1IFSC;
    unsigned char ucT1SendSeq;
    unsigned char ucT1RecvSeq;
    unsigned char ucT1EDCMode;
    unsigned char ucCardType;				// 卡的类型
}ICCINFO;

extern ICCINFO ICCInfo[4];


#define ICCMETHOD_ASYNC					0x00
#define ICCMETHOD_SYNC					0x01

#define ICCPROTOCOL_T1                  0x01
#define ICCPROTOCOL_T0                  0x00

#define ICCAPDU_CASE0                   0x00
#define ICCAPDU_CASE1                   0x01
#define ICCAPDU_CASE2                   0x02
#define ICCAPDU_CASE3                   0x03
#define ICCAPDU_CASE4                   0x04
#define ICCAPDU_CASE5                   0x05
#define ICCAPDU_CASE6                   0x06
#define ICCAPDU_CASE7                   0x07

#define ICCSTATUS_INSERT                0x01
#define ICCSTATUS_REMOVE                0x02
#define ICCSTATUS_POWERED               0x03
#define ICCSTATUS_ISSYNCCARD			0x04

#define ICCTYPE_CPUCARD					0x00
#define ICCTYPE_SLE4442CARD				0x01

#define ICCSESSION_IDLE                 0x00
#define ICCSESSION_COLDRESET            0x01
#define ICCSESSION_WARMRESET            0x02
#define ICCSESSION_DATAEXCH             0x03

#define ICCSESSIONSTEP_IDLE             0x00
#define ICCSESSIONSTEP_COLDRESET1       0x01
#define ICCSESSIONSTEP_COLDRESET2       0x02
#define ICCSESSIONSTEP_COLDRESET3       0x03

#define ICCSESSIONSTEP_DATAEXCHSEND1    0x04
#define ICCSESSIONSTEP_DATAEXCHRECV1    0x05
#define ICCSESSIONSTEP_DATAEXCHSEND2    0x06
#define ICCSESSIONSTEP_DATAEXCHRECV2    0x07
#define ICCSESSIONSTEP_DATAEXCHSEND3    0x08
#define ICCSESSIONSTEP_DATAEXCHRECV3    0x09
#define ICCSESSIONSTEP_DATAEXCHSEND4    0x0A
#define ICCSESSIONSTEP_DATAEXCHRECV4	0x0B

#define ICCCMD_SUCCESS					0x00
#define ICCCMD_BADPACKET				0x90
#define ICCCMD_NODATASEND				0x91

#define ICCCMD_MAXNB                    0x0C
#define ICCCMD_MAXBUF					256
#define LIBICC_MAXATRBUF                100
#define LIBICC_MAXCOMMBUF	            270

#define LIBICC_COLDRESET                0x00
#define LIBICC_WARMRESET                0x01


#define LIBICC_WWTOUT					0x00
#define LIBICC_BWTOUT					0x01
#define LIBICC_CWTOUT					0x02

#define	REV_TIME_OUT					100
#endif

extern unsigned char aucT1FrameSendBuf[LIBICC_MAXCOMMBUF];
extern unsigned char aucATR[LIBICC_MAXATRBUF];
extern unsigned char aucAPDURecvBuf[LIBICC_MAXCOMMBUF];


#if 0
/******************************** old API ********************************/
// card detection in a smart card reader
unsigned char Os__ICC_detect( unsigned char reader_nb );

// execute a command of the ICC or SIM driver
ICC_ANSWER * Os__ICC_command( unsigned char reader_nb, ICC_ORDER * order );
DRV_OUT *Os__ICC_insert(void);
DRV_OUT *Os__ICC_Async_order(unsigned char *pucOrder,unsigned short uiLen);
DRV_OUT *Os__ICC_order(DRV_IN  *pDrvIn);

void Os__ICC_remove(void);
void Os__ICC_off(void);
#endif

/******************************** new API ********************************/
void    OSICC_SetReport(unsigned char ucICCStatus);

unsigned char OSICC_NewPowerOn(unsigned char ucType);

unsigned char OSICC_ChangeReader(unsigned char ucReader);
unsigned char OSICC_PowerOn(unsigned char ucResetType);
unsigned char OSICC_Detect(unsigned char ucReader);
unsigned char OSICC_ResetUart(unsigned char ucReader);
unsigned char OSICC_SetClock(unsigned char ucReader, unsigned char ucStopFlag,unsigned char ucLevel);
unsigned char OSICC_SetMethod(unsigned char ucReader,unsigned char ucMethod);
unsigned char OSICC_SetBaud(unsigned char ucReader, unsigned char ucBaudMulti);
unsigned char OSICC_SetDi(unsigned char ucReader, unsigned char ucDi);
unsigned char OSICC_SetProtocol(unsigned char ucReader,unsigned char ucProtocol);
unsigned char OSICC_SetConvention(unsigned char ucReader, unsigned char ucConvention);
unsigned char OSICC_SetPowerType(unsigned char ucReader, unsigned char ucPowerType);
unsigned char OSICC_SetPEC(unsigned char ucReader, unsigned char ucPEC);
ICC_ANSWER * Os__ICC_command( unsigned char reader_nb, ICC_ORDER * order );
ICC_ANSWER *OSICC_Command(unsigned char ucReader, ICC_ORDER *pOrder);
void    OSICC_SetReport(unsigned char ucICCStatus);
unsigned char OSICC_ChangeReader(unsigned char ucReader);
unsigned char OSICC_Activate(unsigned char ucReader);
unsigned char OSICC_PowerOn(unsigned char ucResetType);
unsigned char OSICC_SetWaitTime(unsigned char ucReader,
                                unsigned char ucWaitType,
                                unsigned char ucWaitValue0,unsigned char ucWaitValue1,
                                unsigned char ucWaitValue2,unsigned char ucWaitValue3);
void OSICC_Init(void);



#endif

