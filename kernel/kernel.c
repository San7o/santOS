//#include <stdio.h>

#include "kernel/tty.h"

/* @brief Kernel entry point
 *
 * This is the entry point for the kernel. It initializes the terminal
 * and writes a message to the terminal.
 */
void
kernel_main(void)
{
    terminal_initialize();
    terminal_writestring("Hello, kernel World!\n");
}
