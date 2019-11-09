#include <msp430.h>

void delay(int value);
void turnA1On(void);
void set_smclk(void);
int getS1State();
int getS2State();

#define DELAY 50

#define WDT__ 1
#define TA1__ 0

int selected_timer = TA1__;
volatile int counter = 0;

/*
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;

	P1DIR &= ~BIT7; // s1 dir for reading
    P1OUT |= BIT7;  // set inactive signal level for button s1
	P1REN |= BIT7;  // connect s1 to vcc resistor

	P2DIR &= ~BIT2; // s2 dir for reading
	P2OUT |= BIT2;  // set inactive signal level
	P2REN |= BIT2;  // connect s2 to vcc resistor

	P1DIR |= BIT0;  // set led 1 writing
	P1OUT &=~ BIT0; // set led 1 inactive level

	P8DIR |= BIT1;  // set led 2 writing
	P8OUT &=~ BIT1; // set led 2 inactive level

	P8DIR |= BIT2;  // set led 3 writing
	P8OUT &=~ BIT2; // set led 3 inactive level

	P1DIR |= BIT3;  // set led 6 writing
	P1OUT &=~ BIT3; // set led 6 inactive level
	P1SEL |= BIT3;  // set led 6 to work with periphery

 	P1DIR |= BIT4;  // set led 7 writing
 	P1OUT &=~ BIT4; // set led 7 inactive level

	P1DIR |= BIT5;  // set led 8 writing
	P1OUT &=~ BIT5; // set led 8 inactive level

	UCSCTL4 |= SELA__REFOCLK;

	TA0CTL = TASSEL_1 | ID__1 | MC__UPDOWN | TACLR;		// set source to ACLK, divider to 1, count to up/down and clear A0 counter
	volatile long int second = 32768; 					// 1 second with frequency of 32 kHz (ACLK / ID__1)
	volatile long int period = (long int)(second / 2);	// timer should reach TA0CCR0 for 0.5 sec
	TA0CCR0 = period;
	TA0CCR2 = (long int)(period / 2);					// set TA0CCR2 to be half of TA0CCR0
	TA0CCTL2 = (OUTMOD_7);								// Set out mode to 7 (reset/set) (and enable capture/compare interrupts by setting CCIE (optional))

	P1IES |= BIT7;  // process interrupt on signal's front
	P1IE |= BIT7;   // allow interrupts for s1
	P1IFG &= ~BIT7; // reset interrupt flag for s1

	P2IES |= BIT2;  // process interrupt on signal's front
	P2IE |= BIT2;   // allow interrupts for s2
	P2IFG &= ~BIT2; // reset interrupt flag for s2

	_bis_SR_register(GIE);

	__no_operation();

	return 0;
}

void set_smclk(void)
{
	UCSCTL1 &= 0; // null the register to reset default values
	UCSCTL2 &= 0; // null the register to reset default values
	UCSCTL3 &= 0; // null the register to reset default values

	UCSCTL4 |= SELS__DCOCLKDIV;   // set ref for MCLK to DCOCLKDIV

	UCSCTL1 |= DCORSEL_0; 	// set lowest possible frequency range

	UCSCTL3 &= 0;
	UCSCTL3 |= SELREF__REFOCLK; 	// set FLL ref clk to REFOCLK
	UCSCTL3 |= FLLREFDIV__8;	// set FLL ref div to 8

	UCSCTL2 &= 0;
	UCSCTL2 |= FLLD__16;  // set FLLD to 16
	UCSCTL2 |= FLLN2;	  // set FLLN
	UCSCTL2 |= FLLN1;	  // set FLLN
	UCSCTL2 &= ~FLLN0;	  // to 2

	UCSCTL5 |= DIVS__16;	// got 1798 Hz as SMCLK

	// SMCLK = ( 32768 / 8 * (6 + 1) ) / 16
}

void blink(void)
{
	counter++;

	switch(counter)
	{
		case 3:
			P1OUT |= BIT4;	// turn led 7 on

			break;
		case 5:
			P1OUT &= ~BIT3;	// turn led 6 off

			break;
		case 6:
			P1OUT |= BIT5;	// turn led 8 on

			break;
		case 8:
			P1OUT &= ~BIT4;	// turn led 7 off

			break;
		case 11:
			P1OUT &= ~BIT5;	// turn led 8 off

			P1SEL |= BIT3;  // set led 6 to work with periphery

			TA1CCTL0 &= ~CCIE;	// disable capture/compare timer interrupts
			SFRIE1 &= ~WDTIE;		// enable WDT interrupts

			break;
		default:
			break;
	}
}


#pragma vector = WDT_VECTOR
__interrupt void wdt_isr(void)
{
	blink();
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void ta1_isr (void)
{
	blink();
}


#pragma vector = PORT1_VECTOR
__interrupt void button_s1_isr(void)
{
	delay(DELAY);

	if (getS1State())
	{
		counter = 0;	// reset counter to be able to repeat blinking

		set_smclk();

		P1SEL &= ~BIT3;	// set led 6 to work as I/O

		P1OUT |= BIT3;	// turn led 6 on

		if (selected_timer == TA1__)
		{
			turnA1On();
		}
		else if (selected_timer == WDT__)
		{
			SFRIE1 |=  WDTIE;		// enable WDT interrupts
		}
	}

	P1IFG = 0;	// clear interrupt request flag
}

#pragma vector = PORT2_VECTOR
__interrupt void button_s2_isr (void)
{
	delay(DELAY);

	if (getS2State())
	{
		if (selected_timer == TA1__)
		{
			TA1CCTL0 &= ~CCIE;		// disable capture/compare timer interrupts
			TA1CCTL0 &= ~OUT;

			WDTCTL = WDTPW				// set password for WDT
					+ WDTTMSEL			  // interval mode
					+ WDTCNTCL			  // clear timer
					+ WDTIS__512		  // set source divider to 512 kHz
					+ WDTSSEL__SMCLK	// set SMCLK to be the source
					+ (!WDTHOLD); 		// ensure that dimer is not on hold

			P1OUT ^= BIT0;

			selected_timer = WDT__;
		}
		else if (selected_timer ==  WDT__)
		{
			WDTCTL = WDTPW + WDTHOLD;	// stop WDT

			turnA1On();

			selected_timer = TA1__;

			P1OUT ^= BIT0;
		}
	}

	P2IFG &= ~BIT2;	// clear interrupt request flag
}

void turnA1On(void)
{
	int second = 1798;	// ticks per second
	int interval = (int)(second * 0.3);
	TA1CCR0 = interval;
	TA1CCTL0 |= CCIE;	  // allow capture/compare interrupts
	TA1CTL = TASSEL_2 | ID__1 | MC__UP | TACLR; // set source to SMCLK, divider to 1, count to up and clear A1 counter
}

void delay(int value)
{
	volatile long int i = 0;
	volatile long int temp = 0;
	for (; i < value; i++)
	{
		temp++;
	}
}

int getS1State()
{
	return (P1IN & BIT7) ? 0 : 1;
}

int getS2State()
{
	return (P2IN & BIT2) ? 0 : 1;
}