/*************************************************************************
	> File Name: 4.game.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月22日 星期六 14时50分07秒
 ************************************************************************/
#include<stdio.h>
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


int main() {
    makescreen();

    char ball[2] = "o";
    char blank[2] = " ";
    struct my_ballpos bpos;
    bpos.row = UPEDGE;
    bpos.col = LEFTEDGE;
    int dircol = 1;
    int dirrow = 1;
    while(1) {
        if(bpos.col >= RIGHTEDGE) dircol = -1;
        if(bpos.col <= LEFTEDGE) dircol = 1;
        if(bpos.row >= DOWNEDGE) dirrow = -1;
        if(bpos.row <= UPEDGE) dirrow = 1;
        move(bpos.row, bpos.col);
        addstr(ball);
        
        move(LINES - 1, COLS - 1);
        refresh();
        sleep(1);

        move(bpos.row, bpos.col);
        addstr(blank);
        bpos.col += dircol;
        if(bpos.col >= RIGHTEDGE) bpos.row += dirrow;
    }
    getch();
    endwin();
    return 0;
}
