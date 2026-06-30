CC ?= gcc
CFLAGS ?= -Wall -Werror -g

TARGET = aesdsocket
SRCS = aesdsocket.c

all: $(TARGET)
default: all

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
