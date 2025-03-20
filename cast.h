// MIT License
// 
// Copyright (c) 2025 P. Czarnota
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef include_cast_h_library
#define include_cast_h_library

/**
 * <p align="center">
 * <img src="cast.svg" alt="A logo portraying a magic wand" /><br />
 * Single header, type conversions library
 * </p>
 *
 * Motivation
 * ----------
 *
 * Proper conversion between types is a tedious task.
 *
 * - Some conversions result in a loss of data, implementation defined behavior
 *   or even undefined behavior ([nasal deamons](https://en.wikipedia.org/wiki/Undefined_behavior)).
 * - Converting between types using casting operators `(type)`,
 *   does not convey a clear the intention of the programmer. Is the cast done,
 *   because the value is guaranteed to fit destination type? Is it done
 *   because truncating the data is expected?
 * - Casting operators silence compiler warnings about conversions. If there is
 *   a conversion `(uint8_t)var`, where `var` is `uint32_t`, and in future the
 *   code is changed, so that type of `var` becomes `float`, most likely the code
 *   where conversion occurs should be reviewed again, but it can be overlooked,
 *   because an expression which uses such casting operator usually does not
 *   produce warnings.
 * - Manually writing conditions to check if value fits a given range can be
 *   error prone.
 *
 * This library of helper functions was created to make dealing with these problems easier.
 *
 * And let's be honest. The world just needs an another C single header library.
 *
 *
 * Installation
 * ------------
 *
 * `cast.h` is a
 * [STB-style](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt)
 * single header library, because such libraries are easier to integrate into
 * projects.
 *
 * Just drop in `cast.h` directly into your project and create a `.c` file with
 * the following contents:
 *
 * ```c
 * // a translation unit where you want to implement the cast library
 * #define CAST_IMPLEMENTATION
 * #include "cast.h"
 * ```
 *
 * Once you've done that, you can include `cast.h` in other translation units
 * and start using conversion functions.
 *
 * ```c
 * // in other translation units where you want to use conversion functions
 * #include "cast.h"
 * ```
 *
 * For a more in depth explanation refer to <https://github.com/nothings/stb/blob/master/docs/stb_howto.txt>
 *
 * Usage
 * -----
 *
 * The library provides two kinds of functions:
 *
 * - **Error returning functions** - these functions are in form of
 *   `try_{T'}_from_{U'}(T* dst, U src)` and they return an error on failure.
 *   These functions can be used in cases where value is not guaranteed to
 *   fit the destination type and you want to provide error handling for cases
 *   where it does not fit.
 *
 * - **Panic handler invoking functions** - these functions are in form of
 *   `{T'}_from_{U'}(U src)` and they invoke the panic handler on failure.
 *   The default behavior of the panic handler is to call `exit(1)` and crash the
 *   application. Such functions should be used in cases where conversion is
 *   expected to succeed and failure is a unexpected bug or security error,
 *   which shall lead to immediate program termination.
 *
 * In order to make function conversion names shorter and more memorable,
 * the `T'` and `U'` are short versions of the type
 * `T` and `U` type names respectively. Here is a full list of supported types:
 *
 *- `T`: `uint8_t`, `T'`: `u8`
 *- `T`: `uint16_t`, `T'`: `u16`
 *- `T`: `uint32_t`, `T'`: `u32`
 *- `T`: `uint64_t`, `T'`: `u64`
 *- `T`: `int8_t`, `T'`: `i8`
 *- `T`: `int16_t`, `T'`: `i16`
 *- `T`: `int32_t`, `T'`: `i32`
 *- `T`: `int64_t`, `T'`: `i64`
 *- `T`: `short`, `T'`: `short`
 *- `T`: `int`, `T'`: `int`
 *- `T`: `long`, `T'`: `long`
 *- `T`: `long long`, `T'`: `llong`
 *- `T`: `unsigned short`, `T'`: `ushort`
 *- `T`: `unsigned int`, `T'`: `uint`
 *- `T`: `unsigned long`, `T'`: `ulong`
 *- `T`: `unsigned long long`, `T'`: `ullong`
 *- `T`: `bool`, `T'`: `bool`
 *- `T`: `float`, `T'`: `float`
 *- `T`: `double`, `T'`: `double`
 *- `T`: `size_t`, `T'`: `size`
 *- `T`: `ptrdiff_t`, `T'`: `ptrdiff`
 *- `T`: `uintptr_t`, `T'`: `uptr`
 *
 * ### Error returning functions
 *
 * To convert from `U` type to `T` type use:
 *
 * ```c
 * // Try to convert `src` of type U to type T stored in `dst` and return
 * // 0 on success and non-zero value on failure. In the case of error `dst`
 * // is left unmodified
 * int try_{T'}_from_{U'}(T *dst, U src);
 * ```
 *
 * Example:
 *
 * ```c
 * void foo(int x, short y)
 * {
 * 	uint32_t dst_u32 = 0U;
 * 	int err = try_u32_from_int(&dst_u32, x);
 * 	if (err) {
 * 		// Handle error
 * 	}
 * 	printf("%"PRNu32"\n", dst_u32);
 *
 * 	size_t count = 0U;
 * 	int err = try_size_from_short(&count, y);
 * 	if (err) {
 * 		// Handle error
 * 	}
 * 	printf("%zu\n", count);
 * }
 * ```
 *
 * ### Panic handler invoking functions
 *
 * To convert from `U` type to `T`, without error handling:
 *
 * ```c
 * // Convert `src` of type U to type T and return converted value
 * // If the conversion is not possible to do without loss of data / precision
 * // then the panic handler is invoked (default behaviors is to crash the application).
 * T {T'}_from_{U'}(U src);
 * ```
 *
 * Example:
 *
 * ```c
 * void print_numbers(int *numbers, int count)
 * {
 * 	if (!numbers)
 * 		return;
 * 	// Program will crash if count is < 0
 * 	for (size_t i = 0; i < size_from_int(count); ++i)
 * 		printf("%d\n", numbers[i]);
 * }
 * ```
 *
 * ### Custom panic handler
 *
 * You can overwrite the default panic handler (which just calls `exit(1)`),
 * by defining `CAST_CUSTOM_PANIC` constant before including `cast.h`.
 *
 * ```c
 * #define CAST_IMPLEMENTATION
 * #define CAST_CUSTOM_PANIC
 * #include "cast.h"
 *
 * void cast_panic_impl(const char *msg, ...)
 * {
 * 	printf("your custom panic handler\n");
 * 	exit(1);
 * }
 * ```
 *
 * If you define a panic handler that does not exit the application,
 * then `{T'}_from_{U'}()` when error occurrs those functions will return zero
 * initialized values instead of crashing application.
 *
 * ### Casting integer types
 *
 * Casting integer types will check if destination type
 * can represent the source value. It will return error or invoke
 * panic handler if the source value can't be represented by destination type.
 * This also means that an error will be triggered when you try to convert a negative
 * number to unsigned type.
 *
 * Example of casting unsinged types to unsigned types:
 *
 * ```c
 * // Will panic if x can't fit size_t
 * size_t count = size_from_u32(x);
 *
 * // Will return error if x can't fit size_t
 * if (try_size_from_u32(&count, x)) {
 * 	// handle error
 * }
 * ```
 *
 * Example of casting singed types to unsigned types:
 *
 * ```c
 * // Will panic if x is too large or is negative
 * size_t count = size_from_int(x);
 *
 * // Will return error if x is too large or is negative
 * if (try_size_from_int(&count, x)) {
 * 	// handle error
 * }
 * ```
 *
 * Example of casting unsinged types to signed types:
 *
 * ```c
 * // Will panic if x is too large
 * short number = short_from_size(x);
 *
 * // Will return error if x is too large
 * if (try_short_from_size(&number, x)) {
 * 	// handle error
 * }
 * ```
 *
 * Example of casting singed types to signed types:
 *
 * ```c
 * // Will panic if x is too large or too small
 * int16_t count = i16_from_int(x);
 *
 * // Will return error if x is too large or too small
 * if (try_i16_from_int(&count, x)) {
 * 	// handle error
 * }
 * ```
 *
 * ### Casting integer types to floating point types
 *
 * Casting an integer type to floating point type will only succeed if the integer
 * type can be represented exactly by the the floating point type without any precision loss.
 *
 * For example, this operation will succeed:
 *
 * ```c
 * int64_t x = 1234;
 * float variable = float_from_int(x);
 * ```
 *
 * This operation however, will invoke the panic handler, because it results in
 * unexpected precision loss
 * ```c
 * int64_t x = 0xFFFFFFFF; // 32 ones
 * float var = 0.0f;
 * // error, because float's mantissa has only 24 bits of precision (with implicit bit)
 * if (try_float_from_i64(&var, x)) {
 * 	// handle error
 * }
 *
 * // panic handler will be invoked
 * var = float_from_int(x);
 * ```
 *
 * Note that there are integers larger than `0xFFFFFFFF` that can fit `float`, for
 * example `0xF00000000` is a larger number, but it requires less bits to be accurately
 * represented by the `float` type, because trailing zeros will be represented
 * by using larger exponent.
 *
 * ```c
 * float ok = float_from_u64(0xF00000000ULL); // no error
 * ```
 *
 * Analoguous functions exists for `double` and they allow more integers due
 * to the fact that `double` has 54 bits of mantissa (with implicit bit).
 *
 * ```c
 * double ok = double_from_u32(0xFFFFFFFFUL);
 * double also_ok = double_from_u64(0xFFFFFFFFFFFF0000ULL);
 * double not_ok = double_from_u64(0xFFFFFFFFFFFFFFFFULL); // error
 * ```
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <math.h>

typedef uintmax_t cast_largest_utype;

/**
 * Return floating point upper limit of integer type expressed using min
 * constant for that integer type.
 *
 * This assumes integer implementation is using two's complement.
 *
 * @param float_type     Floating type of the result.
 * @param min            Minimum value of integer type.
 *
 * @return Upper limit of integer type expressed in floating point value.
 */
