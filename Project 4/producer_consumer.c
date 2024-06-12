#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include <linux/cred.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <linux/timekeeping.h>

#define MAX_BUFFER_SIZE 500

#define PCINFO(s, ...) pr_info("###[%s]###" s, __FUNCTION__, ##__VA_ARGS__)

unsigned long long total_time_elapsed = 0;

struct process_info
{
    unsigned long pid;
    unsigned long long start_time;
    unsigned long long boot_time;
} process_default_info = {0, 0};

int total_no_of_process_produced = 0;
int total_no_of_process_consumed = 0;

int end_flag = 0;

static struct task_struct *ctx_producer_thread;
static struct task_struct *ctx_consumer_thread[MAX_BUFFER_SIZE];

struct process_info buffer[MAX_BUFFER_SIZE];
int fill = 0;
int use = 0;

static int buffSize;
static int prod;
static int cons;
static long uuid;

module_param(buffSize, int, 0644);
module_param(prod, int, 0644);
module_param(cons, int, 0644);
module_param(uuid, long, 0644);

static struct semaphore empty;
static struct semaphore full;
static struct semaphore mutex;

int producer_thread_function(void *pv)
{
    struct task_struct *task;

    allow_signal(SIGKILL);

    for_each_process(task)
    {
        if (task->cred->uid.val == uuid)
        {
            if (down_interruptible(&empty))
            {
                break;
            }
            if (down_interruptible(&mutex))
            {
                up(&empty);
                break;
            }
            if (kthread_should_stop())
            {
                up(&mutex);
                up(&empty);
                break;
            }
            buffer[fill].pid = task->pid;
            buffer[fill].start_time = task->start_time;
            buffer[fill].boot_time = task->start_boottime;
            total_no_of_process_produced++;
            fill = (fill + 1) % buffSize;
            PCINFO("Produced Item#:%d at buffer index: %d for PID:%d\n", total_no_of_process_produced, (fill + buffSize - 1) % buffSize, task->pid);
            up(&mutex);
            up(&full);
        }
    }

    PCINFO("Producer Thread stopped.\n");
    ctx_producer_thread = NULL;
    return 0;
}

int consumer_thread_function(void *pv)
{
    int no_of_process_consumed = 0;

    allow_signal(SIGKILL);

    while (!kthread_should_stop())
    {
        if (down_interruptible(&full))
        {
            break;
        }
        if (down_interruptible(&mutex))
        {
            up(&full);
            break;
        }
        if (end_flag)
        {
            up(&mutex);
            up(&full);
            break;
        }
        struct process_info task = buffer[use];
        use = (use + 1) % buffSize;
        unsigned long long start_time_ns = task.start_time;
        unsigned long process_pid = task.pid;

        unsigned long long ktime = ktime_get_ns();
        unsigned long long process_time_elapsed = (ktime - start_time_ns) / 1000000000;
        total_time_elapsed += ktime - start_time_ns;

        unsigned long long process_time_hr = process_time_elapsed / 3600;
        unsigned long long process_time_min = (process_time_elapsed - 3600 * process_time_hr) / 60;
        unsigned long long process_time_sec = (process_time_elapsed - 3600 * process_time_hr) - (process_time_min * 60);

        no_of_process_consumed++;
        total_no_of_process_consumed++;
        PCINFO("Consumed Item#-%d on buffer index:%d::PID:%lu \t Elapsed Time %llu:%llu:%llu\n", no_of_process_consumed, (use + buffSize - 1) % buffSize, process_pid, process_time_hr, process_time_min, process_time_sec);

        up(&mutex);
        up(&empty);
    }

    PCINFO("Consumer Thread stopped.\n");
    return 0;
}

static int __init thread_init_module(void)
{
    PCINFO("Kernel Module Inserted\n");
    PCINFO("Kernel module received the following inputs: UID:%ld, Buffer-Size:%d, No of Producer:%d, No of Consumer:%d", uuid, buffSize, prod, cons);

    if (buffSize > 0 && (prod == 0 || prod == 1) && cons >= 0)
    {
        sema_init(&empty, buffSize);
        sema_init(&full, 0);
        sema_init(&mutex, 1);

        for (int index = 0; index < buffSize; index++)
        {
            buffer[index] = process_default_info;
        }

        if (prod == 1)
        {
            ctx_producer_thread = kthread_run(producer_thread_function, NULL, "producer");
        }

        for (int i = 0; i < cons; i++)
        {
            ctx_consumer_thread[i] = kthread_run(consumer_thread_function, NULL, "consumer%d", i);
        }
    }
    else
    {
        PCINFO("Incorrect Input Parameter Configuration. No kernel threads started. Please check input parameters.\n");
    }

    return 0;
}

static void __exit thread_exit_module(void)
{
    if (buffSize > 0)
    {
        while (1)
        {
            if (total_no_of_process_consumed == total_no_of_process_produced || cons == 0 || prod == 0)
            {
                if (cons == 0)
                {
                    up(&empty);
                }

                if (prod == 1 && ctx_producer_thread)
                {
                    kthread_stop(ctx_producer_thread);
                }

                end_flag = 1;

                for (int i = 0; i < cons; i++)
                {
                    up(&full);
                    up(&mutex);
                }

                for (int i = 0; i < cons; i++)
                {
                    if (ctx_consumer_thread[i])
                    {
                        kthread_stop(ctx_consumer_thread[i]);
                    }
                }
                break;
            }
        }

        total_time_elapsed = total_time_elapsed / 1000000000;

        unsigned long long total_time_hr = total_time_elapsed / 3600;
        unsigned long long total_time_min = (total_time_elapsed - 3600 * total_time_hr) / 60;
        unsigned long long total_time_sec = (total_time_elapsed - 3600 * total_time_hr) - (total_time_min * 60);

        PCINFO("Total number of items produced: %d\n", total_no_of_process_produced);
        PCINFO("Total number of items consumed: %d\n", total_no_of_process_consumed);
        PCINFO("The total elapsed time of all processes for UID %ld is \t%llu:%llu:%llu\n", uuid, total_time_hr, total_time_min, total_time_sec);
    }

    PCINFO("Kernel Module Removed\n");
}

module_init(thread_init_module);
module_exit(thread_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Divanshu Chauhan");
MODULE_DESCRIPTION("CSE330 2024 Summer Project 4\n");
MODULE_VERSION("0.1");
