#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cJSON.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "memory_struct.h"
#include "json_utils.h"
#include "api_key_utils.h"

int main(int argc, char *argv[]) {

  CURL *curl_handle;
  CURLcode res;
  struct MemoryStruct chunk;
  chunk.memory = malloc(1);
  chunk.size = 0;
  
  if (chunk.memory == NULL) {
    printf("Not enough memory (malloc returned NULL)\n");
    return 1;
  }

  char *api_key = NULL;

  // Check for the environment variable
  api_key = getenv("OPENAI_API_KEY");

  // Read from config file if the environment variable is not set
  if (!api_key) {
    char *config_file_path = "/etc/openai/chatgpt.config";
    struct stat st;
    if (stat(config_file_path, &st) == 0) {
      int file = open(config_file_path, O_RDONLY);
      if (file != -1) {
        api_key = (char *)malloc(st.st_size + 1);
        if (read(file, api_key, st.st_size) == st.st_size) {
          api_key[st.st_size] = '\0';
        } else {
          free(api_key);
          api_key = NULL;
        }
        close(file);
      }
    }
  }

  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init();

  if (curl_handle) {
    char question[256];

    // Check if a question was provided as a command-line argument
    if (argc > 1) {
      strncpy(question, argv[1], sizeof(question) - 1);
      question[sizeof(question) - 1] = '\0';
    } else {
      printf("Enter your question: ");
      if (fgets(question, sizeof(question), stdin) == NULL) {
        printf("Error reading input\n");
        free(chunk.memory);
        curl_global_cleanup();
        return 1;
      }
      question[strcspn(question, "\n")] = 0;
    }

    cJSON *request_data = cJSON_CreateObject();
    cJSON_AddStringToObject(request_data, "model", "gpt-3.5-turbo");

    cJSON *messages = cJSON_CreateArray();
    cJSON *message = cJSON_CreateObject();
    cJSON_AddStringToObject(message, "role", "user");
    cJSON_AddStringToObject(message, "content", question);
    cJSON_AddItemToArray(messages, message);

    cJSON_AddItemToObject(request_data, "messages", messages);
    char *request_body = cJSON_Print(request_data);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    char auth_header[128];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl_handle, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, request_body);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

    res = curl_easy_perform(curl_handle);

    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
      cJSON *response = cJSON_Parse(chunk.memory);
      // printf("Raw API response: %s\n", chunk.memory); // Commented out to hide the raw API response
      if (response) {
        cJSON *choices = extract_json_value(response, "choices");
        cJSON *first_choice = choices ? cJSON_GetArrayItem(choices, 0) : NULL;
        cJSON *text = first_choice ? extract_json_value(first_choice, "message") : NULL;
        cJSON *content = text ? extract_json_value(text, "content") : NULL;
        if (content) {
          printf("Question: %s\n", question);
          printf("Answer: %s\n", content->valuestring);
        }
        cJSON_Delete(response);
      } else {
        printf("Error: cJSON_Parse failed to parse response\n");
      }
    }

    cJSON_Delete(request_data);
    free(request_body);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
  }

  if (api_key != NULL && api_key != getenv("OPENAI_API_KEY")) {
    free(api_key);
  }

  curl_global_cleanup();
  return 0;
}