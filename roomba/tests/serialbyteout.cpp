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

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <cstdlib>
#include <iostream>
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

	boost::mutex SerialPort::mSerialPortMutex;

	SerialPort::SerialPort( std::string device ): mIO( ), mSerialPort( mIO, device.c_str( ) ) {
		mSerialPort.set_option( boost::asio::serial_port_base::baud_rate( 115200 ) );
		mSerialPort.set_option( boost::asio::serial_port_base::character_size( 8 ) );
		mSerialPort.set_option( boost::asio::serial_port_base::parity( boost::asio::serial_port_base::parity::none ) );
		mSerialPort.set_option( boost::asio::serial_port_base::stop_bits( boost::asio::serial_port_base::stop_bits::one ) );	// default one
		mSerialPort.set_option( boost::asio::serial_port_base::flow_control( boost::asio::serial_port_base::flow_control::none ) );
	}

	const std::vector<unsigned char> SerialPort::receive( size_t count ) {
		//TODO: fix me to not read 1 byte at a time.  It works for now and gets around unavailable data on roomba
		boost::lock_guard<boost::mutex> lock( SerialPort::mSerialPortMutex );
		std::vector<unsigned char> ret;
		ret.reserve( count );
		boost::asio::read( mSerialPort, boost::asio::buffer( &ret[0], count ) );
		return ret;
	}


	void SerialPort::send( const unsigned char value ) {
		boost::lock_guard<boost::mutex> lock( SerialPort::mSerialPortMutex );
		const std::vector<unsigned char> data( { value } );
		boost::asio::write( mSerialPort, boost::asio::buffer( &data[0], 1 ) );
		// TODO: make sure all bytes are written and accont for errors
	}

	void SerialPort::send( const std::vector<unsigned char>& values, size_t count ) {
		boost::lock_guard<boost::mutex> lock( SerialPort::mSerialPortMutex );
		if( 0 == count ) {
			count = values.size( );
		}
		boost::asio::write( mSerialPort, boost::asio::buffer( &values[0], count ) );
		// TODO: make sure all bytes are written and accont for errors
	}

}

int main( int argc, char** argv ) {
	if( 3 == argc ) {
		daw::SerialPort outp( argv[1] );
		if( outp.is_open( ) ) {
			unsigned int val = 0;
			{
				std::stringstream ss;
				ss << argv[2];
				ss >> val;
			}
			std::cout << "Outputting '" << (char)val << "'(" << (int)val << ")" << std::endl;
			// Setup mask
			//boost::this_thread::sleep( boost::posix_time::milliseconds( 1 ) );
			outp.send( { 0xFF, 0x40 } );	// 255 is CMD_SETMASKD0, next byte is the mask, 64 sets bit 7 so that D7 is enabled
			outp.send( { 0x40, (uint8_t)val } );		// 64 is CMD_WRITE, next byte is values for pins D0..D7
			return EXIT_SUCCESS;
		} else {
			std::cerr << "Error opening port " << argv[1] << std::endl;
		}
	} else {
		std::cerr << "Not enough parameters " << argv[0] << " <port> <byte>" << std::endl;
	}	
	return EXIT_FAILURE;
}
