#ifndef IDIP_RC_H
#define IDIP_RC_H

typedef enum idip_rc_t {
    //	Success
    IDIP_RC_OK = 0,
    //	Invalid query format. Required fields are absent, or unknown data
    // structure or invalid message syntax. After this reply client must
    // close the connection.
    IDIP_RC_INVALID_FORMAT = 1,
    // LibIDIP internal error.
    IDIP_RC_INTERNAL_ERROR = 2,
    // Unknown method. LibIDIP don't know about this method name and
    // therefore it can't be executed.
    IDIP_RC_UNKNOWN_METHOD = 3,
    // Method is documented in this protocol but it is not implemented
    // in LibIDIP. Client don't closes connection.
    IDIP_RC_METHOD_NOT_IMPLEMENTED = 4,
    // Message are valid but some arguments of method have invalid values
    // or no required arguments in query.
    IDIP_RC_INVALID_ARGUMENTS = 5,
    // Method require authorized access or authorization failed.
    // See method system.init.
    IDIP_RC_NOT_AUTHORIZED = 6,
    // Method doesn't allow windowing
    IDIP_RC_WINDOW_NOT_ALLOWED = 7,
    // Method doesn't allow multiple input HLPs
    IDIP_RC_METHOD_NO_INPUT = 8,
    // Input window overflow
    IDIP_RC_WINDOW_OVERFLOW = 9,

    // Reserved error code range for libidip.
    IDIP_RC_RESERVED_BEGIN = 10,
    IDIP_RC_RESERVED_END   = 99,

    // Method specific codes
    IDIP_RC_METHOD_RES     = 100,
} idip_rc_t;

/// A marker for cases where the Request ID wasn't determined
#define IDIP_UNKNOWN_REQUEST_ID UINT64_MAX

#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */

const char* idip_rc_code_str(idip_rc_t ec, const char* def);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif // IDIP_RC_H
