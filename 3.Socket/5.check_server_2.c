/*************************************************************************
	> File Name: 5.check_server_2.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月18日 星期二 13时51分31秒
 ************************************************************************/

#include<stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd>

struct sockaddr_in* getsock_addr(struct sockaddr_in* sock_addr, char* ip, char* port) {
    memset(sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_familu = AD_INET;
    sock_addr.sin_addr.s_addr = inet_addr(ip);
    sock_addr.sin_port = htos(port);
    return sock_addr
}

int main() {
    char ip[] = "192.168.43.214";
    char port[] = "1234";
    


    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sock_addr;
    sock_addr=getsock_addr(&sock_addr, ip, port);

    bind(sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr));

    listen(sock, 20);

    struct sockaddr_in clnt_addr;
    
    int clnt_sock = accept(sock, ())
    
    
    
    

    return 0;
}
