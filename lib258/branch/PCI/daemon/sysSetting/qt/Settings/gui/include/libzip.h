TIME (__SID |32) /* Set the time the STREAM head will delay when
				   a STREAM is closing and there is data on
				   the write queues.  */
#define I_GETCLTIME (__SID |33) /* Get current value for closing timeout.  */
#define I_CANPUT    (__SID |34) /* Check if a certain band is writable.  */


/* Used in `I_LOOK' request.  */
#define FMNAMESZ	8	/* compatibility w/UnixWare/Solaris.  */

/* Flush options.  */
#define FLUSHR		0x01	/* Flush read queues.  */
#define FLUSHW		0x02	/* Flush write queues.  */
#define FLUSHRW		0x03	/* Flush read and write queues.  */
#ifdef __USE_GNU
# define FLUSHBAND	0x04	/* Flush only specified band.  */
#endif

/* Possible arguments for `I_SETSIG'.  */
#define S_INPUT		0x0001	/* A message, other than a high-priority
				   message, has arrived.  */
#define S_HIPRI		0x0002	/* A high-priority message is present.  */
#define S_OUTPUT	0x0004	/* The write queue for normal data is no longer
				   full.  */
#define S_MSG		0x0008	/* A STREAMS signal message that contains the
				   SIGPOLL signal reaches the front of the
				   STREAM head read queue.  */
#define S_ERROR		0x0010	/* Notification of an error condition.  */
#define S_HANGUP	0x0020	/* Notification of a hangup.  */
#define S_RDNORM	0x0040	/* A normal message has arrived.  */
#define S_WRNORM	S_OUTPUT
#define S_RDBAND	0x0080	/* A message with a non-zero priority has
				   arrived.  */
#define S_WRBAND	0x0100	/* The write queue for a non-zero priority
				   band is no longer full.  */
#define S_BANDURG	0x0200	/* When used in conjunction with S_RDBAND,
				   SIGURG is generated instead of SIGPOLL when
				   a priority message reaches the front of the
				   STREAM head read queue.  */

/* Option for `I_PEEK'.  */
#define RS_HIPRI	0x01	/* Only look for high-priority messages.  */

/* Options for `I_SRDOPT'.  */
#define RNORM		0x0000	/* Byte-STREAM mode, the default.  */
#define RMSGD		0x0001	/* Message-discard mode.   */
#define RMSGN		0x0002	/* Message-nondiscard mode.   */
#define RPROTDAT	0x0004	/* Deliver the control part of a message as
				   data.  */
#define RPROTDIS	0x0008	/* Discard the control part of a message,
				   delivering any data part.  */
#define RPROTNORM	0x0010	/* Fail `read' with EBADMSG if a message
				   containing a control part is at the front
				   of the STREAM head read queue.  */
#ifdef __USE_GNU
# define RPROTMASK	0x001C	/* The RPROT bits */
#endif

/* Possible mode for `I_SWROPT'.  */
#define SNDZERO		0x001	/* Send a zero-length message downstream when a
				   `write' of 0 bytes occurs.  */
#ifdef __USE_GNU
# define SNDPIPE	0x002	/* Send SIGPIPE on write and putmsg if
				   sd_werror is set.  */
#endif

/* Arguments for `I_ATMARK'.  */
#define ANYMARK		0x01	/* Check if the message is marked.  */
#define LASTMARK	0x02	/* Check if the message is the last one marked
				   on the queue.  */

/* Argument for `I_UNLINK'.  */
#ifdef __USE_GNU
# define MUXID_ALL	(-1)	/* Unlink all STREAMs linked to the STREAM
				   associated with `fildes'.  */
#endif


/* Macros for `getmsg', `getpmsg', `putmsg' and `putpmsg'.  */
#define MSG_HIPRI	0x01	/* Send/receive high priority message.  */
#define MSG_ANY		0x02	/* Receive any message.  */
#define MSG_BAND	0x04	/* Receive message from specified band.  */

/* Values returned by getmsg and getpmsg */
#define MORECTL		1	/* More control information is left in
				   message.  */
#define MOREDATA	2	/* More data is left in message.  */


/* Structure used for the I_FLUSHBAND ioctl on streams.  */
struct bandinfo
  {
    unsigned char bi_pri;
    int bi_flag;
  };

struct strbuf
  {
    int maxlen;		/* Maximum buffer length.  */
    int len;		/* Length of data.  */
    char *buf;		/* Pointer to buffer.  */
  };

struct strpeek
  {
    struct strbuf ctlbuf;
    struct strbuf databuf;
    t_uscalar_t flags;			/* UnixWare/Solaris compatibility.  */
  };

struct strfdinsert
  {
    struct strbuf ctlbuf;
    struct strbuf databuf;
    t_uscalar_t flags;			/* UnixWare/Solaris compatibility.  */
    int fildes;
    int offset;
  };

struct strioctl
  {
    int ic_cmd;
    int ic_timout;
    int ic_len;
    char *ic_dp;
  };

struct strrecvfd
  {
    int fd;
    uid_t uid;
    gid_t gid;
    char __fill[8];			/* UnixWare/Solaris compatibility */
  };


struct str_mlist
  {
    char l_name[FMNAMESZ + 1];
  };

struct str_list
  {
    int sl_nmods;
    struct str_mlist *sl_modlist;
  };

#endif /* bits/stropts.h */
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   