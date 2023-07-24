#ifndef IDIP_RX_CACHE_H_INCLUDED
#define IDIP_RX_CACHE_H_INCLUDED

#include <msgpack.h>
#include <idip/idip_rc.h>

/**
 * Looks like a map<request_id, msgpack_unpacker>.
 * This class allows to glue chunked IDIP frames for any request_id. It implies
 * an optimiszation: msgpack_unpacker objects are reusing many times because
 * they store in an internal list of used objects.
 */
struct idip_rx_cache;
typedef struct idip_rx_cache idip_rx_cache_t;

struct rx_cache_item;
typedef struct rx_cache_item rx_cache_item_t;

struct idip_server;

/// Create a new pool+map of msgpack_unpacker
idip_rx_cache_t* idip_rx_cache_new();

/// Delete a pool and free resources
void idip_rx_cache_delete(idip_rx_cache_t* self);

/// Returns a new one @ref rx_cache_item_t item if it wasn't found
/// or a pointer to an existing @ref rx_cache_item_t.
/// Returns NULL when errors where occurred.
rx_cache_item_t* idip_rx_cache_get(idip_rx_cache_t* self,
                                   uint64_t request_id,
                                   size_t rx_window);

/// Mark item @ref rx_cache_item_t with a given @a request_id
/// as unused and push it into an internal pool.
void idip_rx_cache_release(idip_rx_cache_t* self, uint64_t request_id);


bool idip_rx_cache_item_rx_limit_reached(rx_cache_item_t *self, size_t size);

int idip_rx_cache_item_append_data(rx_cache_item_t* self,
                                   const void* buf,
                                   size_t size);

struct msgpack_unpacker*
idip_rx_cache_item_get_unpacker(rx_cache_item_t* self);


void idip_rx_cache_test();

typedef struct idip_rx_data_handle_result_t {
   idip_rc_t                transport_code;
   msgpack_unpack_return    msgpack_code;
} idip_rx_data_handle_result_t;


idip_rx_data_handle_result_t
idip_rx_cache_item_handle_data(rx_cache_item_t *self, bool sequence_complete,
                               const struct idip_server *server);

msgpack_object
idip_rx_cache_item_get_parsed_hlp(rx_cache_item_t *self, size_t *size);

bool
idip_rx_cache_item_get_raw_hlp(rx_cache_item_t *self,
                               void** pbuf, size_t *psize);


#endif // IDIP_RX_CACHE_H_INCLUDED
