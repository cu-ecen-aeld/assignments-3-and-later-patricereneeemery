CC ?= $(CROSS_COMPILE)gcc
CFLAGS ?= -Wall -Werror -g -pthread
LDFLAGS += -Wl,--hash-style=gnu

TARGET = aesdsocket
SRCS = aesdsocket.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