#define CAST_SIGNED_UPPER_LIMIT(float_type, min) (-(float_type)(min))

/**
 * Return floating point upper limit for unsigned integer type.
 *
 * @param float_type     Floating type of the result.
 * @param type           Unsigned integer type.
 *
 * @return Upper limit of integer type expressed in floating point value.
 */
#define CAST_UNSIGNED_UPPER_LIMIT(float_type, type)                            \
	((float_type)2.0 *                                                     \
	 (float_type)(((type)0x1) << (sizeof(type) * 8U - 1U)))

/**
 * Invoke panic handler.
 *
 * @param format   Printf-style format string.
 */
void cast_panic_impl(const char *format, ...)
#if defined(__GNU_C__) || defined(__clang__)
    __attribute__((format(printf, 1, 2)))
#endif
    ;
#define cast_panic(msg, ...)                                                   \
	cast_panic_impl("cast: panic in %s():%d: " msg "\n", __func__,         \
			__LINE__, __VA_ARGS__)

/**
 * Shift all least significant zeros right.
 *
 * @param value    Value for which zeros are to be removed.
 *
 * @return value with all least significant zeros shifted away.
 */
static inline cast_largest_utype
cast_shift_zeros_right(cast_largest_utype value)
{
#if defined(__GNU_C__) || defined(__clang__)
	return value > 0U ? value >> (size_t)__builtin_ctzll(value) : 0U;
#else
	if ((value & 0xFFFFFFFFU) == 0U)
		value >>= 32U;
	if ((value & 0xFFFFU) == 0U)
		value >>= 16U;
	if ((value & 0xFFU) == 0U)
		value >>= 8U;
	if ((value & 0xFU) == 0U)
		value >>= 4U;
	if ((value & 0x3U) == 0U)
		value >>= 2U;
	if ((value & 0x1U) == 0U)
		value >>= 1U;

	return value;
#endif
}

/**
 * This is wrapper for strtoull(), but with easier error handling.
 * 
 * @param dst   Pointer to variable, where conversion result will be stored.
 * @param str   NULL-terminated string to convert to unsigned long long.
 *
 * @return 0 on success, non-zero on failure.
 */
