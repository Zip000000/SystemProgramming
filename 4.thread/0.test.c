/*************************************************************************
	> File Name: 0.test.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月22日 星期六 14时53分11秒
 ************************************************************************/
#include<stdio.h>
#include <curses.h>
#include <stdbool.h>
bool judge() {
    char ch;
    ch = getchar();
    //printf("ch = %c", ch);
    if(ch =='y') return true;
    if(ch == 'n') return false;
}

int main() {
    printf("return the game (y/n)?");
    if(judge()) printf("\nyes  OK!\n");
    else printf("\nNo OK!\n");

    return 0;
}
