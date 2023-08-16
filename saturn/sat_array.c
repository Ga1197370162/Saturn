#include "sat_array.h"

static ret_t sat_array_upper_capcity_ex(sat_array_t* array, uint32_t capcity) {
  void** elems = NULL;
  return_value_if_fail(array != NULL && capcity > 0 && capcity < UINT32_MAX, RET_BAD_PARAM);

  elems = SAT_MEM_REALLOCT(void*, array->elems, capcity);
  return_value_if_fail(elems != NULL, RET_OOM);

  array->elems = elems;
  array->capcity = capcity;

  return RET_OK;
}

static ret_t sat_array_upper_capcity(sat_array_t* array) {
  uint32_t capcity = 0;
  return_value_if_fail(array != NULL, RET_BAD_PARAM);

  if (array->size < array->capcity) {
    return RET_OK;
  }

  capcity = (array->capcity >> 1) + array->capcity + 1;
  return sat_array_upper_capcity_ex(array, capcity);
}

sat_array_t* sat_array_init(sat_array_t* array, uint32_t capcity, sat_destory_t destory,
                            sat_compare_t compare) {
  return_value_if_fail(array != NULL, NULL);

  array->size = 0;
  array->capcity = 0;
  array->elems = NULL;
  array->destory = destory ? destory : sat_dummpy_destory;
  array->compare = compare ? compare : sat_default_compare;

  return array;
}

ret_t sat_array_deinit(sat_array_t* array) {
  return_value_if_fail(array != NULL, RET_BAD_PARAM);

  sat_array_clean(array);
  if (array->elems != NULL) {
    SAT_MEM_FREE(array->elems);
  }
  array->capcity = 0;

  return RET_OK;
}

sat_array_t* sat_array_create(uint32_t capcity, sat_destory_t destory, sat_compare_t compare) {
  sat_array_t* array = SAT_MEM_ZALLOC(sat_array_t);
  return_value_if_fail(array != NULL, NULL);
  goto_error_if_fail(sat_array_init(array, capcity, destory, compare) != NULL);
  return array;
error:
  if (array != NULL) {
    SAT_MEM_FREE(array);
  }
  return NULL;
}

ret_t sat_array_destroy(sat_array_t* array) {
  return_value_if_fail(array != NULL, RET_BAD_PARAM);

  sat_array_deinit(array);
  SAT_MEM_FREE(array);

  return RET_OK;
}

ret_t sat_array_clean(sat_array_t* array) {
  uint32_t i = 0;
  return_value_if_fail(array != NULL, RET_BAD_PARAM);

  for (i = 0; i < array->size; ++i) {
    array->destory(array->elems[i]);
  }
  array->size = 0;

  return RET_OK;
}

uint32_t sat_array_size(sat_array_t* array) {
  return_value_if_fail(array != NULL, -1);
  return array->size;
}

uint32_t sat_array_capcity(sat_array_t* array) {
  return_value_if_fail(array != NULL, -1);
  return array->capcity;
}

bool_t sat_array_is_empty(sat_array_t* array) {
  bool_t is_empty = FALSE;
  return_value_if_fail(array != NULL, FALSE);

  if (array->elems && array->size > 0) {
    is_empty = TRUE;
  }
  return is_empty;
}

static int32_t sat_array_fix_index(sat_array_t* array, int32_t index) {
  return_value_if_fail(array != NULL, index);
  if (array->size == 0) return 0;

  if (index < 0) {
    index = array->size;
    if (index < 0) index = 0;
  } else if ((uint32_t)index > array->size - 1) {
    index = array->size - 1;
  }
  return index;
}

ret_t sat_array_insert(sat_array_t* array, int32_t index, void* data) {
  uint32_t i = 0;
  void* tmp_elem = NULL;
  return_value_if_fail(array != NULL && data != NULL, RET_BAD_PARAM);
  return_value_if_fail(sat_array_upper_capcity(array) == RET_OK, RET_FAIL);

  index = sat_array_fix_index(array, index);

  for (i = index; i <= array->size; ++i) {
    tmp_elem = array->elems[i];
    array->elems[i] = data;
    data = tmp_elem;
  }
  array->size += 1;

  return RET_OK;
}

ret_t sat_array_push_head(sat_array_t* array, void* data) {
  return sat_array_insert(array, 0, data);
}

ret_t sat_array_push_back(sat_array_t* array, void* data) {
  return sat_array_insert(array, -1, data);
}

