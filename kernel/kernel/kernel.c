/* Free standing libraries 
 * A list of free standing libraries can be found here:
 * https://en.cppreference.com/w/cpp/freestanding
 */

#include <kernel/tty.h>
#include "../arch/i386/vga.h"

#if defined(__linux__)
#error "You are not using a cross-compiler,"
       "you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

/*! @brief Kernel entry point
 *
 * This is the entry point for the kernel. It initializes the terminal
 * and writes a message to the terminal.
 */
void
kernel_main(void)
{
    terminal_initialize();
    terminal_setcolor(VGA_COLOR_LIGHT_BLUE);
    char output = '0';
    while (1)
    {
        output++;
        if (output == '9')
        {
            output = '0';
        }
        terminal_writestring("Hello, kernel World!\n");
        terminal_writestring(&output);
    }
}
