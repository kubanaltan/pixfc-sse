/*
 * yuv_pack.h
 *
 * Copyright (C) 2011 PixFC Team (pixelfc@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public  License as published by the
 * Free Software Foundation, either version 3 of the License, or
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


#include <emmintrin.h>
#include <tmmintrin.h>

#include "platform_util.h"


#ifndef GENERATE_UNALIGNED_INLINES
#error "The GENERATE_UNALIGNED_INLINES macro is not defined"
#endif

/*
 * This header file expects the GENERATE_UNALIGNED_INLINES macro to be set to 1
 * to generate inlines for unaligned input buffers.
 * This header file does not have the usual #ifndef / #define / #endif barrier
 * preventing it from being included multiple times in a single source file.
 * This is done so this header file CAN be included multiple times, once with
 * GENERATE_UNALIGNED_INLINES set to 0 to generate inlines for aligned input buffers
 * and once with GENERATE_UNALIGNED_INLINES set to 1 to generate inlines for
 * unaligned input buffers
 */

#undef INLINE_NAME
#undef M128_STORE

#if GENERATE_UNALIGNED_INLINES == 1
	#define INLINE_NAME(fn_suffix, ...)				EXTERN_INLINE void unaligned_ ## fn_suffix(__VA_ARGS__)
	#define M128_STORE(src, dst)					_mm_storeu_si128(&dst, (src))
#else
	#define INLINE_NAME(fn_suffix, ...)				EXTERN_INLINE void fn_suffix(__VA_ARGS__)
	#define M128_STORE(src, dst)					(dst) = (src)
#endif


/*
 * Pack 2 pairs of 422 downsampled Y, UV vectors to 2 vectors YUYV
 *
 * Total latency:			4 cycles
 * Num of pixel handled:	16
 *
 * INPUT:
 *
 * 4 vectors of 8 short
 *
 * yVect1
 * Y1 0		Y2 0	Y3 0	Y4 0	Y5 0	Y6 0	Y7 0	Y8 0
 *
 * uvVect1
 * U12 0	V12 0	U34 0	V34 0	U56 0	V56 0	U78 0	V78 0
 *
 * yVect2
 * Y9 0		Y10 0	Y11 0	Y12 0	Y13 0	Y14 0	Y15 0	Y16 0
 *
 * uvVect2
 * U910 0	V910 0	U1112 0	V1112 0	U1314 0	V1314 0	U1516 0	V1516 0
 *
 * OUTPUT:
 *
 * 2 vectors of 16 char
 *
 * Y1 U12	Y2 V12		Y3 U34		Y4 V34		Y5 U56		Y6 V56		Y7 U78		Y8 V78
 * Y9 U910	Y10 V910	Y11 U1112	Y12 V1112	Y13 U1314	Y14 V1314	Y15 U1516	Y16 V1516
 *
 */
INLINE_NAME(pack_4_y_uv_422_vectors_in_2_yuyv_vectors_sse2, __m128i* in_4_y_uv_422_vectors, __m128i* out_2_yuyv_vectors) {
	M128I(scratch1, 0x0LL, 0x0LL);
	M128I(scratch2, 0x0LL, 0x0LL);

	_M(scratch1) = _mm_packus_epi16(in_4_y_uv_422_vectors[0], in_4_y_uv_422_vectors[2]);// PACKUSWB		1	0.5
	// Y1 Y2	Y3 Y4	Y5 Y6	Y7 Y8	Y9 Y10	Y11 Y12	Y13 Y14	Y15 Y16

	_M(scratch2) = _mm_packus_epi16(in_4_y_uv_422_vectors[1], in_4_y_uv_422_vectors[3]);// PACKUSWB		1	0.5
	// U12 V12	U34 V34	U56 V56	U78 V78	U910 V910	U1112 V1112	U1314 V1314	U1516 V1516

	M128_STORE(_mm_unpacklo_epi8(_M(scratch1), _M(scratch2)), out_2_yuyv_vectors[0]);	// PUNPCKLBW	1	0.5

	M128_STORE(_mm_unpackhi_epi8(_M(scratch1), _M(scratch2)), out_2_yuyv_vectors[1]);	// PUNPCKHBW	1	0.5
}


/*
 * Pack 2 pairs of 422 downsampled Y, UV vectors to 2 vectors UYVY
 *
 * Total latency:			4 cycles
 * Num of pixel handled:	16
 *
 * INPUT:
 *
 * 4 vectors of 8 short
 *
 * yVect1
 * Y1 0		Y2 0	Y3 0	Y4 0	Y5 0	Y6 0	Y7 0	Y8 0
 *
 * uvVect1
 * U12 0	V12 0	U34 0	V34 0	U56 0	V56 0	U78 0	V78 0
 *
 * yVect2
 * Y9 0		Y10 0	Y11 0	Y12 0	Y13 0	Y14 0	Y15 0	Y16 0
 *
 * uvVect2
 * U910 0	V910 0	U1112 0	V1112 0	U1314 0	V1314 0	U1516 0	V1516 0
 *
 * OUTPUT:
 *
 * 2 vectors of 16 char
 *
 * U12 Y1 	V12 Y2		U34 Y3 		V34 Y4		U56 Y5		V56 Y6		U78 Y7		V78 Y8
 * U910 Y9	V910 Y10	U1112 Y11	V1112 Y12	U1314 Y13	V1314 Y14	U1516 Y15	V1516 Y16
 *
 */
INLINE_NAME(pack_4_y_uv_422_vectors_in_2_uyvy_vectors_sse2, __m128i* in_4_y_uv_422_vectors, __m128i* out_2_uyvy_vectors) {
	M128I(scratch1, 0x0LL, 0x0LL);
	M128I(scratch2, 0x0LL, 0x0LL);

	_M(scratch1) = _mm_packus_epi16(in_4_y_uv_422_vectors[0], in_4_y_uv_422_vectors[2]);// PACKUSWB		1	0.5
	// Y1 Y2	Y3 Y4	Y5 Y6	Y7 Y8	Y9 Y10	Y11 Y12	Y13 Y14	Y15 Y16

	_M(scratch2) = _mm_packus_epi16(in_4_y_uv_422_vectors[1], in_4_y_uv_422_vectors[3]);// PACKUSWB		1	0.5
	// U12 V12	U34 V34	U56 V56	U78 V78	U910 V910	U1112 V1112	U1314 V1314	U1516 V1516

	M128_STORE(_mm_unpacklo_epi8(_M(scratch2), _M(scratch1)), out_2_uyvy_vectors[0]);	// PUNPCKLBW	1	0.5

	M128_STORE(_mm_unpackhi_epi8(_M(scratch2), _M(scratch1)), out_2_uyvy_vectors[1]);	// PUNPCKHBW	1	0.5
}


