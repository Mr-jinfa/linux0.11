/*
 *  File name: linux0.11/kernel/lock.c
 *
 *  Author: 黄进发   blogs:https://blog.csdn.net/huang_165
 *
 *  Date: 2018-5
 *  
 *  Description: linux0.11锁源码
 *
 *  GitHub: https://github.com/Mr-jinfa/   Bug Report: 2112201069@qq.com
*/

#include <sys/lock.h>
#include <linux/sched.h>
#include <unistd.h>
#include <asm/segment.h>
#include <linux/tty.h>
#include <linux/kernel.h>
#include <linux/fdreg.h>
#include <asm/system.h>
#include <asm/io.h>

lock_t locktable[LOCKTABLE_LEN];	/* 信号量集合 */
int lock_cnt = 0;

/*功能:打开或创建一个锁
 *name，锁的名字,如果该锁不存在，就创建新的名为name的锁,如果存在，就打开。
 *返回值。当成功时，返回值是该锁的唯一标识（比如，在内核的地址、ID等）。如失败，返回值是NULL。
*/
lock_t *sys_lock_init(const char *name)
{
	char kernelname[LOCK_NAME_LEN];
	int isExist = 0;
	int i=0;
	int name_cnt=0,name_len=0, lock_name_len=0;
	
	/*检查长度*/
	while( get_fs_byte(name+name_cnt) != '\0')
		name_cnt++;
	if(name_cnt>SEM_NAME_LEN)
		return NULL;

	/*获取锁名字*/
	for(i=0;i<name_cnt;i++)
		kernelname[i]=get_fs_byte(name+i);
	name_len = strlen(kernelname);
	/* 再次检查 */
	if(name_len != name_cnt)
		return NULL;

	sem_t *p=NULL;
	for(i=0;i<lock_cnt;i++)
	{
		/*匹配锁表*/
		lock_name_len = strlen(locktable[i].name);
		if(lock_name_len == name_len)
		if( !strcmp(kernelname,locktable[i].name) )
		{
			isExist = 1;
			break;
		}
	}
		if(isExist == 1)
			p=(sem_t*)(&locktable[i]);
		else
		{
			for(i=0;i<name_len;i++)
				locktable[lock_cnt].name[i]=kernelname[i];
			locktable[lock_cnt].lock = 0;
			p=(sem_t*)(&locktable[lock_cnt]);
			/*printk("creat lock!\n");*/
			lock_cnt++;
	}
	return p;
}

/*本函数参考ll_rw_blk.c lock_buffer()
 *功能:锁操作
 *返回:无返回
*/
void sys_mutex_lock(lock_t * lkh)
{
	cli();	/* 关中断 */
	while (lkh->lock)
		sleep_on(&lkh->queue);	/* 如果锁上,那么该进程睡眠并调度 */
	lkh->lock=1;	/* 获得锁 */
	sti();	/* 开中断 */
}

/*本函数参考ll_rw_blk.c unlock_buffer()
 *功能:解锁操作
 *返回:无返回
*/
void sys_mutex_unlock(lock_t * lkh)
{
	if (!lkh->lock)
		printk("ll_rw_block.c: buffer not locked\n\r");
	lkh->lock = 0;	/* 解锁 */
	wake_up(&lkh->queue);
}



/*功能:删除名为name的锁
 *name:信号量名
 *返回:成功返回0,失败返回负数
*/
int sys_lock_del(const char *name)
{
	char kernelname[LOCK_NAME_LEN];
	int isExist = 0;
	int i=0;
	int name_cnt=0,name_len=0, lock_name_len=0;
	
	/*检查长度*/
	while( get_fs_byte(name+name_cnt) != '\0')
		name_cnt++;
	if(name_cnt>LOCK_NAME_LEN)
		return -1;

	/*获取信号量名字*/
	for(i=0;i<name_cnt;i++)
		kernelname[i]=get_fs_byte(name+i);
	
	name_len = strlen(kernelname);
	printk("name_len:%d   name_cnt:%d\n",name_len, name_cnt);
	/* 再次检查 */
	if(name_len != name_cnt)
		return -1;
	for(i=0;i<lock_cnt;i++)
	{
		/*匹配信号量表*/
		lock_name_len = strlen(locktable[i].name);
		if(lock_name_len == name_len)
		if( !strcmp(kernelname,locktable[i].name) )
		{
			isExist = 1;
			break;
		}
	}
	if(isExist == 1)
	{
		int tmp=0;
		for(tmp=i; tmp<lock_cnt; tmp++)
		{
			locktable[tmp] = locktable[tmp+1];
		}
		lock_cnt--;
	}
	else
	return -1;
	/*printk("close lock!\n");*/

	return 0;
}

