#ifndef HAS_DAWOPENCVMAT_HEADER
#define HAS_DAWOPENCVMAT_HEADER

#include <atomic>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "nullptr.h"

namespace daw {
	class OpenCVMat {
	private:
		CvMat* ptr;
	public:
		OpenCVMat( CvMat* mat, const bool makeCopy = true ): ptr( nullptr ) {
			nullcheck( mat, "OpenCVMat( CvMat* mat, const bool makeCopy ) - Null mat passed to OpenCVMat" );
			if( makeCopy ) {
				ptr = cvCloneMat( mat );
			} else {
				ptr = mat;
			}
		}
		OpenCVMat( const OpenCVMat& other ): ptr( cvCloneMat( other.ptr ) ) { }
		OpenCVMat( OpenCVMat& other ): ptr( cvCloneMat( other.ptr ) ) { }
		
		OpenCVMat& operator=( const OpenCVMat& other ) {
			OpenCVMat tmp( other );
			std::swap( *this, tmp );
			return *this;
		}

		OpenCVMat& operator=( OpenCVMat& other ) {
			OpenCVMat tmp( other );
			std::swap( *this, tmp );
			return *this;
		}

		~OpenCVMat( ) {
			if( nullptr != ptr  ) {
				cvReleaseMat( &ptr );
			}
		}

		CvMat* get( ) {
			return ptr;
		}
	};
}

#endif	//HAS_DAWOPENCVMAT_HEADER

