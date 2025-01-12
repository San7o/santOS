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

#ifndef _TESTS_TESTS_H
#define _TESTS_TESTS_H

#include <stdint.h>     // uint32_t

#define KASSERT(...)          \
  do                          \
  {                           \
    if (!(__VA_ARGS__))	      \
    {                         \
      return -1;              \
    }                         \ 
  }                           \
  while(0)

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

#define KTEST_END \
    return 0
    
typedef struct {
    uint32_t marker;                   // To validate the Test
    const char* testSuite;
    const char* functionName;
    const char* testName;
    const char* fileName;
    uint32_t lineNumber;
    int (*functionPointer)(void);

} Test;

void tests_main();

#endif  // _TESTS_TESTS_H
