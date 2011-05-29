/*
 * rgb_unpack.h
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

#ifndef RGB_UNPACK_H_
#define RGB_UNPACK_H_


#include <emmintrin.h>
#include <stdint.h>
#include <tmmintrin.h>

#include "platform_util.h"


/*
 * Convert 2 vectors of 16 char ARGB to 4 vectors of 8 short AG1, RB1, AG2 & RB2
 *
 * Total latency:				4
 * Number of pixels handled:	8
 *
 * INPUT
 * 2 vectors of 16 char
 * A1  R1	G1  B1	A2  R2	G2  B2	A3  R3	G3  B3	A4  R4	G4  B4
 *
 * A5  R5	G5  B5	A6  R6	G6  B6	A7  R7	G7  B7	A8  R8	G8  B8
 *
 * OUTPUT:
 *
 * 4 vectors of 8 short
 * agVect1
 * A1 0		G1 0	A2 0	G2 0	A3 0	G3 0	A4 0	G4 0
 *
 * rbVect1
 * R1 0		B1 0	R2 0	B2 0	R3 0	B3 0	R4 0	B4 0
 *
 * agVect2
 * A5 0		G5 0	A6 0	G6 0	A7 0	G7 0	A8 0	G8 0
 *
 * rbVect3
 * R5 0		B5 0	R6 0	B6 0	R7 0	B7 0	R8 0	B8 0
 */
EXTERN_INLINE void unpack_argb_to_ag_rb_vectors_sse2(__m128i* in_2_v8i_argb_vectors, __m128i* out_4_v16i_ag_rb_vectors)
{
	CONST_M128I(mask_off_rb, 0x00FF00FF00FF00FFLL, 0x00FF00FF00FF00FFLL);

	out_4_v16i_ag_rb_vectors[0] = _mm_and_si128(in_2_v8i_argb_vectors[0], _M(mask_off_rb));	// PAND		1	0.33
	// A1 0		G1 0	A2 0	G2 0	A3 0	G3 0	A4 0	G4 0

	out_4_v16i_ag_rb_vectors[1] = _mm_srli_epi16(in_2_v8i_argb_vectors[0], 8);				// PSRLW	1	1
	// R1 0		B1 0	R2 0	B2 0	R3 0	B3 0	R4 0	B4 0

	out_4_v16i_ag_rb_vectors[2] = _mm_and_si128(in_2_v8i_argb_vectors[1], _M(mask_off_rb));	// PAND		1	0.33
	// A5 0		G5 0	A6 0	G6 0	A7 0	G7 0	A8 0	G8 0

	out_4_v16i_ag_rb_vectors[3] = _mm_srli_epi16(in_2_v8i_argb_vectors[1], 8);				// PSRLW	1	1
	// R5 0		B5 0	R6 0	B6 0	R7 0	B7 0	R8 0	B8 0
};


/*
 * Convert 2 vectors of 16 char ARGB to 4 vectors of 8 short AG1, RB1, AG2 & RB2
 *
 * Total latency:				4
 * Number of pixels handled:	8
 *
 * INPUT
 * 2 vectors of 16 char
 * A1  R1	G1  B1	A2  R2	G2  B2	A3  R3	G3  B3	A4  R4	G4  B4
 *
 * A5  R5	G5  B5	A6  R6	G6  B6	A7  R7	G7  B7	A8  R8	G8  B8
 *
 * OUTPUT:
 *
 * 4 vectors of 8 short
 * agVect1
 * A1 0		G1 0	A2 0	G2 0	A3 0	G3 0	A4 0	G4 0
 *
 * rbVect1
 * R1 0		B1 0	R2 0	B2 0	R3 0	B3 0	R4 0	B4 0
 *
 * agVect2
 * A5 0		G5 0	A6 0	G6 0	A7 0	G7 0	A8 0	G8 0
 *
 * rbVect3
 * R5 0		B5 0	R6 0	B6 0	R7 0	B7 0	R8 0	B8 0
 */
EXTERN_INLINE void unpack_argb_to_ag_rb_vectors_sse2_ssse3(__m128i* in_2_v8i_argb_vectors, __m128i* out_4_v16i_ag_rb_vectors)
{
	CONST_M128I(shuf_out1, 0xFF06FF04FF02FF00LL, 0xFF0EFF0CFF0AFF08LL);
	CONST_M128I(shuf_out2, 0xFF07FF05FF03FF01LL, 0xFF0FFF0DFF0BFF09LL);

	out_4_v16i_ag_rb_vectors[0] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_out1));// PSHUFB	1	0.5
	// A1 0		G1 0	A2 0	G2 0	A3 0	G3 0	A4 0	G4 0

	out_4_v16i_ag_rb_vectors[1] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_out2));// PSHUFB	1	0.5
	// R1 0		B1 0	R2 0	B2 0	R3 0	B3 0	R4 0	B4 0

	out_4_v16i_ag_rb_vectors[2] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_out1));// PSHUFB	1	0.5
	// A5 0		G5 0	A6 0	G6 0	A7 0	G7 0	A8 0	G8 0

	out_4_v16i_ag_rb_vectors[3] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_out2));// PSHUFB	1	0.5
	// R5 0		B5 0	R6 0	B6 0	R7 0	B7 0	R8 0	B8 0
};


/*
 * Convert 2 vectors of 16 char ARGB to 3 vectors of 8 short:
 *
 * Total latency:				15
 * Number of pixels handled:	8
 *
 * INPUT
 * 2 vectors of 16 char
 * A1  R1	G1  B1	A2  R2	G2  B2	A3  R3	G3  B3	A4  R4	G4  B4
 *
 * A5  R5	G5  B5	A6  R6	G6  B6	A7  R7	G7  B7	A8  R8	G8  B8
 *
 * OUTPUT:
 *
 * 3 vectors of 8 short
 * rVect
 * R1 0		R2 0	R3 0	R4 0	R5 0	R6 0	R7 0	R8 0
 *
 * gb1Vect
 * G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0
 *
 * gb2Vect
 * B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
 */
