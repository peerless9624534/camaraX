#ifndef AVG_H_INCLUDED
#define AVG_H_INCLUDED

#include <stddef.h>
#include <stdint.h>

typedef struct avg  avg_t;
typedef struct avgd avgd_t;

#ifdef __cplusplus
extern "C" {
#endif

avg_t* avg_new(size_t size);

void avg_delete(avg_t* self);

void avg_append(avg_t* self, int64_t v);

void avg_set(avg_t* self, int64_t v);

int64_t avg_get(const avg_t* self);


avgd_t* avgd_new(size_t size, double initial_value);

void avgd_delete(avgd_t* self);

void avgd_append(avgd_t* self, double v);

double avgd_get(const avgd_t* self);

#ifdef __cplusplus
}
#endif

#endif // AVG_H_INCLUDED
