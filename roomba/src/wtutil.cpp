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
#include <Wt/WRasterImage>
#include <Wt/WImage>
#include <Wt/WLink>
#include <Wt/WColor>
#include "opencvimage.h"
#include "nullptr.h"
#include "wtutil.h"

namespace daw { namespace roomba { namespace util {
	void clearWRaster( Wt::WImage* wimg, Wt::WRasterImage* wraster ) {
		if( nullptr != wraster  ) {
			wimg->setImageLink( Wt::WLink( ) );
		}
	}

	void openCVImageToWRaster( Wt::WImage* wimg, Wt::WRasterImage* wraster, daw::imaging::OpenCVImage* input_image, Wt::WObject* parent ) {
		if( nullptr != input_image ) {
			const int height = input_image->get( )->height;
			const int width = input_image->get( )->width;

			if( nullptr == wraster ) {
				wraster = new Wt::WRasterImage( "JPG", width, height, parent );
				wimg->setResource( wraster );
			}
			const int widthStep = input_image->get( )->widthStep;
			const int nChannels = input_image->get( )->nChannels;
			for( int y=0; y<height; ++y ) {
				uchar* row = (uchar*)(input_image->get( )->imageData + widthStep*y );
				for( int x=0; x<width; ++x ) {
					uchar* cur_val = (row + x*nChannels);
					wraster->setPixel( x, y, Wt::WColor( cur_val[2], cur_val[1], cur_val[0] ) );
				}
			}
		}
	}

	void setWRaster( Wt::WImage* wimg, Wt::WRasterImage* wraster, daw::imaging::OpenCVImage* image, Wt::WObject* parent ) {
		if( nullptr != image ) {
			openCVImageToWRaster( wimg, wraster, image, parent );
			nullcheck( wraster, "setWRaster: wraster returned from cvImgToRaster is null" );
			wimg->refresh( );
		}
	}

}}}

