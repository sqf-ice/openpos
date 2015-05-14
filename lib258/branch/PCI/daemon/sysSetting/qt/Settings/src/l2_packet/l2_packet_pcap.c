EOF)				      \
			     break;					      \
			   *__ptr++ = __c;				      \
			 }						      \
		       ((size_t) (size) * (size_t) (n) - __cnt)		      \
			/ (size_t) (size); })				      \
		  : (((__builtin_constant_p (size) && (size_t) (size) == 0)   \
		      || (__builtin_constant_p (n) && (size_t) (n) == 0))     \
			/* Evaluate all parameters once.  */		      \
		     ? ((void) (ptr), (void) (stream), (void) (size),	      \
			(void) (n), (size_t) 0)				      \
		     : fread_unlocked (ptr, size, n, stream))))

# define fwrite_unlocked(ptr, size, n, stream) \
  (__extension__ ((__builtin_constant_p (size) && __builtin_constant_p (n)    \
		   && (size_t) (size) * (size_t) (n) <= 8		      \
		   && (size_t) (size) != 0)				      \
		  ? ({ const char *__ptr = (const char *) (ptr);	      \
		       FILE *__stream = (stream);			      \
		       size_t __cnt;					      \
		       for (__cnt = (size_t) (size) * (size_t) (n);	      \
			    __cnt > 0; --__cnt)				      \
			 if (_IO_putc_unlocked (*__ptr++, __stream) == EOF)   \
			   break;					      \
		       ((size_t) (size) * (size_t) (n) - __cnt)		      \
			/ (size_t) (size); })				      \
		  : (((__builtin_constant_p (size) && (size_t) (size) == 0)   \
		      || (__builtin_constant_p (n) && (size_t) (n) == 0))     \
			/* Evaluate all parameters once.  */		      \
		     ? ((void) (ptr), (void) (stream), (void) (size),	      \
			(void) (n), (size_t) 0)				      \
		     : fwrite_unlocked (ptr, size, n, stream))))
#endif

/* Define helper macro.  */
#undef __STDIO_INLINE
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          /* Copyright (C) 1994, 1997, 1998, 1999, 2009 Free Software Foundation, Inc.
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

#if !defined _STDIO_H && !defined __need_FOPEN_MAX && !defined __need_IOV_MAX
# error "Never include <bits/stdio_lim.h> directly; use <stdio.h> instead."
#endif

#ifdef _STDIO_H
# define L_tmpnam 20
# define TMP_MAX 238328
# define FILENAME_MAX 4096

# ifdef __USE_POSIX
#  define L_ctermid 9
#  if !defined __USE_XOPEN2K || defined __USE_GNU
#   define L_cuserid 9
#  endif
# endif
#endif

#if defined __need_FOPEN_MAX || defined _STDIO_H
# undef  FOPEN_MAX
# define FOPEN_MAX 16
#endif

#if defined __need_IOV_MAX && !defined IOV_MAX
# define IOV_MAX 1024
#endif
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                /* -mlong-double-64 compatibility mode for <stdlib.h> functions.
   Copyright (C) 2006, 2007 Free Software Foundation, Inc.
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

#ifndef _STDLIB_H
# error "Never include <bits/stdlib-ldbl.h> directly; use <stdlib.h> instead."
#endif

#ifdef	__USE_ISOC99
__BEGIN_NAMESPACE_C99
__LDBL_REDIR1_DECL (strtold, strtod)
__END_NAMESPACE_C99
#endif

#ifdef __USE_GNU
__LDBL_REDIR1_DECL (strtold_l, strtod_l)
#endif

#if defined __USE_SVID || defined __USE_XOPEN_EXTENDED
# ifdef __USE_MISC
__LDBL_REDIR1_DECL (qecvt, ecvt)
__LDBL_REDIR1_DECL (qfcvt, fcvt)
__LDBL_REDIR1_DECL (qgcvt, gcvt)
__LDBL_REDIR1_DECL (qecvt_r, ecvt_r)
__LDBL_REDIR1_DECL (qfcvt_r, fcvt_r)
# endif
#en