/*************************************************************************
	> File Name: 2.judge_2.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月16日 星期二 14时40分24秒
 ************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

struct termios old_tm;
struct termios new_tm;

bool judge() {
    tcgetattr(0, &old_tm);
    tcgetattr(0, &new_tm);
    new_tm.c_lflag &= ~ICANON;
    new_tm.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &new_tm);
    char ch;
    while(1) {
        switch(ch = getchar()) {
            case 'Y':
            case 'y':return true;
            case EOF:
            case 'n':return false;
            default :continue;
        }
    }
}

int main() {
    printf("return the game (y/n)?");
    if(judge()) printf("\nyes  OK!\n");
    else printf("\nNo OK!\n");
    tcsetattr(0, TCSANOW, &old_tm);
    return 0;
}
