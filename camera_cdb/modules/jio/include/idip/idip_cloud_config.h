#ifndef IDIP_CLOUD_CONFIG_H
#define IDIP_CLOUD_CONFIG_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <idip/idip_utils.h>
#include <idip/idip_enum.h>
#include <idip/idip_data_types.h>
#include <idip/idip_cfg_param.h>

// forwards
struct idip_server;
struct idip_stream_config;

/// Synonym for @ref idip_cfg_meta
typedef struct idip_cfg_meta idip_cfg_meta_t;

/**
 * Configuration value hook status codes.
 * See @ref idip_cfg_bool_hook_t, @ref idip_cfg_int_hook_t,
 * @ref idip_cfg_double_hook_t, @ref idip_cfg_string_hook_t,
 * @ref idip_cfg_pos_hook_t, @ref idip_cfg_poly_hook_t and
 * @ref idip_cfg_meta_handle_updates.
 */
typedef enum {
    /// Success. @ref idip_cfg_meta_handle_updates() should update
    /// a value by itself.
    IDIP_CFG_HOOK_OK = 0,
    /// Success. But @ref idip_cfg_meta_handle_updates() SHOULD NOT update
    /// a value because the value was updated by the hook.
    IDIP_CFG_HOOK_DONT_UPDATE_VALUE = 1,
    /// Hook failure. @ref idip_cfg_meta_handle_updates() should not update
    /// a value and should return immediately.
    IDIP_CFG_HOOK_ERR = -1
}
idip_cfg_hook_status_t;

/**
 * Cloud configuration value generic hook type.
 * @param target - target name.
 * @param param_name - cloud configuration parameter name.
 * @param value - pointer to @ref idip_cfg_param_t instance which contains
 * new value of parameter.
 * @param meta - pointer to bound meta info.
 * @param hook_user_data - pointer to Initialized user data.
 */
typedef idip_status_code_t
(*idip_cfg_hook_t)(const char* target,
                   const char* param_name,
                   const idip_cfg_param_t* value,
                   idip_cfg_meta_t* meta,
                   void* hook_user_data);

/**
 * Cloud configuration boolean value hook type. This callback type can be set
 * to handle updates of boolean variables during a call of
 * @ref idip_cfg_meta_handle_updates(). The hook is calling BEFORE actual update
 * of a bound variable. The return value controls update procedure.
 * @param target - a target name.
 * The name is going from @ref idip_cfg_meta_handle_updates() arguments and
 * can be used to implement target dependent logic.
 * @param param_name - cloud configuration parameter name. The name also is
 * going from @ref idip_cfg_meta_handle_updates() arguments.
 * @param value - the new value of a variable to be applied.
 * @param meta - pointer to bound meta info.
 * @param hook_user_data - pointer to some user data which was passed to
 * @ref idip_cfg_meta_handle_updates().
 * @return one of @ref idip_cfg_hook_status_t
 */
typedef idip_cfg_hook_status_t
(*idip_cfg_bool_hook_t)(const char* target,
                        const char* param_name,
                        bool value,
                        idip_cfg_meta_t* meta,
                        void* hook_user_data);

/**
 * Cloud configuration integer value hook type.
 * This callback type can be set to handle an updates of integer variables
 * during a call of @ref idip_cfg_meta_handle_updates() or to handle an updates
 * of @ref IDIP_CFG_CHOICE_INT variables. The hook is calling
 * BEFORE actual update of a bound variable. The return value controls update
 * procedure.
 * @param target - a target name.
 * The name is going from @ref idip_cfg_meta_handle_updates() arguments and
 * can be used to implement target dependent logic.
 * @param param_name - cloud configuration parameter name. The name also is
 * going from @ref idip_cfg_meta_handle_updates() arguments.
 * @param value - the new value of a variable to be applied.
 * @param meta - pointer to bound meta info.
 * @param hook_user_data - pointer to some user data which was passed to
 * @ref idip_cfg_meta_handle_updates().
 * @return one of @ref idip_cfg_hook_status_t
 */
typedef idip_cfg_hook_status_t
(*idip_cfg_int_hook_t)(const char* target,
                       const char* param_name,
                       int64_t value,
                       idip_cfg_meta_t* meta,
                       void* hook_user_data);

/**
 * Cloud configuration double value hook type. This callback type can be set
 * to handle updates of double variables during a call of
 * @ref idip_cfg_meta_handle_updates(). The hook is calling BEFORE actual update
 * of a bound variable. The return value controls update procedure.
 * @param target - a target name.
 * The name is going from @ref idip_cfg_meta_handle_updates() arguments and
 * can be used to implement target dependent logic.
 * @param param_name - cloud configuration parameter name. The name also is
 * going from @ref idip_cfg_meta_handle_updates() arguments.
 * @param value - the new value of a variable to be applied.
 * @param meta - pointer to bound meta info.
 * @param hook_user_data - pointer to some user data which was passed to
 * @ref idip_cfg_meta_handle_updates().
 * @return one of @ref idip_cfg_hook_status_t
 */
typedef idip_cfg_hook_status_t
(*idip_cfg_double_hook_t)(const char* target,
                          const char* param_name,
                          double value,
                          idip_cfg_meta_t* meta,
                          void* hook_user_data);

/**
 * Cloud configuration string value hook type.
 * This callback type can be set to handle updates of @ref IDIP_CFG_STRING,
 * @ref IDIP_CFG_GRID and @ref IDIP_CFG_CHOICE_STR variables during a call of
 * @ref idip_cfg_meta_handle_updates(). The hook is calling BEFORE actual update
 * of a bound variable. The return value controls update procedure.
 * @param target - a target name.
 * The name is going from @ref idip_cfg_meta_handle_updates() arguments and
 * can be used to implement target dependent logic.
 * @param param_name - cloud configuration parameter name. The name also is
 * going from @ref idip_cfg_meta_handle_updates() arguments.
 * @param value - the new value of a variable to be applied.
 * @param meta - pointer to bound meta info.
 * @param hook_user_data - pointer to some user data which was passed to
 * @ref idip_cfg_meta_handle_updates().
 * @return one of @ref idip_cfg_hook_status_t
 */
typedef idip_cfg_hook_status_t
(*idip_cfg_string_hook_t)(const char* target,
                          const char* param_name,
                          const char* value,
                          idip_cfg_meta_t* meta,
                          void* hook_user_data);

/**
 * Cloud configuration position value hook type.
 * This callback type can be set to handle updates of @ref IDIP_CFG_POS
 * (in fact @ref idip_pos_t) variables during a call of @ref idip_cfg_meta_handle_updates().
 * The hook is calling BEFORE actual update of a bound variable. The return
 * value controls update procedure.
 * @param target - a target name.
 * The name is going from @ref idip_cfg_meta_handle_updates() arguments and
 * can be used to implement target dependent logic.
 * @param param_name - cloud configuration parameter name. The name also is
 * going from @ref idip_cfg_meta_handle_updates() arguments.
 * @param value - the new value of a variable to be applied.
 * @param meta - pointer to bound meta info.
 * @param hook_user_data - pointer to some user data which was passed to
 * @ref idip_cfg_meta_handle_updates().
 * @return one of @ref idip_cfg_hook_status_t
 */
