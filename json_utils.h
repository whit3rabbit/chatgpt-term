#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <cJSON.h>

cJSON *extract_json_value(cJSON *json, const char *key);

#endif // JSON_UTILS_H