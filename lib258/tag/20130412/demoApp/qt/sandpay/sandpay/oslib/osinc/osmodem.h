nbt_getdc_got_reader            parse_getdc_response            cli_prep_mailslot libsmb/clistr.c:35    Pushing string of 'unlimited' length into non-SMB buffer!
 clistr_push_fn       parse_quota_record: called with NULL POINTER!   \$Extend\$Quota:$Q:$INDEX_ALLOCATION    cli_get_user_quota() called with NULL Pointer!  NT_TRANSACT_GET_USER_QUOTA failed: %s
  Got INVALID NT_TRANSACT_GET_USER_QUOTA reply.
  cli_set_user_quota() called with NULL Pointer!  NT_TRANSACT_SET_USER_QUOTA failed: %s
  cli_list_user_quota() called with NULL Pointer! Failed to parse the quota record
       cli_get_fs_quota_info() called with NULL Pointer!       SMB_FS_QUOTA_INFORMATION failed: %s
    cli_set_fs_quota_info() called with NULL Pointer! libsmb/cliquota.c:41 libsmb/cliquota.c:146 libsmb/cliquota.c:152 libsmb/cliquota.c:155 libsmb/cliquota.c:156 libsmb/cliquota.c:201 libsmb/cliquota.c:247 SMB_USER_QUOTA_LIST libsmb/cliquota.c:257 talloc_init() failed
 libsmb/cliquota.c:268 SMB_NTQUOTA_LIST libsmb/cliquota.c:273 TALLOC_ZERO() failed
 libsmb/cliquota.c:274 SMB_NTQUOTA_STRUCT libsmb/cliquota.c:279 libsmb/cliquota.c:280 libsmb/cliquota.c:293 libsmb/cliquota.c:294 libsmb/cliquota.c:309 libsmb/cliquota.c:325 libsmb/cliquota.c:330 libsmb/cliquota.c:331 libsmb/cliquota.c:336 libsmb/cliquota.c:337 libsmb/cliquota.c:349 libsmb/cliquota.c:350 libsmb/cliquota.c:388 libsmb/cliquota.c:407 libsmb/cliquota.c:457        cli_get_user_quota              cli_set_user_quota              cli_list_user_quota             cli_get_fs_quota_info           cli_set_fs_quota_info   DATA_BLOB: libsmb/clifsinfo.c:542       DATA_BLOB: libsmb/clifsinfo.c:543       struct cli_get_fs_attr_info_state       struct cli_unix_extensions_version_state        struct cli_set_unix_extensions_capabilities_state       make_cli_gss_blob: gss_init_sec_context failed with %s
 DATA_BLOB: libsmb/clifsinfo.c:750       cli_force_encryption: cli_unix_extensions_version returned %s
 libsmb/clifsinfo.c:545 libsmb/clifsinfo.c:546 libsmb/clifsinfo.c:299 libsmb/clifsinfo.c:301 libsmb/clifsinfo.c:308 libsmb/clifsinfo.c:310 libsmb/clifsinfo.c:314 libsmb/clifsinfo.c:315 libsmb/clifsinfo.c:75 libsmb/clifsinfo.c:77 libsmb/clifsinfo.c:84 libsmb/clifsinfo.c:86 libsmb/clifsinfo.c:94 libsmb/clifsinfo.c:95 libsmb/clifsinfo.c:215 libsmb/clifsinfo.c:217 *�H�� libsmb/clifsinfo.c:742 libsmb/clifsinfo.c:52 libsmb/clifsinfo.c:65 libsmb/clifsinfo.c:104 libsmb/clifsinfo.c:155 libsmb/clifsinfo.c:182 libsmb/clifsinfo.c:203 libsmb/clifsinfo.c:257 libsmb/clifsinfo.c:277 libsmb/clifsinfo.c:289 libsmb/clifsinfo.c:321 libsmb/clifsinfo.c:353 libsmb/clifsinfo.c:399 libsmb/clifsinfo.c:449 cifs libsmb/clifsinfo.c:852         make_cli_gss_blob               cli_force_encryption %c%s%c%s%c%s libsmb/clidfs.c:511 libsmb/clidfs.c:515 libsmb/clidfs.c:516 libsmb/clidfs.c:517 %d:	server=%s, share=%s
 struct client_dfs_referral libsmb/clidfs.c:762 libsmb/clidfs.c:764 Connection to %s failed
 libsmb/clidfs.c:179  session request ok
 session setup failed: %s
 did you forget to run kinit?
 Anonymous login successful
 libsmb/clidfs.c:219 cli_init_creds() failed: %s
 libsmb/clidfs.c:227 libsmb/clidfs.c:230 OS=[%s] Server=[%s]
 libsmb/clidfs.c:233  session setup ok
 tree connect failed: %s
 libsmb/clidfs.c:276  tconx ok
 libsmb/clidfs.c:289 libsmb/clidfs.c:291        Encryption required and server that doesn't support UNIX extensions - failing connect
  Encryption required and can't get UNIX CIFS extensions version from server.
    Encryption required and share %s doesn't support encryption.
   Encryption required and setup failed with error %s.
    Connection to %s failed (Error %s)
     protocol negotiation failed: %s
        Domain=[%s] OS=[%s] Server=[%s]
        cli_cm_open() Unable to open connection [\%s\%s] without auth info
     Unable to follow dfs referral [\%s\%s]
 cli_resolve_path: dfs_path (%s) not in correct format.
 do_connect libsmb/clioplock.c:69 libsmb/clioplock.c:73 libsmb/clioplock.c:75 libsmb/clioplock.c:78 libsmb/clioplock.c:43 struct cli_oplock_ack_state libsmb/clioplock.c:59 libsmb/clioplock.c:120 libsmb/clirap2.c:270 Access Denied
 libsmb/clirap2.c:272 Group does not exist
 libsmb/clirap2.c:274 NetGroupDelete res=%d
 libsmb/clirap2.c:278 NetGroupDelete failed
 libsmb/clirap2.c:315 Malloc fail
 B21Bz WsT libsmb/clirap2.c:343 libsmb/clirap2.c:345 Group already exists
 libsmb/clirap2.c:347 NetGroupAdd res=%d
 libsmb/clirap2.c:351 NetGroupAdd failed
 libsmb/clirap2.c:394 libsmb/clirap2.c:396 NetGroupEnum gave error %d
 libsmb/clirap2.c:401 libsmb/clirap2.c:438 libsmb/clirap2.c:440 NetGroupEnum res=%d
 B21 libsmb/clirap2.c:483 libsmb/clirap2.c:485 libsmb/clirap2.c:490 libsmb/clirap2.c:515 libsmb/clirap2.c:558 libsmb/clirap2.c:561 Not supported by server
 libsmb/clirap2.c:564 libsmb/clirap2.c:567 User does not exist
 libsmb/clirap2.c:570 User is not in group
 libsmb/clirap2.c:573 NetGroupDelUser res=%d
 libsmb/clirap2.c:577 NetGroupDelUser failed
 libsmb/clirap2.c:618 libsmb/clirap2.c:621 libsmb/clirap2.c:624 libsmb/clirap2.c:627 libsmb/clirap2.c:630 NetGroupAddUser res=%d
 libsmb/clirap2.c:634 NetGroupAddUser failed
 libsmb/clirap2.c:674 libsmb/clirap2.c:679 libsmb/clirap2.c:703 NetGroupGetUsers res=%d
 libsmb/clirap2.c:743 libsmb/clirap2.c:748 libsmb/clirap2.c:772 NetUserGetGroups res=%d
 libsmb/clirap2.c:816 libsmb/clirap2.c:818 libsmb/clirap2.c:820 NetUserDelete res=%d
 libsmb/clirap2.c:824 NetUserDelete failed
 B21BB16DWzzWz WsTWW libsmb/clirap2.c:899 libsmb/clirap2.c:901 User already exists
 libsmb/clirap2.c:903 NetUserAdd res=%d
 libsmb/clirap2.c:907 NetUserAdd failed
 libsmb/clirap2.c:949 NetUserEnum gave error %d
 libsmb/clirap2.c:954 NetUserEnum no data returned
 libsmb/clirap2.c:1013 libsmb/clirap2.c:1015 NetUserEnum res=%d
 libsmb/clirap2.c:1057 libsmb/clirap2.c:1062 libsmb/clirap2.c:1086 libsmb/clirap2.c:1129 libsmb/clirap2.c:1131 NetFileClose2 res=%d
 libsmb/clirap2.c:1135 NetFileClose2 failed
 DWWzz DWrLh libsmb/clirap2.c:1209 libsmb/clirap2.c:1211 NetFileGetInfo2 res=%d
 libsmb/clirap2.c:1215 NetFileGetInfo2 failed
 zzWrLehb8g8 libsmb/clirap2.c:1322 libsmb/clirap2.c:1324 NetFileEnum2 res=%d
 libsmb/clirap2.c:1327 NetFileEnum2 failed
 B13BWzWWWzB9B libsmb/clirap2.c:1396 NetShareAdd res=%d
 libsmb/clirap2.c:1399 NetShareAdd failed
 zW libsmb/clirap2.c:1442 NetShareDelete res=%d
 libsmb/clirap2.c:1445 NetShareDelete failed
 libsmb/clirap2.c:1533 libsmb/clirap2.c:1537 zzzBBzz libsmb/clirap2.c:1616 libsmb/clirap2.c:1728 libsmb/clirap2.c:1733 pull_ascii failed
 libsmb/clirap2.c:1738 talloc_strdup failed
 libsmb/clirap2.c:1834 WDW zzWb38WrLh libsmb/clirap2.c:1895 B13BWWWzzzzzWN WB21BB16B10zWWzDDz libsmb/clirap2.c:1936 NetPrintQEnum gave error %d
 libsmb/clirap2.c:1941 libsmb/clirap2.c:2067 libsmb/clirap2.c:2069 NetPrintQEnum res=%d
 zWrLh libsmb/clirap2.c:2114 libsmb/clirap2.c:2119 libsmb/clirap2.c:2242 libsmb/clirap2.c:2244 NetPrintQGetInfo res=%d
 B16WDWB64 libsmb/clirap2.c:2287 libsmb/clirap2.c:2289 NetServiceEnum gave error %d
 libsmb/clirap2.c:2294 libsmb/clirap2.c:2327 NetServiceEnum res=%d
 zzWWWDDDz libsmb/clirap2.c:2370 NetSessionEnum gave error %d
 libsmb/clirap2.c:2375 libsmb/clirap2.c:2424 libsmb/clirap2.c:2426 NetSessionEnum res=%d
 libsmb/clirap2.c:2473 libsmb/clirap2.c:2478 libsmb/clirap2.c:2527 libsmb/clirap2.c:2529 NetSessionGetInfo res=%d
 libsmb/clirap2.c:2575 libsmb/clirap2.c:2579 WWWWDzz libsmb/clirap2.c:2622 libsmb/clirap2.c:2627 libsmb/clirap2.c:2669 libsmb/clirap2.c:2671 NetConnectionEnum res=%d
      Not all group names were returned (such as those longer than 21 characters)
    NetGroupEnum no data returned
  NetGroupGetUsers gave error %d
 NetGroupGetUsers no data returned
      NetUserGetGroups gave error %d
 NetUserGetGroups no data returned
      NetFileClose2 - attempt to close non-existant file open instance
       cli_get_pdc_name: machine %s failed the NetServerEnum call. Error was : %s.
    invalid data count %d, expected >= 16
  cli_ns_check_server_type: machine %s failed the NetServerEnum call. Error was : %s.
    NetwkstaUserLogoff gave error %d
       NetPrintQEnum no data returned
 NetPrintQGetInfo gave error %d
 NetPrintQGetInfo no data returned
      Not all service names were returned (such as those longer than 15 characters)
  NetServiceEnum no data returned
        NetSesssionEnum no data returned
       NetSessionGetInfo gave error %d
        NetSessionGetInfo no data returned
     NetConnectionEnum gave error %d
        NetConnectionEnum no data returned
     cli_NetGroupDelete              cli_NetGroupAdd cli_RNetGroupEnum               cli_RNetGroupEnum0              cli_NetGroupDelUser             cli_NetGroupAddUser             cli_NetGroupGetUsers            cli_NetUserGetGroups            cli_NetUserDelete cli_NetUserAdd                cli_RNetUserEnum                cli_RNetUserEnum0               cli_NetFileClose                cli_NetFileGetInfo              cli_Ne