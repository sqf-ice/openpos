/*
 * EAP peer method: EAP-TLS (RFC 2716)
 * Copyright (c) 2004-2008, Jouni Malinen <j@w1.fi>
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

#include "common.h"
#include "crypto/tls.h"
#include "eap_i.h"
#include "eap_tls_common.h"
#include "eap_config.h"


static void eap_tls_deinit(struct eap_sm *sm, void *priv);


struct eap_tls_data {
	struct eap_ssl_data ssl;
	u8 *key_data;
};


static void * eap_tls_init(struct eap_sm *sm)
{
	struct eap_tls_data *data;
	struct eap_peer_config *config = eap_get_config(sm);
	if (config == NULL ||
	    ((sm->init_phase2 ? config->private_key2 : config->private_key)
	     == NULL &&
	     (sm->init_phase2 ? config->engine2 : config->engine) == 0)) {
		wpa_printf(MSG_INFO, "EAP-TLS: Private key not configured");
		return NULL;
	}

	data = os_zalloc(sizeof(*data));
	if (data == NULL)
		return NULL;

	if (eap_peer_tls_ssl_init(sm, &data->ssl, config)) {
		wpa_printf(MSG_INFO, "EAP-TLS: Failed to initialize SSL.");
		eap_tls_deinit(sm, data);
		if (config->engine) {
			wpa_printf(MSG_DEBUG, "EAP-TLS: Requesting Smartcard "
				   "PIN");
			eap_sm_request_pin(sm);
			sm->ignore = TRUE;
		} else if (config->private_key && !config->private_key_passwd)
		{
			wpa_printf(MSG_DEBUG, "EAP-TLS: Requesting private "
				   "key passphrase");
			eap_sm_request_passphrase(sm);
			sm->ignore = TRUE;
		}
		return NULL;
	}

	return data;
}


static void eap_tls_deinit(struct eap_sm *sm, void *priv)
{
	struct eap_tls_data *data = priv;
	if (data == NULL)
		return;
	eap_peer_tls_ssl_deinit(sm, &data->ssl);
	os_free(data->key_data);
	os_free(data);
}


static struct wpabuf * eap_tls_failure(struct eap_sm *sm,
				       struct eap_tls_data *data,
				       struct eap_method_ret *ret, int res,
				       struct wpabuf *resp, u8 id)
{
	wpa_printf(MSG_DEBUG, "EAP-TLS: TLS processing failed");

	ret->methodState = METHOD_DONE;
	ret->decision = DECISION_FAIL;

	if (res == -1) {
		struct eap_peer_config *config = eap_get_config(sm);
		if (config) {
			/*
			 * The TLS handshake failed. So better forget the old
			 * PIN. It may be wrong, we cannot be sure but trying
			 * the wrong one again might block it on the card--so
			 * better ask the user again.
			 */
			os_free(config->pin);
			config->pin = NULL;
		}
	}

	if (resp) {
		/*
		 * This is likely an alert message, so send it instead of just
		 * ACKing the error.
		 */
		return resp;
	}

	return eap_peer_tls_build_ack(id, EAP_TYPE_TLS, 0);
}


static void eap_tls_success(struct eap_sm *sm, struct eap_tls_data *data,
			    struct eap_method_ret *ret)
{
	wpa_printf(MSG_DEBUG, "EAP-TLS: Done");

	ret->methodState = METHOD_DONE;
	ret->decision = DECISION_UNCOND_SUCC;

	os_free(data->key_data);
	data->key_data = eap_peer_tls_derive_key(sm, &data->ssl,
						 "client EAP encryption",
						 EAP_TLS_KEY_LEN +
						 EAP_EMSK_LEN);
	if (data->key_data) {
		wpa_hexdump_key(MSG_DEBUG, "EAP-TLS: Derived key",
				data->key_data, EAP_TLS_KEY_LEN);
		wpa_hexdump_key(MSG_DEBUG, "EAP-TLS: Derived EMSK",
				data->key_data + EAP_TLS_KEY_LEN,
				EAP_EMSK_LEN);
	} else {
		wpa_printf(MSG_INFO, "EAP-TLS: Failed to derive key");
	}
}


