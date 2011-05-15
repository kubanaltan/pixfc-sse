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
#include <tmmintrin.h>

#include "platform_util.h"


/*
 * Convert 2 vectors of 16 char ARGB to 3 vectors of 8 short:
 *
 * Total latency:				28
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
 * gb1Vect
 * G1 0		B1 0	G2 0	B2 0	G3 0	B3 0	G4 0	B4 0
 *
 * gb2Vect
 * G5 0		B5 0	G6 0	B6 0	G7 0	B7 0	G8 0	B8 0
 */
EXTERN_INLINE void unpack_argb_to_r_gb_vectors_sse2(__m128i* in_2_v8i_argb_vectors, __m128i* out_3_v16i_r_gb_vectors)
{
	CONST_M128I(mask_off_gb, 0x000000FF000000FFLL, 0x000000FF000000FFLL);
	CONST_M128I(mask_off_rb, 0x0000FF000000FF00LL, 0x0000FF000000FF00LL);
	CONST_M128I(mask_off_rg, 0x00FF000000FF0000LL, 0x00FF000000FF0000LL);
	M128I(scratch1, 0x0LL, 0x0LL);
	M128I(scratch2, 0x0LL, 0x0LL);

	_M(scratch1) = _mm_srli_epi32(in_2_v8i_argb_vectors[0], 8);								// PSRLD	2	2
	// R1 G1	B1 0	R2 G2	B2 0	R3 G3	B3 0	R4 G4	B4 0

	out_3_v16i_r_gb_vectors[0] = _mm_and_si128(_M(scratch1), _M(mask_off_gb));				// PAND		2   2
	// R1 0		0 0		R2 0	0 0		R3 0	0 0		R4 0	0 0

	out_3_v16i_r_gb_vectors[1] = _mm_and_si128(_M(scratch1), _M(mask_off_rb));				// PAND		2   2
	// 0 G1 	0 0		0 G2	0 0		0 G3	0 0		0 G4 	0 0

	out_3_v16i_r_gb_vectors[1] = _mm_srli_epi32(out_3_v16i_r_gb_vectors[1], 8);				// PSRLD	2	2
	// G1 0 	0 0		G2 0	0 0		G3 0	0 0		G4 0 	0 0

	_M(scratch1) = _mm_and_si128(_M(scratch1), _M(mask_off_rg));							// PAND		2   2
	// 0 0		B1 0	0 0		B2 0	0 0		B3 0	0 0		B4 0

	out_3_v16i_r_gb_vectors[1] = _mm_or_si128(out_3_v16i_r_gb_vectors[1], _M(scratch1));	// POR		2   2
	// G1 0		B1 0	G2 0	B2 0	G3 0	B3 0	G4 0	B4 0

	_M(scratch1) = _mm_srli_epi32(in_2_v8i_argb_vectors[1], 8);								// PSRLD	2	2
	// R5 G5	B5 0	R6 G6	B6 0	R7 G7	B7 0	R8 G8	B8 0

	_M(scratch2) = _mm_and_si128(_M(scratch1), _M(mask_off_gb));							// PAND		2   2
	// R5 0		0 0		R6 0	0 0		R7 0	0 0		R8 0	0 0

	out_3_v16i_r_gb_vectors[0] = _mm_packs_epi32(out_3_v16i_r_gb_vectors[0], _M(scratch2));//  PACKSSDW	4 4 2 2
	// R1 0		R2 0	R3 0	R4 0	R5 0	R6 0	R7 0	R8 0

	out_3_v16i_r_gb_vectors[2] = _mm_and_si128(_M(scratch1), _M(mask_off_rb));				// PAND		2   2
	// 0 G5 	0 0		0 G6	0 0		0 G7	0 0		0 G8 	0 0

	out_3_v16i_r_gb_vectors[2] = _mm_srli_epi32(out_3_v16i_r_gb_vectors[2], 8);				// PSRLD	2	2
	// G5 0 	0 0		G6 0	0 0		G7 0	0 0		G8 0 	0 0

	_M(scratch1) = _mm_and_si128(_M(scratch1), _M(mask_off_rg));							// PAND 	2   2
	// 0 0		B5 0	0 0		B6 0	0 0		B7 0	0 0		B8 0

	out_3_v16i_r_gb_vectors[2] = _mm_or_si128(out_3_v16i_r_gb_vectors[2], _M(scratch1));	// POR		2   2
	// G5 0		B5 0	G6 0	B6 0	G7 0	B7 0	G8 0	B8 0
};


