#include "kernel.h"
#include "common.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

/// These values are set by the linker.
/// __bss and __bss_end are a pair, and store the range of addresses holding
/// variables which have statically-assigned memory locations.
/// __stack_top is the top of the stack.
extern char __bss[], __bss_end[], __stack_top[];

/// Make a call to SBI.
struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid) {
  register long a0 __asm__("a0") = arg0;
  register long a1 __asm__("a1") = arg1;
  register long a2 __asm__("a2") = arg2;
  register long a3 __asm__("a3") = arg3;
  register long a4 __asm__("a4") = arg4;
  register long a5 __asm__("a5") = arg5;
  register long a6 __asm__("a6") = fid;
  register long a7 __asm__("a7") = eid;

  __asm__ __volatile__("ecall"
                       : "=r"(a0), "=r"(a1)
                       : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
                         "r"(a6), "r"(a7)
                       : "memory");
  return (struct sbiret){.error = a0, .value = a1};
}

void putchar(char ch) {
  // Console putchar.
  sbi_call(ch, 0, 0, 0, 0, 0, 0, 1);
}

void kernel_main(void) {
  // Initialise all statically allocated variables to zero.
  // Some bootloaders do this automatically (Q: Which ones?)
  memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);

  printf("\n\nHello %s\n", "World!");

  PANIC("Aaaaaah!");

  printf("1 + 2 = %d, %x\n", 1 + 2, 0x1234abcd);

  // Waiting for interrupt causes the kernel to pause execution, which is a bit
  // better than forever running a jump operation.
  for (;;) {
    __asm__ __volatile__("wfi");
  }
}

/// Program entrypoint.
/// boot is placed at the start of the program in the .text.boot section to
/// because that's where OpenSBI jumps by default.
__attribute__((section(".text.boot")))
// Don't decorate boot with implicit instructions such as a return.
// The function contains the inline assembly and nothing but.
__attribute__((naked)) void boot(void) {
  __asm__ __volatile__(
      // Set the stack pointer
      "mv sp, %[stack_top]\n"

      // Jump to the kernel main function
      "j kernel_main\n"
      :
      : [stack_top] "r"(
          __stack_top) // Pass the stack top address as %[stack_top]
  );
}
