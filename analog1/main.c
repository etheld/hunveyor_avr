#define SCL_CLOCK 100000L
#define I2C_READ 1
#define I2C_WRITE 0
#define USART_BAUDRATE 9600  // serial connection speed is 9600 bps

#include <avr/io.h>
#include <compat/twi.h>
#include <avr/interrupt.h>

#include "TWI_slave.h"
#include <util/twi.h>

#define BDIR	0x00 	//
#define CDIR	0x00    // port directions
#define DDIR	0x72	//

#define BINIT	0x00 	//
#define CINIT	0x00	// port initial states
#define DINIT	0x00	//
#define SLAVE_ADDR 0x07

void init_i2c() {
	DDRB = BDIR;
	DDRC = CDIR;
	DDRD = DDIR;
	PORTB = BINIT;
	PORTC = CINIT;
	PORTD = DINIT;

	TWI_Slave_Initialise( (SLAVE_ADDR<<1) | (FALSE<<TWI_GEN_BIT) ); // initialize TWI interface with slave_addr
}

int main(void)
{

	init_i2c();
	unsigned char messagebuf[8];
	//I2C_init(0x4);
	sei(); // allow interrupts
	TWI_Start_Transceiver();
	do {
		if (!TWI_Transceiver_Busy() && TWI_statusReg.lastTransOK && TWI_statusReg.RxDataInBuf) {
			TWI_Get_Data_From_Transceiver(messagebuf, 2);
			if (messagebuf[0] == 0x17) {
				messagebuf[0] = 0xAA;
				TWI_Start_Transceiver_With_Data(messagebuf,1);
			}
			if (! TWI_Transceiver_Busy()) {
				TWI_Start_Transceiver();
			}
		}
	} while(1);
// Set Port D pins as all outputs
//DDRD = 0xff;
// Set all Port D pins as HIGH
//PORTD = 0x00;
return 1;
}
