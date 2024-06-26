#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/mm.h>
#include <linux/sched/mm.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/hugetlb.h>
#define _PAGE_BIT_ACCESSED 5

static int pid = 0;
module_param(pid, int, 0644);

struct task_struct *task = NULL;

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

            VMA_ITERATOR(vmi_iter, mm, 0);

            for_each_vma(vmi_iter, vma)
            {

                for (unsigned long page = vma->vm_start; page < vma->vm_end; page += PAGE_SIZE)
                {

                    pgd_t *pgd;
                    p4d_t *p4d;
                    pmd_t *pmd;
                    pud_t *pud;
                    pte_t *ptep, pte;

                    pgd = pgd_offset(mm, page);
                    p4d = p4d_offset(pgd, page);
                    pud = pud_offset(p4d, page);
                    pmd = pmd_offset(pud, page);
                    ptep = pte_offset_kernel(pmd, page);
                    pte = *ptep;

                    if (!pte_none(pte))
                    {

                        if (pte_present(pte))
                        {
                            total_rss++;

                            if (pte_young(pte))
                            {
                                total_wss++;
                                test_and_clear_bit(_PAGE_BIT_ACCESSED, (unsigned long *)ptep);
                            }
                        }
                        else
                        {
                            total_swap++;
                        }
                    }
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
    printk("CSE330 Project Kernel Module Inserted\n");
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
    printk("CSE330 Project Kernel Module Removed\n");
}

module_init(memory_init);
module_exit(memory_cleanup);

MODULE_VERSION("0.1");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Divanshu Chauhan");
MODULE_DESCRIPTION("CSE330 Project 6: Virtual Memory Traversal");
