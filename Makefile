CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
CFLAGS += -I/usr/include -I/usr/include/cjson/
CFLAGS += -I/usr/local/include/cjson/ -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lcurl -lcjson -lssl -lcrypto -lpthread -lz -ldl
LDFLAGS_STATIC = -L/usr/local/lib -Wl,-Bstatic -lcurl -lcjson -lssl -lcrypto -Wl,-Bdynamic -lpthread -lz -ldl -lidn2 -l>TARGET = chatgpt
PREFIX = /usr

all: $(TARGET)

static: LDFLAGS = $(LDFLAGS_STATIC)
static: $(TARGET)-static

$(TARGET): chatgpt.o memory_struct.o json_utils.o api_key_utils.o cjson_utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET)-static: chatgpt.o memory_struct.o json_utils.o api_key_utils.o cjson_utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

chatgpt.o: chatgpt.c
	$(CC) $(CFLAGS) -c -o $@ $<

memory_struct.o: memory_struct.c
	$(CC) $(CFLAGS) -c -o $@ $<

cjson_utils.o: cjson_utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

json_utils.o: json_utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

api_key_utils.o: api_key_utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

install: $(TARGET)
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/
	mkdir -p /etc/openai
	cp chatgpt.config.sample /etc/openai/chatgpt.config
	chmod 600 /etc/openai/chatgpt.config
	@echo "Remember to update the API key in /etc/openai/chatgpt.config"

local: PREFIX = $(CURDIR)/bin
local: install

clean:
	rm -f $(TARGET) $(TARGET)-static *.o