static struct wpabuf * eap_tls_process(struct eap_sm *sm, void *priv,
				       struct eap_method_ret *ret,
				       const struct wpabuf *reqData)
{
	size_t left;
	int res;
	struct wpabuf *resp;
	u8 flags, id;
	const u8 *pos;
	struct eap_tls_data *data = priv;

	pos = eap_peer_tls_process_init(sm, &data->ssl, EAP_TYPE_TLS, ret,
					reqData, &left, &flags);
	if (pos == NULL)
		return NULL;
	id = eap_get_id(reqData);

	if (flags & EAP_TLS_FLAGS_START) {
		wpa_printf(MSG_DEBUG, "EAP-TLS: Start");
		left = 0; /* make sure that this frame is empty, even though it
			   * should always be, anyway */
	}

	resp = NULL;
	res = eap_peer_tls_proP|×˙v    AKG O
AADDDA    4   Ĺ |×˙Ě    BAD ą
ABEADB  4   źĹ 0}×˙î    AKG Ę
AAADDA    4   ôĹ č}×˙Ě    BAD ą
ABEADB  $   ,Ć ~×˙ß    MMD0|
F   ,   TĆ 8×˙   GTI0K
A      $   Ć ×˙Ş   HU I
J5 $   ŹĆ  ×˙ß    MMD0|
F   $   ÔĆ X×˙    HU `
CU   D   üĆ Đ×˙t   BDJ t
ABI
ABGDAB     $   DÇ ×˙f    HP u
CQ      4   lÇ P×˙Ě    BAD ą
ABEADB  4   ¤Ç č×˙   AKG ę
AAADDA    4   ÜÇ Ŕ×˙Ě    BAD ľ
ABAADB  $   Č X×˙ß    MMD0|
F   $   <Č ×˙    HU `
CU   D   dČ ×˙t   BDJ t
ABI
ABGDAB     $   ŹČ Ŕ×˙f    HP u
CQ      4   ÔČ ×˙Ě    BAD ą
ABEADB  4   É  ×˙î    AKG Ć
AAEDDA    4   DÉ X×˙Ě    BAD ą
ABEADB  $   |É đ×˙é    MMD0F
D   $   ¤É ¸×˙   HU I
J% $   ĚÉ 0×˙é    MMD0F
D   $   ôÉ ř×˙    HU `
CU   D   Ę p×˙´   BDJ t
ABIM
ABHDAB     4   dĘ č×˙v    AKG O
AADDDA    4   Ę 0×˙Ě    BAD ą
ABEADB  4   ÔĘ Č×˙   AKG Ü
AAGDDA    4   Ë  ×˙Ě    BAD ą
ABEADB  4   DË 8×˙>   AKG 
AAFDDA   4   |Ë @×˙Ě    BAD ľ
ABAADB  4   ´Ë Ř×˙Ţ    AKG ¸
AACDDA    4   ěË ×˙Ě    BAD ľ
ABAADB  4   $Ě ×˙~   AKG R
AAIDDA   4   \Ě `×˙Ě    BAD ľ
ABAADB  $   Ě ř×˙ŕ   MMD0b
H   $   źĚ °×˙    HU `
CU   D   äĚ (×˙´   BDJ t
ABIM
ABHDAB     $   ,Í  ×˙9   MMD0
A   $   TÍ ¸ ×˙z   GU H
D 4   |Í ˘×˙v    AKG O
AADDDA    4   ´Í X˘×˙Ě    BAD ą
ABEADB  4   ěÍ đ˘×˙Î    AKG ˘
AAIDDA    4   $Î Ł×˙Ě    BAD ľ
ABAADB  4   \Î  ¤×˙~   AKG R
AAIDDA   4   Î hĽ×˙Ü    BAD ź
ABJADB  4   ĚÎ Ś×˙Ţ    AKG ¸
AACDDA    4   Ď ¸Ś×˙Ě    BAD ą
ABEADB  4   <Ď P§×˙   AKG i
AABDDA   4 