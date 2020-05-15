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
#include <linux/net.h>
#include <linux/fdtable.h>

#define SUCCESS 1
#define FAILURE -1
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

static inline int print_address_family(struct socket *sock) {
	PTR_NULL_CHECK(sock);

	switch(sock->ops->family) {
		case AF_UNIX:	pr_info("Address family: AF_UNIX/AF_LOCAL\n"); break;
		case AF_INET:	pr_info("Address family: AF_INET\n"); break;
		case AF_INET6:	pr_info("Address family: AF_INET6\n"); break;
		case AF_PACKET:	pr_info("Address family: AF_PACKET\n"); break;
		default:	pr_info("Address family: (%d)\n", sock->ops->family); // Check socket.h
	}

	return SUCCESS;
}

static inline int print_sock_type(short s_type) {
	switch(s_type) {
		case SOCK_STREAM:	pr_info("Type: SOCK_STREAM\n"); break;
		case SOCK_DGRAM:	pr_info("Type: SOCK_DGRAM\n"); break;
		case SOCK_RAW:		pr_info("Type: SOCK_RAW\n"); break;
		case SOCK_RDM:		pr_info("Type: SOCK_RDM\n"); break;
		case SOCK_SEQPACKET:	pr_info("Type: SOCK_SEQPACKET\n"); break;
		case SOCK_DCCP:		pr_info("Type: SOCK_DCCP\n"); break;
		case SOCK_PACKET:	pr_info("Type: SOCK_PACKET\n"); break;
		default:		pr_info("Type: Unknown\n");
	}

	return SUCCESS;
}

static inline int print_file_descriptor_details(const void *arg, struct file *f, unsigned fd_value) {
	char *binary_path, *temp_path;
	struct socket *sock_data;
	int err;

	temp_path = (char *) kmalloc(PATH_MAX, GFP_KERNEL);
	binary_path = d_path(&f->f_path, temp_path, PATH_MAX);
	pr_info("File descriptor: %d\n", fd_value);
	pr_info("File path: %s\n", binary_path);
	kfree(temp_path);

	sock_data = sock_from_file(f, &err);
	if(sock_data) {
		print_sock_type(sock_data->type);
		print_address_family(sock_data);
	}

	return SUCCESS;
}

static inline int print_list_of_open_files(struct task_struct* task) {
	iterate_fd(task->files, 0, print_file_descriptor_details, NULL);

	return SUCCESS;
}
