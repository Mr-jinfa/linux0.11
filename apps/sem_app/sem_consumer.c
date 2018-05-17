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
 *2018.4.30 完全实现ubantu下的消费者功能
 *2018.4.30 完全实现linux0.11下的消费者功能
 *2018.5.18 实现ubantu下的共享内存消费者功能
*/

#include "head.h"
#ifdef UBANTU
key_t sem_id;
#endif
int global_var=0;

#ifndef UBANTU
/*sem*/
_syscall2(sem_t*, sem_open, const char *, name, unsigned int, value)
_syscall1(int, sem_wait, sem_t*, sem)
_syscall1(int, sem_post, sem_t*, sem)
_syscall1(int, sem_unlink, const char *, name)
/*lock*/
_syscall1(lock_t*, lock_init, const char *, name)
_syscall1(void, mutex_lock, lock_t*, lkh)
_syscall1(void, mutex_unlock, lock_t*, lkh)
_syscall1(int, lock_del, const char *, name)

/*shm*/
_syscall2(int, shmget, unsigned int, key, unsigned int, size)
_syscall1(void *, shmat, int, shmid)
#endif

int main(int argc, char *argv[])
{
	char c_data[size_with] = {NULL};
	int share_file=0;
	pid_t pid=0;
	int *wait_stat;
	
#ifdef UBANTU
    pthread_mutex_t m;
	pthread_mutex_init(&m, NULL);
	char *shmaddr = NULL;

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
	lock_t *m;
	s_empty = sem_open(sem_empty, 10);
	s_full = sem_open(sem_full, 0);
	if(s_empty==NULL || s_full==NULL)
	{
		fprintf(stderr,"get sem fail\n");
		return -1;
	}

	m = lock_init(m_lock);
	if(m == NULL)
	{
		fprintf(stderr,"get lock fail\n");
		return -1;
	}
#endif
	if (condition_share == fshare)
		share_file = open(fbuff, O_RDONLY ,0666);

	pid = fork();
	fprintf(stderr,"\t I am:%d\n",getpid());
	while(1)
	{
#ifdef UBANTU
		sem_p(sem_id, full);
		pthread_mutex_lock(&m);
		if (condition_share == mshare)
			memcpy(c_data, shmaddr, size_with);
#else
		sem_wait(s_full);
		mutex_lock(m);
#endif
		/*读1个数*/
		if (condition_share == fshare)
			read(share_file, c_data, size_with);
		if(c_data[0] !=NULL)
			fprintf(stderr,"%d: %s\n",getpid(), c_data);
#ifdef UBANTU
			pthread_mutex_unlock(&m);
			sem_v(sem_id, empty);
#else
			mutex_unlock(m);
			sem_post(s_empty);
#endif
		/* quit */
		if(c_data[0] == 'q')
			break;
	}
    fprintf(stderr, "\n%d - finished\n", getpid());
if (condition_share == fshare)
	close(share_file);
	wait(wait_stat);
#ifdef UBANTU
	del_sem();
if (condition_share == mshare)
	shmdt(shmaddr);  /* 解除映射 */
#else	
	sem_unlink(s_empty);
	sem_unlink(s_full);
	if(lock_del(m_lock) !=0)
		fprintf(stderr, "\n%d - close mutex fail\n", getpid());
#endif
	return 0;
}
