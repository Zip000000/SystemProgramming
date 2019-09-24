/*************************************************************************
	> File Name: 12.recv_server.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年09月24日 星期二 11时41分18秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <error.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#define IPSTR "127.0.0.1"
#define PORT 8888

int main() {
    int listensock = socket(AF_INET, SOCK_STREAM, 0);
    if (listensock == -1) { perror("socket"); exit(1); }
    
    unsigned long ul = 1;
    ioctl(listensock, FIONBIO, &ul);
    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IPSTR);
    serv_addr.sin_port = htons(PORT);
    printf("before bind\n");
    if (bind(listensock, (struct sockaddr* )&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind"); exit(1); 
    }
    
    printf("befor listen\n");
    listen(listensock, 20);
    printf("after listen\n");
    
    struct sockaddr clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    int clnt_addr_len = sizeof(clnt_addr);
    int maxfd = listensock;
    fd_set all_rfds;
    FD_SET(listensock, &all_rfds);
    while (1) {
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        fd_set rfds;
        FD_ZERO(&rfds);
        rfds = all_rfds;
        int sel_ret = select(maxfd + 1, &rfds, NULL, NULL, &timeout);
        if (sel_ret == -1) {
            perror("select");  exit(1);
        } else if (sel_ret == 0) {
            printf("select timeout!\n");
        } else {
            printf("select ret = %d\n", sel_ret);
            if (FD_ISSET(listensock, &rfds)) {
                printf("准备accept\n");
            }
        }
    }

    int clnt_sock = accept(listensock, &clnt_addr, &clnt_addr_len);
    if (clnt_sock == -1) {
        perror("accept"); exit(1);
    }
    printf("accept successful!!\n");

    
    while (1) {
        char buff[1024] = {0};
        printf("before recv\n");
        int recv_ret = recv(clnt_sock, buff, sizeof(buff), 0);
        printf("recv : %s recv_ret = %d\n", buff, recv_ret);
        sleep(1);
    }
    return 0;
}
