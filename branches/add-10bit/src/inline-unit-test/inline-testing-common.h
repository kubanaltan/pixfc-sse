#include "platform_util.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


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
 

#define dprintf(fmt, ...)  do { fprintf (stderr, "[ %s:%-3d ] " fmt,\
                                strrchr(__FILE__, '/')+1,\
                                __LINE__, ## __VA_ARGS__);\
                                fflush(stderr); } while(0)

#define print_xmm8u(prefix, vec) dprintf(prefix "\t % 3hhu % 3hhu % 3hhu % 3hhu % 3hhu % 3hhu % 3hhu % 3hhu % 3hhu % 3hhu % 3hhu % 3hhu % 3hhu % 3hhu % 3hhu % 3hhu\n",\
                        vec[0], vec[1], vec[2], vec[3], vec[4], vec[5], vec[6], vec[7], vec[8], vec[9], vec[10], vec[11], vec[12], vec[13], vec[14], vec[15])

#define print_xmm16u(prefix, vec) dprintf(prefix "\t%5hu\t%5hu\t%5hu\t%5hu\t%5hu\t%5hu\t%5hu\t%5hu\n",\
						vec[0], vec[1], vec[2], vec[3], vec[4], vec[5], vec[6], vec[7]);\

#define   compare_8bit_output(check_last, scalar_out, sse_out, sse_out_count, max_diff, prefix) do {\
        uint8_t* scalar = (uint8_t*) scalar_out;\
        uint8_t* sse = (uint8_t*) sse_out;\
        uint8_t  index;\
        for(index = 0; index < 16 * sse_out_count; index++) {\
			if ((check_last != 0) && (16 - (index % 16) > check_last))\
				continue;\
            if (abs(scalar[index] - sse[index]) > max_diff) {\
				uint8_t* vec = &sse[(index / 16) * 16];\
                dprintf("== %s\n", prefix);\
				dprintf("Value @ %hhu in '%s' vector %u differs by %u: sse: %hhu - scalar: %hhu\n", (index % 16), check_last == 0 ? "OUTPUT" : "PREVIOUS", (index / 16), abs(sse[index]-scalar[index]), sse[index], scalar[index]);\
				dprintf("#     \t  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15\n");\
				print_xmm8u("SSE   ", vec);\
				vec = &scalar[(index / 16) * 16];\
				print_xmm8u("Scalar", vec);\
                break;\
            }\
        }\
    } while(0)

#define   compare_16bit_output(check_last, scalar_out, sse_out, sse_out_count, max_diff, prefix) do {\
        uint16_t* scalar = (uint16_t*) scalar_out;\
        uint16_t* sse = (uint16_t*) sse_out;\
        uint8_t  index;\
        for(index = 0; index < 8 * sse_out_count; index++) {\
			if ((check_last != 0) && (8 - (index % 8) > check_last))\
				continue;\
            if (abs(scalar[index] - sse[index]) > max_diff) {\
				uint16_t* vec = &sse[(index / 8) * 8];\
                dprintf("== %s\n", prefix);\
				dprintf("Value @ %hhu in '%s' vector %u differs by %u: sse: %hu - scalar: %hu\n", (index % 8), check_last == 0 ? "OUTPUT" : "PREVIOUS", (index / 8), abs(sse[index]-scalar[index]), sse[index], scalar[index]);\
				dprintf("#     \t    0\t    1\t    2\t    3\t    4\t    5\t    6\t    7\n");\
				print_xmm16u("SSE   ", vec);\
				vec = &scalar[(index / 8) * 8];\
				print_xmm16u("Scalar", vec);\
				break;\
            }\
        }\
    } while(0)

#define DO_CHECK_INLINE_1IN(inline_scalar, inline1_sse, inline2_sse, input, scalar_out, sse_out, sse_out_count, max_diff, compare_fn)\
	do {\
		int i = 0;\
		dprintf("Checking " #inline1_sse "\n");\
		inline_scalar(input, scalar_out);\
		inline1_sse(input, sse_out);\
		compare_fn(0, scalar_out, sse_out, sse_out_count, max_diff, #inline1_sse);\
		for(i = 0; i < sse_out_count; i++)\
			sse_out[i] = _mm_setzero_si128();\
		dprintf("Checking " #inline2_sse "\n");\
		inline2_sse(input, sse_out);\
		compare_fn(0, scalar_out, sse_out, sse_out_count, max_diff, #inline2_sse);\
	} while (0)

#define	 CHECK_SSE2_SSSE3_INLINE_1_IN_16BIT(inline_prefix, input, scalar_output, sse_output, sse_output_count, max_diff)	\
	DO_CHECK_INLINE_1IN(inline_prefix ## scalar, inline_prefix ## sse2, inline_prefix ##sse2_ssse3, input, scalar_output, sse_output, sse_output_count, max_diff, compare_16bit_output)

#define DO_CHECK_INLINE_2IN(inline_scalar, inline1_sse, inline2_sse, input, scalar_prev_input, sse_prev_input, prev_input_count, check_last, scalar_out, sse_out, sse_out_count, max_diff, compare_fn)\
	do {\
		int i = 0;\
		__m128i sse_prev_input_copy[prev_input_count];\
		__m128i scalar_prev_input_copy[prev_input_count];\
		for(i = 0; i < prev_input_count; i++){\
			sse_prev_input_copy[i] = sse_prev_input[i];\
			scalar_prev_input_copy[i] = scalar_prev_input[i];\
		}\
		dprintf("Checking " #inline1_sse "\n");\
		inline_scalar(input, scalar_prev_input, scalar_out);\
		inline1_sse(input, sse_prev_input, sse_out);\
		compare_fn(0, scalar_out, sse_out, sse_out_count, max_diff, #inline1_sse);\
		compare_fn(check_last, scalar_prev_input, sse_prev_input, prev_input_count, 0, #inline1_sse);\
		for(i = 0; i < sse_out_count; i++){\
			sse_out[i] = _mm_setzero_si128();\
			scalar_out[i] = _mm_setzero_si128();\
		}\
		dprintf("Checking " #inline2_sse "\n");\
		inline_scalar(input, scalar_prev_input_copy, scalar_out);\
		inline2_sse(input, sse_prev_input_copy, sse_out);\
		compare_fn(0, scalar_out, sse_out, sse_out_count, max_diff, #inline2_sse);\
		compare_fn(check_last, scalar_prev_input_copy, sse_prev_input_copy, prev_input_count, 0, #inline2_sse);\
	} while (0)

#define	 CHECK_SSE2_SSSE3_INLINE_2_IN_16BIT(inline_prefix, input, scalar_prev_input, sse_prev_input, prev_input_count, check_last, scalar_output, sse_output, sse_output_count, max_diff)	\
	DO_CHECK_INLINE_2IN(inline_prefix ## scalar, inline_prefix ## sse2, inline_prefix ##sse2_ssse3, input, scalar_prev_input, sse_prev_input, prev_input_count, check_last, scalar_output, sse_output, sse_output_count, max_diff, compare_16bit_output)

