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

void usage(void)
{
	printf("please input ./whoami \n");
}
int main(int argc, char const *argv[])
{
	char *name;
	int size = 23;
  int ret=0;
	if(argc !=1){
		usage();
		return -1;
	}
	name = malloc(size);
	ret = whoami(name, size);
	if(ret == -1)
		perror("get my name fail");
   printf("%s", name);
	return 0;
}
