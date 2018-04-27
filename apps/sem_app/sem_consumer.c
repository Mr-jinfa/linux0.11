#include "head.h"

key_t sem_id;
char c_data[size_with] = {0};
int global_var=0;

int main(int argc, char *argv[])
{
    pthread_mutex_t m;
	pthread_mutex_init(&m, NULL);	
    #ifdef UBANTU
	sem_id = semget((key_t)sem_name, 2,IPC_CREAT|IPC_EXCL|0666);   
	 if(sem_id >= 0)
		fprintf(stderr,"sem init\n");

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
	init_sem(sem_id, empty, 1);
	init_sem(sem_id, full, 0);
    #endif
    FILE *share_file = fopen(fbuff, "w+");
	fseek(share_file,0,SEEK_SET);

	while(1)
	{
		bzero(c_data, size_with);
		/*读一个数*/
		fread(c_data, size_with, 1, share_file);
		if(c_data[0] !=NULL)
		{
			sem_p(sem_id, full);
			pthread_mutex_lock(&m);
			fprintf(stderr,"%d: %s\n",getpid(), c_data);
			fflush(stderr);
			pthread_mutex_unlock(&m);
			sem_v(sem_id, empty);
			/* quit */
			if(c_data[0] == 'q')
				break;
			global_var++;
			if(global_var %10 ==0)
				share_file = fopen(fbuff, "w+");
		}
	}
    fprintf(stderr, "\n%d - finished\n", getpid());
	fclose(share_file);
    return 0;
}    

