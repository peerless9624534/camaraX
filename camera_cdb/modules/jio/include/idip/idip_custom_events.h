#ifndef IDIP_CUSTOM_EVENTS_H
#define IDIP_CUSTOM_EVENTS_H

#include <idip/idip_server.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Notify IDIP server instance about simple motion event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_motion_event(idip_server_t* server, const char* target,
                              int64_t now);

/// Notify IDIP server instance about motion event in a zone.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @param zone_number - zone number where detection was occurred.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_motion_zone_event(idip_server_t* server, const char* target,
                                   int64_t now, unsigned int zone_number);

/// Notify IDIP server instance about sound detection event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_sound_event(idip_server_t* server, const char* target,
                             int64_t now);

/// Notify IDIP server instance about PIR detection event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_pir_event(idip_server_t* server, const char* target,
                          int64_t now);

/// Notify IDIP server instance about alarm event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_alarm_event(idip_server_t* server, const char* target,
                             int64_t now);

/// Notify IDIP server instance about "call" event (e.g. a "call" button on a
/// doorbell was pushed).
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_call_event(idip_server_t* server, const char* target,
                            int64_t now);

/// Send line detector event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @param line_number - line detector number.
/// @param direction - crossing direction.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_vca_line_detection_crossed_event(idip_server_t* server,
                                                 const char* target,
                                                 int64_t now,
                                                 unsigned int line_number,
                                                 const char* direction);

/// Send field detector event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @param field_number - detector field number.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_vca_field_detection_inside_event(idip_server_t* server,
                                                 const char* target,
                                                 int64_t now,
                                                 unsigned int field_number);

/// Send loitering detector event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @param field_number - loitering detector field number.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_vca_loitering_detection_event(idip_server_t* server,
                                             const char* target,
                                             int64_t now,
                                             unsigned int field_number);

/// Send crowd detector simple event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @param field_number - crowd detector field number.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_vca_crowd_detection_event(idip_server_t* server,
                                         const char* target,
                                         int64_t now,
                                         unsigned int field_number);

/// Send crowd detector event with count of detected objects.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @param field_number - crowd detector field number.
/// @param num_of_objects - detected objects count.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_vca_crowd_detection_cnt_event(idip_server_t* server,
                                             const char* target,
                                             int64_t now,
                                             unsigned int field_number,
                                             unsigned int num_of_objects);


/// Send tampering detector "global scene change" event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_vca_tampering_detection_global_scene_change_event(idip_server_t* server,
                                                                   const char* target,
                                                                   int64_t now);

/// Send tampering detector "image too dark" event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_vca_tampering_detection_image_too_dark_event(idip_server_t* server,
                                                            const char* target,
                                                            int64_t now);

/// Send tampering detector "image to bright" event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_vca_tampering_detection_image_too_bright_event(idip_server_t* server,
                                                             const char* target,
                                                             int64_t now);

/// Send tampering detector "signal loss" event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_vca_tampering_detection_signal_loss_event(idip_server_t* server,
                                                         const char* target,
                                                         int64_t now);

/// Send face detector event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @param params - face recognition attributes.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_vca_face_detection_face_event(idip_server_t* server,
                                              const char* target,
                                              int64_t now,
                                              const idip_face_detector_event_params_t* params);

/// Send removal object detector event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @param field - field number.
/// @param id - object string identifier or NULL.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_vca_removal_object_detection_object_removed_event(idip_server_t* server,
                                                                   const char* target,
                                                                   int64_t now,
                                                                   int field,
                                                                   const char* id);

/// Send unclaimed object detection event.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @param field - field number.
/// @param id - object string identifier or NULL.
/// @return @a IDIP_OK of success or @a IDIP_GENERIC_ERROR if fail occurs.
idip_status_code_t
idip_server_send_vca_unclaimed_object_detection_new_object_event(idip_server_t* server,
                                                                 const char* target,
                                                                 int64_t now,
                                                                 int field,
                                                                 const char* id);

/// A helper struct that represents a bounding rectangle on an image
typedef struct idip_ivs_vca_rect {
    int x, y;       ///< coordinates of the BOTTOM RIGHT point of a bounding rectangle
    int height;     ///< bounding rectangle height
    int width;      ///< bounding rectangle width
}
idip_ivs_vca_rect_t;

/// A helper struct that represents a picture shot for events
typedef struct idip_ivs_vca_shot {
    /// Path to the image on the local (camera, dvr) file system
    /// or an public url. The libidip checks that the path is a path to local
    /// file by calling access(2) and sends the content of the given file in event.
    /// In other case libidip assumes the string is an url and sends it to the
    /// cloud directly.
    char* image_url_or_path;
    /// A temperature of a region of image.
    float temp_in_celsius;
    /// An array of temperature tresholds. Optional. Can be NULL.
    float* temp_treshold;
    /// A size of the @a temp_treshold array. Set it to 0 when temp_threshold = NULL
    size_t temp_threshold_size;
    /// An array of notification regions (rects). Can be NULL.
    idip_ivs_vca_rect_t* rects;
    /// Count of @a rect items.
    size_t rects_size;
}
idip_ivs_vca_shot_t;

/// Send a thermal event when a detection temperature is greater than a threshold.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @param temp - detected temperature anomaly value in Celsius.
/// @param temp_threshold - a temperature threshold value in Celsius.
/// @param shot - external data for the event. See @ref idip_ivs_vca_shot_t description.
idip_status_code_t
idip_server_send_vca_body_temp_high(idip_server_t* server,
                                    const char* target,
                                    int64_t now,
                                    float temp,
                                    float temp_treshold,
                                    const idip_ivs_vca_shot_t* shot);

/// Send a thermal event when a detection temperature is less than a threshold.
/// @param server - pointer to an idip server instance.
/// @param target - event target name.
/// @param now - event time, milliseconds since epoch, UTC.
/// @param temp - detected temperature anomaly value in Celsius.
/// @param temp_threshold - a temperature threshold value in Celsius.
/// @param shot - external data for the event. See @ref idip_ivs_vca_shot_t description.
idip_status_code_t
idip_server_send_vca_body_temp_low(idip_server_t* server,
                                   const char* target,
                                   int64_t now,
                                   float temp,
                                   float temp_treshold,
                                   const idip_ivs_vca_shot_t *shot);

#ifdef __cplusplus
}
#endif

#endif // IDIP_CUSTOM_EVENTS_H
