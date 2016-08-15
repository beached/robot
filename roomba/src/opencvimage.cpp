//opencvimage.cpp

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <stdexcept>
#include <string>
#include "nullptr.h"
#include "opencvimage.h"


namespace daw { namespace imaging {
	CvHaarClassifierCascade* OpenCVImage::mCascade = nullptr;
	CvMemStorage* OpenCVImage::mMemStorage = nullptr;
	boost::mutex OpenCVImage::mMutexFaceDetection;

	OpenCVImage::OpenCVImage( OpenCVImage&& src ):mImage( nullptr ) {
		nullcheck( src.mImage, "OpenCVImage::OpenCVImage( OpenCVImage&& src ) - tried to pass a null source image" );
		*this = std::move( src );
	}

	OpenCVImage& OpenCVImage::operator=( OpenCVImage&& src ) {
		nullcheck( src.mImage, "OpenCVImage& OpenCVImage::operator=( OpenCVImage&& src ) - tried to pass a null source image" );
		if( &src != this ) {
			mImage = src.mImage;
			src.mImage = nullptr;
		}
		return *this;
	}

	OpenCVImage::OpenCVImage( const IplImage* const src ): mImage( nullptr ) {
		nullcheck( src, "OpenCVImage::OpenCVImage( const IplImage* const src ) - src was null" );
		mImage = cvCreateImage( cvSize( src->width, src->height ), src->depth, src->nChannels );
		cvCopy( src, mImage );
		nullcheck( mImage, "OpenCVImage::OpenCVImage( const IplImage* const src ) - could not create new image" );
	}

	OpenCVImage& OpenCVImage::operator=( const IplImage* const src ) {
		nullcheck( src, "OpenCVImage& OpenCVImage::operator=( const IplImage* const src ) - src was null" );
		OpenCVImage tmp( src );
		std::swap( *this, tmp );
		return *this;
	}

	OpenCVImage::OpenCVImage( const int width, const int height, const int depth, const int channels ):mImage( cvCreateImage( cvSize( width, height ), depth, channels ) ) {
		nullcheck( mImage, "OpenCVImage::OpenCVImag( const int width, const int height, const int depth, const int channels ) - Could not create IplImage" );
	}

	OpenCVImage::OpenCVImage( const std::string &filename, const int iscolor ):mImage( cvLoadImage( filename.c_str( ), iscolor ) ) {
		nullcheck( mImage, "OpenCVImage::OpenCVImag - Could not create IplImage" );
	}

	OpenCVImage OpenCVImage::copy( ) const {
		nullcheck( mImage, "OpenCVImage&& OpenCVImage::copy( ) - image was null" );
		OpenCVImage ret( mImage->width, mImage->height, mImage->depth, mImage->nChannels );
		nullcheck( ret.mImage, "OpenCVImage&& OpenCVImage::copy( const OpenCVImage& src ) - could not create new image" );
		cvCopy( mImage, ret.mImage );
		return ret;
	}

	OpenCVImage::~OpenCVImage( ) {
		if( nullptr != mImage ) {
			cvReleaseImage( &mImage );
			mImage = nullptr;
		}
		/* Now static
		if( nullptr != mCascade ) {
			cvReleaseHaarClassifierCascade( &mCascade );
			mCascade = nullptr;
		}
		if( nullptr != mMemStorage ) {
			cvReleaseMemStorage( &mMemStorage );
			mMemStorage = nullptr;
		}
		*/
	}

	const IplImage* OpenCVImage::get( ) const {
		return mImage;
	}

	IplImage* OpenCVImage::get( ) {
		return mImage;
	}

	int OpenCVImage::width( ) const {
		return mImage->width;
	}

	int OpenCVImage::height( ) const {
		return mImage->height;
	}

	int OpenCVImage::widthStep( ) const {
		return mImage->widthStep;
	}

	int OpenCVImage::nChannels( ) const {
		return mImage->nChannels;
	}

	void OpenCVImage::markFaces( ) {
		{	// Lock
			boost::lock_guard<boost::mutex> lock( OpenCVImage::mMutexFaceDetection );	// Only one capture at a time allowed
			if( nullptr == mMemStorage ) {
				std::cerr << "OpenCVImage::markFaces( ) - Allocating storage" << std::endl;
				mMemStorage = cvCreateMemStorage( 0 );
				if( nullptr == mMemStorage ) {
					const std::string msg = "Could not alocate memory for face detection";
					std::cerr << msg << std::endl << std::flush;
					throw std::runtime_error( msg );
				}
				
			} else {
				cvClearMemStorage( mMemStorage );
			}
			if( nullptr == mCascade ) {
				std::cerr << "OpenCVImage::markFaces( ) - Creating Haar Cascade" << std::endl;
				mCascade = static_cast<CvHaarClassifierCascade*>( cvLoad( "./cascades/haarcascades/haarcascade_frontalface_default.xml", 0, 0, 0 ) );
				if( nullptr == mCascade ) {
					const std::string msg = "Could not load Haar Cascade, make sure path is correct";
					std::cerr << msg << std::endl << std::flush;
					throw std::runtime_error( msg );
				}
			}
		}
		CvSeq* faces = cvHaarDetectObjects( mImage, mCascade, mMemStorage, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize( 40, 40 ) );
		for( int n=0; n<(nullptr != faces? faces->total: 0); ++n ) {
			CvRect* r = (CvRect*)cvGetSeqElem( faces, n );
			CvPoint p1 = { r->x, r->y };
			CvPoint p2 = { r->x + r->width, r->y + r->height };
			cvRectangle( mImage, p1, p2, CV_RGB( 0, 255, 0 ), 3, 8, 0 );
		}
	}
}}

