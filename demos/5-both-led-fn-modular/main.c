//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  // P1OUT &= ~LED_GREEN;
  // P1OUT |= LED_RED;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);		/* CPU off, GIE on */
}

void greenControl(int on)
{
  if (on) {
    P1OUT |= LED_GREEN;
  } else {
    P1OUT &= ~LED_GREEN;
  }
}

void redControl(int on)
{
  if (on) {
    P1OUT |= LED_RED;
  } else {
    P1OUT &= ~LED_RED;
  }
}

// blink state machine
static int bright_BL = 5;   // start off brighter blink limit
static int dim_BL = 7;      // start off dimmer blink limit
static int bright_counter = 0;   // blink state for bright-to-dim
static int dim_counter    = 0;   // blink state for dim-to-bright
void greenBlinkUpdate() // called every 1/250s to blink with duty cycle 1/blinkLimit
{
  bright_counter ++;
  if (bright_counter >= bright_BL) {
    bright_counter = 0;
    greenControl(1);
  } else
    greenControl(0);
}

void redBlinkUpdate()
{
  dim_counter ++;
  if (dim_counter >= dim_BL) {
    dim_counter = 0;
    redControl(1);
  } else
    redControl(0);
}

void oncePerSecond() // repeatedly start bright and gradually lower duty cycle, one step/sec
{
  bright_BL ++;  // reduce duty cycle
  if (bright_BL >= 8)  // but don't let duty cycle go below 1/7.
    bright_BL = 0;

  dim_BL --;
  if (dim_BL < 1)
    dim_BL = 7;
}

void secondUpdate()  // called every 1/250 sec to call oncePerSecond once per second
{
  static int secondCount = 0; // state variable representing repeating time 0…1s
  secondCount ++;
  if (secondCount >= 250) { // once each second
    secondCount = 0;
    oncePerSecond();
  }
}

void timeAdvStateMachines() // called every 1/250 sec
{
  greenBlinkUpdate();
  redBlinkUpdate();
  secondUpdate();
}


void __interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  // handle blinking   
  timeAdvStateMachines();
} 
