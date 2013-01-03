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
#include <Wt/WEnvironment>
#include <Wt/WBreak>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WTimer>
#include <Wt/WImage>
#include <Wt/WContainerWidget>
#include <Wt/WRasterImage>
#include <Wt/WFileResource>
#include <Wt/WGridLayout>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "roombacontrol.h"
#include "roombasensors.h"
#include <iostream>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

#include "opencvimage.h"
#include "nullptr.h"
#include "util.h"
#include "roombawebserver.h"
#include "roombawebimagepush.h"
#include "roombawebapplication.h"

namespace daw { namespace roomba {
	RoombaWebApplication::RoombaWebApplication( const Wt::WEnvironment& env, RoombaWebServer& server, const std::string& serialPort ): Wt::WApplication( env ), mRC( serialPort ), txtSpeed( nullptr ), txtRadius( nullptr ), txtDuration( nullptr ), rstCurrentView( nullptr ), lblStatus( nullptr ), imgCurrentView( nullptr ), mSpeed( 0 ), mRotSpeed( 0 ), mServer( server ), mIsCleaning( false ) {
		//Init roomba
		enableUpdates( true );
		root( )->decorationStyle( ).setBackgroundColor( Wt::WColor( 50, 50, 50 ) );
		mRC.modeStart( );
		mRC.modeSafe( );
		// Init Page
		setTitle( "Roomba Web" );
		root( )->addWidget( new Wt::WText( "Speed: " ) );
		txtSpeed = new Wt::WLineEdit( root( ) );
		root( )->addWidget( new Wt::WText( " Turn Radius: " ) );
		txtRadius = new Wt::WLineEdit( root( ) );
		root( )->addWidget( new Wt::WText( " Duration: " ) );
		txtDuration = new Wt::WLineEdit( root( ) );
		Wt::WPushButton *button = new Wt::WPushButton( "Move", root( ) );
		button->clicked( ).connect( this, &RoombaWebApplication::move );
		root( )->addWidget( new Wt::WBreak( ) );

		root( )->addWidget( new Wt::WText( "Turn Angle: " ) );
		txtTurnAngle = new Wt::WLineEdit( root( ) );
		Wt::WPushButton *buttonTurn = new Wt::WPushButton( "Turn", root( ) );
		buttonTurn->clicked( ).connect( this, &RoombaWebApplication::turn );

		root( )->addWidget( new Wt::WBreak( ) );
		{
			std::stringstream ss;
			const Wt::WEnvironment& env = Wt::WApplication::instance( )->environment( );
			ss << "http://" << env.hostName( ) << ":8765";
			Wt::log( "info" ) << "Using " << ss.str( ) << " as url for video feed";
			Wt::WImage* imgMjpeg = new Wt::WImage( ss.str( ).c_str( ) );
			root( )->addWidget( imgMjpeg );
		}
		root( )->addWidget( new Wt::WBreak( ) );
		Wt::WImage* imgUp = new Wt::WImage( Wt::WLink( "arrows/up.png" ) );
		imgUp->clicked( ).connect( this, &RoombaWebApplication::moveUp );

		Wt::WImage* imgDown = new Wt::WImage( Wt::WLink( "arrows/down.png" ) );
		imgDown->clicked( ).connect( this, &RoombaWebApplication::moveDown );

		Wt::WImage* imgLeft = new Wt::WImage( Wt::WLink( "arrows/left.png" ) );
		imgLeft->clicked( ).connect( this, &RoombaWebApplication::moveLeft );

		Wt::WImage* imgRight = new Wt::WImage( Wt::WLink( "arrows/right.png" ) );
		imgRight->clicked( ).connect( this, &RoombaWebApplication::moveRight );

		Wt::WImage* imgStop = new Wt::WImage( Wt::WLink( "arrows/stop.png" ) );
		imgStop->clicked( ).connect( this, &RoombaWebApplication::moveStop );

		Wt::WImage* imgRotateCW = new Wt::WImage( Wt::WLink( "arrows/rotatecw.png" ) );
		imgRotateCW->clicked( ).connect( this, &RoombaWebApplication::turnClockwise );
		
		Wt::WImage* imgExplore = new Wt::WImage( Wt::WLink( "arrows/explore.png" ) );
		imgExplore->clicked( ).connect( this, &RoombaWebApplication::explore );
		
		Wt::WImage* imgRotateCCW = new Wt::WImage( Wt::WLink( "arrows/rotateccw.png" ) );
		imgRotateCCW->clicked( ).connect( this, &RoombaWebApplication::turnCounterClockwise );
		
		Wt::WImage* imgVacuum = new Wt::WImage( Wt::WLink( "arrows/vacuum.png" ) );
		imgVacuum->clicked( ).connect( this, &RoombaWebApplication::toggleVacuum );
		
		Wt::WGridLayout* grdControls = new Wt::WGridLayout( );
		grdControls->addWidget( imgRotateCW, 0, 1 );
		grdControls->addWidget( imgUp, 0, 2 );
		grdControls->addWidget( imgRotateCCW, 0, 3 );
		grdControls->addWidget( imgLeft, 1, 1 );
		grdControls->addWidget( imgStop, 1, 2 );
		grdControls->addWidget( imgRight, 1, 3 );
		grdControls->addWidget( imgDown, 2, 2 );
		grdControls->addWidget( imgVacuum, 2, 3 );
		
		Wt::WContainerWidget* w = new Wt::WContainerWidget( root( ) );
		w->resize( 200, 200 );
		w->setLayout( grdControls );
		root( )->addWidget( w );

		root( )->addWidget( new Wt::WBreak( ) );
		lblStatus = new Wt::WText( root( ) );
		root( )->addWidget( lblStatus );
	}
	
