/*************************************************************************
	> File Name: 7.save_file_server.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月18日 星期二 18时20分04秒
 ************************************************************************/
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

typedef struct savefile{
    char filename[50];
    char buf[100000];
    int len;
}ssave;

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
        printf("进入子程序\n");
        struct savefile ssave;
        recv(clnt_sock, &ssave, sizeof(ssave), 0);
        printf("recv success!\n");
        printf("what is in it \n %s\n", ssave.buf);
        printf("size is %d \n", ssave.len);
        FILE *fp_to;
        
        //if((fp_to = fopen(strcat(ssave.filename,"_from_server"), "w+")) == NULL) {
        if((fp_to = fopen(strcat(ssave.filename, "_FROME_SEND"), "w+")) == NULL) {
            perror("fopen");
            exit(1);
        }
        printf("what is in it \n %s\n", ssave.buf);

        if((fwrite(ssave.buf, 1, ssave.len, fp_to)) <=0) {
            perror("fwrite");
            exit(1);
        }
        fclose(fp_to);
        printf("PID = %d : ID : %d : filename = %s\n", getpid(), my_id, ssave.filename);
    }
    close(clnt_sock);
    close(serv_sock);
	return 0;

}

