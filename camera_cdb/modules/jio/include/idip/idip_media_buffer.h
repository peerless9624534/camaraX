#ifndef IDIP_MEDIA_BUFFER_H
#define IDIP_MEDIA_BUFFER_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <idip/idip_av_packet.h>
#include <idip/idip_av_limits.h>
#include <idip/details/idip_byte_storage.h>

/// Maximal amounts for a media buffer
typedef struct idip_media_buffer_initializer
{
    /// Maximal buffer size.
    size_t data_size_hard_limit;

    /// Initial data size in bytes. Set the same value
    /// as @c data_size_limit_bytes to avoid data buffer reallocations.
    /// Set to 0 for automatically calculation the initial capacity.
    size_t data_size_init;

    /// The maximum time that frame could be in the media buffer.
    int64_t time_limit_msec;
}
idip_media_buffer_initializer_t;

//------------------------------------------------------------------------------
/// A helper structure to find frames in the buffer
typedef struct av_find_options
{
    /// A frame type that looking for.
    /// Set to IDIP_AV_PACKET_NONE to look for both audio and video.
    idip_av_packet_type_t type;
    /// The minimal timestamp in milliseconds of @ref idip_monotonic_ms_now().
    /// A frame which is looking for will have a greater or equal time stamp.
    /// @note depends on @a want_time value.
    uint64_t timestamp;
    /// Look for key frame only
    bool want_key;
    /// Use @a timestamp value for look up.
    bool want_time;
}
av_find_options_t;

/// An opaque data structure to represent a circular buffer that allows to store
/// audio and video frames. It implements a sheme: one writer - many readers.
///
typedef struct idip_media_buffer idip_media_buffer_t;

/// An opaque data structure that allows to forward iterating throught
/// @ref idip_media_buffer items.
typedef struct idip_media_buffer_iterator idip_media_buffer_iterator_t;

