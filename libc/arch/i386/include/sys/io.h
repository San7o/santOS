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

#ifndef _LIBC_SYS_IO_H
#define _LIBC_SYS_IO_H

#include <stdint.h>

#if __STDC_HOSTED__ == 0  // Freestanding

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

/* TODO
static inline uint8_t inb_p(uint16_t port);
static inline uint16_t inw(uint16_t port);
static inline uint16_t inw_p(uint16_t port);
static inline unsigned int inl(uint16_t port);
static inline unsigned int inl_p(uint16_t port);
*/

static inline void outb(uint8_t val, uint16_t port)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
    // There's an outb %al, $imm8 encoding, for compile-time constant
    // port numbers that fit in 8b. (N constraint). Wider immediate
    // constants would be truncated at assemble-time (e.g. "i" constraint).
    // The  outb  %al, %dx  encoding is the only option for all other cases.
    // %1 expands to %dx because  port  is a uint16_t.  %w1 could be
    // used if we had the port number a wider C type
}

/* TODO
static inline void outb_p(uint8_t value, uint16_t port);
static inline void outw(uint16_t value, uint16_t port);
static inline void outw_p(uint16_t value, uint16_t port);
static inline void outl(unsigned int value, uint16_t port);
static inline void outl_p(unsigned int value, uint16_t port);

static inline void insb(uint16_t port, void* addr,
                        unsigned long count);
static inline void insw(uint16_t port, void* addr,
                        unsigned long count);
static inline void insl(uint16_t port, void* addr,
                        unsigned long count);
static inline void outsb(uint16_t port, const void* addr,
                         unsigned long count);
static inline void outsw(uint16_t port, const void* addr,
                         unsigned long count);
static inline void outsl(uint16_t port, const void* addr,
                         unsigned long count);
*/

#else // Hosted

// Hosted should have the same functions but with a different
// implementation. I will leave this for later.

#endif //both

// Nothing here to see...

#endif // _LIBC_SYS_IO_H
