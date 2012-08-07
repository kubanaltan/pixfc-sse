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
#include "conversion_routines_from_yuv420p.h"
#include "conversion_routines_from_yuv422p.h"
#include "conversion_routines_from_argb.h"
#include "conversion_routines_from_bgra.h"
#include "conversion_routines_from_rgb24.h"
#include "conversion_routines_from_bgr24.h"
#include "conversion_routines_from_v210.h"

#define 	DECLARE_CONV_BLOCK(convert_fn, src_fmt, dst_fmt, cpuid_flags, attributes, width_mult_count, height_mult_count, row_pix_mult, desc)\
{ convert_fn, src_fmt, dst_fmt, cpuid_flags, attributes, width_mult_count, height_mult_count, row_pix_mult, desc }

//

// NNB resampling NON-SSE {FR, bt.601, bt.709) macros
#define		DECLARE_NNB_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, desc_str_prefix)\
DECLARE_CONV_BLOCK(convert_fn_prefix##_nonsse, 			src_fmt, dst_fmt, 		CPUID_FEATURE_NONE, 		NNB_RESAMPLING, width_mult_count, height_mult_count, 1, desc_str_prefix " - FR - NON-SSE - fast resampling")
#define		DECLARE_NNB_BT601_CONV_BLOCK(bt601_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt601_convert_fn_prefix##_nonsse, 		src_fmt, dst_fmt, 		CPUID_FEATURE_NONE, 		BT601_CONVERSION | NNB_RESAMPLING, width_mult_count, height_mult_count, 1, desc_str_prefix " - bt.601 - NON-SSE - fast resampling")
#define		DECLARE_NNB_BT709_CONV_BLOCK(bt709_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt709_convert_fn_prefix##_nonsse, 		src_fmt, dst_fmt, 		CPUID_FEATURE_NONE, 		BT709_CONVERSION | NNB_RESAMPLING, width_mult_count, height_mult_count, 1, desc_str_prefix " - bt.709 - NON-SSE - fast resampling")

// AVG resampling NON-SSE {FR, bt.601, bt.709) macros
#define		DECLARE_AVG_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, desc_str_prefix)\
DECLARE_CONV_BLOCK(convert_fn_prefix##_nonsse, 			src_fmt, dst_fmt, 		CPUID_FEATURE_NONE, 		DEFAULT_ATTRIBUTE, width_mult_count, height_mult_count, 1, desc_str_prefix " - FR - NON-SSE - avg resampling")
#define		DECLARE_AVG_BT601_CONV_BLOCK(bt601_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt601_convert_fn_prefix##_nonsse, 		src_fmt, dst_fmt, 		CPUID_FEATURE_NONE, 		BT601_CONVERSION, width_mult_count, height_mult_count, 1, desc_str_prefix " - bt.601 - NON-SSE - avg resampling")
#define		DECLARE_AVG_BT709_CONV_BLOCK(bt709_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt709_convert_fn_prefix##_nonsse, 		src_fmt, dst_fmt, 		CPUID_FEATURE_NONE, 		BT709_CONVERSION, width_mult_count, height_mult_count, 1, desc_str_prefix " - bt.709 - NON-SSE - avg resampling")

//

// NNB resampling SSE2 {FR, bt.601, bt.709) macros
#define		DECLARE_NNB_SSE2_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(convert_fn_prefix##_sse2, 				src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2, 		NNB_RESAMPLING, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - FR - SSE2 - fast resampling")
#define		DECLARE_NNB_BT601_SSE2_CONV_BLOCK(bt601_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt601_convert_fn_prefix##_sse2, 			src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2, 		BT601_CONVERSION | NNB_RESAMPLING, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - bt.601 - SSE2 - fast resampling")
#define		DECLARE_NNB_BT709_SSE2_CONV_BLOCK(bt709_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt709_convert_fn_prefix##_sse2, 			src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2, 		BT709_CONVERSION | NNB_RESAMPLING, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - bt.709 - SSE2 - fast resampling")

// AVG resampling SSE2 {FR, bt.601, bt.709) macros
#define		DECLARE_AVG_SSE2_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(convert_fn_prefix##_sse2, 				src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2, 		DEFAULT_ATTRIBUTE, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - FR - SSE2 - avg resampling")
#define		DECLARE_AVG_BT601_SSE2_CONV_BLOCK(bt601_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt601_convert_fn_prefix##_sse2, 		src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2, 		BT601_CONVERSION, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - bt.601 - SSE2 - avg resampling")
#define		DECLARE_AVG_BT709_SSE2_CONV_BLOCK(bt709_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt709_convert_fn_prefix##_sse2, 		src_fmt, dst_fmt,		CPUID_FEATURE_SSE2, 		BT709_CONVERSION, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - bt.709 - SSE2 - avg resampling")

//

// NNB resampling SSSE3 {FR, bt.601, bt.709) macros
#define		DECLARE_NNB_SSE2_SSSE3_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(convert_fn_prefix##_sse2_ssse3, 		src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3, 	NNB_RESAMPLING, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - FR - SSE2/SSSE3 - fast resampling")
#define		DECLARE_NNB_BT601_SSE2_SSSE3_CONV_BLOCK(bt601_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt601_convert_fn_prefix##_sse2_ssse3, 	src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3, 	BT601_CONVERSION | NNB_RESAMPLING, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - bt.601 - SSE2/SSSE3 - fast resampling")
#define		DECLARE_NNB_BT709_SSE2_SSSE3_CONV_BLOCK(bt709_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt709_convert_fn_prefix##_sse2_ssse3, 	src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3, 	BT709_CONVERSION | NNB_RESAMPLING, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - bt.709 - SSE2/SSSE3 - fast resampling")

// AVG resampling SSSE3 {FR, bt.601, bt.709) macros
#define		DECLARE_AVG_SSE2_SSSE3_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(convert_fn_prefix##_sse2_ssse3, 		src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3, 	DEFAULT_ATTRIBUTE, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - FR - SSE2/SSSE3 - avg resampling")
#define		DECLARE_AVG_BT601_SSE2_SSSE3_CONV_BLOCK(bt601_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt601_convert_fn_prefix##_sse2_ssse3, 	src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3, 	BT601_CONVERSION, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - bt.601 - SSE2/SSSE3 - avg resampling")
#define		DECLARE_AVG_BT709_SSE2_SSSE3_CONV_BLOCK(bt709_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt709_convert_fn_prefix##_sse2_ssse3, 	src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3, 	BT709_CONVERSION,  width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - bt.709 - SSE2/SSSE3 - avg resampling")

//

// NNB resampling SSE41 {FR, bt.601, bt.709) macros
#define		DECLARE_NNB_SSE2_SSSE3_SSE41_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(convert_fn_prefix##_sse2_ssse3_sse41, 		src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3 | CPUID_FEATURE_SSE41, 	NNB_RESAMPLING, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - Fr - SSE2/SSSE3/SSE41 - fast resampling")
#define		DECLARE_NNB_BT601_SSE2_SSSE3_SSE41_CONV_BLOCK(bt601_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt601_convert_fn_prefix##_sse2_ssse3_sse41, 	src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3 | CPUID_FEATURE_SSE41, 	BT601_CONVERSION | NNB_RESAMPLING, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - bt.601 - SSE2/SSSE3/SSE41 - fast resampling")
#define		DECLARE_NNB_BT709_SSE2_SSSE3_SSE41_CONV_BLOCK(bt709_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt709_convert_fn_prefix##_sse2_ssse3_sse41, 	src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3 | CPUID_FEATURE_SSE41, 	BT709_CONVERSION | NNB_RESAMPLING, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - bt.709 - SSE2/SSSE3/SSE41 - fast resampling")

// AVG resampling SSE41 {FR, bt.601, bt.709) macros
#define		DECLARE_AVG_SSE2_SSSE3_SSE41_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(convert_fn_prefix##_sse2_ssse3_sse41, 		src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3 | CPUID_FEATURE_SSE41, 	DEFAULT_ATTRIBUTE, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - FR - SSE2/SSSE3/SSE41 - avg resampling")
#define		DECLARE_AVG_BT601_SSE2_SSSE3_SSE41_CONV_BLOCK(bt601_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt601_convert_fn_prefix##_sse2_ssse3_sse41, 	src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3 | CPUID_FEATURE_SSE41, 	BT601_CONVERSION, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - bt.601 - SSE2/SSSE3/SSE41 - avg resampling")
#define		DECLARE_AVG_BT709_SSE2_SSSE3_SSE41_CONV_BLOCK(bt709_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(bt709_convert_fn_prefix##_sse2_ssse3_sse41, 	src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3 | CPUID_FEATURE_SSE41, 	BT709_CONVERSION,  width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - bt.709 - SSE2/SSSE3/SSE41 - avg resampling")


/*
 * The following macro declares the following conversion blocks:
 * - Fast Nearest NeighBour resampling SSE2/SSSE3 full range
 * - Fast Nearest NeighBour resampling SSE2/SSSE3 bt.601
 * - Fast Nearest NeighBour resampling SSE2/SSSE3 bt.709
 *
 * - Average resampling SSE2/SSSE3 full range
 * - Average resampling SSE2/SSSE3 bt.601
 * - Average resampling SSE2/SSSE3 bt.709
 *
 * - Fast Nearest NeighBour resampling SSE2 full range
 * - Fast Nearest NeighBour resampling SSE2 bt.601
 * - Fast Nearest NeighBour resampling SSE2 bt.709
 *
 * - Average resampling SSE2 full range
 * - Average resampling SSE2 bt.601
 * - Average resampling SSE2 bt.709
 *
 * - Fast Nearest NeighBour resampling NON-SSE full range
 * - Fast Nearest NeighBour resampling NON-SSE bt.601
 * - Fast Nearest NeighBour resampling NON-SSE bt.709
 *
 * - Average resampling NON-SSE full range
 * - Average  resampling NON-SSE bt.601
 * - Average  resampling NON-SSE bt.709
 *
 */
#define		DECLARE_CONV_BLOCKS(convert_fn_prefix, resample_n_convert_fn_prefix, non_sse_convert_fn_prefix, non_sse_resample_n_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, nonsse_width_mult_count, nonsse_height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_NNB_SSE2_SSSE3_CONV_BLOCK		(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_BT601_SSE2_SSSE3_CONV_BLOCK	(convert_fn_prefix##_bt601, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_BT709_SSE2_SSSE3_CONV_BLOCK	(convert_fn_prefix##_bt709, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_AVG_SSE2_SSSE3_CONV_BLOCK		(resample_n_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_AVG_BT601_SSE2_SSSE3_CONV_BLOCK	(resample_n_convert_fn_prefix##_bt601, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_AVG_BT709_SSE2_SSSE3_CONV_BLOCK	(resample_n_convert_fn_prefix##_bt709, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_SSE2_CONV_BLOCK				(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_BT601_SSE2_CONV_BLOCK		(convert_fn_prefix##_bt601, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_BT709_SSE2_CONV_BLOCK		(convert_fn_prefix##_bt709, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_AVG_SSE2_CONV_BLOCK				(resample_n_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_AVG_BT601_SSE2_CONV_BLOCK		(resample_n_convert_fn_prefix##_bt601, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_AVG_BT709_SSE2_CONV_BLOCK		(resample_n_convert_fn_prefix##_bt709, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_CONV_BLOCK					(non_sse_convert_fn_prefix, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix),\
DECLARE_NNB_BT601_CONV_BLOCK			(non_sse_convert_fn_prefix##_bt601, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix),\
DECLARE_NNB_BT709_CONV_BLOCK			(non_sse_convert_fn_prefix##_bt709, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix),\
DECLARE_AVG_CONV_BLOCK					(non_sse_resample_n_convert_fn_prefix, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix),\
DECLARE_AVG_BT601_CONV_BLOCK			(non_sse_resample_n_convert_fn_prefix##_bt601, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix),\
DECLARE_AVG_BT709_CONV_BLOCK			(non_sse_resample_n_convert_fn_prefix##_bt709, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix)





/*
 * The following macro declares only NNB conversion blocks:
 * - Fast Nearest NeighBour resampling SSE2/SSSE3 full range
 * - Fast Nearest NeighBour resampling SSE2/SSSE3 bt.601
 * - Fast Nearest NeighBour resampling SSE2/SSSE3 bt.709
 *
 * - Fast Nearest NeighBour resampling SSE2 full range
 * - Fast Nearest NeighBour resampling SSE2 bt.601
 * - Fast Nearest NeighBour resampling SSE2 bt.709
 *
 * - Fast Nearest NeighBour resampling NON-SSE full range
 * - Fast Nearest NeighBour resampling NON-SSE bt.601
 * - Fast Nearest NeighBour resampling NON-SSE bt.709
 */
#define		DECLARE_NNB_ONLY_CONV_BLOCKS(convert_fn_prefix, non_sse_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, nonsse_width_mult_count, nonsse_height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_NNB_SSE2_SSSE3_CONV_BLOCK		(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_BT601_SSE2_SSSE3_CONV_BLOCK	(convert_fn_prefix##_bt601, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_BT709_SSE2_SSSE3_CONV_BLOCK	(convert_fn_prefix##_bt709, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_SSE2_CONV_BLOCK				(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_BT601_SSE2_CONV_BLOCK		(convert_fn_prefix##_bt601, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_BT709_SSE2_CONV_BLOCK		(convert_fn_prefix##_bt709, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_CONV_BLOCK					(non_sse_convert_fn_prefix, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix),\
DECLARE_NNB_BT601_CONV_BLOCK			(non_sse_convert_fn_prefix##_bt601, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix),\
DECLARE_NNB_BT709_CONV_BLOCK			(non_sse_convert_fn_prefix##_bt709, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix)






/*
 * Repacking conversion blocks
 */
#define		DECLARE_REPACK_SSE2_SSSE3_SSE41_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(convert_fn_prefix##_sse2_ssse3_sse41,src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3 | CPUID_FEATURE_SSE41, 	DEFAULT_ATTRIBUTE, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - FR - SSE2/SSSE3/SSE41 - no resampling")
#define		DECLARE_REPACK_SSE2_SSSE3_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(convert_fn_prefix##_sse2_ssse3, 		src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2 | CPUID_FEATURE_SSSE3, 	DEFAULT_ATTRIBUTE, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - FR - SSE2/SSSE3 - no resampling")
#define		DECLARE_REPACK_SSE2_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_CONV_BLOCK(convert_fn_prefix##_sse2, 		src_fmt, dst_fmt, 		CPUID_FEATURE_SSE2, 	DEFAULT_ATTRIBUTE, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix " - FR - SSE2 - no resampling")
#define		DECLARE_REPACK_NONSSE_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix)\
DECLARE_CONV_BLOCK(convert_fn_prefix##_nonsse, 		src_fmt, dst_fmt, 		CPUID_FEATURE_NONE, 	DEFAULT_ATTRIBUTE, nonsse_width_mult_count, nonsse_height_mult_count, 1, desc_str_prefix " - FR - NON-SSE - no resampling")

// The following macro defines non-sse, sse2 and ssse3 repacking conversion blocks
#define		DECLARE_REPACK_CONV_BLOCK(convert_fn_prefix, non_sse_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, nonsse_width_mult_count, nonsse_height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_REPACK_SSE2_SSSE3_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_REPACK_SSE2_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_REPACK_NONSSE_CONV_BLOCK(non_sse_convert_fn_prefix, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix)

// The following macro defines non-sse, ssse3 and sse41 repacking conversion blocks
#define		DECLARE_V210_REPACK_CONV_BLOCK(convert_fn_prefix, non_sse_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, nonsse_width_mult_count, nonsse_height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_REPACK_SSE2_SSSE3_SSE41_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_REPACK_SSE2_SSSE3_CONV_BLOCK(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_REPACK_NONSSE_CONV_BLOCK(non_sse_convert_fn_prefix, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix)




/*
 * V210 conversion blocks (no SSE2 only conversions)
 *
 * The following macro declares the following conversion blocks:
 * - Fast Nearest NeighBour resampling SSE2/SSSE3/SSE41 full range
 * - Fast Nearest NeighBour resampling SSE2/SSSE3/SSE41 bt.601
 * - Fast Nearest NeighBour resampling SSE2/SSSE3/SSE41 bt.709
 *
 * - Average resampling SSE2/SSSE3/SSE41 full range
 * - Average resampling SSE2/SSSE3/SSE41 bt.601
 * - Average resampling SSE2/SSSE3/SSE41 bt.709
 *
 * - Fast Nearest NeighBour resampling SSE2/SSSE3 full range
 * - Fast Nearest NeighBour resampling SSE2/SSSE3 bt.601
 * - Fast Nearest NeighBour resampling SSE2/SSSE3 bt.709
 *
 * - Average resampling SSE2/SSSE3 full range
 * - Average resampling SSE2/SSSE3 bt.601
 * - Average resampling SSE2/SSSE3 bt.709
 *
 * - Fast Nearest NeighBour resampling NON-SSE full range
 * - Fast Nearest NeighBour resampling NON-SSE bt.601
 * - Fast Nearest NeighBour resampling NON-SSE bt.709
 *
 * - Average resampling NON-SSE full range
 * - Average resampling NON-SSE bt.601
 * - Average resampling NON-SSE bt.709
 */
#define		DECLARE_V210_CONV_BLOCKS(convert_fn_prefix, resample_n_convert_fn_prefix, non_sse_convert_fn_prefix, non_sse_resample_n_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, nonsse_width_mult_count, nonsse_height_mult_count, row_pix_mult, desc_str_prefix)\
DECLARE_NNB_SSE2_SSSE3_SSE41_CONV_BLOCK		(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_BT601_SSE2_SSSE3_SSE41_CONV_BLOCK	(convert_fn_prefix##_bt601, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_BT709_SSE2_SSSE3_SSE41_CONV_BLOCK	(convert_fn_prefix##_bt709, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_AVG_SSE2_SSSE3_SSE41_CONV_BLOCK		(resample_n_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_AVG_BT601_SSE2_SSSE3_SSE41_CONV_BLOCK	(resample_n_convert_fn_prefix##_bt601, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_AVG_BT709_SSE2_SSSE3_SSE41_CONV_BLOCK	(resample_n_convert_fn_prefix##_bt709, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_SSE2_SSSE3_CONV_BLOCK				(convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_BT601_SSE2_SSSE3_CONV_BLOCK		(convert_fn_prefix##_bt601, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_BT709_SSE2_SSSE3_CONV_BLOCK		(convert_fn_prefix##_bt709, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_AVG_SSE2_SSSE3_CONV_BLOCK				(resample_n_convert_fn_prefix, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_AVG_BT601_SSE2_SSSE3_CONV_BLOCK		(resample_n_convert_fn_prefix##_bt601, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_AVG_BT709_SSE2_SSSE3_CONV_BLOCK		(resample_n_convert_fn_prefix##_bt709, src_fmt, dst_fmt, width_mult_count, height_mult_count, row_pix_mult, desc_str_prefix),\
DECLARE_NNB_CONV_BLOCK					(non_sse_convert_fn_prefix, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix),\
DECLARE_NNB_BT601_CONV_BLOCK			(non_sse_convert_fn_prefix##_bt601, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix),\
DECLARE_NNB_BT709_CONV_BLOCK			(non_sse_convert_fn_prefix##_bt709, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix),\
DECLARE_AVG_CONV_BLOCK					(non_sse_resample_n_convert_fn_prefix, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix),\
DECLARE_AVG_BT601_CONV_BLOCK			(non_sse_resample_n_convert_fn_prefix##_bt601, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix),\
DECLARE_AVG_BT709_CONV_BLOCK			(non_sse_resample_n_convert_fn_prefix##_bt709, src_fmt, dst_fmt, nonsse_width_mult_count, nonsse_height_mult_count, desc_str_prefix)



/*
 * In this array, conversion blocks for a given pair of source and destination
 * formats must be sorted: fastest first, slowest last !!!
 */
const struct  ConversionBlock		conversion_blocks[] = {

	//
	// ARGB to YUYV
	DECLARE_CONV_BLOCKS(convert_argb_to_yuyv, downsample_n_convert_argb_to_yuyv, convert_rgb_to_yuv422, downsample_n_convert_rgb_to_yuv422, PixFcARGB, PixFcYUYV, 16, 1, 2, 1, 1, "ARGB to YUYV"),

	// ARGB to UYVY
	DECLARE_CONV_BLOCKS(convert_argb_to_uyvy, downsample_n_convert_argb_to_uyvy, convert_rgb_to_yuv422, downsample_n_convert_rgb_to_yuv422, PixFcARGB, PixFcUYVY, 16, 1, 2, 1, 1, "ARGB to UYVY"),

	// ARGB to YUV422P
	DECLARE_CONV_BLOCKS(convert_argb_to_yuv422p, downsample_n_convert_argb_to_yuv422p, convert_rgb_to_yuv422, downsample_n_convert_rgb_to_yuv422, PixFcARGB, PixFcYUV422P, 32, 1, 2, 1, 1, "ARGB to YUV422P"),

	// ARGB to YUV420P (NNB only for now)
	DECLARE_NNB_ONLY_CONV_BLOCKS(convert_argb_to_yuv420p, convert_rgb_to_yuv420, PixFcARGB, PixFcYUV420P, 32, 2, 2, 2, 1, "ARGB to YUV420P"),

	// ARGB to v210
	DECLARE_V210_CONV_BLOCKS(convert_argb_to_v210, downsample_n_convert_argb_to_v210, convert_rgb_to_v210, downsample_n_convert_rgb_to_v210, PixFcARGB, PixFcV210, 16, 1, 2, 1, 48, "ARGB to v210"),


	//
	// BGRA to YUYV
	DECLARE_CONV_BLOCKS(convert_bgra_to_yuyv, downsample_n_convert_bgra_to_yuyv, convert_rgb_to_yuv422, downsample_n_convert_rgb_to_yuv422, PixFcBGRA, PixFcYUYV, 16, 1, 2, 1, 1, "BGRA to YUYV"),

	// BGRA to UYVYconvert_fn_prefix
	DECLARE_CONV_BLOCKS(convert_bgra_to_uyvy, downsample_n_convert_bgra_to_uyvy, convert_rgb_to_yuv422, downsample_n_convert_rgb_to_yuv422, PixFcBGRA, PixFcUYVY, 16, 1, 2, 1, 1, "BGRA to UYVY"),

	// BGRA to YUV422P
	DECLARE_CONV_BLOCKS(convert_bgra_to_yuv422p, downsample_n_convert_bgra_to_yuv422p, convert_rgb_to_yuv422, downsample_n_convert_rgb_to_yuv422, PixFcBGRA, PixFcYUV422P, 32, 1, 2, 1, 1, "BGRA to YUV422P"),

	// BGRA to YUV420P (NNB only for now)
	DECLARE_NNB_ONLY_CONV_BLOCKS(convert_bgra_to_yuv420p, convert_rgb_to_yuv420, PixFcBGRA, PixFcYUV420P, 32, 2, 2, 2, 1, "BGRA to YUV420P"),

	// BGRA to v210
	DECLARE_V210_CONV_BLOCKS(convert_bgra_to_v210, downsample_n_convert_bgra_to_v210, convert_rgb_to_v210, downsample_n_convert_rgb_to_v210, PixFcBGRA, PixFcV210, 16, 1, 2, 1, 48, "BGRA to v210"),


	//
	// RGB24 to YUYV
	DECLARE_CONV_BLOCKS(convert_rgb24_to_yuyv, downsample_n_convert_rgb24_to_yuyv, convert_rgb_to_yuv422, downsample_n_convert_rgb_to_yuv422, PixFcRGB24, PixFcYUYV, 16, 1, 2, 1, 1, "RGB24 to YUYV"),

	// RGB24 to UYVY
	DECLARE_CONV_BLOCKS(convert_rgb24_to_uyvy, downsample_n_convert_rgb24_to_uyvy, convert_rgb_to_yuv422, downsample_n_convert_rgb_to_yuv422, PixFcRGB24, PixFcUYVY, 16, 1, 2, 1, 1, "RGB24 to UYVY"),

	// RGB24 to YUV422P
	DECLARE_CONV_BLOCKS(convert_rgb24_to_yuv422p, downsample_n_convert_rgb24_to_yuv422p, convert_rgb_to_yuv422, downsample_n_convert_rgb_to_yuv422, PixFcRGB24, PixFcYUV422P, 32, 1, 2, 1, 1, "RGB24 to YUV422P"),

	// RGB24 to YUV420P (NNB only for now)
	DECLARE_NNB_ONLY_CONV_BLOCKS(convert_rgb24_to_yuv420p, convert_rgb_to_yuv420, PixFcRGB24, PixFcYUV420P, 32, 2, 2, 2, 1, "RGB24 to YUV420P"),

	// RGB24 to v210
	DECLARE_V210_CONV_BLOCKS(convert_rgb24_to_v210, downsample_n_convert_rgb24_to_v210, convert_rgb_to_v210, downsample_n_convert_rgb_to_v210, PixFcRGB24, PixFcV210, 16, 1, 2, 1, 48, "RGB24 to v210"),


	//
	// BGR24 to YUYV
	DECLARE_CONV_BLOCKS(convert_bgr24_to_yuyv, downsample_n_convert_bgr24_to_yuyv, convert_rgb_to_yuv422, downsample_n_convert_rgb_to_yuv422, PixFcBGR24, PixFcYUYV, 16, 1, 2, 1, 1, "BGR24 to YUYV"),

	// BGR24 to UYVY
	DECLARE_CONV_BLOCKS(convert_bgr24_to_uyvy, downsample_n_convert_bgr24_to_uyvy, convert_rgb_to_yuv422, downsample_n_convert_rgb_to_yuv422, PixFcBGR24, PixFcUYVY, 16, 1, 2, 1, 1, "BGR24 to UYVY"),

	// BGR24 to YUV422P
	DECLARE_CONV_BLOCKS(convert_bgr24_to_yuv422p, downsample_n_convert_bgr24_to_yuv422p, convert_rgb_to_yuv422, downsample_n_convert_rgb_to_yuv422, PixFcBGR24, PixFcYUV422P, 32, 1, 2, 1, 1, "BGR24 to YUV422P"),

	// BGR24 to YUV420P (NNB only for now)
	DECLARE_NNB_ONLY_CONV_BLOCKS(convert_bgr24_to_yuv420p, convert_rgb_to_yuv420, PixFcBGR24, PixFcYUV420P, 32, 2, 2, 2, 1, "BGR24 to YUV420P"),

	// BGR24 to v210
	DECLARE_V210_CONV_BLOCKS(convert_bgr24_to_v210, downsample_n_convert_bgr24_to_v210, convert_rgb_to_v210, downsample_n_convert_rgb_to_v210, PixFcBGR24, PixFcV210, 16, 1, 2, 1, 48, "BGR24 to v210"),


	//
	// YUYV to ARGB
	DECLARE_CONV_BLOCKS(convert_yuyv_to_argb, upsample_n_convert_yuyv_to_argb, convert_yuv422i_to_any_rgb, upsample_n_convert_yuv422i_to_any_rgb, PixFcYUYV, PixFcARGB, 16, 1, 2, 1, 1, "YUYV to ARGB"),

	// YUYV to BGRA
	DECLARE_CONV_BLOCKS(convert_yuyv_to_bgra, upsample_n_convert_yuyv_to_bgra, convert_yuv422i_to_any_rgb, upsample_n_convert_yuv422i_to_any_rgb, PixFcYUYV, PixFcBGRA, 16, 1, 2, 1, 1, "YUYV to BGRA"),

	// YUYV to RGB24
	DECLARE_CONV_BLOCKS(convert_yuyv_to_rgb24, upsample_n_convert_yuyv_to_rgb24, convert_yuv422i_to_any_rgb, upsample_n_convert_yuv422i_to_any_rgb, PixFcYUYV, PixFcRGB24, 16, 1, 2, 1, 1, "YUYV to RGB24"),

	// YUYV to BGR24
	DECLARE_CONV_BLOCKS(convert_yuyv_to_bgr24, upsample_n_convert_yuyv_to_bgr24, convert_yuv422i_to_any_rgb, upsample_n_convert_yuv422i_to_any_rgb, PixFcYUYV, PixFcBGR24, 16, 1, 2, 1, 1, "YUYV to BGR24"),

	// YUYV to YUV422P
	DECLARE_REPACK_CONV_BLOCK(convert_yuyv_to_yuv422p, convert_yuv422i_to_yuv422p, PixFcYUYV, PixFcYUV422P, 32, 1, 2, 1, 1, "YUYV to YUV422P"),

	// YUYV to V210
	DECLARE_V210_REPACK_CONV_BLOCK(convert_yuyv_to_v210, convert_yuv422i_to_v210, PixFcYUYV, PixFcV210, 16, 1, 2, 1, 48, "YUYV to v210"),


	//
	// UYVY to ARGB
	DECLARE_CONV_BLOCKS(convert_uyvy_to_argb, upsample_n_convert_uyvy_to_argb, convert_yuv422i_to_any_rgb, upsample_n_convert_yuv422i_to_any_rgb, PixFcUYVY, PixFcARGB, 16, 1, 2, 1, 1, "UYVY to ARGB"),

	// UYVY to BGRA
	DECLARE_CONV_BLOCKS(convert_uyvy_to_bgra, upsample_n_convert_uyvy_to_bgra, convert_yuv422i_to_any_rgb, upsample_n_convert_yuv422i_to_any_rgb, PixFcUYVY, PixFcBGRA, 16, 1, 2, 1, 1, "UYVY to BGRA"),

	// UYVY to RGB24
	DECLARE_CONV_BLOCKS(convert_uyvy_to_rgb24, upsample_n_convert_uyvy_to_rgb24, convert_yuv422i_to_any_rgb, upsample_n_convert_yuv422i_to_any_rgb, PixFcUYVY, PixFcRGB24, 16, 1, 2, 1, 1, "UYVY to RGB24"),

	// UYVY to BGR24
	DECLARE_CONV_BLOCKS(convert_uyvy_to_bgr24, upsample_n_convert_uyvy_to_bgr24, convert_yuv422i_to_any_rgb, upsample_n_convert_yuv422i_to_any_rgb, PixFcUYVY, PixFcBGR24, 16, 1, 2, 1, 1, "UYVY to BGR24"),

	// UYVY to YUV422P
	DECLARE_REPACK_CONV_BLOCK(convert_uyvy_to_yuv422p, convert_yuv422i_to_yuv422p, PixFcUYVY, PixFcYUV422P, 32, 1, 2, 1, 1, "UYVY to YUV422P"),

	// YUYV to V210
	DECLARE_V210_REPACK_CONV_BLOCK(convert_uyvy_to_v210, convert_yuv422i_to_v210, PixFcUYVY, PixFcV210, 16, 1, 2, 1, 48, "UYVY to v210"),


	//
	// YUV422P to ARGB
	DECLARE_CONV_BLOCKS(convert_yuv422p_to_argb, upsample_n_convert_yuv422p_to_argb, convert_yuv422p_to_any_rgb, upsample_n_convert_yuv422p_to_any_rgb, PixFcYUV422P, PixFcARGB, 32, 1, 1, 1, 1, "YUV422P to ARGB"),

	// YUV422P to BGRA
	DECLARE_CONV_BLOCKS(convert_yuv422p_to_bgra, upsample_n_convert_yuv422p_to_bgra, convert_yuv422p_to_any_rgb, upsample_n_convert_yuv422p_to_any_rgb, PixFcYUV422P, PixFcBGRA, 32, 1, 1, 1, 1, "YUV422P to BGRA"),

	// YUV422P to RGB24
	DECLARE_CONV_BLOCKS(convert_yuv422p_to_rgb24, upsample_n_convert_yuv422p_to_rgb24, convert_yuv422p_to_any_rgb, upsample_n_convert_yuv422p_to_any_rgb, PixFcYUV422P, PixFcRGB24, 32, 1, 1, 1, 1, "YUV422P to RGB24"),

	// YUV422P to BGR24
	DECLARE_CONV_BLOCKS(convert_yuv422p_to_bgr24, upsample_n_convert_yuv422p_to_bgr24, convert_yuv422p_to_any_rgb, upsample_n_convert_yuv422p_to_any_rgb, PixFcYUV422P, PixFcBGR24, 32, 1, 1, 1, 1, "YUV422P to BGR24"),

	// YUV422P to YUYV
	DECLARE_REPACK_SSE2_CONV_BLOCK(convert_yuv422p_to_yuyv, PixFcYUV422P, PixFcYUYV, 32, 1, 1, "YUV422P to YUYV"),
	DECLARE_REPACK_NONSSE_CONV_BLOCK(convert_yuv422p_to_yuyv, PixFcYUV422P, PixFcYUYV, 2, 1, "YUV422P to YUYV"),

	// YUV422P to UYVY
	DECLARE_REPACK_SSE2_CONV_BLOCK(convert_yuv422p_to_uyvy, PixFcYUV422P, PixFcUYVY, 32, 1, 1, "YUV422P to UYVY"),
	DECLARE_REPACK_NONSSE_CONV_BLOCK(convert_yuv422p_to_uyvy, PixFcYUV422P, PixFcUYVY, 2, 1, "YUV422P to UYVY"),

	
	//
	// YUV420P to ARGB
	DECLARE_NNB_ONLY_CONV_BLOCKS(convert_yuv420p_to_argb, convert_yuv420p_to_any_rgb, PixFcYUV420P, PixFcARGB, 32, 2, 1, 2, 1, "YUV420P to ARGB"),
	
	// YUV420P to BGRA
	DECLARE_NNB_ONLY_CONV_BLOCKS(convert_yuv420p_to_bgra, convert_yuv420p_to_any_rgb, PixFcYUV420P, PixFcBGRA, 32, 2, 1, 2, 1, "YUV420P to BGRA"),
	
	// YUV420P to RGB24
	DECLARE_NNB_ONLY_CONV_BLOCKS(convert_yuv420p_to_rgb24, convert_yuv420p_to_any_rgb, PixFcYUV420P, PixFcRGB24, 32, 2, 1, 2, 1, "YUV420P to RGB24"),

	// YUV420P to BGR24
	DECLARE_NNB_ONLY_CONV_BLOCKS(convert_yuv420p_to_bgr24, convert_yuv420p_to_any_rgb, PixFcYUV420P, PixFcBGR24, 32, 2, 1, 2, 1, "YUV420P to BGR24"),


	//
	// v210 to ARGB
	DECLARE_V210_CONV_BLOCKS(convert_v210_to_argb, upsample_n_convert_v210_to_argb, convert_v210_to_any_rgb, upsample_n_convert_v210_to_any_rgb, PixFcV210, PixFcARGB, 16, 1, 2, 1, 48, "v210 to ARGB"),

	// v210 to BGRA
	DECLARE_V210_CONV_BLOCKS(convert_v210_to_bgra, upsample_n_convert_v210_to_bgra, convert_v210_to_any_rgb, upsample_n_convert_v210_to_any_rgb, PixFcV210, PixFcBGRA, 16, 1, 2, 1, 48, "v210 to BGRA"),

	// v210 to RGB24
	DECLARE_V210_CONV_BLOCKS(convert_v210_to_rgb24, upsample_n_convert_v210_to_rgb24, convert_v210_to_any_rgb, upsample_n_convert_v210_to_any_rgb, PixFcV210, PixFcRGB24, 16, 1, 2, 1, 48, "v210 to RGB24"),

	// v210 to BGR24
	DECLARE_V210_CONV_BLOCKS(convert_v210_to_bgr24, upsample_n_convert_v210_to_bgr24, convert_v210_to_any_rgb, upsample_n_convert_v210_to_any_rgb, PixFcV210, PixFcBGR24, 16, 1, 2, 1, 48, "v210 to BGR24"),

	// v210 to YUYV
	DECLARE_V210_REPACK_CONV_BLOCK(convert_v210_to_yuyv, convert_v210_to_yuv422i, PixFcV210, PixFcYUYV, 16, 1, 2, 1, 48, "v210 to YUYV"),

	// v210 to UYVY
	DECLARE_V210_REPACK_CONV_BLOCK(convert_v210_to_uyvy, convert_v210_to_yuv422i, PixFcV210, PixFcUYVY, 16, 1, 2, 1, 48, "v210 to UYVY"),
};


const uint32_t		conversion_blocks_count = sizeof(conversion_blocks) / sizeof(conversion_blocks[0]);


