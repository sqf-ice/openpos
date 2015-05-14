 CHIOSVOLTAG
 *    set volume tag
 */
struct changer_set_voltag {
	int	csv_type;	 /* type/unit */
	int	csv_unit;
	char    csv_voltag[36];  /* volume tag */
	int     csv_flags;
};
#define CSV_PVOLTAG   0x01       /* primary volume tag */
#define CSV_AVOLTAG   0x02       /* alternate volume tag */
#define CSV_CLEARTAG  0x04       /* clear volume tag */

/* ioctls */
#define CHIOMOVE       _IOW('c', 1,struct changer_move)
#define CHIOEXCHANGE   _IOW('c', 2,struct changer_exchange)
#define CHIOPOSITION   _IOW('c', 3,struct changer_position)
#define CHIOGPICKER    _IOR('c', 4,int)                        /* not impl. */
#define CHIOSPICKER    _IOW('c', 5,int)                        /* not impl. */
#define CHIOGPARAMS    _IOR('c', 6,struct changer_params)
#define CHIOGSTATUS    _IOW('c', 8,struct changer_element_status)
#define CHIOGELEM      _IOW('c',16,struct changer_get_element)
#define CHIOINITELEM   _IO('c',17)
#define CHIOSVOLTAG    _IOW('c',18,struct changer_set_voltag)
#define CHIOGVPARAMS   _IOR('c',19,struct changer_vendor_params)

/* ---------------------------------------------------------------------- */

/*
 * Local variables:
 * c-basic-offset: 8
 * End:
 */
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               /*****************************************************************************/

/*
 *	cdk.h  -- CDK interface definitions.
 *
 *	Copyright (C) 1996-1998  Stallion Technologies
 *	Copyright (C) 1994-1996  Greg Ungerer.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*****************************************************************************/
#ifndef	_CDK_H
#define	_CDK_H
/*****************************************************************************/

#pragma	pack(2)

/*
 *	The following set of definitions is used to communicate with the
 *	shared memory interface of the Stallion intelligent multiport serial
 *	boards. The definitions in this file are taken directly from the
 *	document titled "Generic Stackable Interface, Downloader and
 *	Communications Development Kit".
 */

/*
 *	Define the set of important shared memory addresses. These are
 *	required to initialize the board and get things started. All of these
 *	addresses are relative to the start of the shared memory.
 */
#define	CDK_SIGADDR	0x200
#define	CDK_FEATADDR	0x280
#define	CDK_CDKADDR	0x300
#define	CDK_RDYADDR	0x262

#define	CDK_ALIVEMARKER	13

/*
 *	On hardware power up the ROMs located on the EasyConnection 8/64 will
 *	fill out the following signature information into shared memory. This
 *	way the host system can quickly determine that the board is present
 *	and is operational.
 */
typedef struct cdkecpsig {
	unsigned long	magic;
	unsigned short	romver;
	unsigned short	cputype;
	unsigned char	panelid[8];
} cdkecpsig_t;

#define	ECP_MAGIC	0x21504345

/*
 *	On hardware power up the ROMs located on the ONboard, Stallion and
 *	Brumbys will fill out the following signature information into shared
 *	memory. This way the host system can quickly determine that the board
 *	is present and is operational.
 */
typedef struct cdkonbsig {
	unsigned short	magic0;
	unsigned short	magic1;
	unsigned short	magic2;
	unsigned short	magic3;
	unsigned short	romver;
	unsigned short	memoff;
	unsigned short	memseg;
	unsigned short	amask0;
	unsigned short	pic;
	unsigned short	status;
	unsigned short	btype;
	unsigned short	clkticks;
	unsigned short	clkspeed;
	unsigned short	amask1;
	unsigned short	amask2;
} cdkonbsig_t;

#define	ONB_MAGIC0	0xf2a7
#define	ONB_MAGIC1	0xa149
#define	ONB_MAGIC2	0x6352
#define	ONB_MAGIC3	0xf121

/*
 *	Define the feature area structure. The feature area is the set of
 *	startup parameters used by the slave image when it starts executing.
 *	They allow for the specification of buffer sizes, debug trace, etc.
 */
typedef struct cdkfeature {
	unsigned long	debug;
	unsigned long	banner;
	unsigned long	etype;
	unsigned long	nrdevs;
	unsigned long	brdspec;
	unsigned long	txrqsize;
	unsigned long	rxrqsize;
	unsigned long	flags;
} cdkfeature_t;

