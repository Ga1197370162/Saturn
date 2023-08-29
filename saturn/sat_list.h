#ifndef __SAT_LIST_H__
#define __SAT_LIST_H__

#include "sat_types.h"

BEGIN_C_DECLS

/**
 * sat_node_t:
 *  双向/环形链表，存放的数据为 void* , 如果是普通数值类型可以使用 sat_pointer_from_int。
 *
 * create:
 *  sat_list_t* list = sat_list_create(NULL, NULL);
 *  sat_list_append(list, sat_pointer_from_int(5));
 *  sat_list_prepend(list, sat_pointer_from_int(4));
 *  sat_list_head_pop(list, sat_pointer_from_int(3));
 *  sat_list_destory(list);
 */

typedef struct _sat_node_t {
  void* data;
  struct _sat_node_t* prve;
  struct _sat_node_t* next;
} sat_node_t;

typedef struct _sat_list_t {
  bool_t is_cycle;
  sat_node_t* first;
  sat_destory_t destory;
  sat_compare_t compare;
} sat_list_t;

sat_list_t* sat_list_init(sat_list_t* list, sat_destory_t destory, sat_compare_t compare);
ret_t sat_list_deinit(sat_list_t* list);

sat_list_t* sat_list_create(sat_destory_t destory, sat_compare_t compare);
ret_t sat_list_destory(sat_list_t* list);

ret_t sat_list_clean(sat_list_t* list);
ret_t sat_list_set_sycle(sat_list_t* list, bool_t is_cycle);

inline bool_t sat_list_is_empty(sat_list_t* list);
inline bool_t sat_list_is_cycle(sat_list_t* list);
inline uint32_t sat_list_count_size(sat_list_t* list);

ret_t sat_list_append(sat_list_t* list, void* data);
ret_t sat_list_prepend(sat_list_t* list, void* data);
ret_t sat_list_insert(sat_list_t* list, int32_t index, void* data);
ret_t sat_list_insert_sort(sat_list_t* list, sat_compare_t compare, void* data);

ret_t sat_list_head_pop(sat_list_t* list);
ret_t sat_list_tail_pop(sat_list_t* list);
ret_t sat_list_remove(sat_list_t* list, int32_t index);
ret_t sat_list_remove_element(sat_list_t* list, sat_compare_t compare, void* data);
ret_t sat_list_remove_element_with_num(sat_list_t* list, sat_compare_t compare, void* data,
                                       uint32_t num);

void* sat_list_get(sat_list_t* list, int32_t index);
void* sat_list_find(sat_list_t* list, sat_compare_t compare, void* ctx);
bool_t sat_list_find_elem_is_exist(sat_list_t* list, sat_compare_t compare, void* ctx);

ret_t sat_list_set(sat_list_t* list, int32_t index, void* data);
ret_t sat_list_replace(sat_list_t* list, int32_t index, void* data);

ret_t sat_list_sort(sat_list_t* list, sat_compare_t compare);
ret_t sat_list_foreach(sat_list_t* list, sat_foreach_t foreach, void* ctx);

END_C_DECLS

#endif /*!__SAT_LIST_H__*/