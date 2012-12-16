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
#include <Wt/WServer>

#include "roombawebserver.h"
#include "roombawebapplication.h"
#include "mjpegserver.h"

using namespace daw::roomba;


Wt::WApplication *createApplication( const Wt::WEnvironment& env, RoombaWebServer& server, const std::string& serialPort ) {
	return new RoombaWebApplication( env, server, serialPort );
}

int main( int argc, char **argv ) {
	try {
		if( argc < 3 ) {
			return EXIT_FAILURE;
		}
		Wt::WServer server( argv[0] );
		RoombaWebServer imageServer( server );
		int width = -1;
		int height = -1;
		if( argc > 1) {
			std::stringstream ss;
			ss << argv[2];
			ss >> width;
			ss.str( std::string( ) );
			ss.clear( );
			ss << argv[3];
			ss >> height;
		}
		std::cout << argv[0] << ": Starting Camera";
		if( width > 0 && height > 0 ) {
			std::cout << "with a resolution of " << width << "x" << height;
		}
		std::cout << std::endl;
		daw::Camera camera( width, height, false );

		daw::MjpegServer mjs( 8765, camera, 1 );
		mjs.startBackgroundCapture( );
		const int argcount = argc - 1;
		char** arguments = new char*[argc];
		arguments[0] = argv[0];
		for( int n=4; n<argc; ++n ) {
			arguments[n-1] = argv[n];
		}
		std::string serialPort = argv[1];
		server.setServerConfiguration( argcount, arguments, WTHTTP_CONFIGURATION );

		server.addEntryPoint( Wt::Application, boost::bind( createApplication, _1, boost::ref( imageServer ), boost::ref( serialPort )  ) );

		if( server.start( ) ) {
			int sig = Wt::WServer::waitForShutdown( );
			std::cerr << "Shutting down: (signal = " << sig << ")" << std::endl;
			server.stop( );
			mjs.stopBackgroundCapture( );
		}
	} catch( std::exception e ) {
		std::cerr << "Uncaught exception: " << e.what( ) << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

