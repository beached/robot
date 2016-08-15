#ifndef HAS_DAWMJPEGSERVER
#define HAS_DAWMJPEGSERVER

#include <boost/asio.hpp>
#include <atomic>
#include "camera.h"

namespace daw {
	///
	/// Uses OpenCV to capture a video and serves it via a small web server
	///
	class MjpegServer {
	private:
		boost::asio::io_service mIoService;
		boost::asio::ip::tcp::acceptor mAcceptor;
		daw::Camera& mCamera;
		std::atomic<bool> mRun;
		const uint32_t mInterval;
		std::thread mServerThread;
	public:
		MjpegServer( const uint16_t port, daw::Camera& camera, const uint32_t interval = 25 );
		~MjpegServer( );
		void startBackgroundCapture( );
		void stopBackgroundCapture( );
		void wait( );
	};
}

#endif	//HAS_DAWMJPEGSERVER