typedef idip_cfg_hook_status_t
(*idip_cfg_pos_hook_t)(const char* target,
                      const char* param_name,
                      idip_pos_t value,
                      idip_cfg_meta_t* meta,
                      void* hook_user_data);

/**
 * Cloud configuration polygon/rect/polyline value hook type.
 * This callback type can be set to handle updates of @ref IDIP_CFG_POLYGON,
 * @ref IDIP_CFG_POLYLINE and @ref IDIP_CFG_RECT type variables. A value of these
 * types is always an array of @ref idip_pos_t.
 * The hook is calling BEFORE actual update of a bound variable. The return
 * value controls update procedure.
 * @param target - a target name.
 * The name is going from @ref idip_cfg_meta_handle_updates() arguments and
 * can be used to implement target dependent logic.
 * @param param_name - cloud configuration parameter name. The name also is
 * going from @ref idip_cfg_meta_handle_updates() arguments.
 * @param value - the new value of a variable to be applied.
 * @param meta - pointer to bound meta info.
 * @param hook_user_data - pointer to some user data which was passed to
 * @ref idip_cfg_meta_handle_updates().
 */
typedef idip_cfg_hook_status_t
(*idip_cfg_poly_hook_t)(const char* target,
                        const char* param_name,
                        const idip_pos_t* value,
                        size_t value_len,
                        idip_cfg_meta_t* meta,
                        void* hook_user_data);


/**
 * Helper union type that represent all hook types.
 */
typedef union idip_cfg_hook_union
{
    idip_cfg_hook_t         hook;
    idip_cfg_bool_hook_t    bool_hook;
    idip_cfg_int_hook_t     int_hook;
    idip_cfg_double_hook_t  double_hook;
    idip_cfg_string_hook_t  string_hook;
    idip_cfg_pos_hook_t     pos_hook;
    idip_cfg_poly_hook_t    poly_hook;
}
idip_cfg_hook_union_t;


/**
 * A hook type for read operations.
 */
typedef idip_status_code_t
(*idip_cfg_read_hook_t)(const char* target,
                        const idip_cfg_meta_t* meta,
                        struct idip_stream_config* out_stream,
                        void* hook_user_data);

/**
 * Helper structure to create a generic algorithm that works with configuration.
 * In most cases you don't have to fill this structure directly. Use a set of
 * IDIP_CFG_META_INIT_XXX macro instead.
 */
struct idip_cfg_meta
{
    /// Cloud configuration variable name. See handbook.
    const char* name;
    /// Configuration value type
    idip_cfg_param_value_type_t type;
    /// Pointer to a variable that contains actual value
    union {
        int*         v_bool;
        int64_t*     v_int;
        double*      v_double;
        char*        v_str;
        const char** v_cstr;
        idip_pos_t*  v_pos;
        idip_pos_t** v_poly;
    };
    /// Pointer to variable which contains current length (items count)
    /// for v_poly arrays.
    size_t* v_poly_length;
    /// Pointer to range description.
    union {
        /// Pointer to range of strings
        const char* const*  s_range;
        /// Pointer to range of integers. For @ref IDIP_CFG_INT type
        /// it should point to array of 3 integers (min, max, step) or NULL.
        /// For @ref IDIP_CFG_STRING type it should point to array of 2 integers:
        /// [min_length, max_length] or NULL.
        /// For @ref IDIP_CFG_CHOICE_INT type it should point to a set of integers
        /// which values are a full set of given choice variable.
        /// For @ref IDIP_CFG_POS and @ref IDIP_CFG_GRID types it should point to
        /// an array of 2 items: [width, height].
        /// For @ref IDIP_CFG_POLYGON, @ref IDIP_CFG_POLYLINE types it should point
        /// to an array of 5 items: [ x_min, x_max, y_min, y_max, max_points ].
        /// For @ref IDIP_CFG_RECT type it should point to an array of 4 items:
        /// [ x_min, x_max, y_min, y_max ].
        const int64_t*      i_range;
    };
    /// Count of items in range
    size_t range_length;
    /// Don't analyze hook type. Call generic hook (w_hook.hook) if present.
    int is_generic_w_hook;
    /// Value update hook. If specified then @ref idip_cfg_meta_handle_updates()
    /// will call this hook before variable update.
    idip_cfg_hook_union_t   w_hook;
    /// Value read hook. This hook can be called from
    /// @ref idip_cfg_meta_call_rhooks().
    idip_cfg_read_hook_t    r_hook;

    ///
    int was_updated;
    /// Pointer to additional user data
    void* user_data;
};

typedef struct
{
    idip_cfg_meta_t* meta;          ///< pointer to first element of array
    size_t           meta_count;    ///< array length
}
/**
 * Helper structure for handling arrays of @ref idip_cfg_meta_t.
 * It is used in @ref idip_cfg_meta_find(),
 * @ref idip_cfg_meta_handle_updates(), @ref idip_cfg_meta_call_rhooks(),
 * @ref idip_cfg_meta_put_values_to_stream().
 */
idip_cfg_all_meta_t;

/**
 * Range of 3 elements "on", "off", "auto". It can be used in
 * initialization of @ref idip_cfg_meta.
 */
extern const char* const idip_cfg_range_on_off_auto[3];

/**
 * Range of 2 elements. "50 Hz", "60 Hz".
 */
extern const char* const idip_cfg_range_power_freq[2];

/**
 * Range for @ref IDIP_CFG_INT type that represents default range for integers
 * [MIN_INT64, MAX_INT64]
 */
extern int64_t const idip_cfg_range_int_dflt[3];

/**
 * Range for @ref IDIP_CFG_INT type that represents a positive integers range
 * and zero [0, MAX_INT64]
 */
extern int64_t const idip_cfg_range_int_positive[3];

/**
 * Range for @ref IDIP_CFG_INT type that represents a common enough
 * time interval range (in milliseconds).
 * It cam be used common with XXX_DURARION configuration parameters.
 */
extern int64_t const idip_cfg_range_int_one_day_msec[3];

/**
 * Range for @ref IDIP_CFG_INT type that represents a range [0, 100].
 * A lot of integer values are in percent notation. We recommend use this
 * range for percent-based values.
 */
extern int64_t const idip_cfg_range_percents[3];

/**
 * Same as @ref idip_cfg_range_percents, but represents a range [0, 100]
 * divided by 20 percents. There are values from the range: { 0, 20, 40, 60,
 * 80, 100 }.
 */
extern int64_t const idip_cfg_range_percents_step20[3];

/**
 * Helper macro. Don't use it directly if you don't understand it.
 */
#define _IDIP_STRINGIFY( x ) #x

