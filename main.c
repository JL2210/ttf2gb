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

#include <stdio.h>
#include <stdlib.h>

#define TILE_SIZE 8

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

void draw_tile(FT_Bitmap *bitmap)
{
	FT_UInt i, j;
	for (i = 0; i < TILE_SIZE; i++) {
		fputs("\tDW `", stdout);
		for (j = 0; j < TILE_SIZE; j++) {
			if (i >= bitmap->rows || j >= bitmap->width) {
				putchar('0');
				continue;
			}
			putchar(bitmap->buffer[i * bitmap->width + j] ? '3' : '0');
		}
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

	error = FT_Set_Pixel_Sizes(face, 0, TILE_SIZE);
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
		draw_tile(&face->glyph->bitmap);
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	return EXIT_SUCCESS;
}
