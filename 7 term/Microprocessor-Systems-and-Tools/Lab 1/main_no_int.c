#include <msp430.h> 

const long int SMALL_DELAY = 1000;
const long int BIG_DELAY = 1000;

void ledAHandler();
void ledAOnHandler();
void ledAOffHandler();
void delay(long int num_of_ops);
void buttonPressHandler();
int getS1State();
int getS2State();

/*
 * main.c
 */
int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

  P1DIR &= ~BIT7; // s1 dir for reading
  P2DIR &= ~BIT2; // s2 dir for reading

  P1DIR |= BIT1; // led 4 dir for write
  P1DIR |= BIT2; // led 5 dir for write

  P1OUT |= BIT7; // set inactive signal level for button s1
  P2OUT |= BIT2; // set inactive signal level for button s2

  P1REN |= BIT7;
  P2REN |= BIT2;

  P1OUT &= ~BIT1; // led 4 shut down
  P1OUT &= ~BIT2; // led 5 shut down


  while(1)
  {
    buttonPressHandler();
  }

  return 0;
}

void buttonPressHandler()
{
	if (getS2State())
	{
		delay(SMALL_DELAY);
		if(getS2State())
		{
			// here we caught S2 press event

			P1OUT |= BIT2; // turn led 5 on

			if (!getS1State())
			{
				P1OUT &= ~BIT1; // led 4 shut down
			}

			// delay(BIG_DELAY);

			while(1)
			{
				if(!getS2State())
				{
					delay(SMALL_DELAY);
					if(!getS2State())
					{
						// here we caught S2 release event

						if (!getS1State())
						{
							P1OUT |= BIT1; // turn on led 4
						} 
						else
						{
							P1OUT &= ~BIT2; // led 5 shut down
						}

						delay(BIG_DELAY);

						return;
					}
				}
			}
		}
	}
}

void delay(long int num_of_ops)
{
	volatile long int a = 0;
	volatile long int i = 0;
	for(; i < num_of_ops; i++)
	{
		a += i;
	}
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