GCC = gcc
FLAGS = --std=gnu99 -Wall  -Werror
all : server client

server: server.c
	$(GCC)  -o server $(FLAGS)  server.c

client: client.c
	$(GCC) -o client $(FLAGS) client.c
# use  -lsocket for solaris