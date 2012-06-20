/*
 * inline-testing-common.h
 *
 * Copyright (C) 2012 PixFC Team (pixelfc@gmail.com)
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


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "platform_util.h"
#include "common.h"

#define MAX_DIFF_8BIT      2


// 0		15		16		235		236		240		241     255
#define DECLARE_1_8BIT_VECT(var)	M128I((var), 0x00EB0010000F0000LL, 0x00FF00F100F000ECLL)

// 0		1		2		15		16		17		127		128
// 129		234		235		236		240		241		254		255
#define DECLARE_2_8BIT_VECT(var)	__m128i (var)[] = { { 0x000F000200010000LL, 0x0080007F00110010LL }, \
	                                                    { 0x00EC00EB00EA0081LL, 0x00FF00FE00F100F0LL } }

// 0		15		16		235		236		240		241     255
// 0		15		16		235		236		240		241     255
// 0		15		16		235		236		240		241     255
#define DECLARE_3_8BIT_VECT(var)	__m128i (var)[] = { { 0x00EB0010000F0000LL, 0x00FF00F100F000ECLL}, \
														{ 0x00EB0010000F0000LL, 0x00FF00F100F000ECLL}, \
														{ 0x00EB0010000F0000LL, 0x00FF00F100F000ECLL}, }

// 0		1		2		15		16		17		127		128
// 0		1		2		15		16		17		127		128
// 129		234		235		236		240		241		254		255
// 129		234		235		236		240		241		254		255
#define DECLARE_4_8BIT_VECT(var)	__m128i (var)[] = { { 0x000F000200010000LL, 0x0080007F00110010LL }, \
		                                                { 0x000F000200010000LL, 0x0080007F00110010LL }, \
	                                                    { 0x00EC00EB00EA0081LL, 0x00FF00FE00F100F0LL }, \
														{ 0x00EC00EB00EA0081LL, 0x00FF00FE00F100F0LL },}

// 0		15		16		235		236		240		241     255
// 0		15		16		235		236		240		241     255
// 0		15		16		235		236		240		241     255
// 0		15		16		235		236		240		241     255
// 0		15		16		235		236		240		241     255
// 0		15		16		235		236		240		241     255
#define DECLARE_6_8BIT_VECT(var)	__m128i (var)[] = { { 0x00EB0010000F0000LL, 0x00FF00F100F000ECLL}, \
										    			{ 0x00EB0010000F0000LL, 0x00FF00F100F000ECLL}, \
										    			{ 0x00EB0010000F0000LL, 0x00FF00F100F000ECLL}, \
                                                        { 0x00EB0010000F0000LL, 0x00FF00F100F000ECLL}, \
												    	{ 0x00EB0010000F0000LL, 0x00FF00F100F000ECLL}, \
													    { 0x00EB0010000F0000LL, 0x00FF00F100F000ECLL}, }



// 0		63		64		940		941		960		961     1023
#define DECLARE_1_10BIT_VECT(var)	M128I((var), 0x03AC0040003F0000LL, 0x03FF03C103C003ADLL)

// 0		1		2		63		64		65		511		512
// 513		939		940		941		960		961		1022	1023
#define DECLARE_2_10BIT_VECT(var)	__m128i (var)[] = { { 0x003F000200010000LL, 0x020001FF00410040LL }, \
														{ 0x03AD03AC03AB0201LL, 0x03FF03FE03C103C0LL }, }
 
// 0		63		64		940		941		960		961     1023
// 0		63		64		940		941		960		961     1023
// 0		63		64		940		941		960		961     1023
#define DECLARE_3_10BIT_VECT(var)	__m128i (var)[] = { { 0x03AC0040003F0000LL, 0x03FF03C103C003ADLL },\
														{ 0x03AC0040003F0000LL, 0x03FF03C103C003ADLL },\
														{ 0x03AC0040003F0000LL, 0x03FF03C103C003ADLL }, }

// 0		1		2		63		64		65		511		512
// 0		1		2		63		64		65		511		512
// 513		939		940		941		960		961		1022	1023
// 513		939		940		941		960		961		1022	1023
#define DECLARE_4_10BIT_VECT(var)	__m128i (var)[] = { { 0x003F000200010000LL, 0x020001FF00410040LL }, \
														{ 0x003F000200010000LL, 0x020001FF00410040LL }, \
														{ 0x03AD03AC03AB0201LL, 0x03FF03FE03C103C0LL }, \
														{ 0x03AD03AC03AB0201LL, 0x03FF03FE03C103C0LL }, }
 
// 0		63		64		940		941		960		961     1023
// 0		63		64		940		941		960		961     1023
// 0		63		64		940		941		960		961     1023
// 0		63		64		940		941		960		961     1023
// 0		63		64		940		941		960		961     1023
// 0		63		64		940		941		960		961     1023
#define DECLARE_6_10BIT_VECT(var)	__m128i (var)[] = { { 0x03AC0040003F0000LL, 0x03FF03C103C003ADLL },\
														{ 0x03AC0040003F0000LL, 0x03FF03C103C003ADLL },\
														{ 0x03AC0040003F0000LL, 0x03FF03C103C003ADLL },\
                                                        { 0x03AC0040003F0000LL, 0x03FF03C103C003ADLL },\
														{ 0x03AC0040003F0000LL, 0x03FF03C103C003ADLL },\
														{ 0x03AC0040003F0000LL, 0x03FF03C103C003ADLL }, }


extern float        rgb_8bit_to_yuv_8bit_fr_coef[][3];
extern uint32_t     rgb_8bit_to_yuv_8bit_fr_off[];
extern float        rgb_8bit_to_yuv_8bit_bt601_coef[][3];
extern uint32_t     rgb_8bit_to_yuv_8bit_bt601_off[];
extern float        rgb_8bit_to_yuv_8bit_bt709_coef[][3];
extern uint32_t     rgb_8bit_to_yuv_8bit_bt709_off[];

extern float        rgb_8bit_to_yuv_10bit_fr_coef[][3];
extern uint32_t     rgb_8bit_to_yuv_10bit_fr_off[];
extern float        rgb_8bit_to_yuv_10bit_bt601_coef[][3];
extern uint32_t     rgb_8bit_to_yuv_10bit_bt601_off[];
extern float        rgb_8bit_to_yuv_10bit_bt709_coef[][3];
extern uint32_t     rgb_8bit_to_yuv_10bit_bt709_off[];



#define dprintf(fmt, ...)  do { fprintf (stderr, "[ %s:%-3d ] " fmt,\
                                strrchr(__FILE__, '/')+1,\
                                __LINE__, ## __VA_ARGS__);\
                                fflush(stderr); } while(0)
#ifdef DEBUG
	void	print_xmm8u_array(uint32_t count, char *prefix, void *array);
	void	print_xmm16u_array(uint32_t count, char *prefix, void *array);
#else
	#define print_xmm8u_array(...)
	#define print_xmm16u_array(...)
#endif

void	compare_8bit_output(uint8_t check_last, void *scalar_out, void *sse_out, uint8_t output_count, uint32_t max_diff, char *prefix);
void	compare_16bit_output(uint8_t check_last, void *scalar_out, void *sse_out, uint8_t output_count, uint32_t max_diff, char *prefix);

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))

#define CHECK_INLINE_1_IN(inline_scalar, inline_sse, input, scalar_out, sse_out, output_count, max_diff, compare_fn)\
	do {\
		dprintf("Checking " #inline_sse "\n");\
        /*print_xmm16u_array(ARRAY_SIZE(input), "INPUT", input); */\
		inline_scalar(input, scalar_out);\
        /*print_xmm16u_array(ARRAY_SIZE(scalar_out), "SCALAR OUT", scalar_out); */\
		inline_sse(input, sse_out);\
        /*print_xmm16u_array(ARRAY_SIZE(sse_out), "SSE OUT", sse_out); */\
		compare_fn(0, scalar_out, sse_out, output_count, max_diff, #inline_sse);\
	} while (0)



