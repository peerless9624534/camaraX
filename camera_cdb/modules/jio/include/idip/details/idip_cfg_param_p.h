/**
 * IDIP cloud configuration parameter private implementation hader file
 */
#ifndef IDIP_CFG_PARAM_P_H
#define IDIP_CFG_PARAM_P_H

#include <stdint.h>
#include <stddef.h>
#include <idip/idip_data_types.h>
#include <idip/idip_cfg_param.h>
#include <idip/idip_cloud_config.h>

typedef enum idip_cfg_value_type
{
    // basic types
    IDIP_CFG_VALUE_TYPE_BOOL        = IDIP_CFG_BOOL,
    IDIP_CFG_VALUE_TYPE_INT         = IDIP_CFG_INT,
    IDIP_CFG_VALUE_TYPE_DOUBLE      = IDIP_CFG_DOUBLE,
    IDIP_CFG_VALUE_TYPE_STRING      = IDIP_CFG_STRING,
    IDIP_CFG_VALUE_TYPE_POSITION    = IDIP_CFG_POS,

    // advanced types
    IDIP_CFG_VALUE_TYPE_CHOICE_STR  = IDIP_CFG_CHOICE_STR,
    IDIP_CFG_VALUE_TYPE_CHOICE_INT  = IDIP_CFG_CHOICE_INT,
    IDIP_CFG_VALUE_TYPE_GRID        = IDIP_CFG_GRID,
    IDIP_CFG_VALUE_TYPE_POLYLINE    = IDIP_CFG_POLYLINE,
    IDIP_CFG_VALUE_TYPE_POLYGON     = IDIP_CFG_POLYGON,
    IDIP_CFG_VALUE_TYPE_RECT        = IDIP_CFG_RECT,

    // Special type which can be interpreted as POLYLINE, POLYGON and RECT types
    IDIP_CFG_VALUE_TYPE_POLY_GENERIC,
}
idip_cfg_value_type_t;

typedef struct idip_cfg_int
{
    int64_t value;
    int64_t min;
    int64_t max;
    int64_t step;
}
idip_cfg_int_t;

typedef struct idip_cfg_double
{
    double value;
}
idip_cfg_double_t;

typedef struct idip_cfg_str
{
    uint32_t min_length;
    uint32_t max_length;
    uint32_t offset;
}
idip_cfg_str_t;

typedef struct idip_cfg_pos
{
    int64_t x;
    int64_t y;
    int64_t width;
    int64_t height;
}
idip_cfg_pos_t;

typedef struct idip_cfg_grid
{
    uint64_t width;
    uint64_t height;
    uint32_t offset;
}
idip_cfg_grid_t;

typedef struct idip_cfg_choice
{
    uint32_t offset; // offset to value
    uint32_t range_offset; // offset to range offsets
    uint32_t range_length;
}
idip_cfg_choice_s_t;

typedef struct idip_cfg_choice_int
{
    int64_t  value;
    uint32_t range_offset;
    uint32_t range_length;
}
idip_cfg_choice_i_t;

/// Configuration value is an continuous memory area, which divided to
/// some structural pieces of data relative to value type. This structure
/// stores offsets to arrays of idip_pos_t values and its lengths.
/// We don't save addresses of arrays because copy of configuration value
/// invalidate addresses.
typedef struct idip_cfg_poly_helper
{
    size_t val_offset;
    size_t range_offset;
    size_t val_size;
    size_t range_size;
}
idip_cfg_poly_helper_t;

typedef union idip_cfg_value
{
    int                 v_bool;
    idip_cfg_int_t      v_int;
    idip_cfg_double_t   v_double;
    idip_cfg_str_t      v_string;
    idip_cfg_pos_t      v_pos;
    idip_cfg_grid_t     v_grid;
    idip_cfg_choice_s_t v_choice;
    idip_cfg_choice_i_t v_choice_int;
    idip_cfg_poly_helper_t v_poly;
}
idip_cfg_value_t;

/// Structure to store type + name + value + range in sequentially memory block.
/// @note We use the next memory layout:
/// [ idip_cfg_param | name | value | range ]
/// @note Strings are storing with ending '\0' (we allocate extra space for them)
struct idip_cfg_param
{
    // Item type
    idip_cfg_value_type_t type;
    // Constant value
    idip_cfg_value_t      value;
    //
    size_t                data_size;
    // variance-length data
    char                  data[0];
};


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//------------------------------------------------------------------------------
const char*
idip_cfg_param_str_val_ptr(const struct idip_cfg_param* p);

const char*
idip_cfg_param_grid_val_ptr(const struct idip_cfg_param* p);

const char*
idip_cfg_param_choice_val_ptr(const struct idip_cfg_param* p);

const uint32_t*
idip_cfg_param_choice_range_ptr(const struct idip_cfg_param* p);

const char*
idip_cfg_param_choice_range_val_ptr_at(const struct idip_cfg_param* p, unsigned int i);

const uint64_t*
idip_cfg_param_choicei_range_ptr(const struct idip_cfg_param* p);

const idip_pos_t*
idip_cfg_param_poly_val_ptr(const struct idip_cfg_param* p);

const idip_axis_range_t*
idip_cfg_param_poly_range_ptr(const struct idip_cfg_param* p);

//------------------------------------------------------------------------------
idip_cfg_param_t*
idip_cfg_param_new_bool(const char* name, int value);

idip_cfg_param_t*
idip_cfg_param_new_int(const char* name, int64_t value, int64_t min,
                       int64_t max, int64_t step);

idip_cfg_param_t*
idip_cfg_param_new_double(const char* name, double value);

idip_cfg_param_t*
idip_cfg_param_new_string(const char* name, const char* value, uint32_t min_len,
                          uint32_t max_len);

idip_cfg_param_t *
idip_cfg_param_new_string_mpk(const char* name, const char* value, uint32_t value_len);

idip_cfg_param_t*
idip_cfg_param_new_pos(const char* name, const idip_pos_t* value,
                       int64_t width, int64_t height);

idip_cfg_param_t*
idip_cfg_param_new_grid(const char* name,  const char* value,
                        uint64_t width, uint64_t height);

idip_cfg_param_t*
idip_cfg_param_new_choice(const char* name,  const char* value,
                          const char* const* range,
                          size_t range_size);

idip_cfg_param_t*
idip_cfg_param_new_choice_int(const char* name, int64_t value,
                              const int64_t* range, size_t range_size);

/// Create instance of @ref idip_cfg_param_t of type
/// @ref IDIP_CFG_VALUE_TYPE_POLYLINE and copy to its value array of given
/// values and copy given ranges
/// @param name - name of parameter. ASCIIZ. Cannot be NULL.
/// @param value - pointer to array of points that are vertexes of polyline. Not NULL.
/// @param value_size - count of points in value array
/// @param range_x - acceptable range of X axis
/// @param range_y - acceptable range of Y axis
/// @param range_n - maximum points of polyline (it is not equals to value_size in general case)
idip_cfg_param_t*
idip_cfg_param_new_polyline(const char* name, const idip_pos_t* value,
                            size_t value_size, idip_axis_range_t range_x,
                            idip_axis_range_t range_y, size_t range_n);

idip_cfg_param_t*
idip_cfg_param_new_polygon(const char* name, const idip_pos_t* value,
                           size_t value_size, idip_axis_range_t range_x,
                           idip_axis_range_t range_y, size_t range_n);

idip_cfg_param_t *
idip_cfg_param_new_rect(const char *name,
                        const idip_pos_t *value,
                        idip_axis_range_t range_x, idip_axis_range_t range_y);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // IDIP_CFG_PARAM_P_H
