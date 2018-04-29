#ifndef _LOCK_H
#define _LOCK_H

#define LOCKTABLE_LEN    32
#define LOCK_NAME_LEN    32

typedef struct mutex{
    char name[LOCK_NAME_LEN];
    int lock;
    struct task_struct *queue;
} lock_t;
extern lock_t locktable[LOCKTABLE_LEN];

#endif