/*
 * Convert 2 vectors of 16 char ARGB to 3 vectors of 8 short:
 *
 * Total latency:				6
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
 * gb1Vect
 * G1 0		B1 0	G2 0	B2 0	G3 0	B3 0	G4 0	B4 0
 *
 * gb2Vect
 * G5 0		B5 0	G6 0	B6 0	G7 0	B7 0	G8 0	B8 0
 */
EXTERN_INLINE void unpack_argb_to_r_gb_vectors_sse2_ssse3(__m128i* in_2_v8i_argb_vectors, __m128i* out_3_v16i_r_gb_vectors)
{
	CONST_M128I(shuf_r1, 0xFF0DFF09FF05FF01LL, 0xFFFFFFFFFFFFFFFFLL);
	CONST_M128I(shuf_r2, 0xFFFFFFFFFFFFFFFFLL, 0xFF0DFF09FF05FF01LL);
	CONST_M128I(shuf_gb, 0xFF07FF06FF03FF02LL, 0xFF0FFF0EFF0BFF0ALL);

	M128I(scratch1, 0x0LL, 0x0LL);

	_M(scratch1) = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_r1));					// PSHUFB	1 1 3 0 1 2
	// R1 0		R2 0	R3 0	R4 0	0 0		0 0		0 0		0 0

	out_3_v16i_r_gb_vectors[0] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_r2));	// PSHUFB	1 1 3 0 1 2
	// 0 0		0 0		0 0		0 0		R5 0	R6 0	R7 0	R8 0

	out_3_v16i_r_gb_vectors[0] = _mm_or_si128(out_3_v16i_r_gb_vectors[0], _M(scratch1));	// POR		2   2
	// R1  0	R2  0	R3  0	R4  0	R5  0	R6  0	R7  0	R8  0

	out_3_v16i_r_gb_vectors[1] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_gb));	// PSHUFB	1 1 3 0 1 2
	// G1 0		B1 0	G2 0	B2 0	G3 0	B3 0	G4 0	B4 0

	out_3_v16i_r_gb_vectors[2] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_gb));	// PSHUFB	1 1 3 0 1 2
	// G5 0		B5 0	G6 0	B6 0	G7 0	B7 0	G8 0	B8 0
};









