/*
 * linux/can.h
 *
 * Definitions for CAN network layer (socket addr / CAN frame / CAN filter)
 *
 * Authors: Oliver Hartkopp <oliver.hartkopp@volkswagen.de>
 *          Urs Thuermann   <urs.thuermann@volkswagen.de>
 * Copyright (c) 2002-2007 Volkswagen Group Electronic Research
 * All rights reserved.
 *
 */

#ifndef CAN_H
#define CAN_H

#include <linux/types.h>
#include <linux/socket.h>

/* controller area network (CAN) kernel definitions */

/* special address description flags for the CAN_ID */
#define CAN_EFF_FLAG 0x80000000U /* EFF/SFF is set in the MSB */
#define CAN_RTR_FLAG 0x40000000U /* remote transmission request */
#define CAN_ERR_FLAG 0x20000000U /* error frame */

/* valid bits in CAN ID for frame formats */
#define CAN_SFF_MASK 0x000007FFU /* standard frame format (SFF) */
#define CAN_EFF_MASK 0x1FFFFFFFU /* extended frame format (EFF) */
#define CAN_ERR_MASK 0x1FFFFFFFU /* omit EFF, RTR, ERR flags */

/*
 * Controller Area Network Identifier structure
 *
 * bit 0-28	: CAN identifier (11/29 bit)
 * bit 29	: error frame flag (0 = data frame, 1 = error frame)
 * bit 30	: remote transmission request flag (1 = rtr frame)
 * bit 31	: frame format flag (0 = standard 11 bit, 1 = extended 29 bit)
 */
typedef __u32 canid_t;

/*
 * Controller Area Network Error Frame Mask structure
 *
 * bit 0-28	: error class mask (see include/linux/can/error.h)
 * bit 29-31	: set to zero
 */
typedef __u32 can_err_mask_t;

/**
 * struct can_frame - basic CAN frame structure
 * @can_id:  the CAN ID of the frame and CAN_*_FLAG flags, see above.
 * @can_dlc: the data length field of the CAN frame
 * @data:    the CAN frame payload.
 */
struct can_frame {
	canid_t can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
	__u8    can_dlc; /* data length code: 0 .. 8 */
	__u8    data[8] __attribute__((aligned(8)));
};

/* particular protocols of the protocol family PF_CAN */
#define CAN_RAW		1 /* RAW sockets */
#define CAN_BCM		2 /* Broadcast Manager */
#define CAN_TP16	3 /* VAG Transport Protocol v1.6 */
#define CAN_TP20	4 /* VAG Transport Protocol v2.0 */
#define CAN_MCNET	5 /* Bosch MCNet */
#define CAN_ISOTP	6 /* ISO 15765-2 Transport Protocol */
#define CAN_NPROTO	7

#define SOL_CAN_BASE 100

/**
 * struct sockaddr_can - the sockaddr structure for CAN sockets
 * @can_family:  address family number AF_CAN.
 * @can_ifindex: CAN network interface index.
 * @can_addr:    protocol specific address information
 */
struct sockaddr_can {
	__kernel_sa_family_t can_family;
	int         can_ifindex;
	union {
		/* transport protocol class address information (e.g. ISOTP) */
		struct { canid_t rx_id, tx_id; } tp;

		/* reserved for future CAN protocols address information */
	} can_addr;
};

/**
 * struct can_filter - CAN ID based filter in can_register().
 * @can_id:   relevant bits of CAN ID which are not masked out.
 * @can_mask: CAN mask (see description)
 *
 * Description:
 * A filter matches, when
 *
 *          <received_can_id> & mask == can_id & mask
 *
 * The filter can be inverted (CAN_INV_FILTER bit set in can_id) or it can
 * filter for error frames (CAN_ERR_FLAG bit set in mask).
 */
struct can_filter {
	canid_t can_id;
	canid_t can_mask;
};

#define CAN_INV_FILTER 0x20000000U /* to be set in can_filter.can_id */

#endif /* CAN_H */
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   /*
 * This is <linux/capability.h>
 *
 * Andrew G. Morgan <morgan@kernel.org>
 * Alexander Kjeldaas <astor@guardian.no>
 * with help from Aleph1, Roland Buresund and Andrew Main.
 *
 * See here for the libcap library ("POSIX draft" compliance):
 *
 * ftp://www.kernel.org/pub/linux/libs/security/linux-privs/kernel-2.6/
 */

