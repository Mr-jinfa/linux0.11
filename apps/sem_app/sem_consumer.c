#include "head.h"
#ifdef UBANTU
key_t sem_id;
#endif
int global_var=0;
_syscall2(sem_t*, sem_open, const char *, name, unsigned int, value)
_syscall1(int, sem_wait, sem_t*, sem)
_syscall1(int, sem_post, sem_t*, sem)
_syscall1(int, sem_unlink, const char *, name)

int main(int argc, char *argv[])
{
	char c_data[10*size_with] = {0};
	int share_file=0;
	pid_t pid=0;
	int *wait_stat;

#ifdef UBANTU
    pthread_mutex_t m;
	pthread_mutex_init(&m, NULL);	

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
#else
		sem_t *s_empty, *s_full;
		s_empty = sem_open(sem_empty, 10) ;
		s_full = sem_open(sem_full, 0);
		if(s_empty==NULL || s_full==NULL)
		{
			fprintf(stderr,"get sem fail\n");
			return -1;
		}
#endif

    share_file = open(fbuff, O_RDONLY|O_CREAT ,0666);
	pid = fork();
	fprintf(stderr,"\t I am:%d\n",getpid());
	while(1)
	{
		bzero(c_data, size_with);
		/*读1个数*/
		read(share_file, c_data, size_with);
		if(c_data[0] !=NULL)
		{
#ifdef UBANTU
			sem_p(sem_id, full);
			pthread_mutex_lock(&m);
#else
			sem_wait(s_full);
#endif
			fprintf(stderr,"%d: %s\n",getpid(), c_data);
			fflush(stderr);
#ifdef UBANTU
			pthread_mutex_unlock(&m);
			sem_v(sem_id, empty);
#else
			sem_post(s_empty);
#endif
			/* quit */
			if(c_data[0] == 'q')
				break;
		}
	}
    fprintf(stderr, "\n%d - finished\n", getpid());
	wait(wait_stat);
#ifdef UBANTU
	del_sem();
#else	
	sem_unlink(s_empty);
	sem_unlink(s_full);
#endif
	close(share_file);
    return 0;
}    

