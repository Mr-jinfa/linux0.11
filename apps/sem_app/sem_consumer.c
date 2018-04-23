#include "head.h"

key_t sem_id;
char c_data[size_with] = {0};


/* 定义共享文件中单个条目格式 */
struct share_subclass {
	char *data;//数据域:
};

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
    int index = 0,width=0;
    union semun sem_union;
    sem_union.val = 1;
    #ifdef UBANTU
	sem_id = semget((key_t)sem_name, 2, IPC_CREAT|IPC_EXCL|0666);   
	if(sem_id >= 0)
	{
		// 对信号量进行初始化
		init_sem(sem_id, SPACE, 0);
		init_sem(sem_id, DATA, 0);
	}
	else if(sem_id == -1 && errno == EEXIST)  //如果之前已经创建
	{
		sem_id = semget((key_t)sem_name, 2, 0666);
	}
	else if(sem_id == -1)
	{
		perror("semget() failed");
		exit(0);
	}
    #endif
    FILE *share_file = fopen(fbuff, "r");
    
	struct share_subclass subc;
	char c[1];
	while(1)
	{
		int i=0;
		for(i=0; i<9; i++)
		{
			sem_p(sem_id, DATA);			
			subc.data = malloc(width);
			fprintf(stderr,"%d: ", getpid());
			while(1)
			{
				fread(&c_data[size_with- width], 1, 1, share_file);	//倒序存储
				if(c_data[size_with- width] ==NULL){
					break;
				}
				width++;
			}
			
			fprintf(stderr,"%s", &c_data[size_with-width + 1]);
			width = 0;
//			sem_v(sem_id, SPACE);
			fprintf(stderr,"\n");
			usleep(500000);

		}
		
		//回卷
		fseek(share_file,0,SEEK_SET);
	}
       
    
    printf("\n%d - finished\n", getpid());     
	fclose(share_file);
	del_sem();    
   
    return 0;
}    
  

