# Compile server.c into an executable called server and client.c into an executable called client
make all: server client

clean:
	rm -f server client

# Path: server.c
server: server.c
	gcc -g -Wall -o server server.c -pthread

# Path: client.c
client: client.c
	gcc -g -Wall -o client client.c -pthread
