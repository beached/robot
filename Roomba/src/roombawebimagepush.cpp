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

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WImage>
#include <Wt/WRasterImage>

#include "roombawebimagepush.h"
#include "roombawebserver.h"

namespace daw { namespace roomba {
	RoombaWebImagePush::RoombaWebImagePush( RoombaWebServer& server, Wt::WContainerWidget *parent ): Wt::WImage( parent ), mServer( server ), mRasterImage( nullptr ) {
		Wt::WApplication *app = Wt::WApplication::instance( );

		mServer.connect( this, app->bind( boost::bind( &RoombaWebImagePush::updateImage, this ) ) );
		app->enableUpdates( true );
	}

	void RoombaWebImagePush::updateImage( ) {
		// Update the raster data from the static image data.
		if( nullptr == mRasterImage ) {
			mRasterImage = mServer.getImage( );
			setResource( mRasterImage );
		} else {
			mRasterImage = mServer.getImage( );
		}		
		mRasterImage->Wt::WResource::setChanged( );
		Wt::WApplication::instance( )->triggerUpdate( );
	}

	RoombaWebImagePush::~RoombaWebImagePush( ) {
		mServer.disconnect( this );
		Wt::WApplication::instance( )->enableUpdates( false );
	}
}}

