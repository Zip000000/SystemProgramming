/*************************************************************************
	> File Name: Homework.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年04月14日 星期日 19时28分19秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main()
{
    char hostname[32];
    char* name;
    char* path;
    name = getenv("LOGNAME");
    path = getenv("PWD");
    gethostname(hostname,sizeof(hostname));   //<unistd.h>
    printf("\033[;32m%s\033[0m",name);
    printf("\033[;31m@\033[0m");
    printf("\033[;33m%s\033[0m",hostname);
    printf("\033[;31m:\033[0m");
    printf("\033[;34m%s ",path);
    printf("\n");

    return 0;
}
