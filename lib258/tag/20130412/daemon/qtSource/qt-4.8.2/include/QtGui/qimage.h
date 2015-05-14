/*
 * These are the public elements of the Linux kernel AX.25 code. A similar
 * file netrom.h exists for the NET/ROM protocol.
 */

#ifndef	AX25_KERNEL_H
#define	AX25_KERNEL_H

#include <linux/socket.h>

#define AX25_MTU	256
#define AX25_MAX_DIGIS  8

#define AX25_WINDOW	1
#define AX25_T1		2
#define AX25_N2		3
#define AX25_T3		4
#define AX25_T2		5
#define	AX25_BACKOFF	6
#define	AX25_EXTSEQ	7
#define	AX25_PIDINCL	8
#define AX25_IDLE	9
#define AX25_PACLEN	10
#define AX25_IAMDIGI	12

#define AX25_KILL	99

#define SIOCAX25GETUID		(SIOCPROTOPRIVATE+0)
#define SIOCAX25ADDUID		(SIOCPROTOPRIVATE+1)
#define SIOCAX25DELUID		(SIOCPROTOPRIVATE+2)
#define SIOCAX25NOUID		(SIOCPROTOPRIVATE+3)
#define SIOCAX25OPTRT		(SIOCPROTOPRIVATE+7)
#define SIOCAX25CTLCON		(SIOCPROTOPRIVATE+8)
#define SIOCAX25GETINFOOLD	(SIOCPROTOPRIVATE+9)
#define SIOCAX25ADDFWD		(SIOCPROTOPRIVATE+10)
#define SIOCAX25DELFWD		(SIOCPROTOPRIVATE+11)
#define SIOCAX25DEVCTL          (SIOCPROTOPRIVATE+12)
#define SIOCAX25GETINFO         (SIOCPROTOPRIVATE+13)

#define AX25_SET_RT_IPMODE	2

#define AX25_NOUID_DEFAULT	0
#define AX25_NOUID_BLOCK	1

typedef struct {
	char		ax25_call[7];	/* 6 call + SSID (shifted ascii!) */
} ax25_address;

struct sockaddr_ax25 {
	__kernel_sa_family_t sax25_family;
	ax25_address	sax25_call;
	int		sax25_ndigis;
	/* Digipeater ax25_address sets follow */
};

#define sax25_uid	sax25_ndigis

struct full_sockaddr_ax25 {
	struct sockaddr_ax25 fsa_ax25;
	ax25_address	fsa_digipeater[AX25_MAX_DIGIS];
};

struct ax25_routes_struct {
	ax25_address	port_addr;
	ax25_address	dest_addr;
	unsigned char	digi_count;
	ax25_address	digi_addr[AX25_MAX_DIGIS];
};

struct ax25_route_opt_struct {
	ax25_address	port_addr;
	ax25_address	dest_addr;
	int		cmd;
	int		arg;
};

struct ax25_ctl_struct {
        ax25_address            port_addr;
        ax25_address            source_addr;
        ax25_address            dest_addr;
        unsigned int            cmd;
        unsigned long           arg;
        unsigned char           digi_count;
        ax25_address            digi_addr[AX25_MAX_DIGIS];
};

/* this will go away. Please do not export to user land */
struct ax25_info_struct_deprecated {
	unsigned int	n2, n2count;
	unsigned int	t1, t1timer;
	unsigned int	t2, t2timer;
	unsigned int	t3, t3timer;
	unsigned int	idle, idletimer;
	unsigned int	state;
	unsigned int	rcv_q, snd_q;
};

struct ax25_info_struct {
	unsigned int	n2, n2count;
	unsigned int	t1, t1timer;
	unsigned int	t2, t2timer;
	unsigned int	t3, t3timer;
	unsigned int	idle, idletimer;
	unsigned int	state;
	unsigned int	rcv_q, snd_q;
	unsigned int	vs, vr, va, vs_max;
	unsigned int	paclen;
	unsigned int	window;
};

struct ax25_fwd_struct {
	ax25_address	port_from;
	ax25_address	port_to;
};

#endif
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       /* $Id: b1lli.h,v 1.8.8.3 2001/09/23 22:25:05 kai Exp $
 *
 * ISDN lowlevel-module for AVM B1-card.
 *
 * Copyright 1996 by Carsten Paeth (calle@calle.in-berlin.de)
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#ifndef _B1LLI_H_
#define _B1LLI_H_
/*
 * struct for loading t4 file 
 */
typedef struct avmb1_t4file {
	int len;
	unsigned char *data;
} avmb1_t4file;

typedef struct avmb1_loaddef {
	int contr;
	avmb1_t4file t4file;
} avmb1_loaddef;

typedef struct avmb1_loadandconfigdef {
	int contr;
	avmb1_t4file t4file;
        avmb1_t4file t4config; 
} avmb1_loadandconfigdef;

typedef struct avmb1_resetdef {
	int contr;
} avmb1_resetdef;

typedef struct avmb1_getdef {
	int contr;
	int cardtype;
	int cardstate;
} avmb1_getdef;

