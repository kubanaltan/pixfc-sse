/*
 * test-utils.h
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

#ifndef _TEST_UTILS_H_
#define _TEST_UTILS_H_

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifndef WIN32
#include <strings.h>
#endif

#include "pixfc-sse.h"

// printf helper which prints file name and line number
#ifndef WIN32
#define pixfc_log(fmt, ...) 	do { fprintf (stderr, "[ %s:%-3d ] " fmt,\
								rindex(__FILE__, '/')+1,\
								__LINE__, ## __VA_ARGS__);\
								fflush(stderr); } while(0)
#else
#define pixfc_log(fmt, ...) 	do { fprintf (stderr, "[ %s:%-3d ] " fmt,\
								strrchr(__FILE__, '\\')+1,\
								__LINE__, ## __VA_ARGS__);\
								fflush(stderr); } while(0)
#endif




typedef struct {
	PixFcPixelFormat	format;
	uint32_t			width;
	uint32_t			height;
	char *				filename;
} InputFile;


// Relative path to the test-image dir
#define		PATH_TO_TEST_IMG	"../test-images/"
// Array of InputFiles each describing a single test file in the test-images dir
extern const InputFile		input_files[];
// Number of elements in the previous array
extern const uint32_t		input_files_size;

/*
 * There might be several InputFile for the given format.
 * Return the InputFile matching the given format at the given index (starting at 0), or NULL
 */
const InputFile* 	find_input_file_for_format(PixFcPixelFormat format, uint32_t index);

/*
 * Allocate a buffer large enough for the given format, width and height, and
 * fill it with whatever if directed by the InputFile struct
 * Returns 0 if OK, -1 otherwise
 */
int32_t 		get_buffer_from_input_file(const InputFile*, void **buffer);


/*
 * Print (using printf) the known pixel formats in PixFxPixelFormat
 */
void 			print_known_pixel_formats();

/*
 * Print (using printf) the known flags in PixFcFlag
 */
void 			print_known_flags();

/*
 * Convert the provided string to integer and return the PixFcFlag equivalent
 */
PixFcFlag		get_matching_flags(char *flag_string);

/*
 * Print (using printf) the value of the given flags
 */
void			print_flags(PixFcFlag flags);

/*
 * Look for PixFcPixelFormat enum entry matching the given pixel format
 * passed as a string. (Uses pixfmt_descriptions array to look for match)
 * Return the matching PixFcPixelFormat enum entry or 'PixFcFormatCount'
 * if not found.
 */
PixFcPixelFormat	find_matching_pixel_format(char *format_string);


/*
 * Validate the image dimensions for a given image format:
 * - Make sure the number of pixels is multiple of 16 (many SSE conversion blocks have this req.)
 * - Make sure buffer size is an even number.
 * Return 0 if OK, -1 otherwise
 */
uint32_t	validate_image_dimensions(PixFcPixelFormat fmt, uint32_t width, uint32_t height);

/*
 * Validate the image dimensions (with function above) and allocate a 16-byte aligned memory buffer
 * large enough to contain an image in the give pixel format. The buffer can be released with free().
 * Returns 0 if OK, -1 if memory allocation fails, -2 if the width / height are invalid for SSE conversion
 * in the given pixel format
 */
uint32_t	allocate_aligned_buffer(PixFcPixelFormat fmt, uint32_t width, uint32_t height, void **buffer);

/*
 * Same as above for unaligned memory
 */
uint32_t	allocate_unaligned_buffer(PixFcPixelFormat fmt, uint32_t width, uint32_t height, void **buffer);

/*
 * Fill image buffer with the fill found in the struct PixelFormatDescription (see pifmt_descriptions.h)
 */
void		fill_image(PixFcPixelFormat fmt, uint32_t buffer_size, void *buffer);

/*
 * Allocate a buffer large enough for the given format, width and height, and 
 * fill it with the contents of the given file
 * Returns 0 if OK, -1 otherwise
 */
