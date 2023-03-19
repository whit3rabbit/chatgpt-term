#include <stdio.h>

#include "json_utils.h"

cJSON *extract_json_value(cJSON *json, const char *key) {
  cJSON *value = cJSON_GetObjectItem(json, key);
  if (!value) {
    printf("Error: %s key not found in response\n", key);
  }
  return value;
}