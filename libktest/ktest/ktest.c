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

/*
  This file contains the main test runner. The runner is compiled
  only if tests were enabled in the build systems and It is called by
  the main kernel function if the parameter "test" was provided
  during boot time.
*/

#include <ktest.h>
#include <kernel/tty.h>
#include <stdio.h>
#include <stdbool.h>

extern Test __start_utest_records[];
extern Test __stop_utest_records[];

#define GREEN 2
#define RED 4

void tests_main()
{
  terminal_setcolor(GREEN);

  Test* current = __start_utest_records;
  printk("Number of tests: %d\n", __stop_utest_records - __start_utest_records);
  while (current < __stop_utest_records)
  {
    if (current->marker == 0xDeadBeaf)
    {
      printk("Running test: %s\n", current->testName);
      int ret = current->functionPointer();       // Execute the test.
      if (ret != 0)
      {
        terminal_setcolor(RED);
        printk("Test Failed: %s\n", current->testName);
        terminal_setcolor(GREEN);
      }
    }
    current++;
  }

  return;
}
