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
#ifndef HAS_SERIALPORTQUEUE_HEADER
#define HAS_SERIALPORTQUEUE_HEADER

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <string>
#include <vector>

namespace daw { 
	class SerialPort: public boost::noncopyable {
	private:
		boost::asio::io_service mIO;
		boost::asio::serial_port mSerialPort;
		static boost::mutex mSerialPortMutex;
	public:
		SerialPort( std::string device );
		const std::vector<unsigned char> receive( size_t count );
		void send( const unsigned char value );
		void send( const std::vector<unsigned char>& values, size_t count = 0 );
		const bool is_open( ) {
			return mSerialPort.is_open( );
		}
		void close( ) {
			mSerialPort.close( );
		}
	};
}

#endif	//HAS_SERIALPORTQUEUE_HEADER

