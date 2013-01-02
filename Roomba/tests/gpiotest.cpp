#include "include/gpio.h"
#include <cstdlib>


int main( int argc, char** argv ) {
	if( argc < 1 ) {
		daw::Gpio p17( 0, true );
		std::cout << p17 << std::endl;
	} else {
		daw::Gpio p17( 0, false );
		p17.DigitalWrite( (argv[1][0] == '0' ? daw::Gpio::PinLow : daw::Gpio::PinHigh) );
	}
	return EXIT_SUCCESS;
}

