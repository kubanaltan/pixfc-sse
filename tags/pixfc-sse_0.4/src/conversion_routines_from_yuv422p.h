/*
 * conversion_routines_from_yuv422.h
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

#ifndef CONVERSION_ROUTINES_FROM_YUV422_H_
#define CONVERSION_ROUTINES_FROM_YUV422_H_

#include "pixfc-sse.h"

/*
 *
 *
 *	F U L L   R A N G E
 *
 *  C O N V E R S I O N S
 *
 *
 */

// YUV422P to ARGB			SSE2 SSS3
void		upsample_n_convert_yuv422p_to_argb_sse2_ssse3(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_argb_sse2_ssse3(const struct PixFcSSE *, void *, void *);
// YUV422P to ARGB  		SSE2
void		upsample_n_convert_yuv422p_to_argb_sse2(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_argb_sse2(const struct PixFcSSE *, void *, void *);


// YUV422P to BGRA 		SSE2 SSSE3
void		upsample_n_convert_yuv422p_to_bgra_sse2_ssse3(const struct PixFcSSE *, void* , void*);
void		convert_yuv422p_to_bgra_sse2_ssse3(const struct PixFcSSE *, void *, void *);
// YUV422P to BGRA 		SSE2
void		upsample_n_convert_yuv422p_to_bgra_sse2(const struct PixFcSSE *, void*, void*);
void		convert_yuv422p_to_bgra_sse2(const struct PixFcSSE *, void *, void *);


// YUV422P to RGB24		SSE2 SSSE3
void		upsample_n_convert_yuv422p_to_rgb24_sse2_ssse3(const struct PixFcSSE *, void*, void*);
void		convert_yuv422p_to_rgb24_sse2_ssse3(const struct PixFcSSE *, void *, void *);
// YUV422P to RGB24		SSE2 slow repacking
void		upsample_n_convert_yuv422p_to_rgb24_sse2(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_rgb24_sse2(const struct PixFcSSE *, void *, void *);


// YUV422P to BGR24		SSE2 SSSE3
void		upsample_n_convert_yuv422p_to_bgr24_sse2_ssse3(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_bgr24_sse2_ssse3(const struct PixFcSSE *, void*, void*);
// YUV422P to BGR24		SSE2 slow repacking
void		upsample_n_convert_yuv422p_to_bgr24_sse2(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_bgr24_sse2(const struct PixFcSSE *, void *, void *);


// YUV422P to anyRGB		NON SSE
void 		convert_yuv422p_to_any_rgb_nonsse(const struct PixFcSSE*, void*, void*);
void 		upsample_n_convert_yuv422p_to_any_rgb_nonsse(const struct PixFcSSE*, void*, void*);






/*
 *
 *
 *	S T A N D A R D   D E F I N I T I O N
 *
 *  C O N V E R S I O N S
 *
 *  ( BT 601 )
 *
 *
 */

// YUV422P to ARGB			SSE2 SSS3
void		upsample_n_convert_yuv422p_to_argb_bt601_sse2_ssse3(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_argb_bt601_sse2_ssse3(const struct PixFcSSE *, void *, void *);
// YUV422P to ARGB  		SSE2
void		upsample_n_convert_yuv422p_to_argb_bt601_sse2(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_argb_bt601_sse2(const struct PixFcSSE *, void *, void *);


// YUV422P to BGRA 		SSE2 SSSE3
void		upsample_n_convert_yuv422p_to_bgra_bt601_sse2_ssse3(const struct PixFcSSE *, void* , void*);
void		convert_yuv422p_to_bgra_bt601_sse2_ssse3(const struct PixFcSSE *, void *, void *);
// YUV422P to BGRA 		SSE2
void		upsample_n_convert_yuv422p_to_bgra_bt601_sse2(const struct PixFcSSE *, void*, void*);
void		convert_yuv422p_to_bgra_bt601_sse2(const struct PixFcSSE *, void *, void *);


// YUV422P to RGB24		SSE2 SSSE3
void		upsample_n_convert_yuv422p_to_rgb24_bt601_sse2_ssse3(const struct PixFcSSE *, void*, void*);
void		convert_yuv422p_to_rgb24_bt601_sse2_ssse3(const struct PixFcSSE *, void *, void *);
// YUV422P to RGB24		SSE2 slow repacking
void		upsample_n_convert_yuv422p_to_rgb24_bt601_sse2(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_rgb24_bt601_sse2(const struct PixFcSSE *, void *, void *);


// YUV422P to BGR24		SSE2 SSSE3
void		upsample_n_convert_yuv422p_to_bgr24_bt601_sse2_ssse3(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_bgr24_bt601_sse2_ssse3(const struct PixFcSSE *, void*, void*);
// YUV422P to BGR24		SSE2 slow repacking
void		upsample_n_convert_yuv422p_to_bgr24_bt601_sse2(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_bgr24_bt601_sse2(const struct PixFcSSE *, void *, void *);


// YUV422P to anyRGB		NON SSE
void 		convert_yuv422p_to_any_rgb_bt601_nonsse(const struct PixFcSSE*, void*, void*);
void 		upsample_n_convert_yuv422p_to_any_rgb_bt601_nonsse(const struct PixFcSSE*, void*, void*);




/*
 *
 *
 *	H I G H   D E F I N I T I O N
 *
 *  C O N V E R S I O N S
 *
 *  ( BT 709 )
 *
 *
 */

// YUV422P to ARGB			SSE2 SSS3
void		upsample_n_convert_yuv422p_to_argb_bt709_sse2_ssse3(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_argb_bt709_sse2_ssse3(const struct PixFcSSE *, void *, void *);
// YUV422P to ARGB  		SSE2
void		upsample_n_convert_yuv422p_to_argb_bt709_sse2(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_argb_bt709_sse2(const struct PixFcSSE *, void *, void *);


// YUV422P to BGRA 		SSE2 SSSE3
void		upsample_n_convert_yuv422p_to_bgra_bt709_sse2_ssse3(const struct PixFcSSE *, void* , void*);
void		convert_yuv422p_to_bgra_bt709_sse2_ssse3(const struct PixFcSSE *, void *, void *);
// YUV422P to BGRA 		SSE2
void		upsample_n_convert_yuv422p_to_bgra_bt709_sse2(const struct PixFcSSE *, void*, void*);
void		convert_yuv422p_to_bgra_bt709_sse2(const struct PixFcSSE *, void *, void *);


// YUV422P to RGB24		SSE2 SSSE3
void		upsample_n_convert_yuv422p_to_rgb24_bt709_sse2_ssse3(const struct PixFcSSE *, void*, void*);
void		convert_yuv422p_to_rgb24_bt709_sse2_ssse3(const struct PixFcSSE *, void *, void *);
// YUV422P to RGB24		SSE2 slow repacking
void		upsample_n_convert_yuv422p_to_rgb24_bt709_sse2(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_rgb24_bt709_sse2(const struct PixFcSSE *, void *, void *);


// YUV422P to BGR24		SSE2 SSSE3
void		upsample_n_convert_yuv422p_to_bgr24_bt709_sse2_ssse3(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_bgr24_bt709_sse2_ssse3(const struct PixFcSSE *, void*, void*);
// YUV422P to BGR24		SSE2 slow repacking
void		upsample_n_convert_yuv422p_to_bgr24_bt709_sse2(const struct PixFcSSE *, void *, void *);
void		convert_yuv422p_to_bgr24_bt709_sse2(const struct PixFcSSE *, void *, void *);


// YUV422P to anyRGB		NON SSE
void 		convert_yuv422p_to_any_rgb_bt709_nonsse(const struct PixFcSSE*, void*, void*);
void 		upsample_n_convert_yuv422p_to_any_rgb_bt709_nonsse(const struct PixFcSSE*, void*, void*);


/*
 * 		Y U V 4 2 2 P
 *
 * 		T O
 *
 * 		Y U Y V
 *
 */
void		convert_yuv422p_to_yuyv_sse2(const struct PixFcSSE*, void*, void*);
void		convert_yuv422p_to_yuyv_nonsse(const struct PixFcSSE*, void*, void*);


/*
 * 		Y U V 4 2 2 P
 *
 * 		T O
 *
 * 		U Y V Y
 *
 */
void		convert_yuv422p_to_uyvy_sse2(const struct PixFcSSE*, void*, void*);
void		convert_yuv422p_to_uyvy_nonsse(const struct PixFcSSE*, void*, void*);

#endif /* CONVERSION_ROUTINES_FROM_YUV422_H_ */
