#include <stdio.h>

int putk(const char *s)
{
  (void) s;
  return 1;
}
int printk(const char *restrict format, ...)
{
  (void) format;
  return 1;
}
