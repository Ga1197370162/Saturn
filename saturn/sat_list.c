#include "sat_list.h"

static sat_node_t* sat_list_get_node_by_index(sat_list_t* list, int32_t index);

static sat_node_t* sat_node_create(void* data) {
  sat_node_t* node = SAT_MEM_ZALLOC(sat_node_t);
  return_value_if_fail(node != NULL, NULL);
  node->data = data;
  node->prve = node->next = NULL;
  return node;
}

static ret_t sat_node_destory(sat_node_t* node, sat_destory_t destory) {
  return_value_if_fail(node != NULL, RET_BAD_PARAM);

  if (node->data != NULL) {
    destory(node->data);
  }

  node->prve = node->next = NULL;
  SAT_MEM_FREE(node);
  return RET_OK;
}

sat_list_t* sat_list_init(sat_list_t* list, sat_destory_t destory, sat_compare_t compare) {
  return_value_if_fail(list != NULL, NULL);

  list->first = NULL;
  list->is_cycle = FALSE;
  list->destory = destory;
  list->compare = compare;

  return list;
}

ret_t sat_list_deinit(sat_list_t* list) {
  return_value_if_fail(list != NULL, RET_BAD_PARAM);
  sat_list_clean(list);
  return RET_OK;
}

sat_list_t* sat_list_create(sat_destory_t destory, sat_compare_t compare) {
  sat_list_t* list = SAT_MEM_ZALLOC(sat_list_t);
  return_value_if_fail(list != NULL, NULL);
  sat_list_init(list, destory, compare);
  return list;
}

ret_t sat_list_destory(sat_list_t* list) {
  return_value_if_fail(list != NULL, RET_BAD_PARAM);
  sat_list_deinit(list);
  SAT_MEM_FREE(list);
  return RET_OK;
}

ret_t sat_list_clean(sat_list_t* list) {
  sat_node_t* iter = NULL;
  return_value_if_fail(list != NULL, RET_BAD_PARAM);

  iter = list->first;
  while (iter != NULL && iter->next != list->first) {
    sat_node_t* node = iter;

    iter = iter->next;
    sat_node_destory(node, list->destory);
  }

  return RET_OK;
}

ret_t sat_list_set_sycle(sat_list_t* list, bool_t is_cycle) {
  return_value_if_fail(list != NULL, RET_BAD_PARAM);

  if (list->first != NULL && list->is_cycle != is_cycle) {
    sat_node_t *head = NULL, *tail = NULL;

    if (is_cycle) {
      head = list->first;
      tail = sat_list_get_node_by_index(list, -1);

      head->prve = tail;
      tail->next = head;
    } else {
      head = list->first;
      tail = list->first->prve;

      head->prve = NULL;
      tail->next = NULL;
    }
  }

  list->is_cycle = is_cycle;
  return RET_OK;
}

bool_t sat_list_is_empty(sat_list_t* list) {
  return_value_if_fail(list != NULL, TRUE);
  return (list->first == NULL) ? TRUE : FALSE;
}

bool_t sat_list_is_cycle(sat_list_t* list) {
  return_value_if_fail(list != NULL, FALSE);
  return list->is_cycle;
}

uint32_t sat_list_count_size(sat_list_t* list) {
  uint32_t i = 0;
  sat_node_t* iter = NULL;
  return_value_if_fail(list != NULL, 0);

  iter = list->first;
  while (iter != NULL && iter->next != list->first && i++) {
    iter = iter->next;
  }
  return i;
}

static int32_t sat_list_fix_index(sat_list_t* list, int32_t index) {
  int32_t list_size = sat_list_count_size(list);

  if (list_size == 0) return 0;

  if (index < -1) {
    index += list_size + 1;
    if (index < 0) index = 0;
  } else if ((uint32_t)index > list_size - 1) {
    index = -1;
  }

  return index;
}

static sat_node_t* sat_list_get_node_by_index(sat_list_t* list, int32_t index) {
  uint32_t i = 0;
  sat_node_t* node = NULL;
  return_value_if_fail(list != NULL, NULL);

  node = list->first;
  if (node == NULL) {
    return NULL;
  }

  if (list->is_cycle && index < 0) {
    while (i++ < ABS(index)) {
      node = index > 0 ? node->next : node->prve;
    }
  } else {
    while (i++ < index) {
      node = node->next;
    }
  }
  return_value_if_fail(node != NULL, NULL);

  return node;
}

