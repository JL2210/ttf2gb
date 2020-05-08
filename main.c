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

#include <ft2build.h>
#include FT_FREETYPE_H

#ifndef NDEBUG
#include <ctype.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#define TILE_ROWS 8
#define TILE_WIDTH 8

/* assumes horizontal font */

static const char *ftstrerror(FT_Error error)
{
#undef FTERRORS_H_
#define FT_ERRORDEF(error_code, value, string) case error_code: return string;
#define FT_ERROR_START_LIST switch(error) {
#define FT_ERROR_END_LIST default: return "Unknown error"; }
#include FT_ERRORS_H
}

static void ftperror(const char *message, FT_Error error)
{
    if(message)
        fprintf(stderr, "%s: %s\n", message, ftstrerror(error));
    else
        fprintf(stderr, "%s\n", ftstrerror(error));
}

void draw_tile(FT_Face face)
{
	FT_Int i, j;
	FT_GlyphSlot glyph = face->glyph;
	FT_BBox box = face->bbox;
	FT_Bitmap *bitmap = &glyph->bitmap;
	const FT_UShort hEM = face->units_per_EM / TILE_WIDTH;
	const FT_UShort vEM = face->units_per_EM / TILE_ROWS;

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

int main(int argc, char **argv)
{
	const int args_expected = 2;
	FT_Library library;
	FT_Error error;
	FT_Face face;
	int i;

	if (argc != args_expected) {
		fprintf(stderr, "Expected %d argument(s), got %d\n"
				"  Usage: ttf2gb file\n",
			args_expected - 1, argc - 1);
		return EXIT_FAILURE;
	}

	error = FT_Init_FreeType(&library);
	if (error) {
		ftperror("FT_Init_Library", error);
		return EXIT_FAILURE;
	}

	error = FT_New_Face(library, argv[1], 0, &face);
	if (error) {
		ftperror("FT_New_Face", error);
		return EXIT_FAILURE;
	}

	error = FT_Set_Pixel_Sizes(face, TILE_WIDTH, TILE_ROWS);
	if (error) {
		ftperror("FT_Set_Pixel_Sizes", error);
		return EXIT_FAILURE;
	}

	for (i = 0; i < 128; i++) {
		error = FT_Load_Char(face, i, FT_LOAD_RENDER);
		if(error) {
			ftperror("FT_Load_Char", error);
			return EXIT_FAILURE;
		}
#ifndef NDEBUG
		if(isprint(i))
			fprintf(stderr, "%c\n", i);
		else
			fprintf(stderr, "0x%hhx\n", i);
#endif
		draw_tile(face);
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	return EXIT_SUCCESS;
}
