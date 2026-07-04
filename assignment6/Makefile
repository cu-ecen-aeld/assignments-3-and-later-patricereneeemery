TARGET = aesdsocket
SRCS = aesdsocket.c

all: $(TARGET)
default: all

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o
