#ifndef IDIP_TRANSPORT_H
#define IDIP_TRANSPORT_H

#include <stdint.h>
#include <stddef.h>
#include <idip/idip_rc.h>

/// Flags using on IDIPv2 Transport Layer
enum idip_v2_transport_message_flags {
    /// An error occured in transmitter side.
    /// The packet is the last packet from the transmitter.
    /// The system.error.code field contains error code.
    IDIP_V2_FLAG_ERROR = 0x01,
    /// End-Of-Stream. The given packet is the last in a packet sequence
    /// for a given request_id.
    IDIP_V2_FLAG_EOS = 0x02,
    /// Transmitter requires to use an windowing algorithm.
    /// A window size is in system.window (in bytes).
    IDIP_V2_FLAG_WINDOW   = 0x04,
    /// Fragment of data.
    /// The packet contains a part/fragment of underline IDIPv2 message.
    IDIP_V2_FLAG_CHUNK = 0x08,
    /// Acknowledge number for transmitter.
    /// The packet contains system.ack field with a number of bytes
    /// that were handled by receiver.
    IDIP_V2_FLAG_ACK = 0x10,
    /// Reset flag.
    /// Receiver doesn't want handle all messages for the given request_id.
    /// Transmitter should immediately drop all packets with the request_id.
    IDIP_V2_FLAG_RST = 0x20,
    /// The last chunk/fragment of chunked data
    IDIP_V2_FLAG_LAST_CHUNK = 0x80,
};

typedef struct transport_message
{
    /// IDIP protocol version
    int64_t         protocol_version;
    /// Packet creation time stamp. Millisecnds UTC.
    uint64_t        time_stamp_ms_utc;
    /// A IDIPv2 Request ID number
    uint64_t        request_id;
    /// Packet sequence number for the given request_id.
    uint64_t        sequence_id;
    /// Flags
    uint64_t        flags;
    /// System/extended fields
    struct system_field {
        uint64_t    window;
        uint64_t    ack;
        int         error_code;
    } system;

} transport_message_t;


typedef struct idip_v2_transport_sysext_t
{
    int         flags;  ///< present flags
    uint64_t    window; ///< window size
    uint64_t    ack;    ///< acknowledge number
    int         ec;     ///< error code
    const char* emsg;   ///< error message
}
idip_v2_transport_sysext_t;

extern const idip_v2_transport_sysext_t idip_default_system_map;

#ifdef __cplusplus
extern "C" {
#endif

/// Creates a new one buffer for a idip v2 transport message.
/// The buffer will contains preallocated LWS_PRE bytes before the message.
/// LWS requires a buffer for modify outgoing packets by add LWS_PRE header
/// and XOR data with a random sequence.
struct msgpack_sbuffer* idip_v2_transport_container_get();

/// Releases a buffer that was maden with @ref idip_v2_transport_container_get().
void idip_v2_transport_container_release(struct msgpack_sbuffer* frame);

/// Returns a pointer to begin of the message. The method skips LWS_PRE bytes.
void* idip_v2_transport_container_get_data(struct msgpack_sbuffer* frame);

/// Return message size (in bytes) of underline frame. The size is corrected
/// for LWS_PRE offset.
size_t idip_v2_transport_container_get_data_size(const struct msgpack_sbuffer* frame);

/// Parse IDIP transport message
struct msgpack_object;
idip_rc_t idip_v2_transport_parse_msg(const struct msgpack_object* msg,
                                      transport_message_t* header,
                                      struct msgpack_object* payload,
                                      struct msgpack_object* system);

#ifdef __cplusplus
}
#endif
#endif // IDIP_TRANSPORT_H
