i/auth/ntlmssp_sign.c:99        ntlmssp_make_packet_signature: RECV seq = %u, len = %u, pdu_len = %u
   ../libcli/auth/ntlmssp_sign.c:165       NTLMSSP Signing not negotiated - cannot sign packet!
   ../libcli/auth/ntlmssp_sign.c:170       NO session key, cannot check sign packet
       ../libcli/auth/ntlmssp_sign.c:199       NO session key, cannot check packet signature
  ../libcli/auth/ntlmssp_sign.c:205       NTLMSSP packet check failed due to short signature (%lu bytes)!
        talloc_new: ../libcli/auth/ntlmssp_sign.c:208   ../libcli/auth/ntlmssp_sign.c:222       NTLMSSP packet sig creation failed with %s
     ../libcli/auth/ntlmssp_sign.c:223       ../libcli/auth/ntlmssp_sign.c:230       BAD SIG NTLM2: wanted signature of
     ../libcli/auth/ntlmssp_sign.c:233       ../libcli/auth/ntlmssp_sign.c:236       NTLMSSP NTLM2 packet check failed due to invalid signature!
    ../libcli/auth/ntlmssp_sign.c:237       ../libcli/auth/ntlmssp_sign.c:243       BAD SIG NTLM1: wanted signature of
     ../libcli/auth/ntlmssp_sign.c:246       ../libcli/auth/ntlmssp_sign.c:249       NTLMSSP NTLM1 packet check failed due to invalid signature!
    ../libcli/auth/ntlmssp_sign.c:250       ../libcli/auth/ntlmssp_sign.c:255       ntlmssp_check_packet: NTLMSSP signature OK !
   ../libcli/auth/ntlmssp_sign.c:257       ../libcli/auth/ntlmssp_sign.c:273       NTLMSSP Sealing not negotiated - cannot seal packet!
   ../libcli/auth/ntlmssp_sign.c:278       NO session key, cannot seal packet
     ../libcli/auth/ntlmssp_sign.c:282       ../libcli/auth/ntlmssp_sign.c:362       NO session key, cannot unseal packet
   ../libcli/auth/ntlmssp_sign.c:366       ../libcli/auth/ntlmssp_sign.c:386       NTLMSSP packet check for unseal failed due to invalid signature on %llu bytes of input:
        DATA_BLOB: ../libcli/auth/ntlmssp_sign.c:400    ../libcli/auth/ntlmssp_sign.c:415       DATA_BLOB: ../libcli/auth/ntlmssp_sign.c:421    ../libcli/auth/ntlmssp_sign.c:436       DATA_BLOB: ../libcli/auth/ntlmssp_sign.c:440    DATA_BLOB: ../libcli/auth/ntlmssp_sign.c:462    DATA_BLOB: ../libcli/auth/ntlmssp_sign.c:478    ../libcli/auth/ntlmssp_sign.c:518       NTLMSSP packet check for unwrap failed due to invalid signature
        DATA_BLOB: ../libcli/auth/ntlmssp_sign.c:522    session key to server-to-client sealing key magic constant      session key to server-to-client signing key magic constant      session key to client-to-server sealing key magic constant      session key to client-to-server signing key magic constant      ../libcli/auth/ntlmssp_sign.c:535       NTLMSSP Sign/Seal - Initialising with flags:
   ../libcli/auth/ntlmssp_sign.c:539       NO session key, cannot intialise signing
       NTLMSSP send seal arc4 state