EXTERN_INLINE void unpack_argb_to_r_g_b_vectors_sse2(__m128i* in_2_v8i_argb_vectors, __m128i* out_3_v16i_r_g_b_vectors)
{
	CONST_M128I(mask_off_agb, 0x0000FF000000FF00LL, 0x0000FF000000FF00LL);
	CONST_M128I(mask_off_arb, 0x00FF000000FF0000LL, 0x00FF000000FF0000LL);
	CONST_M128I(mask_off_arg, 0xFF000000FF000000LL, 0xFF000000FF000000LL);
	M128I(scratch1, 0x0LL, 0x0LL);

	out_3_v16i_r_g_b_vectors[0] = _mm_and_si128(in_2_v8i_argb_vectors[0], _M(mask_off_agb));// PAND		1	0.33
	// 0 R1 	0 0		0 R2	0 0		0 R3 	0 0		0 R4 	0 0

	out_3_v16i_r_g_b_vectors[0] = _mm_srli_epi16(out_3_v16i_r_g_b_vectors[0], 8);			// PSRLW	1	1
	// R1  0	0 0		R2 0	0 0		R3 0	0 0		R4 0	0 0

	_M(scratch1) = _mm_and_si128(in_2_v8i_argb_vectors[1], _M(mask_off_agb));				// PAND		1	0.33
	// 0 R5 	0 0		0 R6	0 0		0 R7 	0 0		0 R8 	0 0

	_M(scratch1) = _mm_srli_epi16(_M(scratch1), 8);											// PSRLW	1	1
	// R5  0	0 0		R6 0	0 0		R7 0	0 0		R8 0	0 0

	out_3_v16i_r_g_b_vectors[0] = _mm_packs_epi32(out_3_v16i_r_g_b_vectors[0], _M(scratch1));//PACKSSDW	1	0.5
	// R1 0		R2 0	R3 0	R4 0	R5 0	R6 0	R7 0	R8 0



	out_3_v16i_r_g_b_vectors[1] = _mm_and_si128(in_2_v8i_argb_vectors[0], _M(mask_off_arb));// PAND		1	0.33
	// 0 0		G1 0	0 0		G2 0	0 0		G3 0	0 0		G4 0

	out_3_v16i_r_g_b_vectors[1] = _mm_srli_epi32(out_3_v16i_r_g_b_vectors[1], 16);			// PSRLD	1	1
	// G1 0 	0 0		G2 0	0 0		G3 0	0 0		G4 0 	0 0

	_M(scratch1) = _mm_and_si128(in_2_v8i_argb_vectors[1], _M(mask_off_arb));				// PAND		1	0.33
	// 0 0		G5 0	0 0		G6 0	0 0		G7 0	0 0		G8 0

	_M(scratch1) = _mm_srli_epi32(_M(scratch1), 16);										// PSRLD	1	1
	// G5 0 	0 0		G6 0	0 0		G7 0	0 0		G8 0 	0 0

	out_3_v16i_r_g_b_vectors[1] = _mm_packs_epi32(out_3_v16i_r_g_b_vectors[1], _M(scratch1));//PACKSSDW	1	0.5
	// G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0



	out_3_v16i_r_g_b_vectors[2] = _mm_and_si128(in_2_v8i_argb_vectors[0], _M(mask_off_arg));// PAND		1	0.33
	// 0 0		0 B1	0 0		0 B2	0 0		0 B3	0 0		0 B4

	out_3_v16i_r_g_b_vectors[2] = _mm_srli_epi32(out_3_v16i_r_g_b_vectors[2], 24);			// PSRLD	1	1
	// B1 0 	0 0		B2 0	0 0		B3 0	0 0		B4 0 	0 0

	_M(scratch1) = _mm_and_si128(in_2_v8i_argb_vectors[1], _M(mask_off_arg));				// PAND		1	0.33
	// 0 0		0 B5	0 0		0 B6	0 0		0 B7	0 0		0 B8

	_M(scratch1) = _mm_srli_epi32(_M(scratch1), 24);										// PSRLD	1	1
	// B5 0 	0 0		B6 0	0 0		B7 0	0 0		B8 0 	0 0

	out_3_v16i_r_g_b_vectors[2] = _mm_packs_epi32(out_3_v16i_r_g_b_vectors[2], _M(scratch1));//PACKSSDW	1	0.5
	// B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
};


/*
 * Convert 2 vectors of 16 char ARGB to 3 vectors of 8 short:
 *
 * Total latency:				9
 * Number of pixels handled:	8
 *
 * INPUT
 * 2 vectors of 16 char
 * A1  R1	G1  B1	A2  R2	G2  B2	A3  R3	G3  B3	A4  R4	G4  B4
 *
 * A5  R5	G5  B5	A6  R6	G6  B6	A7  R7	G7  B7	A8  R8	G8  B8
 *
 * OUTPUT:
 *
 * 3 vectors of 8 short
 * rVect
 * R1  0	R2  0	R3  0	R4  0	R5  0	R6  0	R7  0	R8  0
 *
 * gVect
 * G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0
 *
 * bVect
 * B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
 */
EXTERN_INLINE void unpack_argb_to_r_g_b_vectors_sse2_ssse3(__m128i* in_2_v8i_argb_vectors, __m128i* out_3_v16i_r_g_b_vectors)
{
	CONST_M128I(shuf_r1, 0xFF0DFF09FF05FF01LL, 0xFFFFFFFFFFFFFFFFLL);
	CONST_M128I(shuf_r2, 0xFFFFFFFFFFFFFFFFLL, 0xFF0DFF09FF05FF01LL);
	CONST_M128I(shuf_g1, 0xFF0EFF0AFF06FF02LL, 0xFFFFFFFFFFFFFFFFLL);
	CONST_M128I(shuf_g2, 0xFFFFFFFFFFFFFFFFLL, 0xFF0EFF0AFF06FF02LL);
	CONST_M128I(shuf_b1, 0xFF0FFF0BFF07FF03LL, 0xFFFFFFFFFFFFFFFFLL);
	CONST_M128I(shuf_b2, 0xFFFFFFFFFFFFFFFFLL, 0xFF0FFF0BFF07FF03LL);

	M128I(scratch1, 0x0LL, 0x0LL);

	_M(scratch1) = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_r1));					// PSHUFB	1	0.5
	// R1 0		R2 0	R3 0	R4 0	0 0		0 0		0 0		0 0

	out_3_v16i_r_g_b_vectors[0] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_r2));	// PSHUFB	1	0.5
	// 0 0		0 0		0 0		0 0		R5 0	R6 0	R7 0	R8 0

	out_3_v16i_r_g_b_vectors[0] = _mm_or_si128(out_3_v16i_r_g_b_vectors[0], _M(scratch1));	// POR		1	0.33
	// R1  0	R2  0	R3  0	R4  0	R5  0	R6  0	R7  0	R8  0


	_M(scratch1) = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_g1));					// PSHUFB	1	0.5
	// G1 0		G2 0	G3 0	G4 0	0 0		0 0		0 0		0 0

	out_3_v16i_r_g_b_vectors[1] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_g2));	// PSHUFB	1	0.5
	// 0 0		0 0		0 0		0 0		G5 0	G6 0	G7 0	G8 0

	out_3_v16i_r_g_b_vectors[1] = _mm_or_si128(out_3_v16i_r_g_b_vectors[1], _M(scratch1));	// POR		1	0.33
	// G1  0	G2 0	G3  0	G4  0	G5  0	G6  0	G7  0	G8  0


	_M(scratch1) = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_b1));					// PSHUFB	1	0.5
	// B1 0		B2 0	B3 0	B4 0	0 0		0 0		0 0		0 0

	out_3_v16i_r_g_b_vectors[2] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_b2));	// PSHUFB	1	0.5
	// 0 0		0 0		0 0		0 0		B5 0	B6 0	B7 0	B8 0

	out_3_v16i_r_g_b_vectors[2] = _mm_or_si128(out_3_v16i_r_g_b_vectors[2], _M(scratch1));	// POR		1	0.33
	// B1  0	B2 0	B3  0	B4  0	B5  0	B6  0	B7  0	B8  0
};









