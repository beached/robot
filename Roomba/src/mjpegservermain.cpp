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
//
// Test the mjpegserver portion on it's own
#include <cstdlib>
#include <algorithm>
#include "camera.h"
#include "mjpegserver.h"

daw::MjpegServer* mjsptr = nullptr;

void sigint_handler( int sig ) {
	std::cout << "Exiting"  << std::endl;
	if( nullptr != mjsptr ) {
		mjsptr->stopBackgroundCapture( );
		exit( EXIT_SUCCESS );
	} else {
		exit( EXIT_FAILURE );
	}
}

int main( int argc, char** argv ) {
	signal( SIGINT, sigint_handler );
	int width = -1;
	int height = -1;
	bool findFaces = false;
	if( argc > 1) {
		std::stringstream ss;
		ss << argv[1];
		ss >> width;
		ss.str( std::string( ) );
		ss.clear( );
		ss << argv[2];
		ss >> height;
		if( argc > 2 ) {
			ss.str( std::string( ) );
			ss.clear( );
			ss << argv[3];
			std::string shouldFind = ss.str( );
			std::transform( shouldFind.begin( ), shouldFind.end( ), shouldFind.begin( ), ::tolower );
			findFaces = "true" == ss.str( );
		}
	}
	std::cout << argv[0] << ": Starting Camera";
	if( width > 0 && height > 0 ) {
		std::cout << "with a resolution of " << width << "x" << height;
	}
	std::cout << std::endl;
	daw::Camera camera( width, height, findFaces );
	daw::MjpegServer mjs( 8765, camera, 25 );
	mjsptr = &mjs;
	mjs.startBackgroundCapture( );
	mjs.wait( );
	return EXIT_SUCCESS;
}

