//    Copyright (c) 2014 Parabaik C Converter Project
//
//    Parabaik C Converter is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Parabaik C Converter is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __parabaik_C_converter__parabaik__
#define __parabaik_C_converter__parabaik__

#include <stdio.h>

int zuconverter_open(); // returns non-zero on error
int zuconverter_close(); // returns non-zero on error

char *zawgyi_to_unicode(const char *input); // returns converted utf-8 encoded string
char *unicode_to_zawgyi(const char *input); // returns converted utf-8 encoded string

#endif /* defined(__parabaik_C_converter__parabaik__) */
