//create by jinfa 2018.4.6
/*
@添加foo()系统调用的步骤。

在inlcude/unistd.h 添加系统调用编号__NR_foo，并声明函数原型int foo();

修改kernel/system_call.s中系统调用总数nr_system_call;

在include/linux/sys.h中添加extern int foo()，并且在系统调用数组表中fn_ptr_sys_call_table[]添加sys_foo;

在kerne目录中实现foo(); 

最后修改Makefile的OBJS，加入xxx.o，添加xxx.s xxx.o 的依赖规则(包含头文件的路径)。

*/
#define __LIBRARY__
#include <errno.h>
#include <asm/segment.h>
#include "unistd.h"
#include <errno.h>

#define MAX_SIZE        32
char usrname[MAX_SIZE]={0};
#define MAX_LEN 23
int sys_whoami(char *name, unsigned int size)
{
	int cnt=0,i=0;
  while(usrname[cnt] != '\0')
          cnt++;

   if(size < cnt){
       errno = EINVAL;
       return -1;
   }

  for(i=0; i<=cnt; i++)
          put_fs_byte(usrname[i], (name+i));
  return cnt;  
}

int sys_iam(const char *name)
{
  int cnt=0,i=0;
  while(get_fs_byte(name+cnt) != '\0')
          cnt++;

  if(cnt > MAX_LEN){
          errno = EINVAL;
          return -1;
  }

  for(i=0; i<=cnt; i++)
          usrname[i] = get_fs_byte(name+i);
  return cnt;
}
