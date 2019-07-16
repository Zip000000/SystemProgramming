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
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>

struct termios old_tm;
struct termios new_tm;

char my_getchar() {
    char ch;
    while((ch = getchar()) != EOF && strchr("yYnN", ch) == NULL);
    return ch;
}

void set_io_attr() {
    tcgetattr(0, &old_tm);
    tcgetattr(0, &new_tm);
    new_tm.c_lflag &= ~ICANON;
    new_tm.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &new_tm);
}

void return_io_attr() {
    tcsetattr(0, TCSANOW, &old_tm);
}

bool judge() {
    set_io_attr();
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    while(1) {
        char ch;
        struct timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        int ret = select(1, &rfds, NULL, NULL, &tv);
        if(ret == 0) return false;
        switch(ch = my_getchar()) {
            case 'Y':
            case 'y':return true;
            case 'N':
            case 'n':return false;
            default :continue;
        }
    }
    return false;
}

int main() {
    printf("return the game (y/n)?");
    fflush(stdout);
    fcntl(0, F_SETFL, fcntl(0, F_GETFL)|O_NONBLOCK);
    if(judge()) printf("\nyes  OK!\n"); else printf("\nNo OK!\n");
    return_io_attr(); 
    return 0;
}