#define	ETYP_DDK	0
#define	ETYP_CDK	1

/*
 *	Define the CDK header structure. This is the info that the slave
 *	environment sets up after it has been downloaded and started. It
 *	essentially provides a memory map for the shared memory interface.
 */
typedef struct cdkhdr {
	unsigned short	command;
	unsigned short	status;
	unsigned short	port;
	unsigned short	mode;
	unsigned long	cmd_buf[14];
	unsigned short	alive_cnt;
	unsigned short	intrpt_mode;
	unsigned char	intrpt_id[8];
	unsigned char	ver_release;
	unsigned char	ver_modification;
	unsigned char	ver_fix;
	unsigned char	deadman_restart;
	unsigned short	deadman;
	unsigned short	nrdevs;
	unsigned long	memp;
	unsigned long	hostp;
	unsigned long	slavep;
	unsigned char	hostreq;
	unsigned char	slavereq;
	unsigned char	cmd_reserved[30];
} cdkhdr_t;

#define	MODE_DDK	0
#define	MODE_CDK	1

#define	IMD_INTR	0x0
#define	IMD_PPINTR	0x1
#define	IMD_POLL	0xff

/*
 *	Define the memory mapping structure. This structure is pointed to by
 *	the memp field in the stlcdkhdr struct. As many as these structures
 *	as required are laid out in shared memory to define how the rest of
 *	shared memory is divided up. There will be one for each port.
 */
typedef struct cdkmem {
	unsigned short	dtype;
	unsigned long	offset;
} cdkmem_t;

#define	TYP_UNDEFINED	0x0
#define	TYP_ASYNCTRL	0x1
#define	TYP_ASYNC	0x20
#define	TYP_PARALLEL	0x40
#define	TYP_SYNCX21	0x60

/*****************************************************************************/

/*
 *	Following is a set of defines and structures used to actually deal
 *	with the serial ports on the board. Firstly is the set of commands
 *	that can be applied to ports.
 */
#define	ASYCMD		(((unsigned long) 'a') << 8)

#define	A_NULL		(ASYCMD | 0)
#define	A_FLUSH		(ASYCMD | 1)
#define	A_BREAK		(ASYCMD | 2)
#define	A_GETPORT	(ASYCMD | 3)
#define	A_SETPORT	(ASYCMD | 4)
#define	A_SETPORTF	(ASYCMD | 5)
#define	A_SETPORTFTX	(ASYCMD | 6)
#define	A_SETPORTFRX	(ASYCMD | 7)
#define	A_GETSIGNALS	(ASYCMD | 8)
#define	A_SETSIGNALS	(ASYCMD | 9)
#define	A_SETSIGNALSF	(ASYCMD | 10)
#define	A_SETSIGNALSFTX	(ASYCMD | 11)
#define	A_SETSIGNALSFRX	(ASYCMD | 12)
#define	A_GETNOTIFY	(ASYCMD | 13)
#define	A_SETNOTIFY	(ASYCMD | 14)
#define	A_NOTIFY	(ASYCMD | 15)
#define	A_PORTCTRL	(ASYCMD | 16)
#define	A_GETSTATS	(ASYCMD | 17)
#define	A_RQSTATE	(ASYCMD | 18)
#define	A_FLOWSTATE	(ASYCMD | 19)
#define	A_CLEARSTATS	(ASYCMD | 20)

/*
 *	Define those arguments used for simple commands.
 */
#define	FLUSHRX		0x1
#define	FLUSHTX		0x2

#define	BREAKON		-1
#define	BREAKOFF	-2

/*
 *	Define the port setting structure, and all those defines that go along
 *	with it. Basically this structure defines the characteristics of this
 *	port: baud rate, chars, parity, input/output char cooking etc.
 */
typedef struct asyport {
	unsigned long	baudout;
	unsigned long	baudin;
	unsigned long	iflag;
	unsigned long	oflag;
	unsigned long	lflag;
	unsigned long	pflag;
	unsigned long	flow;
	unsigned long	spare1;
	unsigned short	vtime;
	unsigned short	vmin;
	unsigned short	txlo;
	unsigned short	txhi;
	unsigned short	rxlo;
	unsigned short	rxhi;
	unsigned short	rxhog;
	unsigned short	spare2;
	unsigned char	csize;
	unsigned char	stopbs;
	unsigned char	parity;
	unsigned char	stopin;
	unsigned char	startin;
	unsigned char	stopout;
	unsigned char	startout;
	unsigned char	parmark;
	unsigned char	brkmark;
	unsigned char	cc[11];
} asyport_t;

