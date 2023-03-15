CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
CFLAGS += -I/usr/local/include/cjson/ -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lcurl -lcjson -lssl -lcrypto -lpthread -lz -ldl
LDFLAGS_STATIC = -static -L/usr/local/lib -lcurl -lcjson -lssl -lcrypto -lpthread -lz -ldl -lidn2 -lunistring
TARGET = chatgpt
PREFIX = /usr/local

all: $(TARGET)

static: LDFLAGS = $(LDFLAGS_STATIC)
static: $(TARGET)-static

$(TARGET): chatgpt.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

chatgpt.o: chatgpt.c
	$(CC) $(CFLAGS) -c -o $@ $<

install: $(TARGET)
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/

local: PREFIX = $(CURDIR)/bin
local: install

clean:
	rm -f $(TARGET) *.o