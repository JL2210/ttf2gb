/*
 * Copyright © 2020 James Larrowe
 *
 * This file is part of ttf2gb.
 *
 * ttf2gb is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ttf2gb is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ttf2gb.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "ttf2gb.h"

#include <ft2build.h>
#include FT_FREETYPE_H /* FT_FaceRec, FT_GlyphSlotRec */
#include FT_IMAGE_H /* FT_BBox, FT_Bitmap, FT_UShort, FT_Int */

/* assumes horizontal font */

void draw_tile(const FT_FaceRec *face)
{
	FT_Int i, j;
	FT_BBox box = face->bbox;
	const FT_GlyphSlotRec *glyph = face->glyph;
	const FT_Bitmap *bitmap = &glyph->bitmap;
	const FT_UShort hEM = face->units_per_EM / bitmap->width;
	const FT_UShort vEM = face->units_per_EM / bitmap->rows;

	box.xMin /= hEM;
	box.xMax /= hEM;
	box.yMin /= vEM;
	box.yMax /= vEM;

#ifndef NDEBUG
	fprintf(stderr, "rows:%u, width:%u\n", bitmap->rows, bitmap->width);
	fprintf(stderr, "top:%u, left:%u\n", glyph->bitmap_top, glyph->bitmap_left);
	fprintf(stderr, "horizontal units per EM:%hu, vertical:%hu\n", hEM, vEM);
	fprintf(stderr, "xmin:%ld, xmax:%ld\n", box.xMin, box.xMax);
	fprintf(stderr, "ymin:%ld, ymax:%ld\n", box.yMin, box.yMax);
#endif

	for (i = 0; i < box.yMax - glyph->bitmap_top; i++) {
		fputs("\tDW `", stdout);
		for (j = box.xMin; j < box.xMax; j++)
			putchar('0');
		putchar('\n');
	}
	for (i = 0; i < (FT_Int)bitmap->rows; i++) {
		fputs("\tDW `", stdout);
		for (j = box.xMin; j < glyph->bitmap_left; j++)
			putchar('0');
		for (j = 0; j < (FT_Int)bitmap->width; j++)
			putchar(bitmap->buffer[i * bitmap->width + j] ? '3' : '0');
		for (j = box.xMin + glyph->bitmap_left + bitmap->width; j < box.xMax; j++)
			putchar('0');
		putchar('\n');
	}
	for (i = box.yMin; i < glyph->bitmap_top - (FT_Int)bitmap->rows; i++) {
		fputs("\tDW `", stdout);
		for (j = box.xMin; j < box.xMax; j++)
			putchar('0');
		putchar('\n');
	}

	putchar('\n');
}