/*
 * Convert 2 vectors of 16 char BGRA to 4 vectors of 8 short AG1, RB1, AG2 & RB2
 *
 * Total latency:				12
 * Number of pixels handled:	8
 *
 * INPUT
 * 2 vectors of 16 char
 * B1  G1	R1  A1	B2  G2	R2  A2	B3  G3	R3  A3	B4  G4	R4  A4
 *
 * B5  G5	R5  A5	B6  G6	R6  A6	B7  G7	R7  A7	B8  G8	R8  A8
 *
 * OUTPUT:
 *
 * 4 vectors of 8 short
 * agVect1
 * A1 0		G1 0	A2 0	G2 0	A3 0	G3 0	A4 0	G4 0
 *
 * rbVect1
 * R1 0		B1 0	R2 0	B2 0	R3 0	B3 0	R4 0	B4 0
 *
 * agVect2
 * A5 0		G5 0	A6 0	G6 0	A7 0	G7 0	A8 0	G8 0
 *
 * rbVect3
 * R5 0		B5 0	R6 0	B6 0	R7 0	B7 0	R8 0	B8 0
 */
EXTERN_INLINE void unpack_bgra_to_ag_rb_vectors_sse2(__m128i* in_2_v8i_argb_vectors, __m128i* out_4_v16i_ag_rb_vectors)
{
	CONST_M128I(mask_off_ag, 0x00FF00FF00FF00FFLL, 0x00FF00FF00FF00FFLL);

	out_4_v16i_ag_rb_vectors[1] = _mm_and_si128(in_2_v8i_argb_vectors[0], _M(mask_off_ag));	// PAND		1	0.33
	// B1 0		R1 0	B2 0	R2 0	B3 0	R3 0	B4 0	R4 0

	out_4_v16i_ag_rb_vectors[1] = _mm_shufflelo_epi16(out_4_v16i_ag_rb_vectors[1], 0xB1);	// PSHUFLW	1	0.5
	// R1 0		B1 0	R2 0	B2 0	B3 0	R3 0	B4 0	R4 0

	out_4_v16i_ag_rb_vectors[1] = _mm_shufflehi_epi16(out_4_v16i_ag_rb_vectors[1], 0xB1);	// PSHUFHW	1	0.5
	// R1 0		B1 0	R2 0	B2 0	R3 0	B3 0	R4 0	B4 0

	out_4_v16i_ag_rb_vectors[0] = _mm_srli_epi16(in_2_v8i_argb_vectors[0], 8);				// PSRLW	1	1
	// G1 0		A1 0	G2 0	A2 0	G3 0	A3 0	G4 0	A4 0

	out_4_v16i_ag_rb_vectors[0] = _mm_shufflelo_epi16(out_4_v16i_ag_rb_vectors[0], 0xB1);	// PSHUFLW	1	0.5
	// A1 0		G1 0	A2 0	G2 0	G3 0	A3 0	G4 0	A4 0

	out_4_v16i_ag_rb_vectors[0] = _mm_shufflehi_epi16(out_4_v16i_ag_rb_vectors[0], 0xB1);	// PSHUFHW	1	0.5
	// A1 0		G1 0	A2 0	G2 0	A3 0	G3 0	A4 0	G4 0

	out_4_v16i_ag_rb_vectors[3] = _mm_and_si128(in_2_v8i_argb_vectors[1], _M(mask_off_ag));	// PAND		1	0.33
	// B5 0		R5 0	B6 0	R6 0	B7 0	R7 0	B8 0	R8 0

	out_4_v16i_ag_rb_vectors[3] = _mm_shufflelo_epi16(out_4_v16i_ag_rb_vectors[3], 0xB1);	// PSHUFLW	1	0.5
	// R5 0		B5 0	R6 0	B6 0	B7 0	R7 0	B8 0	R8 0

	out_4_v16i_ag_rb_vectors[3] = _mm_shufflehi_epi16(out_4_v16i_ag_rb_vectors[3], 0xB1);	// PSHUFHW	1	0.5
	// R5 0		B5 0	R6 0	B6 0	R7 0	B7 0	R8 0	B8 0

	out_4_v16i_ag_rb_vectors[2] = _mm_srli_epi16(in_2_v8i_argb_vectors[1], 8);				// PSRLW	1	1
	// G5 0		A5 0	G6 0	A6 0	G7 0	A7 0	G8 0	A8 0

	out_4_v16i_ag_rb_vectors[2] = _mm_shufflelo_epi16(out_4_v16i_ag_rb_vectors[2], 0xB1);	// PSHUFLW	1	0.5
	// A5 0		G5 0	A6 0	G6 0	G7 0	A7 0	G8 0	A8 0

	out_4_v16i_ag_rb_vectors[2] = _mm_shufflehi_epi16(out_4_v16i_ag_rb_vectors[2], 0xB1);	// PSHUFHW	1	0.5
	// A5 0		G5 0	A6 0	G6 0	A7 0	G7 0	A8 0	G8 0
};


/*
 * Convert 2 vectors of 16 char BGRA to 4 vectors of 8 short AG1, RB1, AG2 & RB2
 *
 * Total latency:				4
 * Number of pixels handled:	8
 *
 * INPUT
 * 2 vectors of 16 char
 * B1  G1	R1  A1	B2  G2	R2  A2	B3  G3	R3  A3	B4  G4	R4  A4
 *
 * B5  G5	R5  A5	B6  G6	R6  A6	B7  G7	R7  A7	B8  G8	R8  A8
 *
 * OUTPUT:
 *
 * 4 vectors of 8 short
 * agVect1
 * A1 0		G1 0	A2 0	G2 0	A3 0	G3 0	A4 0	G4 0
 *
 * rbVect1
 * R1 0		B1 0	R2 0	B2 0	R3 0	B3 0	R4 0	B4 0
 *
 * agVect2
 * A5 0		G5 0	A6 0	G6 0	A7 0	G7 0	A8 0	G8 0
 *
 * rbVect3
 * R5 0		B5 0	R6 0	B6 0	R7 0	B7 0	R8 0	B8 0
 */
