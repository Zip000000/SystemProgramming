/*************************************************************************
	> File Name: 6.send_file_client.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月18日 星期二 18时21分27秒
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
    typedef struct sendfile{
        char filename[50] ;
        char buf[100000];
        int len;
    }ssend;

int main(int argc, char* argv[]) {
    if(argc == 1) printf("没有参数\n"), exit(1);

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.2.90");
    serv_addr.sin_port = htons(8888);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    struct sendfile ssend;
    FILE *fp_from = NULL;
    strcpy(ssend.filename, argv[1]);
    if((fp_from = fopen(ssend.filename, "r")) == NULL) {
        perror("fopen");
        exit(1);
    }
    
    fseek(fp_from, 0L, SEEK_END);
    ssend.len = ftell(fp_from);
    printf("******ssend.len = %d \n", ssend.len);
    //ssend.buf = (char*)malloc(sizeof(char) * len);
    memset(ssend.buf, 0, ssend.len);
    rewind(fp_from);

    fread(ssend.buf, 1, ssend.len, fp_from);
    ssend.buf[ssend.len] = 0;
    printf("what is in it : %s \n", ssend.buf);
    printf("size is :  %d \n", ssend.len);

    //char buffer[] = "hello, i am sending file";    
    send(sock, &ssend, sizeof(ssend), 0);
    printf("send successful!\n");
    
    
    
    
    close(sock);

    return 0;
}
