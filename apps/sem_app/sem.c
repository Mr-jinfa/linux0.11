#include "head.h"

void init_sem(int semid, int semnum, int val)
{
	union semun
	{
		int val;
		struct semid_ds *buf;
		unsigned short *array;
		struct seminfo *__buf;
	}a;

	a.val = val;
	semctl(semid, semnum, SETVAL, a);
}

void sem_p(int semid, int semnum)
{
	struct sembuf sops;

	sops.sem_num = semnum; 
	sops.sem_op  = -1;
	sops.sem_flg = SEM_UNDO;

	semop(semid, &sops, 1);
}

void sem_v(int semid, int semnum)
{
	struct sembuf sops;

	sops.sem_num = semnum; /*第几个信号量*/
	sops.sem_op  = +1;
	sops.sem_flg = SEM_UNDO;

	semop(semid, &sops, 1);
}


