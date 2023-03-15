#include "cjson_utils.h"

cJSON *create_request_data(const char *model, const char *question) {
    cJSON *request_data = cJSON_CreateObject();
    cJSON_AddStringToObject(request_data, "model", model);

    cJSON *messages = cJSON_CreateArray();
    cJSON *message = cJSON_CreateObject();
    cJSON_AddStringToObject(message, "role", "user");
    cJSON_AddStringToObject(message, "content", question);
    cJSON_AddItemToArray(messages, message);

    cJSON_AddItemToObject(request_data, "messages", messages);
    return request_data;
}

char *print_request_data(cJSON *request_data) {
    return cJSON_Print(request_data);
}
