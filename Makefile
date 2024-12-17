all: server client

server: Server.c
	gcc Server.c -o server

client: Client.c
	gcc Client.c -o client

testS:
	./server

testC:
	./client < in > out

clean:
	rm -f server client
	rm -f out