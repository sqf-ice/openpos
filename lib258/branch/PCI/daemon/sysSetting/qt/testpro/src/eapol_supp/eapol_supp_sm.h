�H
D�  $   d� ����J   G�U ��H
D�  $   �� Ъ��:   G�U ��H
D�  $   �� ���J   G�U ��D
H�  $   ܅ ���   G�U ��D
H�  $   � ���J   G�U ��D
H�  $   ,� 0���:   G�U ��H
D�  $   T� H���J   G�U ��D
H�  $   |� p���:   G�U ��H
D�  $   �� ����   G�U ��D
H�  $   ̆ ����   G�U ��D
H�  $   � x���   G�U ��D
H�  $   � p���   G�U ��D
H�  $   D� h���   G�U ��D
H�  $   l� `����   H�U ��I
J% D   �� ظ��,   B�K�D �u
ABG�
ABJTAB     D   ܇ �����   B�K�D �u
ABGo
ABFDAB     $   $� X����   H�U ��I
J% ,   L� н���   M��M��NP�� 
H      D   |� `����   B�K�D �u
ABGd
ABIDAB     $   Ĉ �����   H�U ��I
J- D   � p���   B�K�D �u
ABG�
ABHDAB     $   4� 8����   H�U ��I
J5 $   \� �����   H�U ��I
J D   �� 8����   B�K�D �u
ABGQ
ABDDAB     $   ̉ �����   M��N0��p
E     $   � x���   M��N0��j
K     $   � p���/   M��N0��n
G     $   D� x���   M��N0��p
E     $   l� p���   M��N0��j
K     $   �� X���u   M��M��I@�^
G   �� ����              Ԋ ����              � ����              � ����              � ����              4� ����              L� ����              d� ����              |� ����              �� ����              ��  ���              ċ ���              ܋ ���              � ���              �  ���              $� (���              <� 0���              T� 8���              l� @���              �� H���              �� P���              �� X���              ̌ `���              � h���              �� p���              � x���              ,� ����              D� ����              \� ����              t� ����              �� ����              �� ����              �� ����              ԍ ����              � ����              � ����              � ����              4� ����              L� ����              d� ����              |� ����              �� ����              ��  ���              Ď ���              ܎ ���              � ���              �  ���              $� (���              <� 0���              T� 8���              l� @���              �� H���              �� P���              �� X���              ̏ `���              � h���              �� p���              � x���              ,� ����              D� ����              \� ����              t� ����              �� ����              �� ����              �� ����              Ԑ ����              � ����              � ����              � ����              4� ����              L� ����              d� ����              |� ����              �� ����              ��  ���              đ ���              ܑ ���              �� ���              �  ���              $� (���              <� 0���              T� 8���              l� @���'    A�L XA    �� P���'    A�L XA ,   �� `����   M��M��NP���
D         ܒ  ���              �� ����           ,   � ����   M��I ��
H]
K`     $   <� ����   M��I ��
HT
D4   d� ����}    A�F�G I
AAGNIA    ,   �� ����    L��I �^
MT
Dc     ,   ̓ �����    L��I �O
D]
K`     ,   �� 0����    L��I �{
H]
K`      ,   ,� �����    L��I �{
H]
K`      ,   \� 0����    L��I �{
H]
K`      ,   �� �����    L��I �{
H]
K`      ,   �� 0����    L��I �{
H]
K`      ,   � �����    L��I �^
MT
Dc     ,   � @����    L��I �{
H]
K`      ,   L� ;

	/**
	 * set_config_blob - Set or add a named configuration blob
	 * @ctx: Callback context (ctx)
	 * @blob: New value for the blob
	 *
	 * Adds a new configuration blob or replaces the current value of an
	 * existing blob.
	 */
	void (*set_config_blob)(void *ctx, struct wpa_config_blob *blob);

	/**
	 * get_config_blob - Get a named configuration blob
	 * @ctx: Callback context (ctx)
	 * @name: Name of the blob
	 * Returns: Pointer to blob data or %NULL if not found
	 */
	const struct wpa_config_blob * (*get_config_blob)(void *ctx,
							  const char *name);

	/**
	 * aborted_cached - Notify that cached PMK attempt was aborted
	 * @ctx: Callback context (ctx)
	 */
	void (*aborted_cached)(void *ctx);

	/**
	 * opensc_engine_path - Path to the OpenSSL engine for opensc
	 *
	 * This is an OpenSSL specific configuration option for loading OpenSC
	 * engine (engine_opensc.so); if %NULL, this engine is not loaded.
	 */
	const char *opensc_engine_path;

	/**
	 * pkcs11_engine_path - Path to the OpenSSL engine for PKCS#11
	 *
	 * This is an OpenSSL specific configuration option for loading PKCS#11
	 * engine (engine_pkcs11.so); if %NULL, this engine is not loaded.
	 */
	const char *pkcs11_engine_path;

	/**
	 * pkcs11_module_path - Path to the OpenSSL OpenSC/PKCS#11 module
	 *
	 * This is an OpenSSL specific configuration option for configuring
	 * path to OpenSC/PKCS#11 engine (opensc-pkcs11.so); if %NULL, this
	 * module is not loaded.
	 */
	const char *pkcs11_module_path;

	/**
	 * wps - WPS context data
	 *
	 * This is only used by EAP-WSC and can be left %NULL if not available.
	 */
	struct wps_context *wps;

	/**
	 * eap_param_needed - Notify that EAP parameter is needed
	 * @ctx: Callback context (ctx)
	 * @field: Field indicator (e.g., WPA_CTRL_REQ_EAP_IDENTITY)
	 * @txt: User readable text describing the required parameter
	 */
	void (*eap_param_needed)(void *ctx, enum wpa_ctrl_req_type field,
				 const char *txt);

	/**
	 * port_cb - Set port authorized/unauthorized callback (optional)
	 * @ctx: Callback context (ctx)
	 * @authorized: Whether the supplicant port is now in authorized state
	 */
	void (*port_cb)(void *ctx, int authorized);

	/**
	 * cert_cb - Notification of a peer certificate
	 * @ctx: Callback context (ctx)
	 * @depth: Depth in certificate chain (0 = server)
	 * @subject: Subject of the peer certificate
	 * @cert_hash: SHA-256 hash of the certificate
	 * @cert: Peer certificate
	 */
	void (*cert_cb)(void *ctx, int depth, const char *subject,
			const char *cert_hash, const struct wpabuf *cert);

	/**
	 * cert_in_cb - Include server certificates in callback
	 */
	int cert_in_cb;
};


struct eap_peer_config;

#ifdef IEEE8021X_EAPOL
struct eapol_sm *eapol_sm_init(struct eapol_ctx *ctx);
void eapol_sm_deinit(struct eapol_sm *sm);
void eapol_sm_step(struct eapol_sm *sm);
int eapol_sm_get_status(struct eapol_sm *sm, char *buf, size_t buflen,
			int verbose);
int eapol_sm_get_mib(struct eapol_sm *sm, char *buf, size_t buflen);
void eapol_sm_configure(struct eapol_sm *sm, int heldPeriod, int authPeriod,
			int startPeriod, int maxStart);
int eapol_sm_rx_eapol(struct eapol_sm *sm, const u8 *src, const u8 *buf,
		      size_t len);
void eapol_sm_notify_tx_eapol_key(struct eapol_sm *sm);
void eapol_sm_notify_portEnabled(struct eapol_sm *sm, Boolean enabled);
void eapol_sm_notify_portValid(struct eapol_sm *sm, Boolean valid);
void eapol_sm_notify_eap_success(struct eapol_sm *sm, Boolean success);
void eapol_sm_notify_eap_fail(struct eapol_sm *sm, Boolean fail);
void eapol_sm_notify_config(struct eapol_sm *sm,
			    struct eap_peer_config *config,
			    const struct eapol_config *conf);
int eapol_sm_get_key(struct eapol_sm *sm, u8 *key, size_t len);
void eapol_sm_notify_logoff(struct eapol_sm *sm, Boolean logoff);
void eapol_sm_notify_cached(struct eapol_sm *sm);
void eapol_sm_notify_pmkid_attempt(struct eapol_sm *sm, int attempt);
void eapol_sm_register_scard_ctx(struct eapol_sm *sm, void *ctx);
void eapol_sm_notify_portControl(struct eapol_sm *sm, PortControl portControl);
void eapol_sm_notify_ctrl_attac A�C�G }AA          t� ����*           T   �� Е���   B�B�B �D(�D0�G@V
0A(A BBBE�
0C(A BBBH4   � ����   A�K�G �
AAIDDA      � ����              4� x���@              L� �����              d� X���P              |� ����*              �� ����*              �� ����D           $   Ħ ����s    A�C�G eAA<   � P���?   B�F�D �D0I
 AABC AAB   ,� P���           4   D� H���(   B�A�K ��
ABAcAB 4   |� @���   B�I�D �C
ABK�IB ,   �� (���/   A�C�G !AA          � (���@              �� P���+              � h���X             ,� ����             D� ����[              \� ���+              t� ���7              �� 0���P              �� h���*              �� ����+              Ԩ ����P              � Я��P              � ���7              � 0����           <   4� �����   B�C�D �G0I
 AABCl AAB$   t� X����   M��N@���
B    $   ��  ���f   M��M��L@�g
C $   ĩ h���4   M��N@��}
H     |   � �����   B�B�B �F(�G0�A8�GP�
8D0A(B BBBAR
8I0A(B BBBGP8F0A(B BBB      $   l� �����   M��N@���
F    $   �� h���"   M��N@���
G    ,   �� p����   M��M��N`���
F      <   � @����   M��N0���
Mj
F�
N�
E      ,   ,� �����   M��M��N`���
I       ,   \� ����~   M��M��N`���
I       $   �� �����   M��M��I@�r
K  4   �� H����   A�K�G �
AABTDA      �  ���              � ����
           $   � ����   M��N0��p
E        D� ����'    A�L XA ,   d� ����3   A�C�G %AA          �� ����
           $   �� �����   M��N0��p
E     $   Ԭ �����   M��N0��p
E     ,   �� �����   M��M��N@���
F       D   ,� �����   B�D�D �G0�
 AABF�
 AABG     $   t� ����   M��V0���o
F   $   �� � ���    M��N �x
ER   ,   ĭ 8���   M��I �M
MT
D
E,   �� ���G   M��I ��
I]
K`
H  4   $� ����   M��V0���T
I^
B�
K    $   \� `���    M��N �x
ER   4   �� ����   M��N0��l
Ic
M'
I       ,   �� �	���   M��I �f
D]
Ky       � @��'    A�L XA L   � P��U   B�B�B �B(�I0�A8�G�+
8C0A(B BBBH   ,   \� `!��   L��^������
E      $   �� @@���    M��N �x
ER   ,   �� �@���   M��M��NP���
H       $   � �F��K   M