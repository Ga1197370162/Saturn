#ifndef __SAT_TYPE_H__
#define __SAT_TYPE_H__

#ifdef __cplusplus
#define BEGIN_C_DECLS extern "C" {
#define END_C_DECLS }
#else
#define BEGIN_C_DECLS
#define END_C_DECLS
#endif

#include <math.h>
#include <stdio.h>
#include <wchar.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>

/* struct member offset */
#ifndef INSTANCE_OF
#ifdef __compiler_offsetof
#define INSTANCE_OF(TYPE, MEMBER) __compiler_offsetof(TYPE, MEMBER)
#else
#define INSTANCE_OF(TYPE, MEMBER) ((size_t) & ((TYPE*)0)->MEMBER)
#endif
#endif

/* un_use */
#define UNUSE(v) ((void)v)

/* ret_t */
typedef int32_t ret_t;

/* bool */
#define TRUE 1
#define FALSE 0
typedef uint8_t bool_t;

/* math */
#define ABS(v) abs(v)
#define SIN(v) sin(v)
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

/* types */
#define IS_SPACE(c) (c == ' ' || c == '\n' || c == '\t' || c == '\r')
#define IS_ALPHA(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))

/* bits */
#define SET_NULL(p) (p) = NULL
#define ROUND_TO8(size) (((size + 7) >> 3) << 3)

#define SET_BIT(v, n) ((v) |= 1UL << (n))
#define CLEAR_BIT(v, n) ((v) &= ~(1UL << (n)))
#define TOGGLE_BIT(v, n) ((v) ^= (1UL << (n)))
#define TEST_BIT(v, n) (((v) >> (n)) & 1U)

#include "sat_mem.h"
#include "sat_error.h"
#include "sat_assert.h"
#include "sat_utils.h"

/* interface */
typedef int32_t (*sat_compare_t)(const void* ctx, const void* data);
typedef ret_t (*sat_foreach_t)(void* ctx, const void* data);
typedef ret_t (*sat_destory_t)(void* ctx);

#endif /*__SAT_TYPE_H__*/