#include <msp430.h>

int getS1State();
int getS2State();
void delay(int value);

const int DELAY = 100;

int s2_press_count, s1_press_count  = 0;

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

  P7DIR |= BIT7;  // set pin 7 to write
  P7SEL |= BIT7;  // set pin 7 to work with periphery

  P1DIR &= ~BIT7; // s1 dir for reading
  P2DIR &= ~BIT2; // s2 dir for reading

  P1OUT |= BIT7;  // set inactive signal level for button s1
  P2OUT |= BIT2;  // set inactive signal level for button s2

  P1REN |= BIT7;  // connect s1 to vcc resistor
  P2REN |= BIT2;  // connect s2 to vcc resistor

  P1IES |= BIT7;  // process interrupt on signal's front
  P1IE |= BIT7;   // allow interrupts for s1
  P1IFG &= ~BIT7; // reset interrupt flag for s1

  P2IES |= BIT2;  // process interrupt on signal's front
  P2IE |= BIT2;   // allow interrupts for s2
  P2IFG &= ~BIT2; // reset interrupt flag for s2

  __bis_SR_register(BIT3); // allow masked interrupts

  UCSCTL1 &= 0; // null the register to reset default values
  UCSCTL2 &= 0; // null the register to reset default values
  UCSCTL3 &= 0; // null the register to reset default values
  UCSCTL4 &= 0; // null the register to reset default values
  UCSCTL5 &= 0; // null the register to reset default values

  UCSCTL4 &= 0;
  UCSCTL4 |= SELM__DCOCLKDIV;   // set ref for MCLK to DCOCLKDIV

  UCSCTL1 |= DCORSEL_0; 	// set lowest possible frequency range

  UCSCTL3 &= 0;
  UCSCTL3 |= SELREF__REFOCLK; 	// set FLL ref clk to REFOCLK
  UCSCTL3 |= FLLREFDIV__8;	// set FLL ref div to 8

  UCSCTL2 &= 0;
  UCSCTL2 |= FLLD__16; 	  // set FLLD to 16
  UCSCTL2 |= FLLN1;	  // set FLLN
  UCSCTL2 &= ~FLLN0;	  // to 2

  UCSCTL5 |= DIVM__4;	  // set MCLK frequency divider to 4

  __no_operation();
	
	return 0;
}


#pragma vector = PORT2_VECTOR
__interrupt void handleS2StateChange()
{
	// S2 was pressed
  if (!(s2_press_count % 2))
  {
    UCSCTL4 &= 0;   // null the register to reset default values
    UCSCTL5 &= 0;   // null the register to reset default values

    UCSCTL4 |= SELM__REFOCLK; // set ref for MCLK to REFOCLK
    UCSCTL5 |= DIVM__16;      // set MCLK frequency divider to 16

    s2_press_count++;
  }
  else
  {
    UCSCTL4 &= 0; // null the register to reset default values
    UCSCTL5 &= 0; // null the register to reset default values

    UCSCTL4 |= SELM__DCOCLKDIV; // set ref for MCLK to DCOCLKDIV
    UCSCTL5 |= DIVM__4;         // set MCLK frequency divider to 4

    s2_press_count--;
  }

  delay(DELAY);

  P2IFG &= ~BIT2; // reset interrupt flag for s2

	return;
}

#pragma vector = PORT1_VECTOR
__interrupt void handleS1StateChange()
{
	// S1 was pressed
  if (!(s1_press_count % 2))
	{
		__bis_SR_register_on_exit(LPM2_bits);
    		s1_press_count++;
	}
	else
	{	
    		__bic_SR_register_on_exit(LPM2_bits);
    		s1_press_count++;
	}

  delay(DELAY);

	P1IFG &= ~BIT7; // reset interrupt flag for s1

	return;
}

// 0 - off / 1 - on
int getS1State()
{
	return (P1IN & BIT7) ? 0 : 1;
}

// 0 - off / 1 - on
int getS2State()
{
	return (P2IN & BIT2) ? 0 : 1;
}

void delay(int value)
{
	volatile int temp = 0;
	volatile int i = 0;
	for (;i < value; i++)
	{
		temp++;
	}
}