int32_t 	get_buffer_from_file(PixFcPixelFormat fmt, uint32_t width, uint32_t height, char *filename, void **buffer);

/*
 * Write pixels from in to a file.
 * If the buffer is in an RGB format, it will be saved in a PPM file. Otherwise the raw bytes are saved (using the next function)
 */
void 		write_buffer_to_file(PixFcPixelFormat fmt, uint32_t width, uint32_t height, char *filename, void * buffer);

/*
 * Write buffer as is to a file
 */
void 		write_raw_buffer_to_file(PixFcPixelFormat fmt, uint32_t width, uint32_t height, char *filename, void * in);
/*
 *  This function returns the current system's uptime
 *  in nanoseconds (64 bit unsigned)
 */
typedef uint64_t	ticks;
ticks				getticks();

/*
 * Call the first time with timings = NULL to get the current process timing profile.
 * Next, call with a valid stuct timings pointer to store the difference between
 * current process timing profile and previous one. The difference is added to the
 * members in the given struct timings.
 */
struct timings {
	uint64_t	total_time_ns;
	uint64_t	user_time_ns;
	uint64_t	sys_time_ns;
	uint64_t	vcs;
	uint64_t	ivcs;
};
void				do_timing(struct timings *timings);

/*
 * Synthesize PixFcFlags for the given conversion routine
 */
PixFcFlag			synthesize_pixfc_flags(uint32_t index);

/*
 * Create a struct PicFcSSE struct  for the conversion block at the provided index
 * if the CPU has the required features. Caller must release struct when done
 */
uint32_t			create_pixfc_for_conversion_block(uint32_t index, struct PixFcSSE** pixfc, uint32_t width, uint32_t height);

/*
 * Given source/dest formats, flags, width, height and row bytes, this function
 * returns the index of the conversion block which would be returned by create_pixfc,
 * or -1 if error
 */
int32_t				find_conversion_block_index(PixFcPixelFormat src_fmt, PixFcPixelFormat dst_fmt, PixFcFlag flags, uint32_t width, uint32_t height, uint32_t row_bytes);

/*
 * Make conversion block name CSV friendly by replacing " - " with ", "
 * CSV friendly name is returned in csv_name and must be freed by caller.
 * Return 0 if O, -1 if error
 */
int32_t				make_conv_block_name_csv_friendly(uint32_t index, char **csv_name);


#if defined(__linux__) || defined(__APPLE__)

#include <unistd.h>

#define MSSLEEP(ms)								usleep((ms)*1000)

// File IO
#define OPEN									open
#define RD_ONLY_FLAG							O_RDONLY
#define WR_CREATE_FLAG							O_WRONLY | O_CREAT | O_TRUNC
#define RW_PERM									S_IRWXU
#define READ									read
#define WRITE									write
#define	SNPRINTF								snprintf
#define STRDUP									strdup
#define SSCANF									sscanf
#define CLOSE									close

// Aligned allocation
#define ALIGN_MALLOC(var, size, alignment)		do { int ret = posix_memalign((void **) &(var), (alignment), (size)); if (ret != 0) var = NULL; }while(0)
#define ALIGN_FREE(var)							do { if (var) { free(var); (var) = NULL; } } while(0)

#else

#include <malloc.h>
#include <io.h>
#include <windows.h>

#define MSSLEEP(ms)								Sleep(ms)

// File IO
#define OPEN									_open
#define RD_ONLY_FLAG							_O_RDONLY | _O_BINARY
#define	WR_CREATE_FLAG							_O_WRONLY | _O_CREAT | _O_TRUNC
#define RW_PERM									_S_IWRITE
#define READ									_read
#define WRITE									_write
#define SNPRINTF								_snprintf
#define STRDUP									_strdup
#define SSCANF									_sscanf
#define CLOSE									_close

// Aligned allocation
#define ALIGN_MALLOC(var, size, alignment)		(var) = _aligned_malloc((size), (alignment))
#define ALIGN_FREE(var)							_aligned_free(var)

#endif


#endif /* _TEST_UTILS_H_ */
