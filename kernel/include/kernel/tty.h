#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

#define VGA_ADDRESS 0xB8000        /* The VGA buffer starts here     */

extern const size_t VGA_WIDTH;
extern const size_t VGA_HEIGHT;

extern size_t terminal_row;        /* Current row of the terminal    */
extern size_t terminal_column;     /* Current column of the terminal */
extern uint8_t terminal_color;
extern uint16_t* terminal_buffer;

void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
int terminal_write(char* data, size_t size);
int terminal_writestring(char* data);
void terminal_initialize(void);

#endif
