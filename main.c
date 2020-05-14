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
#include FT_FREETYPE_H

#ifndef NDEBUG
#include <ctype.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#define TILE_ROWS 8
#define TILE_WIDTH 8

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