int cast_try_ullong_from_str(unsigned long long *dst, const char *str);

/**
 * This is wrapper for strtoll(), but with easier error handling.
 *
 * @param dst   Pointer to variable, where conversion result will be stored.
 * @param str   NULL-terminated string to convert to long long.
 *
 * @return 0 on success, non-zero on failure.
 */
int cast_try_llong_from_str(long long *dst, const char *str);

/**
 * Define a wrapper conversion function, which will trigger panic handler
 * if conversion can't be performed.
 *
 * @param dst_type         Destination type.
 * @param dst_type_name    Destination type name.
 * @param src_type         Source type.
 * @param dst_type_name    Source type name.
 */
#define CAST_DEFINE_FROM(dst_type, dst_type_name, src_type, src_type_name)     \
	static inline dst_type dst_type_name##_from_##src_type_name(           \
	    src_type src)                                                      \
	{                                                                      \
		dst_type tmp = 0;                                              \
		if (try_##dst_type_name##_from_##src_type_name(&tmp, src)) {   \
			cast_panic("failed to convert %s to %s",               \
				   #src_type_name, #dst_type_name);            \
			return tmp;                                            \
		}                                                              \
		return tmp;                                                    \
	}

/**
 * Define a conversion function for signed to unsigned conversions.
 *
 * @param dst_type         Destination type.
 * @param dst_type_name    Destination type name.
 * @param dst_type_max     Maximum value that fits destination type.
 * @param src_type         Source type.
 * @param dst_type_name    Source type name.
 */
#define CAST_DEFINE_TRY_U_FROM_S(dst_type, dst_type_name, dst_max, src_type,   \
				 src_type_name)                                \
	static inline int try_##dst_type_name##_from_##src_type_name(          \
	    dst_type *dst, src_type src)                                       \
	{                                                                      \
		if (dst == NULL || src < 0 ||                                  \
		    (cast_largest_utype)src > (cast_largest_utype)dst_max) {   \
			return -1;                                             \
		}                                                              \
		*dst = (dst_type)src;                                          \
		return 0;                                                      \
	}                                                                      \
	CAST_DEFINE_FROM(dst_type, dst_type_name, src_type, src_type_name)

/**
 * Define a conversion function for unsigned to unsigned conversions.
 *
 * @param dst_type         Destination type.
 * @param dst_type_name    Destination type name.
 * @param dst_type_max     Maximum value that fits destination type.
 * @param src_type         Source type.
 * @param dst_type_name    Source type name.
 */
#define CAST_DEFINE_TRY_U_FROM_U(...) CAST_DEFINE_TRY_U_FROM_S(__VA_ARGS__)

/**
 * Define a conversion function for signed to unsigned conversions.
 *
 * @param dst_type         Destination type.
 * @param dst_type_name    Destination type name.
 * @param dst_type_max     Maximum value that fits destination type.
 * @param src_type         Source type.
 * @param dst_type_name    Source type name.
 */
#define CAST_DEFINE_TRY_S_FROM_U(...) CAST_DEFINE_TRY_U_FROM_S(__VA_ARGS__)

/**
 * Define a conversion function for signed to signed conversions.
 *
 * @param dst_type         Destination type.
 * @param dst_type_name    Destination type name.
 * @param dst_min          Minimum value that fits destination type.
 * @param dst_max          Maximum value that fits destination type.
 * @param src_type         Source type.
 * @param dst_type_name    Source type name.
 */
#define CAST_DEFINE_TRY_S_FROM_S(dst_type, dst_type_name, dst_min, dst_max,    \
				 src_type, src_type_name)                      \
	static inline int try_##dst_type_name##_from_##src_type_name(          \
	    dst_type *dst, src_type src)                                       \
	{                                                                      \
		if (dst == NULL || src < dst_min || src > dst_max) {           \
			return -1;                                             \
		}                                                              \
		*dst = (dst_type)src;                                          \
		return 0;                                                      \
	}                                                                      \
	CAST_DEFINE_FROM(dst_type, dst_type_name, src_type, src_type_name)

/**
 * Define a conversion function for unsigned from string conversions.
 *
 * @param dst_type         Destination type.
 * @param dst_type_name    Destination type name.
 */
#define CAST_DEFINE_TRY_U_FROM_STR(dst_type, dst_type_name)                    \
	static inline int try_##dst_type_name##_from_str(dst_type *dst,        \
							 const char *str)      \
	{                                                                      \
		unsigned long long tmp = 0U;                                   \
		int ret = cast_try_ullong_from_str(&tmp, str);                 \
		if (ret)                                                       \
			return -1;                                             \
		return try_##dst_type_name##_from_ullong(dst, tmp);            \
	} \
	CAST_DEFINE_FROM(dst_type, dst_type_name, const char *, str)

/**
 * Define a conversion function for signed from string conversions.
 *
 * @param dst_type         Destination type.
 * @param dst_type_name    Destination type name.
 */
#define CAST_DEFINE_TRY_S_FROM_STR(dst_type, dst_type_name)                    \
	static inline int try_##dst_type_name##_from_str(dst_type *dst,        \
							 const char *str)      \
	{                                                                      \
		long long tmp = 0U;                                            \
		int ret = cast_try_llong_from_str(&tmp, str);                  \
		if (ret)                                                       \
			return -1;                                             \
		return try_##dst_type_name##_from_llong(dst, tmp);             \
	}                                                                      \
	CAST_DEFINE_FROM(dst_type, dst_type_name, const char *, str)

/**
 * Define a conversion function for signed to floating point conversions.
 *
 * @param dst_type         Destination type.
 * @param dst_type_name    Destination type name.
 * @param mantissa_bits    Number of mantissa bits
 * @param src_type         Source type.
 * @param src_type_name    Source type name.
 * @param src_min          Minimum value that fits source type.
 */