#define	PT_STOP1	0x0
#define	PT_STOP15	0x1
#define	PT_STOP2	0x2

#define	PT_NOPARITY	0x0
#define	PT_ODDPARITY	0x1
#define	PT_EVENPARITY	0x2
#define	PT_MARKPARITY	0x3
#define	PT_SPACEPARITY	0x4

#define	F_NONE		0x0
#define	F_IXON		0x1
#define	F_IXOFF		0x2
#define	F_IXANY		0x4
#define	F_IOXANY	0x8
#define	F_RTSFLOW	0x10
#define	F_CTSFLOW	0x20
#define	F_DTRFLOW	0x40
#define	F_DCDFLOW	0x80
#define	F_DSROFLOW	0x100
#define	F_DSRIFLOW	0x200

#define	FI_NORX		0x1
#define	FI_RAW		0x2
#define	FI_ISTRIP	0x4
#define	FI_UCLC		0x8
#define	FI_INLCR	0x10
#define	FI_ICRNL	0x20
#define	FI_IGNCR	0x40
#define	FI_IGNBREAK	0x80
#define	FI_DSCRDBREAK	0x100
#define	FI_1MARKBREAK	0x200
#define	FI_2MARKBREAK	0x400
#define	FI_XCHNGBREAK	0x800
#define	FI_IGNRXERRS	0x1000
#define	FI_DSCDRXERRS	0x2000
#define	FI_1MARKRXERRS	0x4000
#define	FI_2MARKRXERRS	0x8000
#define	FI_XCHNGRXERRS	0x10000
#define	FI_DSCRDNULL	0x20000

#define	FO_OLCUC	0x1
#define	FO_ONLCR	0x2
#define	FO_OOCRNL	0x4
#define	FO_ONOCR	0x8
#define	FO_ONLRET	0x10
#define	FO_ONL		0x20
#define	FO_OBS		0x40
#define	FO_OVT		0x80
#define	FO_OFF		0x100
#define	FO_OTAB1	0x200
#define	FO_OTAB2	0x400
#define	FO_OTAB3	0x800
#define	FO_OCR1		0x1000
#define	FO_OCR2		0x2000
#define	FO_OCR3		0x4000
#define	FO_OFILL	0x8000
#define	FO_ODELL	0x10000

#define	P_RTSLOCK	0x1
#define	P_CTSLOCK	0x2
#define	P_MAPRTS	0x4
#define	P_MAPCTS	0x8
#define	P_LOOPBACK	0x10
#define	P_DTRFOLLOW	0x20
#define	P_FAKEDCD	0x40

#define	P_RXIMIN	0x10000
#define	P_RXITIME	0x20000
#define	P_RXTHOLD	0x40000

/*
 *	Define a structure to communicate serial port signal and data state
 *	information.
 */
typedef struct asysigs {
	unsigned long	data;
	unsigned long	signal;
	unsigned long	sigvalue;
} asysigs_t;

#define	DT_TXBUSY	0x1
#define	DT_TXEMPTY	0x2
#define	DT_TXLOW	0x4
#define	DT_TXHIGH	0x8
#define	DT_TXFULL	0x10
#define	DT_TXHOG	0x20
#define	DT_TXFLOWED	0x40
#define	DT_TXBREAK	0x80

#define	DT_RXBUSY	0x100
#define	DT_RXEMPTY	0x200
#define	DT_RXLOW	0x400
#define	DT_RXHIGH	0x800
#define	DT_RXFULL	0x1000
#define	DT_RXHOG	0x2000
#define	DT_RXFLOWED	0x4000
#define	DT_RXBREAK	0x8000

#define	SG_DTR		0x1
#define	SG_DCD		0x2
#define	SG_RTS		0x4
#define	SG_CTS		0x8
#define	SG_DSR		0x10
#define	SG_RI		0x20

/*
 *	Define the notification setting structure. This is used to tell the
 *	port what events we want to be informed about. Fields here use the
 *	same defines as for the asysigs structure above.
 */
typedef struct asynotify {
	unsigned long	ctrl;
	unsigned long	data;
	unsigned long	signal;
	unsigned long	sigvalue;
} asynotify_t;

/*
 *	Define the port control structure. It is used to do fine grain
 *	contr