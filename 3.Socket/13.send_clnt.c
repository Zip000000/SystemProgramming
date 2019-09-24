/*************************************************************************
	> File Name: 13.send_clnt.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年09月24日 星期二 12时06分02秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <error.h>

#define IPSTR "127.0.0.1"
#define PORT 8888

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(IPSTR);
    addr.sin_port = htons(PORT);
    printf("before connect\n");
    int conn_ret = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (conn_ret == -1) { perror("connect"); exit(1); }
    printf("after connect\n");
    char buff[1024] = "Hello_I_am_Zip";
    while (1) {
        printf("befor send\n");
        int send_ret = send(sock, buff, sizeof(buff), 0);
        printf("after send\n");
        sleep(5);
    }


    return 0;
}
