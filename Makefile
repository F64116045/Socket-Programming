all: server client

server: Server.c
	gcc Server.c -o server

client: Client.c
	gcc Client.c -o client

run: server client
	./server & ./client < in > out

test: out ans
	cat out
	diff -s out ans

clean:
	rm -f server client
	rm -f out