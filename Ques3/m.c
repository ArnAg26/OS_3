#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/pid.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/cred.h>


int k=1;
module_param(k,int,0);



static int __init init(void)
{	struct pid *pid_m;
	struct task_struct *task1;
	pid_m=find_get_pid(k);
	task1=pid_task(pid_m,k);
	printk("pid is  %d",task1->pid);
	printk("user id is %d",task1->cred->uid);
	printk("group id is %d",pid_vnr(task_pgrp(task1)));
	printk("process name  %s",task1->comm);		
	printk("hello world%d.\n",k);
	return 0;
}
static void __exit cleanup(void){
	printk("Goodbye,thanks for using");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arnav");   
module_init(init);
module_exit(cleanup);

