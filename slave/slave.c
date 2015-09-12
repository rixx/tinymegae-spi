#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <stdint.h>

void led_control(uint16_t leds);

int main(void)
{
    // Setup code
    clock_prescale_set(clock_div_1);

    // T/C setup code, see also data sheet section 14.10 page 121 ff
//    TCCR1A = 0;                          //WGM{0,1,3} = 0, WGM2 = 1
//    TCCR1C = 0;
//                                         // 16MHz / 1024 = 15625 Ticks per second
//    OCR1A = 1024;                        // 15625/2
//    TIMSK1 = 1 << OCIE1A;                // enable OCIE1A
//    TIFR1 = 0xff;                        // reset stuff
//
//                                         // WGM{0,1,3} = 0, WGM2 = 1
//    TCCR1B = (1 << WGM12) | (5 << CS10); // clock select CLK/1024=0b101, starts le clock
//
    DDRD = 7;                              // controls the LEDs/shift registers.
                                           // PD0 is data, PD1 is clock, PD2 is latch

    // SPI setup, section 17, page 179ff
    PRR0 &= ~(1 << PRSPI);                 // enable SPI by writing 0 to the Power Reduction SPI bit (p179)
    DDRB = (1 << DDB3);                    // set MISO (PB3) to output, all others to input

    SPCR = (1 << SPE);                     // enable SPI
    //SPCR |= (1 << SPIE);                   // enable SPI interrupt

    // enable interrupts
    //sei();

    while (1) {
        while (!(SPSR & (1<<SPIF)));
        led_control((uint16_t)SPDR);
        _delay_ms(400);
    }
}

void led_control(uint16_t leds){
    uint8_t i, pin;
    for (i = 0; i < 16; i++) {
       pin = !(!(leds & (1 << i)));
       PORTD = (pin << PD0);

       PORTD |= (1 << PD1);
       PORTD &= ~(1 << PD1);

    }

    PORTD |= (1 << PD2);
    PORTD &= ~(1 << PD2);
}


