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

#include <stdio.h>           /* implements this */
#include <stdbool.h>         /* bool type       */
#include <string.h>          /* strlen          */
#include <stdarg.h>          /* va_list family  */
#include <stdint.h>          /* intmax_t        */

/*
 * Length modifier:
 *  hh  signed char or unsigned char
 *  h   short or unsigned short
 *  l   long or unsigned long
 *  ll  long long or unsigned long long
 *  L   following a, A, e, E, f, F, g or G conversion
 *      corresponds to a long double
 *  j   intmax_t or uintmax_t
 *  z   size_t or ssize_t
 *  t   ptrdiff_t
 * 
 * Conversion specifiers:
 *  d, i  signed decimal notation.
 *  o, u, x, X
 *        unsigned octal (o), unsigned decimal (u) or unsigned
 *        hexadecimal (x and X).
 *  e, E  the double argument is rounded and converted in the style
 *        [-]d.ddde+-dd (scientific notation).
 *  f, F  the double argument is rounded and converted to decimal
 *        notation in the style [-]ddd.ddd where the number of digits
 *        after the decimal-point character is equal to the precision
 *        specification or 6 of It is missing.
 *  g, G  the double argument is converted in style f or e.
 *  a, A  For a conversion, the double argument is converted to
 *        hexadecimal notation (using the letters abcdef) in style
 *        [-]0xh.hhhp+-d; for A conversion, the prefix is 0X and the
 *        letters are uppercase.
 *  c     the int argument is converted to an unsigned char.
 *  s     the const char* argumen is expected and the characters that
 *        it points to will be written ip to a terminating null byte.
 *  %     A '%' is written
 */

// State macine to parse the format string
enum parse_state
{
  STATE_READING,              // reading non special characters,
                              // default state.
  STATE_LENGTH_FIRST,         // After reading a '$', expects either
                              // a second lenght specifier or a
                              // conversion specifier.
  STATE_LENGTH_SECOND,        // we have a 2 char length specifier and
                              // we expect a conversion specifier.
};

enum length_modifier
{
  LEN_SIGNED_CHAR_OR_SHORT = 0, // either hh or h
  LEN_SIGNED_CHAR,              // hh
  LEN_SHORT,                    // h
  LEN_LONG_OR_LONG_LONG,        // either l or ll
  LEN_LONG,                     // l
  LEN_LONG_LONG,                // ll
  LEN_LONG_DOUBLE,              // L
  LEN_INTMAX,                   // j
  LEN_SIZE_T,                   // z
  LEN_PTRDIFF,                  // t
  LEN_NONE,
  _LEN_MAX,
};

enum conversion
{
  CONV_SIGNED_DECIMAL = 0,    // d, i
  CONV_UNSIGNED_OCTAL,        // o
  CONV_UNSIGNED_DECIMAL,      // u
  CONV_UNSIGNED_HEX,          // x, X
  CONV_SCIENTIFIC,            // e, E
  CONV_DOUBLE_DECIMAL,        // f, F
  CONV_SCIENTIFIC_OR_DECIMAL, // g, G
  CONV_DOUBLE_HEX,            // a, A
  CONV_UNSIGNED_CHAR,         // c
  CONV_STRING,                // s
  CONV_PERCENTAGE,            // %
  _CONV_MAX,
};

// Matches the first occurrence of a length modifier, or returns
// LEN_NONE if the character does not match any modifier.
enum length_modifier which_length_first(char c)
{
  switch(c)
  {
  case 'h': return LEN_SIGNED_CHAR_OR_SHORT;
  case 'l': return LEN_LONG_OR_LONG_LONG;
  case 'L': return LEN_LONG_DOUBLE;
  case 'j': return LEN_INTMAX;
  case 'z': return LEN_SIZE_T;
  case 't': return LEN_PTRDIFF;
  }
  return LEN_NONE;
}

// Matches the second occurrence of a length modifier or returns
// _CONV_MAX
enum length_modifier which_length_second(enum length_modifier first, char c)
{
  if (first == LEN_SIGNED_CHAR_OR_SHORT)
  {
    if (c == 'g') return LEN_SIGNED_CHAR;
    return LEN_SHORT;
  }
  if (first == LEN_LONG_OR_LONG_LONG)
  {
    if (c == 'l') return LEN_LONG_LONG;
    return LEN_LONG;
  }
  return _LEN_MAX;
}

