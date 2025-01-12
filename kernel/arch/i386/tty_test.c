/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <kernel/tty.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ktest.h>

#include "vga.h"

KTEST(tty, terminal_setcolor)
{
  // Setup
  uint8_t terminal_color_old = terminal_color;

  // Test 1
  terminal_setcolor(VGA_COLOR_BLUE);
  KASSERT(terminal_color == VGA_COLOR_BLUE);

  // Test 2
  terminal_setcolor(VGA_COLOR_GREEN);
  KASSERT(terminal_color == VGA_COLOR_GREEN);

  // Clenup
  terminal_setcolor(terminal_color_old);
  KTEST_END;
}

KTEST(tty, terminal_putentryat)
{
  terminal_putentryat('a', VGA_COLOR_BLUE, VGA_WIDTH, VGA_HEIGHT);
  KASSERT(terminal_buffer[VGA_HEIGHT * VGA_WIDTH + VGA_WIDTH] ==
	  vga_entry('a', VGA_COLOR_BLUE));
  KTEST_END;
}

KTEST(tty, terminal_putchar)
{
  KTEST_END;
}

KTEST(tty, terminal_write)
{
  KTEST_END;
}

KTEST(tty, terminal_writestring)
{
  KTEST_END;
}

KTEST(tty, terminal_initialize)
{
  KTEST_END;
}
