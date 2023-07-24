#ifndef IDIP_TARGETS_MAP_H_INCLUDED
#define IDIP_TARGETS_MAP_H_INCLUDED

#include <idip.h>
#include <idip/idip_vca_handler.h>
#include <idip/idip_vector.h>
#include <idip/details/idip_methods.h>

struct idip_av_profile_name_item {
    struct idip_av_profile_name_item* next;
    char* profile_name;
};

/// Structure to associate target and its own callbacks and capabilities
typedef struct idip_thmap_item
{
    /// Target name
    char* target;

    // Live streaming feature
    /// Live stream handler
    idip_stream_av_handler_t            av_handler;
    void*                               av_handler_udata;
    struct idip_av_profile_name_item*   av_profiles;

    // Snapshot feature
    /// Shnapshot handler
    idip_stream_snapshot_handler_t      snapshot_handler;
    void*                               snapshot_handler_udata;

    // Push-To-Talk feature
    /// Push-to-Talk handler
    idip_audio_playback_handler_t       playback_handler;
    void*                               playback_handler_udata;
    /// List of audio codecs which are using for playing on a device.
    audio_codec_t*                      playback_acodecs;
    /// Audio codecs list size. In items.
    size_t                              playback_acodecs_count;
    /// V2 Window Size. 0 - window isn't used
    size_t                              playback_window_size;

    // Archive feature
    /// Archive streaming handler
    idip_stream_archive_handler_t       archive_handler;
    void*                               archive_handler_udata;
    size_t                              archive_window_size;
    /// Archive record items search handler
    idip_stream_archive_list_handler_t  archive_list_handler;
    void*                               archive_list_handler_udata;
    /// Archive days search handler
    idip_stream_archive_days_handler_t  archive_days_handler;
    void*                               archive_days_handler_udata;
    // Archive capabilities
    /// Supported archive records playing speed factors array
    archive_play_speed_t*               archive_speed_factors;
    /// Speed factors array length in items
    size_t                              archive_speed_factors_count;

    // Cloud configuration feature
    /// Full confiruration read handler
    idip_stream_config_get_handler_t    config_get_handler;
    void*                               config_get_handler_udata;
    /// Configuration update handler
    idip_stream_config_update_handler_t config_update_handler;
    void*                               config_update_handler_udata;
    size_t                              config_window_size;

    // PTZ feature
    /// PTZ feature handler
    idip_ptz_handler_t                  ptz_handler;
    void*                               ptz_handler_udata;
    /// Array of supported PTZ commands
    ptz_command_type_t*                 ptz_commands;
    /// Supported PTZ commands array size
    size_t                              ptz_commands_count;

    // VCA feature
    idip_stream_vca_handler_t           vca_handler;
    void*                               vca_handler_udata;
}
idip_thmap_item_t;

/// User-defined implementation and target overrides.
///
typedef struct idip_thmap
{
    /// Default implementations (callbacks)
    idip_thmap_item_t                       default_impl;

    /// Per-target implementation (callbacks) overrides.
    idip_vector_t*                          overrides;

    /// Device-wide user authetication callback
    idip_auth_user_t                        auth_user_handler;
    void*                                   auth_user_handler_udata;

    // Wireless networking feature
    /// Device wifi scanning handler
    idip_wireless_scan_handler_t            wireless_scan_handler;
    void*                                   wireless_scan_handler_udata;
    /// Device wifi setup handler
    idip_wireless_setup_handler_t           wireless_setup_handler;
    void*                                   wireless_setup_handler_udata;

    // Networking generic
    idip_system_get_interfaces_handler_t    system_get_interfaces_handler;
    void*                                   system_get_interfaces_handler_udata;

    /// QR-code/sound code/etc token
    idip_system_token_handler_t             system_token_handler;
    void*                                   system_token_handler_udata;

    // Persistent data storage feature
    /// Device persistend data storage read handler
    idip_stream_persistent_data_get_handler_t persistent_data_get_handler;
    void*                                     persistent_data_get_handler_udata;
    /// Device persistent data storage write handler
    idip_stream_persistent_data_set_handler_t persistent_data_set_handler;
    void*                                     persistent_data_set_handler_udata;
    /// Maximal data storage capacity in bytes
    size_t                                    persistent_data_max_size;
    /// Window Size Limit
    size_t                                    persistent_data_window_size;


    /// Device notify handler
    idip_stream_system_notify_handler_t     system_notify_handler;
    void*                                   system_notify_handler_udata;
    /// Device update notify handler
    idip_stream_update_version_handler_t    system_update_handler;
    void*                                   system_update_handler_udata;
    /// Device OTA handler
    idip_stream_ota_handler_t               system_ota_handler;
    size_t                                  system_ota_max_chunk_size;
    void*                                   system_ota_handler_udata;
    /// Window Size Limit. 0 - window is unused
    size_t                                  system_ota_window_size;


    /// Device account management
    idip_stream_passwd_handler_t            system_passwd_handler;
    void*                                   system_passwd_handler_udata;
    idip_stream_system_get_users_handler_t  system_get_users_handler;
    void*                                   system_get_users_handler_udata;
}
idip_thmap_t;

