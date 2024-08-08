/* Free standing libraries 
 * A list of free standing libraries can be found here:
 * https://en.cppreference.com/w/cpp/freestanding
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "You are not using a cross-compiler,"
       "you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

/* VGA text mode buffer
 * A character on the screen is represented by a
 * 16-bit value, with the following layout:
 *
 * <--------------------------- 16 bits ---------------------------->
 * <--- 8 bits --->|<--- 4 bits --->|<--- 3 bits --->|<--- 1 bit --->
 * =============== | ============== | ============== | =============
 * |     char      |      fg        |       bg       | Blinking bit |
 * =============== | ============== | ============== | =============
 * ^
 * 0
 */

/* Hardware text mode color constants */
enum vga_color
{
    VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t
vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t
vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;        /* Current row of the terminal */
size_t terminal_column;     /* Current column of the terminal */
uint8_t terminal_color;
uint16_t* terminal_buffer;

/* @brief Set the terminal color
 *
 * This function sets the terminal color to the given color.
 *
 * @param color The color to set the terminal to
 */
void
terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

/* @brief Put a character on the terminal
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
void
terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

/* @brief Put a character on the terminal
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
void
terminal_putchar(char c)
{
    if (c == '\n')
    {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
        {
            terminal_row = 0;
        }
        return;
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH)
    {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
        {
            terminal_row = 0;
        }
    }
}

/* @brief Write a string to the terminal
 *
 * This function writes a string to the terminal by looping
 * through each character in the string and calling terminal_putchar
 * on each character.
 *
 * @param data The string to write to the terminal
 */
void
terminal_write(char* data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        terminal_putchar(data[i]);
    }
}

/* @brief Write a string to the terminal
 *
 * This function writes a string to the terminal by looping
 * through each character in the string and calling terminal_putchar
 * on each character.
 *
 * @param data The string to write to the terminal
 */
void
terminal_writestring(char* data)
{
    terminal_write(data, strlen(data));
}

/* @brief Initialize the terminal
 *
 * This function initializes the terminal by setting the row and column
 * to 0, the color to light grey on black, and the buffer to the VGA buffer
 * address. It loops through the buffer and sets each character to a space
 * with the default color.
 */
void
terminal_initialize(void)
{
    terminal_row    = 0;
    terminal_column = 0;
    terminal_color  = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;

    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putentryat(' ', terminal_color, x, y);
        }
    }
}

/* @brief Kernel entry point
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
