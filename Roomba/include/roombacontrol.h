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

#include "roombasensors.h"
#include "serialport.h"
#include "gpio.h"

namespace daw { namespace roomba {
	class opcodes {
	public:
		const static unsigned char START = 128;
		const static unsigned char BAUD = 129;
		const static unsigned char CONTROL = 130;
		const static unsigned char SAFE = 131;
		const static unsigned char FULL = 132;
		const static unsigned char POWER = 133;
		const static unsigned char SPOT = 134;
		const static unsigned char CLEAN = 135;
		const static unsigned char MAX = 136;
		const static unsigned char DRIVE = 137;
		const static unsigned char MOTORS = 138;
		const static unsigned char LEDS = 139;
		const static unsigned char SONG = 140;
		const static unsigned char PLAY = 141;
		const static unsigned char SENSORS = 142;
		const static unsigned char DOCK = 143;
	};
	
	class RoombaControl {
	private:
		daw::SerialPort mSerialPort;
		bool mIsMoving;
		daw::Gpio mGpio;
	public:
		RoombaControl( const std::string& device );
		~RoombaControl( );
		void motorsMove( const short speed, short radius, const bool initControl = true );
		void motorsStop( const bool initControl = true );

		const std::vector<unsigned char> receive( size_t count );	
		void send( const unsigned char value );
		void send( const std::vector<unsigned char>& values, size_t count = 0 );

		void modeStart( );
		void modeControl( );
		void modeSafe( );
		void modeFull( );
		void cleanStart( );
		void cleanStop( );
		void explore( );
		const SensorPacket1 getSensorData1( );
		const SensorPacket3 getSensorData3( );
		const bool& isMoving( ) const;
	};
}}

#endif

