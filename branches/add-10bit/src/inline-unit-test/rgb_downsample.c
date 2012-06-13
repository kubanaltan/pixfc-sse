#include "inline-testing-common.h"
#include "rgb_downsample.h"

/*
 * Create 3 422 downsampled R, G, B vectors from 3 R, G, B vectors
 * using nearest neighbour interpolation
 *
 * INPUT:
 * 4 vectors of 8 short
 * agVect1
 * A1 0     G1 0    A2 0    G2 0    A3 0    G3 0    A4 0    G4 0
 *
 * rbVect1
 * R1 0     B1 0    R2 0    B2 0    R3 0    B3 0    R4 0    B4 0
 *
 * agVect2
 * A5 0     G5 0    A6 0    G6 0    A7 0    G7 0    A8 0    G8 0
 *
 * rbVect3
 * R5 0     B5 0    R6 0    B6 0    R7 0    B7 0    R8 0    B8 0
 *
 * OUTPUT:
 * 2 vectors of 8 short:
 * agVect
 * A1 0     G1 0    A3 0    G3 0    A5 0    G5 0    A7 0    G7 0
 *
 * rbVect
 * R1 0     B1 0    R3 0    B3 0    R5 0    B5 0    R7 0    B7 0
 */
static void nnb_422_downsample_ag_rb_vectors_scalar(__m128i *input, __m128i* output) {
	uint16_t*	in = (uint16_t*) input;
	uint16_t*	out = (uint16_t*) output;
	uint32_t		index = 0;

	for(index = 0; index < 16; index += 2) {
		*out++ = *in;
		*out++ = *in++;
	}
}

static uint32_t	check_nnb_422_downsample_ag_rb_vectors() {
	nnb_422_downsample_ag_rb_vectors_sse2()
}
