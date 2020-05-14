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

#ifndef TTF2GB_H_
#define TTF2GB_H_ 1

#include <ft2build.h>
#include FT_TYPES_H /* FT_Error */
#include FT_FREETYPE_H /* FT_FaceRec */

/* utils.c */
/* a strerror that works on FT_Error instead of errno */
const char *ftstrerror(FT_Error error);
/* a perror for an FT_Error */
void ftperror(const char *message, FT_Error error);

/* drawtile.c */
/* draw a tile to STDOUT as RGBDS-formatted assembly */
void draw_tile(const FT_FaceRec *face);

#endif