/**
 * Helper macro. Initialize an @ref idip_cfg_meta instance that describes a
 * boolean type (@ref IDIP_CFG_BOOL) configuration parameter.
 * @param v_name - variable name. Pointer to constant string.
 * @param v_ptr - pointer to a variable where actual value would be stored.
 * @param wr_hook - pointer to a @ref idip_cfg_bool_hook_t type or NULL.
 * @param rd_hook - pointer to a @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define _IDIP_CFG_META_INIT_BOOL( v_name, v_ptr, wr_hook, rd_hook, u_data ) \
{   \
    .name = v_name,  \
    .type = IDIP_CFG_BOOL,  \
    .v_bool = (v_ptr), \
    .v_poly_length = NULL, \
    .s_range = NULL, \
    .range_length = 0, \
    .is_generic_w_hook = 0, \
    .w_hook = (idip_cfg_hook_union_t){ .bool_hook = (wr_hook) }, \
    .r_hook = (rd_hook), \
    .user_data = (u_data), \
}

/**
 * Helper macro. Initialize an @ref idip_cfg_meta instance that describes a
 * integer type (@ref IDIP_CFG_INT) configuration parameter.
 * @param v_name - variable name. Pointer to constant string.
 * @param v_ptr - pointer to an integer variable where actual value would be stored.
 * @param range - pointer to an array of 3 elements of int64_t type. The layout
 * is [min_value, max_value, step]. If NULL then the value can be set to any
 * integer number without limits.
 * @param range_size - count of items in @a range array. Must be 3 or 0.
 * @param wr_hook - pointer to a @ref idip_cfg_int_hook_t type or NULL.
 * @param rd_hook - pointer to a @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define _IDIP_CFG_META_INIT_INT( v_name, v_ptr, range, range_size, wr_hook, rd_hook, u_data ) \
{   \
    .name = v_name,  \
    .type = IDIP_CFG_INT,  \
    .v_int = (v_ptr), \
    .v_poly_length = NULL, \
    .i_range = (range), \
    .range_length = (range_size), \
    .is_generic_w_hook = 0, \
    .w_hook = (idip_cfg_hook_union_t){ .int_hook = (wr_hook) }, \
    .r_hook = (rd_hook), \
    .user_data = (u_data), \
}
//    IDIP_STATIC_ASSERT((range_size == 3) || (range == NULL ),
//        "IDIDP_CFG_INT requires range of 3 elements [min, max, step] or NULL range")

/**
 * Helper macro. Initialize an @ref idip_cfg_meta instance that describes a
 * double type (@ref IDIP_CFG_DOUBLE) configuration parameter.
 * @param v_name - variable name. Pointer to constant string.
 * @param v_ptr - pointer to a double variable where actual value would be stored.
 * @param wr_hook - pointer to a @ref idip_cfg_double_hook_t type or NULL.
 * @param rd_hook - pointer to a @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define _IDIP_CFG_META_INIT_DOUBLE( v_name, v_ptr, wr_hook, rd_hook, u_data ) \
{   \
    .name = v_name,  \
    .type = IDIP_CFG_DOUBLE,  \
    .v_double = v_ptr, \
    .v_poly_length = NULL, \
    .s_range = NULL, \
    .range_length = 0, \
    .is_generic_w_hook = 0, \
    .w_hook = (idip_cfg_hook_union_t){ .double_hook = (wr_hook) }, \
    .r_hook = (rd_hook), \
    .user_data = (u_data), \
}

/**
 * Helper macro. Initialize an @ref idip_cfg_meta instance that describes a
 * string type (@ref IDIP_CFG_STRING) configuration parameter.
 * @param v_name - variable name. Pointer to constant string.
 * @param v_ptr - pointer to a string buffer where actual value would be stored.
 * @param range - pointer to an array of 2 elements of int64_t type. The layout
 * is [min_length, max_length]. If NULL then the range checking isn't doing.
 * @param wr_hook - pointer to a @ref idip_cfg_string_hook_t type or NULL.
 * @param rd_hook - pointer to a @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define _IDIP_CFG_META_INIT_STRING( v_name, v_ptr, range, wr_hook, rd_hook, u_data ) \
{   \
    .name = v_name,  \
    .type = IDIP_CFG_STRING,  \
    .v_str = (v_ptr), \
    .v_poly_length = NULL, \
    .i_range = (range), \
    .range_length = ((range) ? 2 : 0), \
    .is_generic_w_hook = 0, \
    .w_hook = (idip_cfg_hook_union_t){ .string_hook = (wr_hook) }, \
    .r_hook = (rd_hook), \
    .user_data = (u_data), \
}

/**
 * Helper macro. Initialize an @ref idip_cfg_meta instance that describes a
 * grid/mask type (@ref IDIP_CFG_GRID) configuration parameter.
 * @param v_name - variable name. Pointer to constant string.
 * @param v_ptr - pointer to a string buffer where actual value would be stored.
 * @param range - pointer to an array of 2 elements of int64_t type. The layout
 * is [width, height]. Can NOT be NULL.
 * @param range_size - count of items in @a range array. Must equals to 2.
 * @param wr_hook - pointer to a @ref idip_cfg_string_hook_t type or NULL.
 * @param rd_hook - pointer to a @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define _IDIP_CFG_META_INIT_GRID( v_name, v_ptr, range, range_size, wr_hook, rd_hook, u_data ) \
{   \
    .name = v_name,  \
    .type = IDIP_CFG_GRID,  \
    .v_str = (v_ptr), \
    .v_poly_length = NULL, \
    .i_range = (range), \
    .range_length = 2, \
    .is_generic_w_hook = 0, \
    .w_hook = (idip_cfg_hook_union_t){ .string_hook = (wr_hook) }, \
    .r_hook = (rd_hook), \
    .user_data = (u_data), \
}
//IDIP_STATIC_ASSERT(range_size == 2,
//    "IDIDP_CFG_GRID type requires range of 2 elements [width, height]")

/**
 * Helper macro. Initialize an @ref idip_cfg_meta instance that describes a
 * 2d point type (@ref IDIP_CFG_POS) configuration parameter.
 * @param v_name - variable name. Pointer to constant string.
 * @param v_ptr - pointer to a string buffer where actual value would be stored.
 * @param range - pointer to an array of 2 elements of int64_t type. The layout
 * is [width, height]. Can NOT be NULL.
 * @param range_size - count of items in @a range array. Must equals to 2.
 * @param wr_hook - pointer to a @ref idip_cfg_pos_hook_t type or NULL.
 * @param rd_hook - pointer to a @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define _IDIP_CFG_META_INIT_POS( v_name, v_ptr, range, range_size, wr_hook, rd_hook, u_data ) \
{   \
    .name = v_name,  \
    .type = IDIP_CFG_POS,  \
    .v_pos = (v_ptr), \
    .v_poly_length = NULL, \
    .i_range = (range), \
    .range_length = 2, \
    .is_generic_w_hook = 0, \
    .w_hook = (idip_cfg_hook_union_t){ .pos_hook = (wr_hook) }, \
    .r_hook = (rd_hook), \
    .user_data = (u_data), \
}

/**
 * Helper macro. Initialize an @ref idip_cfg_meta instance that describes a
 * choice-of-string type (@ref IDIP_CFG_CHOICE_STR) configuration parameter.
 * @param v_name - variable name. Pointer to constant string.
 * @param v_ptr - pointer to a string buffer where actual value would be stored.
 * @param range - pointer to an array of constant strings. The strings in the
 * array are a full set of possible values of the @a v_ptr. Can NOT be NULL.
 * @param range_size - count of items in @a range array.
 * @param wr_hook - pointer to a @ref idip_cfg_string_hook_t type or NULL.
 * @param rd_hook - pointer to a @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define _IDIP_CFG_META_INIT_CHOICE( v_name, v_ptr, range, range_size, wr_hook, rd_hook, u_data ) \
{   \
    .name = v_name,  \
    .type = IDIP_CFG_CHOICE_STR,  \
    .v_cstr = (v_ptr), \
    .v_poly_length = NULL, \
    .s_range = (range), \
    .range_length = (range_size), \
    .is_generic_w_hook = 0, \
    .w_hook = (idip_cfg_hook_union_t){ .string_hook = (wr_hook) }, \
    .r_hook = (rd_hook), \
    .user_data = (u_data), \
}

/**
 * Helper macro. Initialize an @ref idip_cfg_meta instance that describes a
 * choice-of-int type (@ref IDIP_CFG_CHOICE_INT) configuration parameter.
 * @param v_name - variable name. Pointer to constant string.
 * @param v_ptr - pointer to a int64_t where actual value would be stored.
 * @param range - pointer to an array of int64_t. These values are a full set
 * of possible values of the variable. Can NOT be NULL.
 * @param range_size - count of items in @a range array.
 * @param wr_hook - pointer to a @ref idip_cfg_int_hook_t type or NULL.
 * @param rd_hook - pointer to a @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define _IDIP_CFG_META_INIT_CHOICE_INT( v_name, v_ptr, range, range_size, wr_hook, rd_hook, u_data ) \
{   \
    .name = v_name,  \
    .type = IDIP_CFG_CHOICE_INT,  \
    .v_int = (v_ptr), \
    .v_poly_length = NULL, \
    .i_range = (range), \
    .range_length = (range_size), \
    .is_generic_w_hook = 0, \
    .w_hook = (idip_cfg_hook_union_t){ .int_hook = (wr_hook) }, \
    .r_hook = (rd_hook), \
    .user_data = (u_data), \
}

/**
 * Helper macro. Initialize an @ref idip_cfg_meta instance that describes a
 * poly line type (@ref IDIP_CFG_POLYLINE) configuration parameter.
 * A polyline is an array of points that are points of line segments.
 * @param v_name - variable name. Pointer to constant string.
 * @param v_ptr - pointer to an array (buffer) of @ref idip_pos_t where actual
 * value would be stored.
 * @param v_length - actual length (in points) of the array.
 * @param range - pointer to an array of 5 elements of int64_t. The layout is:
 * [x_min, x_max, y_min, y_max, max_points]. Cannot be NULL.
 * @param range_size - count of items in @a range array.
 * @param wr_hook - pointer to a @ref idip_cfg_poly_hook_t type or NULL.
 * @param rd_hook - pointer to a @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define _IDIP_CFG_META_INIT_POLYLINE( v_name, v_ptr, v_length, range, range_size, wr_hook, rd_hook, u_data ) \
{   \
    .name = v_name,  \
    .type = IDIP_CFG_POLYLINE,  \
    .v_poly = (v_ptr), \
    .v_poly_length = (v_length), \
    .i_range = (range), \
    .range_length = 5, \
    .is_generic_w_hook = 0, \
    .w_hook = (idip_cfg_hook_union_t){ .poly_hook = (wr_hook) }, \
    .r_hook = (rd_hook), \
    .user_data = (u_data), \
}

/**
 * Helper macro. Initialize an @ref idip_cfg_meta instance that describes a
 * polygon type (@ref IDIP_CFG_POLYGON) configuration parameter.
 * A polygon is an array of points that are vertices of polygon.
 * @param v_name - variable name. Pointer to constant string.
 * @param v_ptr - pointer to an array (buffer) of @ref idip_pos_t where actual
 * value would be stored.
 * @param v_length - actual length (in points) of the array.
 * @param range - pointer to an array of 5 elements of int64_t. The layout is:
 * [x_min, x_max, y_min, y_max, max_points]. Cannot be NULL.
 * @param range_size - count of items in @a range array.
 * @param wr_hook - pointer to a @ref idip_cfg_poly_hook_t type or NULL.
 * @param rd_hook - pointer to a @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define _IDIP_CFG_META_INIT_POLYGON( v_name, v_ptr, v_length, range, range_size, wr_hook, rd_hook, u_data ) \
{ \
    .name = v_name,  \
    .type = IDIP_CFG_POLYGON,  \
    .v_poly = (v_ptr), \
    .v_poly_length = (v_length), \
    .i_range = (range), \
    .range_length = 5, \
    .is_generic_w_hook = 0, \
    .w_hook = (idip_cfg_hook_union_t){ .poly_hook = (wr_hook) }, \
    .r_hook = (rd_hook), \
    .user_data = (u_data), \
}

/**
 * Helper macro. Initialize an @ref idip_cfg_meta instance that describes a
 * rectangle type (@ref IDIP_CFG_RECT) configuration parameter.
 * A rectangle is an array of 2 points that are top-left and bottom-right (or
 * top-right and bottom-left) points of the rectangle.
 * @param v_name - variable name. Pointer to constant string.
 * @param v_ptr - pointer to an array (buffer) of @ref idip_pos_t where actual
 * value would be stored.
 * @param v_length - pointer to a variable where current count of vertexes of
 * the rectangle is stored.
 * @param range - pointer to an array of 4 elements of int64_t. The layout is:
 * [x_min, x_max, y_min, y_max]. Cannot be NULL.
 * @param range_size - count of items in @a range array.
 * @param wr_hook - pointer to a @ref idip_cfg_poly_hook_t type or NULL.
 * @param rd_hook - pointer to a @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define _IDIP_CFG_META_INIT_RECT( v_name, v_ptr, v_length, range, range_size, wr_hook, rd_hook, u_data ) \
{   \
    .name = v_name,  \
    .type = IDIP_CFG_RECT,  \
    .v_poly = (v_ptr), \
    .v_poly_length = (v_length), \
    .i_range = range, \
    .range_length = 4, \
    .is_generic_w_hook = 0, \
    .w_hook = (idip_cfg_hook_union_t){ .poly_hook = (wr_hook) }, \
    .r_hook = (rd_hook), \
    .user_data = (u_data), \
}

/**
 * Initialize an @a idip_cfg_meta_t instance with a variable that enables
 * or disables all video streams from camera. It should works like a "mute" signal.
 * @param v_ptr - pointer to a "mute" variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_GENERAL_ONLINE_MODE( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("general/online_mode", (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a string variable
 * that controls timezone camera setting.
 * @param v_ptr - pointer to a variable.
 * @param range - pointer to an array of constant strings where time zone names are.
 * @param range_szie - count of items in the @a range.
 * @param w_hook - an update hook of @ref idip_cfg_string_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_GENERAL_TIMEZONE( v_ptr, range, range_szie, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_CHOICE("general/timezone", \
        (v_ptr), (range), (range_szie), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with pointer to a variable
 * that enables or disables onboard microphone. The setting allows to send
 * audio data and allows to work a sound detection subsystem.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_GENERAL_MICROPHONE_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("microphone/enabled", (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with pointer to a variable
 * that enables or disables onboard status LEDs.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_LED_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("led/enabled", (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with pointer to a variable
 * that controls onboard infrared lighting.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_string_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_NIGHT_VISION_MODE( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_CHOICE("night_vision/mode", \
        (v_ptr), idip_cfg_range_on_off_auto, 3, (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with pointer to a variable
 * that allows to control a correlation with power frequency.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_string_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_GENERAL_POWER_FREQ( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_CHOICE("general/power_freq", \
        (v_ptr), idip_cfg_range_power_freq, 2, (w_hook), (r_hook), (u_data))


/**
 * Initialize an @a idip_cfg_meta_t instance with pointer to a variable
 * that allows to set up resolution of N-th stream. It is available for three
 * streams: 0, 1, 2 (N=0..2). The stream 0 is a main video stream, the stream 1
 * is a sub stream, the stream 2 is a third stream.
 * @param N - a number of stream.
 * @param v_ptr - pointer to a variable.
 * @param range - array of constant string that are describing a full set
 * of supported resolution of the stream.
 * @param range_size - @a range elements count.
 * @param w_hook - an update hook of @ref idip_cfg_string_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_STREAMS_N_RESOLUTION( N, v_ptr, range, range_size, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_CHOICE("streams/" _IDIP_STRINGIFY(N) "/resolution", \
        (v_ptr), (range), (range_size), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with pointer to a variable
 * that allows to set up a bitrate of N-th stream.
 * @param N - a number of stream.
 * @param v_ptr - pointer to a variable.
 * @param range - array of int64_t that are describing a full set
 * of supported stream bitrates.
 * @param range_size - @a range elements count.
 * @param w_hook - an update hook of @ref idip_cfg_int_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_STREAMS_N_BITRATE( N, v_ptr, range, range_size, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_CHOICE_INT("streams/" _IDIP_STRINGIFY(N) "/bitrate", \
        (v_ptr), (range), (range_size), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with pointer to a variable
 * that allows to set up a FPS of N-th stream.
 * @param N - a number of stream.
 * @param v_ptr - pointer to a variable.
 * @param range - array of int64_t that are describing a full set
 * of supported stream FPS.
 * @param range_size - @a range elements count.
 * @param w_hook - an update hook of @ref idip_cfg_int_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_STREAMS_N_FPS( N, v_ptr, range, range_size, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_CHOICE_INT("streams/" _IDIP_STRINGIFY(N) "/fps", \
        (v_ptr), (range), (range_size), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with pointer to a variable
 * that allows to set up a video codec name for N-th stream.
 * @param N - a number of stream.
 * @param v_ptr - pointer to a variable.
 * @param range - array of constant string that are describing a full set
 * of supported stream codecs.
 * @param range_size - @a range elements count.
 * @param w_hook - an update hook of @ref idip_cfg_string_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_STREAMS_N_VCODEC( N, v_ptr, range, range_size, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_CHOICE("streams/" _IDIP_STRINGIFY(N) "/codec", \
        (v_ptr), (range), (range_size), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with pointer to a variable
 * that allows to set up a audio codec name for N-th stream.
 * @param N - a number of stream.
 * @param v_ptr - pointer to a variable.
 * @param range - array of constant string that are describing a full set
 * of supported stream codecs.
 * @param range_size - @a range elements count.
 * @param w_hook - an update hook of @ref idip_cfg_string_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_STREAMS_N_ACODEC( N, v_ptr, range, range_size, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_CHOICE("streams/" _IDIP_STRINGIFY(N) "/acodec", \
        (v_ptr), (range), (range_size), (w_hook), (r_hook), (u_data))


/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that controls OSD transparency.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_OSD_TRANSPARENT( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("osd/transparent", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that controls OSD flashing.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_OSD_FLASHING( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("osd/flashing", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that controls visibility of date and time in OSD.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_OSD_DATETIME_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("osd/datetime/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables displaying week number in OSD.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_OSD_DATETIME_DISPLAY_WEEK( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("osd/datetime/display_week", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that controls time displaying format between 24/12 hours format.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_OSD_DATETIME_24H( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("osd/datetime/24h", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that controls date and time displaying format from predefined format set.
 * @param v_ptr - pointer to a variable.
 * @param range - array of constant strings that are describing possible date
 * and time formats.
 * @param range_size - @a range elements count.
 * @param w_hook - an update hook of @ref idip_cfg_string_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_OSD_DATETIME_FORMAT( v_ptr, range, range_size, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_CHOICE("osd/datetime/format", \
        (v_ptr), (range), (range_size), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that controls position on the screen where date and time is displaying.
 * @param v_ptr - pointer to a variable.
 * @param range - array of 2 int64_t items. The layout is: [width, height].
 * @param w_hook - an update hook of @ref idip_cfg_string_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_OSD_DATETIME_POS( v_ptr, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_POS("osd/datetime/pos", \
        (v_ptr), (range), 2, (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that controls camera name visibility on a screen.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_OSD_NAME_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("osd/name/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a string
 * that contain OSD camera name text.
 * @param v_ptr - pointer to a text buffer.
 * @param range - array of 2 int64_t with the layout: [min_length = 0, max_length].
 * @param w_hook - an update hook of @ref idip_cfg_string_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_OSD_NAME_TEXT( v_ptr, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_STRING("osd/name/text", \
        (v_ptr), (range), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that controls position on the screen where camera name is displaying.
 * @param v_ptr - pointer to a variable.
 * @param range - array of 2 int64_t items. The layout is: [width, height].
 * @param w_hook - an update hook of @ref idip_cfg_string_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_OSD_NAME_POS( v_ptr, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_POS("osd/name/pos", \
        (v_ptr), (range), 2, (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a string
 * that contains OSD custom text.
 * @param N - OSD custom text number (Should be one of 0..4).
 * @param v_ptr - pointer to a text buffer.
 * @param range - array of 2 int64_t with the layout: [min_length = 0, max_length].
 * @param w_hook - an update hook of @ref idip_cfg_string_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_OSD_CUSTOM_N_TEXT( N, v_ptr, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_STRING("osd/custom/" _IDIP_STRINGIFY(N) "/text", \
        (v_ptr), (range), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that controls a position on the screen where N-th custom text is displaying.
 * @param N - OSD custom text number (Should be one of 0..4).
 * @param v_ptr - pointer to a variable.
 * @param range - array of 2 int64_t items. The layout is: [width, height].
 * @param w_hook - an update hook of @ref idip_cfg_string_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_OSD_CUSTOM_N_POS( N, v_ptr, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_POS("osd/custom/" _IDIP_STRINGIFY(N) "/pos", \
        (v_ptr), (range), 2, (w_hook), (r_hook), (u_data))


/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables sound decector.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_SOUND_DETECTION_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("sound_detection/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that controls sound detector sensitivity (volume treshold) in percents.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_int_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_SOUND_DETECTION_SENSITIVITY( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("sound_detection/sensitivity", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

/**
 * The same as @ref IDIP_CFG_META_INIT_SOUND_DETECTION_SENSITIVITY but it allows
 * to setup sensitivity by 20 percents per step. In this way a set of {0%, 20%,
 * 40%, 60%, 80%, 100%} is a valid set of the setting value.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_int_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_SOUND_DETECTION_SENSITIVITY_20EACH( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("sound_detection/sensitivity", \
        (v_ptr), idip_cfg_range_percents_step20, 3, (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables PIR decector.
 *
 * @note PIR detetector is differ from motion detector. Motion detectors works
 * by video stream analyzing, PIR detector use infrared hardware solution.
 *
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_PIR_DETECTION_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("pir_detection/enabled", (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables motion detector algorithm for all detectors.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_int_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_MOTION_DETECTION_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("motion_detector/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that controls a @a N-th motion detector sensitivity in percents. The setting
 * assumes that its value should be in a range of [0..100].
 * @param N - motion detector number.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_int_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_MOTION_DETECTION_SENSITIVITY( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("motion_detector/" _IDIP_STRINGIFY(N) "/sensitivity", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that controls a @a N-th motion detector treshold in percents. The setting
 * assumes that its value should be in a range of [0..100].
 * @param N - motion detector number.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_int_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_MOTION_DETECTION_THRESHOLD( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("motion_detector/" _IDIP_STRINGIFY(N) "/threshold", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that controls a @a N-th motion detector zone.
 * @param N - motion detector number.
 * @param v_ptr - pointer to a variable.
 * @param range - pointer to an array of 2 int64_t items which contains a screen
 * widht and screen height in a cells. In other words, the layout is [width, height].
 * @param w_hook - an update hook of @ref idip_cfg_string_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_MOTION_DETECTION_ZONES( N, v_ptr, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_GRID("motion_detector/" _IDIP_STRINGIFY(N) "/zones", \
        (v_ptr), (range), 2, (w_hook), (r_hook), (u_data))


#define IDIP_CFG_META_INIT_PRIVACY_MASK_N_ENABLED( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("privacy_mask/" _IDIP_STRINGIFY(N) "/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_PRIVACY_MASK_N_MASK( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_POLYGON("privacy_mask/" _IDIP_STRINGIFY(N) "/mask", \
        (v_ptr), (v_ptr_len), (range), 5, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_PRIVACY_MASK_N_MASK_RECT( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_RECT("privacy_mask/" _IDIP_STRINGIFY(N) "/mask", \
        (v_ptr), (v_ptr_len), (range), 4, (w_hook), (r_hook), (u_data))


/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables a line detection algorithm for all segments.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_LINE_DETECTION_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("line_detection/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables N-th line detection segment.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_LINE_DETECTION_N_ENABLED( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("line_detection/" _IDIP_STRINGIFY(N) "/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_LINE_DETECTION_N_SEGMENTS( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_POLYLINE("line_detection/" _IDIP_STRINGIFY(N) "/segments", \
        (v_ptr), (v_ptr_len), (range), 5, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_LINE_DETECTION_N_DIRECTION( N, v_ptr, range, range_size, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_CHOICE( "line_detection/" _IDIP_STRINGIFY(N) "/direction", \
        (v_ptr), (range), (range_size), (w_hook), (r_hook), (u_data) )

#define IDIP_CFG_META_INIT_LINE_DETECTION_N_SENSITIVITY( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("line_detection/" _IDIP_STRINGIFY(N) "/sensitivity", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_LINE_DETECTION_N_SENSITIVITY_20EACH( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("line_detection/" _IDIP_STRINGIFY(N) "/sensitivity", \
        (v_ptr), idip_cfg_range_percents_step20, 3, (w_hook), (r_hook), (u_data))


/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables a field detection algorithm for all regions.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_FIELD_DETECTION_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("field_detection/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables N-th field detection region.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_FIELD_DETECTION_N_ENABLED( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("field_detection/" _IDIP_STRINGIFY(N) "/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_FIELD_DETECTION_N_FIELD( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_POLYGON("field_detection/" _IDIP_STRINGIFY(N) "/field", \
        (v_ptr), (v_ptr_len), (range), 5, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_FIELD_DETECTION_N_FIELD_RECT( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_RECT("field_detection/" _IDIP_STRINGIFY(N) "/field", \
        (v_ptr), (v_ptr_len), (range), 4, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_FIELD_DETECTION_N_SENSITIVITY( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("field_detection/" _IDIP_STRINGIFY(N) "/sensitivity", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_FIELD_DETECTION_N_SENSITIVITY_20EACH( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("field_detection/" _IDIP_STRINGIFY(N) "/sensitivity", \
        (v_ptr), idip_cfg_range_percents_step20, 3, (w_hook), (r_hook), (u_data))

/**
 * @note We recommend to use @ref idip_cfg_range_int_one_day_msec for @a range parameter.
 */
