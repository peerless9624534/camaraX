/**
 * IDIP cloud configuration parameter public API.
 */
#ifndef IDIP_CFG_PARAM_H_INCLUDED
#define IDIP_CFG_PARAM_H_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <idip/idip_enum.h>
#include <idip/idip_data_types.h>

//-----------------------------------------------------------------------------
/// Opaque data structure that represents configuration parameter
struct idip_cfg_param;
typedef struct idip_cfg_param idip_cfg_param_t;

/// Configuration parameter value type code
typedef enum idip_cfg_param_value_type
{
    IDIP_CFG_BOOL,     ///< boolean parameter value type
    IDIP_CFG_INT,      ///< integer parameter value type (signed, 64 bit)
    IDIP_CFG_DOUBLE,   ///< double parameter value type
    IDIP_CFG_STRING,   ///< string parameter value type
    IDIP_CFG_POS,      ///< @ref idip_pos_t parameter value type
    IDIP_CFG_CHOICE_STR, ///< string parameter value which is limited to a set
                         /// of strings. In another words, the value string
                         /// can be one of value from a set of strings.
    IDIP_CFG_CHOICE_INT, ///< integer parameter. The value of the parameter can
                         /// be one of a given set of integers.
    IDIP_CFG_GRID,     ///< a string value that represents a mask of '0' and '1'
                       /// characters.
    IDIP_CFG_POLYLINE, ///< an array of @ref idip_pos_t that are points of lines
    IDIP_CFG_POLYGON,  ///< an array of @ref idip_pos_t that are the vertices
                       /// of a closed polygon.
    IDIP_CFG_RECT,     ///< an array of 2 @ref idip_pos_t that are top-left
                       /// and bottom-right vertices of a rectangle
}
idip_cfg_param_value_type_t;

#ifdef __cplusplus
extern "C" {
#endif

/// Return name of configuration parameter instance
/// @param param - pointer to parameter instance
const char* idip_cfg_param_get_name(const idip_cfg_param_t* param);

/// Return bool value of configuration parameter.
/// @param param - pointer to parameter instance
/// @param value - pointer to returned value. Can't be NULL.
/// Do interpret 0 as false, other values as true.
idip_status_code_t
idip_cfg_param_get_bool(const idip_cfg_param_t* param,
                        int* value);

/// Return integer value with its range from configuration parameter instance.
/// @param param - pointer to parameter instance
/// @param value - pointer to returned value. Can't be NULL.
idip_status_code_t
idip_cfg_param_get_int(const idip_cfg_param_t* param,
                       int64_t* value);

/// Return double value with its range from configuration parameter instance.
/// @param param - pointer to parameter instance
/// @param value - pointer to returned value. Can't be NULL.
idip_status_code_t
idip_cfg_param_get_double(const idip_cfg_param_t* param,
                          double* value);

/// Return string value of configuration parameter.
/// @param param - pointer to parameter instance
/// @param value - pointer to buffer. May be NULL. In this case real_size
/// contains real length of string.
/// @param buffer_size - size of value's buffer in bytes.
/// @param real_size - pointer to variable where function stores count of really
/// written bytes or full string's length (if value is NULL).
idip_status_code_t
idip_cfg_param_get_string(const idip_cfg_param_t* param,
                          char* value,
                          size_t buffer_size,
                          size_t* real_size);

/// Return idip_pos_t value from parameter
/// @param param - pointer to parameter instance
/// @param value - pointer to return value string pointer. Can't be NULL.
idip_status_code_t
idip_cfg_param_get_pos(const idip_cfg_param_t* param,
                       idip_pos_t* value);


/// Return grid value of configuration parameter as string.
/// @param param - pointer to parameter instance
/// @param value - pointer to buffer. May be NULL. In this case real_size
/// contains real length of string
/// @param buffer_size - size of value's buffer in bytes.
/// @param real_size - pointer to variable where function stores count of really
/// written bytes or full string's length (if value is NULL).
idip_status_code_t
idip_cfg_param_get_grid(const idip_cfg_param_t* param,
                        char* value,
                        size_t buffer_size,
                        size_t* real_size);

/// Return polyline value from configuration parameter. Polyline is an array
/// of points (vertexes). The function copies values to given array.
/// @param param - pointer to parameter instance
/// @param values - array to fill. Can be NULL (e.g. to check actual length)
/// @param values_count - pointer to actual length of array. In-Out parameter.
/// Before function call set the pointed value to maximal length of array (in items).
/// After call the *values_count contains actual count of points.
/// If set to NULL then the function only checks type of value.
idip_status_code_t
idip_cfg_param_get_polyline(const idip_cfg_param_t* param,
                            idip_pos_t* values,
                            size_t* values_count);

/// Return polygon value from configuration parameter. Polygon is an array
/// of points (vertexes). The function copies values to given array.
/// @param param - pointer to parameter instance
/// @param values - array to fill. Can be NULL (e.g. to check actual length)
/// @param values_count - pointer to actual length of array. In-Out parameter.
/// Before function call set the pointed value to maximal length of array (in items).
/// After call the *values_count contains actual count of points.
/// If set to NULL then the function only checks type of value.
idip_status_code_t
idip_cfg_param_get_polygon(const idip_cfg_param_t* param,
                           idip_pos_t* values,
                           size_t* values_count);

/// Return rectangle value from configuration parameter. Rectangle is a pair
/// of points that are opposite to each other (e.g. bottom-left and top-right or
/// top-left and bottom-right).
/// @param param - pointer to parameter instance
/// @param value_a - point to 1st point
/// @param value_b - point to 2nd point
idip_status_code_t
idip_cfg_param_get_rect(const idip_cfg_param_t* param,
                        idip_pos_t* value_a,
                        idip_pos_t* value_b);

#ifdef __cplusplus
}
#endif

#endif /* IDIP_CFG_PARAM_H_INCLUDED */

