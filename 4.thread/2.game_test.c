/*************************************************************************
	> File Name: 2.game.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月22日 星期六 14时23分29秒
 ************************************************************************/

#include<stdio.h>
#include <curses.h>
#include <unistd.h>

int main() {
    initscr();
    clear();
    refresh();
    move(10, 20);
    sleep(1);
    addstr("Hello Zip!\n");
    refresh();
    sleep(1);
    move(10, 20);
    refresh();
    sleep(1);
    addstr("         !\n");
    refresh();
    sleep(1);

    move(20, 20);
    addstr("Hello Zip!\n");
    getch();
    endwin();

    return 0;
}
