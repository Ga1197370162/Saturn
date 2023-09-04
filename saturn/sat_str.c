#include "sat_str.h"

#define SAT_STR_REAL(STR) \
  (sat_str_real_t*)((char*)STR - ((uint32_t)INSTANCE_OF(sat_str_real_t, str)))

typedef struct _sat_str_real_t {
  uint32_t used_size;
  uint32_t surplus_size;
  char str[];
} sat_str_real_t;

static uint32_t sat_str_real_obj_len(uint32_t sat_str_len) {
  return sizeof(sat_str_real_t) + sat_str_len;
}

uint32_t sat_str_len(sat_str_t str) {
  sat_str_real_t* sat_str_real = SAT_STR_REAL(str);
  return_value_if_fail(str != NULL && sat_str_real != NULL, 0);
  return sat_str_real->used_size;
}

uint32_t sat_str_avail(sat_str_t str) {
  sat_str_real_t* sat_str_real = SAT_STR_REAL(str);
  return_value_if_fail(str != NULL && sat_str_real != NULL, 0);
  return sat_str_real->surplus_size;
}

sat_str_t sat_str_init(void) {
  return sat_str_init_set_with_len(NULL, 0);
}

sat_str_t sat_str_init_set(const char* str) {
  uint32_t init_len = (str == NULL) ? 0 : strlen(str);
  return sat_str_init_set_with_len(str, init_len);
}

sat_str_t sat_str_init_set_with_len(const char* str, uint32_t init_len) {
  sat_str_real_t* sat_str_real = NULL;

  if (str) {
    sat_str_real = SAT_MEM_MALLOC(sizeof(sat_str_real_t) + init_len + 1);
  } else {
    sat_str_real = SAT_MEM_CALLOC(sizeof(sat_str_real_t) + init_len + 1);
  }
  return_value_if_fail(sat_str_real != NULL, NULL);

  sat_str_real->surplus_size = 0;
  sat_str_real->used_size = init_len;
  if (str && init_len) {
    memcpy(sat_str_real->str, str, init_len);
  }
  sat_str_real->str[init_len] = '\0';

  return sat_str_real->str;
}

ret_t sat_str_clean(sat_str_t str) {
  sat_str_real_t* sat_str_real = SAT_STR_REAL(str);
  return_value_if_fail(str != NULL && sat_str_real != NULL, RET_BAD_PARAM);
  sat_str_real->surplus_size += sat_str_real->used_size;
  sat_str_real->used_size = 0;
  sat_str_real->str[0] = '\0';
  return RET_OK;
}

ret_t sat_str_deinit(sat_str_t str) {
  sat_str_real_t* sat_str_real = SAT_STR_REAL(str);
  return_value_if_fail(str != NULL && sat_str_real != NULL, RET_BAD_PARAM);
  SAT_MEM_FREE(sat_str_real);
  return RET_OK;
}

bool_t sat_str_cmp(sat_str_t src, const char* dst) {
  if (src == NULL || dst == NULL) return FALSE;
  return strcmp(src, dst) == 0 ? TRUE : FALSE;
}

bool_t sat_str_cmp_end_with(sat_str_t src, const char* dst) {
  uint32_t dst_len = strlen(dst);
  uint32_t src_len = sat_str_len(src);
  return strncmp((src + src_len - dst_len), dst, dst_len) == 0 ? TRUE : FALSE;
}

bool_t sat_str_cmp_start_with(sat_str_t src, const char* dst) {
  return strncmp(src, dst, strlen(dst)) == 0 ? TRUE : FALSE;
}

sat_str_t sat_str_dup(sat_str_t str) {
  return sat_str_init_set_with_len(str, strlen(str));
}

ret_t sat_str_remove(sat_str_t str, uint32_t remove_len) {
  sat_str_real_t* sat_str_real = SAT_STR_REAL(str);
  return_value_if_fail(str != NULL && sat_str_real != NULL, RET_BAD_PARAM);
  sat_str_real->str[sat_str_real->used_size - remove_len] = '\0';

  sat_str_real->used_size -= remove_len;
  sat_str_real->surplus_size += remove_len;
  return RET_OK;
}

