#ifndef __SAT_ARRAY_H__
#define __SAT_ARRAY_H__

#include "sat_types.h"

BEGIN_C_DECLS

/**
 * sat_array_t:
 *  可变长数组, 存储的数据为void*, 如果是普通数值类型可以使用 sat_pointer_from_int。
 *
 * create:
 *  sat_array_t* array = sat_array_create(5, NULL, NULL);
 *  sat_array_push_back(array, sat_pointer_from_int(1));
 *  sat_array_push_head(array, sat_pointer_from_int(2));
 *  int i = sat_pointer_to_int(sat_array_get(array, 0));
 *  sat_array_destroy(array);
 */

typedef struct _sat_array_t {
  void** elems;
  uint32_t size;
  uint32_t capcity;

  sat_destory_t destory;
  sat_compare_t compare;
} sat_array_t;

sat_array_t* sat_array_init(sat_array_t* array, uint32_t capcity, sat_destory_t destory,
                            sat_compare_t compare);
ret_t sat_array_deinit(sat_array_t* array);

sat_array_t* sat_array_create(uint32_t capcity, sat_destory_t destory, sat_compare_t compare);
ret_t sat_array_destroy(sat_array_t* array);
ret_t sat_array_clean(sat_array_t* array);

inline uint32_t sat_array_size(sat_array_t* array);
inline uint32_t sat_array_capcity(sat_array_t* array);
inline bool_t sat_array_is_empty(sat_array_t* array);

ret_t sat_array_insert(sat_array_t* array, int32_t index, void* data);
ret_t sat_array_push_head(sat_array_t* array, void* data);
ret_t sat_array_push_back(sat_array_t* array, void* data);

ret_t sat_array_remove(sat_array_t* array, int32_t index);
ret_t sat_array_pop_head(sat_array_t* array);
ret_t sat_array_pop_back(sat_array_t* array);
ret_t sat_array_remove_element(sat_array_t* array, sat_compare_t compare, void* data);
ret_t sat_array_remove_element_with_num(sat_array_t* array, sat_compare_t compare, void* data,
                                        int32_t num);

void* sat_array_get(sat_array_t* array, int32_t index);
void* sat_array_find(sat_array_t* array, sat_compare_t compare, void* ctx);
void* sat_array_binary_find(sat_array_t* array, sat_compare_t compare, void* ctx);
bool_t sat_array_find_elem_is_exist(sat_array_t* array, sat_compare_t compare, void* ctx);

ret_t sat_array_set(sat_array_t* array, int32_t index, void* data);
ret_t sat_array_replace(sat_array_t* array, int32_t index, void* data);

ret_t sat_array_sort(sat_array_t* array, sat_compare_t compare);
ret_t sat_array_foreach(sat_array_t* array, sat_foreach_t foreach, void* ctx);

END_C_DECLS

#endif /*!__SAT_ARRAY_H__*/