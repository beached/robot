#ifndef HAS_DAWV4LCAMERA_HEADER
#define HAS_DAWV4LCAMERA_HEADER

#include <string>

namespace daw {
	class V4LCamera {
	private:
		int mFd;
		int mWidth;
		int mHeight;
	public:
		V4LCamera( std::string deviceName, int width = 640, int height = 480 );
	private:
	};
}

#endif	//HAS_DAWV4LCAMERA_HEADER

#include <asm/types.h> 
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <linux/videodev2.h>
#include <string>
#include <sys/ioctl.h>

namespace daw {
	namespace {
		template<class T>
		clear( T& value ) {
			memset( &value, 0, sizeof( T ) );
		}

		bool initRead( );

		bool openDevice( std::string deviceName, int& fd ) {
			stat st;
			if( -1 == stat( deviceName.c_str( ), &st ) {
				std::cerr << "Cannot identify " << deviceName << ": " << errno << ", " << strerror( errno ) << std::endl;
				return false;
			}

			if( !S_ISCHR( st.st_mode ) ) {
				std::cerr << deviceName << " is not a device" << std::endl;
				return false;
			}

			fd = open( deviceName.c_str( ), O_RDWR | O_NONBLOCK, 0 );
			if( -1 == fd ) {
				std::cerr << "Cannot open " << deviceName << ": " << errno << ", " << strerror( errno ) << std::endl;
				return false;
			}
			return true;
		}

		bool initDevice( std::string deviceName, int& fd, int& width, int& height ) {
			v4l2_capability cap;
			v4l2_cropcap cropCap;
			v4l2_crop crop;
			v4l2_format fmt;
			uint32_t min;
	
			if( -1 == xioctl( fd, VIDIOC_QUERYCAP, &cap ) ) {
				if( EINVAL == errno ) {
					std::cerr << deviceName << " is not a V4L2 device" << std::endl;
					return false;
				} else {
					std::cerr << "Error initializing " << deviceName << ": " << errno << ", " << strerror( errno ) << std::endl;
					return false;
				}	
			}
			
			if( !(cap.capabilities & V4L2_CAP_VIDEOCAPTURE) ) {
				std::cerr << deviceName << " does not support video capture" << std::endl;
				return false;
			}
	
			if( !(cap.capabilities & V4L2_CAP_READWRITE) ) {
				std::cerr << deviceName << " does not reading or writing IO" << std::endl;
				return false;
			}
	
			clear( cropCap );
	
			cropCap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			if( 0 == xioctl( fd, VIDIOC_CROPCAP, &cropCap ) ) {
				cropCap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				crop.c = cropcap.defrect;
	
				if( -1 == xioctl( fd, VIDIOC_S_CROP, &crop ) ) {
					switch( errno ) {
					case EINVAL:
						// Cropping not supported
						break;
					default:
						// Ignore errors;
						break;
					}
				}
			} else {
				// Ignore errors
			}
	
			clear( fmt );
	
			fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			fmt.fmt.pix.width = width; 
			fmt.fmt.pix.height = height;
			fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	//		fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
	    	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	
			if( -1 == xioctl( fd, VIDIOC_S_FMT, &fmt ) ) {
				std::cerr << "Error initializing " << deviceName << ": " << errno << ", " << strerror( errno ) << std::endl;
				return false;
			}
	
			width = fmt.fmt.pix.width;		
			height = fmt.fmt.pix.height;
	
			min = fmt.fmt.pix.width * 2;
			if( fmt.fmt.pix.bytesperline < min ) {
				fmt.fmt.pix.bytesperline = min;
			}

			min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
			if( fmt.fmt.pix.sizeimage < min ) {
				fmt.fmt.pix.sizeimage = min;
			}

			initRead( fmt.fmt.pix.sizeimage );	
		}
	}
	V4LCamera::V4LCamera( std::string deviceName, int width, int height ): mFd( -1 ), mWidth( width ), mHeight( height ) {
		if( !openDevice( deviceName, mFd ) || 0 > mFd ) {
			throw std::runtime_error( "Error opening device." );
		}
		if( !initDevice( deviceName, mFd, mWidth, mHeight ) || 0 > mFd ) {
			throw std::runtime_error( "Error initializing device." );
		}
	}

}
