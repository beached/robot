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

#ifndef HAS_ROOMBACONTROL_HEADER
#define HAS_ROOMBACONTROL_HEADER

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <cstdint>

#include "roombasensors.h"
#include "serialport.h"

namespace daw { namespace roomba {
	class opcodes {
	public:
		const static uint8_t START = 128;
		const static uint8_t BAUD = 129;
		const static uint8_t CONTROL = 130;
		const static uint8_t SAFE = 131;
		const static uint8_t FULL = 132;
		const static uint8_t POWER = 133;
		const static uint8_t SPOT = 134;
		const static uint8_t CLEAN = 135;
		const static uint8_t MAX = 136;
		const static uint8_t DRIVE = 137;
		const static uint8_t MOTORS = 138;
		const static uint8_t LEDS = 139;
		const static uint8_t SONG = 140;
		const static uint8_t PLAY = 141;
		const static uint8_t SENSORS = 142;
		const static uint8_t DOCK = 143;
	};
	
	class RoombaControl {
	private:
		daw::SerialPort mArduinoPort;
		bool mIsMoving;
		bool mLaserOn;
	public:
		RoombaControl( const std::string& arduinoPort );
		~RoombaControl( );
		void motorsMove( const short speed, short radius, const bool initControl = true );
		void motorsStop( const bool initControl = true );

		const std::vector<uint8_t> receive( size_t count );	
		void send( const uint8_t value );
		void send( const std::vector<uint8_t>& values, size_t count = 0 );

		void modeStart( );
		void modeControl( );
		void modeSafe( );
		void modeFull( );
		void cleanStart( );
		void cleanStop( );
		const SensorPackets getSensorData( );
		const bool& isMoving( ) const;
		void laserStart( );
		void laserStop( );
		void laserToggle( );
	};
}}

#endif

