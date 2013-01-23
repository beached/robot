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
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "roombacontrol.h"
#include "roombasensors.h"
#include "util.h"
#include "serialport.h"

namespace {
	const uint8_t bit7 = 1<<7;
}

namespace daw { namespace roomba {
	static void sendRoomba( const daw::SerialPort& arduino, const uint8_t value ) {
		arduino.send( { 32, 1 } );
		arduino.send( value );
	}

	static void sendRoomba( const daw::SerialPort& arduino, const std::vector<uint8_t>& data ) {
		arduino.send( { 32, data.size( ) } );
		arduino.send( data );
	}

	RoombaControl::RoombaControl( const std::string& arduinoPort ): mArduinoPort( arduinoPort, 115200 ), mIsMoving( false ), mLaserOn( false ) {
		std::cerr << "Connected to Roomba on " << roombaPort << " and Arduino on " << arduinoPort << std::endl;
		// Setup Arduino Connection
	}

	RoombaControl::~RoombaControl( ) {
		if( mArduinoPort.is_open( ) ) {
			mArduinoPort.close( );
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
		std::vector<uint8_t> vSpeed = util::shortToChars( speed );
		std::vector<uint8_t> vRadius = util::shortToChars( radius );
		if( 0 == radius ) {
			vRadius[0] = 0x80;
			vRadius[1] = 0x00;
		}
		std::vector<uint8_t> data( { opcodes::DRIVE, vSpeed[0], vSpeed[1], vRadius[0], vRadius[1] } );
		sendRoomba( mArduinoPort, data );
		mIsMoving = true;
	}

	void RoombaControl::motorsStop( const bool initControl ) {
		motorsMove( 0, 0, initControl );
		mIsMoving = false;
	}

	void RoombaControl::modeStart( ) {
		sendRoomba( mArduinoPort, opcodes::START );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}

	void RoombaControl::modeControl( ) {
		sendRoomba( mArduinoPort, opcodes::CONTROL );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}

	void RoombaControl::modeSafe( ) {
		sendRoomba( mArduinoPort, opcodes::SAFE );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}

	void RoombaControl::modeFull( ) {
		sendRoomba( mArduinoPort, opcodes::FULL );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}

	void RoombaControl::cleanStart( ) {
		std::vector<uint8_t> data( { opcodes::MOTORS, 0x07 } );
		sendRoomba( mArduinoPort, data );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}
	void RoombaControl::cleanStop( ) {
		std::vector<uint8_t> data( { opcodes::MOTORS, 0 } );
		sendRoomba( mArduinoPort, data );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}

	const SensorPackets RoombaControl::getSensorData( ) {
		arduinoPort.send( 33 );
		return 	daw::roomba::SensorPackets( mRoombaPort.receive( 26 ) );
	}

	const bool& RoombaControl::isMoving( ) const {
		return mIsMoving;
	}

	void RoombaControl::laserStart( ) {
		mArduinoPort.send( { 0x40, bit7 } );		// 64 is CMD_WRITE, next byte is values for pins D0..D7
		mLaserOn = true;	
	}

	void RoombaControl::laserStop( ) {
		mArduinoPort.send( { 0x40, 0x00 } );		// 64 is CMD_WRITE
		mLaserOn = false;	
	}

	void RoombaControl::laserToggle( ) {
		mLaserOn ? laserStop( ) : laserStart( );
	}

}}

