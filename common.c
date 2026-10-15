#include "common.h"

void putchar(char ch);

void printf(const char *fmt, ...) {
  va_list vargs;
  va_start(vargs, fmt);

  while (*fmt) {
    if (*fmt == '%') {
      // Skip to the char following the '%'.
      fmt++;

      switch (*fmt) {

      // '%' was at the end of the fmt string, so it's a literal '%' not an
      // escape character.
      case '\0':
        putchar('%');
        goto end;

      // Print a literal '%'
      case '%':
        putchar('%');
        break;

      // Print a NULL-terminated string.
      case 's': {
        const char *s = va_arg(vargs, const char *);
        while (*s) {
          putchar(*s);
          s++;
        }
        break;
      }

      // Print an integer in decimal.
      case 'd': {
        int value = va_arg(vargs, int);

        // https://github.com/nuta/operating-system-in-1000-lines/issues/64
        unsigned magnitude = value;
        if (value < 0) {
          putchar('-');
          magnitude = -magnitude;
        }

        unsigned divisor = 1;
        while (magnitude / divisor > 9) {
          divisor *= 10;
        }

        while (divisor > 0) {
          putchar('0' + magnitude / divisor);
          magnitude %= divisor;
          divisor /= 10;
        }

        break;
      }

      // Print an integer in hexadecimal.
      case 'x': {
        unsigned value = va_arg(vargs, unsigned);
        for (int i = 7; i >= 0; i--) {
          unsigned nibble = (value >> (i * 4)) & 0xf;
          putchar("0123456789abcdef"[nibble]);
        }
      }
      }
    } else {
      putchar(*fmt);
    }

    fmt++;
  }

end:
  va_end(vargs);
}
