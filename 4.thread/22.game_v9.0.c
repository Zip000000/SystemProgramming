/*************************************************************************
	> File Name: 16.game_v6.0.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月28日 星期五 20时53分33秒
 ************************************************************************/
#include<stdio.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/epoll.h>
#define EVENTS_MAX 5
#define MAX_USER_NUM 1000
#define LEFTEDGE 6
#define RIGHTEDGE 146
#define UPEDGE 2
#define DOWNEDGE 37

#define UD_MID (UPEDGE/2+DOWNEDGE/2)
#define LR_MID (LEFTEDGE/2+RIGHTEDGE/2)
#define DOORWIDTH 12
#define DOORDEEPTH 8

#define LDOORLEFT (LEFTEDGE+2)
#define LDOORRIGHT (LEFTEDGE+2+DOORDEEPTH)
#define RDOORRIGHT (RIGHTEDGE-2)
#define RDOORLEFT (RIGHTEDGE-2-DOORDEEPTH)
#define DOORUP (UD_MID - (DOORWIDTH/2))
#define DOORDOWN (UD_MID + (DOORWIDTH/2))

int clear_row;
int clear_col;
char ball[2] = "O";
char people[2] = "R";
char blank[2] = " ";

int my_userid = 0;

struct my_ballpos {
    int row;
    int col;
};

struct my_peoplepos {
    int row;
    int col;
};

struct name_and_camp{
    char name[10];
    int camp;
    int score;
};
struct my_score {
    struct name_and_camp nandc[MAX_USER_NUM];
    int score[2];
};

struct my_info {
    struct my_ballpos bpos;
    struct my_peoplepos ppos[MAX_USER_NUM];
    int user_num;
    struct my_score scr;
};

struct my_info info;
struct my_info lastinfo;

void makedoor() {
    for(int i = LDOORLEFT; i <= LDOORRIGHT ; i++) {
        move(DOORDOWN, i);
        addstr("<");
        move(DOORUP, i);
        addstr("<");
    }
    for(int j = DOORUP; j <= DOORDOWN; j++) {
        move(j, LEFTEDGE +2);
        addstr("<");
    }
    for(int i = RDOORRIGHT; i >= RDOORLEFT ; i--) {
        move(DOORUP, i);
        addstr(">");
        move(DOORDOWN, i);
        addstr(">");
    }
    for(int j = DOORUP; j <= DOORDOWN; j++) {
        move(j, RDOORRIGHT);
        addstr(">");
    }
}
void makemidline() {
    for(int j = UPEDGE - 1; j <= DOWNEDGE + 1; j++) {
        if(j == info.bpos.row && LR_MID == info.bpos.col) continue;
        int check_people_midline = 0;
        for(int i = 1; i <= info.user_num; i++){
            if(info.ppos[i].row == j && info.ppos[i].col == LR_MID) check_people_midline = 1;
        }
        if(check_people_midline) continue; //有人就不画
        move(j, LR_MID);
        addstr(".");
    }
}

#define SCORE_WID 36
#define SCORE_L (RIGHTEDGE + 4)
#define SCORE_R (SCORE_L + SCORE_WID)
#define SCORE_MID ((SCORE_L+SCORE_R)/2)

void makescore_edge() {
    for(int i = SCORE_L; i <= SCORE_R; i++) {
        move(UPEDGE - 1, i);
        addstr("*");
        move(DOWNEDGE + 1, i);
        addstr("*");
        move(UPEDGE - 2, i);
        addstr("-");
        move(DOWNEDGE + 2, i);
        addstr("-");
    }
    for(int j = UPEDGE - 1; j <= DOWNEDGE + 1; j++) {
        move(j, SCORE_L);
        addstr("*");
        move(j, SCORE_R);
        addstr("*");
        move(j, SCORE_MID);
        addstr(".");
    }
}

