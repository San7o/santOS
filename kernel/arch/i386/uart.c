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

#include <arch/uart.h>
#include <sys/io.h>

#include <stddef.h>      // size_t

int uart_init()
{
   outb(0x00, UART_COM1 + 1);    // Disable all interrupts
   outb(0x80, UART_COM1 + 3);    // Enable DLAB (set baud rate divisor)
   outb(0x03, UART_COM1 + 0);    // Set divisor to 3 (lo byte) 38400 baud
   outb(0x00, UART_COM1 + 1);    //                  (hi byte)
   outb(0x03, UART_COM1 + 3);    // 8 bits, no parity, one stop bit
   outb(0xC7, UART_COM1 + 2);    // Enable FIFO, clear them, with 14-byte
                                 // threshold
   outb(0x0B, UART_COM1 + 4);    // IRQs enabled, RTS/DSR set
   outb(0x1E, UART_COM1 + 4);    // Set in loopback mode, test the serial chip
   outb(0xAE, UART_COM1 + 0);    // Test serial chip (send byte 0xAE and
                                 // check if serial returns same byte)

   // Check if serial is faulty (i.e: not same byte as sent)
   if(inb(UART_COM1 + 0) != 0xAE) {
      return 1;
   }

   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   outb(0x0F, UART_COM1 + 4);
   return 0;
}

int uart_received()
{
   return inb(UART_COM1 + 5) & 1;
}

char uart_serial()
{
   while (uart_received() == 0);

   return inb(UART_COM1);
}

int uart_is_transmit_empty()
{
   return inb(UART_COM1 + 5) & 0x20;
}

void uart_write(char a)
{
   while (uart_is_transmit_empty() == 0);
   outb(a, UART_COM1);
   return;
}

void uart_write_string(const char* s, size_t size)
{
  for (size_t i = 0; i < size; ++i)
  {
    uart_write(s[i]);
  }
  return;
}