#ifdef __cplusplus
extern "C" {
#endif

/// Creates a new one instance of @ref idip_media_buffer_t
/// @param limits - pointer to buffer limits.
/// @return a pointer to a new initialized instance of @ref idip_media_buffer_t or NULL
idip_media_buffer_t* idip_media_buffer_new(const idip_media_buffer_initializer_t* init);

/// Swaps two instances of @ref idip_media_buffer_t. Addresses of both objects
/// will not change, but internal data of buffers will excanged.
/// @notes Does nothing when @a self == @a other.
/// @param self - pointer to the first object to swap to. Shoudn't be NULL.
/// @param self - pointer to the second object to swap to. Shoudn't be NULL.
/// @retval 0 if uccess
/// @retval EINVAL if @a self or @a other is NULL.
int idip_media_buffer_swap(idip_media_buffer_t *self, idip_media_buffer_t *other);

/// Increment reference counter to the object and returns the given instance back.
/// @param self - a pointer to a buffer instance. Should not be NULL.
/// @return the @a self argument.
///
/// @note DO NOT use @ref idip_media_buffer_delete() when you're using the method.
/// Please use @ref idip_media_buffer_unref() instead.
idip_media_buffer_t*  idip_media_buffer_addref(idip_media_buffer_t* self);

/// Decrements a reference counter to the given object. Deletes the object when
/// the counter goest to 0.
/// @param self - a pointer to a buffer instance. Should not be NULL.
void  idip_media_buffer_unref(idip_media_buffer_t* self);

/// Returns current time limit (in milliseconds) for the given buffer object.
/// The time limit is a limit for packets to being in the buffer. Packets that
/// where appended to the buffer earlier than the limit will be removed.
/// @param self - a pointer to the buffer object.
/// @return current packet buffer time limit in milliseconds.
unsigned
idip_media_buffer_get_time_limit(const idip_media_buffer_t* self);

/// Replaces a current time limit to new one. The limit will applied on the next
/// write to the buffer.
/// @param self - a pointer to the buffer object. Can't be NULL.
/// @param new_time_limit - a new time limit value in milliseconds.
/// @retval 0 if success
/// @retval EINVAL on errors
int
idip_media_buffer_update_time_limit(idip_media_buffer_t* self,
                                    unsigned new_time_limit);

/// Returns current buffer limits.
/// @param self - a pointer to the buffer object. Can't be NULL.
idip_av_limits_t
idip_media_buffer_get_limits(const idip_media_buffer_t* self);

/// Replaces a current time and size limits to a new one. These limits will
/// applied on the next write to the buffer.
/// @param self - a pointer to the buffer object. Can't be NULL.
/// @param limits - a pointer to a new time and size limit values.
/// @retval 0 if success
/// @retval EINVAL on errors
int
idip_media_buffer_update_limits(idip_media_buffer_t* self,
                                const idip_av_limits_t* limits);

/// Returns a count of packets in the buffer.
/// @param self - a pointer to the buffer object. Can't be NULL.
size_t
idip_media_buffer_packets_count(const idip_media_buffer_t* self);

/// Return the actual data size (summ of all packets data) int the buffer
/// @param self - a pointer to the buffer object. Can't be NULL.
size_t
idip_media_buffer_data_size(const idip_media_buffer_t* self);

/// Push a packet to the given buffer.
/// @param self - a pointer to the buffer object. Can't be NULL.
/// @param packet - a pointer to a packetto write to.
/// @retval 0 if success
/// @retval EINVAL when @a self or @a packet is NULL or when the size of
/// the packet is greather than buffer capacity/size limit.
/// @retval ENOMEM when the buffer can't allocate additional space to store the packet
int
idip_media_buffer_write(idip_media_buffer_t* self, idip_av_packet_t *packet);

/// Returns a new instance of @ref idip_media_buffer_iterator_t that points
/// to a packet that matches to @ref av_find_options_t. When no packet is matched
/// then returned iterator points to the end of the buffer.
/// @param self - a pointer to the buffer.
/// @param opts - a pointer to search options (@ref av_find_options_t).
/// @return A new instance of buffer iterator or NULL if errors occured.
///
/// @note The iterator increments a reference counter to the buffer. Then
/// you should use @ref idip_media_buffer_addref() and @ref idip_media_buffer_unref()
/// when you're using iterators,
idip_media_buffer_iterator_t*
idip_media_buffer_find_iterator(idip_media_buffer_t *self, const av_find_options_t* opts);

/// Returns a new instance of @ref idip_media_buffer_iterator_t that points
/// to a VIDEO KEY FRAME packet that timestamp is greter or equal than th @a lower_bound.
/// When no packet is matched then returned iterator points to the end of the buffer.
/// @param self - a pointer to the buffer.
/// @param lower_bound - a time stamp value in milliseconds
///
/// @note The iterator increments a reference counter to the buffer.
idip_media_buffer_iterator_t*
idip_media_buffer_find_vdeo_key_frame_after_time(idip_media_buffer_t *self,
                                                 uint64_t lower_bound);

/// Returns a new instance of @ref idip_media_buffer_iterator_t that points
/// to a VIDEO FRAME (with or without a key attribute) packet that timestamp
/// is greter or equal than th @a lower_bound. When no packet is matched then
/// returned iterator points to the end of the buffer.
/// @param self - a pointer to the buffer.
/// @param lower_bound - a time stamp value in milliseconds
///
/// @note The iterator increments a reference counter to the buffer.
idip_media_buffer_iterator_t*
idip_media_buffer_find_vdeo_frame_after_time(idip_media_buffer_t *self,
                                             uint64_t lower_bound);

/// Returns a new instance of @ref idip_media_buffer_iterator_t that points
/// to a VIDEO OR AUDIO FRAME packet that timestamp
/// is greter or equal than th @a lower_bound. When no packet is matched then
/// returned iterator points to the end of the buffer.
/// @param self - a pointer to the buffer.
/// @param lower_bound - a time stamp value in milliseconds
///
/// @note The iterator increments a reference counter to the buffer.
idip_media_buffer_iterator_t*
idip_media_buffer_find_frame_after_time(idip_media_buffer_t *self,
                                             uint64_t lower_bound);

/// Returns a new instance of @ref idip_media_buffer_iterator_t that points
/// to a first item on the buffer.
/// @param self - a pointer to the buffer.
///
/// @note The iterator increments a reference counter to the buffer.
idip_media_buffer_iterator_t*
idip_media_buffer_begin(idip_media_buffer_t* self);

/// Returns a new instance of @ref idip_media_buffer_iterator_t that points
/// to the end of the buffer.
/// @param self - a pointer to the buffer.
///
/// @note The iterator increments a reference counter to the buffer.
idip_media_buffer_iterator_t*
idip_media_buffer_end(idip_media_buffer_t* self);

/// Deletes an iterator instance (and decrements a reference counter to the
/// underline buffer).
/// @param self - a pointer to an iterator instance.
void idip_media_buffer_iterator_delete(idip_media_buffer_iterator_t* self);

/// Advances the given iterator to the next item position in the buffer if possible.
/// @param self - a pointer to an iterator instance. Can't be NULL.
/// @retval 0 if success
/// @retval EINVAL for invalid arguments
/// @retval EAGAIN when the last position in the buffer is reached.
int idip_media_buffer_iterator_advance(idip_media_buffer_iterator_t* self);

/// Checks that two iterators points to the same position in the same container.
/// @param self - a pointer to the first iterator instance. Can't be NULL.
/// @param other - a pointer to the second iterator instance. Can't be NULL.
bool
idip_media_buffer_iterator_equals(const idip_media_buffer_iterator_t* self,
                                  const idip_media_buffer_iterator_t* other);

/// Tests that the given iterator points to the end position of the underline
/// buffer.
/// @param self - a pointer to a iterator instance. Can't be NULL.
bool
idip_media_buffer_iterator_is_at_end(const idip_media_buffer_iterator_t* self);

/// Dereferences an iterator. Reads data and metadata from a buffer at
/// current iterator position.
/// @param [in] self - pointer to an iterator instance.
/// @param [out] packet_payload_buffer - pointer to a frame buffer. May be NULL.
/// @param [out] node_uid - pointer to a node id to get. May be NULL.
/// @retval 0 if success
/// @retval EINVAL on generic errors (NULL pointers, etc.)
/// @retval EFAULT when the iterator was invalidated. No data actually readed.
/// @retval EAGAIN when the iterator points to an empty buffer (to the end of buffer).
/// No data actually readed in the case.
int
idip_media_buffer_iterator_dereference(const idip_media_buffer_iterator_t* self,
                                       idip_av_packet_t* packet_buffer,
                                       idip_byte_storage_t* packet_payload_buffer,
                                       uint64_t *node_uid);

#ifdef __cplusplus
}
#endif

#endif // IDIP_MEDIA_BUFFER_H
