/*************************************************************************
	> File Name: 4.print_client.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月04日 星期二 20时01分13秒
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include <arpa/inet.h>

#include <unistd.h>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.43.214");
    serv_addr.sin_port = htons(1234);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    char buffer[100];
    read(sock, buffer, sizeof(buffer)-1);

    printf("Message from server: %s\n", buffer);

    close(sock);

    return 0;
}
