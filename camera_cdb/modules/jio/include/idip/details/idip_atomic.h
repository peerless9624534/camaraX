#ifndef IDIP_ATOMIC_H
#define IDIP_ATOMIC_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Atomically increments an integer variable
 * @param atomic - pointer to incremented variable
 */
void idip_atomic_int_inc(volatile int *atomic);

/**
 * Atomically decrements an integer variable and returns true when the value
 * has become equals to 0.
 * @param atomic pointer to the integer variable
 */
bool idip_atomic_int_dec_and_test(volatile int *atomic);

#ifdef __cplusplus
}
#endif

#endif // IDIP_ATOMIC_H
