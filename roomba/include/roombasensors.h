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
#include <cstdint>

namespace daw { namespace roomba {
	class SensorBumpAndWheelDrop {
	public:
		bool wheelDropCaster;
		bool wheelDropLeft;
		bool wheelDropRight;
		bool bumpLeft;
		bool bumpRight;
		SensorBumpAndWheelDrop( ) { };
		SensorBumpAndWheelDrop( const uint8_t data );
		SensorBumpAndWheelDrop& operator=( const SensorBumpAndWheelDrop& value );
		SensorBumpAndWheelDrop& operator=( const uint8_t& data );
		const std::string ToString( ) const;
	};

	class SensorWall {
	public:
		bool wall;
		SensorWall( ) { };
		SensorWall( const uint8_t data );
		SensorWall& operator=( const SensorWall& value );
		SensorWall& operator=( const uint8_t& data );
		const std::string ToString( ) const;
	};

	class SensorButtons {
	public:
		bool max;
		bool clean;
		bool spot;
		bool power;
		SensorButtons( const uint8_t data );
		SensorButtons& operator=( const SensorButtons& value );
		SensorButtons& operator=( const uint8_t& data );
		const std::string ToString( ) const;
	};

	class SensorPacket1 {
	public:
		SensorBumpAndWheelDrop bumpAndWheelDrop;
		SensorWall wall;
		uint8_t cliffLeft;
		uint8_t cliffFrontLeft;
		uint8_t cliffFrontRight;
		uint8_t cliffRight;
		uint8_t virtualWall;
		uint8_t motorOverCurrents;
		uint8_t dirtDetectorLeft;
		uint8_t dirtDetectorRight;
		SensorPacket1( const std::vector<uint8_t>& data, const size_t startByte = 0 );
		const std::string ToString( ) const;
	};

	class SensorPacket2 {
	public:
		uint8_t remoteControlCommand;
		SensorButtons buttons;
		int16_t distance;
		int16_t angle;
		SensorPacket2( const std::vector<uint8_t>& data, const size_t startByte = 0 );
		const std::string ToString( ) const;
	};

	class SensorPacket3 {
	public:
		uint8_t chargingState;	// 0-not charging, 1-charging recovery, 2-charging, 3-trickle charging, 4-waiting, 5-charging error
		uint16_t batteryVoltage;	// 0 to 65535mV
		int16_t batteryCurrent;	// -32767 to 32767mA
		int8_t batteryTemperature;	// -128 to 127 Celcius
		uint16_t batteryCharge;	// 0 to 65535mAh
		uint16_t batteryCapacity;	// 0 to 65535mAh
		SensorPacket3( const std::vector<uint8_t>& data, const size_t startByte = 0 );
		const std::string ToString( ) const;
	};

	class SensorPackets {
	public:
		SensorPacket1 sensorPacket1;
		SensorPacket2 sensorPacket2;
		SensorPacket3 sensorPacket3;
		SensorPackets( const std::vector<uint8_t>& data );
		const std::string ToString( ) const;
	};
}}

std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorPacket1& value );
std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorPacket2& value );
std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorPacket3& value );
std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorPackets& value );
std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorWall& value );
std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorButtons& value );
std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorBumpAndWheelDrop& value );

#endif

