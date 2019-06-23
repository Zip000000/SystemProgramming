/*************************************************************************
	> File Name: 12.game_v4.0.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月23日 星期日 13时44分30秒
 ************************************************************************/
#include<stdio.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_USER_NUM 20
#define LEFTEDGE 10
#define RIGHTEDGE 150
#define UPEDGE 2
#define DOWNEDGE 37

#define UD_MID (UPEDGE/2+DOWNEDGE/2)
#define LR_MID (LEFTEDGE/2+RIGHTEDGE/2)
#define DOORWIDTH 12
#define DOORDEEPTH 8
int clear_row;
int clear_col;
char ball[2] = "O";
char people[2] = "R";
char blank[2] = " ";

struct my_ballpos {
    int row;
    int col;
};

struct my_peoplepos {
    int row;
    int col;
};

struct my_info {
    struct my_ballpos bpos;
    struct my_peoplepos ppos[MAX_USER_NUM];
    int user_num;
};

struct my_info info;
struct my_info lastinfo;
/*
void draw_heng(int l, int r) {
    if(l > r) {
        l = l * r;
        r = l / r;
        l = l / r;
    }
    for(int i = l; i <= r; i++) {
        move()
    }

}
void draw_shu(int u, int d) {

}
*/
void makedoor() {
    for(int i = LEFTEDGE + 2; i <= LEFTEDGE + 2 + DOORDEEPTH ; i++) {
        move(UD_MID + DOORWIDTH / 2, i);
        addstr("<");
        move(UD_MID - DOORWIDTH / 2, i);
        addstr("<");
    }
    for(int j = UD_MID - DOORWIDTH / 2; j <= UD_MID + DOORWIDTH / 2; j++) {
        move(j, LEFTEDGE +2);
        addstr("<");
    }
    for(int i = RIGHTEDGE - 2; i >= RIGHTEDGE - 2 - DOORDEEPTH ; i--) {
        move(UD_MID + DOORWIDTH / 2, i);
        addstr(">");
        move(UD_MID - DOORWIDTH / 2, i);
        addstr(">");
    }
    for(int j = UD_MID - DOORWIDTH / 2; j <= UD_MID + DOORWIDTH / 2; j++) {
        move(j, RIGHTEDGE - 2);
        addstr(">");
    }
    
    
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
        move(UPEDGE - 2, i);
        addstr("-");
        move(DOWNEDGE + 2, i);
        addstr("-");
    }
    for(int j = UPEDGE - 1; j <= DOWNEDGE + 1; j++) {
        move(j, LEFTEDGE - 1);
        addstr("|");
        move(j, RIGHTEDGE + 1);
        addstr("|");
    }
    for(int j = UPEDGE - 1; j <= DOWNEDGE + 1; j++) {
        move(j, LEFTEDGE / 2 + RIGHTEDGE / 2);
        addstr(".");
    }
    
    makedoor();

    refresh();
}
void draw() {
    for(int i = 1; i <= info.user_num; i++) {
        move(info.ppos[i].row, info.ppos[i].col);
        //printf("row = %d col = %d\n", info.ppos[i].row, info.ppos[i].col);
        addstr(people);
    }
    move(info.bpos.row, info.bpos.col);
    addstr(ball);
    
    move(LINES - 1, COLS - 1);
    refresh();
    //sleep(1);
    
    for(int i = 1; i <= info.user_num; i++) {
        move(lastinfo.ppos[i].row, lastinfo.ppos[i].col);
        addstr(blank);
    }
    
    move(lastinfo.bpos.row, lastinfo.bpos.col);
    addstr(blank);

    for(int i = 1; i <= info.user_num; i++) {
        lastinfo.ppos[i].row = info.ppos[i].row;
        lastinfo.ppos[i].col = info.ppos[i].col;
    }
    lastinfo.bpos.row = info.bpos.row;
    lastinfo.bpos.col = info.bpos.col;
}

void* pos_update(void* arg) {
    int sock = *(int *)arg;
    while(1) {
        //memset(&info, 0, sizeof(info));
        recv(sock, &info, sizeof(info), 0);
        //printf("row = %d col = %d\n", info.ppos[1].row, info.ppos[1].col);
        usleep(20000);
    }
}

void* send_opt(void* arg) {
    int sock = *(int *)arg;
    char ch;
    while(ch = getchar()) {
        //printf("%c\n", ch);
        send(sock, &ch, sizeof(ch), 0);
    }
}
struct name_and_camp{
    char name[4];
    int camp;
};
int main(int argc, char* argv[]) {
    if(argc != 5) printf("正确输入格式：/game IP Port 昵称 队号[1 / 2]\n"), exit(1);


    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    
    serv_addr.sin_port = htons(atoi(argv[2]));
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    struct name_and_camp nNc;
    strcpy(nNc.name, argv[3]);
    nNc.camp = atoi(argv[4]);
    send(sock, &nNc, sizeof(nNc), 0);
    char check_nNc = 'N';
    while(check_nNc == 'N') {
        recv(sock, &check_nNc, sizeof(nNc), 0);
        printf("等待对方确认收到姓名与阵营\n");
    }
    printf("对方已经收到！！\n");
    makescreen();
	pthread_t pthread_id[10];
    pthread_create(&pthread_id[0], NULL, pos_update, &sock);
    pthread_create(&pthread_id[1], NULL, send_opt, &sock);
    while(1) {
        draw();
    }
    
    pthread_join(pthread_id[1], NULL);
    endwin();
    return 0;
}
