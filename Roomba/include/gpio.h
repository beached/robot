#ifndef HAS_DAWGPOIO_HEADER
#define HAS_DAWGPOIO_HEADER

#include <cstdint>
#include <iostream>

namespace daw {
	class Gpio {
	private:
		const uint8_t mPin;
		bool mIsReadMode;	// Only 0=INPUT, 1=OUTPUT
		const bool mManualPinMode;
	public:
		static const uint8_t PinLow = 0;
		static const uint8_t PinHigh = 1;
		Gpio( const uint8_t pin, bool isReadMode = true, const bool manualPinMode = false );
		void SetMode( const bool read );

		const bool IsInReadMode( ) const {
			return mIsReadMode;
		}

		const bool IsInWriteMode( ) const {
			return !mIsReadMode;
		}

		const bool ManualPinMode( ) const {
			return mManualPinMode;
		}

		const uint8_t Pin( ) const {
			return mPin;
		}

		const bool DigitalRead( );
		void DigitalWrite( const bool value );
		friend std::ostream& operator<<( std::ostream& stream, daw::Gpio& value );
		friend std::istream& operator>>( std::istream& stream, daw::Gpio& value );
	};
}


#endif	//HAS_DAWGPOIO_HEADER

