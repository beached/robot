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

#ifndef HAS_ROOMBASENSORS_HEADER
#define HAS_ROOMBASENSORS_HEADER

#include <vector>
#include <string>

namespace daw { namespace roomba {
	class SensorBumpAndWheelDrop {
	public:
		bool wheelDropCaster;
		bool wheelDropLeft;
		bool wheelDropRight;
		bool bumpLeft;
		bool bumpRight;
		SensorBumpAndWheelDrop( ) { };
		SensorBumpAndWheelDrop( const unsigned char data );
		SensorBumpAndWheelDrop& operator=( const SensorBumpAndWheelDrop& value );
		SensorBumpAndWheelDrop& operator=( const unsigned char& data );
		const std::string ToString( ) const;
	};

	class SensorWall {
	public:
		bool wall;
		SensorWall( ) { };
		SensorWall( const unsigned char data );
		SensorWall& operator=( const SensorWall& value );
		SensorWall& operator=( const unsigned char& data );
		const std::string ToString( ) const;
	};


	class SensorPacket1 {
	public:
		SensorBumpAndWheelDrop bumpAndWheelDrop;
		SensorWall wall;
		unsigned char cliffLeft;
		unsigned char cliffFrontLeft;
		unsigned char cliffFrontRight;
		unsigned char cliffRight;
		unsigned char virtualWall;
		unsigned char motorOverCurrents;
		unsigned char dirtDetectorLeft;
		unsigned char dirtDetectorRight;
		SensorPacket1( const std::vector<unsigned char>& data, const size_t startByte = 0 );
		const std::string ToString( ) const;
	};

	class SensorPacket2 {
	public:
		const std::string ToString( ) const;
	};

	class SensorPacket3 {
	public:
		unsigned char chargingState;	// 0-not charging, 1-charging recovery, 2-charging, 3-trickle charging, 4-waiting, 5-charging error
		unsigned short batteryVoltage;	// 0 to 65535mV
		signed short batteryCurrent;	// -32767 to 32767mA
		signed char batteryTemperature;	// -128 to 127 Celcius
		unsigned short batteryCharge;	// 0 to 65535mAh
		unsigned short batteryCapacity;	// 0 to 65535mAh
		SensorPacket3( const std::vector<unsigned char>& data, const size_t startByte = 0 );
		const std::string ToString( ) const;
	};
}}

std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorPacket1& value );
std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorPacket2& value );
std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorPacket3& value );
std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorWall& value );
std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorBumpAndWheelDrop& value );

#endif