/*
 * Convert 2 vectors of 16 char ARGB to 3 vectors of 8 short:
 *
 * Total latency:				36
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
	
	out_3_v16i_r_g_b_vectors[0] = _mm_and_si128(in_2_v8i_argb_vectors[0], _M(mask_off_agb));// PAND		2	2
	// 0 R1 	0 0		0 R2	0 0		0 R3 	0 0		0 R4 	0 0

	out_3_v16i_r_g_b_vectors[0] = _mm_srli_epi16(out_3_v16i_r_g_b_vectors[0], 8);			// PSRLW	2   2
	// R1  0	0 0		R2 0	0 0		R3 0	0 0		R4 0	0 0
	
	_M(scratch1) = _mm_and_si128(in_2_v8i_argb_vectors[1], _M(mask_off_agb));				// PAND		2	2
	// 0 R5 	0 0		0 R6	0 0		0 R7 	0 0		0 R8 	0 0

	_M(scratch1) = _mm_srli_epi16(_M(scratch1), 8);											// PSRLW	2   2
	// R5  0	0 0		R6 0	0 0		R7 0	0 0		R8 0	0 0
	
	out_3_v16i_r_g_b_vectors[0] = _mm_packs_epi32(out_3_v16i_r_g_b_vectors[0], _M(scratch1));//PACKSSDW	4 4 2 2
	// R1 0		R2 0	R3 0	R4 0	R5 0	R6 0	R7 0	R8 0



	out_3_v16i_r_g_b_vectors[1] = _mm_and_si128(in_2_v8i_argb_vectors[0], _M(mask_off_arb));// PAND		2	2
	// 0 0		G1 0	0 0		G2 0	0 0		G3 0	0 0		G4 0
	
	out_3_v16i_r_g_b_vectors[1] = _mm_srli_epi32(out_3_v16i_r_g_b_vectors[1], 16);			// PSRLD	2   2
	// G1 0 	0 0		G2 0	0 0		G3 0	0 0		G4 0 	0 0
	
	_M(scratch1) = _mm_and_si128(in_2_v8i_argb_vectors[1], _M(mask_off_arb));				// PAND		2	2
	// 0 0		G5 0	0 0		G6 0	0 0		G7 0	0 0		G8 0
	
	_M(scratch1) = _mm_srli_epi32(_M(scratch1), 16);										// PSRLD	2   2
	// G5 0 	0 0		G6 0	0 0		G7 0	0 0		G8 0 	0 0
	
	out_3_v16i_r_g_b_vectors[1] = _mm_packs_epi32(out_3_v16i_r_g_b_vectors[1], _M(scratch1));//PACKSSDW	4 4 2 2
	// G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0
	
	

	out_3_v16i_r_g_b_vectors[2] = _mm_and_si128(in_2_v8i_argb_vectors[0], _M(mask_off_arg));// PAND		2	2
	// 0 0		0 B1	0 0		0 B2	0 0		0 B3	0 0		0 B4

	out_3_v16i_r_g_b_vectors[2] = _mm_srli_epi32(out_3_v16i_r_g_b_vectors[2], 24);			// PSRLD	2   2
	// B1 0 	0 0		B2 0	0 0		B3 0	0 0		B4 0 	0 0

	_M(scratch1) = _mm_and_si128(in_2_v8i_argb_vectors[1], _M(mask_off_arg));				// PAND		2	2
	// 0 0		0 B5	0 0		0 B6	0 0		0 B7	0 0		0 B8

	_M(scratch1) = _mm_srli_epi32(_M(scratch1), 24);										// PSRLD	2   2
	// B5 0 	0 0		B6 0	0 0		B7 0	0 0		B8 0 	0 0

	out_3_v16i_r_g_b_vectors[2] = _mm_packs_epi32(out_3_v16i_r_g_b_vectors[2], _M(scratch1));//PACKSSDW	4 4 2 2
	// B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
};


/*
 * Convert 2 vectors of 16 char ARGB to 3 vectors of 8 short:
 *
 * Total latency:				12
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
	
	_M(scratch1) = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_r1));					// PSHUFB	1 1 3 0 1 2
	// R1 0		R2 0	R3 0	R4 0	0 0		0 0		0 0		0 0
	
	out_3_v16i_r_g_b_vectors[0] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_r2));	// PSHUFB	1 1 3 0 1 2
	// 0 0		0 0		0 0		0 0		R5 0	R6 0	R7 0	R8 0
	
	out_3_v16i_r_g_b_vectors[0] = _mm_or_si128(out_3_v16i_r_g_b_vectors[0], _M(scratch1));	// POR		2   2
	// R1  0	R2  0	R3  0	R4  0	R5  0	R6  0	R7  0	R8  0

	
	_M(scratch1) = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_g1));					// PSHUFB	1 1 3 0 1 2
	// G1 0		G2 0	G3 0	G4 0	0 0		0 0		0 0		0 0
	
	out_3_v16i_r_g_b_vectors[1] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_g2));	// PSHUFB	1 1 3 0 1 2
	// 0 0		0 0		0 0		0 0		G5 0	G6 0	G7 0	G8 0
	
	out_3_v16i_r_g_b_vectors[1] = _mm_or_si128(out_3_v16i_r_g_b_vectors[1], _M(scratch1));	// POR		2   2
	// G1  0	G2 0	G3  0	G4  0	G5  0	G6  0	G7  0	G8  0

	
	_M(scratch1) = _mm_shuffle_epi8(in_2_v8i_argb_vectors[0], _M(shuf_b1));					// PSHUFB	1 1 3 0 1 2
	// B1 0		B2 0	B3 0	B4 0	0 0		0 0		0 0		0 0
	
	out_3_v16i_r_g_b_vectors[2] = _mm_shuffle_epi8(in_2_v8i_argb_vectors[1], _M(shuf_b2));	// PSHUFB	1 1 3 0 1 2
	// 0 0		0 0		0 0		0 0		B5 0	B6 0	B7 0	B8 0
	
	out_3_v16i_r_g_b_vectors[2] = _mm_or_si128(out_3_v16i_r_g_b_vectors[2], _M(scratch1));	// POR		2   2
	// B1  0	B2 0	B3  0	B4  0	B5  0	B6  0	B7  0	B8  0
};






/*
 * Create 3 422 downsampled R, G, B vectors from 3 R, G, B vectors
 * using nearest neighbour interpolation
 *
 * TOTAL LATENCY:	12
 *
 * INPUT:
 * 3 vectors of 8 short
 * rVect
 * R1  0	R2  0	R3  0	R4  0	R5  0	R6  0	R7  0	R8  0
 *
 * gVect
 * G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0
 *
 * bVect
 * B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
 *
 * OUTPUT:
 * 3 vectors of 8 short
 * rVect
 * R1 0 R1 0	R3 0 R3	0	R5 0 R5	0	R7 0 R7 0
 *
 * gVect
 * G1 0 G1 0	G3 0 G3 0	G5 0 G5	0	G7 0 G7	0
 *
 * bVect
 * B1 0 B1 0	B3 0 B3 0	B5 0 B5	0	B7 0 B7	0	
 */
