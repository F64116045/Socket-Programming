all:
	gcc Client.c -o client
	gcc Server.c -o server

run_client: client
	./client