#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cstdint>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "mjpegserver.h"
#include "opencvimage.h"
#include "opencvmat.h"
#include "camera.h"

using boost::asio::ip::tcp;
typedef boost::shared_ptr<tcp::socket> socket_ptr;

namespace {
	void session( socket_ptr sock, daw::Camera& camera, const uint32_t interval ) {
		try {
			std::cout << "Connection from: " << sock->remote_endpoint( ).address( ).to_string( ) << std::endl;
			const std::string eol = "\r\n";
			const std::string boundary = "--myboundary" + eol;
			{
				std::stringstream ss;
				ss << "HTTP/1.0 200 OK" << eol;
				ss << "Cache-Control: no-cache" << eol;
				ss << "Pragma: no-cache" << eol;
				ss << "Expires: Thu, 01 Dec 1994 16:00:00 GMT" << eol;
				ss << "Connection: close" << eol;
				ss << "Content-Type: multipart/x-mixed-replace;boundary=myboundary" << eol;
				ss << boundary << eol;
				const std::string strHeader = ss.str( );
				boost::asio::write( *sock, boost::asio::buffer( strHeader.c_str( ), strHeader.size( ) ) );
			}
			boost::system::error_code errcode;
			int imageCount = 0;
			while( sock->is_open( ) ) {
				if( camera.imageCount( ) != imageCount ) {	// Will roll over and the count is always incrementing by one
					try {	
						daw::OpenCVMat jpeg = camera.imageJpeg( imageCount );
						std::stringstream ss;
						ss << boundary;
						ss << "Content-Type: image/jpeg" << eol;
						ss << "Content-Length: " << jpeg.get( )->cols << eol << eol;
						const std::string imgData = ss.str( );
						boost::asio::write( *sock, boost::asio::buffer( imgData.c_str( ), imgData.size( ) ), boost::asio::transfer_all( ), errcode );
						if( errcode ) {
							std::cerr << "Error writing: " << errcode.message( ) << std::endl;
							break;
						}
						boost::asio::write( *sock, boost::asio::buffer( jpeg.get( )->data.ptr, jpeg.get( )->cols ), boost::asio::transfer_all( ), errcode );
						if( errcode ) {
							std::cerr << "Error writing: " << errcode.message( ) << std::endl;
							break;
						}
						boost::asio::write( *sock, boost::asio::buffer( eol.c_str( ), eol.size( ) ), boost::asio::transfer_all( ), errcode );
						if( errcode ) {
							std::cerr << "Error writing: " << errcode.message( ) << std::endl;
							break;
						}
					} catch( std::exception& ex ) {
						std::cerr << "void session( socket_ptr sock, daw::Camera& camera, const uint32_t interval ) - Error grabbing image from camera: " << ex.what( ) << std::endl;
					}
				}
				boost::this_thread::sleep( boost::posix_time::milliseconds( interval ) );
			}
		} catch( std::exception& ex ) {
			std::cerr << "void session( socket_ptr sock, daw::Camera& camera, const uint32_t interval ) - Uncaught Exception" << ex.what( ) << std::endl;
		}
	}
}

namespace daw {
	MjpegServer::MjpegServer( const uint16_t port, daw::Camera& camera, const uint32_t interval ): mIoService( ), mAcceptor( mIoService, tcp::endpoint( tcp::endpoint( tcp::v4( ), port ) ) ), mCamera( camera ), mRun( false ), mInterval( interval ), mServerThread( ) { }
	
	MjpegServer::~MjpegServer( ) {
		try {
			stopBackgroundCapture( );
			if( mServerThread.joinable( ) ) {
				mServerThread.join( );
			}
		} catch( std::exception& ex ) {
			std::cerr << "Exception while closing MJPEG service: " << ex.what( ) << std::endl;
		}
	}
	
	void MjpegServer::startBackgroundCapture( ) {
		mRun = true;
		mIoService.run( );
		mServerThread = std::thread( [&]( ) {
			while( mRun ) {
				mCamera.startBackgroundCaptureIfNotRunning( mInterval );
				socket_ptr sock( new tcp::socket( mIoService ) );
				mAcceptor.accept( *sock );
				boost::thread t( boost::bind( session, sock, boost::ref( mCamera ), mInterval ) );
			}
		} );
	}

	void MjpegServer::stopBackgroundCapture( ) {
		mRun = false;
		mIoService.stop( );
	}

	void MjpegServer::wait( ) {
		if( mServerThread.joinable( ) ) {
			mServerThread.join( );
		}
	}
}

