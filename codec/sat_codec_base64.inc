#include "sat_codec.h"

static const char base64_encodetable[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz0123456789+/";

static const char base64_decodetable[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62,
    -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, 0,  -1, -1, -1, 0,
    1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
};

static ret_t sat_codec_encode_base64(const char* src, uint32_t length, sat_str_t* output);
static ret_t sat_codec_decode_base64(const char* src, uint32_t length, sat_str_t* output);

static sat_codec_interface_t base64_interface = {
    .encode = sat_codec_encode_base64,
    .decode = sat_codec_decode_base64,
};

sat_codec_interface_t* sat_codec_base64_get_interface(void) {
  return &base64_interface;
}

static ret_t sat_codec_encode_base64(const char* src, uint32_t length, sat_str_t* output) {
  int i, j;
  uint8_t counter = 0;
  sat_str_t bin = NULL;
  return_value_if_fail(src != NULL && output != NULL, RET_BAD_PARAM);

  bin = sat_str_init();
  return_value_if_fail(bin != NULL, RET_OOM);
  return_value_if_fail(sat_codec_str_to_bin(src, length, &bin) == RET_OK, RET_FAIL);

  for (i = 0, j = 0; i < sat_str_len(bin); ++i) {
    counter <<= 1;
    counter += (bin[i] - '0');

    if ((i + 1) % 6 == 0) {
      *output = sat_str_append_char(*output, base64_encodetable[counter]);
      j = j == 3 ? 0 : j + 1;
      counter = 0;
    }
  }
  sat_str_deinit(bin);

  if (i % 6 != 0) {
    while (i % 6 != 0) {
      i += 1;
      counter <<= 1;
    }
    j = j == 3 ? 0 : j + 1;
    *output = sat_str_append_char(*output, base64_encodetable[counter]);
  }

  while (j != 0) {
    *output = sat_str_append_char(*output, '=');
    j = j == 3 ? 0 : j + 1;
  }

  return RET_OK;
}

static ret_t sat_codec_decode_base64(const char* src, uint32_t length, sat_str_t* output) {
  int i;
  ret_t ret = RET_OK;
  sat_str_t bin = NULL;
  return_value_if_fail(src != NULL && output != NULL, RET_BAD_PARAM);

  sat_str_clean(*output);

  bin = sat_str_init();
  return_value_if_fail(bin != NULL, RET_OOM);

  for (i = 0; i < length && src[i] != '='; ++i) {
    int j = 0;
    char temp[7] = "000000";
    uint8_t counter = base64_decodetable[(uint8_t)src[i]];

    while (j < 6 && counter > 0) {
      temp[5 - j] = (counter % 2) + '0';
      j += 1;
      counter /= 2;
    }
    bin = sat_str_append(bin, temp);
  }

  ret = sat_codec_bin_to_str(bin, sat_str_len(bin), output);
  sat_str_deinit(bin);

  return ret;
}