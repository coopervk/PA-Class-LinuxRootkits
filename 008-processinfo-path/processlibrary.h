#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/pid.h>
#include <linux/cred.h>
#include <linux/path.h>
#include <linux/slab.h>
#include <linux/limits.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/limits.h>

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

static inline int print_task_binary_name(struct task_struct *task) {
	char *temp_path;
	char *binary_path;

	PTR_NULL_CHECK(task);

	if(task->mm) {
		down_read(&task->mm->mmap_sem);
		if(task->mm->exe_file) {
			temp_path = kmalloc(PATH_MAX, GFP_KERNEL);
			if(!temp_path) panic("kmalloc failed");

			binary_path = d_path(&task->mm->exe_file->f_path, temp_path, PATH_MAX);
			pr_info("Binary path: %s\n", binary_path);

			kfree(temp_path);
		} else {
			pr_info("Binary path: (none)\n");
			up_read(&task->mm->mmap_sem);
			return FAILURE;
		}
		up_read(&task->mm->mmap_sem);
	} else {
		pr_info("target_process->mm is NULL - Kernel thread?\n"); // Only supports userspace threads
	}

	return SUCCESS;
}


static inline int print_task_root_path_pwd(struct task_struct *task) {
	char *temp_path;
	struct path root_path, pwd_path;
	char *root_path_name, *pwd_path_name;

	PTR_NULL_CHECK(task);

	temp_path = kmalloc(PATH_MAX, GFP_KERNEL);
	if(!temp_path) panic("kmalloc failed");

	get_fs_root(task->fs, &root_path);
	root_path_name = d_path(&root_path, temp_path, PATH_MAX);
	pr_info("Root path: %s\n", root_path_name);

	get_fs_pwd(task->fs, &pwd_path);
	pwd_path_name = d_path(&pwd_path, temp_path, PATH_MAX);
	pr_info("PWD path: %s\n", pwd_path_name);

	kfree(temp_path);

	return SUCCESS;
}
