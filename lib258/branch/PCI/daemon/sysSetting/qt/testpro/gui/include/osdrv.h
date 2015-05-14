/* Copyright (C) 2004 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef _MQUEUE_H
# error "Never use <bits/mqueue.h> directly; include <mqueue.h> instead."
#endif

typedef int mqd_t;

struct mq_attr
{
  long int mq_flags;	/* Message queue flags.  */
  long int mq_maxmsg;	/* Maximum number of messages.  */
  long int mq_msgsize;	/* Maximum message size.  */
  long int mq_curmsgs;	/* Number of messages currently queued.  */
  long int __pad[4];
};
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           /* Checking macros for mq functions.
   Copyright (C) 2007, 2011 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef	_FCNTL_H
# error "Never include <bits/mqueue2.h> directly; use <mqueue.h> instead."
#endif

/* Check that calls to mq_open with O_CREAT set have an appropriate third and fourth
   parameter.  */
extern mqd_t mq_open (__const char *__name, int __oflag, ...)
     __THROW __nonnull ((1));
extern mqd_t __mq_open_2 (__const char *__name, int __oflag)
     __THROW __nonnull ((1));
extern mqd_t __REDIRECT_NTH (__mq_open_alias, (__const char *__name,
					       int __oflag, ...), mq_open)
     __nonnull ((1));
__errordecl (__mq_open_wrong_number_of_args,
	     "mq_open can be called either with 2 or 4 arguments");
__errordecl (__mq_open_missing_mode_and_attr,
	     "mq_open with O_CREAT in second argument needs 4 arguments");

__extern_always_inline mqd_t
__NTH (mq_open (__const char *__name, int __oflag, ...))
{
  