typedef struct idip_method_handler_t
{
    method_t                                    method;
    union {
        // a "generic" handler pointer to check the handler is set
        // and for libidip internally implemented handlers (not defined in idip.h).
        void*                                   handler;
        // Handlers that can be implemented by libidip user. They are defined in idip.h
        idip_stream_av_handler_t                av_handler;
        idip_stream_archive_handler_t           archive_handler;
        idip_stream_archive_days_handler_t      archive_days_handler;
        idip_stream_archive_list_handler_t      archive_list_handler;
        idip_stream_snapshot_handler_t          snapshot_handler;
        idip_ptz_handler_t                      ptz_handler;
        idip_stream_persistent_data_get_handler_t persistent_data_get_handler;
        idip_stream_persistent_data_set_handler_t persistent_data_set_handler;
        idip_stream_vca_handler_t               vca_handler;
        idip_wireless_scan_handler_t            wireless_scan_handler;
        idip_wireless_setup_handler_t           wireless_setup_handler;
        idip_stream_config_get_handler_t        config_get_handler;
        idip_stream_config_update_handler_t     config_update_handler;
        idip_audio_playback_handler_t           playback_handler;

        idip_stream_system_notify_handler_t     system_notify_handler;
        idip_stream_update_version_handler_t    system_update_handler;
        idip_stream_passwd_handler_t            system_passwd_handler;
        idip_stream_system_get_users_handler_t  system_get_users_handler;
        idip_system_get_interfaces_handler_t    system_get_interfaces_handler;
        idip_system_token_handler_t             system_token_handler;
    };
    void*                                       user_data;
}
idip_method_handler_t;

/**
 * Initialize a @ref idip_thmap_t instance. Initialized instance should be
 * destroyed using @ref idip_thmap_destroy() function.
 * @param self - pointer to uninitialized instance of @ref idip_thmap_t.
 * @retval 0 if success
 * @retval -1 if failed. The function sets errno to appropriate value.
 */
int idip_thmap_init(idip_thmap_t* self);

/**
 * Destroy an @ref idip_thmap_t instance.
 * @param self - pointer to initialized instance of @ref idip_thmap_t.
 */
void idip_thmap_destroy(idip_thmap_t* self);

/**
 * Looks up an @a idip_thmap_item_t instance that matches the @a target name.
 * @param self - pointer to a initialized @a idip_thmap_t instance.
 * @param target - target name to match. NULL and empty ("") strings are
 * matching to a "default" target.
 * @return pointer to a matched item or NULL if nothing was found.
 */
idip_thmap_item_t* idip_thmap_look_up(const idip_thmap_t* self, const char* target);

/**
 * The function works like @ref idip_thmap_look_up but it creates a new empty
 * instance (all fields are NULL) if the given target wasn't found.
 * @param target - target name to match. NULL and empty ("") strings are
 * matching to a "default" target.
 * @return pointer to a matched item or NULL if errors were occurs. A @c errno
 * variable is set to an appropriate value.
 */
idip_thmap_item_t* idip_thmap_get(idip_thmap_t* self, const char* target);

/**
 * A generig way to look up a handler by target (if applicable) name and method type
 * @param self [in] - pointer to the targets map object. Cannot be NULL.
 * @param target [in] - a target name (if applicable) or NULL
 * @param method [in] - method type
 * @param handler [out] - a pointer to a @ref idip_method_handler_t to fill.
 * @return 0 - ok, -1 handler wasn't found (for a given target if applicable)
 */
int idip_thmap_get_handler(const idip_thmap_t* self,
                           const char* target,
                           const method_t method,
                           idip_method_handler_t *handler);

/**
 * Set up a live streaming @a handler for a @a given target name.
 * @param self - pointer to a map instance
 * @param target - target name. NULL or empty strings are matched to a
 * "default" handler.
 * @param handler - pointer to a handler.
 * @return 0 if success, -1 if errors were occurred.
 */
