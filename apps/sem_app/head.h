#ifndef __HEAD_H
#define __HEAD_H
#define __LIBRARY__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/sem.h>

/* 定义共享文件中单个条目格式 */
struct share_subclass {
	char *data;/*数据域:*/
};
/*#define UBANTU*/
#undef UBANTU
#ifndef UBANTU
/*这个在linux0.11使用*/
#define sem_full "666"
#define sem_empty "667"


#else
/*这个在ubantu使用*/
#include <sys/ipc.h>
#include <sys/shm.h>

#define sem_name 668
union semun  
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};
extern 	key_t sem_id;
#endif

#define empty 0
#define full  1
#define fbuff  "sem_buff_1.txt"    /*用文件建立一个共享缓冲区*/
#define size_with	4
#define count 3
#endif
