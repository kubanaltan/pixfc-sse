/*
 * conversion_routines_from_yuyv.c
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

#include "common.h"
#include "pixfc-sse.h"
#include "yuv_conversion_recipes.h"

#define UPSAMPLE_AND_CONVERT_TO_RGB32(pack_fn, instr_set)\
		DO_CONVERSION_1U_1P(\
				UPSAMPLE_YUV422I_TO_RGB_RECIPE,\
				unpack_yuyv_to_y_uv_vectors_,\
				pack_fn,\
				convert_y_uv_vectors_to_rgb_vectors_,\
				4,\
				instr_set\
		)


#define UPSAMPLE_AND_CONVERT_TO_RGB24(pack_fn, instr_set)\
		DO_CONVERSION_1U_1P(\
				UPSAMPLE_YUV422I_TO_RGB_RECIPE,\
				unpack_yuyv_to_y_uv_vectors_,\
				pack_fn,\
				convert_y_uv_vectors_to_rgb_vectors_,\
				3,\
				instr_set\
		)



#define CONVERT_TO_RGB32(pack_fn, instr_set)\
		DO_CONVERSION_1U_1P(\
				YUV422I_TO_RGB_RECIPE,\
				unpack_yuyv_to_y_uv_vectors_,\
				pack_fn,\
				nnb_upsample_n_convert_y_uv_vectors_to_rgb_vectors_,\
				4,\
				instr_set\
		)

#define CONVERT_TO_RGB24(pack_fn, instr_set)\
		DO_CONVERSION_1U_1P(\
				YUV422I_TO_RGB_RECIPE,\
				unpack_yuyv_to_y_uv_vectors_,\
				pack_fn,\
				nnb_upsample_n_convert_y_uv_vectors_to_rgb_vectors_,\
				3,\
				instr_set\
		)

#define UPSAMPLE_AND_CONVERT_TO_RGB10(pack_fn, instr_set) \
		DO_CONVERSION_1U_1P(\
				UPSAMPLE_YUV422I_TO_RGB_RECIPE,\
				unpack_yuyv_to_y_uv_vectors_,\
				pack_fn,\
				convert_8bit_y_uv_vectors_to_10bit_rgb_vectors_,\
				4,\
				instr_set\
		)

#define CONVERT_TO_RGB10(pack_fn, instr_set)\
		DO_CONVERSION_1U_1P(\
				YUV422I_TO_RGB_RECIPE,\
				unpack_yuyv_to_y_uv_vectors_,\
				pack_fn,\
				nnb_upsample_n_convert_8bit_y_uv_vectors_to_10bit_rgb_vectors_,\
				4,\
				instr_set\
		)


/*
 *
 * 		Y U Y V
 *
 * 		to
 *
 * 		A R G B
 */
void		upsample_n_convert_yuyv_to_argb_sse2_ssse3(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	 UPSAMPLE_AND_CONVERT_TO_RGB32(pack_6_rgb_vectors_in_4_argb_vectors_sse2, sse2_ssse3);
}

void		convert_yuyv_to_argb_sse2_ssse3(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	CONVERT_TO_RGB32(pack_6_rgb_vectors_in_4_argb_vectors_sse2,	sse2_ssse3);
}

void		upsample_n_convert_yuyv_to_argb_sse2(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	UPSAMPLE_AND_CONVERT_TO_RGB32(pack_6_rgb_vectors_in_4_argb_vectors_sse2, sse2);
}

void		convert_yuyv_to_argb_sse2(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	CONVERT_TO_RGB32(pack_6_rgb_vectors_in_4_argb_vectors_sse2, sse2);
}


/*
 * 		Y U Y V
 *
 * 		to
 *
 * 		B G R A
 *
 */
void		upsample_n_convert_yuyv_to_bgra_sse2_ssse3(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	UPSAMPLE_AND_CONVERT_TO_RGB32(pack_6_rgb_vectors_in_4_bgra_vectors_sse2, sse2_ssse3);
}

void		convert_yuyv_to_bgra_sse2_ssse3(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	CONVERT_TO_RGB32(pack_6_rgb_vectors_in_4_bgra_vectors_sse2,	sse2_ssse3);
}

void		upsample_n_convert_yuyv_to_bgra_sse2(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	UPSAMPLE_AND_CONVERT_TO_RGB32(pack_6_rgb_vectors_in_4_bgra_vectors_sse2, sse2);
}

void		convert_yuyv_to_bgra_sse2(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	CONVERT_TO_RGB32(pack_6_rgb_vectors_in_4_bgra_vectors_sse2, sse2);
}


/*
 *
 * 		Y U Y V
 *
 * 		to
 *
 * 		R G B   2 4
 */
void		upsample_n_convert_yuyv_to_rgb24_sse2_ssse3(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	UPSAMPLE_AND_CONVERT_TO_RGB24(pack_6_rgb_vectors_in_3_rgb24_vectors_sse2_ssse3, sse2_ssse3);
}

