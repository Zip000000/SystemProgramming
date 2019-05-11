/*************************************************************************
	> File Name: 1.ls_su.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年05月11日 星期六 14时56分12秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>

int ls_opt = 0;

void do_ls (char* dirname) {
    printf("dirname = %s\n", dirname);
    
    
    
    
    

}

int main (int argc, char* argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "al")) != -1) {
        switch (opt) {
            case 'a': ls_opt+=1; break;
            case 'l': ls_opt+=2; break;
            default: exit(1);
        }
    }
    if (argc == 1) {
        do_ls(".");
    }
    
    for(int i = optind; i < argc; i++) {
        do_ls(argv[i]);
    }

    return 0;
}
