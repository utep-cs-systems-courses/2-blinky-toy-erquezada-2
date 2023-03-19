//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  or_sr(0x18);		/* CPU off, GIE on */
}

int counter = 0;   // for seconds
char led = 0;     // to see if led was on or not
void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{

  counter++;  
  
  if (counter == 250){  // can modify to get fast or slow blinking lights
    counter = 0;
    led = !led;              //  either a 1  or a 0
    if (led){
      P1OUT |= LED_RED;      //  turn on leds
      P1OUT |= LED_GREEN;    
    }else{
      P1OUT &= ~LED_RED;  // turn off leds
      P1OUT &= ~LED_GREEN; 
    }
  }
} 