#define DO_CHECK_INLINE_1IN(inline_scalar, inline1_sse, inline2_sse, input, scalar_out, sse_out, output_count, max_diff, compare_fn)\
	do {\
		int i = 0;\
		dprintf("Checking " #inline1_sse "\n");\
		inline_scalar(input, scalar_out);\
		inline1_sse(input, sse_out);\
		compare_fn(0, scalar_out, sse_out, output_count, max_diff, #inline1_sse);\
		for(i = 0; i < output_count; i++)\
			sse_out[i] = _mm_setzero_si128();\
		dprintf("Checking " #inline2_sse "\n");\
		inline2_sse(input, sse_out);\
		compare_fn(0, scalar_out, sse_out, output_count, max_diff, #inline2_sse);\
	} while (0)

#define	 CHECK_SSE2_SSSE3_INLINE_1_IN_16BIT(inline_prefix, input, scalar_output, sse_output, sse_output_count, max_diff)	\
	DO_CHECK_INLINE_1IN(inline_prefix ## scalar, inline_prefix ## sse2, inline_prefix ##sse2_ssse3, input, scalar_output, sse_output, sse_output_count, max_diff, compare_16bit_output)

#define	 CHECK_SSE2_SSSE3_INLINE_1_IN_8BIT(inline_prefix, input, scalar_output, sse_output, sse_output_count, max_diff)	\
	DO_CHECK_INLINE_1IN(inline_prefix ## scalar, inline_prefix ## sse2, inline_prefix ##sse2_ssse3, input, scalar_output, sse_output, sse_output_count, max_diff, compare_8bit_output)



#define DO_CHECK_INLINE_1IN_1PREV(inline_scalar, inline1_sse, inline2_sse, input, input_count, scalar_prev_input, sse_prev_input, prev_input_count, check_last, scalar_out, sse_out, output_count, max_diff, compare_fn)\
	do {\
		int i = 0;\
		__m128i sse_prev_input_copy[prev_input_count];\
		__m128i scalar_prev_input_copy[prev_input_count];\
		for(i = 0; i < prev_input_count; i++){\
			sse_prev_input_copy[i] = sse_prev_input[i];\
			scalar_prev_input_copy[i] = scalar_prev_input[i];\
		}\
		dprintf("Checking " #inline1_sse "\n");\
		/* print_xmm16u_array(input_count, "INPUT SCALAR ", input); \
		print_xmm16u_array(prev_input_count, "SCALAR_PREV_INPUT", scalar_prev_input); */\
		inline_scalar(input, scalar_prev_input, scalar_out);\
		/* print_xmm16u_array(prev_input_count, "SCALAR_PREV_INPUT", scalar_prev_input);\
		print_xmm16u_array(output_count, "SCALAR_OUTPUT", scalar_out);\
		print_xmm16u_array(input_count, "INPUT SSE ", input);\
		print_xmm16u_array(prev_input_count, "SSE_PREV_INPUT", sse_prev_input); */\
		inline1_sse(input, sse_prev_input, sse_out);\
		/*print_xmm16u_array(prev_input_count, "SSE_PREV_INPUT", sse_prev_input);\
		print_xmm16u_array(output_count, "SSE_OUT", sse_out); */\
		compare_fn(0, scalar_out, sse_out, output_count, max_diff, #inline1_sse);\
		compare_fn(check_last, scalar_prev_input, sse_prev_input, prev_input_count, 0, #inline1_sse);\
		for(i = 0; i < output_count; i++){\
			sse_out[i] = _mm_setzero_si128();\
			scalar_out[i] = _mm_setzero_si128();\
		}\
		dprintf("Checking " #inline2_sse "\n");\
		/* print_xmm16u_array(input_count, "INPUT SCALAR ", input);\
		print_xmm16u_array(prev_input_count, "SCALAR_PREV_INPUT", scalar_prev_input_copy); */\
		inline_scalar(input, scalar_prev_input_copy, scalar_out);\
		/* print_xmm16u_array(prev_input_count, "SCALAR_PREV_INPUT", scalar_prev_input_copy);\
		print_xmm16u_array(output_count, "SCALAR_OUT", scalar_out);\
		print_xmm16u_array(input_count, "INPUT SSE ", input);\
		print_xmm16u_array(prev_input_count, "SSE_PREV_INPUT", sse_prev_input_copy); */\
		inline2_sse(input, sse_prev_input_copy, sse_out);\
		/* print_xmm16u_array(prev_input_count, "SSE_PREV_INPUT", sse_prev_input_copy);\
		print_xmm16u_array(output_count, "SSE_OUT", sse_out); */\
		compare_fn(0, scalar_out, sse_out, output_count, max_diff, #inline2_sse);\
		compare_fn(check_last, scalar_prev_input_copy, sse_prev_input_copy, prev_input_count, 0, #inline2_sse);\
	} while (0)

#define	 CHECK_SSE2_SSSE3_INLINE_1_IN_1_PREV_16BIT(inline_prefix, input, input_count, scalar_prev_input, sse_prev_input, prev_input_count, check_last, scalar_output, sse_output, sse_output_count, max_diff)	\
	DO_CHECK_INLINE_1IN_1PREV(inline_prefix ## scalar, inline_prefix ## sse2, inline_prefix ##sse2_ssse3, input, input_count, scalar_prev_input, sse_prev_input, prev_input_count, check_last, scalar_output, sse_output, sse_output_count, max_diff, compare_16bit_output)


#define DO_CHECK_INLINE_2IN(inline_scalar, inline1_sse, inline2_sse, input1, input2, input_count,scalar_out, sse_out, output_count, max_diff, compare_fn)\
	do {\
		int i = 0;\
        dprintf("Checking " #inline1_sse "\n");\
		/* print_xmm16u_array(input_count, "INPUT1 SCALAR ", input1);\
		print_xmm16u_array(input_count, "INPUT2 SCALAR", input2); */\
		inline_scalar(input1, input2, scalar_out);\
		/* print_xmm16u_array(output_count, "SCALAR_OUTPUT", scalar_out);\
        print_xmm16u_array(input_count, "INPUT1 SCALAR ", input1);\
		print_xmm16u_array(input_count, "INPUT2 SCALAR", input2); */\
		inline1_sse(input1, input2, sse_out);\
		/* print_xmm16u_array(output_count, "SSE_OUT", sse_out); */\
		compare_fn(0, scalar_out, sse_out, output_count, max_diff, #inline1_sse);\
		for(i = 0; i < output_count; i++){\
			sse_out[i] = _mm_setzero_si128();\
			scalar_out[i] = _mm_setzero_si128();\
		}\
		dprintf("Checking " #inline2_sse "\n");\
        /* print_xmm16u_array(input_count, "INPUT1 SCALAR ", input1);\
		print_xmm16u_array(input_count, "INPUT2 SCALAR", input2); */\
		inline_scalar(input1, input2, scalar_out);\
		/* print_xmm16u_array(output_count, "SCALAR_OUTPUT", scalar_out);\
        print_xmm16u_array(input_count, "INPUT1 SCALAR ", input1);\
		print_xmm16u_array(input_count, "INPUT2 SCALAR", input2); */\
		inline2_sse(input1, input2, sse_out);\
		/* print_xmm16u_array(output_count, "SSE_OUT", sse_out); */\
		compare_fn(0, scalar_out, sse_out, output_count, max_diff, #inline2_sse);\
	} while (0)

#define	 CHECK_SSE2_SSSE3_INLINE_2_IN_16BIT(inline_prefix, input1, input2, input_count, scalar_output, sse_output, sse_output_count, max_diff)	\
	DO_CHECK_INLINE_2IN(inline_prefix ## scalar, inline_prefix ## sse2, inline_prefix ##sse2_ssse3, input1, input2, input_count, scalar_output, sse_output, sse_output_count, max_diff, compare_16bit_output)

