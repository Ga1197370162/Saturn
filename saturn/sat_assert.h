#ifndef __SAT_ASSERT_H__
#define __SAT_ASSERT_H__

#ifndef DEBUG
#define assert(p)                                                    \
  do {                                                               \
    if (!(p)) {                                                      \
      printf("%s:%d %s %s\n", __FILE__, __LINE__, __FUNCTION__, #p); \
    }                                                                \
  } while (0);

#define return_if_fail(p)                                            \
  do {                                                               \
    if ((p) != TRUE) {                                               \
      printf("%s:%d %s %s\n", __FILE__, __LINE__, __FUNCTION__, #p); \
      return;                                                        \
    }                                                                \
  } while (0);

#define return_value_if_fail(p, value)                               \
  do {                                                               \
    if ((p) != TRUE) {                                               \
      printf("%s:%d %s %s\n", __FILE__, __LINE__, __FUNCTION__, #p); \
      return (value);                                                \
    }                                                                \
  } while (0);

#define goto_error_if_fail(p)                                        \
  do {                                                               \
    if ((p) != TRUE) {                                               \
      printf("%s:%d %s %s\n", __FILE__, __LINE__, __FUNCTION__, #p); \
      goto error;                                                    \
    }                                                                \
  } while (0);
#endif /*DEBUG*/

#endif /*!__SAT_ASSERT_H__*/