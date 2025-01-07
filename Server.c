// Server side C program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char const* argv[])
{
    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = { 0 };

    // 創建Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    //設定 socket 選項(允許地址和port重複使用)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET; //指定伺服器使用 IPv4 
    address.sin_addr.s_addr = INADDR_ANY; // 指定伺服器可以綁定到所有可用的網路介面
    address.sin_port = htons(PORT);


    //將伺服器的 socket 與指定的address綁定。
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    //開始監聽
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    //接受連線(當有新的連線進來時，會為該連線創建一個新的 socket)
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  &addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    while (1) {//讀取指令，進行操作
        valread = read(new_socket, buffer, 1024 - 1);
        buffer[valread] = '\0';

        if (strncmp(buffer, "kill", 4) == 0) {
            break;
        }
        if (strncmp(buffer, "abs", 3) == 0){
            int a;
            sscanf(buffer, "abs %d", &a);
            if(a < 0) a = -a;
            sprintf(buffer, "%d\n", a);
            send(new_socket, buffer, strlen(buffer), 0);
        }else if (strncmp(buffer, "add", 3) == 0) {
            int a, b, result;
            sscanf(buffer, "add %d %d", &a, &b);
            result = a + b;

            sprintf(buffer, "%d\n", result);
            send(new_socket, buffer, strlen(buffer), 0);
        }else if(strncmp(buffer, "mul", 3) == 0){
            int a, b, result;
            sscanf(buffer, "mul %d %d", &a, &b);
            result = a * b;

            sprintf(buffer, "%d\n", result);
            send(new_socket, buffer, strlen(buffer), 0);
        } else {
            sprintf(buffer, "Hello\n");
            send(new_socket, buffer, strlen(buffer), 0);
        }
    }


    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    close(server_fd);
    return 0;
}

