#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include "api_key_utils.h"

char *get_api_key(void) {
  char *api_key = NULL;

  // Check for the environment variable
  api_key = getenv("OPENAI_API_KEY");

  // Read from config file if the environment variable is not set
  if (!api_key) {
    char *config_file_path = "/etc/openai/chatgpt.config";
    struct stat st;
    if (stat(config_file_path, &st) == 0 && st.st_size <= MAX_API_KEY_LENGTH) {
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

  return api_key;
}