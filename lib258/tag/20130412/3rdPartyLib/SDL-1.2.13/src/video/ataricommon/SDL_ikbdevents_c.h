tters for group version */
#define GROUP_API_VERSION_GET_MAJOR(v) ((v) >> 16)
#define GROUP_API_VERSION_GET_MINOR(v) ((v) & 0xffff)
#define GROUP_API_VERSION_SET_MAJOR(vp, n) do { \
    *(vp) = (*(vp) & 0x0000ffff) | ((n) << 16); \
} while(0)
#define GROUP_API_VERSION_SET_MINOR(vp, n) do { \
    *(vp) = (*(vp) & 0xffff0000) | (n); \
} while(0)

/*
 * version: for compatibility checking
 * group_init: return 1 on success, 0 if unconfigured, -1 on error.
 * group_cleanup: called to clean up resources used by provider
 * user_in_group: returns 1 if user is in group, 0 if not.
 *                note that pwd may be NULL if the user is not in passwd.
 */
struct sudoers_group_plugin {
    unsigned int version;
    int (*init)(int version, sudo_printf_t sudo_printf, char *const argv[]);
    void (*cleanup)(void);
    int (*query)(const char *user, const char *group, const struct passwd *pwd);
};

#endif /* _SUDO_PLUGIN_H */
                                                                                                                                                                                                                                                                                                                                                                                                              