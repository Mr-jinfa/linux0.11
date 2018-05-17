/*create by jinfa
 *#实现功能:
 在Ubuntu下编写程序，用信号量解决生产者——消费者问题；
 在0.11中实现信号量，用生产者—消费者程序检验之。
 *
 *#题目要求:
 建立一个生产者进程，N个消费者进程（N>1）；
 用文件/共享内存建立一个共享缓冲区；
 生产者进程依次向缓冲区写入整数0,1,2,...,M，M>=500；
 消费者进程从缓冲区读数，每次读一个，并将读出的数字从缓冲区删除，然后将本进程ID和+ 数字输出到标准输出；
 缓冲区同时最多只能保存10个数。
 *
 *log:
 *2018.4.24 最多支持size_with位数,也就说程序不支持动态调整位数.
 *2018.4.30 完全实现ubantu下的生产者功能
 *2018.5.18 实现ubantu下的共享内存生产者功能
 *2018.5.18 实现linux0.11下的共享内存生产者功能
 *
*/

#include "head.h"
#ifdef UBANTU
key_t sem_id;
#endif

char c_data[size_with] = {NULL};
int global_var=0;
#define SPACE 0
#define DATA  1

#ifndef UBANTU
_syscall2(sem_t*, sem_open, const char *, name, unsigned int, value)
_syscall1(int, sem_wait, sem_t*, sem)
_syscall1(int, sem_post, sem_t*, sem)
_syscall1(int, sem_unlink, const char *, name)

/*shm*/
_syscall2(int, shmget, unsigned int, key, unsigned int, size)
_syscall1(void *, shmat, int, shmid)

#endif

int get_bit(int value)
{
	char __count=1;
	/* 回调思想 */
	while(1)
	{
		if(!( value /10))
			return __count;
		__count ++;
		value /=10;
	}
}

int main(int argc, char *argv[])
{
    char index = 1, width=0,circul=0;
	char c_q[size_with]={NULL};
	struct share_subclass subc;
	int var=0; int i=0; int share_file=0;
	char k=0; char n; char m; char c;
	int shm_id;
	char *shmaddr = NULL;
#ifdef UBANTU
    sem_id = semget((key_t)sem_name, 2,IPC_CREAT|IPC_EXCL|0666);
    if(sem_id >= 0)
	{
	   fprintf(stderr,"sem init\n");
	   init_sem(sem_id, empty, 10);
	   init_sem(sem_id, full, 0);
	}
	else if(sem_id == -1 && errno == EEXIST)
	{
		fprintf(stderr,"sem have create\n");
		sem_id = semget((key_t)sem_name, 2,0666);
		init_sem(sem_id, empty, 10);
	   	init_sem(sem_id, full, 0);
	}
	else if(sem_id == -1)
	{
		perror("semget() failed");
		exit(0);
	}

	if (condition_share == mshare)
	{
		/* 创建或者打开key指定的IPC对象 */
		key_t key = ftok(PROJ_PATH, PROJ_ID);
		int id = shmget(key, 32, IPC_CREAT|0666);
		if(id == -1)
		{
			perror("shmget() failed");
			exit(0);
		}

		/* 将共享内存映射到本进程的内存空间中 */
		shmaddr = shmat(id, NULL, 0);
		if(shmaddr == (void*)-1)
		{
			perror("shmat() failed");
			exit(0);
		}
	}
#else
	sem_t *s_empty, *s_full;
	s_empty = sem_open(sem_empty, 10) ;
	s_full = sem_open(sem_full, 0);
	if(s_empty==NULL || s_full==NULL)
	{
		fprintf(stderr,"get sem fail\n");
		return -1;
	}
	if (condition_share == mshare)
	{
		shm_id = shmget(PROJ_ID, 4096);
		shmaddr = shmat(shm_id);
	}
#endif

if (condition_share == fshare)
    share_file = open(fbuff, O_RDWR|O_CREAT|O_TRUNC, 0666);
    while(circul < count)
    {
		width = get_bit(global_var);
		for(index=0; index< 10; index++)
		{
#ifdef UBANTU
			sem_p(sem_id, empty);
#else
			sem_wait(s_empty);
#endif
			c_data[0] = '0' + index;	/*个位*/
			i=1;
			while(i <width){	/* 填充后面若干位*/
				c_data[i] = ( global_var/(int)pow(10, width-1) )%(int)pow(10, width) +'0';
				i++;
			}
			/*倒序存储*/
			if(width %2){
				k = (width+1) /2; /*c_data[k-1]为中间值*/
				m = 0; n=width-1;
				while( m<= k -2){
					c = c_data[n];
					c_data[n] = c_data[m];
					c_data[m] = c;
					n--; m++;
				}
			}
			else{
				m = 0; n=width-1;
				while(n> m){
					c = c_data[n];
					c_data[n] = c_data[m];
					c_data[m] = c;
					n--; m++;
				}
			}
if (condition_share == fshare)
			write(share_file, c_data, size_with);
if (condition_share == mshare)
			memcpy(shmaddr, c_data, size_with);
//			fprintf(stderr,"%d:write:%s	\n",getpid(), c_data);
#ifdef UBANTU
			sem_v(sem_id, full);
#else
			sem_post(s_full);
#endif
			sleep(1);
			global_var ++;
		}
		circul++;
    }
	c_q[0] = 'q';
	fprintf(stderr,"%d:write:%s	\n",getpid(), c_q);
#ifdef UBANTU
	sem_p(sem_id, empty);
if (condition_share == fshare)
    write(share_file, &c_q[0], size_with);
if (condition_share == mshare)
			memcpy(shmaddr, c_q, size_with);

	sem_v(sem_id, full);

	sem_p(sem_id, empty);
if (condition_share == fshare)
	write(share_file, &c_q[0], size_with);
if (condition_share == mshare)
			memcpy(shmaddr, c_q, size_with);

	sem_v(sem_id, full);
#else
	sem_wait(s_empty);
if (condition_share == fshare)
	write(share_file, &c_q[0], size_with);
if (condition_share == mshare)
		memcpy(shmaddr, c_q, size_with);
	sem_post(s_full);

	sem_wait(s_empty);
if (condition_share == fshare)
	write(share_file, &c_q[0], size_with);
if (condition_share == mshare)
			memcpy(shmaddr, c_q, size_with);
	sem_post(s_full);
#endif
	fprintf(stderr, "\n%d - finished\n", getpid());

	close(share_file);
	
	return 0;
}

