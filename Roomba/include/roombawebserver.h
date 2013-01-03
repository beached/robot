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
#ifndef HAS_ROOMBAWEBSERVER_HEADER
#define HAS_ROOMBAWEBSERVER_HEADER

#include <atomic>

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <Wt/WRasterImage>
#include <vector>
#include <Wt/WServer>
#include "opencvimage.h"
#include "gpio.h"

namespace daw { namespace roomba {
	class RoombaWebServer: boost::noncopyable {
	public:
		class Client { };
	private:
		struct Connection {
			Connection( const std::string& id, Client *c, const boost::function<void( )>& f ): sessionId( id ), client( c ), function( f ) { }
			std::string sessionId;
			Client *client;
			boost::function<void( )> function;
		};
		mutable boost::mutex mMutex;
		boost::thread mThread;
		std::atomic<bool> mRun;
		std::vector<Connection> mConnections;
		Wt::WServer& mServer;
		const int interval = 500;
		void run( );
		Wt::WRasterImage* mRasterImage;
		daw::Gpio mGpio;
		bool mLaserOn;
	public:
		RoombaWebServer( Wt::WServer& server );
		~RoombaWebServer( );
		void connect( Client *client, const boost::function<void( )>& function );
		void disconnect( Client *client );
		Wt::WRasterImage* getImage( );
		void ToggleLaser( );
	};
}}
#endif	//HAS_ROOMBAWEBSERVER_HEADER

