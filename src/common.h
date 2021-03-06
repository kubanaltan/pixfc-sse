/*
 * common.h
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

#ifndef COMMON_H_
#define COMMON_H_

#include "platform_util.h"
#include "debug_support.h"

#include <stdio.h>
#include <stdint.h>
#ifdef __INTEL_CPU__
#include <emmintrin.h>
#endif

// Clamp a pixel component's value to 0-255
#define CLIP_PIXEL(x) 		(uint8_t)( ((x) > 255) ? 255 : ((x) < 0) ? 0 : (x) )
#define CLIP_10BIT_PIXEL(x) ( ((x) > 1023) ? 1023 : ((x) < 0) ? 0 : (x) )


extern const float        rgb_8bit_to_yuv_8bit_coef[][3][3];
extern const int32_t      rgb_8bit_to_yuv_8bit_off[][3];

extern const float        rgb_8bit_to_yuv_10bit_coef[][3][3];
extern const int32_t      rgb_8bit_to_yuv_10bit_off[][3];

extern const float        rgb_10bit_to_yuv_8bit_coef[][3][3];
extern const int32_t      rgb_10bit_to_yuv_8bit_off[][3];

extern const float        rgb_10bit_to_yuv_10bit_coef[][3][3];
extern const int32_t      rgb_10bit_to_yuv_10bit_off[][3];

extern const float        yuv_8bit_to_rgb_8bit_coef[][3][3];
extern const int32_t      yuv_8bit_to_rgb_8bit_off[][3];

extern const float        yuv_8bit_to_rgb_10bit_coef[][3][3];
extern const int32_t      yuv_8bit_to_rgb_10bit_off[][3];

extern const float        yuv_10bit_to_rgb_8bit_coef[][3][3];
extern const int32_t      yuv_10bit_to_rgb_8bit_off[][3];

extern const float        yuv_10bit_to_rgb_10bit_coef[][3][3];
extern const int32_t      yuv_10bit_to_rgb_10bit_off[][3];



// Conversion matrices left-shifted by 8
extern const int32_t      rgb_8bit_to_yuv_8bit_coef_lhs8[][3][3];
extern const int32_t      rgb_8bit_to_yuv_10bit_coef_lhs8[][3][3];
extern const int32_t      rgb_10bit_to_yuv_8bit_coef_lhs8[][3][3];
extern const int32_t      rgb_10bit_to_yuv_10bit_coef_lhs8[][3][3];

extern const int32_t      yuv_8bit_to_rgb_8bit_coef_lhs8[][3][3];
extern const int32_t      yuv_10bit_to_rgb_8bit_coef_lhs8[][3][3];
extern const int32_t      yuv_8bit_to_rgb_10bit_coef_lhs8[][3][3];
extern const int32_t      yuv_10bit_to_rgb_10bit_coef_lhs8[][3][3];

#ifdef __INTEL_CPU__

//
// The following DO_CONVERSION_* macros help us detect whether the input and
// output buffers are aligned / unaligned and run the conversion macro (1st arg)
// with the right aligned / unaligned pack & unpack routines.
//
// The variations of the DO_CONVERSION_* macros are there to accept different
// numbers of pack / unpack routines, as indicated by the _Px_Uy suffix
#ifdef WIN32

// Visual Studio's handling of variadic macros is buggy at best.
// We need the following in order to have __VA_ARGS__ handled properly.
#define LeftParenthesis (
#define RightParenthesis )



/*
 * Outer loop for conversions to / from v210.
 * This loop makes the following assumptions:
 * - a width multiple of 8,
 * - the v210 core conversion loop handles 24 pixels at a time,
 * - v210 has a buffer large enough to accommodate multiple of 48 pixels
 *   (even though the width (in pixel) does not have to be multiple of 48
 *    thanks to padding bytes at the end of each line).
 *
 * There are only 3 cases to cover depending on the width:
 *
 * - width % 24 == 0,
 * - width % 24 == 8,
 * - width % 24 == 16
 *
 * The macro manages the v210 and output buffer pointers so they always will point to the right place.
 *
 */
