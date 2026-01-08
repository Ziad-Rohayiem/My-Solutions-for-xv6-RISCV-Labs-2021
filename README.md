# Lab: Copy-on-Write (COW) Fork

## Problems / Challenges

The challenge in this lab was to optimize the `fork()` system call using the Copy-on-Write (COW) technique.
1.  **Inefficient Forking**: The original xv6 `fork()` eagerly copies all user memory from the parent to the child. This is wasteful, especially if the child calls `exec()` shortly after, discarding the copied memory.
2.  **Implementation Complexity**: Implementing COW requires careful management of physical memory. Multiple processes (parent and children) might share the same physical page. The kernel must ensure a page is only freed when *no* process needs it anymore.
3.  **Lazy Allocation**: Writing to a shared read-only page triggers a page fault. The kernel must handle this exception transparently, allocating a new page and resolving the fault without killing the process.

## Explanation of the Solution

### 1. Reference Counting
To manage shared physical pages, I implemented a reference counting mechanism in `kernel/kalloc.c`:
*   **Structure**: I added a global array `refcnt.count[]` protected by a `spinlock` to track the number of references to each physical page.
*   **Modifications**:
    *   `kalloc()`: Initializes the reference count of a newly allocated page to 1.
    *   `kfree()`: Decrements the reference count. It only actually frees the physical memory (adds it back to the free list) when the count reaches 0.
    *   `krefpage()`: A new helper function that increments the reference count of a page. This is called when a page is shared (e.g., during `fork`).

### 2. VM Modifications (uvmcopy)
I modified `uvmcopy` in `kernel/vm.c`, which is used by `fork` to duplicate process memory:
*   Instead of allocating new space and `memmove`-ing data, I map the child's virtual pages to the **same physical pages** as the parent.
*   I disable write access (`PTE_W`) on both the parent's and child's PTEs.
*   I set a dedicated `PTE_COW` bit (RSW bit in RISC-V PTE) to mark these pages as Copy-on-Write.
*   I call `krefpage()` for each shared physical page to reflect the new reference.

### 3. Page Fault Handling (COW Handler)
I modified `usertrap` in `kernel/trap.c` to handle page faults (scause 15):
*   **Detection**: When a "Store page fault" occurs, I check if it corresponds to a COW page (valid PTE with `PTE_COW` set).
*   **Resolution (`cowhandler`)**:
    *   I allocate a new physical page using `kalloc()`.
    *   I copy the contents of the shared (read-only) page to this new page.
    *   I update the faulty process's page table to point to the new page.
    *   I set `PTE_W` (writeable) and clear `PTE_COW` on the new mapping.
    *   I call `kfree()` (or a decrement helper) on the old physical page, as the current process no longer shares it.
*   **Result**: The process retries the instruction, finds a writable private page, and proceeds, unaware that the allocation happened lazily.
