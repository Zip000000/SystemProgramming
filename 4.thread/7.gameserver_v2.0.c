/*************************************************************************
	> File Name: 7.gameserver_v2.0.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月22日 星期六 16时42分58秒
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

int get_serv_sock() {
    int serv_sock;
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr("192.168.2.90");
    sock_addr.sin_port = htons(1234);
    bind(serv_sock, (struct sockaddr* )&sock_addr, sizeof(sock_addr));
    return serv_sock;
}

void *sendpos(void *arg) {
    int clnt_sock;
    clnt_sock = *(int *)arg;
    while(1){
        //printf("%d %d \n", bpos.row, bpos.col);
        send(clnt_sock, &bpos, sizeof(bpos), 0);
        usleep(10000);
    }
    return NULL;
}

void* recv_opt(void* arg) {
    int clnt_sock;
    clnt_sock = *(int *)arg;
    char ch;
	while(1) {
        recv(clnt_sock, &ch, sizeof(ch), 0);
        switch(ch) {
            case 'a': 
                if(bpos.col > LEFTEDGE) bpos.col -= 1; break;
            case 'd': 
                if(bpos.col < RIGHTEDGE) bpos.col += 1; break;
            case 's': 
                if(bpos.row < DOWNEDGE) bpos.row += 1; break;
            case 'w': 
                if(bpos.row > UPEDGE) bpos.row -= 1; break;
        }
        //printf("你输入的是 %c\n", ch);
        //send(clnt_sock, &bpos, sizeof(bpos), 0);
    }
}

int clear_row;
int clear_col;
char ball[2] = "o";
char blank[2] = " ";
void draw(struct my_ballpos* bpos) {
    move(bpos->row, bpos->col);
    addstr(ball);
    
    move(LINES - 1, COLS - 1);
    refresh();
    //sleep(1);

    move(clear_row, clear_col);
    addstr(blank);
    
    clear_row = bpos->row;
    clear_col = bpos->col;
}
void makescreen() {
    initscr();
    clear();
    int x = LEFTEDGE - 1;
    int y = UPEDGE - 1;
    refresh();
    for(int i = LEFTEDGE - 1; i <= RIGHTEDGE + 1; i++) {
        move(UPEDGE - 1, i);
        addstr("*");
        move(DOWNEDGE + 1, i);
        addstr("*");
    }
    for(int j = UPEDGE - 1; j <= DOWNEDGE + 1; j++) {
        move(j, LEFTEDGE - 1);
        addstr("*");
        move(j, RIGHTEDGE + 1);
        addstr("*");
    }
    refresh();
}


int main(){
	int serv_sock = get_serv_sock();
    listen(serv_sock, 100);
    //printf("正在监听......\n");

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock;
	int id_cnt = 0;
    char ball[2] = "o";
    char blank[2] = " ";
    bpos.row = UPEDGE;
    bpos.col = LEFTEDGE;
    int dircol = 1;
    int dirrow = 1;
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    //printf("第 %d 次连接：\n", id_cnt);
    //printf("连接成功，准备发送坐标包......\n");
	pthread_t pthread_id[10];
    pthread_create(&pthread_id[0], NULL, sendpos, &clnt_sock);
    //printf("sendpos OK!\n");
    pthread_create(&pthread_id[1], NULL, recv_opt, &clnt_sock);
    //printf("recv_opt OK!\b");

    makescreen();

    while(1) {
        draw(&bpos);
    }

    pthread_join(pthread_id[1], NULL);
    close(serv_sock);

	return 0;
}