#define FROM_V120_24_PIX_OUTER_CONVERSION_LOOP(v210_ptr, out_ptr, core, core_last24, leftover_8, leftover_16, ...)\
		uint32_t	width = pixfc->width;\
		uint32_t	line = pixfc->height;\
		uint8_t		*src = (uint8_t *)source_buffer;\
		uint8_t		*dst = (uint8_t *)dest_buffer;\
		uint32_t	src_row_byte_count = ROW_SIZE(pixfc->source_fmt, width);\
		uint32_t	dst_row_byte_count = ROW_SIZE(pixfc->dest_fmt, width);\
		uint32_t	pixel;\
		if ((width % 24) == 0) {\
			while(line-- > 0) {\
				v210_ptr = (__m128i *)src;\
				out_ptr = (__m128i *)dst;\
				pixel = width - 24; /* Handle the last 24 pixels outside the loop*/\
				while(pixel > 0) {\
					core LeftParenthesis __VA_ARGS__ RightParenthesis;\
					pixel -= 24;\
				}\
				core_last24 LeftParenthesis __VA_ARGS__ RightParenthesis;\
				src += src_row_byte_count;\
				dst += dst_row_byte_count;\
			}\
		} else if ((width % 24) == 8) {\
			while(line-- > 0) {\
				v210_ptr = (__m128i *)src;\
				out_ptr = (__m128i *)dst;\
				pixel = width; /* Handle the last 24 pixels outside the loop*/\
				while(pixel > 24) {\
					core LeftParenthesis __VA_ARGS__ RightParenthesis;\
					pixel -= 24;\
				}\
				leftover_8 LeftParenthesis __VA_ARGS__ RightParenthesis;\
				src += src_row_byte_count;\
				dst += dst_row_byte_count;\
			}\
		} else { /* width % 24 == 16 */\
			while(line-- > 0) {\
				v210_ptr = (__m128i *)src;\
				out_ptr = (__m128i *)dst;\
				pixel = width; /* Handle the last 24 pixels outside the loop*/\
				while(pixel > 24) {\
					core LeftParenthesis __VA_ARGS__ RightParenthesis;\
					pixel -= 24;\
				}\
				leftover_16 LeftParenthesis __VA_ARGS__ RightParenthesis;\
				src += src_row_byte_count;\
				dst += dst_row_byte_count;\
			}\
		}

#define TO_V120_24_PIX_OUTER_CONVERSION_LOOP(in_ptr, v210_ptr, core_first24, core, first_leftover_8, leftover_8, first_leftover_16, leftover_16, ...)\
		uint32_t	width = pixfc->width;\
		uint32_t	line = pixfc->height;\
		uint8_t		*src = (uint8_t *)source_buffer;\
		uint8_t		*dst = (uint8_t *)dest_buffer;\
		uint32_t	src_row_byte_count = ROW_SIZE(pixfc->source_fmt, width);\
		uint32_t	dst_row_byte_count = ROW_SIZE(pixfc->dest_fmt, width);\
		uint32_t	pixel;\
		if ((width % 24) == 0) {\
			while(line-- > 0) {\
				in_ptr = (__m128i *)src;\
				v210_ptr = (__m128i *)dst;\
				src += src_row_byte_count;\
				dst += dst_row_byte_count;\
				pixel = width - 24; /* Handle the first 24 pixels outside the loop*/\
				core_first24 LeftParenthesis __VA_ARGS__ RightParenthesis;\
				while(pixel >= 24) {\
					core LeftParenthesis __VA_ARGS__ RightParenthesis;\
					pixel -= 24;\
				}\
			}\
		} else if ((width % 24) == 8) {\
			if (width == 8) {/* there are only 8 pixels on each line*/\
				while(line-- > 0) {\
					in_ptr = (__m128i *)src;\
					v210_ptr = (__m128i *)dst;\
					src += src_row_byte_count;\
					dst += dst_row_byte_count;\
					first_leftover_8 LeftParenthesis __VA_ARGS__ RightParenthesis;\
				}\
			} else {\
				while(line-- > 0) {\
					in_ptr = (__m128i *)src;\
					v210_ptr = (__m128i *)dst;\
					src += src_row_byte_count;\
					dst += dst_row_byte_count;\
					pixel = width - 24; /* Handle the first 24 pixels outside the loop*/\
					core_first24 LeftParenthesis __VA_ARGS__ RightParenthesis;\
					while(pixel > 24) {\
						core LeftParenthesis __VA_ARGS__ RightParenthesis;\
						pixel -= 24;\
					}\
					leftover_8 LeftParenthesis __VA_ARGS__ RightParenthesis;\
				}\
			}\
		} else { /* width % 24 == 16 */\
			if (width == 16) {/* there are only 16 pixels on each line*/\
				while(line-- > 0) {\
					in_ptr = (__m128i *)src;\
					v210_ptr = (__m128i *)dst;\
					src += src_row_byte_count;\
					dst += dst_row_byte_count;\
					first_leftover_16 LeftParenthesis __VA_ARGS__ RightParenthesis;\
					v210_ptr += 4;\
				}\
			} else {\
				while(line-- > 0) {\
					in_ptr = (__m128i *)src;\
					v210_ptr = (__m128i *)dst;\
					src += src_row_byte_count;\
					dst += dst_row_byte_count;\
					pixel = width - 24; /* Handle the first 24 pixels outside the loop*/\
					core_first24 LeftParenthesis __VA_ARGS__ RightParenthesis;\
					while(pixel > 24) {\
						core LeftParenthesis __VA_ARGS__ RightParenthesis;\
						pixel -= 24;\
					}\
					leftover_16 LeftParenthesis __VA_ARGS__ RightParenthesis;\
				}\
			}\
		}

