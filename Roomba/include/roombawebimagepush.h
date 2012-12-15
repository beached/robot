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
#ifndef HAS_DAWROOMBAWEBIMAGEPUSH_HEADER
#define HAS_DAWROOMBAWEBIMAGEPUSH_HEADER

#include <Wt/WImage>
#include <Wt/WRasterImage>
#include <Wt/WContainerWidget>

#include "roombawebserver.h"


namespace daw { namespace roomba {
	class RoombaWebImagePush: public Wt::WImage, public RoombaWebServer::Client {
	private:
		void updateImage( );
		RoombaWebServer& mServer;
		Wt::WRasterImage* mRasterImage;
	public:
		RoombaWebImagePush( RoombaWebServer& server, Wt::WContainerWidget *parent = nullptr );
		virtual ~RoombaWebImagePush( );
	};
}}

#endif	//HAS_DAWROOMBAWEBIMAGEPUSH_HEADER

