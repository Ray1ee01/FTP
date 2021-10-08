all: server

server: server.c socket_utils.c socket_utils.h
	gcc -o server -Wall server.c socket_utils.c
clean:
	rm server