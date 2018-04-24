#include "head.h"

key_t sem_id;
char c_data[size_with] = {0};

int get_bit(int bit)
{
	int count=1;
	/* 回调思想 */
	while(1)
	{
		if(!( bit /10))
			return count;
		count ++;
		bit /=10;
	}
}
void *inver_order(char *__dest, char *__src, int __n)
{
	int i;
	char *d = (char *)__dest, *s = (char *)__src;
	for (i = 0; i < __n; i++)
		d[i] = s[__n-i -1];

	return __dest;
}

int main(int argc, char *argv[])
{    

    int index = 0,width=0, g_null=0;
    pthread_mutex_t m;
	pthread_mutex_init(&m, NULL);
	
    #ifdef UBANTU
	sem_id = semget((key_t)sem_name, 2, IPC_CREAT|IPC_EXCL|0666);   
	if(sem_id >= 0)
	{
		fprintf(stderr,"sem init\n");
		init_sem(sem_id, SPACE, 1);
		init_sem(sem_id, DATA, 0);
	}
	else if(sem_id == -1 && errno == EEXIST)
	{
		fprintf(stderr,"sem have create\n");
		sem_id = semget((key_t)sem_name, 2, 0666);
	}
	else if(sem_id == -1)
	{
		perror("semget() failed");
		exit(0);
	}
    #endif
    FILE *share_file = fopen(fbuff, "r");
	fseek(share_file,0,SEEK_SET);
	
	while(1)
	{
		pthread_mutex_lock(&m);
		sem_p(sem_id, DATA);
		/*读一个数*/
		while(1)
		{
			fread(&c_data[ width], 1, 1, share_file);
			while(c_data[width] ==NULL){	/*将NULL读出*/
				width++;
				fread(&c_data[ width], 1, 1, share_file);
				g_null = 1;
				/*ubantu 文件缓冲区4K*/
				if(width >=2048){	/* 消费者先运行时,生产者写入数据慢(fflush)导致这里段错误 */
					g_null = 2;
					break;
				}
			}
			if(g_null){
				g_null =0;
				if(g_null !=2)
				fseek(share_file,-1, SEEK_CUR);
				break;
			}
			width++;
		}
		/*回卷*/
		if(ftell(share_file) == size_with*10 -1)
			fseek(share_file,0,SEEK_SET);
		/* quit */
		if(c_data[0] == 'q')
			break;

		if(c_data[0] !=NULL){
			fprintf(stderr,"%d: ",getpid());
			fprintf(stderr,"%s	ftell:%d \n", c_data, ftell(share_file));
		}
		width =0;
		sem_v(sem_id, SPACE);
		pthread_mutex_unlock(&m);
		usleep(500000);
	}
       
    
    printf("\n%d - finished\n", getpid());
	fclose(share_file);
//	del_sem();    
   
    return 0;
}    
  

