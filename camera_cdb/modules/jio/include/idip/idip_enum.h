/**
 * IDIP common enumerations
 */
#ifndef IDIP_ENUM_H_INCLUDED
#define IDIP_ENUM_H_INCLUDED

/// Return codes for all API methods
typedef enum idip_status_code {
    /// Success
    IDIP_OK = 0,

    /// Generic implementation error.
    IDIP_GENERIC_ERROR = -1,

    /// Invalid argument's value.
    IDIP_INVALID_ARGUMENTS = -2,

    /// An timeout was occurred during send/put.
    IDIP_SEND_TIMEOUT = -3,
}
idip_status_code_t;

#endif /* IDIP_ENUM_H_INCLUDED */

