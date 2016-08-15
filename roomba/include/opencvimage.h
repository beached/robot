//opencvimage.h
#ifndef OPENCVIMAGE_H
#define OPENCVIMAGE_H

#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <opencv/cv.h>
#include <string>

namespace daw { namespace imaging {

class OpenCVImage: public boost::noncopyable {
private:
	IplImage* mImage;
	static CvHaarClassifierCascade* mCascade;
	static CvMemStorage* mMemStorage;
	static boost::mutex mMutexFaceDetection;
public:
//	OpenCVImage( );
	OpenCVImage( OpenCVImage&& src );
	OpenCVImage& operator=( OpenCVImage&& src );
	OpenCVImage( const int width, const int height, const int depth, const int channels );
	OpenCVImage( const std::string &filename, const int iscolor = 1 );
	OpenCVImage( const IplImage* const src );
	OpenCVImage& operator=( const IplImage* const src );
	~OpenCVImage( );
	OpenCVImage copy( ) const;
	IplImage* get( );
	const IplImage* get( ) const;
	int width( ) const;
	int height( ) const;
	int widthStep( ) const;
	int nChannels( ) const;
	void markFaces( );
};

} }

#endif // OPENCVIMAGE_H