void		convert_yuyv_to_rgb24_sse2_ssse3(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	CONVERT_TO_RGB24(pack_6_rgb_vectors_in_3_rgb24_vectors_sse2_ssse3, sse2_ssse3);
}

void		upsample_n_convert_yuyv_to_rgb24_sse2(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	UPSAMPLE_AND_CONVERT_TO_RGB24(pack_6_rgb_vectors_to_3_rgb24_vectors_sse2_slowpacking, sse2);
}

void		convert_yuyv_to_rgb24_sse2(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	CONVERT_TO_RGB24(pack_6_rgb_vectors_to_3_rgb24_vectors_sse2_slowpacking, sse2);
}

/*
 *
 * 		Y U V Y
 *
 * 		to
 *
 * 		B G R 2 4
 *
 */
void		upsample_n_convert_yuyv_to_bgr24_sse2_ssse3(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	UPSAMPLE_AND_CONVERT_TO_RGB24(pack_6_rgb_vectors_in_3_bgr24_vectors_sse2_ssse3, sse2_ssse3);
}

void		convert_yuyv_to_bgr24_sse2_ssse3(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	CONVERT_TO_RGB24(pack_6_rgb_vectors_in_3_bgr24_vectors_sse2_ssse3, sse2_ssse3);
}

void		upsample_n_convert_yuyv_to_bgr24_sse2(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	UPSAMPLE_AND_CONVERT_TO_RGB24(pack_6_rgb_vectors_to_3_bgr24_vectors_sse2_slowpacking, sse2);
}

void		convert_yuyv_to_bgr24_sse2(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	CONVERT_TO_RGB24(pack_6_rgb_vectors_to_3_bgr24_vectors_sse2_slowpacking, sse2);
}


/*
 *
 * 		Y U V Y
 *
 * 		to
 *
 * 		R 2 1 0
 *
 */
void		upsample_n_convert_yuyv_to_r210_sse2_ssse3(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	UPSAMPLE_AND_CONVERT_TO_RGB10(pack_6_r_g_b_vectors_to_4_r210_sse2_ssse3, sse2_ssse3);
}

void		convert_yuyv_to_r210_sse2_ssse3(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	CONVERT_TO_RGB10(pack_6_r_g_b_vectors_to_4_r210_sse2_ssse3, sse2_ssse3);
}


/*
 * Original yuv to rgb conversion - left here for ref
 */
#ifdef __INTEL_CPU__
void 		convert_yuyv_to_rgb_original(const struct PixFcSSE* conv, void* in, void* out){
	__m128i*	yuyv_8pixels = (__m128i *) in;
	__m128i*	rgb_out_buf = (__m128i *) out;
	uint32_t	pixel_count = conv->pixel_count;
	__m128i		unpack_out[2];
	__m128i		convert_out[6];
	while(pixel_count > 0) {
		unpack_yuyv_to_y_u_v_vectors_sse2(yuyv_8pixels, unpack_out);
		nnb_upsample_n_convert_y_uv_vectors_to_rgb_vectors_sse2(unpack_out, convert_out);
		unpack_yuyv_to_y_u_v_vectors_sse2(&yuyv_8pixels[1], unpack_out);
		nnb_upsample_n_convert_y_uv_vectors_to_rgb_vectors_sse2(unpack_out, &convert_out[3]);
		pack_6_rgb_vectors_to_3_bgr24_vectors_sse2_slowpacking(convert_out, rgb_out_buf);
		yuyv_8pixels += 2;
		rgb_out_buf += 3;
		pixel_count -= 16;
	};
}
#endif


/*
 *  	Y U Y V
 *
 * 		T O
 *
 * 		Y U V 4 2 2 P
 */
void		convert_yuyv_to_yuv422p_sse2(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	DO_REPACK(YUV422I_TO_YUV422P_RECIPE, repack_yuyv_to_yuv422p_, sse2);
}

void		convert_yuyv_to_yuv422p_sse2_ssse3(const struct PixFcSSE * pixfc, void* source_buffer, void* dest_buffer) {
	DO_REPACK(YUV422I_TO_YUV422P_RECIPE, repack_yuyv_to_yuv422p_, sse2_ssse3);
}




/*
 *
 *	Y U Y V
 *
 *	T O
 *
 *	V 2 1 0
 *
 *
 */
// YUYV to V210
void		convert_yuyv_to_v210_sse2_ssse3_sse41(const struct PixFcSSE* pixfc, void* source_buffer, void* dest_buffer) {
	DO_REPACK2(YUV422I_TO_V210_RECIPE, unpack_yuyv_to_y_uv_vectors_sse2_ssse3, pack_6_y_uv_vectors_to_4_v210_vectors_sse2_ssse3_sse41);
}

void		convert_yuyv_to_v210_sse2_ssse3(const struct PixFcSSE* pixfc, void* source_buffer, void* dest_buffer) {
	DO_REPACK2(YUV422I_TO_V210_RECIPE, unpack_yuyv_to_y_uv_vectors_sse2_ssse3, pack_6_y_uv_vectors_to_4_v210_vectors_sse2_ssse3);
}

