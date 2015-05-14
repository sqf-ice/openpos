/***************************************************************************/
/*                                                                         */
/*  ftbbox.h                                                               */
/*                                                                         */
/*    FreeType exact bbox computation (specification).                     */
/*                                                                         */
/*  Copyright 1996-2001, 2003, 2007 by                                     */
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
  /* This component has a _single_ role: to compute exact outline bounding */
  /* boxes.                                                                */
  /*                                                                       */
  /* It is separated from the rest of the engine for various technical     */
  /* reasons.  It may well be integrated in `ftoutln' later.               */
  /*                                                                       */
  /*************************************************************************/


#ifndef __FTBBOX_H__
#define __FTBBOX_H__


#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef FREETYPE_H
#error "freetype.h of FreeType 1 has been loaded!"
#error "Please fix the directory search order for header files"
#error "so that freetype.h of FreeType 2 is found first."
#endif


FT_BEGIN_HEADER


  /*************************************************************************/
  /*                                                                       */
  /* <Section>                                                             */
  /*    outline_processing                                                 */
  /*                                                                       */
  /*************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    FT_Outline_Get_BBox                                                */
  /*                                                                       */
  /* <Description>                                                         */
  /*    Compute the exact bounding box of an outline.  This is slower      */
  /*    than computing the control box.  However, it uses an advanced      */
  /*    algorithm which returns _very_ quickly when the two boxes          */
  /*    coincide.  Otherwise, the outline Bézier arcs are traversed to     */
  /*    extract their extrema.                                             */
  /*                                                                       */
  /* <Input>                                                               */
  /*    outline :: A pointer to the source outline.                        */
  /*                                                                       */
  /* <Output>                                                              */
  /*    abbox   :: The outline's exact bounding box.                       */
  /*                                 T_STATUS_SECTION_NOT_IMAGE NT_STATUS_SECTION_PROTECTION NT_STATUS_EAS_NOT_SUPPORTED NT_STATUS_EA_TOO_LARGE NT_STATUS_NO_EAS_ON_FILE NT_STATUS_EA_CORRUPT_ERROR NT_STATUS_FILE_LOCK_CONFLICT NT_STATUS_LOCK_NOT_GRANTED NT_STATUS_DELETE_PENDING NT_STATUS_UNKNOWN_REVISION NT_STATUS_REVISION_MISMATCH NT_STATUS_INVALID_OWNER NT_STATUS_NO_LOGON_SERVERS NT_STATUS_NO_SUCH_PRIVILEGE NT_STATUS_PRIVILEGE_NOT_HELD NT_STATUS_USER_EXISTS NT_STATUS_NO_SUCH_USER NT_STATUS_GROUP_EXISTS NT_STATUS_NO_SUCH_GROUP NT_STATUS_MEMBER_IN_GROUP NT_S