EXTERN_INLINE void unpack_bgra_to_ag_rb_vectors_sse2_ssse3(__m128i* in_2_v8i_argb_vectors, __m128i* out_4_v16i_ag_rb_vectors)
{
	CONST_M128I(shuf_out1, 0xFF05FF07FF01FF03LL, 0xFF0DFF0FFF09FF0BLL);
	CONST_M128I(shuf_out2, 0xFF04FF06FF00FF02LL, 0xFF0CFF0EFF08FF0ALL);

	out_4_v16i_ag_rb_vectors[0] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_out1));// PSHUFB	1	0.5
	// A1 0		G1 0	A2 0	G2 0	A3 0	G3 0	A4 0	G4 0

	out_4_v16i_ag_rb_vectors[1] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_out2));// PSHUFB	1	0.5
	// R1 0		B1 0	R2 0	B2 0	R3 0	B3 0	R4 0	B4 0

	out_4_v16i_ag_rb_vectors[2] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_out1));// PSHUFB	1	0.5
	// A5 0		G5 0	A6 0	G6 0	A7 0	G7 0	A8 0	G8 0

	out_4_v16i_ag_rb_vectors[3] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_out2));// PSHUFB	1	0.5
	// R5 0		B5 0	R6 0	B6 0	R7 0	B7 0	R8 0	B8 0
};

/*
 * Convert 2 vectors of 16 char BGRA to 3 vectors of 8 short:
 *
 * Total latency:				13
 * Number of pixels handled:	8
 *
 * INPUT
 * 2 vectors of 16 char
 * B1  G1	R1  A1	B2  G2	R2  A2	B3  G3	R3  A3	B4  G4	R4  A4
 *
 * B5  G5	R5  A5	B6  G6	R6  A6	B7  G7	R7  A7	B8  G8	R8  A8
 *
 * OUTPUT:
 *
 * 3 vectors of 8 short
 * rVect
 * R1 0		R2 0	R3 0	R4 0	R5 0	R6 0	R7 0	R8 0
 *
 * gVect
 * G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0
 *
 * bVect
 * B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
 */
EXTERN_INLINE void unpack_bgra_to_r_g_b_vectors_sse2(__m128i* in_2_v8i_argb_vectors, __m128i* out_3_v16i_r_g_b_vectors)
{
	CONST_M128I(mask_off_bga, 0x00FF000000FF0000LL, 0x00FF000000FF0000LL);
	CONST_M128I(mask_off_bra, 0x0000FF000000FF00LL, 0x0000FF000000FF00LL);
	CONST_M128I(mask_off_gra, 0x000000FF000000FFLL, 0x000000FF000000FFLL);
	M128I(scratch1, 0x0LL, 0x0LL);
	
	out_3_v16i_r_g_b_vectors[0] = _mm_and_si128(in_2_v8i_argb_vectors[0], _M(mask_off_bga));// PAND		1	0.33
	// 0 0		R1 0	0 0		R2 0	0 0		R3 0	0 0		R4 0

	out_3_v16i_r_g_b_vectors[0] = _mm_srli_epi32(out_3_v16i_r_g_b_vectors[0], 16);			// PSRLD	1	1
	// R1  0	0 0		R2 0	0 0		R3 0	0 0		R4 0	0 0
	
	_M(scratch1) = _mm_and_si128(in_2_v8i_argb_vectors[1], _M(mask_off_bga));				// PAND		1	0.33
	// 0 0		R5 0	0 0		R6 0	0 0		R7 0	0 0		R8 0

	_M(scratch1) = _mm_srli_epi32(_M(scratch1), 16);										// PSRLD	1	1
	// R5  0	0 0		R6 0	0 0		R7 0	0 0		R8 0	0 0
	
	out_3_v16i_r_g_b_vectors[0] = _mm_packs_epi32(out_3_v16i_r_g_b_vectors[0], _M(scratch1));//PACKSSDW	1	0.5
	// R1 0		R2 0	R3 0	R4 0	R5 0	R6 0	R7 0	R8 0



	out_3_v16i_r_g_b_vectors[1] = _mm_and_si128(in_2_v8i_argb_vectors[0], _M(mask_off_bra));// PAND		1	0.33
	// 0 G1		0 0		0 G2	0 0		0 G3	0 0		0 G4	0 0
	
	out_3_v16i_r_g_b_vectors[1] = _mm_srli_epi16(out_3_v16i_r_g_b_vectors[1], 8);			// PSRLW	1	1
	// G1 0 	0 0		G2 0	0 0		G3 0	0 0		G4 0 	0 0
	
	_M(scratch1) = _mm_and_si128(in_2_v8i_argb_vectors[1], _M(mask_off_bra));				// PAND		1	0.33
	// 0 G5		0 0		0 G6	0 0		0 G7	0 0		0 G8	0 0
	
	_M(scratch1) = _mm_srli_epi16(_M(scratch1), 8);											// PSRLW	1	1
	// G5 0 	0 0		G6 0	0 0		G7 0	0 0		G8 0 	0 0
	
	out_3_v16i_r_g_b_vectors[1] = _mm_packs_epi32(out_3_v16i_r_g_b_vectors[1], _M(scratch1));//PACKSSDW	1	0.5
	// G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0
	
	

	out_3_v16i_r_g_b_vectors[2] = _mm_and_si128(in_2_v8i_argb_vectors[0], _M(mask_off_gra));// PAND		1	0.33
	// B1 0		0 0		B2 0	0 0		B3 0	0 0		B4 0	0 0

	_M(scratch1) = _mm_and_si128(in_2_v8i_argb_vectors[1], _M(mask_off_gra));				// PAND		1	0.33
	// B5 0		0 0		B6 0	0 0		B7 0	0 0		B8 0	0 0

	out_3_v16i_r_g_b_vectors[2] = _mm_packs_epi32(out_3_v16i_r_g_b_vectors[2], _M(scratch1));//PACKSSDW	1	0.5
	// B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
};


/*
 * Convert 2 vectors of 16 char BGRA to 3 vectors of 8 short:
 *
 * Total latency:				9
 * Number of pixels handled:	8
 *
 * INPUT
 * 2 vectors of 16 char
 * B1  G1	R1  A1	B2  G2	R2  A2	B3  G3	R3  A3	B4  G4	R4  A4
 *
 * B5  G5	R5  A5	B6  G6	R6  A6	B7  G7	R7  A7	B8  G8	R8  A8
 *
 * OUTPUT:
 *
 * 3 vectors of 8 short
 * rVect
 * R1  0	R2  0	R3  0	R4  0	R5  0	R6  0	R7  0	R8  0
 *
 * gVect
 * G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0
 *
 * bVect
 * B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
 */
