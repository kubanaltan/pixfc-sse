#include "platform_util.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


// 0		15		16		235		236		240		241     255
#define DECLARE_1_8BIT_VECT(var)	M128I((var), 0x00EB001000F0000LL, 0x00FF00F100F0E00ECLL)

// 0		1		2		15		16		17		127		128
// 129		234		235		236		240		241		254		255
#define DECLARE_2_8BIT_VECT(var) __m128i (var)[] = { { 0x000F000200010000LL, 0x0080007F00110010LL }, \
                                                     { 0x00EC00EB00EA0081LL, 0x00FF00FE00F100F0LL } }

// 0		15		16		235		236		240		241     255
// 0		15		16		235		236		240		241     255
// 0		15		16		235		236		240		241     255
#define DECLARE_3_8BIT_VECT(var) __m128i (var)[] = {{0x00EB001000F0000LL, 0x00FF00F100F0E00ECLL}, \
                                                    {0x00EB001000F0000LL, 0x00FF00F100F0E00ECLL}, \
                                                    {0x00EB001000F0000LL, 0x00FF00F100F0E00ECLL}, }

// 0		1		2		15		16		17		127		128
// 0		1		2		15		16		17		127		128
// 129		234		235		236		240		241		254		255
// 129		234		235		236		240		241		254		255
#define DECLARE_4_8BIT_VECT(var) __m128i (var)[] = { { 0x000F000200010000LL, 0x0080007F00110010LL }, \
                                                     { 0x000F000200010000LL, 0x0080007F00110010LL }, \
                                                     { 0x00EC00EB00EA0081LL, 0x00FF00FE00F100F0LL }, \
                                                     { 0x00EC00EB00EA0081LL, 0x00FF00FE00F100F0LL },}




#define MAX_DIFF_8BIT       2

#define dprintf(fmt, ...)  do { fprintf (stderr, "[ %s:%-3d ] " fmt,\
                                strrchr(__FILE__, '\\')+1,\
                                __LINE__, ## __VA_ARGS__);\
                                fflush(stderr); } while(0)


#define   compare_8bit_output(scalar_out, sse_out, max_diff, prefix) do {\
        uint8_t* scalar = (uint8_t*) scalar_out;\
        uint8_t* sse = (uint8_t*) sse_out;\
        uint8_t  index;\
        for(index = 0; index < 16; index++) {\
            if (abs(scalar[index] - sse[index]) > max_diff) {\
                dprintf("== %s\nByte %hhu differ by %u: sse: %hhu - scalar: %hhu\n", prefix, index, sse[index], scalar[index], abs(sse[index]-scalar[index]));\
                return -1;\
            }\
        }\
    } while(0)

