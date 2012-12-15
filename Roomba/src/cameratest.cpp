#include <boost/thread.hpp>
#include <cstdlib>
#include <iostream>
#include "camera.h"
#include "opencvimage.h"

int main( int argc, char** argv ) {
	const int interval = 1000;
	int width = -1;
	int height = -1;
	if( argc > 1) {
		std::stringstream ss;
		ss << argv[1];
		ss >> width;
		ss.str( std::string( ) );
		ss.clear( );
		ss << argv[2];
		ss >> height;
	}
	std::cout << argv[0] << ": Starting Camera";
	if( width > 0 && height > 0 ) {
		std::cout << "with a resolution of " << width << "x" << height;
	}
	std::cout << std::endl;
	daw::Camera camera( width, height, true );

	// Init camera
	for( size_t n=0; n<10; ++n ) {
		camera.capture( );
	}
	camera.startBackgroundCapture( interval );
	int count = 0;
	while( true ) {
		if( camera.hasImage( ) ) {
			daw::imaging::OpenCVImage img = camera.image( );	
			std::cout << "width: " << img.width( ) << " height: " << img.height( ) << std::endl;
			std::stringstream ss;
			ss << "./cameratest_" << count++ << ".jpg";
			const std::string fname = ss.str( );
			std::cerr << "Saving to " << fname << std::endl;
			cvSaveImage( fname.c_str( ), img.get( ) );
			if( count >= 10 ) {
				break;
			}
		}
		boost::this_thread::sleep( boost::posix_time::milliseconds( interval ) );
	}
	camera.stopWaitBackgroundCapture( );

	return EXIT_SUCCESS;
}

