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
 * all copies or substantial portions of the Software.
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
#include <stdio.h>
#include <string.h>
#include <boot/multiboot.h>

#ifdef KERNEL_TEST
#include <ktest.h>
#endif

#if defined(__linux__)
#error "You are not using a cross-compiler,"
       "you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

#define GREEN 2  // vga terminal

void kernel_main(struct multiboot_info* info)
{
    terminal_initialize();
    terminal_setcolor(GREEN);
  
    char* cmdline = multiboot_cmdline(info);
    if (cmdline != NULL)
    {
      if (strstr((char*) info->cmdline, "test\0") != NULL)
      {
	#ifdef KERNEL_TEST
	tests_main();
	#else
        printk("Error: the kernel was not compiled with tests\n");
	#endif
	return;
      }
    }

    printk("Hello World!");
    return;
}
