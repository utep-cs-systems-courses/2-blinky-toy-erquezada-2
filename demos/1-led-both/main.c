//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;         /*set gate for 65 bits for red and green led */
  P1OUT &= ~LED_RED;    // turning off red led
  P1OUT |= LED_GREEN;  // turning on green led
  
  or_sr(0x18);		/* CPU off, GIE on */
}
