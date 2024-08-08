#include "kernel/tty.h"
#include "vga.h"

// strlen
// TODO: Remove when libc is done
uint16_t
strlen(const char* str)
{
    uint16_t len = 0;
    while (str[len])
    {
        len++;
    }
    return len;
}

void
terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void
terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

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

void
terminal_write(char* data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        terminal_putchar(data[i]);
    }
}

void
terminal_writestring(char* data)
{
    terminal_write(data, strlen(data));
}

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

