/*
 * unit-testing.c
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "conversion_blocks.h"
#include "conversion_routines_from_v210.h"
#include "conversion_routines_from_argb.h"
#include "pixfc-sse.h"
#include "pixfmt_descriptions.h"
#include "platform_util.h"
#include "test-utils.h"

#define		WIDTH				1920
#define 	HEIGHT				1080
#define		NUM_RUNS			100
#define		NUM_INPUT_BUFFERS	5



static int		time_conversion_block(struct PixFcSSE *pixfc, struct timings *timings) {
	void *					input[NUM_INPUT_BUFFERS] = {0};
	void *					output = NULL;
	uint32_t				w = WIDTH;
	uint32_t				h = HEIGHT;
	uint32_t				run_count;
	uint32_t				i;		// index into the input buffer array
	int						result = -1;

	// Allocate and fill source buffers
	for ( i = 0; i < NUM_INPUT_BUFFERS; i++) {
		if (allocate_aligned_buffer(pixfc->source_fmt, w, h, &input[i]) != 0) {
			pixfc_log("Error allocating input buffer\n");
			goto done;
		}

		fill_image(pixfc->source_fmt, (w * h), input[i]);
	}

	// Allocate destination buffers
	if (allocate_aligned_buffer(pixfc->dest_fmt, w, h, &output) != 0){
		pixfc_log("Error allocating out buffer\n");
		goto done;
	}

	i = 0;
	for(run_count = 0; run_count < NUM_RUNS; run_count++) {
		// Perform conversion
		do_timing(NULL);
		pixfc->convert(pixfc, input[i], output);
		do_timing(timings);

		// Force context switch to invalidate caches
		MSSLEEP(15);

		// Move on to next buffer
		i = (i + 1) < NUM_INPUT_BUFFERS ? (i + 1) : 0;
	}

	// All went well
	result = 0;

done:
	// Free resources
	i = NUM_INPUT_BUFFERS;
	while (i-- > 0)
		ALIGN_FREE(input[i]);
	ALIGN_FREE(output);

	return result;
}

static uint32_t		time_conversion_blocks(PixFcPixelFormat source_fmt, PixFcPixelFormat dest_fmt) {
	uint32_t			index = 0;
	struct PixFcSSE *	pixfc;
	struct timings		timings;

	pixfc_log("Input size: %d x %d - %d run(s) per conversion routine.\n", WIDTH, HEIGHT, NUM_RUNS);
	printf("%-80s\t%10s\t%10s\t%10s\t%5s\n","Conversion Block Name", "Avg Time", "Avg User Time",
			"Avg Sys Time", "Ctx Sw");

	// Loop over all conversion blocks
	for(index = 0; index < conversion_blocks_count; index++) {
		if ((source_fmt != PixFcFormatCount) && (conversion_blocks[index].source_fmt != source_fmt))
			continue;

		if ((dest_fmt != PixFcFormatCount) && (conversion_blocks[index].dest_fmt != dest_fmt))
			continue;

		if (create_pixfc_for_conversion_block(index, &pixfc, WIDTH, HEIGHT) != 0) {
			printf("Unable to test conversion block '%s'\n", conversion_blocks[index].name);
			continue;
		}

		// Reset timing info
		memset((void *)&timings, 0, sizeof(timings));

		// Time this conversion block
		if (time_conversion_block(pixfc, &timings) != 0)
			return -1;

		destroy_pixfc(pixfc);

		printf("%-80s\t%10f\t%10f\t%10f\t%5.1f\n",
				conversion_blocks[index].name,
				(double)((double)timings.total_time_ns / (double) (NUM_RUNS * 1000000)),
				(double)((double)timings.user_time_ns / (double) (NUM_RUNS * 1000000)),
				(double)((double)timings.sys_time_ns / (double) (NUM_RUNS * 1000000)),
				(double)((double)(timings.vcs + timings.ivcs) / (double) NUM_RUNS )
		);

		// Add a blank line if the next conversion block uses different
		// source or destinaton format
		if (((index + 1) < conversion_blocks_count)
				&& ((conversion_blocks[(index + 1)].source_fmt !=
						conversion_blocks[index].source_fmt)
						|| (conversion_blocks[(index + 1)].dest_fmt !=
								conversion_blocks[index].dest_fmt)))
			printf("\n");
	}

	return 0;
}

static uint32_t			check_formats_enum() {
	uint32_t		index;
	uint32_t		result = 0;

	// Enumerate the entire PixFcSSE pixel formats
	for(index = 0; index<pixfmt_descriptions_count; index++) {

		// Each pixel format description has a member indicating which
		// PixFcPixelFormat it describes. Also, the pixel format
		// descriptions array must be sorted according to the
		// PixFcPixelFormat enum. Here we enforce this rule.
		if (pixfmt_descriptions[index].pixFcFormat != index) {
			pixfc_log("Element at index %d in pixel format description does not"
					" match the expected PixFcPixelFormat enum entry (it "
					"matches entry %d)\n", index, pixfmt_descriptions[index].pixFcFormat);
			result = -1;
		}
	}


	// Check that both arrays have the same number of elements
	if (index != PixFcFormatCount) {
		pixfc_log("The number of pixel format descriptions (%d does not"
				" match the number of PixFcPixelFormat entries (%d)).\n",
				index, PixFcFormatCount);
		result = -1;
	}

	return result;
}

static int			check_unaligned_conversions() {
	struct PixFcSSE *				pixfc;
	void *							input[2] = { NULL };	// 1 aligned & 1 unaligned input buffer
	void *							output[2] = { NULL };	// 1 aligned & 1 unaligned output buffer
	uint32_t						w = 96,	h = 2, index = 0;

	// Loop over all conversion blocks
	for(index = 0; index < conversion_blocks_count; index++) {
		if (create_pixfc_for_conversion_block(index, &pixfc, w, h) != 0) {
			pixfc_log("Unable to test conversion block '%s'\n", conversion_blocks[index].name);
			continue;
		}

		pixfc_log("%-80s\n", conversion_blocks[index].name);

		// Allocate the input & output buffers
		if (allocate_aligned_buffer(pixfc->source_fmt, w, h, &input[0]) != 0) {
			pixfc_log("Error allocating aligned input buffer\n");
			return -1;
		}
		if (allocate_unaligned_buffer(pixfc->source_fmt, w, h, &input[1]) != 0) {
			pixfc_log("Error allocating unaligned input buffer\n");
			return -1;
		}
		if (allocate_aligned_buffer(pixfc->dest_fmt, w, h, &output[0]) != 0) {
			pixfc_log("Error allocating aligned output buffer\n");
			return -1;
		}
		if (allocate_unaligned_buffer(pixfc->dest_fmt, w, h, &output[1]) != 0) {
			pixfc_log("Error allocating unaligned output buffer\n");
			return -1;
		}

		// Fill input buffers
		fill_image(pixfc->source_fmt, IMG_SIZE(pixfc->source_fmt, w, h), input[0]);
		fill_image(pixfc->source_fmt, IMG_SIZE(pixfc->source_fmt, w, h), input[1]);

		// Do conversion with aligned input & output buffers
		pixfc->convert(pixfc, input[0], output[0]);
		pixfc_log(".\n");

		// Do conversion with aligned input & unaligned output buffers
		pixfc->convert(pixfc, input[0], output[1]);
		pixfc_log(".\n");

		// Do conversion with unaligned input & aligned output buffers
		pixfc->convert(pixfc, input[1], output[0]);
		pixfc_log(".\n");

		// Do conversion with unaligned input & output buffers
		pixfc->convert(pixfc, input[1], output[1]);
		pixfc_log(". OK !\n");

		// Free resources
		destroy_pixfc(pixfc);
		ALIGN_FREE(input[0]);
		free(input[1]);
		ALIGN_FREE(output[0]);
		free(output[1]);
	}

	return 0;
}

uint32_t		check_conversion_enumeration() {
	uint32_t 			index;
	struct PixFcSSE * 	pixfc = NULL;
	uint32_t			w = 96, h = 2;

	for(index = 0; index < conversion_blocks_count; index++) {
		pixfc_log("Checking '%s' ... \n", conversion_blocks[index].name);

		if (create_pixfc_for_conversion_block(index, &pixfc, w, h) != 0) {
			pixfc_log("Unable to test conversion block\n");
			continue;
		}

		if (pixfc->convert != conversion_blocks[index].convert_fn) {
			pixfc_log("Wrong conversion function returned for index %u\n", index);
			return -1;
		}

		if (pixfc->source_fmt!= conversion_blocks[index].source_fmt) {
			pixfc_log("Wrong source format returned for index %u\n", index);
			return -1;
		}

		if (pixfc->dest_fmt!= conversion_blocks[index].dest_fmt) {
			pixfc_log("Wrong destination format returned for index %u\n", index);
			return -1;
		}
		pixfc_log(" OK !\n");
	}

	return 0;
}

uint32_t 	do_flag_check(PixFcPixelFormat source_fmt, PixFcPixelFormat dest_fmt, PixFcFlag flag, ConversionBlockFn expected_conv_fn) {
	struct PixFcSSE *	pixfc;
	uint32_t			result = -1;

	if (create_pixfc(&pixfc, source_fmt, dest_fmt, 96, 2, ROW_SIZE(source_fmt, 96), flag) == 0) {
		if (pixfc->convert != expected_conv_fn)
			pixfc_log("Wrong conversion function\n");
		else
			result = 0;

		destroy_pixfc(pixfc);
	} else
		pixfc_log("Error creating pixfc\n");

	return result;
}

// Checks that passing PixFcFlags_* results in the right conversion block
// being chosen.
uint32_t	check_pixfc_flags() {
	// Default flag
	pixfc_log("Checking default flag\n");
	if (do_flag_check(PixFcV210, PixFcARGB, PixFcFlag_Default, upsample_n_convert_v210_to_argb_sse2_ssse3_sse41) != 0) {
		pixfc_log("Default flag check failed\n");
		return -1;
	}
	pixfc_log("OK !\n");


	// NoSSE flag
	pixfc_log("Checking NoSSE flag\n");
	if (do_flag_check(PixFcV210, PixFcARGB, PixFcFlag_NoSSE, convert_v210_to_any_rgb_nonsse) != 0) {
		pixfc_log("NoSSE flag check failed\n");
		return -1;
	}
	pixfc_log("OK !\n");


	pixfc_log("Checking NoSSE | NNB flag\n");
	// NoSSE | NNBflag
	if (do_flag_check(PixFcV210, PixFcARGB, PixFcFlag_NoSSE  | PixFcFlag_NNbResamplingOnly, convert_v210_to_any_rgb_nonsse) != 0) {
		pixfc_log("NoSSE | NNB flags check failed\n");
		return -1;
	}
	pixfc_log("OK !\n");


	pixfc_log("Checking SSE2Only flag\n");
	// SSE2Only flag
	if (do_flag_check(PixFcARGB, PixFcYUYV, PixFcFlag_SSE2Only, downsample_n_convert_argb_to_yuyv_sse2) != 0) {
		pixfc_log("SSE2Only flag check failed\n");
		return -1;
	}
	pixfc_log("OK !\n");
	
	pixfc_log("Checking SSE2Only flag\n");
	// SSE2_SSSE3Only flag
	if (do_flag_check(PixFcV210, PixFcARGB, PixFcFlag_SSE2_SSSE3Only, upsample_n_convert_v210_to_argb_sse2_ssse3) != 0) {
		pixfc_log("SSE2_SSSE3Only flag check failed\n");
		return -1;
	}
	pixfc_log("OK !\n");
	


	pixfc_log("Checking SSE2Only | NNB flag\n");
	// SSE2Only | NNB flag
	if (do_flag_check(PixFcARGB, PixFcYUYV, PixFcFlag_SSE2Only | PixFcFlag_NNbResamplingOnly, convert_argb_to_yuyv_sse2) != 0) {
		pixfc_log("SSE2Only | NNB flags check failed\n");
		return -1;
	}
	pixfc_log("OK !\n");


	pixfc_log("Checking Bt601 flag\n");
	// BT601 flag
	if (do_flag_check(PixFcV210, PixFcARGB, PixFcFlag_BT601Conversion, upsample_n_convert_v210_to_argb_bt601_sse2_ssse3_sse41) != 0) {
		pixfc_log("Bt601 flag check failed\n");
		return -1;
	}
	pixfc_log("OK !\n");


	pixfc_log("Checking Bt601 | NNB flag\n");
	// BT601 | NNB flag
	if (do_flag_check(PixFcV210, PixFcARGB, PixFcFlag_BT601Conversion | PixFcFlag_NNbResamplingOnly, convert_v210_to_argb_bt601_sse2_ssse3_sse41) != 0) {
		pixfc_log("Bt601 | NNB flags check failed\n");
		return -1;
	}
	pixfc_log("OK !\n");


	pixfc_log("Checking Bt709 flag\n");
	// BT709 flag
	if (do_flag_check(PixFcV210, PixFcARGB, PixFcFlag_BT709Conversion, upsample_n_convert_v210_to_argb_bt709_sse2_ssse3_sse41) != 0) {
		pixfc_log("Bt709 flag check failed\n");
		return -1;
	}
	pixfc_log("OK !\n");


	pixfc_log("Checking Bt709 | NNB flag\n");
	// BT709 | NNB flag
	if (do_flag_check(PixFcV210, PixFcARGB, PixFcFlag_BT709Conversion | PixFcFlag_NNbResamplingOnly, convert_v210_to_argb_bt709_sse2_ssse3_sse41) != 0) {
		pixfc_log("Bt709 | NNB flags check failed\n");
		return -1;
	}
	pixfc_log("OK !\n");


	pixfc_log("Checking NNBOnly flag\n");
	// NNB flag
	if (do_flag_check(PixFcV210, PixFcARGB, PixFcFlag_NNbResamplingOnly, convert_v210_to_argb_sse2_ssse3_sse41) != 0) {
		pixfc_log("NNB flag check failed\n");
		return -1;
	}
	pixfc_log("OK !\n");

	return 0;
}

static uint32_t do_conversion(uint32_t index, uint32_t w, uint32_t h) {
	struct PixFcSSE *	pixfc;
	void *				input = NULL;	// 1 aligned & 1 unaligned input buffer
	void *				output = NULL;	// 1 aligned & 1 unaligned output buffer

	if (create_pixfc_for_conversion_block(index, &pixfc, w, h) != 0) {
		pixfc_log("Unable to test conversion block '%s'\n", conversion_blocks[index].name);
		return -1;
	}

	pixfc_log("%-80s%dx%d\n", conversion_blocks[index].name, w, h);

	// Allocate the input & output buffers
	if (allocate_aligned_buffer(pixfc->source_fmt, w, h, &input) != 0) {
		pixfc_log("Error allocating input buffer\n");
		return -1;
	}
	if (allocate_aligned_buffer(pixfc->dest_fmt, w, h, &output) != 0) {
		pixfc_log("Error allocating output buffer\n");
		ALIGN_FREE(input);
		return -1;
	}

	// Fill input buffers
	fill_image(pixfc->source_fmt, IMG_SIZE(pixfc->source_fmt, w, h), input);

	// Do conversion with aligned input & output buffers
	pixfc->convert(pixfc, input, output);

	// Free resources
	destroy_pixfc(pixfc);
	ALIGN_FREE(input);
	ALIGN_FREE(output);

	return 0;
}

static uint32_t check_v210_conversion_buffer_sizes() {
	static const uint32_t	w_16_1[] = {16, 16*2, 16*3, 16*4, 16*5, 16*6, 16, 16*2, 16*3, 16*4, 16*5, 16*6, 720, 720, 960, 960+16, 960+32, 960+48, 1280, 1920};
	static const uint32_t	h_16_1[] = {1,  1,    1,    1,    1,    1,    2,  2,    2,    2,    2,    2,    480, 576, 720,  720,    720,    720,   1024, 1080};
	static const uint32_t 	size_count_16_1 = sizeof(w_16_1)/sizeof(w_16_1[0]);
	//
	static const uint32_t	w_32_1[] = {32, 32*2, 32*3, 32*4, 32*5, 32*6, 32, 32*2, 32*3, 32*4, 32*5, 32*6, 640, 960, 1280, 1920};
	static const uint32_t	h_32_1[] = {1,  1,    1,    1,    1,    1,    2,  2,    2,    2,    2,    2,    480, 720, 1024, 1080};
	static const uint32_t 	size_count_32_1 = sizeof(w_32_1)/sizeof(w_32_1[0]);
	//
	static const uint32_t	w_32_2[] = {32, 32*2, 32*3, 32*4, 32*5, 32*6, 736, 736, 960, 1280, 1920};
	static const uint32_t	h_32_2[] = {2,  2,    2,    2,    2,    2,    480, 576, 720, 1024, 1080};
	static const uint32_t 	size_count_32_2 = sizeof(w_32_2)/sizeof(w_32_2[0]);
	uint32_t				size_index = 0;

	//
	const uint32_t*		width_array = NULL;
	const uint32_t*		height_array = NULL;
	uint32_t			array_size = 0;
	uint32_t			conv_index = 0;

	// Loop over all conversion blocks
	for(conv_index = 0; conv_index < conversion_blocks_count; conv_index++) {
		if ((conversion_blocks[conv_index].width_multiple == 16) && (conversion_blocks[conv_index].height_multiple == 1)) {
			width_array = w_16_1;
			height_array = h_16_1;
			array_size = size_count_16_1;
		} else if ((conversion_blocks[conv_index].width_multiple == 32) && (conversion_blocks[conv_index].height_multiple == 1)) {
			width_array = w_32_1;
			height_array = h_32_1;
			array_size = size_count_32_1;
		} else if ((conversion_blocks[conv_index].width_multiple == 32) && (conversion_blocks[conv_index].height_multiple == 2)) {
			width_array = w_32_2;
			height_array = h_32_2;
			array_size = size_count_32_2;
		} else if ((conversion_blocks[conv_index].width_multiple == 2) && (conversion_blocks[conv_index].height_multiple == 2)) {
			// Non-sse YUV420p conversions
			width_array = w_32_2;
			height_array = h_32_2;
			array_size = size_count_32_2;
		} else if ((conversion_blocks[conv_index].width_multiple == 2) && (conversion_blocks[conv_index].height_multiple == 1)) {
			// Some non-sse conversions
			width_array = w_32_1;
			height_array = h_32_1;
			array_size = size_count_32_1;
		} else if ((conversion_blocks[conv_index].width_multiple == 1) && (conversion_blocks[conv_index].height_multiple == 2)) {
			// Some non-sse conversions
			width_array = w_32_2;
			height_array = h_32_2;
			array_size = size_count_32_2;
		} else if ((conversion_blocks[conv_index].width_multiple == 1) && (conversion_blocks[conv_index].height_multiple == 1)) {
			// All other non-sse conversions
			width_array = w_32_1;
			height_array = h_32_1;
			array_size = size_count_32_1;
		} else {
			pixfc_log("Unhandled width / height multiple: %u - %u for conversion %-80s\n",
					conversion_blocks[conv_index].width_multiple,
					conversion_blocks[conv_index].height_multiple,
					conversion_blocks[conv_index].name);
			return -1;
		}

		// try each image size
		for(size_index = 0; size_index < array_size; size_index ++) {
			if (do_conversion(conv_index, width_array[size_index], height_array[size_index]) != 0) {
				pixfc_log("Error testing conversion '%-80s'\n", conversion_blocks[conv_index].name);
				return -1;
			}
		}
	}

	return 0;
}

/*
 * Here we run a few tests to make sure things are sound internally
 * and also to print some averages of conversion latency.
 */
