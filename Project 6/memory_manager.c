//
// Created by chang on 10/15/23.
//
#include <linux/mm.h>
#include <linux/sched/mm.h>
#include <linux/module.h>

// TODO 1: Define your input parameter (pid - int) here
// Then use module_param to pass them from insmod command line. (--Assignment 2)

struct task_struct *task = NULL;
// Initialize memory statistics variables
unsigned long total_rss = 0;
unsigned long total_swap = 0;
unsigned long total_wss = 0;

static void parse_vma(void)
{
    struct vm_area_struct *vma = NULL;
    struct mm_struct *mm = NULL;

    if (pid > 0)
    {
        task = pid_task(find_vpid(pid), PIDTYPE_PID);
        if (task && task->mm)
        {
            mm = task->mm;
            // TODO 2: mm_struct to initialize the VMA_ITERATOR (-- Assignment 4)
            // Hint: Only one line of code is needed

            // TODO 3: Iterate through the VMA linked list with for_each_vma (-- Assignment 4)
            // Hint: Only one line of code is needed

            // TODO 4: Iterate through each page of the VMA
            // declear "page" as unsigned long, start from vma->vm_start to vma->vm_end with PAGE_SIZE step
            // Hint: Only one line of code is needed

            // TODO 5: Use pgd_offset, p4d_offset, pud_offset, pmd_offset, pte_offset_map to get the page table entry
            // Hint: Copy from Background Knowledge in the instruction
            // Hint: change the address in the instruction to "page" variable you defined above

            // TODO 6: use pte_none(pte) to check if the page table entry is valid
            // Hint: one if statement here

            // TODO 7: use pte_present(pte) to check if the page is in memory, otherwise it is in swap

            // TODO 8: use pte_young(pte) to check if the page is actively used
            // if it is actively used, update wss and clear the accessed bit by: "test_and_clear_bit(_PAGE_BIT_ACCESSED,(unsigned long *)ppte);"
        }
    }
}
}
}

unsigned long timer_interval_ns = 10e9; // 10 sec timer
static struct hrtimer hr_timer;

enum hrtimer_restart timer_callback(struct hrtimer *timer_for_restart)
{
    ktime_t currtime, interval;
    currtime = ktime_get();
    interval = ktime_set(0, timer_interval_ns);
    hrtimer_forward(timer_for_restart, currtime, interval);
    total_rss = 0;
    total_swap = 0;
    total_wss = 0;
    parse_vma();
    printk("[PID-%i]:[RSS:%lu MB] [Swap:%lu MB] [WSS:%lu MB]\n", pid, total_rss * 4 / 1024, total_swap * 4 / 1024, total_wss * 4 / 1024);
    return HRTIMER_RESTART;
}

int memory_init(void)
{
    printk("CSE330 Project 2 Kernel Module Inserted\n");
    ktime_t ktime;
    ktime = ktime_set(0, timer_interval_ns);
    hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    hr_timer.function = &timer_callback;
    hrtimer_start(&hr_timer, ktime, HRTIMER_MODE_REL);
    return 0;
}

void memory_cleanup(void)
{
    int ret;
    ret = hrtimer_cancel(&hr_timer);
    if (ret)
        printk("HR Timer cancelled ...\n");
    printk("CSE330 Project 2 Kernel Module Removed\n");
}

module_init(memory_init);
module_exit(memory_cleanup);

MODULE_VERSION("0.1");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("CSE330 Project 2 Memory Management\n");