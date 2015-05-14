 dynamic loading of these - for now,
 * they're just being used to share the ATMIOC_BACKEND ioctls
 */
#define ATM_BACKEND_RAW		0	
#define ATM_BACKEND_PPP		1	/* PPPoATM - RFC2364 */
#define ATM_BACKEND_BR2684	2	/* Bridged RFC1483/2684 */

/* for ATM_GETTYPE */
#define ATM_ITFTYP_LEN	8	/* maximum length of interface type name */

/*
 * Loopback modes for ATM_{PHY,SAR}_{GET,SET}LOOP
 */

/* Point of loopback				CPU-->SAR-->PHY-->line--> ... */
#define __ATM_LM_NONE	0	/* no loop back     ^     ^     ^      ^      */
#define __ATM_LM_AAL	1	/* loop back PDUs --'     |     |      |      */
#define __ATM_LM_ATM	2	/* loop back ATM cells ---'     |      |      */
/* RESERVED		4	loop back on PHY side  ---'		      */
#define __ATM_LM_PHY	8	/* loop back bits (digital) ----'      |      */
#define __ATM_LM_ANALOG 16	/* loop back the analog signal --------'      */

/* Direction of loopback */
#define __ATM_LM_MKLOC(n)	((n))	    /* Local (i.e. loop TX to RX) */
#define __ATM_LM_MKRMT(n)	((n) << 8)  /* Remote (i.e. loop RX to TX) */

#define __ATM_LM_XTLOC(n)	((n) & 0xff)
#define __ATM_LM_XTRMT(n)	(((n) >> 8) & 0xff)

#define ATM_LM_NONE	0	/* no loopback */

#define ATM_LM_LOC_AAL	__ATM_LM_MKLOC(__ATM_LM_AAL)
#define ATM_LM_LOC_ATM	__ATM_LM_MKLOC(__ATM_LM_ATM)
#define ATM_LM_LOC_PHY	__ATM_LM_MKLOC(__ATM_LM_PHY)
#define ATM_LM_LOC_ANALOG __ATM_LM_MKLOC(__ATM_LM_ANALOG)

#define ATM_LM_RMT_AAL	__ATM_LM_MKRMT(__ATM_LM_AAL)
#define ATM_LM_RMT_ATM	__ATM_LM_MKRMT(__ATM_LM_ATM)
#define ATM_LM_RMT_PHY	__ATM_LM_MKRMT(__ATM_LM_PHY)
#define ATM_LM_RMT_ANALOG __ATM_LM_MKRMT(__ATM_LM_ANALOG)

/*
 * Note: ATM_LM_LOC_* and ATM_LM_RMT_* can be combined, provided that
 * __ATM_LM_XTLOC(x) <= __ATM_LM_XTRMT(x)
 */


struct atm_iobuf {
	int length;
	void *buffer;
};

/* for ATM_GETCIRANGE / ATM_SETCIRANGE */

#define ATM_CI_MAX      -1              /* use maximum range of VPI/VCI */
 
struct atm_cirange {
	signed char	vpi_bits;	/* 1..8, ATM_CI_MAX (-1) for maximum */
	signed char	vci_bits;	/* 1..16, ATM_CI_MAX (-1) for maximum */
};

/* for ATM_SETSC; actually taken from the ATM_VF number space */

#define ATM_SC_RX	1024		/* enable RX single-copy */
#define ATM_SC_TX	2048		/* enable TX single-copy */

#define ATM_BACKLOG_DEFAULT 32 /* if we get more, we're likely to time out
				  anyway */

/* MF: change_qos (Modify) flags */

#define ATM_MF_IMMED	 1	/* Block until change is effective */
#define ATM_MF_INC_RSV	 2	/* Change reservation on increase */
#define ATM_MF_INC_SHP	 4	/* Change shaping on increase */
#define ATM_MF_DEC_RSV	 8	/* Change reservation on decrease */
#define ATM_MF_DEC_SHP	16	/* Change shaping on decrease */
#define ATM_MF_BWD	32	/* Set the backward direction parameters */

#define ATM_MF_SET	(ATM_MF_INC_RSV | ATM_MF_INC_SHP | ATM_MF_DEC_RSV | \
			  ATM_MF_DEC_SHP | ATM_MF_BWD)

/*
 * ATM_VS_* are used to express VC state in a human-friendly way.
 */

#define ATM_VS_IDLE	0	/* VC is not used */
#define ATM_VS_CONNECTED 1	/* VC is connected */
#define ATM_VS_CLOSING	2	/* VC is closing */
#define ATM_VS_LISTEN	3	/* VC is listening for incoming setups */
#define ATM_VS_INUSE	4	/* VC is in use (registered with atmsigd) */
#define ATM_VS_BOUND	5	/* VC is bound */

#define ATM_VS2TXT_MAP \
    "IDLE", "CONNECTED", "CLOSING", "LISTEN", "INUSE", "BOUND"

#define ATM_VF2TXT_MAP \
    "ADDR",	"READY",	"PARTIAL",	"REGIS", \
    "RELEASED", "HASQOS",	"LISTEN",	"META", \
    "256",	"512",		"1024",		"2048", \
    "SESSION",	"HASSAP",	"BOUND",	"CLOSE"



#endif
                                                                             