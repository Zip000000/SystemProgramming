/*************************************************************************
	> File Name: 8.send_file_client.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月19日 星期三 09时53分16秒
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#define MAXBUF 1024
#define MAXFILENAME 50


int main() {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.2.90");
    serv_addr.sin_port = htons(8888);
    int n = 10;
    while (n--) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        close(sock);
    }
    sleep(1);
    printf("i : %d\n", 8888);
    printf("after connect\n");
    perror("connect");
    

    return 0;
}
