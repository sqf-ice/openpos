 �����V��>3��2�          x�     � �����V��>3��2�          ��     � �[#\�L����3IS��	�          ��      ��F���y�6���(`          ��      �d�������-�	�V	�          ��     �   ����5��"N��U
�          ��         !?����$>(��0�          ��         Y��:��@�"��]          ��         Y��:��@�"��]          ��         ������:}z�?�                    ��������6�����                    c�>���06	y��:          *          c�>���06	y��:          8      ���R-�����/Xe�j          @         >�">����@,�e�          L          \(^�p�!�/9���W
�           X        ������ �&mk���          f        �=/�}��>%�����          u          )z�[�e�O?�	�_�/          �        ��m.��?���0|��w�          �         :@t����`/�����          �      �  �������c<������,�    �         @!��J���6=�	��E�          �          W��L�*��:�L�W�          �        ��$���+�W?�:�T          �          �4���G�jA�	���2          �          w&��`����?�5�S�          �          w&��`����?�5�S�          �          w&��_����?�6�S�          �        ��4���F�jA�	���1                   �4���G�jA�	���2                   �4���G�jA�	���2          *     � �V�������0�;���          9     � �,,�e�3��/(��S          H     � �V�������0�;���          U     ~   ������ާ?W	i�%	Q          d     �   %v�����*32����           s     �   Y��s�s��>g���!          �     �   gP�9�d�g/�=��-          �     �   gP�9�d�g/�=��-          �     �   ��{���M1	R�
V          �     t   �������-
���          �     �   ���L���0�
��L@          �     �   ���L���0�
��L@          �     �   $Q�� x��$�2��X          �     �   �;������1�
 ��          �     �   b��a�3�,.0W�|;          �     �   ,,�e�3��/(��S          �     �   sX�����60�q��          �     �   sX�����60�q��                                  �     �     ��     �     �     ��     �     ��     �          =�     �          ��               %     *     ;     I     V                             �!      "     �"     �"                                                                                                                                                                                      �     P�                     ��     ��                                                                             �     ��     ��     ��     ��      �                              �     ��     @�     P�     `�     ��     `�      �      �     ��      �     �     P�     `�      �     ��      �                             `�     ��     ��     p�      �     ��     ��     ��     `�     ��     ��     ��     ��     ��     ��     ��      �                             0�     ��     ��     p�     ��     @�      �     �     ��      �     ��     �     ��     ��      �     ��      �            �1              2             2             2             '2             12             R2             ��             ��     ���o    �             �8             �      
       �2                           ��$            �	                           ��             �o             �]      	              ���o    o      ���o           ���o    Zk      ���o    �                                                                                      �c     0     @�     P�     @(             �c              d                     �     �D     p�     �c             0     @     �O     �$     @�      �     ��      ��             ��      P�      �      @t             �3             �     p     ��      ��              ��      [             @�      ��      0                     �c     @�      ��      �;             @�      �             �
             pj     0     `     �     �     ��     ��     ��     ��     pq             ��$                     ��      ��      ��      ��      ��      �      �      &�      6�      F�      V�      f�      v�      ��      ��      ��      ��      ��      ��      ��      ��      �      �      &�      6�      F�      V�      f�      v�      ��      ��      ��      ��      ��      ��      ��      ��      �      �      &�      6�      F�      V�      f�      v�      ��      ��      ��      ��      ��      ��      ��      ��      �      �      &�      6�      F�      V�      f�      v�      ��      ��      ��      ��      ��      ��      ��      ��      �      �      &�      6�      F�      V�      f�      v�      ��      ��      ��      ��      ��      ��      ��      ��      �      �      &�      6�      F�      V�      f�      v�      ��      ��      ��      ��      ��      ��      ��      ��      �      �      &�      @�$                               ��                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            d FTIE");
		return -1;
	}

	if (os_memcmp(ftie->snonce, sm->snonce, WPA_NONCE_LEN) != 0) {
		wpa_printf(MSG_DEBUG, "FT: SNonce mismatch in FTIE");
		wpa_hexdump(MSG_DEBUG, "FT: Received SNonce",
			    ftie->snonce, WPA_NONCE_LEN);
		wpa_hexdump(MSG_DEBUG, "FT: Expected SNonce",
			    sm->snonce, WPA_NONCE_LEN);
		return -1;
	}

	if (parse.r0kh_id == NULL) {
		wpa_printf(MSG_DEBUG, "FT: No R0KH-ID subelem in FTIE");
		return -1;
	}

	if (parse.r0kh_id_len != sm->r0kh_id_len ||
	    os_memcmp(parse.r0kh_id, sm->r0kh_id, parse.r0kh_id_len) != 0) {
		wpa_printf(MSG_DEBUG, "FT: R0KH-ID in FTIE did not match with "
			   "the current R0KH-ID");
		wpa_hexdump(MSG_DEBUG, "FT: R0KH-ID in FTIE",
			    parse.r0kh_id, parse.r0kh_id_len);
		wpa_hexdump(MSG_DEBUG, "FT: The current R0KH-ID",
			    sm->r0kh_id, sm->r0kh_id_len);
		return -1;
	}

	if (parse.r1kh_id == NULL) {
		wpa_printf(MSG_DEBUG, "FT: No R1KH-ID subelem in FTIE");
		return -1;
	}

	if (parse.rsn_pmkid == NULL ||
	    os_memcmp(parse.rsn_pmkid, sm->pmk_r0_name, WPA_PMK_NAME_LEN)) {
		wpa_printf(MSG_DEBUG, "FT: No matching PMKR0Name (PMKID) in "
			   "RSNIE");
		return -1;
	}

	os_memcpy(sm->r1kh_id, parse.r1kh_id, FT_R1KH_ID_LEN);
	wpa_hexdump(MSG_DEBUG, "FT: R1KH-ID", sm->r1kh_id, FT_R1KH_ID_LEN);
	wpa_hexdump(MSG_DEBUG, "FT: SNonce", sm->snonce, WPA_NONCE_LEN);
	wpa_hexdump(MSG_DEBUG, "FT: ANonce", ftie->anonce, WPA_NONCE_LEN);
	os_memcpy(sm->anonce, ftie->anonce, WPA_NONCE_LEN);
	wpa_derive_pmk_r1(sm->pmk_r0, sm->pmk_r0_name, sm->r1kh_id,
			  sm->own_addr, sm->pmk_r1, sm->pmk_r1_name);
	wpa_hexdump_key(MSG_DEBUG, "FT: PMK-R1", sm->pmk_r1, PMK_LEN);
	wpa_hexdump(MSG_DEBUG, "FT: PMKR1Name",
		    sm->pmk_r1_name, WPA_PMK_NAME_LEN);

	bssid = target_ap;
	ptk_len = sm->pairwise_cipher == WPA_CIPHER_CCMP ? 48 : 64;
	wpa_pmk_r1_to_ptk(sm->pmk_r1, sm->snonce, ftie->anonce, sm->own_addr,
			  bssid, sm->pmk_r1_name,
			  (u8 *) &sm->ptk, ptk_len, ptk_name);
	wpa_hexdump_key(MSG_DEBUG, "FT: PTK",
			(u8 *) &sm->ptk, ptk_len);
	wpa_hexdump(MSG_DEBUG, "FT: PTKName", ptk_name, WPA_PMK_NAME_LEN);

	ft_ies = wpa_ft_gen_req_ies(sm, &ft_ies_len, ftie->anonce,
				    sm->pmk_r1_name, sm->ptk.kck, bssid,
				    ric_ies, ric_ies_len,
				    parse.mdie ? parse.mdie - 2 : NULL);
	if (ft_ies) {
		wpa_sm_update_ft_ies(sm, sm->mobility_domain,
				     ft_ies, ft_ies_len);
		os_free(ft_ies);
	}

	wpa_sm_mark_authenticated(sm, bssid);
	ret = wpa_ft_install_ptk(sm, bssid);
	if (ret) {
		/*
		 * Some drivers do not support key configuration when we are
		 * not associated with the target AP. Work around this by
		 * trying again after the following reassociation gets
		 * completed.
		 */
		wpa_printf(MSG_DEBUG, "FT: Failed to set PTK prior to "
			   "association - try again after reassociation");
		sm->set_ptk_after_assoc = 1;
	} else
		sm->set_ptk_after_assoc = 0;

	sm->ft_completed = 1;
	if (ft_action) {
		/*
		 * The caller is expected trigger re-association with the
		 * Target AP.
		 */
		os_memcpy(sm->bssid, target_ap, ETH_ALEN);
	}

	return 0;
}


int wpa_ft_is_completed(struct wpa_sm *sm)
{
	if (sm == NULL)
		return 0;

	if (sm->key_mgmt != WPA_KEY_MGMT_FT_IEEE8021X &&
	    sm->key_mgmt != WPA_KEY_MGMT_FT_PSK)
		return 0;

	return sm->ft_completed;
}


static int wpa_ft_process_gtk_subelem(struct wpa_sm *sm, const u8 *gtk_elem,
				      size_t gtk_elem_len)
{
	u8 gtk[32];
	int keyidx;
	enum wpa_alg alg;
	size_t gtk_len, keylen, rsc_len;

	if (gtk_elem == NULL) {
		wpa_printf(MSG_DEBUG, "FT: No GTK included in FTIE");
		return 0;
	}

	wpa_hexdump_key(MSG_DEBUG, "FT: Received GTK in Reassoc Resp",
			gtk_elem, gtk_elem_len);

	if (gtk_elem_len < 11 + 24 || (gtk_elem_len - 11) % 8 ||
	    gtk_elem_len - 19 > sizeof(gtk)) {
		wpa_printf(MSG_DEBUG, "FT: Invalid GTK sub-elem "
			   "length %lu", (unsigned long) gtk_elem_len);
		return -1;
	}
	gtk_len = gtk_elem_len - 19;
	if (aes_unwrap(sm->ptk.kek, gtk_len / 8, gtk_elem + 11, gtk)) {
		wpa_printf(MSG_WARNING, "FT: AES unwrap failed - could not "
			   "decrypt GTK");
		return -1;
	}

	switch (sm->group_cipher) {
	case WPA_CIPHER_CCMP:
		keylen = 16;
		rsc_len = 6;
		alg = WPA_ALG_CCMP;
		break;
	case WPA_CIPHER_TKIP:
		keylen = 32;
		rsc_len = 6;
		alg = WPA_ALG_TKIP;
		break;
	case WPA_CIPHER_WEP104:
		keylen = 13;
		rsc_len = 0;
		alg = WPA_ALG_WEP;
		break;
	case WPA_CIPHER_WEP40:
		keylen = 5;
		rsc_len = 0;
		alg = WPA_ALG_WEP;
		break;
	default:
		wpa_printf(MSG_WARNING, "WPA: Unsupported Group Cipher %d",
			   sm->group_cipher);
		return -1;
	}

	if (gtk_len < keylen) {
		wpa_printf(MSG_DEBUG, "FT: Too short GTK in FTIE");
		return -1;
	}

	/* Key Info[2] | Key Length[1] | RSC[8] | Key[5..32]. */

	keyidx = WPA_GET_LE16(gtk_elem) & 0x03;

	if (gtk_elem[2] != keylen) {
		wpa_printf(MSG_DEBUG, "FT: GTK length mismatch: received %d "
			   "negotiated %lu",
			   gtk_elem[2], (unsigned long) keylen);
		return -1;
	}

	wpa_hexdump_key(MSG_DEBUG, "FT: GTK from Reassoc Resp", gtk, keylen);
	if (wpa_sm_set_key(sm, alg, broadcast_ether_addr, keyidx, 0,
			   gtk_elem + 3, rsc_len, gtk, keylen) < 0) {
		wpa_printf(MSG_WARNING, "WPA: Failed to set GTK to the "
			   "driver.");
		return -1;
	}

	return 0;
}


#ifdef CONFIG_IEEE80211W
static int wpa_ft_process_igtk_subelem(struct wpa_sm *sm, const u8 *igtk_elem,
				       size_t igtk_elem_len)
{
	u8 igtk[WPA_IGTK_LEN];
	u16 keyidx;

	if (sm->mgmt_group_cipher != WPA_CIPHER_AES_128_CMAC)
		return 0;

	if (igtk_elem == NULL) {
		wpa_printf(MSG_DEBUG, "FT: No IGTK included in FTIE");
		return 0;
	}

	wpa_hexdump_key(MSG_DEBUG, "FT: Received IGTK in Reassoc Resp",
			igtk_elem, igtk_elem_len);

	if (igtk_elem_len != 2 + 6 + 1 + WPA_IGTK_LEN + 8) {
		wpa_printf(MSG_DEBUG, "FT: Invalid IGTK sub-elem "
			   "length %lu", (unsigned long) igtk_elem_len);
		return -1;
	}
	if (igtk_elem[8] != WPA_IGTK_LEN) {
		wpa_printf(MSG_DEBUG, "FT: Invalid IGTK sub-elem Key Length "
			   "%d", igtk_elem[8]);
		return -1;
	}

	if (aes_unwrap(sm->ptk.kek, WPA_IGTK_LEN / 8, igtk_elem + 9, igtk)) {
		wpa_printf(MSG_WARNING, "FT: AES unwrap failed - could not "
			   "decrypt IGTK");
		return -1;
	}

	/* KeyID[2] | IPN[6] | Key Length[1] | Key[16+8] */

	keyidx = WPA_GET_LE16(igtk_elem);

	wpa_hexdump_key(MSG_DEBUG, "FT: IGTK from Reassoc Resp", igtk,
			WPA_IGTK_LEN);
	if (wpa_sm_set_key(sm, WPA_ALG_IGTK, broadcast_ether_addr, keyidx, 0,
			   igtk_elem + 2, 6, igtk, WPA_IGTK_LEN) < 0) {
		wpa_printf(MSG_WARNING, "WPA: Failed to set IGTK to the "
			   "driver.");
		return -1;
	}

	return 0;
}
#endif /* CONFIG_IEEE80211W */


int wpa_ft_validate_reassoc_resp(struct wpa_sm *sm, const u8 *ies,
				 size_t ies_len, const u8 *src_addr)
{
	struct wpa_ft_ies parse;
	struct rsn_mdie *mdie;
	struct rsn_ftie *ftie;
	unsigned int count;
	u8 mic[16];

	wpa_hexdump(MSG_DEBUG, "FT: Response IEs", ies, ies_len);

	if (sm->key_mgmt != WPA_KEY_MGMT_FT_IEEE8021X &&
	    sm->key_mgmt != WPA_KEY_MGMT_FT_PSK) {
		wpa_printf(MSG_DEBUG, "FT: Reject FT IEs since FT is not "
			   "enabled for this connection");
		return -1;
	}

	if (wpa_ft_parse_ies(ies, ies_len, &parse) < 0) {
		wpa_printf(MSG_DEBUG, "FT: Failed to parse IEs");
		return -1;
	}

	mdie = (struct rsn_mdie *) parse.mdie;
	if (mdie == NULL || parse.mdie_len < sizeof(*mdie) ||
	    os_memcmp(mdie->mobility_domain, sm->mobility_domain,
		      MOBILITY_DOMAIN_ID_LEN) != 0) {
		wpa_printf(MSG_DEBUG, "FT: Invalid MDIE");
		return -1;
	}

	ftie = (struct rsn_ftie *) parse.ftie;
	if (ftie == NULL || parse.ftie_len < sizeof(*ftie)) {
		wpa_printf(MSG_DEBUG, "FT: Invalid FTIE");
		return -1;
	}

	if (os_memcmp(ftie->snonce, sm->snonce, WPA_NONCE_LEN) != 0) {
		wpa_printf(MSG_DEBUG, "FT: SNonce mismatch in FTIE");
		wpa_hexdump(MSG_DEBUG, "FT: Received SNonce",
			    ftie->snonce, WPA_NONCE_LEN);
		wpa_hexdump(MSG_DEBUG, "FT: Expected SNonce",
			    sm->snonce, WPA_NONCE_LEN);
		return -1;
	}

	if (os_memcmp(ftie->anonce, sm->anonce, WPA_NONCE_LEN) != 0) {
		wpa_printf(MSG_DEBUG, "FT: ANonce mismatch in FTIE");
		wpa_hexdump(MSG_DEBUG, "FT: Received ANonce",
			    ftie->anonce, WPA_NONCE_LEN);
		wpa_hexdump(MSG_DEBUG, "FT: Expected ANonce",
			    sm->anonce, WPA_NONCE_LEN);
		return -1;
	}

	if (parse.r0kh_id == NULL) {
		wpa_printf(MSG_DEBUG, "FT: No R0KH-ID subelem in FTIE");
		return -1;
	}

	if (parse.r0kh_id_len != sm->r0kh_id_len ||
	    os_memcmp(parse.r0kh_id, sm->r0kh_id, parse.r0kh_id_len) != 0) {
		wpa_printf(MSG_DEBUG, "FT: R0KH-ID in FTIE did not match with "
			   "the current R0KH-ID");
		wpa_hexdump(MSG_DEBUG, "FT: R0KH-ID in FTIE",
			    parse.r0kh_id, parse.r0kh_id_len);
		wpa_hexdump(MSG_DEBUG, "FT: The current R0KH-ID",
			    sm->r0kh_id, sm->r0kh_id_len);
		return -1;
	}

	if (parse.r1kh_id == NULL) {
		wpa_printf(MSG_DEBUG, "FT: No R1KH-ID subelem in FTIE");
		return -1;
	}

	if (os_memcmp(parse.r1kh_id, sm->r1kh_id, FT_R1KH_ID_LEN) != 0) {
		wpa_printf(MSG_DEBUG, "FT: Unknown R1KH-ID used in "
			   "ReassocResp");
		return -1;
	}

	if (parse.rsn_pmkid == NULL ||
	    os_memcmp(parse.rsn_pmkid, sm->pmk_r1_name, WPA_PMK_NAME_LEN)) {
		wpa_printf(MSG_DEBUG, "FT: No matching PMKR1Name (PMKID) in "
			   "RSNIE (pmkid=%d)", !!parse.rsn_pmkid);
		return -1;
	}

	count = 3;
	if (parse.ric)
		count += ieee802_11_ie_count(parse.ric, parse.ric_len);
	if (ftie->mic_control[1] != count) {
		wpa_printf(MSG_DEBUG, "FT: Unexpected IE count in MIC "
			   "Control: received %u expected %u",
			   ftie->mic_control[1], count);
		return -1;
	}

	if (wpa_ft_mic(sm->ptk.kck, sm->own_addr, src_addr, 6,
		       parse.mdie - 2, parse.mdie_len + 2,
		       parse.ftie - 2, parse.ftie_len + 2,
		       parse.rsn - 2, parse.rsn_len + 2,
		       parse.ric, parse.ric_len,
		       mic) < 0) {
		wpa_printf(MSG_DEBUG, "FT: Failed to calculate MIC");
		return -1;
	}

	if (os_memcmp(mic, ftie->mic, 16) != 0) {
		wpa_printf(MSG_DEBUG, "FT: Invalid MIC in FTIE");
		wpa_hexdump(MSG_MSGDUMP, "FT: Received MIC", ftie->mic, 16);
		wpa_hexdump(MSG_MSGDUMP, "FT: Calculated MIC", mic, 16);
		return -1;
	}

	if (wpa_ft_process_gtk_subelem(sm, parse.gtk, parse.gtk_len) < 0)
		return -1;

#ifdef CONFIG_IEEE80211W
	if (wpa_ft_process_igtk_subelem(sm, parse.igtk, parse.igtk_len) < 0)
		return -1;
#endif /* CONFIG_IEEE80211W */

	if (sm->set_ptk_after_assoc) {
		wpa_printf(MSG_DEBUG, "FT: Try to set PTK again now that we "
			   "are associated");
		if (wpa_ft_install_ptk(sm, src_addr) < 0)
			return -1;
		sm->set_ptk_after_assoc = 0;
	}

	if (parse.ric) {
		wpa_hexdump(MSG_MSGDUMP, "FT: RIC Response",
			    parse.ric, parse.ric_len);
		/* TODO: parse response and inform driver about results */
	}

	return 0;
}


/**
 * wpa_ft_start_over_ds - Generate over-the-DS auth request
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @target_ap: Target AP Address
 * @mdie: Mobility Domain IE from the target AP
 * Returns: 0 on success, -1 on failure
 */
int wpa_ft_start_over_ds(struct wpa_sm *sm, const u8 *target_ap,
			 const u8 *mdie)
{
	u8 *ft_ies;
	size_t ft_ies_len;

	wpa_printf(MSG_DEBUG, "FT: Request over-the-DS with " MACSTR,
		   MAC2STR(target_ap));

	/* Generate a new SNonce */
	if (random_get_bytes(sm->snonce, WPA_NONCE_LEN)) {
		wpa_printf(MSG_INFO, "FT: Failed to generate a new SNonce");
		return -1;
	}

	ft_ies = wpa_ft_gen_req_ies(sm, &ft_ies_len, NULL, sm->pmk_r0_name,
				    NULL, target_ap, NULL, 0, mdie);
	if (ft_ies) {
		sm->over_the_ds_in_progress = 1;
		os_memcpy(sm->target_ap, target_ap, ETH_ALEN);
		wpa_sm_send_ft_action(sm, 1, target_ap, ft_ies, ft_ies_len);
		os_free(ft_ies);
	}

	return 0;
}

#endif /* CONFIG_IEEE80211R */
