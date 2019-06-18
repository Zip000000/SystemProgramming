/*************************************************************************
	> File Name: 2.check_server.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月04日 星期二 19时59分00秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>



int main() {
    int my_id = 1;
    pid_t cpid = 1;
    int serv_sock;
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr("192.168.2.90");
    sock_addr.sin_port = htons(8888);

    bind(serv_sock, (struct sockaddr* )&sock_addr, sizeof(sock_addr));
    listen(serv_sock, 100);
    printf("listenning......\n");

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock;
    while(1) {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        printf("accept success!\n");
        if(cpid != 0) {
            my_id++;
            cpid = fork();
        }
        if(cpid == 0) {
            break;
        }
    }

    if(cpid == 0) {
        char username[50];
        recv(clnt_sock, username, sizeof(username), 0);
        printf("PID = %d : ID : %d : username = %s\n", getpid(), my_id, username);
        //sleep(10000);
    }
    close(clnt_sock);
    close(serv_sock);
	return 0;

}

