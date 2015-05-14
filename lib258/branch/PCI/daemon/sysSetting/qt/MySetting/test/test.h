#ifndef _TEST_H
#define _TEST_H

#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <errno.h>
#include <termios.h>

#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//#include "sand_types.h"
//#include "osicc.h" //error
//#include "smart.h"
//#include "libicc.h" //error
#ifdef __cplusplus
extern "C" {
#endif

//ICC_ORDER icc_order;

#define OSMENUERR_SUCCESS		0x00
#define OSMENU_PAGEMENUITEM		6
#define fU8(fn)     (unsigned char  (*)())(fn)

typedef struct
{
    unsigned char *pucText;
    unsigned char (*pfnProc)(void);
}OSMENU;

struct _if {
    char name[8];    //"eth0","ppp0","lo" ....
    char state[8];   //UP or DOWN
    char mac[32];    //string of MAC address
    char ip[32];     //string of IP address
    char br[32];     //string of broadcast address
    char netmsk[32]; //string of netmask address
};

#define OK              0
#define	SUCCESS 	0x00
#define FAILED    	0x01
#define BAUD_ERROR	0x02

#define PRN_ERROR_NO_PAPER      1
#define PRN_ERROR_HOT           2
#define PRN_ERROR_HOT_PAPER     3
#define PRN_ERROR_FIFO          4

unsigned char OSGSM_ATCmd(unsigned char *cmd, unsigned char *rsp, unsigned short ms);
unsigned char OSGSM_openPort(void);
unsigned char SGSM_setupUart(int fd);
unsigned char OSGSM_init(void);


unsigned char OSGSM_answerCalling(void);
unsigned char OSGSM_hangUp(void);
unsigned char OSGSM_exit(void);

unsigned char SLE4442_ATR(unsigned char *pucBuf);
unsigned char SLE4442_ResetAddress(unsigned char *pucBuf);
unsigned char SLE4442_NReadBits(unsigned char *pucBuf, unsigned char ucbitscnt);
unsigned char SLE4442_End_ATR(unsigned char *pucBuf);
unsigned char SLE4442_ReadMainMemory(unsigned char *pucBuf, unsigned char ucaddr,unsigned char ucsize);

unsigned char PBOC_warmreset(unsigned char ucReader,unsigned char * pucReset,unsigned short * pusLength);

unsigned long asc_long(unsigned char *Pts, unsigned char Ls);
//ISO7816OUT iso7816out;
//ICC_ANSWER *picc_answer;
//int OSTEST_printer_result(int retval);
//unsigned char Os__checkpaper();
//unsigned char Os__GB2312_xprint (uchar * Text, uchar Size) ;

//OSMSGDATA unsigned char OSERRMSG_ICCAsyncReset[]
//#ifdef OSMSGVALUE
//       = "ERR: RESET RESULT    "
//#endif
//;
unsigned char OSGSM_init(void);



#ifdef __cplusplus
}
#endif

#endif  //__SS205_API_H