ret_t sat_array_remove(sat_array_t* array, int32_t index) {
  uint32_t i = 0;
  void* tmp_elem = NULL;
  return_value_if_fail(array != NULL, RET_BAD_PARAM);

  index = sat_array_fix_index(array, index);
  array->size -= 1;

  for (i = index; i < array->size; ++i) {
    tmp_elem = array->elems[i];
    array->elems[i] = array->elems[i + 1];
    array->elems[i + 1] = tmp_elem;
  }

  array->destory(array->elems[array->size + 1]);
  array->elems[array->size + 1] = NULL;

  return RET_OK;
}

ret_t sat_array_pop_head(sat_array_t* array) {
  return sat_array_remove(array, 0);
}

ret_t sat_array_pop_back(sat_array_t* array) {
  return sat_array_remove(array, -1);
}

ret_t sat_array_remove_element(sat_array_t* array, sat_compare_t compare, void* data) {
  uint32_t i = 0;
  return_value_if_fail(array != NULL, RET_BAD_PARAM);

  compare = compare ? compare : array->compare;
  for (i = 0; i < array->size; ++i) {
    if (compare(data, array->elems[i]) == 0) {
      return sat_array_remove(array, i);
    }
  }
  return RET_NOT_FOUND;
}

ret_t sat_array_remove_element_with_num(sat_array_t* array, sat_compare_t compare, void* data,
                                        int32_t num) {
  ret_t ret = RET_OK;
  return_value_if_fail(array != NULL, RET_BAD_PARAM);

  if (num < 0) {
    num = array->size;
  }

  while (ret == RET_OK && num > 0) {
    ret = sat_array_remove_element(array, compare, data);
    if (ret == RET_NOT_FOUND) {
      break;
    }
    num -= 1;
  }
  return ret;
}

void* sat_array_get(sat_array_t* array, int32_t index) {
  return_value_if_fail(array != NULL, NULL);
  index = sat_array_fix_index(array, index);
  return array->elems[index];
}

void* sat_array_find(sat_array_t* array, sat_compare_t compare, void* ctx) {
  uint32_t i = 0;
  return_value_if_fail(array != NULL, NULL);

  compare = compare ? compare : array->compare;
  for (i = 0; i < array->size; ++i) {
    if (compare(ctx, array->elems[i]) == 0) {
      return array->elems[i];
    }
  }

  return NULL;
}

void* sat_array_binary_find(sat_array_t* array, sat_compare_t compare, void* ctx) {
  int32_t start = 0, end = 0, index = 0, result = 0;
  return_value_if_fail(array != NULL, NULL);

  start = 0;
  end = array->size - 1;
  compare = compare ? compare : array->compare;

  while (start <= end) {
    index = start + ((end - start) >> 1);
    result = compare(ctx, array->elems[index]);
    if (result == 0) {
      return array->elems[index];
    } else if (result > 0) {
      start = index + 1;
    } else if (result < 0) {
      end = index - 1;
    }
  }

  return NULL;
}

bool_t sat_array_find_elem_is_exist(sat_array_t* array, sat_compare_t compare, void* data) {
  bool_t is_exist = FALSE;

  if (sat_array_find(array, compare, data) != NULL) {
    is_exist = TRUE;
  }
  return is_exist;
}

ret_t sat_array_set(sat_array_t* array, int32_t index, void* data) {
  return_value_if_fail(array != NULL && data != NULL, RET_BAD_PARAM);

  index = sat_array_fix_index(array, index);
  array->elems[index] = data;

  return RET_OK;
}

ret_t sat_array_replace(sat_array_t* array, int32_t index, void* data) {
  void* tmp_elem = NULL;
  return_value_if_fail(array != NULL && data != NULL, RET_BAD_PARAM);

  tmp_elem = sat_array_get(array, index);
  sat_array_set(array, index, data);

  return array->destory(tmp_elem);
}

ret_t sat_array_sort(sat_array_t* array, sat_compare_t compare) {
  uint32_t i = 0;
  void* tmp_elem = NULL;
  return_value_if_fail(array != NULL, RET_BAD_PARAM);

  compare = compare ? compare : array->compare;
  for (i = 0; i < array->size - 2; ++i) {
    if (compare(array->elems[i], array->elems[i + 1]) > 0) {
      tmp_elem = array->elems[i];
      array->elems[i] = array->elems[i + 1];
      array->elems[i + 1] = tmp_elem;
    }
  }

  return RET_OK;
}

ret_t sat_array_foreach(sat_array_t* array, sat_foreach_t foreach, void* ctx) {
  uint32_t i = 0;
  ret_t ret = RET_OK;
  return_value_if_fail(array != NULL, RET_BAD_PARAM);

  if (foreach == NULL) {
    return RET_OK;
  }

  for (i = 0; ret == RET_OK && i < array->size; ++i) {
    ret = foreach (ctx, array->elems[i]);
  }

  return ret;
}