EXTERN_INLINE void unpack_bgra_to_r_g_b_vectors_sse2_ssse3(__m128i* in_2_v8i_argb_vectors, __m128i* out_3_v16i_r_g_b_vectors)
{
	CONST_M128I(shuf_r1, 0xFF0EFF0AFF06FF02LL, 0xFFFFFFFFFFFFFFFFLL);
	CONST_M128I(shuf_r2, 0xFFFFFFFFFFFFFFFFLL, 0xFF0EFF0AFF06FF02LL);
	CONST_M128I(shuf_g1, 0xFF0DFF09FF05FF01LL, 0xFFFFFFFFFFFFFFFFLL);
	CONST_M128I(shuf_g2, 0xFFFFFFFFFFFFFFFFLL, 0xFF0DFF09FF05FF01LL);
	CONST_M128I(shuf_b1, 0xFF0CFF08FF04FF00LL, 0xFFFFFFFFFFFFFFFFLL);
	CONST_M128I(shuf_b2, 0xFFFFFFFFFFFFFFFFLL, 0xFF0CFF08FF04FF00LL);
	
	M128I(scratch1, 0x0LL, 0x0LL);
	
	_M(scratch1) = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_r1));					// PSHUFB	1	0.5
	// R1 0		R2 0	R3 0	R4 0	0 0		0 0		0 0		0 0
	
	out_3_v16i_r_g_b_vectors[0] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_r2));	// PSHUFB	1	0.5
	// 0 0		0 0		0 0		0 0		R5 0	R6 0	R7 0	R8 0
	
	out_3_v16i_r_g_b_vectors[0] = _mm_or_si128(out_3_v16i_r_g_b_vectors[0], _M(scratch1));	// POR		1	0.33
	// R1  0	R2  0	R3  0	R4  0	R5  0	R6  0	R7  0	R8  0

	
	_M(scratch1) = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_g1));					// PSHUFB	1	0.5
	// G1 0		G2 0	G3 0	G4 0	0 0		0 0		0 0		0 0
	
	out_3_v16i_r_g_b_vectors[1] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_g2));	// PSHUFB	1	0.5
	// 0 0		0 0		0 0		0 0		G5 0	G6 0	G7 0	G8 0
	
	out_3_v16i_r_g_b_vectors[1] = _mm_or_si128(out_3_v16i_r_g_b_vectors[1], _M(scratch1));	// POR		1	0.33
	// G1  0	G2 0	G3  0	G4  0	G5  0	G6  0	G7  0	G8  0

	
	_M(scratch1) = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_b1));					// PSHUFB	1	0.5
	// B1 0		B2 0	B3 0	B4 0	0 0		0 0		0 0		0 0
	
	out_3_v16i_r_g_b_vectors[2] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_b2));	// PSHUFB	1	0.5
	// 0 0		0 0		0 0		0 0		B5 0	B6 0	B7 0	B8 0
	
	out_3_v16i_r_g_b_vectors[2] = _mm_or_si128(out_3_v16i_r_g_b_vectors[2], _M(scratch1));	// POR		1	0.33
	// B1  0	B2 0	B3  0	B4  0	B5  0	B6  0	B7  0	B8  0
};






/*
 * Convert 2 vectors of 16 char RGB24 to 7 vectors of 8 short AG1, RB1, AG2 & RB2,
 * AG3, RB3, AG4 & RB4
 *
 * Total latency:				X
 * Number of pixels handled:	16
 *
 * INPUT
 * 3 vectors of 16 char
 * R1 G1	B1 R2	G2 B2	R3 G3	B3 R4	G4 B4	R5 G5	B5 R6
 *
 * G6 B6	R7 G7	B7 R8	G8 B8	R9 G9	B9 R10	G10 B10	R11 G11
 *
 * B11 R12	G12 B12	R13 G13	B13 R14	G14 B14	R15 G15	B15 R16 G16 B16
 *
 * OUTPUT:
 *
 * 8 vectors of 8 short
 * agVect1
 * A1 0		G1 0	A2 0	G2 0	A3 0	G3 0	A4 0	G4 0
 *
 * rbVect1
 * R1 0		B1 0	R2 0	B2 0	R3 0	B3 0	R4 0	B4 0
 *
 * agVect2
 * A5 0		G5 0	A6 0	G6 0	A7 0	G7 0	A8 0	G8 0
 *
 * rbVect2
 * R5 0		B5 0	R6 0	B6 0	R7 0	B7 0	R8 0	B8 0
 *
 * agVect3
 * A9 0		G9 0	A10 0	G10 0	A11 0	G11 0	A12 0	G12 0
 *
 * rbVect3
 * R9 0		B9 0	R10 0	B10 0	R11 0	B11 0	R12 0	B12 0
 *
 * agVect4
 * A13 0	G13 0	A14 0	G14 0	A15 0	G15 0	A16 0	G16 0
 *
 * rbVect4
 * R13 0	B13 0	R14 0	B14 0	R15 0	B15 0	R16 0	B16 0
 */
