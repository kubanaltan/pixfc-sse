/*
 * rgb_image.h
 *
 * This copyright notice applies to all parts of this file, with the exception
 * of the contents of the 'header_data' array, to which the next copyright
 * notice applies. 
 * 
 * Copyright (C) 2011 PixFC Team (pixelfc@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public  License as published by the
 * Free Software Foundation, either version 3 of the License, or
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

/*
 * The C array 'header_data' contains a test image as RGB values.
 * OneRiverMedia (http://codecs.onerivermedia.com) holds the copyright for 
 * this test image and credits go to them for it. Big thanks to Marco Solorio
 * for allowing us to re-use it in PixFC. (permission granted by email on
 * 11th June 2011).
 *
 *
 * Copyright notice applicable to this image, retrieved on 11th June 2011 from
 * http://codecs.onerivermedia.com/copyright :
 *
 * 
 * Any media, including text, JPEG, GIF, GIF89, PNG, SuperPNG, QuickTime video,
 * MP3 audio, Flash, SWF and any other media (listed or not) that appears on 
 * www.onerivermedia.com (this includes any subdomains like 
 * codecs.onerivermedia.com) may NOT be used without written and signed
 * permission by OneRiver Media. OneRiver Media fully supports copyright 
 * infringement laws.
 *
 * All the media on onerivermedia.com are the sole and exclusive property of 
 * OneRiver Media. All media, including text, images and video are copyright
 * to OneRiver Media. All rights reserved.
 *
 * Whether in part or in whole, the contents of this website, including all 
 * copyrighted text, images or videos may NOT be reproduced, or transmitted, 
 * or stored in a retrieval system, in any form or by any means, electronic,
 * mechanical, photocopied, photographed, reprint, recorded or otherwise, 
 * without the prior written consent of OneRiver Media.
 *
 * These copyright laws protect all written articles, image 
 * comparisons/examples, website content, streaming/downloadable video/audio 
 * and any other media offered off onerivermedia.com.
 *
 */

#ifndef RGB_IMAGE_H_
#define RGB_IMAGE_H_

/*
 *  Fill provided ARGB buffer with RGB image in header file generated by gimp
 */
uint32_t	fill_argb_image_with_rgb_buffer(PixFcPixelFormat fmt, uint32_t width, uint32_t height, void * buf);

#endif /* RGB_IMAGE_H_ */