ret_t sat_list_append(sat_list_t* list, void* data) {
  sat_node_t* node = NULL;
  return_value_if_fail(list != NULL && data != NULL, RET_BAD_PARAM);

  node = sat_node_create(data);
  return_value_if_fail(node != NULL, RET_OOM);

  if (list->is_cycle) {
    sat_node_t* head = list->first == NULL ? node : list->first;
    sat_node_t* tail = list->first == NULL ? node : list->first->next;

    node->prve = tail;
    node->next = head;

    head->prve = node;
    tail->next = node;
  } else {
    node->next = list->first;
    if (list->first != NULL) {
      list->first->prve = node;
    }
  }

  list->first = node;
  return RET_OK;
}

ret_t sat_list_prepend(sat_list_t* list, void* data) {
  sat_node_t* node = NULL;
  return_value_if_fail(list != NULL && data != NULL, RET_BAD_PARAM);

  node = sat_node_create(data);
  return_value_if_fail(node != NULL, RET_OOM);

  if (list->is_cycle) {
    sat_node_t* head = list->first == NULL ? node : list->first;
    sat_node_t* tail = list->first == NULL ? node : list->first->prve;

    node->prve = tail;
    node->next = head;

    head->prve = node;
    tail->next = node;
  } else {
    sat_node_t* iter = sat_list_get_node_by_index(list, -1);

    node->prve = iter;
    if (iter != NULL) {
      iter->next = node;
    }
  }

  return RET_OK;
}

ret_t sat_list_insert(sat_list_t* list, int32_t index, void* data) {
  ret_t ret = RET_OK;
  return_value_if_fail(list != NULL && data != NULL, RET_BAD_PARAM);

  index = sat_list_fix_index(list, index);

  if (index == 0) {
    ret = sat_list_append(list, data);
  } else if (index == -1) {
    ret = sat_list_prepend(list, data);
  } else {
    sat_node_t *node = sat_node_create(data), *next = NULL;
    return_value_if_fail(node != NULL, RET_OOM);

    next = sat_list_get_node_by_index(list, index);
    node->next = next;
    node->prve = next->prve;
    next->prve = node;
  }

  return ret;
}

ret_t sat_list_insert_sort(sat_list_t* list, sat_compare_t compare, void* data) {
  int32_t index = 0;
  sat_node_t* iter = NULL;
  return_value_if_fail(list != NULL && data != NULL, RET_BAD_PARAM);

  iter = list->first;
  compare = compare != NULL ? compare : list->compare;
  while (iter != NULL && iter->next != list->first && compare(data, iter->data) >= 0) {
    index += 1;
    iter = iter->next;
  }

  return sat_list_insert(list, index, data);
}

ret_t sat_list_head_pop(sat_list_t* list) {
  sat_node_t* node = NULL;
  return_value_if_fail(list != NULL, RET_BAD_PARAM);

  if (list->first == NULL) {
    return RET_OK;
  }

  node = list->first;
  if (list->is_cycle) {
    sat_node_t* head = node->prve;
    sat_node_t* tail = node->next;

    head->next = tail;
    tail->prve = head;

    list->first = node->next == node ? NULL : node->next;
  } else {
    if (node->next != NULL) {
      node->next->prve = NULL;
    }
    list->first = node->next;
  }

  sat_node_destory(node, list->destory);
  return RET_OK;
}

ret_t sat_list_tail_pop(sat_list_t* list) {
  sat_node_t* node = NULL;
  return_value_if_fail(list != NULL, RET_BAD_PARAM);

  node = sat_list_get_node_by_index(list, -1);
  if (node == NULL) {
    return RET_OK;
  }

  if (list->is_cycle) {
    sat_node_t* head = node->prve;
    sat_node_t* tail = node->next;

    head->next = tail;
    tail->prve = head;
  } else {
    if (node->prve != NULL) {
      node->prve = NULL;
    }
  }

  sat_node_destory(node, list->destory);
  return RET_OK;
}

ret_t sat_list_remove(sat_list_t* list, int32_t index) {
  ret_t ret = RET_OK;
  sat_node_t* node = NULL;
  return_value_if_fail(list != NULL, RET_BAD_PARAM);

  index = sat_list_fix_index(list, index);

  if (index == -1) {
    ret = sat_list_head_pop(list);
  } else if (index == 0) {
    ret = sat_list_tail_pop(list);
  } else {
    node = sat_list_get_node_by_index(list, index);

    node->prve->next = node->next;
    node->next->prve = node->prve;

    sat_node_destory(node, list->destory);
  }

  return ret;
}

