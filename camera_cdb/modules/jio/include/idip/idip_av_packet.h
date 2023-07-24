#ifndef IDIP_AV_PACKET_H
#define IDIP_AV_PACKET_H

#include <idip/idip_data_types.h>

#include <stdbool.h>
#include <stddef.h>

typedef enum idip_av_packet_type
{
    IDIP_AV_PACKET_NONE = 0,
    IDIP_AV_PACKET_VIDEO,
    IDIP_AV_PACKET_AUDIO
} idip_av_packet_type_t;

typedef struct idip_av_packet
{
    uint64_t fqid;              ///< Frame sequence ID in a Queue
    idip_av_packet_type_t type; ///< Packet type in the @a data buffer.
    bool is_key;                ///< @a data holda a key video frame.
    uint64_t ts_ms;             ///< Packet timestamp in milliseconds from the Epoch.
    uint64_t write_ts_ms;       ///< Packet put to a buffer timestamp in milliseconds from the Epoch.
    size_t size;                ///< Number of bytes at the @a data address.
    const idip_byte_t* data;    ///< Pointer to read-only data.
}
idip_av_packet_t;




void idip_av_packet_init_defaults(idip_av_packet_t* self);

#endif // IDIP_AV_PACKET_H