/*
 * Outer loop for conversions to / from v210.
 * This loop makes the following assumptions:
 * - a width multiple of 16,
 * - the v210 core conversion loop handles 48 pixels at a time,
 * - v210 has a buffer large enough to accommodate multiple of 48 pixels
 *   (even though the width (in pixel) does not have to be multiple of 48
 *    thanks to padding bytes at the end of each line).
 *
 * Because of these assumptions, there are 6 cases to cover depending on width % 48:
 * 0, 8, 16, 24, 32, 40
 */
#define FROM_V210_48_PIX_INNER_LOOP(v210_ptr, out_ptr, core_preamble, core, leftovers, ...) \
			while(line-- > 0) {\
				v210_ptr = (__m128i *)src;\
				out_ptr = (__m128i *)dst;\
				core_preamble LeftParenthesis __VA_ARGS__ RightParenthesis;\
				while(pixel > 48) /* handle as many chunks of 48 pixels in the loop */{\
					core LeftParenthesis __VA_ARGS__ RightParenthesis;\
					pixel -= 48;\
				}\
				leftovers LeftParenthesis __VA_ARGS__ RightParenthesis;\
				src += src_row_byte_count;\
				dst += dst_row_byte_count;\
				pixel = width;\
			}

#define FROM_V120_48_PIX_OUTER_CONVERSION_LOOP(v210_ptr, out_ptr, core_preamble, core, last_48, leftover_8, leftover_16, leftover_24, leftover_32, leftover_40, ...)\
		uint32_t	width = pixfc->width;\
		uint32_t	line = pixfc->height;\
		uint32_t	pixel = width;\
		uint8_t		*src = (uint8_t *)source_buffer;\
		uint8_t		*dst = (uint8_t *)dest_buffer;\
		uint32_t	src_row_byte_count = ROW_SIZE(pixfc->source_fmt, width);\
		uint32_t	dst_row_byte_count = ROW_SIZE(pixfc->dest_fmt, width);\
		if ((width % 48) == 0) {\
			FROM_V210_48_PIX_INNER_LOOP LeftParenthesis v210_ptr, out_ptr, core_preamble, core, last_48, __VA_ARGS__ RightParenthesis\
		} else if ((width % 48) == 8) {\
			FROM_V210_48_PIX_INNER_LOOP LeftParenthesis v210_ptr, out_ptr, core_preamble, core, leftover_8, __VA_ARGS__ RightParenthesis\
		} else if ((width % 48) == 16) {\
			FROM_V210_48_PIX_INNER_LOOP LeftParenthesis v210_ptr, out_ptr, core_preamble, core, leftover_16, __VA_ARGS__ RightParenthesis\
		} else if ((width % 48) == 24) {\
			FROM_V210_48_PIX_INNER_LOOP LeftParenthesis v210_ptr, out_ptr, core_preamble, core, leftover_24, __VA_ARGS__ RightParenthesis\
		} else if ((width % 48) == 32) {\
			FROM_V210_48_PIX_INNER_LOOP LeftParenthesis v210_ptr, out_ptr, core_preamble, core, leftover_32, __VA_ARGS__ RightParenthesis\
 		} else if ((width % 48) == 40) {\
			FROM_V210_48_PIX_INNER_LOOP LeftParenthesis v210_ptr, out_ptr, core_preamble, core, leftover_40, __VA_ARGS__ RightParenthesis\
		}

#define TO_V120_48_PIX_OUTER_CONVERSION_LOOP(core_first48, core, first_leftover_16, leftover_16, first_leftover_32, leftover_32, ...)\
		uint32_t	width = pixfc->width;\
		uint32_t	line = pixfc->height;\
		uint32_t	pixel = width - 48; /* handle the fist 48 pixels outside the loop*/\
		if ((width % 48) == 0) {\
			while(line-- > 0) {\
				core_first48 LeftParenthesis __VA_ARGS__ RightParenthesis;\
				while(pixel > 0) {\
					core LeftParenthesis __VA_ARGS__ RightParenthesis;\
					pixel -= 48;\
				}\
				/* v210_ptr already points to the start of next line */;\
				pixel = width - 48;\
			}\
		} else if ((width % 48) == 16) {\
			if (width == 16){\
				while(line-- > 0) {\
					first_leftover_16 LeftParenthesis __VA_ARGS__ RightParenthesis;\
					/* v210_ptr already points to the start of next line */;\
					pixel = width - 48;\
				}\
			} else {\
				while(line-- > 0) {\
					core_first48 LeftParenthesis __VA_ARGS__ RightParenthesis;\
					while(pixel > 48) {\
						core LeftParenthesis __VA_ARGS__ RightParenthesis;\
						pixel -= 48;\
					}\
					leftover_16 LeftParenthesis __VA_ARGS__ RightParenthesis;\
					/* v210_ptr already points to the start of next line */;\
					pixel = width - 48;\
				}\
			}\
		} else { /* width % 48 == 32 */\
			if (width == 32) {\
				while(line-- > 0) {\
					first_leftover_32 LeftParenthesis __VA_ARGS__ RightParenthesis;\
					/* v210_ptr already points to the start of next line */;\
				}\
			} else {\
				while(line-- > 0) {\
					core_first48 LeftParenthesis __VA_ARGS__ RightParenthesis;\
					while(pixel > 48) {\
						core LeftParenthesis __VA_ARGS__ RightParenthesis;\
						pixel -= 48;\
					}\
					leftover_32 LeftParenthesis __VA_ARGS__ RightParenthesis;\
					/* v210_ptr already points to the start of next line */;\
					pixel = width - 48;\
				}\
			}\
		}

