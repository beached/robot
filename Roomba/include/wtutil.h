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
#ifndef HAS_WUTIL_HEADER
#define HAS_WUTIL_HEADER

#include <Wt/WRasterImage>
#include <Wt/WImage>
#include "opencvimage.h"

namespace daw { namespace roomba { namespace util {
	void clearWRaster( Wt::WImage* wimg, Wt::WRasterImage* wraster );
	void openCVImageToWRaster( Wt::WImage* wimg, Wt::WRasterImage* wraster, daw::imaging::OpenCVImage* input_image, Wt::WObject* parent );
	void setWRaster( Wt::WImage* wimg, Wt::WRasterImage* wraster, daw::imaging::OpenCVImage* image, Wt::WObject* parent );
}}}

#endif	//HAS_UTIL_HEADER

