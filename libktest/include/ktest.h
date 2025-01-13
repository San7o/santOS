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

/* =================================================================*\
 *                                                                   *
 * libktest is the testing framework of the kernel. Test cases are   *
 * registered with the macro KTEST which will be run by test main.   *
 * The runner is compiled only if tests were enabled in the build    *
 * systems and It is called by the main kernel function if the       *
 * parameter "test" was provided during boot time.                   *
 *                                                                   *
\* =================================================================*/

#ifndef _KTEST_KTEST_H
#define _KTEST_KTEST_H

#include <stdint.h>     // for uint32_t

// Assertions

#define KASSERT(...)          \
  do                          \
  {                           \
    if (!(__VA_ARGS__))	      \
    {                         \
      return -1;              \
    }                         \ 
  }                           \
  while(0)

// Register a test case, It should end with KTEST_END
// Thanks Sam P.
#define KTEST(suiteName, uTtestName)                           \
    static int suiteName##_##uTtestName(void);                 \
    static Test Record_##suiteName##_##uTtestName              \
        __attribute__((section(".utest_records"), used)) = {   \
        .marker = 0xDeadBeaf,                                  \
        .testSuite = #suiteName,                               \
        .functionName = #uTtestName,                           \
        .testName = #suiteName "_" #uTtestName,	               \
        .fileName = __FILE__,                                  \
        .lineNumber = __LINE__,                                \
        .functionPointer = suiteName##_##uTtestName            \
    };                                                         \
    static int suiteName##_##uTtestName(void)

// Add this at the end of a test case
#define KTEST_END \
    return 0

// Each test will create this struct in the section .utest_records
typedef struct {
    uint32_t marker;                   // To validate the Test
    const char* testSuite;
    const char* functionName;
    const char* testName;
    const char* fileName;
    uint32_t lineNumber;
    int (*functionPointer)(void);

} Test;

// Run all the tests
void tests_main();

#endif  // _TESTS_TESTS_H
