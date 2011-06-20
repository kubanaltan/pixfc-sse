/*
 * yuv_upsample.h
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

#ifndef YUV_UPSAMPLE_H_
#define YUV_UPSAMPLE_H_

#include <emmintrin.h>
#include <tmmintrin.h>

#include "debug_support.h"
#include "platform_util.h"



/*
 * Calculate missing chroma values for the last 8 pix in an image
 * by averaging previous and current chroma values. The last pixel reuses
 * the chroma values of the before-last pix.
 *
 *
 * TOTAL LATENCY:			4
 *
 * INPUT:
 * U1 0		V1 0	U3 0	V3 0	U5 0	V5 0	U7 0	V7 0
 *
 * OUTPUT:
 * U2 0		V2 0	U4 0	V4 0	U6 0	V6 0	U8 0	V8 0
 */
EXTERN_INLINE void	reconstruct_last_missing_uv_sse2(__m128i* current_uv, __m128i* out) {
	M128I(avgB, 0x0LL, 0x0LL);

	_M(avgB) = _mm_srli_si128(*current_uv, 4);							// PSRLDQ	1	0.5
	// U3 0		V3 0	U5 0	V5 0	U7 0	V7 0	0 0		0 0

	_M(avgB) = _mm_shufflehi_epi16(_M(avgB), 0x44);						// PSHUFHW	1	0.5
	// U3 0		V3 0	U5 0	V5 0	U7 0	V7 0	U7 0	V7 0

	_M(avgB) = _mm_or_si128(_M(avgB), _M(avgB));						// POR		1	0.33
	// U3 0		V3 0	U5 0	V5 0	U7 0	V7 0	U7 0	V7 0

	*out = _mm_avg_epu16(*current_uv, _M(avgB));						// PAVGW	1	0.5
}

/*
 * Calculate missing chroma values by averaging previous and current chroma values
 *
 * TOTAL LATENCY:			4
 *
 * INPUT:
 * U1 0		V1 0	U3 0	V3 0	U5 0	V5 0	U7 0	V7 0
 * U9 0		V9 0	U11 0 	V11 0	U13 0	V13 0	U15 0	V15 0
 *
 * OUTPUT:
 * U2 0		V2 0	U4 0	V4 0	U6 0	V6 0	U8 0	V8 0
 */
EXTERN_INLINE void	reconstruct_missing_uv_sse2(__m128i* current_uv, __m128i* next_uv, __m128i* out) {
	M128I(avgB, 0x0LL, 0x0LL);
	M128I(tmp, 0x0LL, 0x0LL);

	_M(avgB) = _mm_srli_si128(*current_uv, 4);							// PSRLDQ	1	0.5
	// U3 0		V3 0	U5 0	V5 0	U7 0	V7 0	0 0		0 0

	_M(tmp) = _mm_slli_si128(*next_uv, 12);								// PSLLDQ	1	0.5
	// 0 0		0 0		0 0		0 0		0 0		0 0		U9 0	V9 0

	_M(avgB) = _mm_or_si128(_M(tmp), _M(avgB));							// POR		1	0.33
	// U3 0		V3 0	U5 0	V5 0	U7 0	V7 0	U9 0	V9 0

	*out = _mm_avg_epu16(*current_uv, _M(avgB));						// PAVGW	1	0.5
}

/*
 * Calculate missing chroma values for the last 8 pix in an image
 * by averaging previous and current chroma values. The last pixel reuses
 * the chroma values of the before-last pix.
 *
 *
 * TOTAL LATENCY:			2
 *
 * INPUT:
 * U1 0		V1 0	U3 0	V3 0	U5 0	V5 0	U7 0	V7 0
 *
 * OUTPUT:
 * U2 0		V2 0	U4 0	V4 0	U6 0	V6 0	U8 0	V8 0
 */
EXTERN_INLINE void	reconstruct_last_missing_uv_sse2_ssse3(__m128i* current_uv, __m128i* out) {
	CONST_M128I(shuff1, 0xFF0AFF08FF06FF04LL, 0xFF0EFF0CFF0EFF0CLL);
	M128I(avgB, 0x0LL, 0x0LL);

	_M(avgB) = _mm_shuffle_epi8(*current_uv, _M(shuff1));				// PSHUFB	1	0.5
	// U3 0		V3 0	U5 0	V5 0	U7 0	V7 0	U7 0	V7 0

	*out = _mm_avg_epu16(*current_uv, _M(avgB));						// PAVGW	1	0.5
}

/*
 * Calculate missing chroma values by averaging previous and current chroma values
 *
 * TOTAL LATENCY:			4
 *
 * INPUT:
 * U1 0		V1 0	U3 0	V3 0	U5 0	V5 0	U7 0	V7 0
 * U9 0		V9 0	U11 0 	V11 0	U13 0	V13 0	U15 0	V15 0
 *
 * OUTPUT:
 * U2 0		V2 0	U4 0	V4 0	U6 0	V6 0	U8 0	V8 0
 */
EXTERN_INLINE void	reconstruct_missing_uv_sse2_ssse3(__m128i* current_uv, __m128i* next_uv, __m128i* out) {
	CONST_M128I(shuff1, 0xFF0AFF08FF06FF04LL, 0xFFFFFFFFFF0EFF0CLL);
	CONST_M128I(shuff2, 0xFFFFFFFFFFFFFFFFLL, 0xFF02FF00FFFFFFFFLL);
	M128I(avgB, 0x0LL, 0x0LL);
	M128I(tmp, 0x0LL, 0x0LL);

	_M(avgB) = _mm_shuffle_epi8(*current_uv, _M(shuff1));				// PSHUFB	1	0.5
	// U3 0		V3 0	U5 0	V5 0	U7 0	V7 0	0 0		0 0

	_M(tmp) = _mm_shuffle_epi8(*next_uv, _M(shuff2));					// PSHUFB	1	0.5
	// 0 0		0 0		0 0		0 0		0 0		0 0		U9 0	V9 0

	_M(avgB) = _mm_or_si128(_M(tmp), _M(avgB));							// POR		1	0.33
	// U3 0		V3 0	U5 0	V5 0	U7 0	V7 0	U9 0	V9 0

	*out = _mm_avg_epu16(*current_uv, _M(avgB));						// PAVGW	1	0.5
}


#endif /* YUV_UPSAMPLE_H_ */
