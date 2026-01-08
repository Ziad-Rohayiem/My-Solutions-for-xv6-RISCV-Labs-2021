# Lab: System Calls

## Problems / Challenges

This lab focuses on understanding how system calls enable user programs to interact with the kernel. The challenges involved modifying the kernel to add new system calls and tracking system resources.

1.  **System Call Tracing (`trace`)**: The goal was to implement a `trace` system call that takes an integer mask as an argument. For every subsequent system call executed by the process (or its children), if the corresponding bit in the mask is set, the kernel should print the process ID, the system call name, and its return value. This required modifying the `proc` structure, the `fork` mechanism (to copy the trace mask), and the central `syscall` dispatcher.

2.  **System Info (`sysinfo`)**: The task was to add a system call that collects information about the running system and returns it to the user. Specifically, it needed to report the amount of free memory (in bytes) and the number of processes whose state is not `UNUSED`. This required traversing kernel data structures (free memory list and process table) and copying the result safely to user space.

## Descriptive Explanation of Solutions

### 1. System Call Tracing (`trace`)
*   **Kernel Modifications**: 
    *   I added a `tracemask` field to the `struct proc` in `proc.h` to store the mask for each process.
    *   In `sys_trace` (implemented in `sysproc.c`), I retrieve the mask argument and store it in the current process's `tracemask`.
    *   I modified `fork` in `proc.c` to copy the `tracemask` from the parent to the child, ensuring tracing propagates to child processes.
    *   I updated the `syscall` function in `syscall.c`. After a system call executes, I check if the corresponding bit in `p->tracemask` is set (using `1 << num`). If it is, I print the PID, the name of the system call (from a new `syscallnames` array), and the return value stored in `p->trapframe->a0`.

### 2. System Info (`sysinfo`)
*   **Memory Counting**: I added a helper function `freemem()` in `kalloc.c`. It iterates through the free list (`kmem.freelist`), counting the number of nodes, and multiplies by `PGSIZE` (4096) to return the total free bytes.
*   **Process Counting**: I added a helper function `nproc()` in `proc.c`. It iterates through the `proc` array and counts processes where `p->state != UNUSED`.
*   **System Call Implementation**:
    *   I created a specific `sysinfo` structure (defined in a header) to hold `freemem` and `nproc`.
    *   In `kernel/sysinfo.c`, I implemented the `systeminfo` helper which calls `freemem()` and `nproc()`, populates the structure, and uses `copyout` to copy the struct to the user-provided virtual address.
    *   `sys_sysinfo` in `sysproc.c` handles the argument parsing and calls `systeminfo`.
