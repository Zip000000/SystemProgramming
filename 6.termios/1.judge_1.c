/*************************************************************************
	> File Name: 1.judge_1.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月16日 星期二 14时39分49秒
 ************************************************************************/

#include<stdio.h>
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
