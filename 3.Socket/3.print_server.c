/*************************************************************************
	> File Name: 3.print_server.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月04日 星期二 20时00分42秒
 ************************************************************************/

#include<stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>  //好几个

#include <unistd.h>  //write



int main() {
    
    int serv_sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.43.214");
    serv_addr.sin_port = htons(1234);
    //printf("port = %d\n", ntohs(serv_addr.sin_port));
    bind(serv_sock, (struct sockaddr* )&serv_addr, sizeof(serv_addr));
    printf("listen ready\n");

    listen(serv_sock, 20);

    printf("listenning......\n");

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr* )&clnt_addr, &clnt_addr_size);

    char str[] = "Hello! I am server. I received your message!";
    printf("write ready\n");
    write(clnt_sock, str, sizeof(str));
    printf("write over\n");

    close(clnt_sock);
    close(serv_sock);
    

    return 0;
}
