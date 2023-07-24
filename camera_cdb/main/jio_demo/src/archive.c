#include "archive.h"
#include "avpkt_source.h"
#include <inttypes.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include <idip/idip_utils.h>
#include <idip/idip_vector.h>

typedef struct archive_entry_t {
    int64_t start;
    int64_t duration;
    int path_len;
    char path[255];
} archive_entry_t;

//------------------------------------------------------------------------------
struct archive {
    char* list_file_name;
    int64_t start;
    int64_t end;
    int continous;
    idip_ptr_vector_t* list;
};

//------------------------------------------------------------------------------
/**
 * Compares two instances @ref archive_entry_t by start time. This is a helper
 * function. See @ref archive_load_list() implementation.
 * @param pa - pointer to pointer to an left side entry
 * @param pb - pointer to pointer to an right side entry
 * @retval 0 when start time points are equals
 * @retval -1 when left side start time is strongly less than right start time
 * @retval 1 when left side start time is strongly great than right start time
 */
static int archive_entry_start_compare(const void* pa, const void* pb)
{
    const archive_entry_t* a = *(const archive_entry_t* const *)pa;
    const archive_entry_t* b = *(const archive_entry_t* const *)pb;

    if( a->start == b->start )
        return 0; // equals
    return (a->start < b->start ? -1 : 1); // less or great
}
//------------------------------------------------------------------------------
static void archive_load_list(archive_t* self)
{
    FILE* f = fopen(self->list_file_name, "rt");
    if( !f ) {
        fprintf(stderr, "Can't open file \"%s\": %s\n",
                (self->list_file_name ? self->list_file_name : "(null)"),
                strerror(errno));
        return;
    }

    fprintf(stdout,
            "Reading %s. start=%" PRId64 ", end=%" PRId64 "...\n",
            self->list_file_name, self->start, self->end);

    char path[PATH_MAX];
    for (;;) {

        int64_t start, duration, end = 0l;
        memset(path, 0, sizeof(path));
        int rc = fscanf(f, "%" PRId64 " %" PRId64 " %s\n", &start, &duration, path); // NOLINT
        if( rc < 0 ) break;
        if( rc != 3 ) {
            if( ferror(f) )
                break;
            else
                continue;
        }
        end = start + duration;

        if( (self->end > 0) && (start > self->end) ) {
            fprintf(stdout,
                    "Skip %" PRId64 "..%" PRId64 ": %" PRId64 ". It is great than upper bound. %s\n",
                    start, end, duration, path);
            continue;
        }
        if( (self->start > 0) && (end < self->start) ) {
            fprintf(stdout,
                    "Skip %" PRId64 "..%" PRId64 ": %" PRId64 ". It is less than lower bound. %s.\n",
                    start, end, duration, path);
            continue;
        }

        size_t len = strlen(path) + 1;
        size_t pad = (len > 255 ? len - 255 : 0);
        archive_entry_t* new_ent = IDIP_NEW_PAD(*new_ent, pad);
        if( !new_ent ) {
            fprintf(stdout,
                    "Skip %" PRId64 " %" PRId64 " %s: %s\n",
                    start, duration, path, strerror(errno));
            continue;
        }
        new_ent->start = start;
        new_ent->duration = duration;
        new_ent->path_len = (int)len - 1;
        strncpy(new_ent->path, path, len);
        if( !idip_ptr_vector_push_back(self->list, new_ent) ) {
            free(new_ent);
        }

        fprintf(stdout, "Added %" PRId64 " %" PRId64 " %s\n",
                start, duration, path);
    }
    fclose(f);

    if( self->list->length ) {
        qsort(self->list->data, self->list->length, sizeof(void*),
              archive_entry_start_compare);
    }
}
//------------------------------------------------------------------------------
void archive_print_list(archive_t* self)
{
    size_t i = 0;
    if( self->list->length == 0 ) {
        fprintf(stdout, "The archive list is empty!\n");
    }
    for(i = 0; i < self->list->length; ++i) {
        archive_entry_t* ent = idip_ptr_vector_index(self->list, i);
        fprintf(stdout, "%zd. %s %" PRId64 " %" PRId64 "\n", i,
                       ent->path, ent->start, ent->duration);
    }
}
//------------------------------------------------------------------------------
archive_t *archive_new(const char *list_file_name, int64_t start, int64_t end, int continous)
{
    archive_t* self = IDIP_NEW(*self);
    if( !self ) {
        fprintf(stderr, "Can't allocate memory for instance of archive\n");
        return NULL;
    }

    self->start = start;
    self->end = end;
    self->continous = continous;
    self->list_file_name = (list_file_name ? strdup(list_file_name) : NULL);
    if( !self->list_file_name ) {
        fprintf(stderr, "Can't copy archive list name: %s\n",
                 (list_file_name ? strerror(errno) : "path is empty"));
        archive_delete(self);
        return NULL;
    }
    self->list = idip_ptr_vector_new_with_free_func(free);
    if( !self->list ) {
        fprintf(stderr, "Can't create archive list: %s\n", strerror(errno));
        archive_delete(self);
        return NULL;
    }

    archive_load_list(self);
    archive_print_list(self);

    return self;
}
//------------------------------------------------------------------------------
void archive_delete(archive_t *self)
{
    if( !self ) return;
    IDIP_FREE(self->list_file_name);
    IDIP_DELETE(self->list, idip_ptr_vector_delete);
    free(self);
}
//------------------------------------------------------------------------------
int archive_get_count(const archive_t *self)
{
    return (self ? (int)self->list->length : 0);
}
//------------------------------------------------------------------------------
int archive_get_name(const archive_t *self, size_t n, char *name, size_t name_len)
{
    if( !self || !name || !name_len )
        return 0;
    if( n >= (size_t)self->list->length )
        return 0;
    archive_entry_t* entry = idip_ptr_vector_index(self->list, n);
    return snprintf(name, name_len, "%s", entry->path);
}
//------------------------------------------------------------------------------
int archive_get_times(const archive_t *self, size_t n, int64_t *start, int64_t *duration)
{
    if( !self || !start || !duration )
        return 0;
    if( n >= (size_t)self->list->length )
        return 0;
    archive_entry_t* entry = idip_ptr_vector_index(self->list, n);
    *start = entry->start;
    *duration = entry->duration;
    return 1;
}