EXTERN_INLINE void	nnb_422_downsample_r_g_b_vectors_sse2(__m128i* in_3_v16i_r_g_b_vectors, __m128i *out_3_v16i_nnb_422_r_g_b_vectors) {
	M128I(scratch1, 0x0LL, 0x0LL);

	_M(scratch1) = _mm_shufflehi_epi16(in_3_v16i_r_g_b_vectors[0], 0xA0);			//	PSHUFHW		2	2
	// R1 0 R2 0	R3 0 R4 0	R5 0 R5 0	R7 0 R7 0
	
	out_3_v16i_nnb_422_r_g_b_vectors[0] = _mm_shufflelo_epi16(_M(scratch1), 0xA0);	//	PSHUFLW		2	2
	// R1 0 R1 0	R3 0 R3 0	R5 0 R5 0	R7 0 R7 0
	
	_M(scratch1) = _mm_shufflehi_epi16(in_3_v16i_r_g_b_vectors[1], 0xA0);			//	PSHUFHW		2	2
	// G1 0 G2 0	G3 0 G3 0	G5 0 G5	0	G7 0 G7	0
	
	out_3_v16i_nnb_422_r_g_b_vectors[1] = _mm_shufflelo_epi16(_M(scratch1), 0xA0);	//	PSHUFLW		2	2
	// G1 0 G1 0	G3 0 G3 0	G5 0 G5	0	G7 0 G7	0
	
	_M(scratch1) = _mm_shufflehi_epi16(in_3_v16i_r_g_b_vectors[2], 0xA0);			//	PSHUFHW		2	2
	// B1 0 B2 0	B3 0 B4 0	B5 0 B5	0	B7 0 B7	0
	
	out_3_v16i_nnb_422_r_g_b_vectors[2] = _mm_shufflelo_epi16(_M(scratch1), 0xA0);	//	PSHUFLW		2	2
	// B1 0 B1 0	B3 0 B3 0	B5 0 B5	0	B7 0 B7	0
}


/*
 * Create 3 422 downsampled R, G, B vectors from 3 422 R, G, B vectors
 * using nearest neighbour interpolation
 *
 * TOTAL LATENCY:	3
 *
 * INPUT:
 * 3 vectors of 8 short
 * rVect
 * R1  0	R2  0	R3  0	R4  0	R5  0	R6  0	R7  0	R8  0
 *
 * gVect
 * G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0
 *
 * bVect
 * B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
 *
 * OUTPUT:
 * 3 vectors of 8 short
 * rVect
 * R1 0 R1 0	R3 0 R3	0	R5 0 R5	0	R7 0 R7 0
 *
 * gVect
 * G1 0 G1 0	G3 0 G3 0	G5 0 G5	0	G7 0 G7	0
 *
 * bVect
 * B1 0 B1 0	B3 0 B3 0	B5 0 B5	0	B7 0 B7	0	
 */
