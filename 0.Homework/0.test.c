/*************************************************************************
	> File Name: test.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年04月27日 星期六 19时13分40秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<termios.h>
int main()
{
    struct winsize size;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&size);   //取得宽度  需要再研究一下
    printf("col = %d\n",size.ws_col);   //宽度
    //printf("row = %d\n",size.ws_row);
    
    printf("\n");
    for (int i = 0; i < size.ws_col; i++ ) {
        printf("a");
    }
    printf("\n");
    for (int i = 0; i < size.ws_col; i++ ) {
        printf("b");
    }
    printf("\n");



    

    return 0;
}
