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
#include <boost/thread.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "roombacontrol.h"
#include "roombasensors.h"

int main( int argc, char** argv ) {
	daw::roomba::RoombaControl rc( argv[1], argv[2] );
	std::stringstream ss;
	ss << argv[3];
	int speed = 0;
	ss >> speed;
	ss.clear( );
	ss << argv[4];
	int radius = 0;
	ss >> radius;
	int duration = 0;
	ss.clear( );
	ss << argv[5];
	ss >> duration;
	rc.modeStart( );
	boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
// Move
	if( !(0 == speed && 0 == radius) ) {
		rc.modeControl( );
		boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
		std::cout << "Moving at " << speed << "mm/s with a turn radius of " << radius << "mm for " << duration << "s" << std::endl;
		rc.motorsMove( speed, radius );
		boost::this_thread::sleep( boost::posix_time::seconds( duration ) );
		rc.motorsStop( );
	}
// Music
//	rc.send( {0x8C,0x01,13,107,0x20,105,0x20,103,0x20,105,0x20,107,0x20,107,0x20,107,0x20,105,0x20,105,0x20,105,0x20,110,0x20,110,0x20,110,0x20} );
//	rc.send( {0x8D,0x01 } );

	daw::roomba::SensorPacket1 sensor1 = rc.getSensorData1( );
	boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );
	std::cout << sensor1.ToString( ) << std::endl;

	daw::roomba::SensorPacket3 sensor3 = rc.getSensorData3( );
	boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );
	std::cout << sensor3.ToString( ) << std::endl;
	return EXIT_SUCCESS;
}

