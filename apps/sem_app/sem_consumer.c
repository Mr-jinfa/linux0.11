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
	
	fprintf(stderr,"-1\n");
		// 对信号量进行初始化
		init_sem(sem_id, SPACE, 1);
		init_sem(sem_id, DATA, 0);
	}
	else if(sem_id == -1 && errno == EEXIST)  //如果之前已经创建
	{
		fprintf(stderr,"0\n");
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
	
	int i=0,g_null=0;
	subc.data = malloc(size_with);
//	fprintf(stderr,"1\n");
	while(1)
	{
//		fprintf(stderr,"2\n");
		sem_p(sem_id, DATA);
		/*读一个数*/
		while(1)
		{
		
//		fprintf(stderr,"3\n");
			fread(&c_data[ width], 1, 1, share_file);
			
//			fprintf(stderr,"4\n");
			while(c_data[width] ==NULL){	/*将NULL读出*/
				width++;
				fread(&c_data[ width], 1, 1, share_file);
				g_null = 1;
				/*ubantu 文件缓冲区4K*/
				if(width >=2048){	/* 消费者先运行时,生产者写入数据慢(fflush)导致这里段错误 */
					g_null = 2;
					break;
				}
//				fprintf(stderr,"5 :%d\n", width);
			}
			if(g_null){
				g_null =0;
				if(g_null !=2)
					fseek(share_file,-1, SEEK_CUR);
				
//				fprintf(stderr,"6\n");
				break;
			}
			width++;
		}
		width =0;
		if(c_data[0] !=NULL){
			fprintf(stderr,"%d: ", getpid());
			fprintf(stderr,"%s	ftell:%d \n", c_data, ftell(share_file));
		}
		sem_v(sem_id, SPACE);
		usleep(500000);
		/*回卷*/
		if(ftell(share_file) == size_with*10 -1)			
			fseek(share_file,0,SEEK_SET);
			
	}		
       
    
    printf("\n%d - finished\n", getpid());     
	fclose(share_file);
	del_sem();    
   
    return 0;
}    
  