/*
 * struct for adding new cards 
 */
typedef struct avmb1_carddef {
	int port;
	int irq;
} avmb1_carddef;

#define AVM_CARDTYPE_B1		0
#define AVM_CARDTYPE_T1		1
#define AVM_CARDTYPE_M1		2
#define AVM_CARDTYPE_M2		3

typedef struct avmb1_extcarddef {
	int port;
	int irq;
        int cardtype;
        int cardnr;  /* for HEMA/T1 */
} avmb1_extcarddef;

#define	AVMB1_LOAD		0	/* load image to card */
#define AVMB1_ADDCARD		1	/* add a new card - OBSOLETE */
#define AVMB1_RESETCARD		2	/* reset a card */
#define	AVMB1_LOAD_AND_CONFIG	3	/* load image and config to card */
#define	AVMB1_ADDCARD_WITH_TYPE	4	/* add a new card, with cardtype */
#define AVMB1_GET_CARDINFO	5	/* get cardtype */
#define AVMB1_REMOVECARD	6	/* remove a card - OBSOLETE */

#define	AVMB1_REGISTERCARD_IS_OBSOLETE

#endif				/* _B1LLI_H_ */
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          /*
 * The Linux BAYCOM driver for the Baycom serial 1200 baud modem
 * and the parallel 9600 baud modem
 * (C) 1997-1998 by Thomas Sailer, HB9JNX/AE4WA
 */

#ifndef _BAYCOM_H
#define _BAYCOM_H

/* -------------------------------------------------------------------- */
/*
 * structs for the IOCTL commands
 */

struct baycom_debug_data {
	unsigned long debug1;
	unsigned long debug2;
	long debug3;
};

struct baycom_ioctl {
	int cmd;
	union {
		struct baycom_debug_data dbg;
	} data;
};

/* -------------------------------------------------------------------- */

/*
 * ioctl values change for baycom
 */
#define BAYCOMCTL_GETDEBUG       0x92

/* -------------------------------------------------------------------- */

#endif /* _BAYCOM_H */

/* --------------------------------------------------------------------- */
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            /*
 *	include/linux/bfs_fs.h - BFS data structures on disk.
 *	Copyright (C) 1999 Tigran Aivazian <tigran@veritas.com>
 */

#ifndef _LINUX_BFS_FS_H
#define _LINUX_BFS_FS_H

#include <linux/types.h>

#define BFS_BSIZE_BITS		9
#define BFS_BSIZE		(1<<BFS_BSIZE_BITS)

#define BFS_MAGIC		0x1BADFACE
#define BFS_ROOT_INO		2
#define BFS_INODES_PER_BLOCK	8

/* SVR4 vnode type values (bfs_inode->i_vtype) */
#define BFS_VDIR 2L
#define BFS_VREG 1L

/* BFS inode layout on disk */
struct bfs_inode {
	__le16 i_ino;
	__u16 i_unused;
	__le32 i_sblock;
	__le32 i_eblock;
	__le32 i_eoffset;
	__le32 i_vtype;
	__le32 i_mode;
	__le32 i_uid;
	__le32 i_gid;
	__le32 i_nlink;
	__le32 i_atime;
	__le32 i_mtime;
	__le32 i_ctime;
	__u32 i_padding[4];
};

#define BFS_NAMELEN		14	
#define BFS_DIRENT_SIZE		16
#define BFS_DIRS_PER_BLOCK	32

struct bfs_dirent {
	__le16 ino;
	char name[BFS_NAMELEN];
};

/* BFS superblock layout on disk */
struct bfs_super_block {
	__le32 s_magic;
	__le32 s_start;
	__le32 s_end;
	__le32 s_from;
	__le32 s_to;
	__s32 s_bfrom;
	__s32 s_bto;
	char  s_fsname[6];
	char  s_volume[6];
	__u32 s_padding[118];
};


#define BFS_OFF2INO(offset) \
        ((((offset) - BFS_BSIZE) / sizeof(struct bfs_inode)) + BFS_ROOT_INO)

#define BFS_INO2OFF(ino) \
	((__u32)(((ino) - BFS_ROOT_INO) * sizeof(struct bfs_inode)) + BFS_BSIZE)
#define BFS_NZFILESIZE(ip) \
        ((le32_to_cpu((ip)->i_eoffset) + 1) -  le32_to_cpu((ip)->i_sblock) * BFS_BSIZE)

#define BFS_FILESIZE(ip) \
        ((ip)->i_sblock == 0 ? 0 : BFS_NZFILESIZE(ip))

#define BFS_FILEBLOCKS(ip) \
        ((ip)->i_sblock == 0 ? 0 : (le32_to_cpu((ip)->i_eblock) + 1) -  le32_to_cpu((ip)->i_sblock))
#define BFS_UNCLEAN(bfs_sb, sb)	\
	((le32_to_cpu(bfs_sb->s_from) != -1) && (le32_to_cpu(bfs_sb->s_to) != -1) && !(sb->s_flags & MS_RDONLY))


#endif	/* _LINUX_BFS_FS_H */
                                                                                                                                          