#define CAST_DEFINE_TRY_F_FROM_S(dst_type, dst_type_name, mantissa_bits,       \
				 src_type, src_type_name, src_min)             \
	static inline int try_##dst_type_name##_from_##src_type_name(          \
	    dst_type *dst, src_type src)                                       \
	{                                                                      \
		if (!dst)                                                      \
			return -1;                                             \
		if (sizeof(src) < sizeof(*dst)) {                              \
			*dst = (dst_type)src;                                  \
			return 0;                                              \
		}                                                              \
		/* Prune UB from `-(src)` operation. */                        \
		if (src == src_min) {                                          \
			*dst = (dst_type)src;                                  \
			return 0;                                              \
		}                                                              \
		cast_largest_utype relevant = cast_shift_zeros_right(          \
		    (cast_largest_utype)(src < 0 ? -src : src));               \
		if (relevant > (1ULL << mantissa_bits) - 1ULL)                 \
			return -1;                                             \
		*dst = (dst_type)src;                                          \
		return 0;                                                      \
	}                                                                      \
	CAST_DEFINE_FROM(dst_type, dst_type_name, src_type, src_type_name)

/**
 * Define a conversion function for unsigned to floating point conversions.
 *
 * @param dst_type         Destination type.
 * @param dst_type_name    Destination type name.
 * @param mantissa_bits    Number of mantissa bits
 * @param src_type         Source type.
 * @param src_type_name    Source type name.
 */
#define CAST_DEFINE_TRY_F_FROM_U(dst_type, dst_type_name, mantissa_bits,       \
				 src_type, src_type_name)                      \
	static inline int try_##dst_type_name##_from_##src_type_name(          \
	    dst_type *dst, src_type src)                                       \
	{                                                                      \
		if (!dst)                                                      \
			return -1;                                             \
		if (sizeof(src) < sizeof(*dst)) {                              \
			*dst = (dst_type)src;                                  \
			return 0;                                              \
		}                                                              \
		cast_largest_utype relevant = cast_shift_zeros_right(src);     \
		if (relevant > (1ULL << mantissa_bits) - 1ULL)                 \
			return -1;                                             \
		*dst = (dst_type)src;                                          \
		return 0;                                                      \
	}                                                                      \
	CAST_DEFINE_FROM(dst_type, dst_type_name, src_type, src_type_name)

/**
 * Define a conversion function for floating point to unsigned conversions.
 *
 * @param dst_type         Destination type.
 * @param dst_type_name    Destination type name.
 * @param src_type         Source type.
 * @param src_type_name    Source type name.
 */
#define CAST_DEFINE_TRY_U_FROM_F(dst_type, dst_type_name, src_type,            \
				 src_type_name)                                \
	static inline int try_##dst_type_name##_from_##src_type_name(          \
	    dst_type *dst, src_type src)                                       \
	{                                                                      \
		if (!dst)                                                      \
			return -1;                                             \
		const double src_upper = CAST_UNSIGNED_UPPER_LIMIT(double,     \
								   dst_type);  \
		double trunced_src = trunc(src);                               \
		if (trunced_src < 0.0 || src_upper <= trunced_src)             \
			return -1;                                             \
		*dst = (dst_type)trunced_src;                                  \
		return 0;                                                      \
	}                                                                      \
	CAST_DEFINE_FROM(dst_type, dst_type_name, src_type, src_type_name)

/**
 * Define a conversion function for floating point to signed conversions.
 *
 * @param dst_type         Destination type.
 * @param dst_type_name    Destination type name.
 * @param src_type         Source type.
 * @param src_type_name    Source type name.
 */
#define CAST_DEFINE_TRY_S_FROM_F(dst_type, dst_type_name, dst_min, src_type,   \
				 src_type_name)                                \
	static inline int try_##dst_type_name##_from_##src_type_name(          \
	    dst_type *dst, src_type src)                                       \
	{                                                                      \
		if (!dst)                                                      \
			return -1;                                             \
		const double src_upper = CAST_SIGNED_UPPER_LIMIT(double,       \
								 dst_min);     \
		const double src_min = (double)(dst_min);                      \
		double trunced_src = trunc(src);                               \
		if (trunced_src < src_min || src_upper <= trunced_src)         \
			return -1;                                             \
		*dst = (dst_type)trunced_src;                                  \
		return 0;                                                      \
	}                                                                      \
	CAST_DEFINE_FROM(dst_type, dst_type_name, src_type, src_type_name)

/**
 * Define a family of functions for conversions to unsigned type.
 *
 * @param dst_type         Destination type.
 * @param dst_type_name    Destination type name.
 * @param dst_type_max     Maximum value that can fit desintation type.
 */
#define CAST_DEFINE_TRY_U(dst_type, dst_type_name, dst_type_max)               \
	/* From signed */                                                      \
	CAST_DEFINE_TRY_U_FROM_S(dst_type, dst_type_name, dst_type_max,        \
				 int8_t, i8)                                   \
	CAST_DEFINE_TRY_U_FROM_S(dst_type, dst_type_name, dst_type_max,        \
				 int16_t, i16)                                 \
	CAST_DEFINE_TRY_U_FROM_S(dst_type, dst_type_name, dst_type_max,        \
				 int32_t, i32)                                 \
	CAST_DEFINE_TRY_U_FROM_S(dst_type, dst_type_name, dst_type_max,        \
				 int64_t, i64)                                 \
	CAST_DEFINE_TRY_U_FROM_S(dst_type, dst_type_name, dst_type_max, int,   \
				 int)                                          \
	CAST_DEFINE_TRY_U_FROM_S(dst_type, dst_type_name, dst_type_max, short, \
				 short)                                        \
	CAST_DEFINE_TRY_U_FROM_S(dst_type, dst_type_name, dst_type_max, long,  \
				 long)                                         \
	CAST_DEFINE_TRY_U_FROM_S(dst_type, dst_type_name, dst_type_max,        \
				 long long, llong)                             \
	CAST_DEFINE_TRY_U_FROM_S(dst_type, dst_type_name, dst_type_max,        \
				 ptrdiff_t, ptrdiff)                           \
	/* From unsigned */                                                    \
	CAST_DEFINE_TRY_U_FROM_U(dst_type, dst_type_name, dst_type_max,        \
				 uint8_t, u8)                                  \
	CAST_DEFINE_TRY_U_FROM_U(dst_type, dst_type_name, dst_type_max,        \
				 uint16_t, u16)                                \
	CAST_DEFINE_TRY_U_FROM_U(dst_type, dst_type_name, dst_type_max,        \
				 uint32_t, u32)                                \
	CAST_DEFINE_TRY_U_FROM_U(dst_type, dst_type_name, dst_type_max,        \
				 uint64_t, u64)                                \
	CAST_DEFINE_TRY_U_FROM_U(dst_type, dst_type_name, dst_type_max,        \
				 unsigned, uint)                               \
	CAST_DEFINE_TRY_U_FROM_U(dst_type, dst_type_name, dst_type_max,        \
				 unsigned short, ushort)                       \
	CAST_DEFINE_TRY_U_FROM_U(dst_type, dst_type_name, dst_type_max,        \
				 unsigned long, ulong)                         \
	CAST_DEFINE_TRY_U_FROM_U(dst_type, dst_type_name, dst_type_max,        \
				 unsigned long long, ullong)                   \
	CAST_DEFINE_TRY_U_FROM_U(dst_type, dst_type_name, dst_type_max,        \
				 size_t, size)                                 \
	CAST_DEFINE_TRY_U_FROM_U(dst_type, dst_type_name, dst_type_max,        \
				 uintptr_t, uptr)                              \
	/* From floating point */                                              \
	CAST_DEFINE_TRY_U_FROM_F(dst_type, dst_type_name, float, float)        \
	CAST_DEFINE_TRY_U_FROM_F(dst_type, dst_type_name, double, double)      \
	/* From str */                                                         \
	CAST_DEFINE_TRY_U_FROM_STR(dst_type, dst_type_name)                    \
	/* END */

