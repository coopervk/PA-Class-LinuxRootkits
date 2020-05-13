#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/cred.h>

#define SUCCESS 1
#define FAILURE 1
#define PTR_NULL_CHECK(ptr)	if(ptr == NULL) return FAILURE;

static inline struct task_struct* get_task_struct_by_pid(int pid) {
	return pid_task(find_vpid(pid), PIDTYPE_PID);
}

static inline int print_task_pid_details(struct task_struct *task) {
	PTR_NULL_CHECK(task);

	pr_info("Task binary: %s\n", task->comm);
	pr_info("TGID: %d\n", task->tgid);
	pr_info("PID: %d\n", task->pid);

	return SUCCESS;
}

static inline int print_task_parent_pid_details(struct task_struct *task) {
	PTR_NULL_CHECK(task);

	pr_info("Parent task binary: %s\n", task->real_parent->comm);
	pr_info("Parent TGID: %d\n", task->real_parent->tgid);
	pr_info("Parent PID: %d\n", task->real_parent->pid);

	return SUCCESS;
}

static inline int get_task_thread_count(struct task_struct *task) {
	return task->signal->nr_threads;
}
