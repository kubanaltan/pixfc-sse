/*
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

#include <emmintrin.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "pixfc-sse.h"
#include "pixfmt_descriptions.h"
#include "platform_util.h"
#include "test-utils.h"


// PIXFC FLAGS
//#define PIXFC_FLAGS	PixFcFlag_BT601Conversion// | PIXFC_AVG_UPSAMPLING_FLAG
#define PIXFC_FLAGS	PixFcFlag_SSE2Only
//#define PIXFC_FLAGS	PixFcFlag_NoSSE
//#define PIXFC_FLAGS	PixFcFlag_Default



/*
 * Global variables
 */
PixFcPixelFormat		src_fmt = PixFcYUYV;
PixFcPixelFormat		dst_fmt = PixFcARGB;
char *					src_filename = NULL;
char *					dst_filename = "test.ppm";
uint32_t				w = 1280;
uint32_t				h = 1024;

static void 		print_usage(char *prog_name) {
	uint32_t index = 0;

	printf("Convert an image in a source pixel format to a destination pixel format.\n");
	printf("Usage: %s <input format> [input file] <output format> <width> <height>\n", prog_name);
	printf("Where <input format> and <output format> are one of:\n");
	while(index < pixfmt_descriptions_count) {
		printf("  %s\n", pixfmt_descriptions[index].name);
		index++;
	}
	printf("If [input file] is specified and exists, read the source image from that file.\n");
	printf("Otherwise, fill the source image with a predefined fill pattern.\n");
}

// Parse arguments and set global variables accordingly
// Return -1 is cmd line is invalid.
static uint32_t		parse_args(int argc, char **argv){
	uint32_t arg_index = 1;

	// Need 4 or 5 args
	if ((argc != 5) && (argc != 6)) {
		printf("Invalid argument count\n");
		return -1;
	}

	// Parse input format
	if ((src_fmt = find_matching_pixel_format(argv[arg_index++])) == PixFcFormatCount) {
		printf("Invalid source format '%s'\n", argv[(arg_index - 1)]);
		return -1;
	}

	// If we have 6 args, the next arg is the input file
	if (argc == 6)
		src_filename = argv[arg_index++]; // the filename is checked when the file is opened.

	// Parse output format
	if ((dst_fmt = find_matching_pixel_format(argv[arg_index++])) == PixFcFormatCount) {
		printf("Invalid destination format '%s'\n", argv[arg_index - 1]);
		return -1;
	}

	// Parse width and height
	if (((w = atoi(argv[arg_index++])) == 0) || ((h = atoi(argv[arg_index++])) == 0)) {
		printf("Invalid width / height (%d x %d)\n", w, h);
		return -1;
	}

	return 0;
}

int 		main(int argc, char **argv) {
	struct PixFcSSE * 	pixfc;
	struct timings			timings;
	// In / out buffers
	__m128i	*				in;
	__m128i	*				out;

	// Parse arguments and set global variables accordingly
	// (exit if cmd line is invalid)
	if (parse_args(argc, argv) != 0) {
		print_usage(argv[0]);
		return 1;
	}

	// Reset timing info
	memset(&timings, 0, sizeof(timings));

	// If we were given an input file name, get its contents
	if (src_filename != NULL) {
		if (get_buffer_from_file(src_fmt, w, h, src_filename, (void **)&in) < 0) {
			dprint("Error getting buffer from file\n");
			return 1;
		}
	} else {
		// Otherwise, allocate a buffer of the given width and height
		// and fill in buffer with predefined pattern
		if (allocate_buffer(src_fmt, w, h, (void **)&in) != 0) {
			dprint("Error allocating in buffer");
			return 1;
		}
		fill_image(src_fmt, IMG_SIZE(src_fmt, w, h), in);
	}

	// allocate out buffer
	if (allocate_buffer(dst_fmt, w, h, (void **)&out) != 0) {
		dprint("Error allocating out buffer");
		free(in);
		return 1;
	}

	dprint("Image size:\t%d x %d\n", w, h);
	dprint("Input format:\t%s\n", pixfmt_descriptions[src_fmt].name);
	dprint("Output format:\t%s\n", pixfmt_descriptions[dst_fmt].name);

	// create struct pixfc
	if (create_pixfc(&pixfc, src_fmt, dst_fmt, w, h, PIXFC_FLAGS) != 0)
	{
		dprint("error creating struct pixfc\n");
		free(in);
		free(out);
		return 1;
	}

	dprint("%10s\t%10s\t%10s\t%5s\n", "Avg Time", "Avg User Time", "Avg Sys Time", "Ctx Sw");

	// Run conversion function
	do_timing(NULL);
	pixfc->convert(pixfc, in, out);
	do_timing(&timings);

	dprint("%10f\t%10f\t%10f\t%5.1f\n",
			(double)((double)timings.total_time_ns / 1000000.0),
			(double)((double)timings.user_time_ns / 1000000.0),
			(double)((double)timings.sys_time_ns / 1000000.0),
			(double)((double)(timings.vcs + timings.ivcs))
	);

	dprint("Writing PPM file '%s'...\n", dst_filename);
	write_anyrgb_buffer_to_ppm_file(dst_fmt, w, h, dst_filename, out);
	dprint("Done\n");

	// Free data structures
	destroy_pixfc(pixfc);

	ALIGN_FREE(in);
	ALIGN_FREE(out);

	return 0;
}


