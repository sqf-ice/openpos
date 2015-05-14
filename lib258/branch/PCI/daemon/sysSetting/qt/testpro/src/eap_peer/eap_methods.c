/*
 * EAP peer: Method registration
 * Copyright (c) 2004-2007, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#include "includes.h"
#ifdef CONFIG_DYNAMIC_EAP_METHODS
#include <dlfcn.h>
#endif /* CONFIG_DYNAMIC_EAP_METHODS */

#include "common.h"
#include "eap_i.h"
#include "eap_methods.h"


static struct eap_method *eap_methods = NULL;


/**
 * eap_peer_get_eap_method - Get EAP method based on type number
 * @vendor: EAP Vendor-Id (0 = IETF)
 * @method: EAP type number
 * Returns: Pointer to EAP method or %NULL if not found
 */
const struct eap_method * eap_peer_get_eap_method(int vendor, EapType method)
{
	struct eap_method *m;
	for (m = eap_methods; m; m = m->next) {
		if (m->vendor == vendor && m->method == method)
			return m;
	}
	return NULL;
}


/**
 * eap_peer_get_type - Get EAP type for the given EAP method name
 * @name: EAP method name, e.g., TLS
 * @vendor: Buffer for returning EAP Vendor-Id
 * Returns: EAP method type or %EAP_TYPE_NONE if not found
 *
 * This function maps EAP type names into EAP type numbers based on the list of
 * EAP methods included in the build.
 */
EapType eap_peer_get_type(const char *name, int *vendor)
{
	struct eap_method *m;
	for (m = eap_methods; m; m = m->next) {
		if (os_strcmp(m->name, name) == 0) {
			*vendor = m->vendor;
			return m->method;
		}
	}
	*vendor = EAP_VENDOR_IETF;
	return EAP_TYPE_NONE;
}


/**
 * eap_get_name - Get EAP method name for the given EAP type
 * @vendor: EAP Vendor-Id (0 = IETF)
 * @type: EAP method type
 * Returns: EAP method name, e.g., TLS, or %NULL if not found
 *
 * This function maps EAP type numbers into EAP type names based on the list of
 * EAP methods included in the build.
 */
const char * eap_get_name(int vendor, EapType type)
{
	struct eap_method *m;
	if (vendor == EAP_VENDOR_IETF && type == EAP_TYPE_EXPANDED)
		return "expanded";
	for (m = eap_methods; m; m = m->next) {
		if (m->vendor == vendor && m->method == type)
			return m->name;
	}
	return NULL;
}


/**
 * eap_get_names - Get space separated list of names for supported EAP methods
 * @buf: Buffer for names
 * @buflen: Buffer length
 * Returns: Number of characters written into buf (not including nul
 * termination)
 */
size_t eap_get_names(char *buf, size_t buflen)
{
	char *pos, *end;
	struct eap_method *m;
	int ret;

	if (buflen == 0)
		return 0;

	pos = buf;
	end = pos + buflen;

	for (m = eap_methods; m; m = m->next) {
		ret = os_snprintf(pos, end - pos, "%s%s",
				  m == eap_methods ? "" : " ", m->name);
		if (ret < 0 || ret >= end - pos)
			break;
		pos += ret;
	}
	buf[buflen - 1] = '\0';

	return pos - buf;
}


/**
 * eap_get_names_as_string_array - Get supported EAP methods as string array
 * @num: Buffer for returning the number of items in array, not including %NULL
 * terminator. This parameter can be %NULL if the length is not needed.
 * Returns: A %NULL-terminated array of strings, or %NULL on error.
 *
 * This function returns the list of names for all supported EAP methods as an
 * array of strings. The caller must free the returned array items and the
 * array.
 */
char ** eap_get_names_as_string_array(size_t *num)
{
	struct eap_method *m;
	size_t array_len = 0;
	char **array;
	int i = 0, j;

	for (m = eap_methods; m; m = m->next)
		array_len++;

	array = os_zalloc(sizeof(char *) * (array_len + 1));
	if (array == NULL)
		return NULL;

	for (m = eap_methods; m; m = m->next) {
		array[i++] = os_strdup(m->name);
		if (array[i - 1] == NULL) {
			for (j = 0; j < i; j++)
				os_free(array[j]);
			os_free(array);
			return NULL;
		}
	}
	array[i] = NULL;

	if (num)
		*num = array_len;

	return array;
}


/**
 * eap_peer_get_methods - Get a list of enabled EAP peer methods
 * @count: Set to number of available methods
 * Returns: List of enabled EAP peer methods
 */
const /* Copyright (C) 2005 Free Software Foundation, Inc.
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

#ifndef _FEATURES_H
# error "Never use <bits/predefs.h> directly; include <features.h> instead."
#endif

#ifndef _PREDEFS_H
#define _PREDEFS_H

/* We do support the IEC 559 math functionality, real and complex.  */
#define __STDC_IEC_559__		1
#define __STDC_IEC_559_COMPLEX__	1

#endif /* predefs.h */
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               /* Define POSIX options for Linux.
   Copyright (C) 1996-2004, 2006, 2008, 2009 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULA