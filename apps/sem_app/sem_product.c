/*create by jinfa
 *log:
 *2018.4.24 最多支持size_with位数,也就说程序不支持动态调整位数.
 *
 *
*/

#include "head.h"


/* 定义共享文件中单个条目格式 */
struct share_subclass {
	char *data;//数据域:
};

static void del_semvalue();

char c_data[size_with] = {0};
int global_var=0;
#define SPACE 0
#define DATA  1
key_t sem_id;

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
void del_sem()    
{    
    //删除信号量
    union semun sem_union;    
    
    if(semctl(sem_id, 0, IPC_RMID, sem_union) == -1)    
        fprintf(stderr, "Failed to delete semaphore\n");   
    else  
        fprintf(stdout, "已经删除信号量\n");   
  
}

int main(int argc, char *argv[])    
{
    char index = 1, width=0,circul=0;
    union semun sem_union;
    sem_union.val = 1;  //一开始文件无数据,所以初始化为0
    #ifdef UBANTU
    
    sem_id = semget((key_t)sem_name, 2, IPC_CREAT|IPC_EXCL|0666);   
    if(sem_id >= 0)
	{fprintf(stderr,"-1\n");
		// 对信号量进行初始化
		init_sem(sem_id, SPACE, 1);
		init_sem(sem_id, DATA, 0);
	}
	else if(sem_id == -1 && errno == EEXIST)  //如果之前已经创建
	{fprintf(stderr,"0\n");
		sem_id = semget((key_t)sem_name, 2, 0666);
	}
	else if(sem_id == -1)
	{
		perror("semget() failed");
		exit(0);
	}
    #endif
    
    FILE *share_file = fopen(fbuff, "w");
	fseek(share_file,0,SEEK_SET);
	struct share_subclass subc;
	int var=0,i;	
	char c,k=0,n,m;
    while(circul <= 3)
    {
		//回卷
		fseek(share_file,0,SEEK_SET);
		for(index=0; index<10; index++)
		{
			sem_p(sem_id, SPACE);
	    	width = get_bit(global_var);
			subc.data = malloc(width);

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
			
			memcpy(subc.data, c_data, width);
			fwrite(subc.data, size_with, 1, share_file);	/*width+1方便观察*/
			fflush(share_file);
			fprintf(stderr,"写入:%s	\n", subc.data);
			sem_v(sem_id, DATA);
			usleep(500000);
			global_var ++;
		}
		
    	circul++;
//    	free(subc.data);
       
    }
    printf("\n%d - finished\n", getpid());     
	fclose(share_file);
// 	del_sem(); 
    return 0;
}    
   

