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

#ifndef _BOOT_MULTIBOOT_H
#define _BOOT_MULTIBOOT_H

#include <stdint.h>

/*
The following is from the grub multiboot documentation:

Upon entry to the operating system, the EBX register contains the
physical address of a Multiboot information data structure, through
which the boot loader communicates vital information to the operating
system.

The Multiboot information structure and its related substructures may
be placed anywhere in memory by the boot loader (with the exception
of the memory reserved for the kernel and boot modules, of course).
It is the operating system's responsibility to avoid overwriting this
memory until it is done using it.

The format of the Multiboot information structure (as defined so far)
follows: 
*/
typedef struct multiboot_info
{
  uint32_t flags;

  uint32_t mem_lower;        // present if flags[0] is set
  uint32_t mem_upper;        // present if flags[0] is set

  uint32_t boot_device;      // present if flags[1] is set

  uint32_t cmdline;          // present if flags[2] is set

  uint32_t mods_count;       // present if flags[3] is set
  uint32_t mods_addr;        // present if flags[3] is set

  uint32_t syms[32];         // present if flags[4] or flags[5]
                             // is set
  uint32_t mmap_length;      // present if flags[6] is set
  uint32_t mmap_addr;        // present if flags[6] is set
  
  uint32_t drivers_length;   // present if flags[7] is set
  uint32_t drivers_addr;     // present if flags[7] is set

  uint32_t config_table;     // present if flags[8] is set

  uint32_t boot_loader_name; // present if flags[9] is set

  uint32_t apm_table;        // present if flags[10] is set

  uint32_t vbe_control_info; // present if flags[11] is set
  uint32_t vbe_mode_info;
  uint16_t vbe_mode;
  uint16_t vbe_interface_seg;
  uint16_t vbe_interface_off;
  uint16_t vbe_interface_len;
  
  uint64_t framebuffer_addr; // present if flags[12] is set
  uint32_t framebuffer_pitch;
  uint32_t framebuffer_width;
  uint32_t framebuffer_heigth;
  uint32_t bpp_type_color_info;  // 1 bit for framebuffer_bpp
                                 // 1 bit for framebuffer_type
                                 // 5 bits for color_info
};

char* multiboot_cmdline(struct multiboot_info* info);

#endif  // _KERNEL_MULTIBOOT_H