int 				main(int argc, char **argv) {
	PixFcPixelFormat	source_fmt = PixFcFormatCount;
	PixFcPixelFormat	dest_fmt = PixFcFormatCount;

	if (argc >= 2)
		source_fmt = find_matching_pixel_format(argv[1]);

	if (argc == 3)
		dest_fmt = find_matching_pixel_format(argv[2]);

	pixfc_log("\n");
	pixfc_log("\t\tU N I T   T E S T I N G\n");

	pixfc_log("\n");
	pixfc_log("Running internal checks ...\n");
	if (check_conversion_enumeration() != 0) {
		pixfc_log("FAILED\n");
		return -1;
	}
	pixfc_log("PASSED\n");


	pixfc_log("\n");
	pixfc_log("\n");
	pixfc_log("Running PixFcFlags checks ...\n");
	if (check_pixfc_flags()!= 0)
		pixfc_log("FAILED\n");
		// dont exit as this one may fail if the CPU doesnt have SSSE3
	else
		pixfc_log("PASSED\n");


	pixfc_log("\n");
	pixfc_log("\n");
	pixfc_log("Checking PixFcPixelFormat enum and description arrays...\n");
	if (check_formats_enum() != 0) {
		pixfc_log("FAILED\n");
		return -1;
	}
	pixfc_log("PASSED\n");

	pixfc_log("\n");
	pixfc_log("\n");
	pixfc_log("Testing conversion from aligned / unaligned buffers...\n");
	if (check_unaligned_conversions() != 0) {
		pixfc_log("FAILED\n");
		return -1;
	}
	pixfc_log("PASSED\n");

	pixfc_log("\n");
	pixfc_log("\n");
	pixfc_log("Testing v210 conversions with multiple buffer sizes\n");
	if (check_v210_conversion_buffer_sizes() != 0) {
		pixfc_log("FAILED\n");
		return -1;
	}
	pixfc_log("PASSED\n");

	pixfc_log("\n");
	pixfc_log("\n");
	pixfc_log("Checking conversion block timing ... \n");
	if (time_conversion_blocks(source_fmt, dest_fmt) != 0) {
		pixfc_log("FAILED\n");
		return -1;
	}
	pixfc_log("PASSED\n");

	pixfc_log("\n");
	return 0;
}



