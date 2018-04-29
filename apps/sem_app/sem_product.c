/*create by jinfa
 *log:
 *2018.4.24 最多支持size_with位数,也就说程序不支持动态调整位数.
 *
 *
*/

#include "head.h"
#ifdef UBANTU
key_t sem_id;
#endif

char c_data[size_with] = {0};
int global_var=0;
#define SPACE 0
#define DATA  1
_syscall2(sem_t*, sem_open, const char *, name, unsigned int, value)
_syscall1(int, sem_wait, sem_t*, sem)
_syscall1(int, sem_post, sem_t*, sem)
_syscall1(int, sem_unlink, const char *, name)

int get_bit(int bit)
{
	int __count=1;
	/* 回调思想 */
	while(1)
	{
		if(!( bit /10))
			return __count;
		__count ++;
		bit /=10;
	}
}

int main(int argc, char *argv[])
{
    char index = 1, width=0,circul=0;
	char c_q[size_with]={NULL};
	struct share_subclass subc;
	int var=0; int i=0; int share_file=0;
	char k=0; char n; char m; char c;
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
    
    share_file = open(fbuff, O_RDWR|O_CREAT|O_TRUNC|O_APPEND, 0666);
    while(circul < count)
    {
		/*回卷*/
		lseek(share_file,0,SEEK_SET);
		for(index=0; index< 10; index++)
		{
		
#ifdef UBANTU
			sem_p(sem_id, empty);
#else
			sem_wait(s_empty);
#endif
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
			/*size_with方便消费者观察*/
			write(share_file, subc.data, size_with);
			/*fprintf(stderr,"写入:%s	\n", subc.data);*/
#ifdef UBANTU
			sem_v(sem_id, full);
			usleep(250000);
#else
			sem_post(s_full);
			sleep(5);
#endif
			global_var ++;
		}
		circul++;
    	free(subc.data);
    }
	c_q[0] = 'q';
#ifdef UBANTU
	sem_p(sem_id, empty);
    write(share_file, &c_q[0], size_with);
	sem_v(sem_id, full);
#else
	sem_wait(s_empty);
	write(share_file, &c_q[0], size_with);
	sem_post(s_full);
#endif
#ifdef UBANTU
		sem_p(sem_id, empty);
		write(share_file, &c_q[0], size_with);
		sem_v(sem_id, full);
#else
		sem_wait(s_empty);
		write(share_file, &c_q[0], size_with);
		sem_post(s_full);
#endif
	close(share_file);
	
    return 0;
}
   

