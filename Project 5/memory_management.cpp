//
// Author: Divanshu Chauhan
// Date: 06/25/2024
// ASU ID: 1224807311
//
// Description: In this assignment, we will implement some simplified C++ codes in userspace
//              to simulate memory management. This assignment is the necessary preparation
//              for the Project 6.
//

#include <iostream>
#include <vector>

// This is a simplified version of the Linux kernel's memory management: mm_types.h
// You can find the original file at: https://elixir.bootlin.com/linux/v6.4.11/source/include/linux/mm_types.h

// Define the Page structure to track individual pages
struct page
{
    bool pte_present; // A flag to indicate if the page is in memory or swapped
    bool pte_young;   // A flag to indicate if the page is in use
    std::string data; // Name or data associated with the page
};

// Define the vm_area_struct structure (simplified for illustration)
struct vm_area_struct
{
    unsigned long vm_start;  // Start address of the VMA
    unsigned long vm_end;    // End address of the VMA
    std::vector<page> pages; // A list of pages in the VMA
};

// Define a mm_struct structure (simplified for illustration)
struct mm_struct
{
    std::vector<vm_area_struct> vma_list; // A list of VMAs
};

struct vma_iterator
{
    std::vector<vm_area_struct> vma_list; // A list of VMAs
    size_t index;                         // Index to track the current VMA
};

// Define a macro to initialize the vma_iterator
#define VMA_ITERATOR(name, __mm, __addr) \
    struct vma_iterator name = {         \
        .vma_list = (__mm).vma_list,     \
        .index = __addr}

// Function to get the next VMA
vm_area_struct *vma_next(vma_iterator *vmi)
{
    if (vmi->index < vmi->vma_list.size())
    {
        return &vmi->vma_list[vmi->index++];
    }
    else
    {
        return nullptr;
    }
}

// Define a macro to iterate through VMAs
#define for_each_vma(__vmi, __vma) \
    while (((__vma) = vma_next(&(__vmi))) != NULL)

int main()
{
    // Example usage
    mm_struct mm;
    struct vm_area_struct *vma = NULL;

    // Populate mm with some VMAs for demonstration
    vm_area_struct vma1 = {
        0x1000,
        0x3000,
        {{true, true, "Page 1"},
         {false, false, "Page 2"},
         {true, false, "Page 3"},
         {true, true, "Page 4"},
         {false, false, "Page 5"}}};

    vm_area_struct vma2 = {
        0x4000,
        0x6000,
        {{true, false, "Page 6"},
         {false, false, "Page 7"},
         {true, true, "Page 8"},
         {true, false, "Page 9"},
         {true, true, "Page 10"}}};

    mm.vma_list.push_back(vma1);
    mm.vma_list.push_back(vma2);

    // Initialize memory statistics variables
    unsigned long total_rss = 0;
    unsigned long total_swap = 0;
    unsigned long total_wss = 0;

    // TODO 1: Use mm_struct to initialize the VMA_ITERATOR
    VMA_ITERATOR(vmi, mm, 0);

    // TODO 2: Replace the while loop with for_each_vma macro
    for_each_vma(vmi, vma)
    {
        // Initialize VMA statistics variables
        unsigned long vma_rss = 0;
        unsigned long vma_swap = 0;
        unsigned long vma_wss = 0;

        // TODO 3: Iterate through the pages in the VMA and update the memory statistics
        for (const auto &page : vma->pages)
        {
            if (page.pte_present)
            {
                vma_rss++;
                if (page.pte_young)
                {
                    vma_wss++;
                }
            }
            else
            {
                vma_swap++;
            }
        }

        // Update the memory statistics for the entire process
        total_rss += vma_rss;
        total_swap += vma_swap;
        total_wss += vma_wss;

        // Print memory statistics for the VMA
        std::cout << std::endl;
        std::cout << "VMA: " << vma->vm_start << " - " << vma->vm_end << std::endl;
        std::cout << "  Resident Set Size: " << vma_rss << std::endl;
        std::cout << "  Swapped Size: " << vma_swap << std::endl;
        std::cout << "  Working Set Size: " << vma_wss << std::endl;
    }

    // Print memory statistics for the entire process
    std::cout << std::endl;
    std::cout << "Memory Statistics:" << std::endl;
    std::cout << "  Total Resident Set Size: " << total_rss << std::endl;
    std::cout << "  Total Swapped Size: " << total_swap << std::endl;
    std::cout << "  Total Working Set Size: " << total_wss << std::endl;

    return 0;
}
