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
#include <string>
#include <sstream>
#include <vector>
#include "roombasensors.h"

namespace {
	template<class T>
	const bool getBit( T value, size_t bit ) {
		return 0 != (value & (1 << bit) );
	}

	template<class T>
	const T setBit( const T& value, const size_t index, const bool isOn ) {
		if( isOn ) { 
			return value | (1 << index);	
		} else {
			T mask = ~(1 << index);
			return value & mask;
		}
	}
}

namespace daw { namespace roomba {
	SensorPacket1::SensorPacket1( const std::vector<unsigned char>& data, const size_t startByte ) {
		bumpAndWheelDrop = data[startByte + 0];
		wall = data[startByte + 1];
		cliffLeft = data[startByte + 2];
		cliffFrontLeft = data[startByte + 3];
		cliffFrontRight = data[startByte + 4];
		cliffRight = data[startByte + 5];
		virtualWall = data[startByte + 6];
		motorOverCurrents = data[startByte + 7];
		dirtDetectorLeft = data[startByte + 8];
		dirtDetectorRight = data[startByte + 9];
	}

	const std::string SensorPacket1::ToString( ) const {
		std::stringstream ss;
		ss << bumpAndWheelDrop << std::endl;
		ss << wall << std::endl;
		ss << "Cliff Left: " << static_cast<int>( cliffLeft ) << std::endl;
		ss << "Cliff Front Left: " << static_cast<int>( cliffFrontLeft ) << std::endl;
		ss << "Cliff Front Right: " << static_cast<int>( cliffFrontRight ) << std::endl;
		ss << "Cliff Right: " << static_cast<int>( cliffRight ) << std::endl;
		ss << "Virtual Wall: " << static_cast<int>( virtualWall ) << std::endl;
		ss << "Motor Over Currents: " << static_cast<int>( motorOverCurrents ) << std::endl;
		ss << "Dirt Detector Left: " << static_cast<int>( dirtDetectorLeft ) << std::endl;
		ss << "Dirt Detector Right: " << static_cast<int>( dirtDetectorRight ) << std::endl;
		return ss.str( );
	}
	
	SensorPacket3::SensorPacket3( const std::vector<unsigned char>& data, const size_t startByte ) {
		// TODO: validate parameters
		chargingState = data[startByte + 0];
		batteryVoltage = (data[startByte + 1] << 8) | data[startByte + 2];
		batteryCurrent = (data[startByte + 3] << 8) | data[startByte + 4];
		batteryTemperature = data[5];
		batteryCharge = (data[startByte + 6] << 8) | data[startByte + 7];
		batteryCapacity = (data[startByte + 8] << 8) | data[startByte + 9];
	}

	const std::string SensorPacket3::ToString( ) const {
		std::stringstream ss;
		ss << "Charging State: " << static_cast<int>( chargingState ) << std::endl;
		ss << "Battery Voltage: " << batteryVoltage << "mV" << std::endl;
		ss << "Battery Current: " << batteryCurrent << "mA" << std::endl;
		ss << "Battery Temperature: " << static_cast<int>( batteryTemperature ) << "C" << std::endl;
		ss << "Battery Charge: " << batteryCharge << "mAh" << std::endl;
		ss << "Battery Capacity: " << batteryCapacity << "mAh" << std::endl;
		return ss.str( );
	}

	SensorBumpAndWheelDrop::SensorBumpAndWheelDrop( const unsigned char data ) {
		wheelDropCaster = getBit( data, 4 );
		wheelDropLeft = getBit( data, 3 );
		wheelDropRight = getBit( data, 2 );
		bumpLeft = getBit( data, 1 );
		bumpRight = getBit( data, 0 );
	}

	SensorBumpAndWheelDrop& SensorBumpAndWheelDrop::operator=( const SensorBumpAndWheelDrop& value ) {
		if( this != &value ) {
			wheelDropCaster = value.wheelDropCaster;
			wheelDropLeft = value.wheelDropLeft;
			wheelDropRight = value.wheelDropRight;
			bumpLeft = value.bumpLeft;
			bumpRight = value.bumpRight;
		}
		return *this;
	}

	SensorBumpAndWheelDrop& SensorBumpAndWheelDrop::operator=( const unsigned char& data ) {
		SensorBumpAndWheelDrop tmp( data );
		std::swap( *this, tmp );
		return *this;
	}

	const std::string SensorBumpAndWheelDrop::ToString( ) const {
		std::stringstream ss;
		ss << "Wheel Drop Caster: " << wheelDropCaster << std::endl;
		ss << "Wheel Drop Left: " << wheelDropLeft << std::endl;
		ss << "Wheel Drop Right: " << wheelDropRight << std::endl;
		ss << "Bump Left: " << bumpLeft << std::endl;
		ss << "Bump Right: " << bumpRight << std::endl;
		return ss.str( );
	}

	SensorWall::SensorWall( const unsigned char data ) {
		wall = getBit( data, 0 );
	}

	SensorWall& SensorWall::operator=( const SensorWall& value ) {
		if( this != &value ) {
			wall = value.wall;
		}
		return *this;
	}

	SensorWall& SensorWall::operator=( const unsigned char& data ) {
		SensorWall tmp( data );
		std::swap( *this, tmp );
		return *this;
	}

	const std::string SensorWall::ToString( ) const {
		std::stringstream ss;
		ss << "Wall: " << wall << std::endl;
		return ss.str( );
	}
	
	const std::string SensorPacket2::ToString( ) const {
		return "";
	}
}}

std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorPacket1& value ) {
	return o << value.ToString( );
}

std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorPacket2& value ) {
	return o << value.ToString( );
}

std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorPacket3& value ) {
	return o << value.ToString( );
}

std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorWall& value ) {
	return o << value.ToString( );
}

std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorBumpAndWheelDrop& value ) {
	return o << value.ToString( );
}