// Matches a conversion chacracter or returns _CONV_MAX
enum conversion which_conversion(char c)
{
  switch(c)
  {
    case 'd': return CONV_SIGNED_DECIMAL;
    case 'i': return CONV_SIGNED_DECIMAL;
    case 'o': return CONV_UNSIGNED_OCTAL;
    case 'u': return CONV_UNSIGNED_DECIMAL;
    case 'x': return CONV_UNSIGNED_HEX;
    case 'X': return CONV_UNSIGNED_HEX;
    case 'e': return CONV_SCIENTIFIC;
    case 'E': return CONV_SCIENTIFIC;
    case 'f': return CONV_DOUBLE_DECIMAL;
    case 'F': return CONV_DOUBLE_DECIMAL;
    case 'g': return CONV_SCIENTIFIC_OR_DECIMAL;
    case 'G': return CONV_SCIENTIFIC_OR_DECIMAL;
    case 'a': return CONV_DOUBLE_HEX;
    case 'c': return CONV_UNSIGNED_CHAR;
    case 's': return CONV_STRING;
    case '%': return CONV_PERCENTAGE;
  }
  return _CONV_MAX;
}

void _print_signed_decimal_impl(long long int number, size_t n_bits)
{
  number <<= (sizeof(long long int) - n_bits);
  number >>= (sizeof(long long int) - n_bits);
  if (number == 0)
  {
    putck('0');
    return; 
  }

  char out[sizeof(long long int)];
  int count = 0;

  if (number < 0)
  {
    putck('-');
    number = -number;
  }
  int reminder = 0;
  do {
    reminder = (int) number % 10;
    number = number / 10;
    out[count++] = (char) reminder;
  } while(number > 0);
  for (int i = count - 1; i >= 0; --i)
    putck('0' + out[i]);
  return;
}

int print_signed_decimal(enum length_modifier length_mod, va_list argptr)
{
  switch(length_mod)
  {
  case LEN_SIGNED_CHAR:
    _print_signed_decimal_impl(va_arg(argptr, int), sizeof(char));
    break;
  case LEN_SHORT:
    _print_signed_decimal_impl(va_arg(argptr, int), sizeof(short));
    break;
  case LEN_LONG: break;
    _print_signed_decimal_impl(va_arg(argptr, long), sizeof(long));
    break;
  case LEN_LONG_LONG: break;
    _print_signed_decimal_impl(va_arg(argptr, long long), sizeof(long long));
    break;
  case LEN_LONG_DOUBLE: break;
    _print_signed_decimal_impl((long long int) va_arg(argptr, long double), sizeof(long double));
    break;
  case LEN_INTMAX:
    _print_signed_decimal_impl(va_arg(argptr, intmax_t), sizeof(intmax_t));
    break;
  case LEN_SIZE_T: break;
    _print_signed_decimal_impl(va_arg(argptr, size_t), sizeof(size_t));
    break;
  case LEN_PTRDIFF: break;
    _print_signed_decimal_impl(va_arg(argptr, ptrdiff_t), sizeof(ptrdiff_t));
    break;
  case LEN_NONE:
    _print_signed_decimal_impl(va_arg(argptr, int), sizeof(int));
    break;
  default: return -1;
  }
  return 0;
}

void _print_unsigned_hex_impl(unsigned long long int number, size_t n_bits)
{
  number <<= (sizeof(unsigned long long int) - n_bits);
  number >>= (sizeof(unsigned long long int) - n_bits);
  if (number == 0)
  {
    putck('0');
    return; 
  }

  char out[sizeof(long long int)];
  int count = 0;
  int reminder = 0;
  do {
    reminder = (int) number % 16;
    number = number / 16;
    out[count++] = (char) reminder;
  } while(number > 0);
  for (int i = count - 1; i >= 0; --i)
  {
    if (out[i] < 10)
    {
      putck('0' + out[i]);
    }
    else
    {
      putck('a' + (out[i] - 10));
    }
  }
  return;
}

