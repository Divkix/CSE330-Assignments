#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
int main()
{
    long int shoutout = syscall(451);
    printf("System call sys_my_syscall returned %ld\n", shoutout);
    return 0;
}
