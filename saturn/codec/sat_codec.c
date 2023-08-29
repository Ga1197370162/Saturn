#include "sat_codec.h"

static sat_codec_get_conv_instance_t sat_codec_table_lookup(const char* protocol) {
  uint32_t i = 0;

  for (i = 0; i < ARRAY_SIZE(g_sat_codec_table); ++i) {
    if (strcmp(g_sat_codec_table[i].protocol, protocol) == 0) {
      return g_sat_codec_table[i].get_instance;
    }
  }

  return NULL;
}

sat_conv_t sat_codec_get_conv_instance(const char* protocol) {
  sat_conv_t conv = NULL;
  sat_codec_get_conv_instance_t get_instance = NULL;
  return_value_if_fail(protocol != NULL, NULL);

  get_instance = sat_codec_table_lookup(protocol);
  if (get_instance != NULL) {
    conv = get_instance();
  }

  return conv;
}