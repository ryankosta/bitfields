#ifndef PANIC_H
#define PANIC_H
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
__attribute__((noreturn))
static inline void panic(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	printf("PANIC: ");
	vprintf(fmt, args);

	va_end(args);

	exit(1);  // Exit with error code 1
}
#endif
