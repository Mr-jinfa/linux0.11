#ifndef _SHM_H
#define _SHM_H

#include <unistd.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/shm.h>

#define SHMLIST_LEN    32

typedef struct shm{
	size_t key;
	unsigned long page;
	size_t size;
} shm_t;

#endif

