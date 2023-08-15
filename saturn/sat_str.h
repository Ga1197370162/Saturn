#ifndef __SAT_STR_H__
#define __SAT_STR_H__

#include "sat_types.h"

BEGIN_C_DECLS

typedef char* sat_str_t;

#ifndef SAT_STR_MAX_REALLOC
#define SAT_STR_MAX_REALLOC (1024 * 1024)
#endif /*SAT_STR_MAX_REALLOC*/

uint32_t sat_str_len(sat_str_t str);
uint32_t sat_str_avail(sat_str_t str);

sat_str_t sat_str_init(void);
sat_str_t sat_str_init_set(const char* str);
sat_str_t sat_str_init_set_with_len(const char* str, uint32_t init_len);
ret_t sat_str_clean(sat_str_t str);
ret_t sat_str_deinit(sat_str_t str);

bool_t sat_str_cmp(sat_str_t src, const char* dst);
bool_t sat_str_cmp_end_with(sat_str_t src, const char* dst);
bool_t sat_str_cmp_start_with(sat_str_t src, const char* dst);

sat_str_t sat_str_dup(sat_str_t str);
ret_t sat_str_remove(sat_str_t str, uint32_t remove_len);
ret_t sat_str_stay_range(sat_str_t str, int32_t head, int32_t tail);

const char* sat_str_find_segment(sat_str_t str, const char c, uint32_t order_number);

/* will change orgin str function, so ensure original sat_str_t update return value */
sat_str_t sat_str_grow_room(sat_str_t str, uint32_t add_len);
sat_str_t sat_str_grow_zero(sat_str_t str, uint32_t new_len);
sat_str_t sat_str_cpy(sat_str_t src, const char* dst);
sat_str_t sat_str_cpy_with_len(sat_str_t str, const char* dst, uint32_t len);
sat_str_t sat_str_trim(sat_str_t str, const char* cset);
sat_str_t sat_str_append(sat_str_t src, const char* dst);
sat_str_t sat_str_append_char(sat_str_t str, const char c);
sat_str_t sat_str_append_fmt(sat_str_t str, const char* fmt, ...);
sat_str_t sat_str_append_n_chars(sat_str_t str, const char c, uint32_t n);

END_C_DECLS

#endif /*__SAT_STR_H__*/