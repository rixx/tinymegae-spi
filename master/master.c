#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <stdint.h>

volatile uint8_t DIRECTION = 0;
volatile uint16_t LARSON = 1;
void shift(void);

int main(void)
{
    // Setup code
    clock_prescale_set(clock_div_1);

    // SPI setup, section 17, page 179ff
    PRR0 &= ~(1 << PRSPI);                 // enable SPI by writing 0 to the Power Reduction SPI bit (p179)
    DDRB = (1<<DDB0)|(1<<DDB1)|(1<<DDB2);            // set SCK (PB1) and MOSI(PB2) to output, all others to input

    SPCR = (1 << SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);                     // enable SPI
    //SPCR |= (1 << SPIE);                   // enable SPI interrupt
    //SPCR |= (1 << MSTR);                   // set to slave SPI mode
    //SPCR |= (1 << SPR1) | (1 << SPR0);     // set clock speed to f_osc/128

    // enable interrupts
    //sei();

    while (1) {
       SPCR |= (1<<MSTR);
       SPDR = 0x5;
       while (!(SPSR & (1<<SPIF)));
       _delay_ms(500);

       SPCR |= (1<<MSTR);
       SPDR = 0x2;
       while (!(SPSR & (1<<SPIF)));
       _delay_ms(500);

    }
}