ret_t sat_str_stay_range(sat_str_t str, int32_t head, int32_t tail) {
  uint32_t len = 0;
  uint32_t new_len = 0;
  sat_str_real_t* sat_str_real = SAT_STR_REAL(str);
  return_value_if_fail(str != NULL && sat_str_real != NULL, RET_BAD_PARAM);

  if (sat_str_real->used_size == 0) return RET_OK;

  len = sat_str_len(str);
  if (head < 0) {
    head += len;
    if (head < 0) head = 0;
  }

  if (tail < 0) {
    tail += len;
    if (tail < 0) tail = 0;
  }

  new_len = (head > tail) ? 0 : (tail - head) + 1;
  if (new_len != 0) {
    if ((uint32_t)head >= len) {
      new_len = 0;
    } else if ((uint32_t)tail >= len) {
      tail = len - 1;
      new_len = (head > tail) ? 0 : (tail - head) + 1;
    }
  } else {
    head = 0;
  }

  if (head && new_len) {
    memmove(sat_str_real->str, sat_str_real->str + head, new_len);
  }

  sat_str_real->str[new_len] = '\0';
  sat_str_real->surplus_size += sat_str_real->used_size - new_len;
  sat_str_real->used_size = new_len;

  return RET_OK;
}

const char* sat_str_find_segment(sat_str_t str, const char c, uint32_t order_number) {
  char* offset = str;
  sat_str_real_t* sat_str_real = SAT_STR_REAL(str);
  return_value_if_fail(str != NULL && sat_str_real != NULL, NULL);

  while (offset <= str + sat_str_real->used_size && order_number != 0) {
    if (*offset == c) {
      order_number -= 1;
      if (order_number == 0) {
        return offset;
      }
    }
    offset += 1;
  }
  return NULL;
}

sat_str_t sat_str_trim(sat_str_t str, const char* cset) {
  uint32_t len;
  char *sp, *ep, *start, *end;
  sat_str_real_t* sat_str_real = SAT_STR_REAL(str);
  return_value_if_fail(str != NULL && sat_str_real != NULL, NULL);

  start = sp = sat_str_real->str;
  end = ep = sat_str_real->str + sat_str_real->used_size - 1;

  while (sp <= end && strchr(cset, *sp)) sp += 1;
  while (ep > start && strchr(cset, *ep)) ep -= 1;

  len = sp >= ep ? 0 : ((ep - sp) + 1);
  if (sat_str_real->str != sp) {
    memmove(sat_str_real->str, sp, len);
  }
  sat_str_real->str[len] = '\0';

  sat_str_real->surplus_size += (sat_str_real->used_size - len);
  sat_str_real->used_size = len;

  return sat_str_real->str;
}

sat_str_t sat_str_grow_room(sat_str_t str, uint32_t add_len) {
  uint32_t len = 0;
  uint32_t new_len = 0;
  sat_str_real_t* new_str = NULL;
  sat_str_real_t* sat_str_real = SAT_STR_REAL(str);
  return_value_if_fail(str != NULL && sat_str_real != NULL, NULL);

  if (sat_str_real->surplus_size >= add_len) return str;

  len = sat_str_len(str);
  new_len = len + add_len;
  if (new_len < SAT_STR_MAX_REALLOC) {
    new_len *= 2;
  } else {
    new_len += SAT_STR_MAX_REALLOC;
  }

  new_str = (sat_str_real_t*)SAT_MEM_REALLOC(sat_str_real, sat_str_real_obj_len(new_len) + 1);
  return_value_if_fail(new_str != NULL, NULL);

  new_str->surplus_size = new_len - len;
  return new_str->str;
}

sat_str_t sat_str_grow_zero(sat_str_t str, uint32_t new_len) {
  uint32_t len = 0;
  uint32_t total_len = 0;
  sat_str_real_t* sat_str_real = SAT_STR_REAL(str);
  return_value_if_fail(str != NULL && sat_str_real != NULL, NULL);

  len = sat_str_len(str);
  if (len >= new_len) return str;
  str = sat_str_grow_room(str, new_len - len);
  return_value_if_fail(str != NULL, NULL);
  sat_str_real = SAT_STR_REAL(str);

  memset(str + len, 0x00, (new_len - len + 1));
  total_len = sat_str_real->used_size + sat_str_real->surplus_size;
  sat_str_real->used_size = new_len;
  sat_str_real->surplus_size = total_len - sat_str_real->used_size;

  return sat_str_real->str;
}

