
#include <cstdint>
#include <stdexcept>
#include <wiringPi.h>
#include "gpio.h"

namespace daw {
	Gpio::Gpio( const uint8_t pin, const bool isReadMode, const bool manualPinMode ): mPin( pin ), mIsReadMode( isReadMode ), mManualPinMode( manualPinMode ) {
		if( -1 == wiringPiSetup( ) ) {
			throw std::runtime_error( "Error starting Gpio Library" );
		}
		SetMode( mIsReadMode );
	}

	void Gpio::SetMode( const bool read ) {
		mIsReadMode = read;
		pinMode( mPin, mIsReadMode ? 0 : 1 );
	}

	const bool Gpio::DigitalRead( ) {
		if( !IsInReadMode( ) ) {
			if( ManualPinMode( ) ) {
				throw std::runtime_error( "Gpio: Tried to read from a write pin" );
			} else {
				SetMode( true );
			}
		}
		return digitalRead( mPin );
	}

	void Gpio::DigitalWrite( const bool value ) {
		if( !IsInWriteMode( ) ) {
			if( ManualPinMode( ) ) {
				throw std::runtime_error( "Gpio: Tried to write to a read pin" );
			} else {
				SetMode( false );
			}
		}
		digitalWrite( mPin, value );
	}
	
	std::ostream& operator<<( std::ostream& stream, daw::Gpio& value ) {
		stream << value.DigitalRead( );
		return stream;
	}

	std::istream& operator>>( std::istream& stream, daw::Gpio& value ) {
		uint8_t writeValue = -1;
		stream >> writeValue;
		if( 0 != writeValue ) {
			writeValue = daw::Gpio::PinHigh;
		}
		value.DigitalWrite( writeValue );
		return stream;
	}
}