EXTERN_INLINE void	nnb_422_downsample_r_g_b_vectors_sse2_ssse3(__m128i* in_3_v16i_r_g_b_vectors, __m128i *out_3_v16i_nnb_422_r_g_b_vectors)
{
	CONST_M128I(shuf_odd, 0xFF04FF04FF00FF00LL, 0xFF0CFF0CFF08FF08LL);
	
	out_3_v16i_nnb_422_r_g_b_vectors[0] = _mm_shuffle_epi8(in_3_v16i_r_g_b_vectors[0], _M(shuf_odd));// PSHUFB		1 1 3 0 1 2
	// R1 0 R1 0	R3 0 R3	0	R5 0 R5	0	R7 0 R7 0
	
	out_3_v16i_nnb_422_r_g_b_vectors[1] = _mm_shuffle_epi8(in_3_v16i_r_g_b_vectors[1], _M(shuf_odd));// PSHUFB		1 1 3 0 1 2
	// G1 0 G1 0	G3 0 G3 0	G5 0 G5	0	G7 0 G7	0
	
	out_3_v16i_nnb_422_r_g_b_vectors[2] = _mm_shuffle_epi8(in_3_v16i_r_g_b_vectors[2], _M(shuf_odd));// PSHUFB		1 1 3 0 1 2
	// B1 0 B1 0	B3 0 B3 0	B5 0 B5	0	B7 0 B7	0	
}


/*
 * Theory behind the following 422 downsampling average filter is taken from:
 * "Merging Computing with Studio Video: Converting Between R'G'B' and 4:2:2"
 * Charles Poynton
 * http://www.poynton.com/papers/Discreet_Logic/index.html
 *
 *
 * Input: 3 samples at t = {-1, 0, 1}
 * S-1	S0	S1
 *
 * Output: 1 sample
 * S01 = S-1 / 4 + S0 / 2 + S1 / 4
 */

/*
 * Create 3 422 downsampled R, G, B vectors from 3 R, G, B vectors
 * using a simple 3-tap average filter.
 *
 * TOTAL LATENCY:	60
 *
 * INPUT:
 * 3 vectors of 8 short
 * previous rVect
 * R1  0	R2  0	R3  0	R4  0	R5  0	R6  0	R7  0	R8  0
 *
 * previous gVect
 * G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0
 *
 * previous bVect
 * B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
 *
 * 3 vectors of 8 short
 * current rVect
 * R1  0	R2  0	R3  0	R4  0	R5  0	R6  0	R7  0	R8  0
 *
 * current gVect
 * G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0
 *
 * current bVect
 * B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
 *
 * OUTPUT:
 * 3 vectors of 8 short
 * rVect
 * R1 0 	R1 0	R3 0 	R3	0	R5 0 	R5	0	R7 0 	R7 0
 *
 * gVect
 * G1 0 	G1 0	G3 0 	G3 0	G5 0 	G5	0	G7 0 	G7	0
 *
 * bVect
 * B1 0 	B1 0	B3 0 	B3 0	B5 0 	B5	0	B7 0	B7	0
 */