void makescore() {
    int c1_num = 0;
    int c2_num = 0;
    for(int i = 1; i <= info.user_num; i++) {
        if(info.scr.nandc[i].camp == 1) {
            move(UPEDGE + 5 + 2 * c1_num, SCORE_L + 2);
            addstr(info.scr.nandc[i].name);
            //move(UPEDGE + 4 + 2 * c1_num, (SCORE_L+SCORE_MID)/2 + 2);
            //addstr("---");
            move(UPEDGE + 5 + 2 * c1_num, (SCORE_L+SCORE_MID)/2 + 2);
            char tmp_score[5];
            sprintf(tmp_score, "%03d", info.scr.nandc[i].score);
            addstr(tmp_score);
            c1_num++;
        }
        if(info.scr.nandc[i].camp == 2) {
            move(UPEDGE + 5 + 2 * c2_num, SCORE_MID + 2);
            addstr(info.scr.nandc[i].name);
            move(UPEDGE + 5 + 2 * c2_num, (SCORE_R+SCORE_MID)/2 + 2);
            char tmp_score[5];
            sprintf(tmp_score, "%03d", info.scr.nandc[i].score);
            addstr(tmp_score);
            c2_num++;
        }
    }
    
    char tmp_score[5];
    move(UPEDGE + 2, SCORE_L + 2);
    addstr("TEAM 1:");
    move(UPEDGE + 2, (SCORE_L+SCORE_MID)/2 + 2);
    sprintf(tmp_score, "%03d", info.scr.score[0]);
    addstr(tmp_score);

    move(UPEDGE + 2, SCORE_MID + 2);
    addstr("TEAM 2:");
    move(UPEDGE + 2, (SCORE_R+SCORE_MID)/2 + 2);
    sprintf(tmp_score, "%03d", info.scr.score[1]);
    addstr(tmp_score);

}
void makescreen_edge() {
    for(int i = LEFTEDGE - 1; i <= RIGHTEDGE + 1; i++) {
        move(UPEDGE - 1, i);
        addstr("*");
        move(DOWNEDGE + 1, i);
        addstr("*");
        move(UPEDGE - 2, i);
        addstr("-");
        move(DOWNEDGE + 2, i);
        addstr("-");
    }
    for(int j = UPEDGE - 1; j <= DOWNEDGE + 1; j++) {
        move(j, LEFTEDGE - 1);
        addstr("|");
        move(j, RIGHTEDGE + 1);
        addstr("|");
    }

}
void makescreen() {
    initscr();
    clear();
    int x = LEFTEDGE - 1;
    int y = UPEDGE - 1;
    refresh();
    makescreen_edge();
    makemidline();
    makedoor();
    makescore_edge();
    refresh();
}
void draw() {
    for(int i = 1; i <= info.user_num; i++) {
        if(i == my_userid) standout();
        move(info.ppos[i].row, info.ppos[i].col);
        //printf("row = %d col = %d\n", info.ppos[i].row, info.ppos[i].col);
        addstr(people);
        if(i == my_userid) standend();
    }
    move(info.bpos.row, info.bpos.col);
    addstr(ball);
    makescreen_edge();
    makedoor();
    makemidline();
    makescore();
    move(LINES - 1, COLS - 1);
    refresh();
    //sleep(1);
    
    for(int i = 1; i <= info.user_num; i++) {
        lastinfo.ppos[i].row = info.ppos[i].row;
        lastinfo.ppos[i].col = info.ppos[i].col;
    }
    lastinfo.bpos.row = info.bpos.row;
    lastinfo.bpos.col = info.bpos.col;

    for(int i = 1; i <= info.user_num; i++) {
        move(lastinfo.ppos[i].row, lastinfo.ppos[i].col);
        addstr(blank);
    }
    move(lastinfo.bpos.row, lastinfo.bpos.col);
    addstr(blank);
}
int main(int argc, char* argv[]) {
    if(argc != 5) printf("正确输入格式：./game IP Port 昵称 队号[ 1 or 2 ]\n"), exit(1);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    printf("sock = %d\n", sock);
    if(sock == -1) {perror("socket"); exit(1);}
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    
    serv_addr.sin_port = htons(atoi(argv[2]));
    printf("befor connect sock = %d\n", sock);
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("connect"); exit(1);
    }
    printf("after connect sock = %d\n", sock);
    struct name_and_camp nNc;
    strcpy(nNc.name, argv[3]);
    nNc.camp = atoi(argv[4]);
    nNc.score = 0;
    printf("before send sock = %d\n", sock);
    send(sock, &nNc, sizeof(nNc), 0);
    printf("after send sock = %d\n", sock);
    printf("before recv sock = %d\n", sock);
    while(my_userid <= 0) {
        recv(sock, &my_userid, sizeof(my_userid), 0);
        printf("等待对方确认收到姓名与阵营, 并发送我的userID\n");
    }
    printf("after recv sock = %d\n", sock);
    printf("对方已经收到！！ sock = %d\n", sock);
    #ifndef DEFAULT
    makescreen();
    #endif
	
    struct epoll_event ev, events[EVENTS_MAX];
    int epollfd = epoll_create(EVENTS_MAX);
    if(epollfd == -1) {perror("epoll_create"); exit(1);}

    ev.events = EPOLLIN;
    ev.data.fd = 0;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, 0, &ev) == -1) {perror("epoll_ctl"); exit(1);}
    
    ev.events = (EPOLLIN | EPOLLHUP);
    ev.data.fd = sock;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev) == -1) {perror("first epoll_ctl_sock"); exit(1);}
    
    while(1) {
        //printf("正在epoll wait\n");
        int nfds = epoll_wait(epollfd, events, EVENTS_MAX, -1);
        if(nfds == -1) {perror("epoll_wait"); exit(1);}
        for(int i = 0; i < nfds; i++) {
            int now_fd = events[i].data.fd;
            
            if(now_fd == 0) {
                char ch;
                ch = getchar();
                send(sock, &ch, sizeof(ch), 0);
                
            } else if(events[i].events & EPOLLIN){
                int ret_recv = recv(sock, &info, sizeof(info), 0);
                //printf("recv = %d\n", ret_recv);
                #ifndef DEFAULT
                draw();
                #endif
                //printf("row = %d col = %d\n", info.ppos[1].row, info.ppos[1].col);
            } else if(events[i].events & EPOLLHUP) {
                printf("Game Over\n");
                close(sock);
                close (epollfd);
                endwin();
            }
        }
    }
    return 0;
}


    //pthread_t pthread_id[10];
    //pthread_create(&pthread_id[1], NULL, send_opt, &sock);
    //pthread_create(&pthread_id[2], NULL, draw_interface, NULL);
    //pthread_create(&pthread_id[0], NULL, pos_update, &sock);
    //pthread_create(&pthread_id[2], NULL, draw_all, &sock);
    //pthread_join(pthread_id[1], NULL);
/*
void *draw_all(void *arg) {
    while(1) {
        draw();
    }
}
*/
/*
void* pos_update(void* arg) {
    int sock = *(int *)arg;
    while(1) {
        //memset(&info, 0, sizeof(info));
        recv(sock, &info, sizeof(info), 0);
        //printf("row = %d col = %d\n", info.ppos[1].row, info.ppos[1].col);
        usleep(20000);
    }
}
void* send_opt(void* arg) {
    int sock = *(int *)arg;
    char ch;
    while(ch = getchar()) {
        //printf("%c\n", ch);
        send(sock, &ch, sizeof(ch), 0);
    }
}
*/
/*
void draw_heng(int l, int r) {
    if(l > r) {
        l = l * r;
        r = l / r;
        l = l / r;
    }
    for(int i = l; i <= r; i++) {
        move()
    }

}
void draw_shu(int u, int d) {

}
*/
