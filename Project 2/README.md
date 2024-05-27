# Assignment 2 Submission

## Kernel Module
- **Source Code**: `chauhan_divanshu.c`
- **Makefile**: Provided in the `kernel_module` directory.

## System Call
- **Source Code**: `my_syscall.c`
- **Makefile**: Provided in the `kernel_syscall` directory.

## User-space Test Program
- **Source Code**: `syscall_in_userspace_test.c`

## Screenshots
- **insmod_rmmod_output.png**: Shows the output of inserting and removing the kernel module.
- **Syscall_output.png**: Shows the output of running the user-space test program calling the new syscall.

## Special Considerations
- The kernel module and syscall have been tested on a specific kernel version. Ensure you are using a compatible environment.
- If any issues are found in the code or screenshots, refer to this README for explanations and potential workarounds.

## Notes for Graders
- The code compiles and runs without errors on the specified kernel version.
- The syscall number `451` has been chosen arbitrarily for this assignment.
- Make sure to load the module before running the user-space test program.
- I took a bit of refernce from arm64 even for x86 compiling and linking.
- Took guidance from linux kernel documentation.
