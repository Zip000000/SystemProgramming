/*************************************************************************
	> File Name: 9.gameserver_v3.0.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月22日 星期六 19时37分35秒
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

#define MAX_USER_NUM 20
#define LEFTEDGE 10
#define RIGHTEDGE 70
#define UPEDGE 2
#define DOWNEDGE 22
#define ROW 10

struct my_ballpos {
    int row;
    int col;
};

//struct my_ballpos bpos[MAX_USER_NUM];
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
struct all_user {
    struct sockaddr_in clnt_addr;
    int clnt_sock;
    int user_id;
	pthread_t pthread_id[5];
};

int main() {
    bpos.row = UPEDGE;
    bpos.col = LEFTEDGE;
   
    struct all_user user[MAX_USER_NUM];

	int serv_sock = get_serv_sock();
    int curr_user_num = 0;
    while(1) {
        curr_user_num += 1;

        listen(serv_sock, 100);
        printf("正在监听新玩家的加入......\n");

        socklen_t clnt_addr_size = sizeof(user[curr_user_num].clnt_addr);

        user[curr_user_num].clnt_sock = accept(serv_sock, (struct sockaddr*)&user[curr_user_num].clnt_addr, &clnt_addr_size);
        user[curr_user_num].user_id = curr_user_num;
        printf("第 %d 个玩家连接成功！\n", curr_user_num);
        pthread_create(&user[curr_user_num].pthread_id[0], NULL, sendpos, &user[curr_user_num].clnt_sock);
        printf("sendpos is Ready!\n");
        pthread_create(&user[curr_user_num].pthread_id[1], NULL, recv_opt, &user[curr_user_num].clnt_sock);
        printf("recv_opt is Ready!\b");
    }


	return 0;
}




/*
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
*/