ret_t sat_list_remove_element(sat_list_t* list, sat_compare_t compare, void* data) {
  int32_t i = 0;
  sat_node_t* iter = NULL;
  ret_t ret = RET_NOT_FOUND;
  return_value_if_fail(list != NULL && data != NULL, RET_BAD_PARAM);

  iter = list->first;
  compare = compare != NULL ? compare : list->compare;
  while (ret != RET_OK && iter != NULL && iter->next != list->first) {
    if (compare(data, iter->data) == 0) {
      ret = sat_list_remove(list, i);
    } else {
      i += 1;
      iter = iter->next;
    }
  }

  return ret;
}

ret_t sat_list_remove_element_with_num(sat_list_t* list, sat_compare_t compare, void* data,
                                       uint32_t num) {
  uint32_t i = 0;
  ret_t ret = RET_OK;

  while (ret != RET_OK && i < num) {
    ret = sat_list_remove_element(list, compare, data);
    if (ret == RET_OK) {
      i += 1;
    }
  }

  return ret;
}

void* sat_list_get(sat_list_t* list, int32_t index) {
  void* data = NULL;
  sat_node_t* node = NULL;
  return_value_if_fail(list != NULL, NULL);

  index = sat_list_fix_index(list, index);
  node = sat_list_get_node_by_index(list, index);
  if (node != NULL) {
    data = node->data;
  }
  return data;
}

void* sat_list_find(sat_list_t* list, sat_compare_t compare, void* ctx) {
  int32_t i = 0;
  void* data = NULL;
  sat_node_t* iter = NULL;
  return_value_if_fail(list != NULL && ctx != NULL, NULL);

  iter = list->first;
  compare = compare != NULL ? compare : list->compare;
  while (iter != NULL && iter->next != list->first && data == NULL) {
    if (compare(ctx, iter->data) == 0) {
      data = sat_list_get(list, i);
    } else {
      i += 1;
      iter = iter->next;
    }
  }

  return NULL;
}

bool_t sat_list_find_elem_is_exist(sat_list_t* list, sat_compare_t compare, void* ctx) {
  void* data = NULL;
  data = sat_list_find(list, compare, ctx);
  return (data != NULL) ? TRUE : FALSE;
}

ret_t sat_list_set(sat_list_t* list, int32_t index, void* data) {
  sat_node_t* node = NULL;
  return_value_if_fail(list != NULL && list->first != NULL && data != NULL, RET_BAD_PARAM);

  node = sat_list_get_node_by_index(list, index);
  node->data = data;

  return RET_OK;
}

ret_t sat_list_replace(sat_list_t* list, int32_t index, void* data) {
  void* tmp_data = NULL;
  sat_node_t* node = NULL;
  return_value_if_fail(list != NULL && list->first != NULL && data != NULL, RET_BAD_PARAM);

  node = sat_list_get_node_by_index(list, index);

  tmp_data = node->data;
  node->data = data;

  return list->destory(tmp_data);
}

ret_t sat_list_sort(sat_list_t* list, sat_compare_t compare) {
  void* tmp_data = NULL;
  sat_node_t* iter = NULL;
  return_value_if_fail(list != NULL, RET_BAD_PARAM);

  iter = list->first;
  compare = compare != NULL ? compare : list->compare;
  while (iter != NULL && iter->next != NULL && iter->next != list->first) {
    if (compare(iter->data, iter->next->data) > 0) {
      tmp_data = iter->data;
      iter->data = iter->next->data;
      iter->next->data = tmp_data;
    } else {
      iter = iter->next;
    }
  }

  return RET_OK;
}

ret_t sat_list_foreach(sat_list_t* list, sat_foreach_t foreach, void* ctx) {
  ret_t ret = RET_OK;
  sat_node_t* iter = NULL;
  return_value_if_fail(list != NULL, RET_BAD_PARAM);

  if (foreach == NULL) {
    return RET_OK;
  }

  iter = list->first;
  while (ret == RET_OK && iter != NULL && iter->next != list->first) {
    ret = foreach (ctx, iter->data);
    iter = iter->next;
  }

  return ret;
}