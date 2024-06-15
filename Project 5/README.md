# Project 5: Memory Management Simulation

## Summary
To support multiprogramming, modern OSes present virtual memory for processes so that each process sees a contiguous logical memory address space which may not be contiguous in physical memory and may not be completely in memory. The OS maintains the mapping between a process’ virtual memory and the computer’s physical memory through the process’ page table. Every page table entry stores the mapping information of a virtual page, indicating whether the page is present in memory and, if yes, the physical frame number it is mapped to. Additional data structures track the temporarily swapped pages from memory to disk.

In this assignment, we will implement some simplified C++ codes in userspace to simulate memory management. This assignment is the necessary preparation for the Project 6.

## Screenshots
The screenshots are placed in the 'Screenshots' folder.

## Notes:
I completed this project on my MacOS machine. The code is tested on MacOS and Linux. It should work on Windows as well, but I have not tested it on Windows.
