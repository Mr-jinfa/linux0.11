/*
 *  File name: linux0.11/kernel/sem.c
 *
 *  Author: 黄进发   blogs:https://blog.csdn.net/huang_165
 *
 *  Date: 2018-5
 *  
 *  Description: linux0.11信号量源码
 *
 *  GitHub: https://github.com/Mr-jinfa/   Bug Report: 2112201069@qq.com
*/


#include <sys/sem.h>
#include <linux/sched.h>
#include <unistd.h>
#include <asm/segment.h>
#include <linux/tty.h>
#include <linux/kernel.h>
#include <linux/fdreg.h>
#include <asm/system.h>
#include <asm/io.h>

sem_t semtable[SEMTABLE_LEN];	/* 信号量集合 */
int sem_cnt = 0;


/*功能:打开或创建一个信号量
 *name，信号量的名字,如果该信号量不存在，就创建新的名为name的信号量,如果存在，就打开。
 *value，信号量的初值，仅当新建信号量时，此参数才有效，其余情况下它被忽略。
 *返回值。当成功时，返回值是该信号量的唯一标识（比如，在内核的地址、ID等）。如失败，返回值是NULL。
*/
sem_t *sys_sem_open(const char *name, unsigned int value)
{
	char kernelname[SEM_NAME_LEN];
    int isExist = 0;
    int i=0;
    int name_cnt=0,name_len=0, sem_name_len=0;
	
	/*检查长度*/
    while( get_fs_byte(name+name_cnt) != '\0')
   		name_cnt++;
    if(name_cnt>SEM_NAME_LEN)
    	return NULL;

	/*获取信号量名字*/
    for(i=0;i<name_cnt;i++)
    	kernelname[i]=get_fs_byte(name+i);
	
    name_len = strlen(kernelname);
	/* 再次检查 */
	if(name_len != name_cnt)
		return NULL;

    sem_t *p=NULL;
    for(i=0;i<lock_cnt;i++)
    {
    	/*匹配信号量表*/
        sem_name_len = strlen(semtable[i].name);
        if(sem_name_len == name_len)
            if( !strcmp(kernelname,semtable[i].name) )
            {
                isExist = 1;
                break;
            }
    }
    if(isExist == 1)
        p=(sem_t*)(&semtable[i]);
    else
    {
        for(i=0;i<name_len;i++)
        {
            semtable[lock_cnt].name[i]=kernelname[i];
        }
        semtable[lock_cnt].value = value;
        p=(sem_t*)(&semtable[lock_cnt]);
        /*printk("creat sem!\n");*/
        lock_cnt++;
     }
    return p;
}

/*本函数参考ll_rw_blk.c lock_buffer()
 *功能:信号量P操作
 *成功返回0;失败返回负数码
*/
int sys_sem_wait(sem_t *sem)
{
	cli();	/* 关中断 */
	while (sem->value <=0)
		sleep_on(&sem->queue);	/* 如果锁上,那么该进程睡眠并调度 */
	sem->value --;
	sti();	/* 开中断 */
	return 0;
}

/*本函数参考ll_rw_blk.c unlock_buffer()
 *功能:信号量V操作
 *成功返回0;失败返回负数码
*/
int sys_sem_post(sem_t *sem)
{
	sem->value++;
/*PS:这个wake_up使调sleep_on函数睡眠的进程起来,将下一个进程唤醒,然后下一个进程唤醒...直至tmp==NULL就是所有进程唤醒*/
	if(sem->value >=1)
		wake_up(&sem->queue);
	return 0;
}

/*功能:删除名为name的信号量
 *name:信号量名
 *成功返回0;失败返回负数码
*/
int sys_sem_unlink(const char *name)
{
	{
	char kernelname[SEM_NAME_LEN];
    int isExist = 0;
    int i=0;
    int name_cnt=0,name_len=0, sem_name_len=0;
	
	/*检查长度*/
    while( get_fs_byte(name+name_cnt) != '\0')
   		name_cnt++;
    if(name_cnt>SEM_NAME_LEN)
    	return -1;

	/*获取信号量名字*/
    for(i=0;i<name_cnt;i++)
    	kernelname[i]=get_fs_byte(name+i);
	
    name_len = strlen(kernelname);
	printk("name_len:%d   name_cnt:%d\n",name_len, name_cnt);
	/* 再次检查 */
	if(name_len != name_cnt)
		return -1;
    for(i=0;i<cnt;i++)
    {
    	/*匹配信号量表*/
        sem_name_len = strlen(semtable[i].name);
        if(sem_name_len == name_len)
            if( !strcmp(kernelname,semtable[i].name) )
            {
                isExist = 1;
                break;
            }
    }
    if(isExist == 1)
    {
       int tmp=0;
	   for(tmp=i; tmp<=cnt; tmp++)
	   {
			semtable[tmp] = semtable[tmp+1];
	   }
	   cnt--;
    }
    else
       return -1;

    return 0;
}
}

