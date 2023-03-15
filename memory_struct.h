#ifndef MEMORY_STRUCT_H
#define MEMORY_STRUCT_H

#include <stdlib.h>
#include <string.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

#endif // MEMORY_STRUCT_H
