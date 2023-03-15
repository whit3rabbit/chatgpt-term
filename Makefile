CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
CFLAGS += -I/usr/local/include/cjson/ -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lcurl -lcjson -lssl -lcrypto -lpthread -lz -ldl
LDFLAGS_STATIC = -L/usr/local/lib -Wl,-Bstatic -lcurl -lcjson -lssl -lcrypto -Wl,-Bdynamic -lpthread -lz -ldl -lidn2 -lunistring
TARGET = chatgpt
PREFIX = /usr/local

all: $(TARGET)

static: LDFLAGS = $(LDFLAGS_STATIC)
static: $(TARGET)-static

$(TARGET): chatgpt.o memory_struct.o json_utils.o api_key_utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET)-static: chatgpt.o memory_struct.o json_utils.o api_key_utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

chatgpt.o: chatgpt.c
	$(CC) $(CFLAGS) -c -o $@ $<

memory_struct.o: memory_struct.c
	$(CC) $(CFLAGS) -c -o $@ $<

json_utils.o: json_utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

api_key_utils.o: api_key_utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

install: $(TARGET)
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/

local: PREFIX = $(CURDIR)/bin
local: install

clean:
	rm -f $(TARGET) $(TARGET)-static *.o