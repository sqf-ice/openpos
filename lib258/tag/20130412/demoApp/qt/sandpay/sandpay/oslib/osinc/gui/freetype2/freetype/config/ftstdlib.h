/***************************************************************************/
/*                                                                         */
/*  ftstdlib.h                                                             */
/*                                                                         */
/*    ANSI-specific library and header configuration file (specification   */
/*    only).                                                               */
/*                                                                         */
/*  Copyright 2002, 2003, 2004, 2005, 2006, 2007, 2009 by                  */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* This file is used to group all #includes to the ANSI C library that   */
  /* FreeType normally requires.  It also defines macros to rename the     */
  /* standard functions within the FreeType source code.                   */
  /*                                                                       */
  /* Load a file which defines __FTSTDLIB_H__ before this one to override  */
  /* it.                                                                   */
  /*                                                                       */
  /*************************************************************************/


#ifndef __FTSTDLIB_H__
#define __FTSTDLIB_H__


#include <stddef.h>

#define ft_ptrdiff_t  ptrdiff_t


  /**********************************************************************/
  /*                                                                    */
  /*                           integer limits                           */
  /*                                                                    */
  /* UINT_MAX and ULONG_MAX are used to automatically compute the size  */
  /* of `int' and `long' in bytes at compile-time.  So far, this works  */
  /* for all platforms the library has been tested on.                  */
  /*                                                                    */
  /* Note that on the extremely rare platforms that do not provide      */
  /* integer types that are _exactly_ 16 and 32 bits wide (e.g. some    */
  /* old Crays where `int' is 36 bits), we do not make any guarantee    */
  /* about the correct behaviour of FT2 with all fonts.                 */
  /*                                                                    */
  /* In these case, `ftconfig.h' will refuse to compile anyway with a   */
  /* message like `couldn't find 32-bit type' or something similar.     */
  /*                                                                    */
  /**********************************************************************/


#include <limits.h>

#define FT_CHAR_BIT   CHAR_BIT
#define FT_INT_MAX    INT_MAX
#define FT_UINT_MAX   UINT_MAX
#define FT_ULONG_MAX  ULONG_MAX


  /**********************************************************************/
  /*                                                                    */
  /*                 character and string processing                    */
  /*                                                                    */
  /**********************************************************************/


#include <string.h>

