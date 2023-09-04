#include "sat_array.h"
#include "gtest/gtest.h"

#include <iostream>
using namespace std;

static ret_t sat_array_defult_foreach(void* ctx, const void* data) {
  UNUSE(ctx);
  int32_t v = sat_pointer_to_int((void*)data);
  cout << v << endl;
  return RET_OK;
}

TEST(array_t, push) {
  sat_array_t array;

  EXPECT_NE(sat_array_init(&array, 1, NULL, sat_default_compare), nullptr);
  EXPECT_EQ(sat_array_push_back(&array, sat_pointer_from_int(1)), RET_OK);
  EXPECT_EQ(sat_array_push_back(&array, sat_pointer_from_int(2)), RET_OK);
  EXPECT_EQ(sat_array_push_back(&array, sat_pointer_from_int(3)), RET_OK);
  EXPECT_EQ(sat_array_push_back(&array, sat_pointer_from_int(4)), RET_OK);

  EXPECT_EQ(sat_array_push_head(&array, sat_pointer_from_int(1)), RET_OK);
  EXPECT_EQ(sat_array_push_head(&array, sat_pointer_from_int(2)), RET_OK);
  EXPECT_EQ(sat_array_push_head(&array, sat_pointer_from_int(3)), RET_OK);
  EXPECT_EQ(sat_array_push_head(&array, sat_pointer_from_int(4)), RET_OK);

  EXPECT_EQ(sat_array_size(&array), (uint32_t)8);
  EXPECT_EQ(sat_array_is_empty(&array), FALSE);

  EXPECT_EQ(sat_array_foreach(&array, sat_array_defult_foreach, nullptr), RET_OK);
  EXPECT_EQ(sat_array_deinit(&array), RET_OK);
}

TEST(array_t, insert) {
  sat_array_t array;
  EXPECT_NE(sat_array_init(&array, 1, nullptr, sat_default_compare), nullptr);

  EXPECT_EQ(sat_array_insert(&array, 6, sat_pointer_from_int(1)), RET_OK);
  EXPECT_EQ(sat_array_insert(&array, -1, sat_pointer_from_int(2)), RET_OK);
  EXPECT_EQ(sat_array_insert(&array, -3, sat_pointer_from_int(3)), RET_OK);
  EXPECT_EQ(sat_array_insert(&array, -10, sat_pointer_from_int(4)), RET_OK);
  EXPECT_EQ(sat_array_insert(&array, 9, sat_pointer_from_int(5)), RET_OK);
  EXPECT_EQ(sat_array_insert(&array, 8, sat_pointer_from_int(6)), RET_OK);
  EXPECT_EQ(sat_array_insert(&array, 7, sat_pointer_from_int(7)), RET_OK);
  EXPECT_EQ(sat_array_insert(&array, 6, sat_pointer_from_int(8)), RET_OK);

  EXPECT_EQ(sat_array_size(&array), (uint32_t)8);
  EXPECT_EQ(sat_array_is_empty(&array), FALSE);

  EXPECT_EQ(sat_array_foreach(&array, sat_array_defult_foreach, nullptr), RET_OK);
  EXPECT_EQ(sat_array_deinit(&array), RET_OK);
}