#define DO_CONVERSION_1U_1P(conversion_macro, unpack_fn, pack_fn, ...)\
	if (((uintptr_t)source_buffer & 0x0F) == 0) {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			conversion_macro LeftParenthesis unpack_fn, pack_fn, __VA_ARGS__ RightParenthesis\
		} else {\
			conversion_macro LeftParenthesis unpack_fn, unaligned_##pack_fn, __VA_ARGS__ RightParenthesis\
		}\
	} else {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			conversion_macro LeftParenthesis unaligned_##unpack_fn, pack_fn, __VA_ARGS__ RightParenthesis\
		} else {\
			conversion_macro LeftParenthesis unaligned_##unpack_fn, unaligned_##pack_fn, __VA_ARGS__ RightParenthesis\
		}\
	}


#define DO_CONVERSION_1U_2P(conversion_macro, unpack_fn, pack1_fn, pack2_fn, ...)\
	if (((uintptr_t)source_buffer & 0x0F) == 0) {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			conversion_macro LeftParenthesis unpack_fn, pack1_fn, pack2_fn, __VA_ARGS__ RightParenthesis\
		} else {\
			conversion_macro LeftParenthesis unpack_fn, unaligned_##pack1_fn, unaligned_##pack2_fn, __VA_ARGS__ RightParenthesis\
		}\
	} else {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			conversion_macro LeftParenthesis unaligned_##unpack_fn, pack1_fn, pack2_fn, __VA_ARGS__ RightParenthesis\
		} else {\
			conversion_macro LeftParenthesis unaligned_##unpack_fn, unaligned_##pack1_fn, unaligned_##pack2_fn, __VA_ARGS__ RightParenthesis\
		}\
	}

#define DO_CONVERSION_3U_1P(conversion_macro, unpack1_fn, unpack2_fn, unpack3_fn, pack_fn, ...)\
	if (((uintptr_t)source_buffer & 0x0F) == 0) {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			conversion_macro LeftParenthesis unpack1_fn, unpack2_fn, unpack3_fn, pack_fn, __VA_ARGS__ RightParenthesis\
		} else {\
			conversion_macro LeftParenthesis unpack1_fn, unpack2_fn, unpack3_fn, unaligned_##pack_fn, __VA_ARGS__ RightParenthesis\
		}\
	} else {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			conversion_macro LeftParenthesis unaligned_##unpack1_fn, unaligned_##unpack2_fn, unaligned_##unpack3_fn, pack_fn, __VA_ARGS__ RightParenthesis\
		} else {\
			conversion_macro LeftParenthesis unaligned_##unpack1_fn, unaligned_##unpack2_fn, unaligned_##unpack3_fn, unaligned_##pack_fn, __VA_ARGS__ RightParenthesis\
		}\
	}

#define DO_REPACK(repack_macro, repack_fn_suffix, ...) \
	if (((uintptr_t)source_buffer & 0x0F) == 0) {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			repack_macro LeftParenthesis repack_fn_suffix, __VA_ARGS__ RightParenthesis\
		} else {\
			repack_macro LeftParenthesis unaligned_dst_##repack_fn_suffix, __VA_ARGS__ RightParenthesis\
		}\
	} else {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			repack_macro LeftParenthesis unaligned_src_##repack_fn_suffix, __VA_ARGS__ RightParenthesis\
		} else {\
			repack_macro LeftParenthesis unaligned_src_unaligned_dst_##repack_fn_suffix, __VA_ARGS__ RightParenthesis\
		}\
	}

