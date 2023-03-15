#include <stdio.h>

#include "memory_struct.h"

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  void *temp_memory = realloc(mem->memory, mem->size + realsize + 1);
  if (temp_memory == NULL) {
    printf("Not enough memory (realloc returned NULL)\n");
    return 0;
  }
  mem->memory = temp_memory;

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}