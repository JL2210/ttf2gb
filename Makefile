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

CC = gcc
PKG_CONFIG = pkg-config

BUILD = DEBUG
DEBUG_OPTFLAGS = -g3
RELEASE_OPTFLAGS = -O3 -DNDEBUG
CFLAGS = -Wall -Wextra -pedantic `$(PKG_CONFIG) --cflags freetype2` $($(BUILD)_OPTFLAGS)
LDFLAGS = `$(PKG_CONFIG) --libs freetype2`

OBJ = main.o

ttf2gb: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

clean:
	rm -f ttf2gb main.o
