all: server

server: server.c socket_utils.c socket_utils.h client_utils.c client_utils.h global.h message.c message.h command_utils.h command_utils.c
	gcc -o server -Wall server.c socket_utils.c client_utils.c command_utils.c message.c -lpthread
clean:
	rm server