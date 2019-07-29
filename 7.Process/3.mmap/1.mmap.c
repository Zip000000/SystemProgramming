/*************************************************************************
	> File Name: 1.mmap.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月29日 星期一 19时30分39秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

int main() {
    
    FILE *fp = fopen("tmpfile", "w");
    if (fp == NULL) { perror("fopen"); exit(1); }
    
    char buf[] = "HelloHaizei!";
    int n = 5;
    while (n--) {
        int fwrite_ret = fwrite(buf, 1, strlen(buf), fp);
        if (fwrite_ret <= 0) { perror("fwrite"); exit(1); }
    }
    fclose(fp);

    fp = fopen("tmpfile", "r+");
    size_t len = 1024;
    char *share = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fp->_fileno, 0);
    
    printf("share = %s\n", share);
    
    strncpy(share, "XXXXx", 5);
    
    printf("aftermoushare = %s\n", share);

    fclose(fp);
    return 0;
}
