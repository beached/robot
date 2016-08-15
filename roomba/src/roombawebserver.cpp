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
#include <atomic>
#include <boost/thread.hpp>
#include <algorithm>
#include <Wt/WApplication>
#include <Wt/WServer>
#include <Wt/WRasterImage>
#include <boost/thread.hpp>
#include <vector>
#include "opencvimage.h"
#include "roombawebserver.h"

namespace daw { namespace roomba {

	RoombaWebServer::RoombaWebServer( Wt::WServer& server ):  mRun( true ), mServer( server ), mRasterImage( nullptr ), mLaserOn( false ) {
	//	mThread = boost::thread( boost::bind( &RoombaWebServer::run, this ) );
	}
	
	RoombaWebServer::~RoombaWebServer( ) {
		Wt::log( "info" ) << "Stopping RoombaWebServer";
		mLaserOn = false;
		mRun = false;
		if( mThread.joinable( ) ) {
			mThread.join( );
		}
	}
	
	void RoombaWebServer::connect( Client *client, const boost::function<void( )>& function ) {
		Wt::log( "info" ) << "Adding client to RoombaWebServer";
		boost::lock_guard<boost::mutex> lock( mMutex );
		mConnections.push_back( Connection( Wt::WApplication::instance( )->sessionId( ), client, function ) );
	}
	
	void RoombaWebServer::disconnect( Client *client ) {
		Wt::log( "info" ) << "Removing client to RoombaWebServer";
		boost::lock_guard<boost::mutex> lock( mMutex );
		auto position = std::find_if( mConnections.begin( ), mConnections.end( ), [client]( const Connection& currentConnection ) {
			return currentConnection.client == client;
		} );

		if( mConnections.end( ) != position ) {
			mConnections.erase( position );
			if( 0 == mConnections.size( ) ) {
				
			}
			return;
		}
		Wt::log( "error" ) << "Attempt to remove non-existant client";
	}
	
	void RoombaWebServer::run( ) {
		Wt::log( "info" ) << "Starting RoombaWebServer::run( )";
		boost::this_thread::sleep( boost::posix_time::milliseconds( interval ) );
		while( mRun ) {
			boost::this_thread::sleep( boost::posix_time::milliseconds( 2000 ) );
		}
	}

	Wt::WRasterImage* RoombaWebServer::getImage( ) {
		return mRasterImage;
	}

	void RoombaWebServer::ToggleLaser( ) {
		mLaserOn = !mLaserOn;
	}
}}

