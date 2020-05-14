#define pr_fmt(fmt)	KBUILD_MODNAME " -> %s:%d -> " fmt,__func__,__LINE__
#include <linux/module.h>
#include <linux/init.h>
#include "processlibrary.h"

MODULE_AUTHOR("Cooper Van Kampen");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Get thread details");

static int process_id;
module_param(process_id, int, S_IWUSR | S_IRUGO);
MODULE_PARM_DESC(process_id, "Target userspace PID");

static int __init lkm_init(void) {
	struct task_struct *target_task;
	struct task_struct *process_thread;

	target_task = get_task_struct_by_pid(process_id);
	if(!IS_ERR_OR_NULL(target_task)) {
		// PID details
		pr_info("Received uspace PID: %d\n", process_id);
		print_task_pid_details(target_task);
		print_task_parent_pid_details(target_task);

		// Thread details for PID
		pr_info("Number of threads: %d\n", get_task_thread_count(target_task));
		pr_info("--------------\n");
		pr_info("Threads details\n");
		
		for_each_thread(target_task, process_thread) // Uses RCU doubly linked list macro in signal.h
			print_task_pid_details(process_thread);
		pr_info("--------------\n");
	} else {
		pr_info("Received invalid uspace PID: %d\n", process_id);
		return -EINVAL;
	}

	return 0;
}

static void __exit lkm_exit(void) {
	pr_info("Exiting hellwold lkm\n");
}

// Define init and exit functions
module_init(lkm_init);
module_exit(lkm_exit);
