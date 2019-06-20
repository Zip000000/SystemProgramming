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


int main(int argc, char* argv[]) {
    if(argc == 1) printf("没有参数\n"), exit(1);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.43.214");
    serv_addr.sin_port = htons(8888);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    char buf[MAXBUF];
    char filename[MAXFILENAME];
    FILE *fp_from = NULL;
    strcpy(filename, argv[1]);
    if((fp_from = fopen(filename, "rb")) == NULL) {
        perror("fopen");
        exit(1);
    }
    int readcount;
    int checkfilename;
    send(sock, &filename, strlen(filename), 0);
    recv(sock, &checkfilename, 4, 0);
    //printf("filename send successful! return value = %d\n", checkfilename);
    //if(checkfilename == strlen(filename)) {
    //}
    while ((readcount = fread(buf, 1, MAXBUF, fp_from)) >= 0) {   
        //printf("buf = %s ; ", buf);
        //printf("size of buf =  %d ; ", (int)strlen(buf));
        //printf("readcount = %d\n", readcount);
        if(readcount == 0) {
            send(sock, &buf, 0, 0); 
            break;
        }
        send(sock, &buf, readcount, 0);
        memset(buf, 0, sizeof(buf));
        
    }

    printf("sending successful!\n");
    
    
    
    
    close(sock);

    return 0;
}