#ifndef _LINUX_CAPABILITY_H
#define _LINUX_CAPABILITY_H

#include <linux/types.h>

struct task_struct;

/* User-level do most of the mapping between kernel and user
   capabilities based on the version tag given by the kernel. The
   kernel might be somewhat backwards compatible, but don't bet on
   it. */

/* Note, cap_t, is defined by POSIX (draft) to be an "opaque" pointer to
   a set of three capability sets.  The transposition of 3*the
   following structure to such a composite is better handled in a user
   library since the draft standard requires the use of malloc/free
   etc.. */

#define _LINUX_CAPABILITY_VERSION_1  0x19980330
#define _LINUX_CAPABILITY_U32S_1     1

#define _LINUX_CAPABILITY_VERSION_2  0x20071026  /* deprecated - use v3 */
#define _LINUX_CAPABILITY_U32S_2     2

#define _LINUX_CAPABILITY_VERSION_3  0x20080522
#define _LINUX_CAPABILITY_U32S_3     2

typedef struct __user_cap_header_struct {
	__u32 version;
	int pid;
} *cap_user_header_t;

typedef struct __user_cap_data_struct {
        __u32 effective;
        __u32 permitted;
        __u32 inheritable;
} *cap_user_data_t;


#define VFS_CAP_REVISION_MASK	0xFF000000
#define VFS_CAP_REVISION_SHIFT	24
#define VFS_CAP_FLAGS_MASK	~VFS_CAP_REVISION_MASK
#define VFS_CAP_FLAGS_EFFECTIVE	0x000001

#define VFS_CAP_REVISION_1	0x01000000
#define VFS_CAP_U32_1           1
#define XATTR_CAPS_SZ_1         (sizeof(__le32)*(1 + 2*VFS_CAP_U32_1))

#define VFS_CAP_REVISION_2	0x02000000
#define VFS_CAP_U32_2           2
#define XATTR_CAPS_SZ_2         (sizeof(__le32)*(1 + 2*VFS_CAP_U32_2))

#define XATTR_CAPS_SZ           XATTR_CAPS_SZ_2
#define VFS_CAP_U32             VFS_CAP_U32_2
#define VFS_CAP_REVISION	VFS_CAP_REVISION_2

struct vfs_cap_data {
	__le32 magic_etc;            /* Little endian */
	struct {
		__le32 permitted;    /* Little endian */
		__le32 inheritable;  /* Little endian */
	} data[VFS_CAP_U32];
};


/*
 * Backwardly compatible definition for source code - trapped in a
 * 32-bit world. If you find you need this, please consider using
 * libcap to untrap yourself...
 */
#define _LINUX_CAPABILITY_VERSION  _LINUX_CAPABILITY_VERSION_1
#define _LINUX_CAPABILITY_U32S     _LINUX_CAPABILITY_U32S_1



/**
 ** POSIX-draft defined capabilities.
 **/

/* In a system with the [_POSIX_CHOWN_RESTRICTED] option defined, this
   overrides the restriction of changing file ownership and group
   ownership. */

#define CAP_CHOWN            0

/* Override all DAC access, including ACL execute access if
   [_POSIX_ACL] is defined. Excluding DAC access covered by
   CAP_LINUX_IMMUTABLE. */

#define CAP_DAC_OVERRIDE     1

/* Overrides all DAC restrictions regarding read and search on files
   and directories, including ACL restrictions if [_POSIX_ACL] is
   defined. Excluding DAC access covered by CAP_LINUX_IMMUTABLE. */

#define CAP_DAC_READ_SEARCH  2

/* Overrides all restrictions about allowed operations on files, where
   file owner ID must be equal to the user ID, except where CAP_FSETID
   is applicable. It doesn't override MAC and DAC restrictions. */

#define CAP_FOWNER           3

/* Overrides the following restrictions that the effective user ID
   shall match the file owner ID when setting the S_ISUID and S_ISGID
   bits on that file; that the effective group ID (or one of the
   supplementary group IDs) shall match the file owner ID when setting
   the S_ISGID bit on that file; that the S_ISUID and S_ISGID bits are
   cleared on successful return from chown(2) (not implemented). */

