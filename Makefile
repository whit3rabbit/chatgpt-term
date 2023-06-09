include config.mk

TARGET = chatgpt

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
	sudo mkdir -p /etc/openai
	sudo cp chatgpt.config.sample /etc/openai/chatgpt.config
	sudo chmod 600 /etc/openai/chatgpt.config
	@echo "Remember to update the API key in /etc/openai/chatgpt.config"

local: PREFIX = $(CURDIR)/bin
local: install

clean:
	rm -f $(TARGET) $(TARGET)-static *.o