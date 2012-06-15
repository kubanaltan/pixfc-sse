#include <stdint.h>
#include "inline-testing-common.h"

#ifdef DEBUG
void	print_xmm8u_array(uint32_t count, char *prefix, void* array) {
	uint32_t	index = 0;
	char		title[256] = {0};
	__m128i*	data = (__m128i*) array;

	for(index = 0; index < count; index++){
		sprintf(title, "%s %u", prefix, index);
		print_xmm8u(title, &data[index]);
	}
}

void	print_xmm16u_array(uint32_t count, char *prefix, void* array) {
	uint32_t	index = 0;
	char		title[256] = {0};
	__m128i*	data = (__m128i*) array;

	for(index = 0; index < count; index++){
		sprintf(title, "%s %u", prefix, index);
		print_xmm16u(title, &data[index]);
	}
}
#endif

void   compare_8bit_output(uint8_t check_last, void *scalar_out, void *sse_out, uint8_t sse_out_count, uint32_t max_diff, char *prefix) {
	uint8_t* scalar = (uint8_t*) scalar_out;
	uint8_t* sse = (uint8_t*) sse_out;
	uint8_t  index;
	for(index = 0; index < 16 * sse_out_count; index++) {
		if ((check_last != 0) && (16 - (index % 16) > check_last))
			continue;
		if (abs(scalar[index] - sse[index]) > max_diff) {
			dprintf("== %s\n", prefix);
			dprintf("Value @ %hhu in '%s' vector %u differs by %u: sse: %hhu - scalar: %hhu\n", (index % 16), check_last == 0 ? "OUTPUT" : "PREVIOUS", (index / 16), abs(sse[index]-scalar[index]), sse[index], scalar[index]);
			print_xmm8u("SSE   ", (__m128i*) &sse[(index / 16) * 16]);
			print_xmm8u("Scalar", (__m128i*) &scalar[(index / 16) * 16]);
			break;
		}
	}
}

void  compare_16bit_output(uint8_t check_last, void *scalar_out, void *sse_out, uint8_t sse_out_count, uint32_t max_diff, char *prefix){
	uint16_t* scalar = (uint16_t*) scalar_out;
	uint16_t* sse = (uint16_t*) sse_out;
	uint8_t  index;
	for(index = 0; index < 8 * sse_out_count; index++) {
		if ((check_last != 0) && (8 - (index % 8) > check_last))
			continue;
		if (abs(scalar[index] - sse[index]) > max_diff) {
			dprintf("== %s\n", prefix);
			dprintf("Value @ %hhu in '%s' vector %u differs by %u: sse: %hu - scalar: %hu\n", (index % 8), check_last == 0 ? "OUTPUT" : "PREVIOUS", (index / 8), abs(sse[index]-scalar[index]), sse[index], scalar[index]);
			print_xmm16u("SSE   ", (__m128i*) &sse[(index / 8) * 8]);
			print_xmm16u("Scalar", (__m128i*) &scalar[(index / 8) * 8]);
			break;
		}
	}
}