#define CAP_FSETID           4

/* Overrides the restriction that the real or effective user ID of a
   process sending a signal must match the real or effective user ID
   of the process receiving the signal. */

#define CAP_KILL             5

/* Allows setgid(2) manipulation */
/* Allows setgroups(2) */
/* Allows forged gids on socket credentials passing. */

#define CAP_SETGID           6

/* Allows set*uid(2) manipulation (including fsuid). */
/* Allows forged pids on socket credentials passing. */

#define CAP_SETUID           7


/**
 ** Linux-specific capabilities
 **/

/* Without VFS support for capabilities:
 *   Transfer any capability in your permitted set to any pid,
 *   remove any capability in your permitted set from any pid
 * With VFS support for capabilities (neither of above, but)
 *   Add any capability from current's capability bounding set
 *       to the current process' inheritable set
 *   Allow taking bits out of capability bounding set
 *   Allow modification of the securebits for a process
 */

#define CAP_SETPCAP          8

/* Allow modification of S_IMMUTABLE and S_APPEND file attributes */

#define CAP_LINUX_IMMUTABLE  9

/* Allows binding to TCP/UDP sockets below 1024 */
/* Allows binding to ATM VCIs below 32 */

#define CAP_NET_BIND_SERVICE 10

/* Allow broadcasting, listen to multicast */

#define CAP_NET_BROADCAST    11

/* Allow interface configuration */
/* Allow administration of IP firewall, masquerading and accounting */
/* Allow setting debug option on sockets */
/* Allow modification of routing tables */
/* Allow setting arbitrary process / process group ownership on
   sockets */
/* Allow binding to any address for transparent proxying (also via NET_RAW) */
/* Allow setting TOS (type of service) */
/* Allow setting promiscuous mode */
/* Allow clearing driver statistics */
/* Allow multicasting */
/* Allow read/write of device-specific registers */
/* Allow activation of ATM control sockets */

#define CAP_NET_ADMIN        12

/* Allow use of RAW sockets */
/* Allow use of PACKET sockets */
/* Allow binding to any address for transparent proxying (also via NET_ADMIN) */

#define CAP_NET_RAW          13

/* Allow locking of shared memory segments */
/* Allow mlock and mlockall (which doesn't really have anything to do
   with IPC) */

#define CAP_IPC_LOCK         14

/* Override IPC ownership checks */

#define CAP_IPC_OWNER        15

/* Insert and remove kernel modules - modify kernel without limit */
#define CAP_SYS_MODULE       16

/* Allow ioperm/iopl access */
/* Allow sending USB messages to any device via /proc/bus/usb */

#define CAP_SYS_RAWIO        17

/* Allow use of chroot() */

#define CAP_SYS_CHROOT       18

/* Allow ptrace() of any process */

#define CAP_SYS_PTRACE       19

/* Allow configuration of process accounting */

#define CAP_SYS_PACCT        20

/* Allow configuration of the secure attention key */
/* Allow administration of the random device */
/* Allow examination and configuration of disk quotas */
/* Allow setting the domainname */
/* Allow setting the hostname */
/* Allow calling bdflush() */
/* Allow mount() and umount(), setting up new smb connection */
/* Allow some autofs root ioctls */
/* Allow nfsservctl */
/* Allow VM86_REQUEST_IRQ */
/* Allow to read/write pci config on alpha */
/* Allow irix_prctl on mips (setstacksize) */
/* Allow flushing all cache on m68k (sys_cacheflush) */
/* Allow removing semaphores */
/* Used instead of CAP_CHOWN to "chown" IPC message queues, semaphores
   and shared memory */
/* Allow locking/unlocking of shared memory segment */
/* Allow turning swap on/off */
/* Allow forged pids on socket credentials passing */
/* Allow setting readahead and flushing buffers on block devices */
/* Allow setting geometry in floppy driver */
/* Allow turning DMA on/off in xd driver */
/* Allow administration of md devices (mostly the above, but some
   extra ioctls) */
