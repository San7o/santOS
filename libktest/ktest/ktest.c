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

#include <ktest.h>
#include <arch/uart.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

extern Test __start_utest_records[];
extern Test __stop_utest_records[];

void tests_main()
{
  const char* start_message = "Running tests...\n";
  printk("%s", start_message);
  uart_write_string(start_message, strlen(start_message));

  uart_init();

  Test* current = __start_utest_records;
  while (current < __stop_utest_records)
  {
    if (current->marker == 0xDeadBeaf)
    {
      const char* test_message = "[test] ";
      printk("%s%s\n", test_message, current->testName);
      uart_write_string(test_message, strlen(test_message));
      uart_write_string(current->testName, strlen(current->testName));
      uart_write('\n');

      int ret = current->functionPointer();       // Execute the test.
      if (ret != 0)
      {
	const char* fail_message = "Test Failed: ";
	printk("%s%s\n", fail_message, current->testName);
        uart_write_string(fail_message, strlen(fail_message));
	uart_write_string(current->testName, strlen(current->testName));
	uart_write('\n');
      }
    }
    current++;
  }

  const char* done_message = "Done\n";
  uart_write_string(done_message, strlen(done_message));
  printk("Done\n");

  return;
}
