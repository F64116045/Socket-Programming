#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[]) {
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Socket 創建
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 設定伺服器地址
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // 連接伺服器
    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    sleep(3);
    // 從檔案讀取命令
    FILE *fp = fopen("in", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    char input[1024];
    while (fgets(input, sizeof(input), fp) != NULL) { // 從檔案逐行讀取
        send(client_fd, input, strlen(input), 0); // 發送給伺服器

        if (strncmp(input, "kill", 4) == 0) {
            break;
        }

        // 等待伺服器的回應
        valread = read(client_fd, buffer, sizeof(buffer) - 1);
        buffer[valread] = '\0';
        printf("%s", buffer);
    }

    fclose(fp);
    close(client_fd); 
    return 0;
}
