/*
 * trace.h
 *
 *  Created on: 18 мар. 2020 г.
 *      Author: moskvin
 */

#ifndef INCLUDE_TRACE_H_
#define INCLUDE_TRACE_H_

#include <unistd.h>

#if defined(TRACE)
#if defined(__cplusplus)
extern "C"
{
#endif
	// Not used yet
	void trace_initialize(void);

	// Implementation dependent
	ssize_t trace_write(const char* buf, size_t nbyte);

	// Portable
	int trace_printf(const char* format, ...);
	int trace_puts(const char *s);
	int trace_putchar(int c);
	void trace_dump_args(int argc, char* argv[]);
#if defined(__cplusplus)
}
#endif

#else // !defined(TRACE)

#if defined(__cplusplus)
extern "C"
{
#endif
	inline void trace_initialize(void);
	// Implementation dependent
	inline ssize_t trace_write(const char* buf, size_t nbyte);
	inline int trace_printf(const char* format, ...);
	inline int trace_puts(const char *s);
	inline int trace_putchar(int c);
	inline void trace_dump_args(int argc, char* argv[]);
#if defined(__cplusplus)
}
#endif
inline void __attribute__((always_inline)) trace_initialize(void)
{
}

// Empty definitions when trace is not defined
inline ssize_t __attribute__((always_inline)) trace_write(const char* buf __attribute__((unused)), size_t nbyte __attribute__((unused)))
{
	return 0;
}

inline int __attribute__((always_inline)) trace_printf(const char* format __attribute__((unused)), ...)
{
	return 0;
}

inline int __attribute__((always_inline)) trace_puts(const char *s __attribute__((unused)))
{
	return 0;
}

inline int __attribute__((always_inline)) trace_putchar(int c)
{
	return c;
}

inline void __attribute__((always_inline)) trace_dump_args(int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
}

#endif // defined(TRACE)

#endif /* INCLUDE_TRACE_H_ */