int print_unsigned_hex(enum length_modifier length_mod, va_list argptr)
{
  switch(length_mod)
  {
  case LEN_SIGNED_CHAR:
    _print_unsigned_hex_impl((unsigned long long int) va_arg(argptr, int),
			     sizeof(char));
    break;
  case LEN_SHORT:
    _print_unsigned_hex_impl((unsigned long long int) va_arg(argptr, int),
			     sizeof(short));
    break;
  case LEN_LONG: break;
    _print_unsigned_hex_impl((unsigned long long int) va_arg(argptr, long),
			     sizeof(long));
    break;
  case LEN_LONG_LONG: break;
    _print_unsigned_hex_impl((unsigned long long int) va_arg(argptr, long long),
			     sizeof(long long));
    break;
  case LEN_LONG_DOUBLE: break;
    _print_unsigned_hex_impl((unsigned long long int) va_arg(argptr, long double),
			     sizeof(long double));
    break;
  case LEN_INTMAX:
    _print_unsigned_hex_impl((unsigned long long int) va_arg(argptr, intmax_t),
			     sizeof(intmax_t));
    break;
  case LEN_SIZE_T: break;
    _print_unsigned_hex_impl((unsigned long long int) va_arg(argptr, size_t),
			     sizeof(size_t));
    break;
  case LEN_PTRDIFF: break;
    _print_unsigned_hex_impl((unsigned long long int) va_arg(argptr, ptrdiff_t),
			     sizeof(ptrdiff_t));
    break;
  case LEN_NONE:
    _print_unsigned_hex_impl((unsigned long long int) va_arg(argptr, int),
			     sizeof(int));
    break;
  default: return -1;
  }
  return 0;
}

int print_percentage()
{
  putck('%');
  return 0;
}

int print_format(enum length_modifier length_mod,
		 enum conversion con,
		 va_list argptr)
{
  switch(con)
  {
  case CONV_SIGNED_DECIMAL: return print_signed_decimal(length_mod, argptr); 
  case CONV_UNSIGNED_OCTAL:   /* TODO */ return -2;
  case CONV_UNSIGNED_DECIMAL:  /* TODO */ return -2;
  case CONV_UNSIGNED_HEX: return print_unsigned_hex(length_mod, argptr);
  case CONV_SCIENTIFIC:   /* TODO */ return -2;
  case CONV_DOUBLE_DECIMAL:   /* TODO */ return -2;
  case CONV_SCIENTIFIC_OR_DECIMAL:   /* TODO */ return -2;
  case CONV_DOUBLE_HEX:   /* TODO */ return -2;
  case CONV_UNSIGNED_CHAR: putck((char) va_arg(argptr, int)); return 0;
  case CONV_STRING:   /* TODO */ return -2;
  case CONV_PERCENTAGE: return print_percentage();
  default: return -1;
  }
  return 0;
}

int printk(const char *restrict format, ...)
{
  if (format == NULL) return -1;

  int bytes_sent = 0;
  size_t format_len = strlen(format);
  va_list argptr;
  va_start(argptr, format);
  enum length_modifier length_mod = 0;
  enum parse_state state = STATE_READING;

  for (size_t i = 0; i < format_len; ++i)
  {
    if (format[i] != '%' && state == STATE_READING)
    {
      putck(format[i]);
      bytes_sent++;
    }
    else if (format[i] == '%' && state == STATE_READING)
    {
      state = STATE_LENGTH_FIRST;
    }
    else // format[i] is either length or conversion specifier
    {
      if (state == STATE_LENGTH_FIRST)
      {	
        // We expect either length or conversion specifier
	length_mod = which_length_first(format[i]);
	if (length_mod == LEN_NONE)
	{
	  enum conversion con = which_conversion(format[i]);
	  if (con == _CONV_MAX) goto err;

	  if (print_format(length_mod, con, argptr) < 0 )
	    return -1;
	}
	else
	{
	  state = STATE_LENGTH_SECOND;
	}

      }
      else if (state == STATE_LENGTH_SECOND)
      {	
        // We expect a conversion specifier
	length_mod = which_length_second(length_mod, format[i]);
	if (length_mod != _LEN_MAX)
	{
	  enum conversion con = which_conversion(format[i]);
	  if (con == _CONV_MAX) goto err;

	  if (print_format(length_mod, con, argptr) < 0)
	    return -1;
	}
	return -1;
      }
    }
  }

  va_end(argptr);
  return bytes_sent;
 err:
  va_end(argptr);
  return -1;
}