/* Allow tuning the ide driver */
/* Allow access to the nvram device */
/* Allow administration of apm_bios, serial and bttv (TV) device */
/* Allow manufacturer commands in isdn CAPI support driver */
/* Allow reading non-standardized portions of pci configuration space */
/* Allow DDI debug ioctl on sbpcd driver */
/* Allow setting up serial ports */
/* Allow sending raw qic-117 commands */
/* Allow enabling/disabling tagged queuing on SCSI controllers and sending
   arbitrary SCSI commands */
/* Allow setting encryption key on loopback filesystem */
/* Allow setting zone reclaim policy */

#define CAP_SYS_ADMIN        21

/* Allow use of reboot() */

#define CAP_SYS_BOOT         22

/* Allow raising priority and setting priority on other (different
   UID) processes */
/* Allow use of FIFO and round-robin (realtime) scheduling on own
   processes and setting the scheduling algorithm used by another
   process. */
/* Allow setting cpu affinity on other processes */

#define CAP_SYS_NICE         23

/* Override resource limits. Set resource limits. */
/* Override quota limits. */
/* Override reserved space on ext2 filesystem */
/* Modify data journaling mode on ext3 filesystem (uses journaling
   resources) */
/* NOTE: ext2 honors fsuid when checking for resource overrides, so
   you can override using fsuid too */
/* Override size restrictions on IPC message queues */
/* Allow more than 64hz interrupts from the real-time clock */
/* Override max number of consoles on console allocation */
/* Override max number of keymaps */

#define CAP_SYS_RESOURCE     24

/* Allow manipulation of system clock */
/* Allow irix_stime on mips */
/* Allow setting the real-time clock */

#define CAP_SYS_TIME         25

/* Allow configuration of tty devices */
/* Allow vhangup() of tty */

#define CAP_SYS_TTY_CONFIG   26

/* Allow the privileged aspects of mknod() */

#define CAP_MKNOD            27

/* Allow taking of leases on files */

#define CAP_LEASE            28

#define CAP_AUDIT_WRITE      29

#define CAP_AUDIT_CONTROL    30

#define CAP_SETFCAP	     31

/* Override MAC access.
   The base kernel enforces no MAC policy.
   An LSM may enforce a MAC policy, and if it does and it chooses
   to implement capability based overrides of that policy, this is
   the capability it should use to do so. */

#define CAP_MAC_OVERRIDE     32

/* Allow MAC configuration or state changes.
   The base kernel requires no MAC configuration.
   An LSM may enforce a MAC policy, and if it does and it chooses
   to implement capability based checks on modifications to that
   policy or the data required to maintain it, this is the
   capability it should use to do so. */

#define CAP_MAC_ADMIN        33

/* Allow configuring the kernel's syslog (printk behaviour) */

#define CAP_SYSLOG           34

/* Allow triggering something that will wake the system */

#define CAP_WAKE_ALARM            35


#define CAP_LAST_CAP         CAP_WAKE_ALARM

#define cap_valid(x) ((x) >= 0 && (x) <= CAP_LAST_CAP)

/*
 * Bit location of each capability (used by user-space library and kernel)
 */

#define CAP_TO_INDEX(x)     ((x) >> 5)        /* 1 << 5 == bits in __u32 */
#define CAP_TO_MASK(x)      (1 << ((x) & 31)) /* mask for indexed __u32 */


#endif /* !_LINUX_CAPABILITY_H */
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        /* $Id: capi.h,v 1.4.6.1 2001/09/23 22:25:05 kai Exp $
 * 
 * CAPI 2.0 Interface for Linux
 * 
 * Copyright 1997 by Carsten Paeth (calle@calle.in-berlin.de)
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#ifndef __LINUX_CAPI_H__
#define __LINUX_CAPI_H__

#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/kernelcapi.h>

/*
 * CAPI_REGISTER
 */

typedef struct capi_register_params {	/* CAPI_REGISTER */
	__u32 level3cnt;	/* No. of simulatneous user data connections */
	__u32 datablkcnt;	/* No. of buffered data messages */
	__u32 datablklen;	/* Size of buffered data messages */
} capi_register_params;

#define	CAPI_REGISTER	_IOW('C',0x01,struct capi_register_params)

/*
 * CAPI_GET_MANUFACTURER
 */

#define CAPI_MANUFACTURER_LEN		64

