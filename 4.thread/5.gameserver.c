/*************************************************************************
	> File Name: 5.gameserver.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月22日 星期六 15时31分31秒
 ************************************************************************/


#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <pthread.h>

#include <curses.h>
#include <unistd.h>

#define LEFTEDGE 10
#define RIGHTEDGE 70
#define UPEDGE 2
#define DOWNEDGE 22
#define ROW 10

struct my_ballpos {
    int row;
    int col;
};

struct my_ballpos bpos;

void *getpos(void *arg) {
    int clnt_sock;
    clnt_sock = *(int *)arg;

    printf("%d %d \n", bpos.row, bpos.col);
    send(clnt_sock, &bpos, sizeof(bpos), 0);

    return NULL;
}


int main(){
	int serv_sock;
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr("192.168.2.90");
    sock_addr.sin_port = htons(1234);
    bind(serv_sock, (struct sockaddr* )&sock_addr, sizeof(sock_addr));
    listen(serv_sock, 100);
    printf("正在监听......\n");

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock;
    
	int id_cnt = 0;
	pthread_t pthread_id;

    char ball[2] = "o";
    char blank[2] = " ";
    bpos.row = UPEDGE;
    bpos.col = LEFTEDGE;
    int dircol = 1;
    int dirrow = 1;
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    printf("第 %d 次连接：\n", id_cnt);
    printf("连接成功，准备发送坐标包......\n");
    while(1) {
        if(bpos.col >= RIGHTEDGE) dircol = -1;
        if(bpos.col <= LEFTEDGE) dircol = 1;
        if(bpos.row >= DOWNEDGE) dirrow = -1;
        if(bpos.row <= UPEDGE) dirrow = 1;
        sleep(1);
        if(bpos.col >= RIGHTEDGE) bpos.row += dirrow;
        bpos.col += dircol;
        pthread_create(&pthread_id, NULL, getpos, &clnt_sock);

        id_cnt++;
    }

    close(serv_sock);


	return 0;
}

