# Project 3: Kernel Threads and Kernel-space Shared-memory IPC

I tried to make this work on my server but my server crashed, then tried linode but linode crashed as well because of high cpu usage and I wasn't able to fully do it. Then I shifted to UTM but it was too slow, then finally made an AWS account to start from project 1.

If there are any issues, please let me know if it doesn't work. I will try my best to fix it and provide a working link.

## Submitted Files
- producer_consumer.c
- README.md

## Compilation and Execution
1. Place `producer_consumer.c`, `Makefile`, and `thread_count.sh` in the same directory.
2. Change directory to the project folder.
3. Run `make` to compile the kernel module.
4. Load the module: `sudo insmod producer_consumer.ko size=<N> prod=<P> cons=<C>`
5. Unload the module: `sudo rmmod producer_consumer`

## Testing Scenarios
- `size=5 prod=5 cons=10`: Should create 5 producer and 10 consumer threads.
- `size=5 prod=10 cons=5`: Should create 10 producer and 5 consumer threads.
- `size=5 prod=5 cons=0`: Should only produce items without consumption.
- `size=5 prod=0 cons=5`: Should neither produce nor consume items.
- `size=10 prod=5 cons=5`: Should produce and consume items alternately.

## Known Issues
- [If any, list here]

## Additional Notes
- Use `dmesg` to check kernel log messages for module operations.
- Clear `dmesg` logs using `sudo dmesg -C` before each test.
