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
#ifndef HAS_ROOMBAWEBAPPLICATION_HEADER
#define HAS_ROOMBAWEBAPPLICATION_HEADER

#include <Wt/WApplication>
#include <Wt/WLineEdit>
#include <Wt/WImage>
#include <Wt/WRasterImage>
#include <Wt/WTimer>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "concurrentqueue.h"
#include "roombacontrol.h"
#include "opencvimage.h"
#include "roombawebserver.h"

namespace daw { namespace roomba {
	class RoombaWebApplication : public Wt::WApplication {
	public:
		RoombaWebApplication( const Wt::WEnvironment& env, RoombaWebServer& server, const std::string& serialPort );
	private:
		// These are all singleton's and can never have more than one
		daw::roomba::RoombaControl mRC;
		Wt::WLineEdit *txtSpeed;
		Wt::WLineEdit *txtRadius;
		Wt::WLineEdit *txtDuration;
		Wt::WLineEdit *txtTurnAngle;
		Wt::WRasterImage* rstCurrentView;
		Wt::WText* lblStatus;
		Wt::WImage* imgCurrentView;
		int mSpeed;
		int mRotSpeed;
		RoombaWebServer& mServer;
		bool mIsCleaning;
		void updateStatus( );
		void updateVideo( );
		void move( );
		void moveUp( );
		void moveDown( );
		void moveLeft( );
		void moveRight( );
		void moveStop( );
		void turn( );
		void turnClockwise( );
		void turnCounterClockwise( );
		void toggleVacuum( );
		void explore( );
	};
	
}}

#endif	//HAS_ROOMBAWEBAPPLICATION_HEADER