#define DO_REPACK2(repack_macro, unpack_fn_prefix, pack_fn, ...) \
	if (((uintptr_t)source_buffer & 0x0F) == 0) {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			repack_macro LeftParenthesis unpack_fn_prefix, pack_fn, ##__VA_ARGS__ RightParenthesis\
		} else {\
			repack_macro LeftParenthesis unpack_fn_prefix, unaligned_##pack_fn, ##__VA_ARGS__ RightParenthesis\
		}\
	} else {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			repack_macro LeftParenthesis unaligned_##unpack_fn_prefix, pack_fn, ##__VA_ARGS__ RightParenthesis\
		} else {\
			repack_macro LeftParenthesis unaligned_##unpack_fn_prefix, unaligned_##pack_fn, ##__VA_ARGS__ RightParenthesis\
		}\
	}

#else // ! Windows


/*
 * Outer loop for conversions to / from v210.
 * This loop makes the following assumptions:
 * - a width multiple of 8,
 * - the v210 core conversion loop handles 24 pixels at a time,
 * - v210 has a buffer large enough to accommodate multiple of 48 pixels
 *   (even though the width (in pixel) does not have to be multiple of 48
 *    thanks to padding bytes at the end of each line).
 *
 * There are only 3 cases to cover depending on the width:
 *
 * - width % 24 == 0,
 * - width % 24 == 8,
 * - width % 24 == 16
 *
 * The macro manages the v210 and output buffer pointers so they always will point to the right place.
 *
 */
#define FROM_V120_24_PIX_OUTER_CONVERSION_LOOP(v210_ptr, out_ptr, core, core_last24, leftover_8, leftover_16, ...)\
		uint32_t	width = pixfc->width;\
		uint32_t	line = pixfc->height;\
		uint8_t		*src = (uint8_t *)source_buffer;\
		uint8_t		*dst = (uint8_t *)dest_buffer;\
		uint32_t	src_row_byte_count = ROW_SIZE(pixfc->source_fmt, width);\
		uint32_t	dst_row_byte_count = ROW_SIZE(pixfc->dest_fmt, width);\
		uint32_t	pixel;\
		if ((width % 24) == 0) {\
			while(line-- > 0) {\
				v210_ptr = (__m128i *)src;\
				out_ptr = (__m128i *)dst;\
				pixel = width - 24; /* Handle the last 24 pixels outside the loop*/\
				while(pixel > 0) {\
					core(__VA_ARGS__);\
					pixel -= 24;\
				}\
				core_last24(__VA_ARGS__);\
				src += src_row_byte_count;\
				dst += dst_row_byte_count;\
			}\
		} else if ((width % 24) == 8) {\
			while(line-- > 0) {\
				v210_ptr = (__m128i *)src;\
				out_ptr = (__m128i *)dst;\
				pixel = width; /* Handle the last 24 pixels outside the loop*/\
				while(pixel > 24) {\
					core(__VA_ARGS__);\
					pixel -= 24;\
				}\
				leftover_8(__VA_ARGS__);\
				src += src_row_byte_count;\
				dst += dst_row_byte_count;\
			}\
		} else { /* width % 24 == 16 */\
			while(line-- > 0) {\
				v210_ptr = (__m128i *)src;\
				out_ptr = (__m128i *)dst;\
				pixel = width; /* Handle the last 24 pixels outside the loop*/\
				while(pixel > 24) {\
					core(__VA_ARGS__);\
					pixel -= 24;\
				}\
				leftover_16(__VA_ARGS__);\
				src += src_row_byte_count;\
				dst += dst_row_byte_count;\
			}\
		}

