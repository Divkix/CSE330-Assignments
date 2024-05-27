/* Project 2 Module Installation */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>

#define AUTHOR "Divanshu Chauhan"
MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);

// initialize a static int
static int Intparameter = 0;
module_param(Intparameter, int, 0644);
MODULE_PARM_DESC(Intparameter, "Integer Parameter for the module");

// initialize a static char
static char *charparameter = "default";
module_param(charparameter, charp, 0644);
MODULE_PARM_DESC(charparameter, "Char parameter of the module");

static int __init initialize(void)
{
    printk(KERN_INFO "Hello, I am Divanshu Chauhan, a student of CSE330 %s %d\n", charparameter, Intparameter);
    return 0;
}

static void __exit clean_exit(void)
{
    printk(KERN_INFO "Goodbye-Divanshu Chauhan!\n");
}

module_init(initialize);
module_exit(clean_exit);
