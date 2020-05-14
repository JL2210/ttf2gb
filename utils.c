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
#include FT_TYPES_H

const char *ftstrerror(FT_Error error)
{
#undef FTERRORS_H_
#define FT_ERRORDEF(error_code, value, string) case error_code: return string;
#define FT_ERROR_START_LIST switch(error) {
#define FT_ERROR_END_LIST default: return "Unknown error"; }
#include FT_ERRORS_H
}

void ftperror(const char *message, FT_Error error)
{
	if(message)
		fprintf(stderr, "%s: %s\n", message, ftstrerror(error));
	else
		fprintf(stderr, "%s\n", ftstrerror(error));
}
