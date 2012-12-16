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
#ifndef HAS_CAMERA_HEADER
#define HAS_CAMERA_HEADER

#include <atomic>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <cstdint>
#include <opencv/cv.h> 
#include <opencv/highgui.h> 
#include <string>
#include <thread>
#include <vector>
#include <functional>
#include "opencvimage.h"
#include "opencvmat.h"

namespace daw { 
	class Capture {
	private:
		CvCapture* mCapture;
	public:
		Capture( );
		Capture( int cameraIndex );
		~Capture( );
		CvCapture* get( );
	};

	class Camera: public boost::noncopyable {
	private:
		Capture mCapture;
		mutable boost::mutex mCaptureMutex;
		mutable boost::mutex mCameraMutex;
		boost::scoped_ptr<daw::OpenCVMat> mCapturedImageJpeg;
		std::atomic<bool> mRun;
		mutable int mImgCounter;
		std::thread mCaptureThread;
		boost::scoped_ptr<daw::imaging::OpenCVImage> mCapturedImage;
		bool mMarkFaces;
	 	unsigned int mDelay;
		unsigned int mClientDelay;
		void startBackgroundCapture( const unsigned int interval );
	public:
		Camera( int width = -1, int height = -1, bool mMarkFaces = false );
		Camera( int width, int height, int cameraIndex, bool mMarkFaces = false );
		void startBackgroundCaptureIfNotRunning( const size_t interval );
		void stopBackgroundCapture( );
		void stopWaitBackgroundCapture( );
		void capture( );
		const bool hasImage( ) const;
		daw::imaging::OpenCVImage image( int& clientCount ) const;
		daw::OpenCVMat imageJpeg( int& clientCount ) const;
		const bool isRunning( ) const;
		const int imageCount( ) const;
		const unsigned int delay( ) const {
			return mClientDelay;
		}
	};
}

#endif	//HAS_CAMERA_HEADER

