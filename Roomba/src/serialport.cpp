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
#include <boost/thread.hpp>
#include <string>
#include <vector>

#include "serialport.h"

namespace daw { 
	boost::mutex SerialPort::mSerialPortMutex;

	SerialPort::SerialPort( std::string device ): mIO( ), mSerialPort( mIO, device.c_str( ) ) {
		mSerialPort.set_option( boost::asio::serial_port_base::baud_rate( 57600 ) );
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

