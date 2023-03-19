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
#include "cjson_utils.h" 

void remove_leading_newlines(char *str) {
    char *src = str;
    char *dst = str;

    // Skip leading newline characters
    while (*src && *src == '\n') {
        src++;
    }

    // Copy the rest of the string
    while (*src) {
        *dst++ = *src++;
    }
    *dst = '\0';
}

void parse_config_file(const char *config_file_path, char **api_key, char **model) {
  FILE *file = fopen(config_file_path, "r");
  if (file != NULL) {
    char line[256];
    while (fgets(line, sizeof(line), file)) {
      char *key, *value;
      key = strtok(line, "=");
      value = strtok(NULL, "\n");

      if (strcmp(key, "API_KEY") == 0) {
        *api_key = strdup(value);
      } else if (strcmp(key, "MODEL") == 0) {
        *model = strdup(value);
      }
    }
    fclose(file);
  }
}

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
  char *model = NULL;

  // Check for the environment variable
  api_key = getenv("OPENAI_API_KEY");
  model = getenv("OPENAI_MODEL");

  // Read from config file if the environment variable is not set
  if (!api_key || !model) {
    char *config_file_path = "/etc/openai/chatgpt.config";
    parse_config_file(config_file_path, &api_key, &model);
  }

// Set default model if not provided through environment variable or config file
  if (!model) {
    model = strdup("gpt-3.5-turbo");
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

    cJSON *request_data = create_request_data(model, question);
    char *request_body = print_request_data(request_data);

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
    
    long http_response_code = 0;

    res = curl_easy_perform(curl_handle);
    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_response_code);

    if (res != CURLE_OK || http_response_code != 200) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        fprintf(stderr, "HTTP response code: %ld\n", http_response_code);
        fprintf(stderr, "Raw API response: %s\n", chunk.memory);
    } else {
      cJSON *response = cJSON_Parse(chunk.memory);
      // printf("Raw API response: %s\n", chunk.memory); // Commented out to hide the raw API response
      if (response) {
        cJSON *choices = extract_json_value(response, "choices");
        cJSON *first_choice = choices ? cJSON_GetArrayItem(choices, 0) : NULL;
        cJSON *text = first_choice ? extract_json_value(first_choice, "message") : NULL;
        cJSON *content = text ? extract_json_value(text, "content") : NULL;
        if (content) {
            remove_leading_newlines(content->valuestring);
            //printf("Question: %s\n", question); // Question output
            printf("%s\n", content->valuestring); // Answer output
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

  if (model) {
    free(model);
  }

  curl_global_cleanup();
  return 0;
}