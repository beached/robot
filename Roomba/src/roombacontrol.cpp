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

namespace {
	const uint8_t bit7 = 1<<7;
}

namespace daw { namespace roomba {

	RoombaControl::RoombaControl( const std::string& roombaPort, const std::string& arduinoPort ): mRoombaPort( roombaPort ), mArduinoPort( arduinoPort, 115200 ), mIsMoving( false ), mLaserOn( false ) {
		// Setup Arduino Connection
//		mArduinoPort.send( { 0xFF, 0x40 } );	// 255 is CMD_SETMASKD0, next byte is the mask, 64 sets bit 7 so that D7 is enabled
	}

	RoombaControl::~RoombaControl( ) {
		if( mRoombaPort.is_open( ) ) {
			mRoombaPort.close( );
		}
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
		std::vector<unsigned char> vSpeed = util::shortToChars( speed );
		std::vector<unsigned char> vRadius = util::shortToChars( radius );
		if( 0 == radius ) {
			vRadius[0] = 0x80;
			vRadius[1] = 0x00;
		}
		std::vector<unsigned char> data( { opcodes::DRIVE, vSpeed[0], vSpeed[1], vRadius[0], vRadius[1] } );
		mRoombaPort.send( data );
		mIsMoving = true;
	}

	void RoombaControl::motorsStop( const bool initControl ) {
		motorsMove( 0, 0, initControl );
		mIsMoving = false;
	}

	void RoombaControl::modeStart( ) {
		mRoombaPort.send( opcodes::START );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}

	void RoombaControl::modeControl( ) {
		mRoombaPort.send( opcodes::CONTROL );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}

	void RoombaControl::modeSafe( ) {
		mRoombaPort.send( opcodes::SAFE );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}

	void RoombaControl::modeFull( ) {
		mRoombaPort.send( opcodes::FULL );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}

	void RoombaControl::cleanStart( ) {
		std::vector<unsigned char> data( { opcodes::MOTORS, 0x07 } );
		mRoombaPort.send( data );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}
	void RoombaControl::cleanStop( ) {
		std::vector<unsigned char> data( { opcodes::MOTORS, 0 } );
		mRoombaPort.send( data );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
	}

	const SensorPacket1 RoombaControl::getSensorData1( ) {
		mRoombaPort.send( {daw::roomba::opcodes::SENSORS, 1} );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
		return 	daw::roomba::SensorPacket1( mRoombaPort.receive( 10 ) );
	}

	const SensorPacket3 RoombaControl::getSensorData3( ) {
		mRoombaPort.send( {daw::roomba::opcodes::SENSORS, 3} );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
		return 	daw::roomba::SensorPacket3( mRoombaPort.receive( 10 ) );
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

