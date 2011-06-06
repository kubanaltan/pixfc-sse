/*
 * common.h
 *
 * Copyright (C) 2011 PixFC Team (pixelfc@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public  License as published by the
 * Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdint.h>
#include <emmintrin.h>

#include "platform_util.h"

// Clamp a pixel component's value to 0-255
#define CLIP_PIXEL(x) 		(((x)>255) ? 255 : ((x)<0) ? 0 : (x))

/*
 * This function returns the features supported by the cpu
 * as returned by the CPUID instructions. (ECX is in the higher
 * 32 bits, EDX in the lowest ones). Use the following macros to
 * determine which features are supported.
 */
INLINE uint64_t		get_cpu_features();
#define CPUID_FEATURE_SSE2	0x0000000004000000LL
#define CPUID_FEATURE_SSE3	0x0000000100000000LL
#define CPUID_FEATURE_SSSE3	0x0000020000000000LL
#define CPUID_FEATURE_NONE	0xFFFFFFFFFFFFFFFFLL	// matches all CPUs

/*
 * Check if the CPU we are running on has the given features (mask
 * of above-defined CPUID_FEATURE_* macros).
 * Return 0 if yes, -1 if not.
 */
uint32_t			does_cpu_support(uint64_t);




/*
 *  This function returns the current system's uptime
 *  in nanoseconds (64 bit unsigned)
 */
typedef uint64_t	ticks;
ticks				getticks();

/*
 * Call the first time with timings = NULL to get the current process timing profile.
 * Next, call with a valid stuct timings pointer to store the difference between
 * current process timing profile and previous one. The difference is added to the
 * members in the given struct timings.
 */
struct timings {
	uint64_t	total_time_ns;
	uint64_t	user_time_ns;
	uint64_t	sys_time_ns;
	uint64_t	vcs;
	uint64_t	ivcs;
};
void				do_timing(struct timings *timings);



/*
 * Various debugging functions
 */
#ifdef DEBUG

#define dprintf(fmt, ...) 	do { fprintf (stderr, "[ %s:%d ]\t" fmt,\
		__FILE__, __LINE__, ## __VA_ARGS__);\
		fflush(stderr); } while(0)

void 						print_xmm32(char *name, __m128i *reg);
void 						print_xmm16(char *name, __m128i *reg);
void 						print_xmm16u(char *name, __m128i *reg);
void						print_xmm8u(char *name, __m128i *reg);

#else

#define	dprintf(...)
#define print_xmm32(...)
#define print_xmm16(...)
#define print_xmm16u(...)
#define print_xmm8u(...)

#endif	// DEBUG

#endif /* COMMON_H_ */