sat_str_t sat_str_cpy(sat_str_t src, const char* dst) {
  return sat_str_cpy_with_len(src, dst, strlen(dst));
}

sat_str_t sat_str_cpy_with_len(sat_str_t str, const char* dst, uint32_t len) {
  uint32_t total_len = 0;
  sat_str_real_t* sat_str_real = SAT_STR_REAL(str);
  return_value_if_fail(str != NULL && sat_str_real != NULL, NULL);

  total_len = sat_str_real->used_size + sat_str_real->surplus_size;
  if (len > total_len) {
    str = sat_str_grow_room(str, len - total_len);
    return_value_if_fail(str != NULL, NULL);
    sat_str_real = SAT_STR_REAL(str);
    total_len = sat_str_real->used_size + sat_str_real->surplus_size;
  }

  memcpy(sat_str_real->str, dst, len);
  sat_str_real->str[len] = '\0';

  sat_str_real->used_size = len;
  sat_str_real->surplus_size = total_len - len;

  return sat_str_real->str;
}

sat_str_t sat_str_append(sat_str_t src, const char* dst) {
  uint32_t src_len = 0;
  uint32_t dst_len = 0;
  sat_str_real_t* sat_str_real = SAT_STR_REAL(src);
  return_value_if_fail(src != NULL && sat_str_real != NULL, NULL);

  dst_len = strlen(dst);
  src_len = sat_str_real->used_size;
  src = sat_str_grow_room(src, dst_len);
  return_value_if_fail(src != NULL, NULL);

  sat_str_real = SAT_STR_REAL(src);
  memcpy(src + src_len, dst, dst_len);

  sat_str_real->used_size += dst_len;
  sat_str_real->surplus_size -= dst_len;
  sat_str_real->str[src_len + dst_len] = '\0';

  return sat_str_real->str;
}

sat_str_t sat_str_append_char(sat_str_t str, const char c) {
  uint32_t sat_str_len = 0;
  sat_str_real_t* sat_str_real = SAT_STR_REAL(str);
  return_value_if_fail(str != NULL && sat_str_real != NULL, NULL);

  sat_str_len = sat_str_real->used_size;
  str = sat_str_grow_room(str, 1);
  return_value_if_fail(str != NULL, NULL);

  sat_str_real = SAT_STR_REAL(str);
  sat_str_real->str[sat_str_len] = c;

  sat_str_real->used_size += 1;
  sat_str_real->surplus_size -= 1;
  sat_str_real->str[sat_str_len + 1] = '\0';

  return sat_str_real->str;
}

static sat_str_t sat_str_append_fmt_ex(sat_str_t str, const char* fmt, va_list ap) {
  va_list cpy;
  uint32_t buf_len = 0;
  char* buf_ptr = NULL;
  char tmp_buf[1024] = {0};
  return_value_if_fail(str != NULL, NULL);

  buf_len = strlen(fmt) * 2;
  if (buf_len > sizeof(tmp_buf)) {
    buf_ptr = SAT_MEM_CALLOC(buf_len);
    return_value_if_fail(buf_ptr != NULL, NULL);
  } else {
    buf_ptr = tmp_buf;
    buf_len = sizeof(tmp_buf);
  }

  while (1) {
    buf_ptr[buf_len - 2] = '\0';
    va_copy(cpy, ap);
    vsnprintf(buf_ptr, buf_len, fmt, cpy);
    if (buf_ptr[buf_len - 2] != '\0') {
      if (buf_ptr != tmp_buf) SAT_MEM_FREE(buf_ptr);
      buf_len *= 2;
      buf_ptr = SAT_MEM_CALLOC(buf_len);
      return_value_if_fail(buf_ptr != NULL, NULL);
      continue;
    }
    break;
  }

  str = sat_str_append(str, buf_ptr);
  if (buf_ptr != tmp_buf) SAT_MEM_FREE(buf_ptr);
  return str;
}

sat_str_t sat_str_append_fmt(sat_str_t str, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  sat_str_t new_str = sat_str_append_fmt_ex(str, fmt, ap);
  va_end(ap);
  return new_str;
}

sat_str_t sat_str_append_n_chars(sat_str_t str, const char c, uint32_t n) {
  return_value_if_fail(str != NULL, NULL);
  while (n-- > 0) {
    str = sat_str_append_char(str, c);
  }
  return str;
}
