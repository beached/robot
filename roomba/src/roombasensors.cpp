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
#include <stdexcept>
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
	SensorPacket1::SensorPacket1( const std::vector<uint8_t>& data, const size_t startByte ):
		bumpAndWheelDrop( data[startByte + 0] ),
		wall( data[startByte + 1] ),
		cliffLeft( data[startByte + 2] ),
		cliffFrontLeft( data[startByte + 3] ),
		cliffFrontRight( data[startByte + 4] ),
		cliffRight( data[startByte + 5] ),
		virtualWall( data[startByte + 6] ),
		motorOverCurrents( data[startByte + 7] ),
		dirtDetectorLeft( data[startByte + 8] ),
		dirtDetectorRight( data[startByte + 9] ) { }

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

	SensorPacket2::SensorPacket2( const std::vector<uint8_t>& data, const size_t startByte ): 
		remoteControlCommand( data[startByte + 0] ),
		buttons( data[startByte + 1] ),
		distance( (data[startByte + 2] << 8) | data[startByte + 3] ),
		angle( (data[startByte + 4] << 8) | data[startByte + 5] ) { }

	const std::string SensorPacket2::ToString( ) const {
		std::stringstream ss;
		ss << "Remote Control Command: " << remoteControlCommand;
		ss << buttons;
		ss << "Distance: " << distance;
		ss << "Angle: " << angle;
		return ss.str( );
	}

	SensorPacket3::SensorPacket3( const std::vector<uint8_t>& data, const size_t startByte ):
		// TODO: validate parameters
		chargingState( data[startByte + 0] ),
		batteryVoltage( (data[startByte + 1] << 8) | data[startByte + 2] ),
		batteryCurrent( (data[startByte + 3] << 8) | data[startByte + 4] ),
		batteryTemperature( data[5] ),
		batteryCharge( (data[startByte + 6] << 8) | data[startByte + 7] ),
		batteryCapacity( (data[startByte + 8] << 8) | data[startByte + 9] ) { }

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

	SensorBumpAndWheelDrop::SensorBumpAndWheelDrop( const uint8_t data ):
		wheelDropCaster( getBit( data, 4 ) ),
		wheelDropLeft( getBit( data, 3 ) ),
		wheelDropRight( getBit( data, 2 ) ),
		bumpLeft( getBit( data, 1 ) ),
		bumpRight( getBit( data, 0 ) ) { }

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

	SensorBumpAndWheelDrop& SensorBumpAndWheelDrop::operator=( const uint8_t& data ) {
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

	SensorWall::SensorWall( const uint8_t data ): wall( getBit( data, 0 ) ) { }

	SensorWall& SensorWall::operator=( const SensorWall& value ) {
		if( this != &value ) {
			wall = value.wall;
		}
		return *this;
	}

	SensorWall& SensorWall::operator=( const uint8_t& data ) {
		SensorWall tmp( data );
		std::swap( *this, tmp );
		return *this;
	}

	const std::string SensorWall::ToString( ) const {
		std::stringstream ss;
		ss << "Wall: " << wall << std::endl;
		return ss.str( );
	}

	SensorButtons::SensorButtons( const uint8_t data ):
		max( getBit( data, 0 ) ),
		clean( getBit( data, 1 ) ),
		spot( getBit( data, 2 ) ),
		power( getBit( data, 3 ) ) { }

	SensorButtons& SensorButtons::operator=( const SensorButtons& value ) {
		if( this != &value ) {
			max = value.max;
			clean = value.clean;
			spot = value.spot;
			power = value.power;
		}
		return *this;
	}

	SensorButtons& SensorButtons::operator=( const uint8_t& data ) {
		SensorButtons tmp( data );
		std::swap( *this, tmp );
		return *this;
	}

	const std::string SensorButtons::ToString( ) const {
		std::stringstream ss;
		ss << "Max: " << max << std::endl;
		ss << "Clean: " << clean  << std::endl;
		ss << "Spot: " << spot << std::endl;
		ss << "Power: " << power  << std::endl;
		return ss.str( );
	}

	SensorPackets::SensorPackets( const std::vector<uint8_t>& data ):sensorPacket1( data, 0 ), sensorPacket2( data, 10 ), sensorPacket3( data, 16 ) {
		if( data.size( ) != 26 ) {
			std::stringstream ss;
			ss << "Data packet for Sensor data must be 26 bytes in length, it was " << data.size( );
			throw std::runtime_error( ss.str( ) );
		}
	}

	const std::string SensorPackets::ToString( ) const {
		std::stringstream ss;
		ss << sensorPacket1;
		ss << sensorPacket2;
		ss << sensorPacket3;
		return ss.str( );
	}
}}

std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorPackets& value ) {
	return o << value.ToString( );
}

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

std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorButtons& value ) {
	return o << value.ToString( );
}

std::ostream& operator<<( std::ostream& o, const daw::roomba::SensorBumpAndWheelDrop& value ) {
	return o << value.ToString( );
}

