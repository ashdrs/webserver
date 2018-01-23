OBJS=src/main.o src/http.o src/socket.o src/server.o src/deamon.o src/log.o
SRCS=$(OBJS:src/%.o=src/%.c)
CFLAGS=-g -Wall
TARGET=bin/httpd

$(TARGET):$(OBJS)
	mkdir -p log/
	mkdir -p bin/
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

clean:
	-rm -f bin/httpd
	-rm -f src/main.o
	-rm -f src/http.o
	-rm -f src/socket.o
	-rm -f src/server.o
	-rm -f src/deamon.o
	-rm -f src/log.o