EXTERN_INLINE void unpack_rgb24_to_ag_rb_vectors_nosse(__m128i* in_3_v8i_rgb24_vectors, __m128i* out_8_v16i_ag_rb_vectors)
{
	uint8_t* ag1 = (uint8_t *) &out_8_v16i_ag_rb_vectors[0];
	uint8_t* rb1 = (uint8_t *) &out_8_v16i_ag_rb_vectors[1];
	uint8_t* ag2 = (uint8_t *) &out_8_v16i_ag_rb_vectors[2];
	uint8_t* rb2 = (uint8_t *) &out_8_v16i_ag_rb_vectors[3];
	uint8_t* ag3 = (uint8_t *) &out_8_v16i_ag_rb_vectors[4];
	uint8_t* rb3 = (uint8_t *) &out_8_v16i_ag_rb_vectors[5];
	uint8_t* ag4 = (uint8_t *) &out_8_v16i_ag_rb_vectors[6];
	uint8_t* rb4 = (uint8_t *) &out_8_v16i_ag_rb_vectors[7];

	uint8_t* rgb1 = (uint8_t *) &in_3_v8i_rgb24_vectors[0];
	uint8_t* rgb2 = (uint8_t *) &in_3_v8i_rgb24_vectors[1];
	uint8_t* rgb3 = (uint8_t *) &in_3_v8i_rgb24_vectors[2];

	// reset out vectors
	out_8_v16i_ag_rb_vectors[0] = _mm_setzero_si128();
	out_8_v16i_ag_rb_vectors[1] = _mm_setzero_si128();
	out_8_v16i_ag_rb_vectors[2] = _mm_setzero_si128();
	out_8_v16i_ag_rb_vectors[3] = _mm_setzero_si128();
	out_8_v16i_ag_rb_vectors[4] = _mm_setzero_si128();
	out_8_v16i_ag_rb_vectors[5] = _mm_setzero_si128();
	out_8_v16i_ag_rb_vectors[6] = _mm_setzero_si128();
	out_8_v16i_ag_rb_vectors[7] = _mm_setzero_si128();

	ag1[2] = rgb1[1];
	ag1[6] = rgb1[4];
	ag1[10] = rgb1[7];
	ag1[14] = rgb1[10];

	rb1[0] = rgb1[0];
	rb1[2] = rgb1[2];
	rb1[4] = rgb1[3];
	rb1[6] = rgb1[5];
	rb1[8] = rgb1[6];
	rb1[10] = rgb1[8];
	rb1[12] = rgb1[9];
	rb1[14] = rgb1[11];

	ag2[2] = rgb1[13];
	ag2[6] = rgb2[0];
	ag2[10] = rgb2[3];
	ag2[14] = rgb2[6];

	rb2[0] = rgb1[12];
	rb2[2] = rgb1[14];
	rb2[4] = rgb1[15];
	rb2[6] = rgb2[1];
	rb2[8] = rgb2[2];
	rb2[10] = rgb2[4];
	rb2[12] = rgb2[5];
	rb2[14] = rgb2[7];

	ag3[2] = rgb1[9];
	ag3[6] = rgb2[12];
	ag3[10] = rgb2[15];
	ag3[14] = rgb3[2];

	rb3[0] = rgb2[8];
	rb3[2] = rgb2[10];
	rb3[4] = rgb2[11];
	rb3[6] = rgb2[13];
	rb3[8] = rgb2[14];
	rb3[10] = rgb3[0];
	rb3[12] = rgb3[1];
	rb3[14] = rgb3[3];

	ag4[2] = rgb3[5];
	ag4[6] = rgb3[8];
	ag4[10] = rgb3[11];
	ag4[14] = rgb3[14];

	rb4[0] = rgb3[4];
	rb4[2] = rgb3[6];
	rb4[4] = rgb3[7];
	rb4[6] = rgb3[9];
	rb4[8] = rgb3[10];
	rb4[10] = rgb3[12];
	rb4[12] = rgb3[13];
	rb4[14] = rgb3[15];
};


/*
 * Convert 3 vectors of 16 char RGB24 to 4 vectors of 8 short AG1, RB1, AG2 & RB2,
 * AG3, RB3, AG4 & RB4
 *
 * Total latency:				4
 * Number of pixels handled:	16
 *
 * INPUT
 * 3 vectors of 16 char
 *
 * R1 G1	B1 R2	G2 B2	R3 G3	B3 R4	G4 B4	R5 G5	B5 R6
 *
 * G6 B6	R7 G7	B7 R8	G8 B8	R9 G9	B9 R10	G10 B10	R11 G11
 *
 * B11 R12	G12 B12	R13 G13	B13 R14	G14 B14	R15 G15	B15 R16 G16 B16
 *
 *
 * OUTPUT:
 * 8 vectors of 8 short
 *
 * agVect1
 * A1 0		G1 0	A2 0	G2 0	A3 0	G3 0	A4 0	G4 0
 *
 * rbVect1
 * R1 0		B1 0	R2 0	B2 0	R3 0	B3 0	R4 0	B4 0
 *
 * agVect2
 * A5 0		G5 0	A6 0	G6 0	A7 0	G7 0	A8 0	G8 0
 *
 * rbVect2
 * R5 0		B5 0	R6 0	B6 0	R7 0	B7 0	R8 0	B8 0
 *
 * agVect3
 * A9 0		G9 0	A10 0	G10 0	A11 0	G11 0	A12 0	G12 0
 *
 * rbVect3
 * R9 0		B9 0	R10 0	B10 0	R11 0	B11 0	R12 0	B12 0
 *
 * agVect4
 * A13 0	G13 0	A14 0	G14 0	A15 0	G15 0	A16 0	G16 0
 *
 * rbVect4
 * R13 0	B13 0	R14 0	B14 0	R15 0	B15 0	R16 0	B16 0
 */
