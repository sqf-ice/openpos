/* atmioc.h - ranges for ATM-related ioctl numbers */
 
/* Written 1995-1999 by Werner Almesberger, EPFL LRC/ICA */


/*
 * See http://icawww1.epfl.ch/linux-atm/magic.html for the complete list of
 * "magic" ioctl numbers.
 */


#ifndef _LINUX_ATMIOC_H
#define _LINUX_ATMIOC_H

#include <asm/ioctl.h>
		/* everybody including atmioc.h will also need _IO{,R,W,WR} */

#define ATMIOC_PHYCOM	  0x00 /* PHY device common ioctls, globally unique */
#define ATMIOC_PHYCOM_END 0x0f
#define ATMIOC_PHYTYP	  0x10 /* PHY dev type ioctls, unique per PHY type */
#define ATMIOC_PHYTYP_END 0x2f
#define ATMIOC_PHYPRV	  0x30 /* PHY dev private ioctls, unique per driver */
#define ATMIOC_PHYPRV_END 0x4f
#define ATMIOC_SARCOM	  0x50 /* SAR device common ioctls, globally unique */
#define ATMIOC_SARCOM_END 0x50
#define ATMIOC_SARPRV	  0x60 /* SAR dev private ioctls, unique per driver */
#define ATMIOC_SARPRV_END 0x7f
#define ATMIOC_ITF	  0x80 /* Interface ioctls, globally unique */
#define ATMIOC_ITF_END	  0x8f
#define ATMIOC_BACKEND	  0x90 /* ATM generic backend ioctls, u. per backend */
#define ATMIOC_BACKEND_END 0xaf
/* 0xb0-0xbf: Reserved for future use */
#define ATMIOC_AREQUIPA	  0xc0 /* Application requested IP over ATM, glob. u. */
#define ATMIOC_LANE	  0xd0 /* LAN Emulation, globally unique */
#define ATMIOC_MPOA       0xd8 /* MPOA, globally unique */
#define	ATMIOC_CLIP	  0xe0 /* Classical IP over ATM control, globally u. */
#define	ATMIOC_CLIP_END	  0xef
#define	ATMIOC_SPECIAL	  0xf0 /* Special-purpose controls, globally unique */
#define	ATMIOC_SPECIAL_END 0xff

#endif
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 /* atmdev.h - ATM device driver declarations and various related items */
 
/* Written 1995-2000 by Werner Almesberger, EPFL LRC/ICA */
 

#ifndef LINUX_ATMDEV_H
#define LINUX_ATMDEV_H


#include <linux/atmapi.h>
#include <linux/atm.h>
#include <linux/atmioc.h>


#define ESI_LEN		6

#define ATM_OC3_PCR	(155520000/270*260/8/53)
			/* OC3 link rate:  155520000 bps
			   SONET overhead: /270*260 (9 section, 1 path)
			   bits per cell:  /8/53
			   max cell rate:  353207.547 cells/sec */
#define ATM_25_PCR	((25600000/8-8000)/54)
			/* 25 Mbps ATM cell rate (59111) */
#define ATM_OC12_PCR	(622080000/1080*1040/8/53)
			/* OC12 link rate: 622080000 bps
			   SONET overhead: /1080*1040
			   bits per cell:  /8/53
			   max cell rate:  1412830.188 cells/sec */
#define ATM_DS3_PCR	(8000*12)
			/* DS3: 12 cells in a 125 usec time slot */


#define __AAL_STAT_ITEMS \
    __HANDLE_ITEM(tx);			/* TX okay */ \
    __HANDLE_ITEM(tx_err);		/* TX errors */ \
    __HANDLE_ITEM(rx);			/* RX okay */ \
    __HANDLE_ITEM(rx_err);		/* RX errors */ \
    __HANDLE_ITEM(rx_drop);		/* RX out of memory */

struct atm_aal_stats {
#define __HANDLE_ITEM(i) int i
	__AAL_STAT_ITEMS
#undef __HANDLE_ITEM
};


struct atm_dev_stats {
	struct atm_aal_stats aal0;
	struct atm_aal_stats aal34;
	struct atm_aal_stats aal5;
} __ATM_API_ALIGN;


#define ATM_GETLINKRATE	_IOW('a',ATMIOC_ITF+1,struct atmif_sioc)
					/* get link rate */
#define ATM_GETNAMES	_IOW('a',ATMIOC_ITF+3,struct atm_iobuf)
					/* get interface names (numbers) */
#define ATM_GETTYPE	_IOW('a',ATMIOC_ITF+4,struct atmif_sioc)
					/* get interface type name */
#define ATM_GETESI	_IOW('a',ATMIOC_ITF+5,struct atmif_sioc)
					/* get interface ESI */
#define ATM_GETADDR	_IOW('a',ATMIOC_ITF+6,struct atmif_sioc)
					/* get itf's local ATM addr. list */
#define ATM_RSTADDR	_IOW('a',ATMIOC_ITF+7,struct atmif_sioc)
					/* reset itf's ATM address list */
#define ATM_ADDADDR	_IOW('a',ATMIOC_ITF+8,struct atmif_sioc)
					/* add a local ATM address */
#define ATM_DELADDR	_IOW('a',ATMIOC_ITF+9,struct atmif_sioc)
					/* remove a local ATM address */
#define ATM_GETCIRANGE	_IOW('a',ATMIOC_ITF+10,struct atmif_sioc)
					/* get connection identifier range */
#define ATM_SETCIRANGE	_IOW('a',ATMIOC_ITF+11,struct atmif_sioc)
					/* set connection identifier range */
#define ATM_SETESI	_IOW('a',ATMIOC_ITF+12,struc