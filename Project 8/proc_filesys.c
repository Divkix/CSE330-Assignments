#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define MAX_MESSAGE_SIZE 100
static char kernel_buffer[MAX_MESSAGE_SIZE];

static ssize_t write_to_kernel(struct file *file, const char __user *user_buffer, size_t count, loff_t *ppos)
{
    if (count > MAX_MESSAGE_SIZE)
        return -EINVAL;

    if (copy_from_user(kernel_buffer, user_buffer, count))
        return -EFAULT;

    pr_info("Received from user space: %s", kernel_buffer);
    return count;
}

static ssize_t read_from_kernel(struct file *file, char __user *user_buffer, size_t count, loff_t *ppos)
{
    ssize_t retval;

    if (*ppos > strlen(kernel_buffer))
        return -EINVAL;

    retval = min(count, (size_t)(strlen(kernel_buffer) - *ppos));

    if (copy_to_user(user_buffer, kernel_buffer + *ppos, retval))
        return -EFAULT;

    pr_info("Read %zd bytes from kernel space starting at position %lld\n", retval, *ppos);
    pr_info("Read from kernel space: %.*s\n", (int)retval, kernel_buffer + *ppos);

    *ppos += retval;

    return retval;
}

static const struct proc_ops proc_operations = {
    .proc_read = read_from_kernel,
    .proc_write = write_to_kernel,
};

static int __init vfs_init(void)
{
    proc_create("my_proc_file", 0666, NULL, &proc_operations);
    pr_info("Module loaded\n");
    return 0;
}

static void __exit vfs_exit(void)
{
    remove_proc_entry("my_proc_file", NULL);
    pr_info("Module unloaded\n");
}

module_init(vfs_init);
module_exit(vfs_exit);

MODULE_VERSION("0.1");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Divanshu Chauhan");
MODULE_DESCRIPTION("CSE330 Project 8: Kernel File System Implementation\n");