#define TO_V120_24_PIX_OUTER_CONVERSION_LOOP(in_ptr, v210_ptr, core_first24, core, first_leftover_8, leftover_8, first_leftover_16, leftover_16, ...)\
		uint32_t	width = pixfc->width;\
		uint32_t	line = pixfc->height;\
		uint8_t		*src = (uint8_t *)source_buffer;\
		uint8_t		*dst = (uint8_t *)dest_buffer;\
		uint32_t	src_row_byte_count = ROW_SIZE(pixfc->source_fmt, width);\
		uint32_t	dst_row_byte_count = ROW_SIZE(pixfc->dest_fmt, width);\
		uint32_t	pixel;\
		if ((width % 24) == 0) {\
			while(line-- > 0) {\
				in_ptr = (__m128i *)src;\
				v210_ptr = (__m128i *)dst;\
				src += src_row_byte_count;\
				dst += dst_row_byte_count;\
				pixel = width - 24; /* Handle the first 24 pixels outside the loop*/\
				core_first24(__VA_ARGS__);\
				while(pixel >= 24) {\
					core(__VA_ARGS__);\
					pixel -= 24;\
				}\
			}\
		} else if ((width % 24) == 8) {\
			if (width == 8) {/* there are only 8 pixels on each line*/\
				while(line-- > 0) {\
					in_ptr = (__m128i *)src;\
					v210_ptr = (__m128i *)dst;\
					src += src_row_byte_count;\
					dst += dst_row_byte_count;\
					first_leftover_8(__VA_ARGS__);\
				}\
			} else {\
				while(line-- > 0) {\
					in_ptr = (__m128i *)src;\
					v210_ptr = (__m128i *)dst;\
					src += src_row_byte_count;\
					dst += dst_row_byte_count;\
					pixel = width - 24; /* Handle the first 24 pixels outside the loop*/\
					core_first24(__VA_ARGS__);\
					while(pixel > 24) {\
						core(__VA_ARGS__);\
						pixel -= 24;\
					}\
					leftover_8(__VA_ARGS__);\
				}\
			}\
		} else { /* width % 24 == 16 */\
			if (width == 16) {/* there are only 16 pixels on each line*/\
				while(line-- > 0) {\
					in_ptr = (__m128i *)src;\
					v210_ptr = (__m128i *)dst;\
					src += src_row_byte_count;\
					dst += dst_row_byte_count;\
					first_leftover_16(__VA_ARGS__);\
					v210_ptr += 4;\
				}\
			} else {\
				while(line-- > 0) {\
					in_ptr = (__m128i *)src;\
					v210_ptr = (__m128i *)dst;\
					src += src_row_byte_count;\
					dst += dst_row_byte_count;\
					pixel = width - 24; /* Handle the first 24 pixels outside the loop*/\
					core_first24(__VA_ARGS__);\
					while(pixel > 24) {\
						core(__VA_ARGS__);\
						pixel -= 24;\
					}\
					leftover_16(__VA_ARGS__);\
				}\
			}\
		}

/*
 * Outer loop for conversions to / from v210.
 * This loop makes the following assumptions:
 * - a width multiple of 16,
 * - the v210 core conversion loop handles 48 pixels at a time,
 * - v210 has a buffer large enough to accommodate multiple of 48 pixels
 *   (even though the width (in pixel) does not have to be multiple of 48
 *    thanks to padding bytes at the end of each line).
 *
 * Because of these assumptions, there are 6 cases to cover depending on width % 48:
 * 0, 8, 16, 24, 32, 40
 */
#define FROM_V210_48_PIX_INNER_LOOP(v210_ptr, out_ptr, core_preamble, core, leftovers, ...) \
			while(line-- > 0) {\
				v210_ptr = (__m128i *)src;\
				out_ptr = (__m128i *)dst;\
				core_preamble(__VA_ARGS__);\
				while(pixel > 48) /* handle as many chunks of 48 pixels in the loop */{\
					core(__VA_ARGS__);\
					pixel -= 48;\
				}\
				leftovers(__VA_ARGS__);\
				src += src_row_byte_count;\
				dst += dst_row_byte_count;\
				pixel = width;\
			}

#define FROM_V120_48_PIX_OUTER_CONVERSION_LOOP(v210_ptr, out_ptr, core_preamble, core, last_48, leftover_8, leftover_16, leftover_24, leftover_32, leftover_40, ...)\
		uint32_t	width = pixfc->width;\
		uint32_t	line = pixfc->height;\
		uint32_t	pixel = width;\
		uint8_t		*src = (uint8_t *)source_buffer;\
		uint8_t		*dst = (uint8_t *)dest_buffer;\
		uint32_t	src_row_byte_count = ROW_SIZE(pixfc->source_fmt, width);\
		uint32_t	dst_row_byte_count = ROW_SIZE(pixfc->dest_fmt, width);\
		if ((width % 48) == 0) {\
			FROM_V210_48_PIX_INNER_LOOP(v210_ptr, out_ptr, core_preamble, core, last_48, __VA_ARGS__)\
		} else if ((width % 48) == 8) {\
			FROM_V210_48_PIX_INNER_LOOP(v210_ptr, out_ptr, core_preamble, core, leftover_8, __VA_ARGS__)\
		} else if ((width % 48) == 16) {\
			FROM_V210_48_PIX_INNER_LOOP(v210_ptr, out_ptr, core_preamble, core, leftover_16, __VA_ARGS__)\
		} else if ((width % 48) == 24) {\
			FROM_V210_48_PIX_INNER_LOOP(v210_ptr, out_ptr, core_preamble, core, leftover_24, __VA_ARGS__)\
		} else if ((width % 48) == 32) {\
			FROM_V210_48_PIX_INNER_LOOP(v210_ptr, out_ptr, core_preamble, core, leftover_32, __VA_ARGS__)\
 		} else if ((width % 48) == 40) {\
			FROM_V210_48_PIX_INNER_LOOP(v210_ptr, out_ptr, core_preamble, core, leftover_40, __VA_ARGS__)\
		}

