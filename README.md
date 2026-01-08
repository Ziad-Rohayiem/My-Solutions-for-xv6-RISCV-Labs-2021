# Lab: Traps

## Problems / Challenges

This lab explores how code transfers control between user space and kernel space (traps), focusing on stack handling and interrupt processing.

1.  **RISC-V Assembly**: The first part involved analyzing compiled C code to understand RISC-V function calling conventions, stack frames, and register usage.
2.  **Backtrace**: Debugging kernel panics is difficult without knowing the call stack. The challenge was to implement a `backtrace` function that prints the list of function calls (return addresses) leading to the current point of execution.
3.  **Alarm**: The goal was to implement a mechanism to periodically interrupt a user process efficiently. Specifically, implement `sigalarm(interval, handler)` to call a user function every `n` CPU ticks, and `sigreturn()` to resume the original execution flow after the handler returns.

## Descriptive Explanation of Solutions

### 1. RISC-V Assembly
I analyzed `user/call.c` and `user/call.asm`. I determined how arguments are passed (in registers `a0`-`a7`, then on the stack), where the return address is stored, and how the stack frame is constructed. The answers to the specific questions are located in `answers-traps.txt`.

### 2. Backtrace
*   **Implementation**: I added a `backtrace()` function in `kernel/printf.c`.
*   **Logic**:
    *   I retrieved the current frame pointer (`s0` register) using a helper function `r_fp()`.
    *   In RISC-V xv6, the return address is located at offset `-8` from the frame pointer (`fp-8`), and the previous frame pointer is at offset `-16` (`fp-16`).
    *   I looped up the stack chain. To ensure safety and stop at the base of the stack, I checked that the frame pointer remained within the bounds of the current page (`PGROUNDUP(fp) == PGROUNDUP(prev_fp)`).
    *   For each frame, I printed the return address using `%p`.

### 3. Alarm
*   **Process Structure**: I added several fields to `struct proc`:
    *   `alarm_interval`: Tick interval requested by user.
    *   `alarm_handler`: Pointer to the user-space handler function.
    *   `ticks_passed`: Counter for ticks since the last call.
    *   `alarm_in_progress`: Flag to prevent re-entrant calls (calling the handler while it's already running).
    *   `backup_tf`: A `struct trapframe` to save the process state before jumping to the handler.
*   **System Calls**:
    *   `sys_sigalarm`: Sets the `alarm_interval` and `alarm_handler` fields.
    *   `sys_sigreturn`: Restores the original user registers from `backup_tf`, clears `alarm_in_progress`, and resets `ticks_passed` to 0. This effectively resumes the program where it was interrupted.
*   **Trap Handling (`usertrap`)**:
    *   In `kernel/trap.c`, inside the timer interrupt handler, I update the tick counter.
    *   If `ticks_passed` equals `alarm_interval` and no alarm is currently executing (`alarm_in_progress == 0`), I prepare to switch to the handler:
        1.  Save the current `trapframe` (user registers + PC) into `backup_tf`.
        2.  Set the program counter (`epc`) in the trapframe to the address of `alarm_handler`.
        3.  Set `alarm_in_progress` to 1.
    *   When `usertrap` returns, the process resumes execution at the handler function instead of the instruction that was interrupted. The handler (in user space) eventually calls `sigreturn` to restore state.
