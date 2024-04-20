TARGET = ws281x

CC = gcc
CFLAGS = -Wno-unused-parameter -Wall -Wextra -Werror -std=gnu17 -pedantic
LDFLAGS = -shared

ifndef PROD
CFLAGS += -g
else
CFLAGS += -O3 -march=native -mtune=native
LDLAGS += -flto=auto
endif

$(TARGET).so: $(TARGET).c
	$(CC) $< $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f $(TARGET).so

.PHONY: clean
