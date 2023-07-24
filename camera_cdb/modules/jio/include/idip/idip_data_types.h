/**
 * idip_data_types.h
 * Common public data types using with libidip.
 */
#ifndef IDIP_DATA_TYPES_H
#define IDIP_DATA_TYPES_H

#include <stdint.h>

/// Position type (prefer for configuration). Pair of coordinates.
typedef struct idip_pos
{
    int64_t x;
    int64_t y;
}
idip_pos_t;

/// Range type
typedef struct idip_axis_range
{
    int64_t min;
    int64_t max;
}
idip_axis_range_t;

/// Raw byte single byte
typedef uint8_t idip_byte_t;

#endif // IDIP_DATA_TYPES_H
