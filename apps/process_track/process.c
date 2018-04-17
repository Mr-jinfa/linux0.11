#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/wait.h>


void cpuio_bound(int last, int cpu_time, int io_time);

int main(int argc, char * argv[])
{
	int i=3;
	printf("\n============i am parent now i create some child=================\n");
	while(i--)
	{
		pid_t x = fork();
		if(x == 0 &&i==2)
		{
			char *arg[] = {"./child2", NULL};
			execv("./child2", arg);
		}
		else if(x == 0 &&i==1)
		{
			char *arg[] = {"./child1", NULL};
			execv("./child2", arg);
		}
		else if(x == 0 &&i==0)
		{
			char *arg[] = {"./child0", NULL};
			execv("./child0", arg);
		}
	
		/* 对于父进程而言，x就是刚刚产生的子进程的ID号。*/
		if(x > 0)
		{
			int status=0;
			wait(&status);
			printf("      child id:%d\n\n", x);
		}
	}
	printf("\n============i am parent now i exitt=================\n");
	return 0;
}