#define IDIP_CFG_META_INIT_FIELD_DETECTION_N_TIME_INTERVAL( N, v_ptr, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("field_detection/" _IDIP_STRINGIFY(N) "/time_interval", \
        (v_ptr), (range), 3, (w_hook), (r_hook), (u_data))


/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables a loitering detection algorithm for all regions.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_LOITERING_DETECTION_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("loitering_detection/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables N-th loitering detection region.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_LOITERING_DETECTION_N_ENABLED( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("loitering_detection/" _IDIP_STRINGIFY(N) "/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_LOITERING_DETECTION_N_FIELD( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_POLYGON("loitering_detection/" _IDIP_STRINGIFY(N) "/field", \
        (v_ptr), (v_ptr_len), (range), 5, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_LOITERING_DETECTION_N_FIELD_RECT( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_RECT("loitering_detection/" _IDIP_STRINGIFY(N) "/field", \
        (v_ptr), (v_ptr_len), (range), 4, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_LOITERING_DETECTION_N_TIME_THRESHOLD( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("loitering_detection/" _IDIP_STRINGIFY(N) "/time_threshold", \
        (v_ptr), idip_cfg_range_int_positive, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_LOITERING_DETECTION_N_SENSITIVITY( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("loitering_detection/" _IDIP_STRINGIFY(N) "/sensitivity", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_LOITERING_DETECTION_N_SENSITIVITY_20EACH( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("loitering_detection/" _IDIP_STRINGIFY(N) "/sensitivity", \
        (v_ptr), idip_cfg_range_percents_step20, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_LOITERING_DETECTION_N_IS_PERIODIC_NOTIFY( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("loitering_detection/" _IDIP_STRINGIFY(N) "/is_periodic_notify", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_LOITERING_DETECTION_N_TIME_INTERVAL( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("loitering_detection/" _IDIP_STRINGIFY(N) "/time_interval", \
        (v_ptr), idip_cfg_range_int_one_day_msec, 3, (w_hook), (r_hook), (u_data))


/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables a crowd detection algorithm for all regions.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_CROWD_DETECTION_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("crowd_detection/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables N-th crowd detection region.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_CROWD_DETECTION_N_ENABLED( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("crowd_detection/" _IDIP_STRINGIFY(N) "/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_CROWD_DETECTION_N_FIELD( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_POLYGON("crowd_detection/" _IDIP_STRINGIFY(N) "/field", \
        (v_ptr), (v_ptr_len), (range), 5, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_CROWD_DETECTION_N_FIELD_RECT( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_RECT("crowd_detection/" _IDIP_STRINGIFY(N) "/field", \
        (v_ptr), (v_ptr_len), (range), 4, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_CROWD_DETECTION_N_OVERLAP_AREA_THRESHOLD( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("crowd_detection/" _IDIP_STRINGIFY(N) "/overlap_area_threshold", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_CROWD_DETECTION_N_NUM_OF_OBJECTS_THRESHOLD( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("crowd_detection/" _IDIP_STRINGIFY(N) "/num_of_objects_threshold", \
        (v_ptr), idip_cfg_range_int_positive, 3, (w_hook), (r_hook), (u_data))


/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables a tampering detection algorithm for all regions.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("tampering_detection/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables N-th tampering detection region.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_ENABLED( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("tampering_detection/" _IDIP_STRINGIFY(N) "/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_THRESHOLD( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("tampering_detection/" _IDIP_STRINGIFY(N) "/threshold", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_FIELD( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_POLYGON("tampering_detection/" _IDIP_STRINGIFY(N) "/field", \
        (v_ptr), (v_ptr_len), (range), 5, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_FIELD_RECT( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_RECT("tampering_detection/" _IDIP_STRINGIFY(N) "/field", \
        (v_ptr), (v_ptr_len), (range), 4, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_GLOBAL_SCENE_CHANGE_ENABLED( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("tampering_detection/" _IDIP_STRINGIFY(N) "/global_scene_change/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_GLOBAL_SCENE_CHANGE_THRESHOLD( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("tampering_detection/" _IDIP_STRINGIFY(N) "/global_scene_change/threshold", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

/**
 * @note We recommend to use @ref idip_cfg_range_int_one_day_msec for @a range parameter.
 */
#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_GLOBAL_SCENE_CHANGE_DURATION( N, v_ptr, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("tampering_detection/" _IDIP_STRINGIFY(N) "/global_scene_change/duration", \
        (v_ptr), (range), 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_IMAGE_TOO_DARK_ENABLED( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("tampering_detection/" _IDIP_STRINGIFY(N) "/image_too_dark/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_IMAGE_TOO_DARK_THRESHOLD( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("tampering_detection/" _IDIP_STRINGIFY(N) "/image_too_dark/threshold", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

/**
 * @note We recommend to use @ref idip_cfg_range_int_one_day_msec for @a range parameter.
 */
#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_IMAGE_TOO_DARK_DURATION( N, v_ptr, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("tampering_detection/" _IDIP_STRINGIFY(N) "/image_too_dark/duration", \
        (v_ptr), (range), 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_IMAGE_TOO_BRIGHT_ENABLED( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("tampering_detection/" _IDIP_STRINGIFY(N) "/image_to_bright/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_IMAGE_TOO_BRIGHT_THRESHOLD( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("tampering_detection/" _IDIP_STRINGIFY(N) "/image_to_bright/threshold", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

/**
 * @note We recommend to use @ref idip_cfg_range_int_one_day_msec for @a range parameter.
 */
#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_IMAGE_TOO_BRIGHT_DURATION( N, v_ptr, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("tampering_detection/" _IDIP_STRINGIFY(N) "/image_to_bright/duration", \
        (v_ptr), (range), 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_SIGNAL_LOSS_ENABLED( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("tampering_detection/" _IDIP_STRINGIFY(N) "/signal_loss/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_SIGNAL_LOSS_THRESHOLD( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("tampering_detection/" _IDIP_STRINGIFY(N) "/signal_loss/threshold", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

/**
 * @note We recommend to use @ref idip_cfg_range_int_one_day_msec for @a range parameter.
 */
#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_SIGNAL_LOSS_DURATION( N, v_ptr, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("tampering_detection/" _IDIP_STRINGIFY(N) "/signal_loss/duration", \
        (v_ptr), (range), 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_IMAGE_TOO_BLURRY_ENABLED( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("tampering_detection/" _IDIP_STRINGIFY(N) "/image_too_blurry/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_IMAGE_TOO_BLURRY_THRESHOLD( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("tampering_detection/" _IDIP_STRINGIFY(N) "/image_too_blurry/threshold", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

/**
 * @note We recommend to use @ref idip_cfg_range_int_one_day_msec for @a range parameter.
 */
#define IDIP_CFG_META_INIT_TAMPERING_DETECTION_N_IMAGE_TOO_BLURRY_DURATION( N, v_ptr, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("tampering_detection/" _IDIP_STRINGIFY(N) "/image_too_blurry/duration", \
        (v_ptr), (range), 3, (w_hook), (r_hook), (u_data))



#define IDIP_CFG_META_INIT_FACE_DETECTION_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("face_detection/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_FACE_DETECTION_FIELD(v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_POLYGON("face_detection/field", \
        (v_ptr), (v_ptr_len), (range), 5, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_FACE_DETECTION_FIELD_RECT( v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_RECT("face_detection/field", \
        (v_ptr), (v_ptr_len), (range), 4, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_FACE_DETECTION_MIN_FACE_SIZE( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("face_detection/min_face_size", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_FACE_DETECTION_MAX_FACE_SIZE( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("face_detection/max_face_size", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_FACE_DETECTION_MIN_FACE_SIZE_PIXELS( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("face_detection/min_face_size", \
        (v_ptr), idip_cfg_range_int_positive, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_FACE_DETECTION_MAX_FACE_SIZE_PIXELS( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("face_detection/max_face_size", \
        (v_ptr), idip_cfg_range_int_positive, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_FACE_DETECTION_SENSITIVITY( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("face_detection/sensitivity", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_FACE_DETECTION_BLUR_THRESHOLD( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("face_detection/blur_threshold", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_FACE_DETECTION_CONTRAST_THRESHOLD( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("face_detection/contrast_threshold", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))



#define IDIP_CFG_META_INIT_ROI_N_ENABLED( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("roi/" _IDIP_STRINGIFY(N) "/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_ROI_N_FIELD( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_POLYGON("roi/" _IDIP_STRINGIFY(N) "/roi", \
        (v_ptr), (v_ptr_len), (range), 5, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_ROI_N_FIELD_RECT( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_RECT("roi/" _IDIP_STRINGIFY(N) "/roi", \
        (v_ptr), (v_ptr_len), (range), 4, (w_hook), (r_hook), (u_data))


/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables a object removal detection algorithm for all regions.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_REMOVAL_OBJECT_DETECTION_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("removal_object_detection/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables N-th object removal detection region.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_REMOVAL_OBJECT_DETECTION_N_ENABLED( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("removal_object_detection/" _IDIP_STRINGIFY(N) "/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_REMOVAL_OBJECT_DETECTION_N_SENSITIVITY( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("removal_object_detection/" _IDIP_STRINGIFY(N) "/sensitivity", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_REMOVAL_OBJECT_DETECTION_N_ID( v_ptr, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_STRING("removal_object_detection/" _IDIP_STRINGIFY(N) "/id", \
        (v_ptr), (range), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_REMOVAL_OBJECT_DETECTION_N_FIELD( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_POLYGON("removal_object_detection/" _IDIP_STRINGIFY(N) "/field", \
        (v_ptr), (v_ptr_len), (range), 5, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_REMOVAL_OBJECT_DETECTION_N_FIELD_RECT( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_RECT("removal_object_detection/" _IDIP_STRINGIFY(N) "/field", \
        (v_ptr), (v_ptr_len), (range), 4, (w_hook), (r_hook), (u_data))


/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables a unclaimed object detection algorithm for all regions.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_UNCLAIMED_OBJECT_DETECTION_ENABLED( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("unclaimed_object_detection/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize an @a idip_cfg_meta_t instance with a pointer to a variable
 * that enables or disables N-th unclaimed detection region.
 * @param v_ptr - pointer to a variable.
 * @param w_hook - an update hook of @ref idip_cfg_bool_hook_t type or NULL.
 * @param r_hook - a read hook of @ref idip_cfg_read_hook_t type or NULL.
 * @param u_data - pointer to some user data.
 */
#define IDIP_CFG_META_INIT_UNCLAIMED_OBJECT_DETECTION_N_ENABLED( N, v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("unclaimed_object_detection/" _IDIP_STRINGIFY(N) "/enabled", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_UNCLAIMED_OBJECT_DETECTION_N_SENSITIVITY( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("unclaimed_object_detection/" _IDIP_STRINGIFY(N) "/sensitivity", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_UNCLAIMED_OBJECT_DETECTION_N_DETECTION_INTERVAL( v_ptr, range, range_size, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_CHOICE("unclaimed_object_detection/" _IDIP_STRINGIFY(N) "/detection_interval", \
        (v_ptr), (range), (range_size), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_UNCLAIMED_OBJECT_DETECTION_N_FIELD( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_POLYGON("unclaimed_object_detection/" _IDIP_STRINGIFY(N) "/field", \
        (v_ptr), (v_ptr_len), (range), 5, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_UNCLAIMED_OBJECT_DETECTION_N_FIELD_RECT( N, v_ptr, v_ptr_len, range, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_RECT("unclaimed_object_detection/" _IDIP_STRINGIFY(N) "/field", \
        (v_ptr), (v_ptr_len), (range), 4, (w_hook), (r_hook), (u_data))


/**
 * 180 degrees image rotation (based on flip + mirror transformation)
 */
#define IDIP_CFG_META_INIT_IMAGE_UPSIDE_DOWN( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("image/upside_down", \
        (v_ptr), (w_hook), (r_hook), (u_data))

/**
 * Initialize a meta information structure that describes a boolean value which
 * allows user to rotate the image by clockwise for 90 degrees.
 */
#define IDIP_CFG_META_INIT_IMAGE_ROTATE( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("image/rotate", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_IMAGE_BRIGHTNESS( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("image/brightness", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_IMAGE_CONTRAST( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("image/contrast", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_IMAGE_SATURATION( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("image/saturation", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_IMAGE_HUE( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("image/hue", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

/**
 * switch WideDynamicRange feauture of image contrast compensation
 */
#define IDIP_CFG_META_INIT_IMAGE_WDR( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_BOOL("image/wdr", \
        (v_ptr), (w_hook), (r_hook), (u_data))

#define IDIP_CFG_META_INIT_IMAGE_WDR_LEVEL( v_ptr, w_hook, r_hook, u_data ) \
    _IDIP_CFG_META_INIT_INT("image/wdr_level", \
        (v_ptr), idip_cfg_range_percents, 3, (w_hook), (r_hook), (u_data))

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Look up a instance of an @a idip_cfg_meta_t which was initialized with
 * the @a name in meta information array @a all_meta.
 * @param all_meta - meta information array descriptor.
 * @param name - variable name to find.
 * @return pointer to a meta information instance or NULL if not found.
 */
idip_cfg_meta_t*
idip_cfg_meta_find(idip_cfg_all_meta_t all_meta, const char* name);

/**
 * Handle update for cloud configuration parameters are described in
 * a @a all_meta array. The function is trying to update some device settings
 * an call callbacks that were bound to these setting variables.
 *
 * @param all_meta - meta information array.
 * @param target - a target name. As a @a stream this value can be passed from
 * a @ref idip_stream_config_t parameters.
 * @param params - array of pointers to a set of new values from cloud. These
 * values should be applied to the settings.
 * @param params_count - count of elements in @a params array.
 * @param user_data - pointer to user data. This pointer will be passed to
 * all hooks which are bound to setting variables.
 * @return @ref IDIP_OK if success or @ref IDIP_GENERIC_ERROR if failure.
 */
idip_status_code_t
idip_cfg_meta_handle_updates(idip_cfg_all_meta_t all_meta,
                             const char* target,
                             const idip_cfg_param_t** params,
                             size_t params_count,
                             void* user_data);

/**
 * Call @a idip_cfg_meta_t @a r_hook (if it is set) for each setting variable
 * in a ginven @a all_meta array.
 * @param all_meta - meta information array.
 * @param stream - pointer to output stream
 * @param target - target name
 * @param user_data - pointer to user data. The value will be passed to all
 * callbacks.
 */
idip_status_code_t
idip_cfg_meta_call_rhooks(idip_cfg_all_meta_t all_meta,
                          struct idip_stream_config* stream,
                          const char* target,
                          void* user_data);

/**
 * Collect actual values and put them into an output @a stream.
 * @param all_meta - meta information array.
 * @param stream - an output stream.
 * @param only_modified - if @a true then the function will put values where
 * @a was_updated attribute is set. Instead the function will ignore the
 * attribute.
 */
idip_status_code_t
idip_cfg_meta_put_values_to_stream(idip_cfg_all_meta_t all_meta,
                                   struct idip_stream_config* stream,
                                   int only_modified);

/**
 * Modify a range description for a given @a meta.
 * This function is suitable for @a meta that was initialized as
 * @ref IDIP_CFG_CHOICE_INT or @ref IDIP_CFG_CHOICE_STR only.
 *
 * A meta information for choice types is an array of constants. In some cases
 * it is convenient to decrease the count of items in set to show a sub set
 * and then restore the original count. This function modified @a meta->range_count
 * and update the current value in case when it is not in new range.
 *
 * @param meta - pointer to a modifying meta info.
 * @param upper_limit - a new range size.
 * @return 0 if success, -1 if errors.
 */
int idip_cfg_meta_limit_range(idip_cfg_meta_t* meta, size_t upper_limit);

#ifdef __cplusplus
}
#endif

#endif // IDIP_CLOUD_CONFIG_H