int setup_av_handler(idip_thmap_t *self, const char* target,
                     idip_stream_av_handler_t handler, void* udata);

int thmap_append_profile(idip_thmap_t *self, const char* target, const char *profile);

int thmap_remove_profile(idip_thmap_t *self, const char* target, const char *profile);

int thmap_clear_target_profiles(idip_thmap_t *self, const char *target);

int setup_snapshot_handler(idip_thmap_t* self, const char* target,
                         idip_stream_snapshot_handler_t handler, void* udata);

int get_playback_handler(const idip_thmap_t* self, const char* target,
                         idip_audio_playback_handler_t* phandler, void** pudata);
int get_playback_capabilities(const idip_thmap_t* self, const char* target,
                              idip_p2t_playback_capabilities_t* capabilities);
int setup_playback_feature(idip_thmap_t* self, const char* target,
                           idip_audio_playback_handler_t handler, void* udata,
                           idip_p2t_playback_capabilities_t capabilities);

int get_archive_capabilities(const idip_thmap_t* self, const char* target,
                             idip_archive_capabilities_t* capabilities);
int setup_archive_feature(idip_thmap_t* self, const char* target,
                          idip_stream_archive_handler_t handler,
                          void* handler_udata,
                          idip_stream_archive_list_handler_t list_handler,
                          void* list_handler_udata,
                          idip_stream_archive_days_handler_t days_handler,
                          void* days_handler_udata,
                          idip_archive_capabilities_t capabilities);

int setup_config_handlers(idip_thmap_t* self, const char* target,
                          idip_stream_config_get_handler_t get_handler, void *get_handler_udata,
                          idip_stream_config_update_handler_t update_handler, void *update_handler_udata);

int get_ptz_capabilities(const idip_thmap_t* self, const char* target,
                         idip_ptz_capabilities_t* capabilities);
int setup_ptz_feature(idip_thmap_t* self, const char* target,
                      idip_ptz_handler_t handler, void *udata,
                      idip_ptz_capabilities_t capabilities);

int get_auth_user_handler(const idip_thmap_t* self, idip_auth_user_t* phandler, void **pudata);
int setup_auth_user_handler(idip_thmap_t* self, idip_auth_user_t handler, void *udata);

int setup_wireless_handlers(idip_thmap_t *self,
                            idip_wireless_scan_handler_t scan_handler,
                            void* scan_handler_udata,
                            idip_wireless_setup_handler_t setup_handler,
                            void* setup_handler_udata);

int get_persistent_data_max_size(const idip_thmap_t* self, size_t* rsize);
int setup_persistent_data_feature(idip_thmap_t* self,
                                  idip_stream_persistent_data_get_handler_t get_handler, void *get_handler_udata,
                                  idip_stream_persistent_data_set_handler_t set_handler, void *set_handler_udata,
                                  size_t max_size);

int setup_get_interfaces_feature(idip_thmap_t *self,
                                 idip_system_get_interfaces_handler_t handler,
                                 void* udata);

int setup_system_notify_handler(idip_thmap_t* self,
                                idip_stream_system_notify_handler_t handler,
                                void *udata);

int setup_system_update_handler(idip_thmap_t* self,
                                idip_stream_update_version_handler_t handler,
                                void *udata);

int setup_system_ota_handler(idip_thmap_t *self,
                             idip_stream_ota_handler_t handler,
                             size_t maximum_chunk_size,
                             void* udata);

int setup_system_get_interfaces_handler(idip_thmap_t* self,
                                        idip_system_get_interfaces_handler_t handler,
                                        void* udata);

int setup_account_management_handlers(idip_thmap_t *self,
                                      idip_stream_passwd_handler_t passwd_handler,
                                      void* passwd_handler_udata,
                                      idip_stream_system_get_users_handler_t get_users_handler,
                                      void* get_users_handler_udata);

/**
 * Set up a vca @a handler for a @a given target name.
 * @param self - pointer to a map instance
 * @param target - target name. NULL or empty strings are matched to a
 * "default" handler.
 * @param handler - pointer to a handler.
 * @return 0 if success, -1 if errors were occurred.
 */
int setup_vca_handler(idip_thmap_t *self, const char* target,
                      idip_stream_vca_handler_t handler, void* udata);

int setup_system_token_handler(idip_thmap_t* self,
                               idip_system_token_handler_t handler,
                               void* udata);

#endif /* IDIP_TARGETS_MAP_H_INCLUDED */
