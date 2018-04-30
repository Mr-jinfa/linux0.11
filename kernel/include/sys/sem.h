#ifndef _SEM_H
#define _SEM_H
#define SEMTABLE_LEN    32
#define SEM_NAME_LEN    32

typedef struct semaphore{
	char name[SEM_NAME_LEN];
	int value;
	struct task_struct *queue;
} sem_t;
extern sem_t semtable[SEMTABLE_LEN];

#endif
