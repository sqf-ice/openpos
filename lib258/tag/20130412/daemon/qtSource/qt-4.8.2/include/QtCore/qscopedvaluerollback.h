known) does does not lib/smbldap.c:1263 lib/smbldap.c:1294 lib/smbldap.c:765 lib/smbldap.c:1012 lib/smbldap.c:1014 Set dereferencing method: %d
 lib/smbldap.c:1018 lib/smbldap.c:1180 lib/smbldap.c:1194 lib/smbldap.c:1225 1.2.840.113556.1.4.319 lib/smbldap.c:1240 lib/smbldap.c:1316 lib/smbldap.c:1319 lib/smbldap.c:1409 lib/smbldap.c:1310 lib/smbldap.c:1786 lib/smbldap.c:1722 lib/smbldap.c:1724 smbldap_delete: dn => [%s]
 lib/smbldap.c:1744 lib/smbldap.c:1754 lib/smbldap.c:1678 lib/smbldap.c:1680 smbldap_add: dn => [%s]
 lib/smbldap.c:1700 lib/smbldap.c:1710 lib/smbldap.c:1634 lib/smbldap.c:1636 smbldap_modify: dn => [%s]
 lib/smbldap.c:1656 lib/smbldap.c:1666 lib/smbldap.c:1436 lib/smbldap.c:1439 lib/smbldap.c:1459 lib/smbldap.c:1461 smbldap_search_ext: go on!
 lib/smbldap.c:1504 lib/smbldap.c:1514 lib/smbldap.c:1552 lib/smbldap.c:1557 {iO} lib/smbldap.c:1587 lib/smbldap.c:1591 lib/smbldap.c:1597 sambaAlgorithmicRidBase sambaPwdCanChange sambaPwdMustChange sambaLogonTime sambaLogoffTime sambaKickoffTime sambaHomeDrive sambaHomePath sambaLogonScript sambaProfilePath sambaUserWorkstations sambaPrimaryGroupSID sambaLMPassword sambaNTPassword sambaMungedDial sambaBadPasswordCount sambaBadPasswordTime sambaPasswordHistory sambaLogonHours modifyTimestamp pwdCanChange pwdMustChange logonTime logoffTime kickoffTime smbHome homeDrives userWorkstations lmPassword ntPassword acctFlags     smbldap_check_root_dse: nothing to look for
    smbldap_check_root_dse: no idea what to query root-dse for: %s ?
       smbldap_check_root_dse: Could not search rootDSE
       smbldap_check_root_dse: Expected one rootDSE, got %d
   smbldap_check_root_dse: Could not retrieve rootDSE
     smbldap_check_root_dse: LDAP Server does not support any %s
    smbldap_check_root_dse: LDAP Server does not have any %s
       The connection to the LDAP server was closed
   smb_ldap_setup_connection: %s
  Failed to set LDAP_OPT_REFERRALS: %s
   ldap connection not connected...
       ldap connection idle...closing connection
      smbldap_set_mod: out of memory! mods[i]->mod_bvalues[j] != NULL mods[i]->mod_bvalues[j]->bv_val != NULL smbldap_set_mod: String conversion failure!     mods[i]->mod_values[j] != NULL  smbldap_get_single_attribute: [%s] = [<does not exist>]
        smbldap_get_single_a