#define	CAPI_GET_MANUFACTURER	_IOWR('C',0x06,int)	/* broken: wanted size 64 (CAPI_MANUFACTURER_LEN) */

/*
 * CAPI_GET_VERSION
 */

typedef struct capi_version {
	__u32 majorversion;
	__u32 minorversion;
	__u32 majormanuversion;
	__u32 minormanuversion;
} capi_version;

#define CAPI_GET_VERSION	_IOWR('C',0x07,struct capi_version)

/*
 * CAPI_GET_SERIAL
 */

#define CAPI_SERIAL_LEN		8
#define CAPI_GET_SERIAL		_IOWR('C',0x08,int)	/* broken: wanted size 8 (CAPI_SERIAL_LEN) */

/*
 * CAPI_GET_PROFILE
 */

typedef struct capi_profile {
	__u16 ncontroller;	/* number of installed controller */
	__u16 nbchannel;	/* number of B-Channels */
	__u32 goptions;		/* global options */
	__u32 support1;		/* B1 protocols support */
	__u32 support2;		/* B2 protocols support */
	__u32 support3;		/* B3 protocols support */
	__u32 reserved[6];	/* reserved */
	__u32 manu[5];		/* manufacturer specific information */
} capi_profile;

#define CAPI_GET_PROFILE	_IOWR('C',0x09,struct capi_profile)

typedef struct capi_manufacturer_cmd {
	unsigned long cmd;
	void *data;
} capi_manufacturer_cmd;

/*
 * CAPI_MANUFACTURER_CMD
 */

#define CAPI_MANUFACTURER_CMD	_IOWR('C',0x20, struct capi_manufacturer_cmd)

/*
 * CAPI_GET_ERRCODE
 * capi errcode is set, * if read, write, or ioctl returns EIO,
 * ioctl returns errcode directly, and in arg, if != 0
 */

#define CAPI_GET_ERRCODE	_IOR('C',0x21, __u16)

/*
 * CAPI_INSTALLED
 */
#define CAPI_INSTALLED		_IOR('C',0x22, __u16)


/*
 * member contr is input for
 * CAPI_GET_MANUFACTURER, CAPI_VERSION, CAPI_GET_SERIAL
 * and CAPI_GET_PROFILE
 */
typedef union capi_ioctl_struct {
	__u32 contr;
	capi_register_params rparams;
	__u8 manufacturer[CAPI_MANUFACTURER_LEN];
	capi_version version;
	__u8 serial[CAPI_SERIAL_LEN];
	capi_profile profile;
	capi_manufacturer_cmd cmd;
	__u16 errcode;
} capi_ioctl_struct;

/*
 * Middleware extension
 */

#define CAPIFLAG_HIGHJACKING	0x0001

#define CAPI_GET_FLAGS		_IOR('C',0x23, unsigned)
#define CAPI_SET_FLAGS		_IOR('C',0x24, unsigned)
#define CAPI_CLR_FLAGS		_IOR('C',0x25, unsigned)

#define CAPI_NCCI_OPENCOUNT	_IOR('C',0x26, unsigned)

#define CAPI_NCCI_GETUNIT	_IOR('C',0x27, unsigned)

#endif				/* __LINUX_CAPI_H__ */
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               #ifndef CCISS_DEFS_H
#define CCISS_DEFS_H

#include <linux/types.h>

/* general boundary definitions */
#define SENSEINFOBYTES          32 /* note that this value may vary
				      between host implementations */

/* Command Status value */
#define CMD_SUCCESS             0x0000
#define CMD_TARGET_STATUS       0x0001
#define CMD_DATA_UNDERRUN       0x0002
#define CMD_DATA_OVERRUN        0x0003
#define CMD_INVALID             0x0004
#define CMD_PROTOCOL_ERR        0x0005
#define CMD_HARDWARE_ERR        0x0006
#define CMD_CONNECTION_LOST     0x0007
#define CMD_ABORTED             0x0008
#define CMD_ABORT_FAILED        0x0009
#define CMD_UNSOLICITED_ABORT   0x000A
#define CMD_TIMEOUT             0x000B
#define CMD_UNABORTABLE		0x000C

/* transfer direction */
#define XFER_NONE               0x00
#define XFER_WRITE              0x01
#define XFER_READ               0x02
#define XFER_RSVD             