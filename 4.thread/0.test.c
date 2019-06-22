/*************************************************************************
	> File Name: 0.test.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月22日 星期六 14时53分11秒
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

struct my_ballpos bpos;



int makescreen() {
    initscr();
    int x = LEFTEDGE;
    int y = UPEDGE;
    refresh();
    for(int i = LEFTEDGE; i <= RIGHTEDGE; i++) {
        move(UPEDGE, i);
        addstr("*");
        move(DOWNEDGE, i);
        addstr("*");
    }
    for(int j = UPEDGE; j <= DOWNEDGE; j++) {
        move(j, LEFTEDGE);
        addstr("*");
        move(j, RIGHTEDGE);
        addstr("*");
    }
    refresh();
}
void draw() {
    char ball[2] = "o";
    char blank[2] = " ";
    move(bpos.row, bpos.col);
    addstr(ball);
    
    move(LINES - 1, COLS - 1);
    refresh();

    move(bpos.row, bpos.col);
    addstr(blank);
}



int main() {

    makescreen();
    
	bpos.row = LEFTEDGE;
    bpos.col = RIGHTEDGE;
    
    char ch;
    while(1) {
        
        switch(ch = getchar()) {
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
        draw();
    }

    getch();
    endwin();
    return 0;
}