#define TO_V120_48_PIX_OUTER_CONVERSION_LOOP(core_first48, core, first_leftover_16, leftover_16, first_leftover_32, leftover_32, ...)\
		uint32_t	width = pixfc->width;\
		uint32_t	line = pixfc->height;\
		uint32_t	pixel = width - 48; /* handle the fist 48 pixels outside the loop*/\
		if ((width % 48) == 0) {\
			while(line-- > 0) {\
				core_first48(__VA_ARGS__);\
				while(pixel > 0) {\
					core(__VA_ARGS__);\
					pixel -= 48;\
				}\
				/* v210_ptr already points to the start of next line */;\
				pixel = width - 48;\
			}\
		} else if ((width % 48) == 16) {\
			if (width == 16){\
				while(line-- > 0) {\
					first_leftover_16(__VA_ARGS__);\
					/* v210_ptr already points to the start of next line */;\
					pixel = width - 48;\
				}\
			} else {\
				while(line-- > 0) {\
					core_first48(__VA_ARGS__);\
					while(pixel > 48) {\
						core(__VA_ARGS__);\
						pixel -= 48;\
					}\
					leftover_16(__VA_ARGS__);\
					/* v210_ptr already points to the start of next line */;\
					pixel = width - 48;\
				}\
			}\
		} else { /* width % 48 == 32 */\
			if (width == 32) {\
				while(line-- > 0) {\
					first_leftover_32(__VA_ARGS__);\
					/* v210_ptr already points to the start of next line */;\
				}\
			} else {\
				while(line-- > 0) {\
					core_first48(__VA_ARGS__);\
					while(pixel > 48) {\
						core(__VA_ARGS__);\
						pixel -= 48;\
					}\
					leftover_32(__VA_ARGS__);\
					/* v210_ptr already points to the start of next line */;\
					pixel = width - 48;\
				}\
			}\
		}

