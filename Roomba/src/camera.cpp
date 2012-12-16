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
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <chrono>
#include <opencv/cv.h> 
#include <opencv/highgui.h> 
#include <string>
#include <thread>
#include <vector>

#include "opencvimage.h"
#include "opencvmat.h"
#include "camera.h"

namespace daw { 
	Capture::Capture( ): Capture( CV_CAP_ANY ) { }
	Capture::Capture( int cameraIndex ): mCapture( cvCreateCameraCapture( cameraIndex ) ) { }

	Capture::~Capture( ) {
		if( nullptr != mCapture ) {
			cvReleaseCapture( &mCapture );
		}
	}

	CvCapture* Capture::get( ) {
		return mCapture;
	}

	Camera::Camera( int width, int height, bool markFaces ): Camera( width, height, CV_CAP_ANY, markFaces ) { }

	Camera::Camera( int width, int height, int cameraIndex, bool markFaces ):mCapture( cameraIndex ), mCaptureMutex( ), mCameraMutex( ), mCapturedImageJpeg( nullptr ), mRun( false ), mImgCounter( 0 ), mMarkFaces( markFaces ) {
		// Only one at a time allow, other contructors will lock
		boost::lock_guard<boost::mutex> lock( Camera::mCaptureMutex );	// Only one capture at a time allowed
		nullcheck( mCapture.get( ), "Camera::Camera( ) - Error initializing CvCapture" );
		std::cerr << "**** Camera::Camera( )" << std::endl;
		if( nullptr == mCapture.get( ) ) {
			const std::string msg = "Camera::Camera( ) - Error aquiring camera";
			std::cerr << "**** " << msg << std::endl << std::flush;
			throw std::runtime_error( msg );
		}
		if( 0 < width && 0 < height ) {
			cvSetCaptureProperty( mCapture.get( ), CV_CAP_PROP_FRAME_WIDTH, width );
			cvSetCaptureProperty( mCapture.get( ), CV_CAP_PROP_FRAME_HEIGHT, height );
		}

		int fps = cvGetCaptureProperty( mCapture.get( ), CV_CAP_PROP_FPS );
		if( 0 >= fps ) {
			fps = 10;
		}
		mDelay = 1000000/fps;
	}

	void Camera::startBackgroundCapture( const unsigned int interval ) {
		unsigned int delay = mDelay;
		std::cerr << "Delaying capture for " << delay << "µs between frames" << std::endl;
		if( interval*1000 > delay ) {
			delay = interval*1000;
		}
		mCaptureThread = std::thread( [&, delay]( ) {
			mRun = true;
			while( mRun ) {
				capture( );
				boost::this_thread::sleep( boost::posix_time::microseconds( delay ) );
			}
		} );
	}

	void Camera::startBackgroundCaptureIfNotRunning( const size_t interval ) {
		boost::mutex::scoped_lock lock( mCameraMutex, boost::try_to_lock );
		if( lock && !isRunning( ) ) {
			startBackgroundCapture( interval );
		}
	}

	void Camera::stopBackgroundCapture( ) {
		mRun = false;
	}

	void Camera::stopWaitBackgroundCapture( ) {
		if( mRun ) {
			mRun = false;
			if( mCaptureThread.joinable( ) ) {
				mCaptureThread.join( );
			}
		}
	}

	void Camera::capture( ) {
		boost::lock_guard<boost::mutex> lock( mCameraMutex );
		const std::vector<int> jpegParam = { CV_IMWRITE_JPEG_QUALITY, 80, 0 };
		IplImage* imgc = cvQueryFrame( mCapture.get( ) );
		if( nullptr != imgc ) {
			mCapturedImage.reset( new daw::imaging::OpenCVImage( imgc ) );
			if( mMarkFaces ) {
				mCapturedImage->markFaces( );
			}
			CvMat* jpeg = cvEncodeImage( ".jpeg", mCapturedImage->get( ), &jpegParam[0] );
			mCapturedImageJpeg.reset( new daw::OpenCVMat( jpeg, false ) );
			++mImgCounter;
		} else {
			std::cerr << "****Camera::capture( ) - Invalid image captured" << std::endl;
		}
	}

	const int Camera::imageCount( ) const {
		return mImgCounter;
	}

	daw::imaging::OpenCVImage Camera::image( int& clientCount ) const { 
		boost::lock_guard<boost::mutex> lock( mCameraMutex );
		clientCount = mImgCounter;
		return mCapturedImage->copy( );
	}

	daw::OpenCVMat Camera::imageJpeg( int& clientCount ) const {
		boost::lock_guard<boost::mutex> lock( mCameraMutex );
		if( mImgCounter > 0 ) {
			clientCount = mImgCounter;
			return *mCapturedImageJpeg;
		} else {
			throw std::runtime_error( "daw::OpenCVMat Camera::imageJpeg( int& clientCount ) const - Attempt to call before image has been captured" );
		}
	}

	const bool Camera::isRunning( ) const {
		return mRun;
	}
}

