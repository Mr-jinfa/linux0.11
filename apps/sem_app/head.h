#ifndef __HEAD_H
#define __HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <signal.h>
union semun  
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

#define UBANTU
#ifndef UBANTU
struct sembuf{    //这个在Linux0.11使用
    short sem_num;
    short sem_op;
    short sem_flg; // 通常为SEM_UNDO,使操作系统跟踪信号，
                   // 并在进程没有释放该信号量而终止时，操作系统释放信号量
};
#else
#include <sys/sem.h>    //这个在ubantu使用
#endif


#define empty 0
#define full  1


extern 	key_t sem_id;

#define sem_name 668
#define fbuff  "sem_buff.txt"    //用文件建立一个共享缓冲区

#define size_with	4
extern 	key_t sem_id;

#endif
