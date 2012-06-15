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

	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[4];
	out[3] = in[5];
	out[4] = in[16];
	out[5] = in[17];
	out[6] = in[20];
	out[7] = in[21];

	out[8] = in[8];
	out[9] = in[9];
	out[10] = in[12];
	out[11] = in[13];
	out[12] = in[24];
	out[13] = in[25];
	out[14] = in[28];
	out[15] = in[29];
}

uint32_t	check_nnb_422_downsample_ag_rb_vectors() {
    DECLARE_4_8BIT_VECT(input_8bit);
	DECLARE_4_10BIT_VECT(input_10bit);
    __m128i sse_output[2];
    __m128i scalar_output[2];

	CHECK_SSE2_SSSE3_INLINE_1_IN_16BIT(nnb_422_downsample_ag_rb_vectors_, input_8bit, scalar_output, sse_output, 2, 0);
	CHECK_SSE2_SSSE3_INLINE_1_IN_16BIT(nnb_422_downsample_ag_rb_vectors_, input_10bit, scalar_output, sse_output, 2, 0);

	return 0;
}


/*
 * Create 3 422 downsampled R, G, B vectors from 3 R, G, B vectors
 * using nearest neighbour interpolation
 *
 * TOTAL LATENCY:   6
 *
 * INPUT:
 * 3 vectors of 8 short
 * rVect
 * R1  0    R2  0   R3  0   R4  0   R5  0   R6  0   R7  0   R8  0
 *
 * gVect
 * G1 0     G2 0    G3 0    G4 0    G5 0    G6 0    G7 0    G8 0
 *
 * bVect
 * B1 0     B2 0    B3 0    B4 0    B5 0    B6 0    B7 0    B8 0
 *
 * OUTPUT:
 * 3 vectors of 8 short
 * rVect
 * R1 0 R1 0    R3 0 R3 0   R5 0 R5 0   R7 0 R7 0
 *
 * gVect
 * G1 0 G1 0    G3 0 G3 0   G5 0 G5 0   G7 0 G7 0
 *
 * bVect
 * B1 0 B1 0    B3 0 B3 0   B5 0 B5 0   B7 0 B7 0
 */
void  nnb_422_downsample_r_g_b_vectors_scalar(__m128i* input, __m128i *output) {
    uint16_t*   in = (uint16_t*) input;
    uint16_t*   out = (uint16_t*) output;
	uint8_t		index = 0;

	for(index = 0; index < 8; index += 2) {
		out[index] = in[index];
		out[index + 1] = in[index];

		out[index + 8] = in[index + 8];
		out[index + 9] = in[index + 8];

		out[index + 16] = in[index + 16];
		out[index + 17] = in[index + 16];
	}
}

uint32_t	check_nnb_422_downsample_r_g_b_vectors() {
    DECLARE_3_8BIT_VECT(input_8bit);
	DECLARE_3_10BIT_VECT(input_10bit);
    __m128i sse_output[3];
    __m128i scalar_output[3];

	CHECK_SSE2_SSSE3_INLINE_1_IN_16BIT(nnb_422_downsample_r_g_b_vectors_, input_8bit, scalar_output, sse_output, 3, 0);
	CHECK_SSE2_SSSE3_INLINE_1_IN_16BIT(nnb_422_downsample_r_g_b_vectors_, input_10bit, scalar_output, sse_output, 3, 0);

	return 0;
}

/*
 * Create 3 422 downsampled R, G, B vectors from 6 AG, RB vectors
 * using a simple 3-tap average filter. Also, copy the contents of the
 * current vectors in the previous ones
 *
 * TOTAL LATENCY:   20
 *
 * INPUT:
 * 6 vectors of 8 short
 * previous agVect2
 * A5 0     G5 0    A6 0    G6 0    A7 0    G7 0    A8 0    G8 0
 *
 * previous rbVect3
 * R5 0     B5 0    R6 0    B6 0    R7 0    B7 0    R8 0    B8 0
 *
 * current agVect1
 * A1 0     G1 0    A2 0    G2 0    A3 0    G3 0    A4 0    G4 0
 *
 * current rbVect1
 * R1 0     B1 0    R2 0    B2 0    R3 0    B3 0    R4 0    B4 0
 *
 * current agVect2
 * A5 0     G5 0    A6 0    G6 0    A7 0    G7 0    A8 0    G8 0
 *
 * current rbVect3
 * R5 0     B5 0    R6 0    B6 0    R7 0    B7 0    R8 0    B8 0
 *
 * OUTPUT:
 * 2 vectors of 8 short
 * agVect
 * A12 0    G12 0   A34 0   G34 0   A56 0   G56 0   A78 0   G78 0
 *
 * rbVect
 * R12 0    B12 0   R34 0   B34 0   R56 0   B56 0   R78 0   B78 0
 */
void  avg_422_downsample_ag_rb_vectors_n_save_previous_scalar(__m128i* input, __m128i* input_output, __m128i *output) {
    uint16_t*   in = (uint16_t*) input;
	uint16_t	prev_a = ((uint16_t*) input_output)[6];
    uint16_t	prev_g = ((uint16_t*) input_output)[7];
	uint16_t	prev_r = ((uint16_t*) input_output)[8 + 6];
	uint16_t	prev_b = ((uint16_t*) input_output)[8 + 7];
    uint16_t*   out = (uint16_t*) output;
    uint8_t     out_pixel = 0;

	for(out_pixel = 0; out_pixel < 4; out_pixel++) {
		// A
		// TODO: we should really be setting this to 0
		out[0] = ( ((prev_a + in[2]) / 2) + in[0]) / 2; 
		prev_a = in[2];
		
		// G
		out[1] = ( ((prev_g + in[3]) / 2) + in[1]) / 2;
		prev_g = in[3];

		// R
		out[8] = ( ((prev_r + in[8 + 2]) / 2) + in[8]) / 2;
		prev_r = in[8 + 2];

		// B
		out[8 + 1] = ( ((prev_b + in[8 + 3]) / 2) + in[8 + 1]) / 2;
		prev_b = in[8 + 3];

		out += 2;
		in += 4;
		
		if (out_pixel % 2 == 1)
			in += 8;
	}

	input_output[0] = input[2];
	input_output[1] = input[3];
}

uint32_t check_avg_422_downsample_ag_rb_vectors_n_save_previous() {
	DECLARE_2_8BIT_VECT(scalar_prev_input_8bit);
	DECLARE_2_8BIT_VECT(sse_prev_input_8bit);
	DECLARE_4_8BIT_VECT(input_8bit);
	DECLARE_2_10BIT_VECT(scalar_prev_input_10bit);
	DECLARE_2_10BIT_VECT(sse_prev_input_10bit);
    DECLARE_4_10BIT_VECT(input_10bit);
    __m128i sse_output[2];
    __m128i scalar_output[2];

    //CHECK_SSE2_SSSE3_INLINE_2_IN_16BIT(avg_422_downsample_ag_rb_vectors_n_save_previous_, input_8bit, 4, scalar_prev_input_8bit, sse_prev_input_8bit, 2, 2, scalar_output, sse_output, 2, MAX_DIFF_8BIT);
    CHECK_SSE2_SSSE3_INLINE_2_IN_16BIT(avg_422_downsample_ag_rb_vectors_n_save_previous_, input_10bit, 4, scalar_prev_input_10bit, sse_prev_input_10bit, 2, 2, scalar_output, sse_output, 2, MAX_DIFF_8BIT);

    return 0;
}