EXTERN_INLINE void	avg_422_downsample_r_g_b_vectors_sse2(__m128i* in_3_v16i_current_r_g_b_vectors, __m128i* in_3_v16i_previous_r_g_b_vectors, __m128i *out_3_v16i_avg_422_r_g_b_vectors) {
	M128I(scratch1, 0x0LL, 0x0LL);
	M128I(scratch2, 0x0LL, 0x0LL);
	M128I(scratch3, 0x0LL, 0x0LL);

	//
	// construct a vector of R samples at t = {-1, 1, 3, 5}
	// so we can then average it with R samples at t = {1, 3, 5, 7}.
	// The result is then averaged with R samples at t = {0, 2, 4, 6}
	// Since we are doing 422 downsampling, each sample at time t has
	// a value equal to S-1 / 4 + S0 / 2 + S1 / 4
	_M(scratch1) = _mm_srli_si128(in_v16i_previous_r_g_b_vectors[0], 14);			// PSRLDQ		4	4
	// R8 0		0 0		0 0		0 0		0 0		0 0		0 0		0 0

	_M(scratch2) = _mm_srli_epi32(in_3_v16i_r_g_b_vectors[0], 2);					// PSRLD		2	2
	// R2 0		0 0		R4 0	0 0		R6 0	0 0		R8 0	0 0

	_M(scratch3) = _mm_slli_si128(_M(scratch2), 4);									// PSLLDQ		4	4
	// 0 0		0 0		R2 0	0 0		R4 0	0 0		R6 0	0 0

	_M(scratch1) = _mm_and_si128(_M(scratch1), _M(scratch3));						// PAND			2	2
	// R8 0		0 0		R2 0	0 0		R4 0	0 0		R6 0	0 0
	// (R8 belongs to the previous pixel)

	// Average S-1 and S1
	_M(scratch1) = _mm_avg_epu16(_M(scratch1), _M(scratch2));						// PAVGW		2	2
	// Average the previous result with S0
	_M(scratch1) = _mm_avg_epu16(_M(scratch1), in_3_v16i_r_g_b_vectors[0]);			// PAVGW		2	2

	// Duplicate samples at t = {0,2,4,6}
	_M(scratch1) = _mm_shufflehi_epi16(_M(scratch1), 0xA0);							// PSHUFHW		2	2
	// R1 0 xx 0	R3 0 xx 0	R5 0 R5 0	R7 0 R7 0
	out_3_v16i_nnb_422_r_g_b_vectors[0] = _mm_shufflelo_epi16(_M(scratch1), 0xA0);	// PSHUFLW		2	2
	// R1 0 R1 0	R3 0 R3 0	R5 0 R5 0	R7 0 R7 0


	//
	// And repeat for G ...
	_M(scratch1) = _mm_srli_si128(in_v16i_previous_r_g_b_vectors[1], 14);			// PSRLDQ		4	4
	// G8 0		0 0		0 0		0 0		0 0		0 0		0 0		0 0

	_M(scratch2) = _mm_srli_epi32(in_3_v16i_r_g_b_vectors[1], 2);					// PSRLD		2	2
	// G2 0		0 0		G4 0	0 0		G6 0	0 0		G8 0	0 0

	_M(scratch3) = _mm_slli_si128(_M(scratch2), 4);									// PSLLDQ		4	4
	// 0 0		0 0		G2 0	0 0		G4 0	0 0		G6 0	0 0

	_M(scratch1) = _mm_and_si128(_M(scratch1), _M(scratch3));						// PAND			2	2
	// G8 0		0 0		G2 0	0 0		G4 0	0 0		G6 0	0 0
	// (G8 belongs to the previous pixel)

	// Average S-1 and S1
	_M(scratch1) = _mm_avg_epu16(_M(scratch1), _M(scratch2));						// PAVGW		2	2
	// Average the previous result with S0
	_M(scratch1) = _mm_avg_epu16(_M(scratch1), in_3_v16i_r_g_b_vectors[1]);			// PAVGW		2	2

	// Duplicate samples at t = {0,2,4,6}
	_M(scratch1) = _mm_shufflehi_epi16(_M(scratch1), 0xA0);							// PSHUFHW		2	2
	// G1 0 xx 0	G3 0 xx 0	G5 0 G5 0	G7 0 G7 0
	out_3_v16i_nnb_422_r_g_b_vectors[1] = _mm_shufflelo_epi16(_M(scratch1), 0xA0);	// PSHUFLW		2	2
	// G1 0 G1 0	G3 0 G3 0	G5 0 G5 0	G7 0 G7 0


	//
	// And repeat for B
	_M(scratch1) = _mm_srli_si128(in_v16i_previous_r_g_b_vectors[2], 14);			// PSRLDQ		4	4
	// B8 0		0 0		0 0		0 0		0 0		0 0		0 0		0 0

	_M(scratch2) = _mm_srli_epi32(in_3_v16i_r_g_b_vectors[2], 2);					// PSRLD		2	2
	// B2 0		0 0		B4 0	0 0		B6 0	0 0		B8 0	0 0

	_M(scratch3) = _mm_slli_si128(_M(scratch2), 4);									// PSLLDQ		4	4
	// 0 0		0 0		B2 0	0 0		B4 0	0 0		B6 0	0 0

	_M(scratch1) = _mm_and_si128(_M(scratch1), _M(scratch3));						// PAND			2	2
	// B8 0		0 0		B2 0	0 0		B4 0	0 0		B6 0	0 0
	// (B8 belongs to the previous pixel)

	// Average S-1 and S1
	_M(scratch1) = _mm_avg_epu16(_M(scratch1), _M(scratch2));						// PAVGW		2	2
	// Average the previous result with S0
	_M(scratch1) = _mm_avg_epu16(_M(scratch1), in_3_v16i_r_g_b_vectors[2]);			// PAVGW		2	2

	// Duplicate samples at t = {0,2,4,6}
	_M(scratch1) = _mm_shufflehi_epi16(_M(scratch1), 0xA0);							// PSHUFHW		2	2
	// B1 0 xx 0	B3 0 xx 0	B5 0 B5 0	B7 0 B7 0
	out_3_v16i_nnb_422_r_g_b_vectors[2] = _mm_shufflelo_epi16(_M(scratch1), 0xA0);	// PSHUFLW		2	2
	// B1 0 B1 0	B3 0 B3 0	B5 0 B5 0	B7 0 B7 0
}


