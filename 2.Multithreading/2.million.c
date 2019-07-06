/*************************************************************************
	> File Name: 2.million.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月04日 星期四 18时05分37秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <string.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    int my_id = 0;
    pid_t cpid = 1;
    
    for(int i = 0; i < 10; i++) {
        if(cpid != 0) {  //主
            my_id++;
            cpid = fork();
        }
        if(cpid == 0) {  //子
            break;
        }
    }

    if(cpid != 0) { //主
		long long sum = 0, allsum = 0;
		int serv_sock;
		serv_sock = socket(AF_INET, SOCK_STREAM, 0);
		
		struct sockaddr_in sock_addr;
		memset(&sock_addr, 0, sizeof(sock_addr));
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		sock_addr.sin_port = htons(8888);
		
        if( 0 != bind(serv_sock, (struct sockaddr* )&sock_addr, sizeof(sock_addr)) ) {
            perror("bind : ");
        }
		    listen(serv_sock, 100);
		    printf("listenning......\n");
        
        for(int i = 0; i < 10; i++) {
        //sleep(1);
		    struct sockaddr_in clnt_addr;
		    socklen_t clnt_addr_size = sizeof(clnt_addr);
		    int clnt_sock;

            clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
            printf("accept success!\n");
            sum = 0;
            recv(clnt_sock, &sum, sizeof(sum), 0);
            printf("sum[%d] = %lld\n", i, sum);
            allsum += sum;
            close(clnt_sock);
        }

        printf("allsum = %lld\n", allsum);
        close(serv_sock);
        return 0;
    }
    
    if(cpid == 0) {   //子
		 int sock = socket(AF_INET, SOCK_STREAM, 0);
	    struct sockaddr_in serv_addr;
	    memset(&serv_addr, 0, sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	    serv_addr.sin_port = htons(8888);
        printf("No. %d  ready to connect!\n", my_id);
        long long sum = 0;
        for(int j = 1 + (100000) * (my_id-1); j <= 100000 * my_id; j++) {
            sum += j;
        }
        int check = 1;
        int cnt = 1;
        while(check != 0) {
            //printf("No . %d is connecting %d times\n", my_id, cnt);
            check = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
            cnt++;
                       
        }
        printf("[No. %d] sum = %lld\n", my_id, sum);
        send(sock, &sum, sizeof(sum), 0);
        close(sock);
        return 0;
    }
    
}
