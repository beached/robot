/*
 *	(c)2012 Darrell Wright
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <string>
#include <sstream>
#include <vector>
#include "util.h"

namespace daw { namespace roomba { namespace util {
	unsigned short stringToShort( const std::string& str ) {
		std::stringstream ss( str );
		unsigned short ret = 0;
		ss >> ret;
		return ret;
	}

	std::vector<unsigned char> shortToChars( const short value ) {
		std::vector<unsigned char> ret;
		ret.push_back( (value >> 8) & 0xFF );
		ret.push_back( value & 0xFF );
		return ret;
	}
}}}

