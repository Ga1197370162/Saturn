#ifndef __SAT_TYPES_H__
#define __SAT_TYPES_H__

#include <stdint.h>

#ifdef __cplusplus
#define BEGIN_C_DECLS extern "C" {
#define END_C_DECLS }
#else
#define BEGIN_C_DECLS
#define END_C_DECLS
#endif

/* assert */
#ifndef return_if_failed
#define return_if_failed(EXPR) \
  {                            \
    if (!(EXPR)) {             \
      return;                  \
    }                          \
  }
#endif

#ifndef return_value_if_failed
#define return_value_if_failed(EXPR, VAL) \
  if (!(EXPR)) {                          \
    return VAL;                           \
  }
#endif

#ifndef goto_error_if_failed
#define goto_error_if_failed(EXPR) \
  if (!(EXPR)) {                   \
    goto error;                    \
  }
#endif

/* struct member offset */
#ifndef INSTANCE_OF
#ifdef __compiler_offsetof
#define INSTANCE_OF(TYPE, MEMBER) __compiler_offsetof(TYPE, MEMBER)
#else
#define INSTANCE_OF(TYPE, MEMBER) ((size_t) & ((TYPE*)0)->MEMBER)
#endif
#endif

/* types */
typedef int32_t ret_t;
typedef enum _ret_type_t {
  RET_OK = 0x0000,
  RET_FAIL,
  RET_BAD_PARAM,
} ret_type_t;

typedef uint8_t bool_t;
typedef enum _bool_type_t {
  TRUE = 0x01,
  FALSE = 0x00,
} bool_type_t;

/* macros */
#define M_E 2.71828f
#define M_PI 3.1415926f

#define UNUSED(v) ((void)v)
#define ARRAY_SIZE(ARR) (sizeof(ARR) / sizeof(ARR[0]))

#endif /*!__SAT_TYPES_H__*/