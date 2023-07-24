#ifndef IDIP_URI_PARSER_H
#define IDIP_URI_PARSER_H

struct idip_uri;
typedef struct idip_uri idip_uri_t;

#ifdef __cplusplus
extern "C" {
#endif

typedef enum idip_uri_error
{
    IDIP_URI_OK = 0,
    IDIP_URI_NO_MEM = 1,
}
idip_uri_error_t;

typedef enum idip_uri_known_type
{
    IDIP_URI_UNKNOWN = 0,
    IDIP_URI_FILE = 1,
    IDIP_URI_UNIX,
    IDIP_URI_TCP,
    IDIP_URI_UDP,
    IDIP_URI_HTTP,
    IDIP_URI_HTTPS,
    IDIP_URI_FTP,
    IDIP_URI_IDIP,
    IDIP_URI_IDIP_LOCAL,
    IDIP_URI_WEB_SOCKET,
    IDIP_URI_WEB_SOCKET_SECURE,
    IDIP_URI_SYSLOG,
}
idip_uri_known_type_t;

typedef struct idip_uri_parse_options
{
    idip_uri_known_type_t fallback_type;
    int fallback_port;
}
idip_uri_parse_options_t;

idip_uri_t* idip_uri_new_parse(const char* uristr, int* ec,
                               const idip_uri_parse_options_t* opts);

idip_uri_t* idip_uri_new_copy(const idip_uri_t* other, int* ec);

void idip_uri_delete(idip_uri_t* self);

const char* idip_uri_get_sheme(const idip_uri_t* self, const char *fallback);
int         idip_uri_is_sheme_empty(const idip_uri_t* self);

idip_uri_known_type_t idip_uri_get_sheme_type(const idip_uri_t* self);

const char* idip_uri_get_host(const idip_uri_t* self, const char *fallback);
int         idip_uri_is_host_empty(const idip_uri_t* self);

const char* idip_uri_get_host_and_port(const idip_uri_t *self, char *result, size_t maxlen);

const char* idip_uri_get_port(const idip_uri_t* self, const char *fallback);
int         idip_uri_is_port_empty(const idip_uri_t* self);
int         idip_uri_get_port_num(const idip_uri_t* self);

const char* idip_uri_get_user(const idip_uri_t* self, const char *fallback);
int         idip_uri_is_user_empty(const idip_uri_t* self);

const char* idip_uri_get_password(const idip_uri_t* self, const char *fallback);
int         idip_uri_is_password_empty(const idip_uri_t* self);

const char* idip_uri_get_path(const idip_uri_t* self, const char *fallback);
int         idip_uri_is_path_empty(const idip_uri_t* self);
/// returns true if the parsed uri begins from a "///" or "file:///" sequence
int         idip_uri_is_absolute_path(const idip_uri_t* self);
/// returns true if the parsed uri begins from a "./" or "file://./" sequence
int         idip_uri_is_relative_path(const idip_uri_t* self);
/// returns a disjunction of @ref idip_uri_is_absolute_path and @ref idip_uri_is_relative_path
int         idip_uri_is_path(const idip_uri_t* self);

const char* idip_uri_get_query(const idip_uri_t* self, const char *fallback);
int         idip_uri_is_query_empty(const idip_uri_t* self);

const char* idip_uri_get_hash(const idip_uri_t* self, const char *fallback);
int         idip_uri_is_hash_empty(const idip_uri_t* self);

#ifdef __cplusplus
}
#endif

#endif // IDIP_URI_PARSER_H
