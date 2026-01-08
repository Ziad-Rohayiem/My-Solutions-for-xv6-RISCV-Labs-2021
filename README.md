# Lab: Page Tables

## Problems / Challenges

In this lab, we explored page tables and virtual memory in xv6. The goal was to modify the memory management code to support new features and optimizations.

1.  **Speed Up System Calls**: Some system calls (like `getpid`) are simple but incur overhead due to context switching (traps). The challenge was to implement a shared memory region between the kernel and user space to allow the user process to read its PID directly without a system call trap.
2.  **Print a Page Table**: Debugging virtual memory issues is difficult without visibility. The task was to write a function that recursively prints the contents of a page table tree to visualize the mappings.
3.  **Detect Accessed Pages**: To support features like garbage collection or paging out to disk, the kernel needs to know which pages have been accessed recently. The challenge was to implement a system call `pgaccess` that scans page tables and reports accessed pages, clearing the accessed bit in the process.

## Descriptive Explanation of Solutions

### 1. Speed Up System Calls
*   **Shared Page**: I utilized the `USYSCALL` mapping (defined in `memlayout.h` and `riscv.h`) to share data.
*   **Kernel Setup**:
    *   Added a `struct usyscall *usyscallpage` field to `struct proc`.
    *   In `allocproc` (kernel/proc.c), I allocate a physical page for this structure and store the current process's PID in it (`p->usyscallpage->pid = p->pid`).
    *   In `proc_pagetable`, I mapped this page at virtual address `USYSCALL` with permissions `PTE_R | PTE_U`, allowing user space read access.
    *   I ensured this page is freed in `freeproc`.
*   **User Side**: The user library `ugetpid()` now simply reads the PID from the fixed virtual address `USYSCALL` instead of executing the `ecall` instruction.

### 2. Print a Page Table
*   **Implementation**: I implemented the `vmprint(pagetable, level)` function in `kernel/vm.c`.
*   **Logic**:
    *   The function recursively traverses the 3-level page table hierarchy (levels 2, 1, 0).
    *   It loops through all 512 entries of the given page table.
    *   If a valid entry (`PTE_V`) is found, it prints the index, the PTE bits, and the physical address.
    *   If the entry points to a lower-level table (i.e., not a leaf), it recurses with `level + 1`.
    *   I used indentation (" ..") proportional to the depth of recursion to make the output readable.
*   **Integration**: I inserted a call to `vmprint` in `exec.c` to print the page table of the first process (init).

### 3. Detect Accessed Pages (`pgaccess`)
*   **System Call**: I implemented `sys_pgaccess` in `kernel/sysproc.c`.
*   **Logic**:
    *   The syscall accepts a starting virtual address, a number of pages to check, and a user address to store the result bitmask.
    *   I loop through the specified number of pages. For each page:
        *   I calculate the virtual address (`addr + i * PGSIZE`).
        *   I use the `walk` function to find the corresponding PTE.
        *   I check if the `PTE_A` (Accessed) bit is set. (This bit is set by the hardware whenever a page is accessed).
        *   If set, I mark the corresponding bit in a temporary bitmask and **clear** the `PTE_A` bit on the PTE (so subsequent calls can detect new accesses).
    *   Finally, I copy the resulting bitmask to the user buffer using `copyout`.