	void RoombaWebApplication::updateStatus( ) {
		const SensorPacket1 sd1 = mRC.getSensorData1( );
		const SensorPacket3 sd3 = mRC.getSensorData3( );
		const std::string sensorData = "<pre>" + sd1.ToString( ) + "\n" + sd3.ToString( ) + "</pre>";

		lblStatus->setText( Wt::WString::fromUTF8( sensorData ) );

	}

	void RoombaWebApplication::turn( ) {
		const std::string strAngle = txtTurnAngle->text( ).toUTF8( );
		double angle = 0;
		util::stringToT( strAngle, angle );
		bool turnClockwise = 0>angle;
		std::cout << "0 Turning " << strAngle << " degrees" << (turnClockwise?"":"counter-") << "clockwise" << std::endl;
		std::cout << "1 Turning " << angle << " degrees" << (turnClockwise?"":"counter-") << "clockwise" << std::endl;
		if( turnClockwise ) {
			angle *= -1;
		}
		if( 359.0 < angle ) {
			angle = static_cast<double>( static_cast<unsigned short>( angle )%360 );
		}
		if( 0 == angle ) {
			return;
		}
		std::cout << "2 Turning " << angle << " degrees" << (turnClockwise?"":"counter-") << "clockwise" << std::endl;
		double speed = 200.0;
		if( !txtSpeed->text( ).empty( ) ) {
			util::stringToT( txtSpeed->text( ).toUTF8( ), speed );
		}
		const double wheelBase = 258.0;
		const double PI = 3.14159265359;
		const double mmPerDeg = (wheelBase*PI)/360.0;
		const double time = mmPerDeg*angle/speed;
		if( turnClockwise ) {
			mRC.motorsMove( static_cast<unsigned short>( speed ), 1 );
		} else {
			mRC.motorsMove( static_cast<unsigned short>( speed ), -1 );
		}
		boost::this_thread::sleep( boost::posix_time::seconds( time ) );
		mRC.motorsMove( mSpeed, 0 );
	}

	void RoombaWebApplication::move( ) {
		short speed = 0;
		short radius = 0;
		util::stringToT( txtSpeed->text( ).toUTF8( ), speed );
		util::stringToT( txtRadius->text( ).toUTF8( ), radius );
		unsigned long duration = util::stringToShort( txtDuration->text( ).toUTF8( ) );
		// limit duration to 10s
		if( 10 < duration ) {
			duration = 10;
		}
		mRC.motorsMove( speed, radius );
		boost::this_thread::sleep( boost::posix_time::seconds( duration ) );
		mRC.motorsStop( );
	}

	void RoombaWebApplication::moveUp( ) {
		mSpeed += 50;
		if( 500 < mSpeed ) {
			mSpeed = 500;
		}
		mRC.motorsMove( mSpeed, 0 );
	}

	void RoombaWebApplication::moveDown( ) {
		mSpeed -= 50;
		if( -500 > mSpeed ) {
			mSpeed = -500;
		}
		mRC.motorsMove( mSpeed, 0 );
	}	

	void RoombaWebApplication::turnClockwise( ) {
		mSpeed = 0;
		mRotSpeed += 25;
		if( 0 < mRotSpeed ) {
			mRC.motorsMove( static_cast<unsigned short>( -mRotSpeed ), 1 );
		} else if( 0 > mRotSpeed ) {
			mRC.motorsMove( static_cast<unsigned short>( mRotSpeed ), -1 );
		} else {
			mRC.motorsStop( );
		}
	}

	void RoombaWebApplication::turnCounterClockwise( ) {
		mSpeed = 0;
		mRotSpeed -= 25;
		if( 0 < mRotSpeed ) {
			mRC.motorsMove( static_cast<unsigned short>( -mRotSpeed ), 1 );
		} else if( 0 > mRotSpeed ) {
			mRC.motorsMove( static_cast<unsigned short>( mRotSpeed ), -1 );
		} else {
			mRC.motorsStop( );
		}
	}

	void RoombaWebApplication::moveLeft( ) {
		const double wheelBase = 258.0;
		const double PI = 3.14159265359;
		const double mmPerDeg = (wheelBase*PI)/360.0;
		const double speed = 150.0;	
		const double time = mmPerDeg*90.0/speed;
		mRC.motorsMove( speed, 1 );
		boost::this_thread::sleep( boost::posix_time::seconds( time ) );
		mRC.motorsMove( mSpeed, 0 );
	}

	void RoombaWebApplication::moveRight( ) {
		const double wheelBase = 258.0;
		const double PI = 3.14159265359;
		const double mmPerDeg = (wheelBase*PI)/360.0;
		const double speed = 150.0;	
		const double time = mmPerDeg*90.0/speed;
		mRC.motorsMove( speed, -1 );
		boost::this_thread::sleep( boost::posix_time::seconds( time ) );
		mRC.motorsMove( mSpeed, 0 );
	}

	void RoombaWebApplication::moveStop( ) {
		mSpeed = 0;
		mRotSpeed = 0;
		mRC.motorsStop( );
	}

	void RoombaWebApplication::toggleVacuum( ) {
		if( mIsCleaning ) {
			mRC.cleanStop( );
		} else {
			mRC.cleanStart( );
		}
		mIsCleaning = !mIsCleaning;
	}

	void RoombaWebApplication::explore( ) {
		moveStop( );
		mRC.explore( );
	}

}}

