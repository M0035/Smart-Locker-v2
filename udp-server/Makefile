CC=gcc
CFLAGS=-Wall -g
OBJS=main.o server.o
TARGET=main

%.o:%.c server.h
	$(CC) $(CFLAGS) -c $< -o $@
$(TARGET):$(OBJS)
	$(CC) $^ -o $@ -lsqlite3

clean:
	rm -f $(OBJS) $(TARGET)
