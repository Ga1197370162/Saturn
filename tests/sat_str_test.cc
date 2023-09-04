#include "gtest.h"
#include "sat_str.h"

TEST(sat_str_t, basic) {
  {
    sat_str_t string = sat_str_init();
    EXPECT_EQ(sat_str_len(string), 0u);
    EXPECT_EQ(sat_str_avail(string), 0u);
    sat_str_deinit(string);
  }

  {
    sat_str_t string = sat_str_init_set("hello world");
    EXPECT_EQ(sat_str_len(string), 11u);
    EXPECT_EQ(sat_str_avail(string), 0u);
    EXPECT_TRUE(sat_str_cmp(string, "hello world"));
    sat_str_deinit(string);
  }

  {
    sat_str_t string = sat_str_init_set_with_len("hello world", 5);
    EXPECT_EQ(sat_str_len(string), 5u);
    EXPECT_EQ(sat_str_avail(string), 0u);
    EXPECT_TRUE(sat_str_cmp(string, "hello"));
    sat_str_deinit(string);
  }

  {
    sat_str_t string = sat_str_init_set("hello world");
    sat_str_clean(string);
    EXPECT_EQ(sat_str_len(string), 0u);
    EXPECT_EQ(sat_str_avail(string), 11u);
    EXPECT_TRUE(sat_str_cmp(string, ""));
    sat_str_deinit(string);
  }
}

TEST(sat_str_t, cmp) {
  sat_str_t string = sat_str_init_set("<saturn><body>hello world</body></saturn>");

  EXPECT_TRUE(sat_str_cmp(string, "<saturn><body>hello world</body></saturn>"));
  EXPECT_FALSE(sat_str_cmp(string, "<pris><body>hello world</body></saturn>"));
  EXPECT_FALSE(sat_str_cmp(string, "<saturn><body>hello wor/body></saturn>"));

  EXPECT_TRUE(sat_str_cmp_end_with(string, "</saturn>"));
  EXPECT_FALSE(sat_str_cmp_end_with(string, "<slim>"));
  EXPECT_FALSE(sat_str_cmp_end_with(string, "</saturN>"));

  EXPECT_TRUE(sat_str_cmp_start_with(string, "<saturn>"));
  EXPECT_FALSE(sat_str_cmp_start_with(string, "<slim>"));
  EXPECT_FALSE(sat_str_cmp_start_with(string, "<saturN>"));

  sat_str_deinit(string);
}

TEST(sat_str_t, operator1) {
  sat_str_t string = sat_str_init_set("this is buffer temp!");

  sat_str_t string_copy = sat_str_dup(string);
  EXPECT_TRUE(sat_str_cmp_start_with(string, string_copy));
  sat_str_deinit(string_copy);

  sat_str_stay_range(string, 2, -2);
  EXPECT_TRUE(sat_str_cmp_start_with(string, "is is buffer tem"));

  sat_str_stay_range(string, -5, -2);
  EXPECT_TRUE(sat_str_cmp_start_with(string, " tem"));

  sat_str_deinit(string);
}

TEST(sat_str_t, will_change_orgin1) {
  sat_str_t cur = NULL;
  sat_str_t org = sat_str_init_set("will change orgin str, so need deal return str!");

  cur = org = sat_str_grow_room(org, 10);
  EXPECT_EQ(sat_str_len(cur), 47u);
  EXPECT_EQ(sat_str_avail(cur), (uint32_t)(47 + (10 * 2)));

  cur = org = sat_str_grow_zero(org, 10);
  EXPECT_EQ(sat_str_len(cur), 47u);
  EXPECT_EQ(sat_str_avail(cur), (uint32_t)(47 + (10 * 2)));

  cur = org = sat_str_cpy(org, "hello world");
  EXPECT_EQ(sat_str_len(cur), 11u);
  EXPECT_EQ(sat_str_avail(cur), (uint32_t)((47 + 10) * 2 - 11));
  EXPECT_TRUE(sat_str_cmp(cur, "hello world"));

  cur = org = sat_str_cpy_with_len(org, "saturn is study test framework!", 21);
  EXPECT_EQ(sat_str_len(cur), 21u);
  EXPECT_EQ(sat_str_avail(cur), (uint32_t)((47 + 10) * 2 - 21));
  EXPECT_TRUE(sat_str_cmp(cur, "saturn is study test "));

  cur = org = sat_str_trim(org, " ");
  EXPECT_TRUE(sat_str_cmp(cur, "saturn is study test"));

  cur = org = sat_str_cpy(org, "AA...AA.a.aa.aHelloWorld     :::");
  cur = org = sat_str_trim(org, "A. a:");
  EXPECT_TRUE(sat_str_cmp(cur, "HelloWorld"));

  sat_str_deinit(org);
}

TEST(sat_str_t, will_change_orgin2) {
  sat_str_t cur = NULL;

  {
    sat_str_t org = sat_str_init();
    cur = org = sat_str_append(org, "hello world");
    EXPECT_TRUE(sat_str_cmp(cur, "hello world"));
    sat_str_deinit(org);
  }

  {
    sat_str_t org = sat_str_init_set("will change orgin str");
    cur = org = sat_str_append(org, "z z  z   ");
    EXPECT_TRUE(sat_str_cmp(cur, "will change orgin strz z  z   "));
    cur = org = sat_str_append(org, "\t\n\\\\");
    EXPECT_TRUE(sat_str_cmp(cur, "will change orgin strz z  z   \t\n\\\\"));
    sat_str_deinit(org);
  }

  {
#define PI 3.14159623
    sat_str_t org = sat_str_init_set("");
    cur = sat_str_append_fmt(org, "[%s:%d]hello saturn, ptr: %s, %.2lf\n", "will_change_orgin2",
                             126, "hello world", PI);
    printf("%s\n", cur);
    EXPECT_TRUE(sat_str_cmp(cur, "[will_change_orgin2:126]hello saturn, ptr: hello world, 3.14\n"));
    sat_str_deinit(cur);
  }
}
