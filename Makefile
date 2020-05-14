#########################################################################
# Copyright © 2020 James Larrowe					#
#									#
# This file is part of ttf2gb.						#
#									#
# ttf2gb is free software: you can redistribute it and/or modify	#
# it under the terms of the GNU General Public License as published by	#
# the Free Software Foundation, either version 3 of the License, or	#
# (at your option) any later version.					#
#									#
# ttf2gb is distributed in the hope that it will be useful,		#
# but WITHOUT ANY WARRANTY; without even the implied warranty of	#
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		#
# GNU General Public License for more details.				#
#									#
# You should have received a copy of the GNU General Public License	#
# along with ttf2gb.  If not, see <https://www.gnu.org/licenses/>.	#
#########################################################################
.POSIX:

# variables to be set by the user
CC = gcc
CFLAGS =
LDFLAGS =
PKG_CONFIG = pkg-config

# variables to be set by the maintainer
BUILD = RELEASE
DEBUG_OPTFLAGS = -g3
RELEASE_OPTFLAGS = -O3 -DNDEBUG
WARNFLAGS = -Wall -Wextra -std=c89 -pedantic
FT_CFLAGS = `$(PKG_CONFIG) --cflags freetype2`
FT_LDFLAGS = `$(PKG_CONFIG) --libs freetype2`
REAL_CFLAGS = $($(BUILD)_OPTFLAGS) $(WARNFLAGS) $(FT_CFLAGS) $(CFLAGS)
REAL_LDFLAGS = $(REAL_CFLAGS) $(FT_LDFLAGS) $(LDFLAGS)

OBJ = main.o utils.o drawtile.o

ttf2gb: $(OBJ)
	$(CC) $(REAL_LDFLAGS) -o $@ $(OBJ)

.c.o:
	$(CC) $(REAL_CFLAGS) -c -o $@ $<

clean:
	rm -f ttf2gb $(OBJ)
