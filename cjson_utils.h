#ifndef CJSON_UTILS_H
#define CJSON_UTILS_H

#include <cJSON.h>

cJSON *create_request_data(const char *model, const char *question);
char *print_request_data(cJSON *request_data);

#endif // CJSON_UTILS_H
