/*************************************************************************
	> File Name: 3.game.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月22日 星期六 14时39分32秒
 ************************************************************************/

#include<stdio.h>
#include <curses.h>
#include <unistd.h>

#define LEFTEDGE 10
#define RIGHTEDGE 30
#define ROW 10



int main() {
    char ball[2] = "o";
    char blank[2] = " ";

    int dir = 1;

    int pos = LEFTEDGE;

    initscr();
    clear();
    refresh();

    while(1) {
        move(ROW, pos);
        addstr(ball);
        refresh();
        move(LINES - 1, COLS - 1);
        refresh();
        sleep(1);
        move(ROW, pos);
        addstr(blank);
        pos += dir;

        if(pos >= RIGHTEDGE) dir = -1;
        if(pos <= LEFTEDGE) dir = 1;

    }



    return 0;
}
