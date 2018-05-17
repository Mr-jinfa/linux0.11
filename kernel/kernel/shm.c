#define __LIBRARY__
#include <sys/shm.h>

static shm_t shm_list[SHMLIST_LEN] = {{0,0,0},};

/*功能:打开或创建一个共享内存
 *key，共享内存的键值,如果该共享内存不存在，就创建新的共享内存,如果存在，就打开。
 *size，共享内存的大小，仅当新建信号量时，此参数才有效，其余情况下它被忽略。
 *返回值。当成功时，返回值是该共享内存的唯一标识。如果size超过一页内存的大小，
 *返回-1，并置errno为EINVAL。如果系统无空闲内存，返回-1，并置errno为ENOMEM
*/
int sys_shmget(size_t key, size_t size)
{
    int i;
    void *page;
    if(size > PAGE_SIZE)
        return -EINVAL;
    page = get_free_page();
    if(!page)
        return -ENOMEM;
    printk("shmget get memory's address is 0x%08x\n",page);
    for(i=0; i<SHMLIST_LEN; i++)
    {
        if(shm_list[i].key == key)
            return i;
    }
    for(i=0; i<SHMLIST_LEN; i++)
    {
        if(shm_list[i].key == 0)
        {
            shm_list[i].page = page;
            shm_list[i].key = key;
            shm_list[i].size = size;
            return i;
        }
    }
    return -1;
}

/*功能:将shmid指定的共享页面映射到当前进程的虚拟地址空间中
本课程内容，由作者授权实验楼发布，未经允许，禁止转载、下载及非法传播
 *shmid，共享内存的标识。
 *返回值。当成功时，返回值共享页面的首地址，如果shmid非法，返回-1，并置errno为EINVAL
*/
void * sys_shmat(int shmid)
{
    int i;
    unsigned long data_base, brk;

    if(shmid < 0 || SHMLIST_LEN <= shmid || shm_list[shmid].page==0 || shm_list[shmid].key <= 0)
        return (void *)-EINVAL;

    data_base = get_base(current->ldt[2]);
    printk("current's data_base = 0x%08x,new page = 0x%08x\n",data_base,shm_list[shmid].page);

    brk = current->brk + data_base;
    current->brk += PAGE_SIZE;

    if(put_page(shm_list[shmid].page, brk) == 0)
        return (void *)-ENOMEM;

    return (void *)(current->brk - PAGE_SIZE);
}

