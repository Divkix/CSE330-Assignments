# Project 8: Kernel File System Implementation
## Summary
The Virtual Filesystem (VFS) is a kernel component that handles all system calls related to files and file systems. It also provides an abstraction within the kernel, allowing different filesystem implementations to coexist. This abstraction simplifies the implementation of file systems and provides an easier integration of multiple file systems. This way, a file system is implemented using the API provided by the VFS, and the VFS handles the generic hardware and I/O subsystem communication parts. Linux kernel supports many different types of filesystems. While network and disk-based file systems enable the user to handle information stored outside the kernel, special file systems may provide an easy way for system programs and administrators to manipulate the data structures of the kernel and to implement special features of the operating system.

 

In this project, you will learn about one of the special Linux file systems “proc” which acts as a general access point to the kernel data structures. These special file systems are not bound to physical block devices. However, the kernel assigns to each mounted special filesystem a fictitious block device that has a major number as 0 and an arbitrary unique value as the minor number.  The fictitious block device identifiers help the kernel handle special filesystems and regular ones in a uniform way.  This project will help you understand how to create your own custom proc fs and define read and write file operations to these proc entires showcasing how proc fs acts as a bridge connecting the userspace and the kernel space.

## Screenshots
Screenshots are placed in the screenshots folder.
