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
#include<string.h>
int main() {
    char* a = "/homg/zip/haizei/a.c";
    char* fname = strrchr(a, '/');
    printf("fname = %s\n", fname);
    printf("a = %c\n", a[strlen(a)-1]);
    return 0;
}