/*
 * Create 3 422 downsampled R, G, B vectors from 3 422 R, G, B vectors
 * using nearest neighbour interpolation
 *
 * TOTAL LATENCY:	3
 *
 * INPUT:
 * 3 vectors of 8 short
 * rVect
 * R1  0	R2  0	R3  0	R4  0	R5  0	R6  0	R7  0	R8  0
 *
 * gb1Vect
 * G1 0		G2 0	G3 0	G4 0	G5 0	G6 0	G7 0	G8 0
 *
 * gb2Vect
 * B1 0		B2 0	B3 0	B4 0	B5 0	B6 0	B7 0	B8 0
 *
 * OUTPUT:
 * 3 vectors of 8 short
 * rVect
 * R1 0 R1 0	R3 0 R3	0	R5 0 R5	0	R7 0 R7 0
 *
 * gVect
 * G1 0 G1 0	G3 0 G3 0	G5 0 G5	0	G7 0 G7	0
 *
 * bVect
 * B1 0 B1 0	B3 0 B3 0	B5 0 B5	0	B7 0 B7	0
 */
EXTERN_INLINE void	avg_422_downsample_r_g_b_vectors_sse2_ssse3(__m128i* in_3_v16i_r_g_b_vectors, __m128i *out_3_v16i_nnb_422_r_g_b_vectors)
{
	// TODO : write me
//	CONST_M128I(shuf_odd, 0xFF04FF04FF00FF00LL, 0xFF0CFF0CFF08FF08LL);
//
//	out_3_v16i_nnb_422_r_g_b_vectors[0] = _mm_shuffle_epi8(in_3_v16i_r_g_b_vectors[0], _M(shuf_odd));// PSHUFB		1 1 3 0 1 2
//	// R1 0 R1 0	R3 0 R3	0	R5 0 R5	0	R7 0 R7 0
//
//	out_3_v16i_nnb_422_r_g_b_vectors[1] = _mm_shuffle_epi8(in_3_v16i_r_g_b_vectors[1], _M(shuf_odd));// PSHUFB		1 1 3 0 1 2
//	// G1 0 G1 0	G3 0 G3 0	G5 0 G5	0	G7 0 G7	0
//
//	out_3_v16i_nnb_422_r_g_b_vectors[2] = _mm_shuffle_epi8(in_3_v16i_r_g_b_vectors[2], _M(shuf_odd));// PSHUFB		1 1 3 0 1 2
//	// B1 0 B1 0	B3 0 B3 0	B5 0 B5	0	B7 0 B7	0
}


#endif /* RGB_UNPACK_H_ */
