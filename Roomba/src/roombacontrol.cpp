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
#include <sstream>
#include <vector>

#include "roombacontrol.h"
#include "roombasensors.h"
#include "util.h"
#include "serialport.h"

namespace daw { namespace roomba {

	RoombaControl::RoombaControl( const std::string& device ): mSerialPort( device ), mIsMoving( false ) { }

	RoombaControl::~RoombaControl( ) {
		if( mSerialPort.is_open( ) ) {
			mSerialPort.close( );
		}
	}

	/*
	*	Move the device 
	*	speed Ranges from: -500mm/s to 500mm/s
	*	radius 
	*/
	void RoombaControl::motorsMove( const short speed, const short radius, const bool initControl ) {
		if( initControl ) {
			modeControl( );
		}
		if( -500 > speed || 500 < speed ) {
			throw std::runtime_error( "Speed outside of range (-500,500)" );
		} else if( -2000 > radius || 2000 < radius ) {
			throw std::runtime_error( "Turn radius outside of range (-2000,2000)" );
		}
		std::vector<unsigned char> vSpeed = util::shortToChars( speed );
		std::vector<unsigned char> vRadius = util::shortToChars( radius );
		if( 0 == radius ) {
			vRadius[0] = 0x80;
			vRadius[1] = 0x00;
		}
		std::vector<unsigned char> data( { opcodes::DRIVE, vSpeed[0], vSpeed[1], vRadius[0], vRadius[1] } );
		mSerialPort.send( data );
		mIsMoving = true;
	}

	void RoombaControl::motorsStop( const bool initControl ) {
		motorsMove( 0, 0, initControl );
		mIsMoving = false;
	}

	void RoombaControl::modeStart( ) {
		mSerialPort.send( opcodes::START );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}

	void RoombaControl::modeControl( ) {
		mSerialPort.send( opcodes::CONTROL );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}

	void RoombaControl::modeSafe( ) {
		mSerialPort.send( opcodes::SAFE );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}

	void RoombaControl::modeFull( ) {
		mSerialPort.send( opcodes::FULL );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}

	const SensorPacket1 RoombaControl::getSensorData1( ) {
		mSerialPort.send( {daw::roomba::opcodes::SENSORS, 1} );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
		return 	daw::roomba::SensorPacket1( mSerialPort.receive( 10 ) );
	}

	const SensorPacket3 RoombaControl::getSensorData3( ) {
		mSerialPort.send( {daw::roomba::opcodes::SENSORS, 3} );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
		return 	daw::roomba::SensorPacket3( mSerialPort.receive( 10 ) );
	}

	const bool& RoombaControl::isMoving( ) const {
		return mIsMoving;
	}
}}

