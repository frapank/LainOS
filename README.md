# Lain OS
**Lain OS** is my very first 32-bit operating system project made in C. It’s part of my learning journey. I want to see how far I can take it.

## Previews of the OS
| KShell                  |
| ------------------------|
| ![preview4](screen.png) | 

The LainOS kernel runs in **32-bit mode** and is written mainly in simple C with a few Intel assembly sections.
The bootloader is custom and implemented in assembly, and I plan to expand it to be more GRUB-like. It currently supports only **Legacy BIOS**, but my goal is to add **UEFI** boot support in the future.

A growing set of technical documents explaining the internals of the OS can be found here:
* [Documentation index](docs/index.md)
* [How to build](docs/build.md)

### Roadmap
| Component               | Status    | Notes / Dependencies |
| ----------------------- | --------- | -------------------- |
| Bootloader ( Base )     | Done      | — |
| Kernel                  | Done      | — |
| IDT                     | Done      | — |
| ISR / IRQ Manager       | Done      | IDT |
| PIC                     | Done      | IRQ Manager |
| Keyboard & VGA Drivers  | Done      | IRQs |
| Print Driver            | Done      | VGA driver |
| Test Kernel Shell       | Done      | Print Driver |
| Physical Memory Manager | Done      | Boot info |
| Virtual Memory          | InDev     | PMM, page fault handler |
| Memory Allocator        | Not yet   | PMM (used by pt allocation) |
| Timer (PIT/HPET)        | Not yet   | IRQs, needed for scheduler |
| Scheduler               | Not yet   | Timer, Context switcher |
| Context switcher        | Not yet   | Scheduler, task structures |
| Syscall                 | Not yet   | Context switcher, trap entry |
| User Mode               | Not yet   | Syscall, ELF loader, VM |
| ELF loader              | Not yet   | Filesystem or initramfs |
| File System             | Not yet   | Block device / ramfs |
| System Logs             | Not yet   | Print Driver / storage |
| Test user space shell   | Not yet   | User Mode, ELF loader, Init |
| Init System             | Not yet   | Filesystem, User Mode |

## License
This project is licensed under the GNU GPL v3.0 – see the LICENSE file for details.
