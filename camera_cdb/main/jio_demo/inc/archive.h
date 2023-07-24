#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <stdint.h>
#include <stddef.h>

struct archive;
typedef struct archive archive_t;

#ifdef __cplusplus
extern "C" {
#endif

archive_t* archive_new(const char* list_name, int64_t start, int64_t end, int continous);

int archive_get_count(const archive_t* self);

int archive_get_name(const archive_t* self, size_t n, char* name, size_t name_len);

int archive_get_times(const archive_t* self, size_t n, int64_t* start, int64_t* length);

void archive_delete(archive_t* self);

#ifdef __cplusplus
}
#endif

#endif // ARCHIVE_H