#define ft_memchr   memchr
#define ft_memcmp   memcmp
#defineATUS_UNHANDLED_EXCEPTION NT_STATUS_APP_INIT_FAILURE NT_STATUS_NO_PAGEFILE NT_STATUS_INVALID_LEVEL NT_STATUS_WRONG_PASSWORD_CORE NT_STATUS_PIPE_BROKEN NT_STATUS_REGISTRY_CORRUPT NT_STATUS_REGISTRY_IO_FAILED NT_STATUS_NO_EVENT_PAIR NT_STATUS_UNRECOGNIZED_VOLUME NT_STATUS_NO_SUCH_ALIAS NT_STATUS_MEMBER_NOT_IN_ALIAS NT_STATUS_MEMBER_IN_ALIAS NT_STATUS_ALIAS_EXISTS NT_STATUS_LOGON_NOT_GRANTED NT_STATUS_TOO_MANY_SECRETS NT_STATUS_SECRET_TOO_LONG NT_STATUS_INTERNAL_DB_ERROR NT_STATUS_FULLSCREEN_MODE NT_STATUS_NOT_REGISTRY_FILE NT_STATUS_FT_MISSING_MEMBER NT_STATUS_ILLEGAL_CHARACTER NT_STATUS_UNDEFINED_CHARACTER NT_STATUS_FLOPPY_VOLUME NT_STATUS_DISK_RESET_FAILED NT_STATUS_SHARED_IRQ_BUSY NT_STATUS_FT_ORPHANING NT_STATUS_PARTITION_FAILURE NT_STATUS_EOM_OVERFLOW NT_STATUS_NO_MEDIA NT_STATUS_NO_SUCH_MEMBER NT_STATUS_INVALID_MEMBER NT_STATUS_KEY_DELETED NT_STATUS_NO_LOG_SPACE NT_STATUS_TOO_MANY_SIDS NT_STATUS_KEY_HAS_CHILDREN NT_STATUS_IO_DEVICE_ERROR NT_STATUS_BACKUP_CONTROLLER NT_STATUS_LOG_FILE_FULL NT_STATUS_TOO_LATE NT_STATUS_NO_TRUST_LSA_SECRET NT_STATUS_EVENTLOG_CANT_START NT_STATUS_TRUST_FAILURE NT_STATUS_ACCOUNT_EXPIRED NT_STATUS_POSSIBLE_DEADLOCK NT_STATUS_FS_DRIVER_REQUIRED NT_STATUS_NO_USER_SESSION_KEY NT_STATUS_INVALID_BUFFER_SIZE NT_STATUS_TOO_MANY_ADDRESSES NT_STATUS_ADDRESS_CLOSED NT_STATUS_CONNECTION_RESET NT_STATUS_TOO_MANY_NODES NT_STATUS_TRANSACTION_ABORTED NT_STATUS_NOT_SERVER_SESSION NT_STATUS_NOT_CLIENT_SESSION NT_STATUS_DEBUG_ATTACH_FAILED NT_STATUS_DATA_NOT_ACCEPTED NT_STATUS_VDM_HARD_ERROR NT_STATUS_MAPPED_ALIGNMENT NT_STATUS_NOT_FOUND NT_STATUS_NOT_TINY_STREAM NT_STATUS_RECOVERY_FAILURE NT_STATUS_STACK_OVERFLOW_READ NT_STATUS_FAIL_CHECK NT_STATUS_DUPLICATE_OBJECTID NT_STATUS_OBJECTID_EXISTS NT_STATUS_CONVERT_TO_LARGE NT_STATUS_RETRY NT_STATUS_FOUND_OUT_OF_SCOPE NT_STATUS_ALLOCATE_BUCKET NT_STATUS_PROPSET_NOT_FOUND NT_STATUS_MARSHALL_OVERFLOW NT_STATUS_INVALID_VARIANT NT_STATUS_ACCOUNT_LOCKED_OUT NT_STATUS_HANDLE_NOT_CLOSABLE NT_STATUS_CONNECTION_REFUSED NT_STATUS_GRACEFUL_DISCONNECT NT_STATUS_CONNECTION_INVALID NT_STATUS_CONNECTION_ACTIVE NT_STATUS_NETWORK_UNREACHABLE NT_STATUS_HOST_UNREACHABLE NT_STATUS_PORT_UNREACHABLE NT_STATUS_REQUEST_ABORTED NT_STATUS_CONNECTION_ABORTED NT_STATUS_USER_MAPPED_FILE NT_STATUS_AUDIT_FAILED NT_STATUS_BAD_DLL_ENTRYPOINT NT_STATUS_LPC_REPLY_LOST NT_STATUS_PATH_NOT_COVERED NT_STATUS_NO_CALLBACK_ACTIVE NT_STATUS_PWD_TOO_SHORT NT_STATUS_PWD_TOO_RECENT NT_STATUS_PLUGPLAY_NO_DEVICE NT_STATUS_INVALID_HW_PROFILE NT_STATUS_RESOURCE_NOT_OWNED NT_STATUS_TOO_MANY_LINKS NT_STATUS_FILE_IS_OFFLINE NT_STATUS_DS_NO_MORE_RIDS NT_STATUS_NOT_A_REPARSE_POINT NT_STATUS_NO_MORE_ENTRIES NT_STATUS_RPC_UNKNOWN_IF NT_STATUS_RPC_CALL_FAILED NT_STATUS_RPC_PROTOCOL_ERROR NT_STATUS_RPC_CANNOT_SUPPORT NT_STATUS_RPC_SEC_PKG_ERROR NT_STATUS_RPC_BAD_STUB_DATA NT_STATUS_RPC_PIPE_CLOSED NT_STATUS_RPC_PIPE_EMPTY NT_STATUS_OBJECTID_NOT_FOUND NT_STATUS_DOWNGRADE_DETECTED NT_STATUS_NO_S4U_PROT_SUPPORT NT_STATUS_INVALID_LOCK_RANGE STATUS_SOME_UNMAPPED STATUS_NOTIFY_CLEANUP STATUS_NOTIFY_ENUM_DIR Undetermined error Account locked out Must change password Password is too short Password i