/**
 * Define a family of functions for conversions to signed type.
 *
 * @param dst_type         Destination type.
 * @param dst_type_name    Destination type name.
 * @param dst_type_max     Maximum value that can fit desintation type.
 */
#define CAST_DEFINE_TRY_S(dst_type, dst_type_name, dst_type_max)               \
	/* From signed */                                                      \
	CAST_DEFINE_TRY_S_FROM_S(dst_type, dst_type_name, dst_type_max##_MIN,  \
				 dst_type_max##_MAX, int8_t, i8)               \
	CAST_DEFINE_TRY_S_FROM_S(dst_type, dst_type_name, dst_type_max##_MIN,  \
				 dst_type_max##_MAX, int16_t, i16)             \
	CAST_DEFINE_TRY_S_FROM_S(dst_type, dst_type_name, dst_type_max##_MIN,  \
				 dst_type_max##_MAX, int32_t, i32)             \
	CAST_DEFINE_TRY_S_FROM_S(dst_type, dst_type_name, dst_type_max##_MIN,  \
				 dst_type_max##_MAX, int64_t, i64)             \
	CAST_DEFINE_TRY_S_FROM_S(dst_type, dst_type_name, dst_type_max##_MIN,  \
				 dst_type_max##_MAX, int, int)                 \
	CAST_DEFINE_TRY_S_FROM_S(dst_type, dst_type_name, dst_type_max##_MIN,  \
				 dst_type_max##_MAX, short, short)             \
	CAST_DEFINE_TRY_S_FROM_S(dst_type, dst_type_name, dst_type_max##_MIN,  \
				 dst_type_max##_MAX, long, long)               \
	CAST_DEFINE_TRY_S_FROM_S(dst_type, dst_type_name, dst_type_max##_MIN,  \
				 dst_type_max##_MAX, long long, llong)         \
	CAST_DEFINE_TRY_S_FROM_S(dst_type, dst_type_name, dst_type_max##_MIN,  \
				 dst_type_max##_MAX, ptrdiff_t, ptrdiff)       \
	/* From unsigned */                                                    \
	CAST_DEFINE_TRY_S_FROM_U(dst_type, dst_type_name, dst_type_max##_MAX,  \
				 uint8_t, u8)                                  \
	CAST_DEFINE_TRY_S_FROM_U(dst_type, dst_type_name, dst_type_max##_MAX,  \
				 uint16_t, u16)                                \
	CAST_DEFINE_TRY_S_FROM_U(dst_type, dst_type_name, dst_type_max##_MAX,  \
				 uint32_t, u32)                                \
	CAST_DEFINE_TRY_S_FROM_U(dst_type, dst_type_name, dst_type_max##_MAX,  \
				 uint64_t, u64)                                \
	CAST_DEFINE_TRY_S_FROM_U(dst_type, dst_type_name, dst_type_max##_MAX,  \
				 unsigned, uint)                               \
	CAST_DEFINE_TRY_S_FROM_U(dst_type, dst_type_name, dst_type_max##_MAX,  \
				 unsigned short, ushort)                       \
	CAST_DEFINE_TRY_S_FROM_U(dst_type, dst_type_name, dst_type_max##_MAX,  \
				 unsigned long, ulong)                         \
	CAST_DEFINE_TRY_S_FROM_U(dst_type, dst_type_name, dst_type_max##_MAX,  \
				 unsigned long long, ullong)                   \
	CAST_DEFINE_TRY_S_FROM_U(dst_type, dst_type_name, dst_type_max##_MAX,  \
				 size_t, size)                                 \
	CAST_DEFINE_TRY_S_FROM_U(dst_type, dst_type_name, dst_type_max##_MAX,  \
				 uintptr_t, uptr)                              \
	/* From floating point */                                              \
	CAST_DEFINE_TRY_S_FROM_F(dst_type, dst_type_name, dst_type_max##_MIN,  \
				 float, float)                                 \
	CAST_DEFINE_TRY_S_FROM_F(dst_type, dst_type_name, dst_type_max##_MIN,  \
				 double, double)                               \
	/* From str */                                                         \
	CAST_DEFINE_TRY_S_FROM_STR(dst_type, dst_type_name)                    \
	/* END */

/**
 * Define a family of functions for conversions to floating point type.
 *
 * @param dst_type         Destination type.
 * @param dst_type_name    Destination type name.
 * @param mantissa_bits    Number of mantissa bits.
 */
#define CAST_DEFINE_TRY_F(dst_type, dst_type_name, mantissa_bits)              \
	/* From signed */                                                      \
	CAST_DEFINE_TRY_F_FROM_S(dst_type, dst_type_name, mantissa_bits,       \
				 int8_t, i8, INT8_MIN)                         \
	CAST_DEFINE_TRY_F_FROM_S(dst_type, dst_type_name, mantissa_bits,       \
				 int16_t, i16, INT16_MIN)                      \
	CAST_DEFINE_TRY_F_FROM_S(dst_type, dst_type_name, mantissa_bits,       \
				 int32_t, i32, INT32_MIN)                      \
	CAST_DEFINE_TRY_F_FROM_S(dst_type, dst_type_name, mantissa_bits,       \
				 int64_t, i64, INT64_MIN)                      \
	CAST_DEFINE_TRY_F_FROM_S(dst_type, dst_type_name, mantissa_bits, int,  \
				 int, INT_MIN)                                 \
	CAST_DEFINE_TRY_F_FROM_S(dst_type, dst_type_name, mantissa_bits,       \
				 short, short, SHRT_MIN)                       \
	CAST_DEFINE_TRY_F_FROM_S(dst_type, dst_type_name, mantissa_bits, long, \
				 long, LONG_MIN)                               \
	CAST_DEFINE_TRY_F_FROM_S(dst_type, dst_type_name, mantissa_bits,       \
				 long long, llong, LLONG_MIN)                  \
	CAST_DEFINE_TRY_F_FROM_S(dst_type, dst_type_name, mantissa_bits,       \
				 ptrdiff_t, ptrdiff, PTRDIFF_MIN)              \
	/* From unsigned */                                                    \
	CAST_DEFINE_TRY_F_FROM_U(dst_type, dst_type_name, mantissa_bits,       \
				 uint8_t, u8)                                  \
	CAST_DEFINE_TRY_F_FROM_U(dst_type, dst_type_name, mantissa_bits,       \
				 uint16_t, u16)                                \
	CAST_DEFINE_TRY_F_FROM_U(dst_type, dst_type_name, mantissa_bits,       \
				 uint32_t, u32)                                \
	CAST_DEFINE_TRY_F_FROM_U(dst_type, dst_type_name, mantissa_bits,       \
				 uint64_t, u64)                                \
	CAST_DEFINE_TRY_F_FROM_U(dst_type, dst_type_name, mantissa_bits,       \
				 unsigned int, uint)                           \
	CAST_DEFINE_TRY_F_FROM_U(dst_type, dst_type_name, mantissa_bits,       \
				 unsigned short, ushort)                       \
	CAST_DEFINE_TRY_F_FROM_U(dst_type, dst_type_name, mantissa_bits,       \
				 unsigned long, ulong)                         \
	CAST_DEFINE_TRY_F_FROM_U(dst_type, dst_type_name, mantissa_bits,       \
				 unsigned long long, ullong)                   \
	CAST_DEFINE_TRY_F_FROM_U(dst_type, dst_type_name, mantissa_bits,       \
				 size_t, size)                                 \
	/* From str */                                                         \
	CAST_DEFINE_TRY_S_FROM_STR(dst_type, dst_type_name)                    \
	/* END */

CAST_DEFINE_TRY_U(uint8_t, u8, UINT8_MAX)
CAST_DEFINE_TRY_U(uint16_t, u16, UINT16_MAX)
CAST_DEFINE_TRY_U(uint32_t, u32, UINT32_MAX)
CAST_DEFINE_TRY_U(uint64_t, u64, UINT64_MAX)
CAST_DEFINE_TRY_U(unsigned, uint, UINT_MAX)
CAST_DEFINE_TRY_U(unsigned short, ushort, USHRT_MAX)
CAST_DEFINE_TRY_U(unsigned long, ulong, ULONG_MAX)
CAST_DEFINE_TRY_U(unsigned long long, ullong, ULLONG_MAX)
CAST_DEFINE_TRY_U(size_t, size, SIZE_MAX)
CAST_DEFINE_TRY_U(uintptr_t, uptr, UINTPTR_MAX)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wtautological-constant-out-of-range-compare"
CAST_DEFINE_TRY_S(int8_t, i8, INT8)
CAST_DEFINE_TRY_S(int16_t, i16, INT16)
CAST_DEFINE_TRY_S(int32_t, i32, INT32)
CAST_DEFINE_TRY_S(int64_t, i64, INT64)
CAST_DEFINE_TRY_S(int, int, INT)
CAST_DEFINE_TRY_S(short, short, SHRT)
CAST_DEFINE_TRY_S(long, long, LONG)
CAST_DEFINE_TRY_S(long long, llong, LLONG)
CAST_DEFINE_TRY_S(ptrdiff_t, ptrdiff, PTRDIFF)
#pragma GCC diagnostic pop
CAST_DEFINE_TRY_F(float, float, 24U)
CAST_DEFINE_TRY_F(double, double, 54U)

/* List of all types supported by cast library */
#define CAST_TYPES                                                             \
	F(uint8_t, u8)                                                         \
	F(uint16_t, u16)                                                       \
	F(uint32_t, u32)                                                       \
	F(uint64_t, u64)                                                       \
	F(unsigned, uint)                                                      \
	F(unsigned short, ushort)                                              \
	F(unsigned long, ulong)                                                \
	F(unsigned long long, ullong)                                          \
	F(size_t, size)                                                        \
	F(uintptr_t, uptr)                                                     \
	F(int8_t, i8)                                                          \
	F(int16_t, i16)                                                        \
	F(int32_t, i32)                                                        \
	F(int64_t, i64)                                                        \
	F(int, int)                                                            \
	F(short, short)                                                        \
	F(long, long)                                                          \
	F(long long, llong)                                                    \
	F(ptrdiff_t, ptrdiff)                                                  \
	F(float, float)                                                        \
	F(double, double)                                                      \
	/* END */

static inline int try_bool_from_str(bool *val, const char *str)
{
	if (!val)
		return -1;

	long long tmp = 0;
	int err = try_llong_from_str(&tmp, str);
	if (err)
		return -1;

	*val = tmp != 0U;
	return 0;
}
CAST_DEFINE_FROM(bool, bool, const char *, str)

#ifdef CAST_IMPLEMENTATION
#include <stdio.h>
#include <inttypes.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef CAST_CUSTOM_PANIC
void cast_panic_impl(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(1);
}
#endif

int cast_try_ullong_from_str(unsigned long long *dst, const char *str)
{
	if (!dst)
		return -1;
	if (!str)
		return -1;

	errno = 0;

	char *endptr;
	unsigned long long val = strtoull(str, &endptr, 0);

	/* If there is an error */
	if (errno != 0) {
		return -1;
	}

	/* If there are unparsed characters */
	if (*endptr != '\0') {
		return -1;
	}

	/* If there were not characters */
	if (endptr == str || str[0] == '\0') {
		return -1;
	}

	*dst = val;

	return 0;
}

int cast_try_llong_from_str(long long *dst, const char *str)
{
	if (!dst)
		return -1;
	if (!str)
		return -1;

	errno = 0;

	char *endptr;
	long long val = strtoll(str, &endptr, 0);

	/* If there is an error */
	if (errno != 0) {
		return -1;
	}

	/* If there are unparsed characters */
	if (*endptr != '\0') {
		return -1;
	}

	/* If there were not characters */
	if (endptr == str || str[0] == '\0') {
		return -1;
	}

	*dst = val;

	return 0;
}

#ifdef CAST_TESTS

static inline int try_float_from_float(float *dst, float src)
{
	if (!dst)
		return -1;
	*dst = src;
	return 0;
}
CAST_DEFINE_FROM(float, float, float, float)

static inline int try_double_from_float(double *dst, float src)
{
	if (!dst)
		return -1;
	*dst = (double)src;
	return 0;
}
CAST_DEFINE_FROM(double, double, float, float)

static inline int try_float_from_double(float *dst, double src)
{
	if (!dst)
		return -1;
	*dst = (float)src;
	return 0;
}
CAST_DEFINE_FROM(float, float, double, double)

static inline int try_double_from_double(double *dst, double src)
{
	if (!dst)
		return -1;
	*dst = src;
	return 0;
}
CAST_DEFINE_FROM(double, double, double, double)

#define CAST_NUMBERS_i8                                                        \
	F(INT8_MIN)                                                            \
	F(-1)                                                                  \
	F(0)                                                                   \
	F(1)                                                                   \
	F(INT8_MAX)                                                            \
	/* END */

#define CAST_NUMBERS_i16                                                       \
	F(INT16_MIN)                                                           \
	F(UINT8_MAX)                                                           \
	F(INT16_MAX)                                                           \
	CAST_NUMBERS_i8

#define CAST_NUMBERS_i32                                                       \
	F(INT32_MIN)                                                           \
	F(INT32_MAX)                                                           \
	F(UINT16_MAX)                                                          \
	CAST_NUMBERS_i16

#define CAST_NUMBERS_i64                                                       \
	F(INT64_MIN)                                                           \
	F(INT64_MAX)                                                           \
	F(UINT32_MAX)                                                          \
	CAST_NUMBERS_i32

#define CAST_NUMBERS_u8                                                        \
	F(0)                                                                   \
	F(1)                                                                   \
	F(UINT8_MAX)                                                           \
	/* END */

#define CAST_NUMBERS_u16                                                       \
	F(INT16_MAX)                                                           \
	F(UINT16_MAX)                                                          \
	CAST_NUMBERS_u8

#define CAST_NUMBERS_u32                                                       \
	F(INT32_MAX)                                                           \
	F(UINT32_MAX)                                                          \
	CAST_NUMBERS_u16

#define CAST_NUMBERS_u64                                                       \
	F(INT64_MAX)                                                           \
	F(UINT64_MAX)                                                          \
	CAST_NUMBERS_u32

#define CAST_NUMBERS_float                                                     \
	F(-1.0f)                                                               \
	F(-0.125f)                                                             \
	F(0.0f)                                                                \
	F(0.125f)                                                              \
	F(1.0f)                                                                \
	F((float)INT8_MIN)                                                     \
	F((float)INT8_MAX)                                                     \
	F((float)UINT8_MAX)                                                    \
	F((float)INT16_MIN)                                                    \
	F((float)UINT16_MAX)                                                   \
	F((float)INT32_MIN)                                                    \
	F(-((float)INT32_MIN))                                                 \
	F((float)INT64_MIN)                                                    \
	F(-((float)INT64_MIN))                                                 \
	F(CAST_UNSIGNED_UPPER_LIMIT(float, uint32_t))                          \
	F(CAST_UNSIGNED_UPPER_LIMIT(float, uint64_t))                          \
	/* END */

#define CAST_NUMBERS_double                                                    \
	F(-1.0)                                                                \
	F(-0.125)                                                              \
	F(0.0)                                                                 \
	F(0.125)                                                               \
	F(1.0)                                                                 \
	F((double)INT8_MIN)                                                    \
	F((double)INT8_MAX)                                                    \
	F((double)UINT8_MAX)                                                   \
	F((double)INT16_MIN)                                                   \
	F((double)UINT16_MAX)                                                  \
	F((double)INT32_MIN)                                                   \
	F(-((double)INT32_MIN))                                                \
	F((double)INT64_MIN)                                                   \
	F(-((double)INT64_MIN))                                                \
	F(CAST_UNSIGNED_UPPER_LIMIT(double, uint32_t))                         \
	F(CAST_UNSIGNED_UPPER_LIMIT(double, uint64_t))                         \
	/* END */

#define CAST_NUMBERS_short CAST_NUMBERS_i16
#define CAST_NUMBERS_int CAST_NUMBERS_i16
#define CAST_NUMBERS_long CAST_NUMBERS_i32
#define CAST_NUMBERS_llong CAST_NUMBERS_i64
#define CAST_NUMBERS_ptrdiff CAST_NUMBERS_i16

#define CAST_NUMBERS_ushort CAST_NUMBERS_u16
#define CAST_NUMBERS_uint CAST_NUMBERS_u16
#define CAST_NUMBERS_ulong CAST_NUMBERS_u32
#define CAST_NUMBERS_ullong CAST_NUMBERS_u64
#define CAST_NUMBERS_size CAST_NUMBERS_u16
#define CAST_NUMBERS_uptr CAST_NUMBERS_u16

#define u8_target uint8_t
#define u8_fmt "%" PRIu8
#define u16_target uint16_t
#define u16_fmt "%" PRIu16
#define u32_target uint32_t
#define u32_fmt "%" PRIu32
#define u64_target uint64_t
#define u64_fmt "%" PRIu64
#define i8_target int8_t
#define i8_fmt "%" PRId8
#define i16_target int16_t
#define i16_fmt "%" PRId16
#define i32_target int32_t
#define i32_fmt "%" PRId32
#define i64_target int64_t
#define i64_fmt "%" PRId64
#define ushort_target unsigned short
#define ushort_fmt "%u"
#define uint_target unsigned int
#define uint_fmt "%u"
#define ulong_target unsigned long
#define ulong_fmt "%lu"
#define ullong_target unsigned long long
#define ullong_fmt "%llu"
#define short_target short
#define short_fmt "%d"
#define int_target int
#define int_fmt "%d"
#define long_target long
#define long_fmt "%ld"
#define llong_target long long
#define llong_fmt "%lld"
#define size_target size_t
#define size_fmt "%zu"
#define ptrdiff_target ptrdiff_t
#define ptrdiff_fmt "%zd"
#define float_target float
#define float_fmt "%f"
#define double_target double 
#define double_fmt "%f"

#define EXACT_TYPE_TEST(x)                                                     \
	TEST(x, u8)                                                            \
	TEST(x, u16)                                                           \
	TEST(x, u32)                                                           \
	TEST(x, u64)                                                           \
	TEST(x, i8)                                                            \
	TEST(x, i16)                                                           \
	TEST(x, i32)                                                           \
	TEST(x, i64)                                                           \
	TEST(x, float)                                                         \
	TEST(x, double)                                                        \
	/* END */

#define TESTS                                                                  \
	/* Fixed size types */                                                 \
	EXACT_TYPE_TEST(u8)                                                    \
	EXACT_TYPE_TEST(u16)                                                   \
	EXACT_TYPE_TEST(u32)                                                   \
	EXACT_TYPE_TEST(u64)                                                   \
	EXACT_TYPE_TEST(i8)                                                    \
	EXACT_TYPE_TEST(i16)                                                   \
	EXACT_TYPE_TEST(i32)                                                   \
	EXACT_TYPE_TEST(i64)                                                   \
	EXACT_TYPE_TEST(float)                                                 \
	EXACT_TYPE_TEST(double)                                                \
	/* Variable types */                                                   \
	TEST(int, short)                                                       \
	TEST(int, int)                                                         \
	TEST(long, int)                                                        \
	TEST(long, short)                                                      \
	TEST(long, long)                                                       \
	TEST(llong, short)                                                     \
	TEST(llong, int)                                                       \
	TEST(llong, long)                                                      \
	TEST(llong, llong)                                                     \
	TEST(uint, ushort)                                                     \
	TEST(uint, uint)                                                       \
	TEST(ulong, ushort)                                                    \
	TEST(ulong, uint)                                                      \
	TEST(ulong, ulong)                                                     \
	TEST(ullong, ushort)                                                   \
	TEST(ullong, uint)                                                     \
	TEST(ullong, ulong)                                                    \
	TEST(ullong, ullong)                                                   \
	/* Size types */                                                       \
	TEST(u8, size)                                                         \
	TEST(size, u8)                                                         \
	TEST(u16, size)                                                        \
	TEST(size, ushort)                                                     \
	TEST(ushort, size)                                                     \
	TEST(size, u16)                                                        \
	TEST(size, ptrdiff)                                                    \
	TEST(ptrdiff, size)                                                    \
	TEST(ptrdiff, short)                                                   \
	TEST(short, ptrdiff)                                                   \
	TEST(ptrdiff, i16)                                                     \
	TEST(i16, ptrdiff)                                                     \
	/* END TESTS */

#undef TEST

#define cast_dump(fmt, x) printf(#x " = "fmt"\n", x)

static void cast_tests(void)
{
	int64_t i64;
	cast_dump("%d", try_i64_from_str(&i64, "1"));
	cast_dump("%d", try_i64_from_str(&i64, "0"));

	bool b;
	cast_dump("%d", try_bool_from_str(&b, "1"));
	cast_dump("%d", try_bool_from_str(&b, "0"));

	cast_dump("%d", cast_shift_zeros_right(0U) == 0U);
	cast_dump("%d", cast_shift_zeros_right(1U) == 1U);
	cast_dump("%d", cast_shift_zeros_right(2U) == 1U);
	cast_dump("%d", cast_shift_zeros_right(3U) == 3U);
	cast_dump("%d", cast_shift_zeros_right(16777215) == 16777215);
	cast_dump("%d", cast_shift_zeros_right(16777215 * 2) == 16777215);
	cast_dump("%d", cast_shift_zeros_right(16777215 * 2 + 1) == 33554431);
	cast_dump("%d", cast_shift_zeros_right(ULLONG_MAX) == ULLONG_MAX);
	cast_dump("%f", float_from_int(16777215));
	cast_dump("%f", float_from_int(16777216));
	cast_dump("%f", float_from_int(16777216 * 2));
	float f = 0;
	cast_dump("%d", try_float_from_int(&f, 16777217 * 2));
#define F(number) number,

#define TEST(dst, src)                                                         \
	{                                                                      \
		const src##_target test_cases[] = {CAST_NUMBERS_##src};        \
		for (size_t i = 0;                                             \
		     i < sizeof(test_cases) / sizeof(test_cases[0]); ++i) {    \
			dst##_target result = 0;                               \
			int err = try_##dst##_from_##src(&result,              \
							 test_cases[i]);       \
			printf("%2d == try_%s_from_%s(&result, " src##_fmt     \
			       "), result = " dst##_fmt "\n",                  \
			       err, #dst, #src, test_cases[i], result);        \
		}                                                              \
	}

	TESTS
#undef TEST
#undef F

#define F(type, name) printf("%s = %s\n", #type, #name);
	CAST_TYPES
#undef F
}
#endif

#endif

#endif