EXTERN_INLINE void unpack_rgb24_to_ag_rb_vectors_sse2_ssse3(__m128i* in_3_v8i_rgb24_vectors, __m128i* out_8_v16i_ag_rb_vectors)
{
	CONST_M128I(shuf_ag1, 0xFF04FFFFFF01FFFFLL, 0xFF0AFFFFFF07FFFFLL);
	CONST_M128I(shuf_rb1, 0xFF05FF03FF02FF00LL, 0xFF0BFF09FF08FF06LL);
	CONST_M128I(shuf_ag21, 0xFFFFFFFFFF0DFFFFLL, 0xFFFFFFFFFFFFFFFFLL);
	CONST_M128I(shuf_ag22, 0xFFF00FFFFFFFFFFFLL, 0xFFFFFF06FFFFFF03LL);
	CONST_M128I(shuf_rb21, 0xFFFFFF0FFF0EFF0CLL, 0xFFFFFFFFFFFFFFFFLL);
	CONST_M128I(shuf_rb22, 0xFF01FFFFFFFFFFFFLL, 0xFF07FF05FF04FF02LL);
	CONST_M128I(shuf_ag31, 0xFF0CFFFFFF09FFFFLL, 0xFFFFFFFFFF0FFFFFLL);
	CONST_M128I(shuf_ag32, 0xFFFFFFFFFFFFFFFFLL, 0xFFF02FFFFFFFFFFFLL);
	CONST_M128I(shuf_rb31, 0xFF0DFF0BFF0AFF08LL, 0xFFFFFFFFFFFFFF0ELL);
	CONST_M128I(shuf_rb32, 0xFFFFFFFFFFFFFFFFLL, 0xFF03FF01FF00FFFFLL);
	CONST_M128I(shuf_ag4, 0xFF08FFFFFF05FFFFLL, 0xFF0EFFFFFF0BFFFFLL);
	CONST_M128I(shuf_rb4, 0xFF09FF07FF06FF04LL, 0xFF0FFF0DFF0CFF0ALL);


	out_8_v16i_ag_rb_vectors[0] = _mm_shuffle_epi8(in_3_v8i_rgb24_vectors[0], _M(shuf_ag1));// PSHUFB	1	0.5
	// A1 0		G1 0	A2 0	G2 0	A3 0	G3 0	A4 0	G4 0

	out_4_v16i_ag_rb_vectors[1] = _mm_shuffle_epi8(in_3_v8i_rgb24_vectors[0], _M(shuf_rb1));// PSHUFB	1	0.5
	// R1 0		B1 0	R2 0	B2 0	R3 0	B3 0	R4 0	B4 0

	out_4_v16i_ag_rb_vectors[2] = _mm_shuffle_epi8(in_3_v8i_rgb24_vectors[0], _M(shuf_ag21));// PSHUFB	1	0.5
	// A5 0		G5 0	0 0		0 0		0 0		0 0		0 0		0 0
	out_4_v16i_ag_rb_vectors[2] = _mm_por_si128(out_4_v16i_ag_rb_vectors[2], _mm_shuffle_epi8(in_3_v8i_rgb24_vectors[1], _M(shuf_ag22)));
	// A5 0		G5 0	A6 0	G6 0	A7 0	G7 0	A8 0	G8 0						// POR		1	0.33
																							// PSHUFB	1	0.5

	out_4_v16i_ag_rb_vectors[3] = _mm_shuffle_epi8(in_3_v8i_rgb24_vectors[0], _M(shuf_rb21));// PSHUFB	1	0.5
	// R5 0		B5 0	0 0		0 0		0 0		0 0		0 0		0 0
	out_4_v16i_ag_rb_vectors[3] = _mm_por_si128(out_4_v16i_ag_rb_vectors[3], _mm_shuffle_epi8(in_3_v8i_rgb24_vectors[1], _M(shuf_rb22)));
	// R5 0		B5 0	R6 0	B6 0	R7 0	B7 0	R8 0	B8 0						// POR		1	0.33
																							// PSHUFB	1	0.5

	out_4_v16i_ag_rb_vectors[4] = _mm_shuffle_epi8(in_3_v8i_rgb24_vectors[1], _M(shuf_ag31));// PSHUFB	1	0.5
	// A9 0		G9 0	A10 0	G10 0	A11 0	G11 0	0 0		0 0
	out_4_v16i_ag_rb_vectors[4] = _mm_por_si128(out_4_v16i_ag_rb_vectors[4], _mm_shuffle_epi8(in_3_v8i_rgb24_vectors[2], _M(shuf_ag32)));
	// A9 0		G9 0	A10 0	G10 0	A1 0	G11 0	A12 0	G12 0						// POR		1	0.33
																							// PSHUFB	1	0.5

	out_4_v16i_ag_rb_vectors[5] = _mm_shuffle_epi8(in_3_v8i_rgb24_vectors[1], _M(shuf_rb31));// PSHUFB	1	0.5
	// R9 0		B9 0	R10 0	B10 0	R11 0	0 0		0 0		0 0
	out_4_v16i_ag_rb_vectors[5] = _mm_por_si128(out_4_v16i_ag_rb_vectors[5], _mm_shuffle_epi8(in_3_v8i_rgb24_vectors[2], _M(shuf_rb32)));
	// R9 0		B9 0	R10 0	B10 0	R11 0	B11 0	R12 0	B12 0						// POR		1	0.33
																							// PSHUFB	1	0.5

	out_8_v16i_ag_rb_vectors[6] = _mm_shuffle_epi8(in_3_v8i_rgb24_vectors[2], _M(shuf_ag4));// PSHUFB	1	0.5
	// A13 0	G13 0	A14 0	G14 0	A15 0	G15 0	A16 0	G16 0

	out_4_v16i_ag_rb_vectors[7] = _mm_shuffle_epi8(in_3_v8i_rgb24_vectors[2], _M(shuf_rb4));// PSHUFB	1	0.5
	// R13 0	B13 0	R14 0	B14 0	R15 0	B15 0	R16 0	B16 0
};


/*
 * Convert 2 vectors of 16 char RGB24 to 6 vectors of 8 short:
 *
 * Total latency:				x
 * Number of pixels handled:	16
 *
 * INPUT
 * 3 vectors of 16 char
 * R1 G1	B1 R2	G2 B2	R3 G3	B3 R4	G4 B4	R5 G5	B5 R6
 *
 * G6 B6	R7 G7	B7 R8	G8 B8	R9 G9	B9 R10	G10 B10	R11 G11
 *
 * B11 R12	G12 B12	R13 G13	B13 R14	G14 B14	R15 G15	B15 R16 G16 B16
 *
 * OUTPUT:
 *
 * 6 vectors of 8 short
 * rVect
 * R1 0		R2 0	R3 0	R4 0	R5 0	R6 0	R7 0	R8 0
 *
 * gVect
 * G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0
 *
 * bVect
 * B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
 *
 * rVect
 * R9 0		R10 0	R11 0	R12 0	R13 0	R14 0	R15 0	R16 0
 *
 * gVect
 * G9 0		G10 0	G11 0	G12 0	G13 0	G14 0	G15 0	G16 0
 *
 * bVect
 * B9 0		B10 0	B11 0	B12 0	B13 0	B14 0	B15 0	B16 0
 */
