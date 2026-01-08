# xv6 RISC-V Labs Solutions (2021)

Welcome to my repository of solutions for the MIT 6.S081 / Fall 2020 Operating Systems Engineering labs (xv6-labs-2021).

## 📂 Repository Structure

**Crucial Note**: The `main` branch is empty of code. Each lab assignment is isolated in its own **branch**. You must switch branches to view the code, run tests, or execute the operating system.

The available branches are:

*   **`util`**: Lab 1 - Unix Utilities (`sleep`, `pingpong`, `primes`, `find`, `xargs`)
*   **`syscall`**: Lab 2 - System Calls (`trace`, `sysinfo`)
*   **`pgtbl`**: Lab 3 - Page Tables (`vmprint`, `pgaccess`, speedup)
*   **`traps`**: Lab 4 - Traps (`backtrace`, `sigalarm`/`sigreturn`)
*   **`cow`**: Lab 5 - Copy-on-Write Fork (virtual memory optimization)

## 🚀 How to Use / Navigate

1.  **Clone the Repository**:
    ```bash
    git clone https://github.com/Ziad-Rohayiem/My-Solutions-for-xv6-RISCV-Labs-2021.git
    cd My-Solutions-for-xv6-RISCV-Labs-2021
    ```

2.  **Switch to a Lab Branch**:
    To see the code and run the OS for a specific lab (e.g., `util`), use `git checkout`:
    ```bash
    git checkout util
    ```
    *Replace `util` with `syscall`, `pgtbl`, `traps`, or `cow` as needed.*

## 🛠️ Running the Labs

Once you have checked out a specific branch (e.g., `git checkout util`), you will have access to the source code and the `Makefile`. You can then run the following commands:

*   **Build and Run xv6 (QEMU)**:
    Compiles the kernel and starts the QEMU emulator.
    ```bash
    make qemu
    ```

*   **Grade Your Solution**:
    Runs the automated grading script to verify your solution against the lab requirements.
    ```bash
    make grade
    ```

*   **Clean Build Artifacts**:
    Removes generated files (`.o`, `.img`, binaries) to force a fresh build.
    ```bash
    make clean
    ```

### 💡 What is QEMU?
**QEMU** (Quick Emulator) is a generic and open-source machine emulator and virtualizer. In this project, it simulates a **RISC-V computer** so we can run our xv6 operating system kernel on standard hardware (like your x86 laptop) without needing a physical RISC-V board.

### ⌨️ Useful QEMU Shortcuts
When running xv6 inside QEMU, the terminal captures your input. Here are essential shortcuts:

*   **Exit QEMU**: Press `Ctrl + A`, release, then press `x`.
*   **Show Help**: Press `Ctrl + A`, release, then press `h`.
*   **Switch to Monitor**: Press `Ctrl + A`, release, then press `c` (allows you to inspect registers/memory).
