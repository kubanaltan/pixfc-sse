/*
 * conversion_blocks.c
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

#include <stdint.h>

#include "common.h"
#include "conversion_blocks.h"
#include "conversion_routines_from_yuyv.h"
#include "conversion_routines_from_uyvy.h"
#include "conversion_routines_from_argb.h"


/*
 * In this array, conversion blocks for a given pair of source and destination
 * formats must be sorted: fastest first, slowest last !!!
 */
const struct  ConversionBlock		conversion_blocks[] = {

	/*
	 *	S O U R C E   F O R M A T :
	 *	
	 *	A R G B
	 *
	 */
	// ARGB to YUYV
	{
		convert_argb_to_yuyv_sse2_ssse3,
		PixFcARGB, PixFcYUYV,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		NNB_RESAMPLING,
		16,
		"ARGB to YUYV - SSE2 / SSSE3- fast downsampling",
	},
	{
		convert_argb_to_yuyv_sse2,
		PixFcARGB, PixFcYUYV,
		CPUID_FEATURE_SSE2,
		NNB_RESAMPLING,
		16,
		"ARGB to YUYV - SSE2 - fast downsampling",
	},
	{
		convert2_argb_to_yuyv_sse2,
		PixFcARGB, PixFcYUYV,
		CPUID_FEATURE_SSE2,
		NNB_RESAMPLING,
		16,
		"ARGB to YUYV (2)- SSE2 - fast downsampling",
	},
	{
		downsample_n_convert_argb_to_yuyv_sse2_ssse3,
		PixFcARGB, PixFcYUYV,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		DEFAULT_ATTRIBUTE,
		16,
		"ARGB to YUYV - SSE2 / SSSE3",
		
	},
	{
		downsample_n_convert_argb_to_yuyv_sse2,
		PixFcARGB, PixFcYUYV,
		CPUID_FEATURE_SSE2,
		DEFAULT_ATTRIBUTE,
		16,
		"ARGB to YUYV - SSE2",
		
	},
	{
		downsample_n_convert2_argb_to_yuyv_sse2,
		PixFcARGB, PixFcYUYV,
		CPUID_FEATURE_SSE2,
		DEFAULT_ATTRIBUTE,
		16,
		"ARGB to YUYV (2)- SSE2",

	},


	/*
	 *	S O U R C E   F O R M A T :
	 *
	 *	Y U Y V
	 *
	 */

	// YUYV to ARGB
	{
		convert_yuyv_to_argb_sse2_ssse3,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		NNB_RESAMPLING,
		16,
		"YUYV to ARGB - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_yuyv_to_argb_bt601_sse2_ssse3,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to ARGB - BT601 - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_yuyv_to_argb_bt709_sse2_ssse3,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to ARGB - BT709 - SSE2 / SSSE3 - fast upsampling",
	},


	{
		upsample_n_convert_yuyv_to_argb_sse2_ssse3,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		DEFAULT_ATTRIBUTE,
		16,
		"YUYV to ARGB - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_yuyv_to_argb_bt601_sse2_ssse3,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION,
		16,
		"YUYV to ARGB - BT601 - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_yuyv_to_argb_bt709_sse2_ssse3,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION,
		16,
		"YUYV to ARGB - BT709 - SSE2 / SSSE3",
	},


	{
		convert_yuyv_to_argb_sse2,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_SSE2,
		NNB_RESAMPLING,
		16,
		"YUYV to ARGB - SSE2 - fast upsampling",
	},
	{
		convert_yuyv_to_argb_bt601_sse2,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to ARGB - BT601 - SSE2 - fast upsampling",
	},
	{
		convert_yuyv_to_argb_bt709_sse2,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to ARGB - BT709 - SSE2 - fast upsampling",
	},


	{
		upsample_n_convert_yuyv_to_argb_sse2,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_SSE2,
		DEFAULT_ATTRIBUTE,
		16,
		"YUYV to ARGB - SSE2",
	},
	{
		upsample_n_convert_yuyv_to_argb_bt601_sse2,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION,
		16,
		"YUYV to ARGB - BT601 - SSE2",
	},
	{
		upsample_n_convert_yuyv_to_argb_bt709_sse2,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION,
		16,
		"YUYV to ARGB - BT709 - SSE2",
	},


	{
		convert_yuyv_to_any_rgb_nonsse,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_NONE,
		NNB_RESAMPLING,
		1,
		"YUYV to ARGB - NON-SSE",
	},
	{
		convert_yuyv_to_any_rgb_bt601_nonsse,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_NONE,
		BT601_CONVERSION | NNB_RESAMPLING,
		1,
		"YUYV to ARGB - BT601 - NON-SSE",
	},
	{
		convert_yuyv_to_any_rgb_bt709_nonsse,
		PixFcYUYV, PixFcARGB,
		CPUID_FEATURE_NONE,
		BT709_CONVERSION | NNB_RESAMPLING,
		1,
		"YUYV to ARGB - BT709 - NON-SSE",
	},


	// YUYV to BGRA
	{
		convert_yuyv_to_bgra_sse2_ssse3,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		NNB_RESAMPLING,
		16,
		"YUYV to BGRA - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_yuyv_to_bgra_bt601_sse2_ssse3,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to BGRA - BT601 - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_yuyv_to_bgra_bt709_sse2_ssse3,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to BGRA - BT709 - SSE2 / SSSE3 - fast upsampling",
	},


	{
		upsample_n_convert_yuyv_to_bgra_sse2_ssse3,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		DEFAULT_ATTRIBUTE,
		16,
		"YUYV to BGRA - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_yuyv_to_bgra_bt601_sse2_ssse3,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION,
		16,
		"YUYV to BGRA - BT601 - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_yuyv_to_bgra_bt709_sse2_ssse3,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION,
		16,
		"YUYV to BGRA - BT709 - SSE2 / SSSE3",
	},


	{
		convert_yuyv_to_bgra_sse2,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_SSE2,
		NNB_RESAMPLING,
		16,
		"YUYV to BGRA - SSE2 - fast upsampling",
	},
	{
		convert_yuyv_to_bgra_bt601_sse2,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to BGRA - BT601 - SSE2 - fast upsampling",
	},
	{
		convert_yuyv_to_bgra_bt709_sse2,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to BGRA - BT709 - SSE2 - fast upsampling",
	},


	{
		upsample_n_convert_yuyv_to_bgra_sse2,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_SSE2,
		DEFAULT_ATTRIBUTE,
		16,
		"YUYV to BGRA - SSE2",
	},
	{
		upsample_n_convert_yuyv_to_bgra_bt601_sse2,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION,
		16,
		"YUYV to BGRA - BT601 - SSE2",
	},
	{
		upsample_n_convert_yuyv_to_bgra_bt709_sse2,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION,
		16,
		"YUYV to BGRA - BT709 - SSE2",
	},


	{
		convert_yuyv_to_any_rgb_nonsse,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_NONE,
		NNB_RESAMPLING,
		1,
		"YUYV to BGRA - NON-SSE",
	},
	{
		convert_yuyv_to_any_rgb_bt601_nonsse,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_NONE,
		BT601_CONVERSION | NNB_RESAMPLING,
		1,
		"YUYV to BGRA - BT601 - NON-SSE",
	},
	{
		convert_yuyv_to_any_rgb_bt709_nonsse,
		PixFcYUYV, PixFcBGRA,
		CPUID_FEATURE_NONE,
		BT709_CONVERSION | NNB_RESAMPLING,
		1,
		"YUYV to BGRA - BT709 - NON-SSE",
	},


	// YUYV to RGB24
	{
		convert_yuyv_to_rgb24_sse2_ssse3,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		NNB_RESAMPLING,
		16,
		"YUYV to RGB24 - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_yuyv_to_rgb24_bt601_sse2_ssse3,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to RGB24 - BT601 - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_yuyv_to_rgb24_bt709_sse2_ssse3,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to RGB24 - BT709 - SSE2 / SSSE3 - fast upsampling",
	},


	{
		upsample_n_convert_yuyv_to_rgb24_sse2_ssse3,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		DEFAULT_ATTRIBUTE,
		16,
		"YUYV to RGB24 - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_yuyv_to_rgb24_bt601_sse2_ssse3,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION,
		16,
		"YUYV to RGB24 - BT601 - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_yuyv_to_rgb24_bt709_sse2_ssse3,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION,
		16,
		"YUYV to RGB24 - BT709 - SSE2 / SSSE3",
	},


	{
		convert_yuyv_to_rgb24_sse2_slowrepacking,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_SSE2,
		NNB_RESAMPLING,
		16,
		"YUYV to RGB24 - SSE2 / slower repacking - fast upsampling",
	},
	{
		convert_yuyv_to_rgb24_bt601_sse2_slowrepacking,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to RGB24 - BT601 - SSE2 / slower repacking - fast upsampling",
	},
	{
		convert_yuyv_to_rgb24_bt709_sse2_slowrepacking,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to RGB24 - BT709 - SSE2 / slower repacking - fast upsampling",
	},



	{
		upsample_n_convert_yuyv_to_rgb24_sse2_slowrepacking,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_SSE2,
		DEFAULT_ATTRIBUTE,
		16,
		"YUYV to RGB24 - SSE2 / slower repacking",
	},
	{
		upsample_n_convert_yuyv_to_rgb24_bt601_sse2_slowrepacking,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION,
		16,
		"YUYV to RGB24 - BT601 - SSE2 / slower repacking",
	},
	{
		upsample_n_convert_yuyv_to_rgb24_bt709_sse2_slowrepacking,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION,
		16,
		"YUYV to RGB24 - BT709 - SSE2 / slower repacking",
	},


	{
		convert_yuyv_to_any_rgb_nonsse,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_NONE,
		NNB_RESAMPLING,
		1,
		"YUYV to RGB24 - NON-SSE",
	},
	{
		convert_yuyv_to_any_rgb_bt601_nonsse,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_NONE,
		BT601_CONVERSION | NNB_RESAMPLING,
		1,
		"YUYV to RGB24 - BT601 - NON-SSE",
	},
	{
		convert_yuyv_to_any_rgb_bt709_nonsse,
		PixFcYUYV, PixFcRGB24,
		CPUID_FEATURE_NONE,
		BT709_CONVERSION | NNB_RESAMPLING,
		1,
		"YUYV to RGB24 - BT709 - NON-SSE",
	},

	// YUYV to BGR24
	{
		convert_yuyv_to_bgr24_sse2_ssse3,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		NNB_RESAMPLING,
		16,
		"YUYV to BGR24 - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_yuyv_to_bgr24_bt601_sse2_ssse3,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to BGR24 - BT601 - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_yuyv_to_bgr24_bt709_sse2_ssse3,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to BGR24 - BT709 - SSE2 / SSSE3 - fast upsampling",
	},


	{
		upsample_n_convert_yuyv_to_bgr24_sse2_ssse3,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		DEFAULT_ATTRIBUTE,
		16,
		"YUYV to BGR24 - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_yuyv_to_bgr24_bt601_sse2_ssse3,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION,
		16,
		"YUYV to BGR24 - BT601 - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_yuyv_to_bgr24_bt709_sse2_ssse3,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION,
		16,
		"YUYV to BGR24 - BT709 - SSE2 / SSSE3",
	},



	{
		convert_yuyv_to_bgr24_sse2_slowrepacking,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_SSE2,
		NNB_RESAMPLING,
		16,
		"YUYV to BGR24 - SSE2 / slower repacking - fast upsampling",
	},
	{
		convert_yuyv_to_bgr24_bt601_sse2_slowrepacking,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to BGR24 - BT601 - SSE2 / slower repacking - fast upsampling",
	},
	{
		convert_yuyv_to_bgr24_bt709_sse2_slowrepacking,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"YUYV to BGR24 - BT709 - SSE2 / slower repacking - fast upsampling",
	},


	{
		upsample_n_convert_yuyv_to_bgr24_sse2_slowrepacking,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_SSE2,
		DEFAULT_ATTRIBUTE,
		16,
		"YUYV to BGR24 - SSE2 / slower repacking",
	},
	{
		upsample_n_convert_yuyv_to_bgr24_bt601_sse2_slowrepacking,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION,
		16,
		"YUYV to BGR24 - BT601 - SSE2 / slower repacking",
	},
	{
		upsample_n_convert_yuyv_to_bgr24_bt709_sse2_slowrepacking,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION,
		16,
		"YUYV to BGR24 - BT709 - SSE2 / slower repacking",
	},



	{
		convert_yuyv_to_any_rgb_nonsse,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_NONE,
		NNB_RESAMPLING,
		1,
		"YUYV to BGR24 - NON-SSE",
	},
	{
		convert_yuyv_to_any_rgb_bt601_nonsse,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_NONE,
		BT601_CONVERSION | NNB_RESAMPLING,
		1,
		"YUYV to BGR24 - BT601 - NON-SSE",
	},
	{
		convert_yuyv_to_any_rgb_bt709_nonsse,
		PixFcYUYV, PixFcBGR24,
		CPUID_FEATURE_NONE,
		BT709_CONVERSION | NNB_RESAMPLING,
		1,
		"YUYV to BGR24 - BT709 - NON-SSE",
	},




	//
	//	S O U R C E   F O R M A T :
	//
	//	U Y V Y
	//
	//

	// UYVY to ARGB
	{
		convert_uyvy_to_argb_sse2_ssse3,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		NNB_RESAMPLING,
		16,
		"UYVY to ARGB - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_uyvy_to_argb_bt601_sse2_ssse3,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to ARGB - BT601 - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_uyvy_to_argb_bt709_sse2_ssse3,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to ARGB - BT709 - SSE2 / SSSE3 - fast upsampling",
	},


	{
		upsample_n_convert_uyvy_to_argb_sse2_ssse3,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		DEFAULT_ATTRIBUTE,
		16,
		"UYVY to ARGB - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_uyvy_to_argb_bt601_sse2_ssse3,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION,
		16,
		"UYVY to ARGB - BT601 - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_uyvy_to_argb_bt709_sse2_ssse3,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION,
		16,
		"UYVY to ARGB - BT709 - SSE2 / SSSE3",
	},



	{
		convert_uyvy_to_argb_sse2,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_SSE2,
		NNB_RESAMPLING,
		16,
		"UYVY to ARGB - SSE2 - fast upsampling",
	},
	{
		convert_uyvy_to_argb_bt601_sse2,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to ARGB - BT601 - SSE2 - fast upsampling",
	},
	{
		convert_uyvy_to_argb_bt709_sse2,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to ARGB - BT709 - SSE2 - fast upsampling",
	},



	{
		upsample_n_convert_uyvy_to_argb_sse2,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_SSE2,
		DEFAULT_ATTRIBUTE,
		16,
		"UYVY to ARGB - SSE2",
	},
	{
		upsample_n_convert_uyvy_to_argb_bt601_sse2,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION,
		16,
		"UYVY to ARGB - BT601- SSE2",
	},
	{
		upsample_n_convert_uyvy_to_argb_bt709_sse2,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION,
		16,
		"UYVY to ARGB - BT709 - SSE2",
	},



	{
		convert_uyvy_to_any_rgb_nonsse,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_NONE,
		NNB_RESAMPLING,
		16,
		"UYVY to ARGB - NON-SSE",
	},
	{
		convert_uyvy_to_any_rgb_bt601_nonsse,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_NONE,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to ARGB - BT601 - NON-SSE",
	},
	{
		convert_uyvy_to_any_rgb_bt709_nonsse,
		PixFcUYVY, PixFcARGB,
		CPUID_FEATURE_NONE,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to ARGB - BT709 - NON-SSE",
	},


	// UYVY to BGRA
	{
		convert_uyvy_to_bgra_sse2_ssse3,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		NNB_RESAMPLING,
		16,
		"UYVY to BGRA - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_uyvy_to_bgra_bt601_sse2_ssse3,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to BGRA - BT601 - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_uyvy_to_bgra_bt709_sse2_ssse3,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to BGRA - BT709 - SSE2 / SSSE3 - fast upsampling",
	},



	{
		upsample_n_convert_uyvy_to_bgra_sse2_ssse3,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		DEFAULT_ATTRIBUTE,
		16,
		"UYVY to BGRA - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_uyvy_to_bgra_bt601_sse2_ssse3,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION,
		16,
		"UYVY to BGRA - BT601 - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_uyvy_to_bgra_bt709_sse2_ssse3,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION,
		16,
		"UYVY to BGRA - BT709 - SSE2 / SSSE3",
	},


	{
		convert_uyvy_to_bgra_sse2,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_SSE2,
		NNB_RESAMPLING,
		16,
		"UYVY to BGRA - SSE2 - fast upsampling",
	},
	{
		convert_uyvy_to_bgra_bt601_sse2,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to BGRA - BT601 - SSE2 - fast upsampling",
	},
	{
		convert_uyvy_to_bgra_bt709_sse2,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to BGRA - BT709 - SSE2 - fast upsampling",
	},


	{
		upsample_n_convert_uyvy_to_bgra_sse2,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_SSE2,
		DEFAULT_ATTRIBUTE,
		16,
		"UYVY to BGRA - SSE2",
	},
	{
		upsample_n_convert_uyvy_to_bgra_bt601_sse2,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION,
		16,
		"UYVY to BGRA - BT601 - SSE2",
	},
	{
		upsample_n_convert_uyvy_to_bgra_bt709_sse2,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION,
		16,
		"UYVY to BGRA - BT709 - SSE2",
	},


	{
		convert_uyvy_to_any_rgb_nonsse,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_NONE,
		NNB_RESAMPLING,
		16,
		"UYVY to BGRA - NON-SSE",
	},
	{
		convert_uyvy_to_any_rgb_bt601_nonsse,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_NONE,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to BGRA - BT601 - NON-SSE",
	},
	{
		convert_uyvy_to_any_rgb_bt709_nonsse,
		PixFcUYVY, PixFcBGRA,
		CPUID_FEATURE_NONE,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to BGRA - BT709 - NON-SSE",
	},

	// UYVY to RGB24
	{
		convert_uyvy_to_rgb24_sse2_ssse3,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		NNB_RESAMPLING,
		16,
		"UYVY to RGB24 - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_uyvy_to_rgb24_bt601_sse2_ssse3,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to RGB24 - BT601 - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_uyvy_to_rgb24_bt709_sse2_ssse3,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to RGB24 - BT709 - SSE2 / SSSE3 - fast upsampling",
	},


	{
		upsample_n_convert_uyvy_to_rgb24_sse2_ssse3,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		DEFAULT_ATTRIBUTE,
		16,
		"UYVY to RGB24 - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_uyvy_to_rgb24_bt601_sse2_ssse3,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION,
		16,
		"UYVY to RGB24 - BT601 - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_uyvy_to_rgb24_bt709_sse2_ssse3,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION,
		16,
		"UYVY to RGB24 - BT709 - SSE2 / SSSE3",
	},



	{
		convert_uyvy_to_rgb24_sse2_slowrepacking,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_SSE2,
		NNB_RESAMPLING,
		16,
		"UYVY to RGB24 - SSE2 / slower repacking - fast upsampling",
	},
	{
		convert_uyvy_to_rgb24_bt601_sse2_slowrepacking,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to RGB24 - BT601 - SSE2 / slower repacking - fast upsampling",
	},
	{
		convert_uyvy_to_rgb24_bt709_sse2_slowrepacking,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to RGB24 - BT709 - SSE2 / slower repacking - fast upsampling",
	},


	{
		upsample_n_convert_uyvy_to_rgb24_sse2_slowrepacking,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_SSE2,
		DEFAULT_ATTRIBUTE,
		16,
		"UYVY to RGB24 - SSE2 / slower repacking",
	},
	{
		upsample_n_convert_uyvy_to_rgb24_bt601_sse2_slowrepacking,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION,
		16,
		"UYVY to RGB24 - BT601 - SSE2 / slower repacking",
	},
	{
		upsample_n_convert_uyvy_to_rgb24_bt709_sse2_slowrepacking,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION,
		16,
		"UYVY to RGB24 - BT709 - SSE2 / slower repacking",
	},


	{
		convert_uyvy_to_any_rgb_nonsse,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_NONE,
		NNB_RESAMPLING,
		1,
		"UYVY to RGB24 - NON-SSE",
	},
	{
		convert_uyvy_to_any_rgb_bt601_nonsse,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_NONE,
		BT601_CONVERSION | NNB_RESAMPLING,
		1,
		"UYVY to RGB24 - BT601 - NON-SSE",
	},
	{
		convert_uyvy_to_any_rgb_bt709_nonsse,
		PixFcUYVY, PixFcRGB24,
		CPUID_FEATURE_NONE,
		BT709_CONVERSION | NNB_RESAMPLING,
		1,
		"UYVY to RGB24 - BT709 - NON-SSE",
	},



	// UYVY to BGR24
	{
		convert_uyvy_to_bgr24_sse2_ssse3,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		NNB_RESAMPLING,
		16,
		"UYVY to BGR24 - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_uyvy_to_bgr24_bt601_sse2_ssse3,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to BGR24 - BT601 - SSE2 / SSSE3 - fast upsampling",
	},
	{
		convert_uyvy_to_bgr24_bt709_sse2_ssse3,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to BGR24 - BT709 - SSE2 / SSSE3 - fast upsampling",
	},



	{
		upsample_n_convert_uyvy_to_bgr24_sse2_ssse3,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		DEFAULT_ATTRIBUTE,
		16,
		"UYVY to BGR24 - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_uyvy_to_bgr24_bt601_sse2_ssse3,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT601_CONVERSION,
		16,
		"UYVY to BGR24 - BT601 - SSE2 / SSSE3",
	},
	{
		upsample_n_convert_uyvy_to_bgr24_bt709_sse2_ssse3,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3,
		BT709_CONVERSION,
		16,
		"UYVY to BGR24 - BT709 - SSE2 / SSSE3",
	},



	{
		convert_uyvy_to_bgr24_sse2_slowrepacking,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_SSE2,
		NNB_RESAMPLING,
		16,
		"UYVY to BGR24 - SSE2 / slower repacking - fast upsampling",
	},
	{
		convert_uyvy_to_bgr24_bt601_sse2_slowrepacking,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to BGR24 - BT601 - SSE2 / slower repacking - fast upsampling",
	},
	{
		convert_uyvy_to_bgr24_bt709_sse2_slowrepacking,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION | NNB_RESAMPLING,
		16,
		"UYVY to BGR24 - BT709 - SSE2 / slower repacking - fast upsampling",
	},


	{
		upsample_n_convert_uyvy_to_bgr24_sse2_slowrepacking,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_SSE2,
		DEFAULT_ATTRIBUTE,
		16,
		"UYVY to BGR24 - SSE2 / slower repacking",
	},
	{
		upsample_n_convert_uyvy_to_bgr24_bt601_sse2_slowrepacking,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_SSE2,
		BT601_CONVERSION,
		16,
		"UYVY to BGR24 - BT601 - SSE2 / slower repacking",
	},
	{
		upsample_n_convert_uyvy_to_bgr24_bt709_sse2_slowrepacking,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_SSE2,
		BT709_CONVERSION,
		16,
		"UYVY to BGR24 - BT709 - SSE2 / slower repacking",
	},


	{
		convert_uyvy_to_any_rgb_nonsse,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_NONE,
		NNB_RESAMPLING,
		1,
		"UYVY to BGR24 - NON-SSE",
	},
	{
		convert_uyvy_to_any_rgb_bt601_nonsse,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_NONE,
		BT601_CONVERSION | NNB_RESAMPLING,
		1,
		"UYVY to BGR24 - BT601 - NON-SSE",
	},
	{
		convert_uyvy_to_any_rgb_bt709_nonsse,
		PixFcUYVY, PixFcBGR24,
		CPUID_FEATURE_NONE,
		BT709_CONVERSION | NNB_RESAMPLING,
		1,
		"UYVY to BGR24 - BT709 - NON-SSE",
	},

};

const uint32_t		conversion_blocks_count = sizeof(conversion_blocks) / sizeof(conversion_blocks[0]);