EXTERN_INLINE void unpack_argb_to_r_g_b_vectors_sse2(__m128i* in_3_v8i_rgb24_vectors, __m128i* out_6_v16i_r_g_b_vectors)
{
	CONST_M128I(mask_off_agb, 0x0000FF000000FF00LL, 0x0000FF000000FF00LL);
	CONST_M128I(mask_off_arb, 0x00FF000000FF0000LL, 0x00FF000000FF0000LL);
	CONST_M128I(mask_off_arg, 0xFF000000FF000000LL, 0xFF000000FF000000LL);
	M128I(scratch1, 0x0LL, 0x0LL);

	out_3_v16i_r_g_b_vectors[0] = _mm_and_si128(in_2_v8i_argb_vectors[0], _M(mask_off_agb));// PAND		1	0.33
	// 0 R1 	0 0		0 R2	0 0		0 R3 	0 0		0 R4 	0 0

	out_3_v16i_r_g_b_vectors[0] = _mm_srli_epi16(out_3_v16i_r_g_b_vectors[0], 8);			// PSRLW	1	1
	// R1  0	0 0		R2 0	0 0		R3 0	0 0		R4 0	0 0

	_M(scratch1) = _mm_and_si128(in_2_v8i_argb_vectors[1], _M(mask_off_agb));				// PAND		1	0.33
	// 0 R5 	0 0		0 R6	0 0		0 R7 	0 0		0 R8 	0 0

	_M(scratch1) = _mm_srli_epi16(_M(scratch1), 8);											// PSRLW	1	1
	// R5  0	0 0		R6 0	0 0		R7 0	0 0		R8 0	0 0

	out_3_v16i_r_g_b_vectors[0] = _mm_packs_epi32(out_3_v16i_r_g_b_vectors[0], _M(scratch1));//PACKSSDW	1	0.5
	// R1 0		R2 0	R3 0	R4 0	R5 0	R6 0	R7 0	R8 0



	out_3_v16i_r_g_b_vectors[1] = _mm_and_si128(in_2_v8i_argb_vectors[0], _M(mask_off_arb));// PAND		1	0.33
	// 0 0		G1 0	0 0		G2 0	0 0		G3 0	0 0		G4 0

	out_3_v16i_r_g_b_vectors[1] = _mm_srli_epi32(out_3_v16i_r_g_b_vectors[1], 16);			// PSRLD	1	1
	// G1 0 	0 0		G2 0	0 0		G3 0	0 0		G4 0 	0 0

	_M(scratch1) = _mm_and_si128(in_2_v8i_argb_vectors[1], _M(mask_off_arb));				// PAND		1	0.33
	// 0 0		G5 0	0 0		G6 0	0 0		G7 0	0 0		G8 0

	_M(scratch1) = _mm_srli_epi32(_M(scratch1), 16);										// PSRLD	1	1
	// G5 0 	0 0		G6 0	0 0		G7 0	0 0		G8 0 	0 0

	out_3_v16i_r_g_b_vectors[1] = _mm_packs_epi32(out_3_v16i_r_g_b_vectors[1], _M(scratch1));//PACKSSDW	1	0.5
	// G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0



	out_3_v16i_r_g_b_vectors[2] = _mm_and_si128(in_2_v8i_argb_vectors[0], _M(mask_off_arg));// PAND		1	0.33
	// 0 0		0 B1	0 0		0 B2	0 0		0 B3	0 0		0 B4

	out_3_v16i_r_g_b_vectors[2] = _mm_srli_epi32(out_3_v16i_r_g_b_vectors[2], 24);			// PSRLD	1	1
	// B1 0 	0 0		B2 0	0 0		B3 0	0 0		B4 0 	0 0

	_M(scratch1) = _mm_and_si128(in_2_v8i_argb_vectors[1], _M(mask_off_arg));				// PAND		1	0.33
	// 0 0		0 B5	0 0		0 B6	0 0		0 B7	0 0		0 B8

	_M(scratch1) = _mm_srli_epi32(_M(scratch1), 24);										// PSRLD	1	1
	// B5 0 	0 0		B6 0	0 0		B7 0	0 0		B8 0 	0 0

	out_3_v16i_r_g_b_vectors[2] = _mm_packs_epi32(out_3_v16i_r_g_b_vectors[2], _M(scratch1));//PACKSSDW	1	0.5
	// B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
};


/*
 * Convert 2 vectors of 16 char ARGB to 3 vectors of 8 short:
 *
 * Total latency:				9
 * Number of pixels handled:	8
 *
 * INPUT
 * 2 vectors of 16 char
 * A1  R1	G1  B1	A2  R2	G2  B2	A3  R3	G3  B3	A4  R4	G4  B4
 *
 * A5  R5	G5  B5	A6  R6	G6  B6	A7  R7	G7  B7	A8  R8	G8  B8
 *
 * OUTPUT:
 *
 * 3 vectors of 8 short
 * rVect
 * R1  0	R2  0	R3  0	R4  0	R5  0	R6  0	R7  0	R8  0
 *
 * gVect
 * G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0
 *
 * bVect
 * B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
 */
EXTERN_INLINE void unpack_argb_to_r_g_b_vectors_sse2_ssse3(__m128i* in_2_v8i_argb_vectors, __m128i* out_3_v16i_r_g_b_vectors)
{
	CONST_M128I(shuf_r1, 0xFF0DFF09FF05FF01LL, 0xFFFFFFFFFFFFFFFFLL);
	CONST_M128I(shuf_r2, 0xFFFFFFFFFFFFFFFFLL, 0xFF0DFF09FF05FF01LL);
	CONST_M128I(shuf_g1, 0xFF0EFF0AFF06FF02LL, 0xFFFFFFFFFFFFFFFFLL);
	CONST_M128I(shuf_g2, 0xFFFFFFFFFFFFFFFFLL, 0xFF0EFF0AFF06FF02LL);
	CONST_M128I(shuf_b1, 0xFF0FFF0BFF07FF03LL, 0xFFFFFFFFFFFFFFFFLL);
	CONST_M128I(shuf_b2, 0xFFFFFFFFFFFFFFFFLL, 0xFF0FFF0BFF07FF03LL);

	M128I(scratch1, 0x0LL, 0x0LL);

	_M(scratch1) = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_r1));					// PSHUFB	1	0.5
	// R1 0		R2 0	R3 0	R4 0	0 0		0 0		0 0		0 0

	out_3_v16i_r_g_b_vectors[0] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_r2));	// PSHUFB	1	0.5
	// 0 0		0 0		0 0		0 0		R5 0	R6 0	R7 0	R8 0

	out_3_v16i_r_g_b_vectors[0] = _mm_or_si128(out_3_v16i_r_g_b_vectors[0], _M(scratch1));	// POR		1	0.33
	// R1  0	R2  0	R3  0	R4  0	R5  0	R6  0	R7  0	R8  0


	_M(scratch1) = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_g1));					// PSHUFB	1	0.5
	// G1 0		G2 0	G3 0	G4 0	0 0		0 0		0 0		0 0

	out_3_v16i_r_g_b_vectors[1] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_g2));	// PSHUFB	1	0.5
	// 0 0		0 0		0 0		0 0		G5 0	G6 0	G7 0	G8 0

	out_3_v16i_r_g_b_vectors[1] = _mm_or_si128(out_3_v16i_r_g_b_vectors[1], _M(scratch1));	// POR		1	0.33
	// G1  0	G2 0	G3  0	G4  0	G5  0	G6  0	G7  0	G8  0


	_M(scratch1) = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_b1));					// PSHUFB	1	0.5
	// B1 0		B2 0	B3 0	B4 0	0 0		0 0		0 0		0 0

	out_3_v16i_r_g_b_vectors[2] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_b2));	// PSHUFB	1	0.5
	// 0 0		0 0		0 0		0 0		B5 0	B6 0	B7 0	B8 0

	out_3_v16i_r_g_b_vectors[2] = _mm_or_si128(out_3_v16i_r_g_b_vectors[2], _M(scratch1));	// POR		1	0.33
	// B1  0	B2 0	B3  0	B4  0	B5  0	B6  0	B7  0	B8  0
};


#endif /* RGB_UNPACK_H_ */
