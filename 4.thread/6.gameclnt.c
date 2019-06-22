/*************************************************************************
	> File Name: 4.game.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月22日 星期六 14时50分07秒
 ************************************************************************/
#include<stdio.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define LEFTEDGE 10
#define RIGHTEDGE 70
#define UPEDGE 2
#define DOWNEDGE 22
#define ROW 10

struct my_ballpos {
    int row;
    int col;
};

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
void draw(struct my_ballpos* bpos) {
    char ball[2] = "o";
    char blank[2] = " ";
    move(bpos->row, bpos->col);
    addstr(ball);
    
    move(LINES - 1, COLS - 1);
    refresh();
    sleep(1);

    move(bpos->row, bpos->col);
    addstr(blank);
}

int main() {
    makescreen();

    struct my_ballpos bpos;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.2.90");
    serv_addr.sin_port = htons(1234);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
   
	//pthread_t pthread_id;
    while(1) {
        memset(&bpos, 0, sizeof(bpos));
        recv(sock, &bpos, sizeof(bpos), 0);
        //printf("%d %d \n", bpos.row, bpos.col);
        draw(&bpos);
    }
    getch();
    endwin();
    return 0;
}
