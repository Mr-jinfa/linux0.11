/*
 *  linux/kernel/printk.c
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 * When in kernel-mode, we cannot use printf, as fs is liable to
 * point to 'interesting' things. Make a printf with fs-saving, and
 * all is well.
 */
#include <stdarg.h>
#include <stddef.h>

#include <linux/kernel.h>
#include <linux/sched.h>
#include <sys/stat.h>

static char buf[1024];
static char logbuf[1024];

extern int vsprintf(char * buf, const char * fmt, va_list args);

int printk(const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	__asm__("push %%fs\n\t"
		"push %%ds\n\t"
		"pop %%fs\n\t"
		"pushl %0\n\t"
		"pushl $buf\n\t"
		"pushl $0\n\t"
		"call tty_write\n\t"
		"addl $8,%%esp\n\t"
		"popl %0\n\t"
		"pop %%fs"
		::"r" (i):"ax","cx","dx");
	return i;
}

#include <linux/sched.h>
#include <sys/stat.h>

static char logbuf[1024];
//由printk复制过来并做修改以支持输出到文件
int fprintk(int fd, const char *fmt, ...)
{
	va_list args;
	int i;	
	struct m_inode * inode;
	struct file *file;

	va_start(args, fmt);
	i=vsprintf(logbuf,fmt,args);
	va_end(args);
	if (fd < 3)/*stdin stdout stderr ?*/
	{
		__asm__("push %%fs\n\t"
		"push %%ds\n\t"
		"pop %%fs\n\t"
		"pushl %0\n\t"
		"pushl $logbuf\n\t"
		"pushl %1\n\t"
		"call sys_write\n\t"
		"addl $8,%%esp\n\t"
		"popl %0\n\t"
		"pop %%fs"
		::"r" (i),"r" (fd):"ax","cx","dx");
	}
	else
	{
		if (!(file=task[0]->filp[fd]))    /* 从进程0的文件描述符表中得到文件句柄 */
		return 0;
		inode=file->f_inode; //获取该文件索引,它时文件的标识.

		__asm__("push %%fs\n\t"
		"push %%ds\n\t"
		"pop %%fs\n\t"
		"pushl %0\n\t"
		"pushl $logbuf\n\t"
		"pushl %1\n\t"
		"pushl %2\n\t"
		"call file_write\n\t"
		"addl $12,%%esp\n\t"
		"popl %0\n\t"
		"pop %%fs"
		::"r" (i),"r" (file),"r" (inode):"ax","cx","dx");
	}
	return i;
}

