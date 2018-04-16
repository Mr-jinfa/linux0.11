
#define __LIBRARY__
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utime.h>
#include <sys/stat.h>
#include <linux/sched.h>
#include <linux/tty.h>
#include <linux/kernel.h>
#include "unistd.h"
#include <asm/segment.h>

_syscall1(int, iam, const char*, name)

void usage()
{
	printf("please input ./iam [name]\n");
}
int main(int argc, char const *argv[])
{
	if(argc !=2){
		usage();
		return -1;
	}	
	if(iam(argv[1]) == -1){
     perror("set my name fail");
     return -1;  
   }
	return 0;
}
