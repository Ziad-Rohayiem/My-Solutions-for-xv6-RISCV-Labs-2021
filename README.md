# Lab: Unix Utilities

## Problems / Challenges

This lab introduces the xv6 operating system and its system calls. The goal is to implement several standard Unix utilities to get familiar with the process management and file system APIs. The specific challenges were:

1.  **sleep**: Implement a program that pauses for a user-specified number of ticks.
2.  **pingpong**: Create a program where a parent process sends a byte to a child process via a pipe, the child prints "ping", sends the byte back, and the parent prints "pong".
3.  **primes**: Implement a concurrent version of the Sieve of Eratosthenes using pipes. This requires creating a dynamic pipeline of processes where each process filters out multiples of a specific prime.
4.  **find**: Write a program that searches a directory tree for files with a specific name. It needs to handle directory scaling and recursion.
5.  **xargs**: Implement a simpler version of the UNIX xargs command, which reads lines from standard input and runs a command for each line, appending the line as an argument.

## Descriptive Explanation of Solutions

### 1. Sleep
The solution is straightforward. I verify that the user provided an argument. I convert the string argument to an integer using `atoi` and then pass it to the `sleep` system call. If no argument is provided, I print an error message.

### 2. Pingpong
I used two pipes to establish bidirectional communication:
- `p1`: Parent -> Child
- `p2`: Child -> Parent

The flow is:
1.  Create both pipes.
2.  `fork()` to create a child.
3.  **Parent**: Writes a byte to `p1[1]`, then blocks reading from `p2[0]`.
4.  **Child**: Reads the byte from `p1[0]`, prints `<pid>: received ping`, writes the byte to `p2[1]`, and exits.
5.  **Parent**: Wakes up after reading from `p2[0]`, prints `<pid>: received pong`, and exits.

### 3. Primes
I implemented a generative pipeline using a recursive approach (or loop-based logic simulating recursion):
- **Idea**: The main process generates numbers 2 through 35 and feeds them into a pipe.
- **Process Logic**: Each process reads the first number from its input pipe. This number is a prime. It prints `prime <n>`.
- **Filtering**: It then reads subsequent numbers. If a number is *not* divisible by its prime, it passes it to the next stage.
- **Recursion**: If it has numbers to pass but no next stage exists yet, it creates a new pipe and forks a new child process to handle the next stage of filtering. This creates a chain of processes `2 -> 3 -> 5 -> 7 ...`.

### 4. Find
I implemented a recursive function `find(path, expression)`:
- It opens the file/directory at `path`.
- If it's a file, it checks if the filename matches `expression`.
- If it's a directory, it iterates through the directory entries using `read` and `struct dirent`.
- For every entry (except `.` and `..`), it constructs the full path string and recursively calls `find`.
- Memory manipulation (copying paths to buffers) was handled carefully to ensure null-termination and correct path formatting.

### 5. Xargs
The solution involves parsing standard input and managing process execution:
- I read from `stdin` char-by-char to build a line buffer.
- When a newline (`\n`) is encountered, I treat the accumulated buffer as a new argument.
- I construct a new `argv` array consisting of the initial arguments passed to `xargs` plus the new argument read from input.
- I `fork` a child process. The child calls `exec` with the constructed arguments to run the command.
- The parent waits for the child to finish before processing the next line.