#define DO_CONVERSION_1U_1P(conversion_macro, unpack_fn, pack_fn, ...)\
	if (((uintptr_t)source_buffer & 0x0F) == 0) {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			conversion_macro(unpack_fn, pack_fn, __VA_ARGS__)\
		} else {\
			conversion_macro(unpack_fn, unaligned_##pack_fn, __VA_ARGS__)\
		}\
	} else {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			conversion_macro(unaligned_##unpack_fn, pack_fn, __VA_ARGS__)\
		} else {\
			conversion_macro(unaligned_##unpack_fn, unaligned_##pack_fn, __VA_ARGS__)\
		}\
	}

#define DO_CONVERSION_1U_2P(conversion_macro, unpack_fn, pack1_fn, pack2_fn, ...)\
	if (((uintptr_t)source_buffer & 0x0F) == 0) {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			conversion_macro(unpack_fn, pack1_fn, pack2_fn, __VA_ARGS__)\
		} else {\
			conversion_macro(unpack_fn, unaligned_##pack1_fn, unaligned_##pack2_fn, __VA_ARGS__)\
		}\
	} else {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			conversion_macro(unaligned_##unpack_fn, pack1_fn, pack2_fn, __VA_ARGS__)\
		} else {\
			conversion_macro(unaligned_##unpack_fn, unaligned_##pack1_fn, unaligned_##pack2_fn, __VA_ARGS__)\
		}\
	}

#define DO_CONVERSION_3U_1P(conversion_macro, unpack1_fn, unpack2_fn, unpack3_fn, pack_fn, ...)\
	if (((uintptr_t)source_buffer & 0x0F) == 0) {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			conversion_macro(unpack1_fn, unpack2_fn, unpack3_fn, pack_fn, __VA_ARGS__)\
		} else {\
			conversion_macro(unpack1_fn, unpack2_fn, unpack3_fn, unaligned_##pack_fn, __VA_ARGS__)\
		}\
	} else {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			conversion_macro(unaligned_##unpack1_fn, unaligned_##unpack2_fn, unaligned_##unpack3_fn, pack_fn, __VA_ARGS__)\
		} else {\
			conversion_macro(unaligned_##unpack1_fn, unaligned_##unpack2_fn, unaligned_##unpack3_fn, unaligned_##pack_fn, __VA_ARGS__)\
		}\
	}

#define DO_REPACK(repack_macro, repack_fn_suffix, ...) \
	if (((uintptr_t)source_buffer & 0x0F) == 0) {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			repack_macro(repack_fn_suffix, __VA_ARGS__)\
		} else {\
			repack_macro(unaligned_dst_##repack_fn_suffix, __VA_ARGS__)\
		}\
	} else {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			repack_macro(unaligned_src_##repack_fn_suffix, __VA_ARGS__)\
		} else {\
			repack_macro(unaligned_src_unaligned_dst_##repack_fn_suffix, __VA_ARGS__)\
		}\
	}

#define DO_REPACK2(repack_macro, unpack_fn_prefix, pack_fn, ...) \
	if (((uintptr_t)source_buffer & 0x0F) == 0) {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			repack_macro(unpack_fn_prefix, pack_fn, ##__VA_ARGS__)\
		} else {\
			repack_macro(unpack_fn_prefix, unaligned_##pack_fn, ##__VA_ARGS__)\
		}\
	} else {\
		if (((uintptr_t)dest_buffer & 0x0F) == 0){\
			repack_macro(unaligned_##unpack_fn_prefix, pack_fn, ##__VA_ARGS__)\
		} else {\
			repack_macro(unaligned_##unpack_fn_prefix, unaligned_##pack_fn, ##__VA_ARGS__)\
		}\
	}

#endif

// Declare a __m128i variable, and load one unaligned __m128i vector from the unaligned buffer
#define	DECLARE_VECT_N_UNALIGN_LOAD(var, unaligned_buffer_ptr)\
		__m128i var = _mm_loadu_si128(unaligned_buffer_ptr)

// Declare an array of 2 __m128i elements, and load 2 unaligned __m128i vectors from the unaligned buffer
#define	DECLARE_VECT_ARRAY2_N_UNALIGN_LOAD(var, unaligned_buffer_ptr)\
		__m128i var[2]; (var)[0] = _mm_loadu_si128(unaligned_buffer_ptr); (var)[1] = _mm_loadu_si128(&unaligned_buffer_ptr[1])

// Same as above with 3 elements
#define	DECLARE_VECT_ARRAY3_N_UNALIGN_LOAD(var, unaligned_buffer_ptr)\
		__m128i var[3]; (var)[0] = _mm_loadu_si128(unaligned_buffer_ptr); (var)[1] = _mm_loadu_si128(&unaligned_buffer_ptr[1]); (var)[2] = _mm_loadu_si128(&unaligned_buffer_ptr[2])

// Same as above with 4 elements
#define	DECLARE_VECT_ARRAY4_N_UNALIGN_LOAD(var, unaligned_buffer_ptr)\
		__m128i var[4]; (var)[0] = _mm_loadu_si128(unaligned_buffer_ptr); (var)[1] = _mm_loadu_si128(&unaligned_buffer_ptr[1]); (var)[2] = _mm_loadu_si128(&unaligned_buffer_ptr[2]); (var)[3] = _mm_loadu_si128(&unaligned_buffer_ptr[3])

#else	// __INTEL_CPU__

#define DO_CONVERSION_1U_1P(conversion_macro, unpack_fn, pack_fn, ...)
#define DO_CONVERSION_1U_2P(conversion_macro, unpack_fn, pack1_fn, pack2_fn, ...)
#define DO_CONVERSION_3U_1P(conversion_macro, unpack1_fn, unpack2_fn, unpack3_fn, pack_fn, ...)
#define DO_REPACK(repack_macro, repack_fn_suffix, ...) 
#define DO_REPACK2(...)

#endif // __INTEL_CPU__



#define EMPTY(...)


/*
 * This function returns the features supported by the cpu
 * as returned by the CPUID instructions. (ECX is in the higher
 * 32 bits, EDX in the lowest ones). Use the following macros to
 * determine which features are supported.
 */
uint64_t		get_cpu_features();
#define CPUID_FEATURE_SSE2	0x0000000004000000LL
#define CPUID_FEATURE_SSE3	0x0000000100000000LL
#define CPUID_FEATURE_SSSE3	0x0000020000000000LL
#define CPUID_FEATURE_SSE41	0x0008000000000000LL
#define CPUID_FEATURE_NONE	0xFFFFFFFFFFFFFFFFLL	// matches all CPUs

/*
 * Check if the CPU we are running on has the given features (mask
 * of above-defined CPUID_FEATURE_* macros).
 * Return 0 if yes, -1 if not.
 */
uint32_t			does_cpu_support(uint64_t);


/*
 * Various debugging functions
 */
#if defined(DEBUG) && defined(__INTEL_CPU__)

void 						print_xmm32(char *name, __m128i *reg);
void 						print_xmm32x(char *name, __m128i *reg);
void 						print_xmm16(char *name, __m128i *reg);
void 						print_xmm16u(char *name, __m128i *reg);
void 						print_xmm16x(char *name, __m128i *reg);
void 						print_xmm10u(char *name, __m128i *reg);
void						print_xmm8u(char *name, __m128i *reg);
void						print_xmm8x(char *name, __m128i *reg);

#else

#define print_xmm32(...)
#define print_xmm32x(...)
#define print_xmm16(...)
#define print_xmm16u(...)
#define print_xmm16x(...)
#define print_xmm10u(...)
#define print_xmm8u(...)
#define print_xmm8x(...)

#endif	// DEBUG

#endif /* COMMON_H_ */
