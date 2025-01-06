#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

#define VGA_ADDRESS 0xB8000        /* The VGA buffer starts here     */

extern size_t terminal_row;        /* Current row of the terminal    */
extern size_t terminal_column;     /* Current column of the terminal */
extern uint8_t terminal_color;
extern uint16_t* terminal_buffer;

/*! @brief Set the terminal color
 *
 * This function sets the terminal color to the given color.
 *
 * @param color The color to set the terminal to
 */
void terminal_setcolor(uint8_t color);

/*! @brief Put a character on the terminal
 *
 * This function puts a character on the terminal at the current
 * row and column. If the character is a newline, it will move
 * the cursor to the next line. If the character is a backspace,
 * it will move the cursor back one space. Otherwise, it will
 * put the character on the screen and move the cursor to the
 * next column.
 *
 * @param c The character to put on the terminal
 */
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);

/*! @brief Put a character on the terminal
 *
 * This function puts a character on the terminal at the current
 * row and column. If the character is a newline, it will move
 * the cursor to the next line. If the character is a backspace,
 * it will move the cursor back one space. Otherwise, it will
 * put the character on the screen and move the cursor to the
 * next column.
 *
 * @param c The character to put on the terminal
 */
void terminal_putchar(char c);

/*!
 * @brief Write a string to the terminal
 * @param data The string to write to the terminal
 */
int terminal_write(char* data, size_t size);

/*! @brief Write a string to the terminal
 *
 * This function writes a string to the terminal by looping
 * through each character in the string and calling terminal_putchar
 * on each character.
 *
 * @param data The string to write to the terminal
 */
int terminal_writestring(char* data);

/*! @brief Initialize the terminal
 *
 * This function initializes the terminal by setting the row and column
 * to 0, the color to light grey on black, and the buffer to the VGA buffer
 * address. It loops through the buffer and sets each character to a space
 * with the default color.
 */
void terminal_initialize(void);

#endif
