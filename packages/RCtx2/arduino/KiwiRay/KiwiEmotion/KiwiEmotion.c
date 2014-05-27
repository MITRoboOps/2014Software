#include <htc.h>
#include <stdint.h>
#include <stdbool.h>

//#define MUX_OE RC2
#define DSP_SD RC2
#define DSP_LE RC3
#define DSP_CK RC4
#define DSP_OE RC5
#define MUX_ST( n ) PORTA = ( PORTA & 0xF8 ) | n

#define DSP_LE_STROBE DSP_LE = 1; asm( "NOP" ); asm( "NOP" ); DSP_LE = 0; asm( "NOP" ); asm( "NOP" );
#define DSP_CK_STROBE DSP_CK = 1; asm( "NOP" ); asm( "NOP" ); DSP_CK = 0;
#define DSP_ENABLE    DSP_OE = 0; asm( "NOP" ); asm( "NOP" );
#define DSP_DISABLE   DSP_OE = 1; asm( "NOP" ); asm( "NOP" );

#define I2C_ADDR 0x33

unsigned char porta_mem;
unsigned char i2c_count, i2c_bytes = 0, i2c_data;
unsigned char i2c_packet[ 32 ];
unsigned char i2c_ptr;

interrupt isr() {
	uint8_t data;
	static uint8_t last = 0;
	if( SSP1IF ) {
		// Clear interrupt
		SSP1IF = 0;
		if( BF ) {
			// Check if read
			if( !R_nW ) {
				// Read data
				data = SSPBUF;
				// Check if data/address received
				if( D_nA ) {
					// Handle incoming data
					if( i2c_bytes < 24 ) {
						i2c_packet[ i2c_bytes ] = data;
						i2c_bytes++;
					}
				}
			} else {
				// Check for address
				if( !D_nA ) {
					// Read address
					data = SSPBUF;
				}
				if( ( !D_nA ) || ( !ACKSTAT ) ) {
					// Write data
					SSPBUF = i2c_bytes;
				}
			}
		} else {
			// Start
			i2c_count = 0;
			i2c_bytes = 0;
		}
		// Release clock
		CKP = 1;
	}
}

void setup( void ) {
	unsigned char n;
	OSCCON     = 0b01110000; // Set clock to 32MHz
	ANSELA     = 0b00000000;
	ANSELC     = 0b00000000;
	PORTA      = 0b00000000;
	PORTC      = 0b00000000;
	TRISA      = 0b11111000;
	TRISC      = 0b11000011;

	WPUA       = 0b00000000; // Disable weak pull-up on RA0..5
	WPUC       = 0b00000011; // Enable  weak pull-up on RC0..1
	OPTION_REG = 0b01111111; // Enable WPUEN/weak pull ups
	INLVLA     = 0b00111111; // Enable CMOS scmitt-triggers on RA0..5
	INLVLC     = 0b00111111; // Enable CMOS scmitt-triggers on RC0..5

	for( n = 0; n < 32; n++ ) {
		i2c_packet[ n ] = 0;
	}
	i2c_packet[ 10 ] = 0b00011000;
	i2c_packet[ 13 ] = 0b00011000;

	SSP1IE     = 1;			 // Enable i²c interrupts
	SSPADD     = I2C_ADDR << 1; // Set slave address
	SSPCON3    = 0b00110000; // Set BOEN/disable overflow check
	SSPCON2    = 0b00000001; // Set SEN/clock stretching
	SSPCON1    = 0b00111110; // Set SSPEN/port enabled, CKP/clock floating, SSPM=0110/i²c 7-bit slave
	INTCON     = 0b11000000; // Enable PEIE/peripheral and GIE/global interrupts

}

void main( void ) {
	unsigned char r, g, b, q;
	unsigned char row = 0, rowix;
	unsigned char n;
	unsigned long delay;
	setup();
	while( 1 ) {
		DSP_DISABLE;
		DSP_LE_STROBE;
		MUX_ST( row );
		DSP_ENABLE;
		row = ( row + 1 ) & 7;
		rowix = row * 3;
		q = i2c_packet[ rowix + 0 ];
		for( n = 0; n < 8; n++ ) {
			DSP_SD = 0 != ( q & 0x80 );
			q <<= 1;
			DSP_CK_STROBE;
		}
		q = i2c_packet[ rowix + 1 ];
		for( n = 0; n < 8; n++ ) {
			DSP_SD = 0 != ( q & 0x80 );
			q <<= 1;
			DSP_CK_STROBE;
		}
		q = i2c_packet[ rowix + 2 ];
		for( n = 0; n < 8; n++ ) {
			DSP_SD = 0 != ( q & 0x80 );
			q <<= 1;
			DSP_CK_STROBE;
		}
	}
}