#include <msp430.h>
#include <math.h>

typedef unsigned char uint8_t;

#define COLUMN_ADR_MSB              0x10  //Set SRAM col. addr. before write, last 4 bits = ca4-ca7
#define COLUMN_ADR_LSB              0x00  //Set SRAM col. addr. before write, last 4 bits = ca0-ca3
#define POW_CTL                     0x2F  //Set Power control - booster, regulator, and follower on
#define SCROL_CTL                   0x40  //Scroll image up by SL rows (SL = last 5 bits), range:0-63
#define PAGE_ADR                    0xB0  //Set SRAM page addr (pa = last 4 bits), range:0-8
#define SET_CONTRAST_RESISTOR       0x27  //Set internal resistor ratio Rb/Ra to adjust contrast
#define MSB_ELECT_VOLUME            0x81  //Set Electronic Volume "PM" to adjust contrast
#define LSB_ELECT_VOLUME            0x0F  //Set Electronic Volume "PM" to adjust contrast (PM = last 5 bits)
#define ALL_PIXEL_ON                0xA4  //Disable all pixel on (last bit 1 to turn on all pixels - does not affect memory)
#define LCD_INVERSE                 0xA6  //Inverse display off (last bit 1 to invert display - does not affect memory)
#define LCD_EN                      0xAF  //Enable display (exit sleep mode & restore power)
#define SET_MIRROR_SEG 				0xA0  //Mirror SEG (column) mapping (set bit0 to mirror display)
#define SET_MIRROR_COM 				0xC0  //Mirror COM (row) mapping (set bit3 to mirror display)
#define BIAS_RATIO_VCC              0xA2  //Set voltage bias ratio (BR = bit0)
#define ADV_CTL_MSB                 0xFA  //Set temp. compensation curve to -0.11%/C
#define ADV_CTL_LSB                 0x90
#define ROWS 9
#define COLUMNS 5
#define PAGES ROWS / 2
#define DELAY 500

void delay(int value);
int getS1State();
int getS2State();

// 4 columns (+1 offset) and 9 rows. Each byte => 8 rows == 1 page
uint8_t plus[PAGES][COLUMNS]  = {{0x18, 0x7E, 0x7E, 0x18, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00}};
uint8_t minus[PAGES][COLUMNS] = {{0x18, 0x18, 0x18, 0x18, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00}};

uint8_t num1[PAGES][COLUMNS] = {{0xFF, 0xFF, 0x06, 0x04, 0x00}, {0x80, 0x80, 0x00, 0x00, 0x00}}; // digit 1

uint8_t digits[10][PAGES][COLUMNS] = {
		{{0xFF, 0xFF, 0x06, 0x04, 0x00}, {0x80, 0x80, 0x00, 0x00, 0x00}}, // digit 0
		{{0xFF, 0xFF, 0x06, 0x04, 0x00}, {0x80, 0x80, 0x00, 0x00, 0x00}}, // digit 1
		{{0xCF, 0xDD, 0xF9, 0xF3, 0x00}, {0x00, 0x80, 0x80, 0x00, 0x00}}, // digit 2
		{{0xFF, 0xFF, 0x06, 0x04, 0x00}, {0x80, 0x80, 0x00, 0x00, 0x00}}, // digit 3
		{{0xFF, 0xFF, 0x06, 0x04, 0x00}, {0x80, 0x80, 0x00, 0x00, 0x00}}, // digit 4
		{{0xFF, 0xFF, 0x06, 0x04, 0x00}, {0x80, 0x80, 0x00, 0x00, 0x00}}, // digit 5
		{{0xFF, 0xFF, 0x06, 0x04, 0x00}, {0x80, 0x80, 0x00, 0x00, 0x00}}, // digit 6
		{{0xFF, 0xFF, 0x06, 0x04, 0x00}, {0x80, 0x80, 0x00, 0x00, 0x00}}, // digit 7
		{{0xFF, 0xFF, 0x06, 0x04, 0x00}, {0x80, 0x80, 0x00, 0x00, 0x00}}, // digit 8
		{{0xFF, 0xFF, 0x06, 0x04, 0x00}, {0x80, 0x80, 0x00, 0x00, 0x00}}  // digit 9
};

// int number = 1542;
int number = 1542;
int subtrahend = 101;
int mirror_status = 0; // 0 - default, 1 - mirror horizontal
int COLUMN_START_ADDRESS = 31; // 0 - default(31), 1 - mirror horizontal(0)

uint8_t currentPage = 0, currentColumn = 0;

uint8_t set_mirror_seg[2][1] = {{0xA0}, {0xA1}};

uint8_t Dogs102x6_initMacro[] = {
    SCROL_CTL,
    SET_MIRROR_SEG,
    SET_MIRROR_COM,
    ALL_PIXEL_ON,
    LCD_INVERSE,
    BIAS_RATIO_VCC,
    POW_CTL,
    SET_CONTRAST_RESISTOR,
    MSB_ELECT_VOLUME,
    LSB_ELECT_VOLUME,
    ADV_CTL_MSB,
    ADV_CTL_LSB,
    LCD_EN,
    PAGE_ADR,
    COLUMN_ADR_MSB,
    COLUMN_ADR_LSB
};

void Dogs102x6_writeCommand(uint8_t *sCmd, uint8_t i)
{
	// CS Low
    P7OUT &= ~BIT4;

    // CD Low
    P5OUT &= ~BIT6;
    while (i)
    {
      // USCI_B1 TX buffer ready?
      while (!(UCB1IFG & UCTXIFG)) ;

      // Transmit data
      UCB1TXBUF = *sCmd;

      // Increment the pointer on the array
      sCmd++;

      // Decrement the Byte counter
      i--;
    }

    // Wait for all TX/RX to finish
    while (UCB1STAT & UCBUSY);

    // Dummy read to empty RX buffer and clear any overrun conditions
    UCB1RXBUF;

    // CS High
    P7OUT |= BIT4;

}

void set_ADR(uint8_t page, uint8_t column)
{
  uint8_t cmd[1];

  if (page > 7)
  {
    page = 7;
  }

  if (column > 101)
  {
    column = 101;
  }

  cmd[0] = PAGE_ADR + (7 - page);
  uint8_t H = 0x00;
  uint8_t L = 0x00;
  uint8_t ColumnAddress[] = { COLUMN_ADR_MSB, COLUMN_ADR_LSB };

  currentPage = page;
  currentColumn = column;

  L = (column & 0x0F);
  H = (column & 0xF0);
  H = (H >> 4);

  ColumnAddress[0] = COLUMN_ADR_LSB + L;
  ColumnAddress[1] = COLUMN_ADR_MSB + H;


  Dogs102x6_writeCommand(cmd, 1);
  Dogs102x6_writeCommand(ColumnAddress, 2);
}

void Dogs102x6_writeData(uint8_t *sData, uint8_t i)
{

  // CS Low
  P7OUT &= ~BIT4;
  //CD High
  P5OUT |= BIT6;

  while (i)
  {
    currentColumn++;

    if (currentColumn > 101)
    {
        currentColumn = 101;
    }

    // USCI_B1 TX buffer ready?
    while (!(UCB1IFG & UCTXIFG)) ;

    // Transmit data and increment pointer
    UCB1TXBUF = *sData++;

    // Decrement the Byte counter
    i--;
  }

  // Wait for all TX/RX to finish
  while (UCB1STAT & UCBUSY) ;

  // Dummy read to empty RX buffer and clear any overrun conditions
  UCB1RXBUF;

  // CS High
  P7OUT |= BIT4;

  // Restore original GIE state
}

void displayNum(void)
{
  volatile int lenght = 1;
  volatile int digit = 0;
  volatile int j = 0;
  volatile int i = 10;

  while(1)
  {
      if(number / i != 0)
      {
          i *= 10;
          lenght++;
      }
      else break;
  }

  int temp = number;
  for(j = 0; j < lenght + 1; j++)
  {
      digit = (uint8_t)(temp % 10);

      if (digit < 10)
      {
    set_ADR(0, COLUMN_START_ADDRESS + j * COLUMNS);
    Dogs102x6_writeData(digits[digit][0], COLUMNS);
    set_ADR(1, COLUMN_START_ADDRESS + j * COLUMNS);
    Dogs102x6_writeData(digits[digit][1], COLUMNS);
      }

      temp /= 10;

  }

  if(number >= 0)
	{
		set_ADR(0, COLUMN_START_ADDRESS + lenght * COLUMNS);
		Dogs102x6_writeData(plus[0], COLUMNS);
		set_ADR(1, COLUMN_START_ADDRESS + lenght * COLUMNS);
		Dogs102x6_writeData(plus[1], COLUMNS);
	}
	else
	{
		set_ADR(0, COLUMN_START_ADDRESS + lenght * COLUMNS);
		Dogs102x6_writeData(minus[0], COLUMNS);
		set_ADR(1, COLUMN_START_ADDRESS + lenght * COLUMNS);
		Dogs102x6_writeData(minus[1], COLUMNS);
	}
}

void clear(void)
{
  uint8_t LcdData[] = {0x00};
  uint8_t p, c;

  for (p = 0; p < 8; p++)
  {
      set_ADR(p, 0);
      for (c = 0; c < 132; c++)
      {
          Dogs102x6_writeData(LcdData, 1);
      }
  }
}

void init_INT()
{
  P1IE |= BIT7;
  P1IES |= BIT7;
  P1IFG = 0;

  P2IE |= BIT2;
  P2IES |= BIT2;
  P2IFG = 0;
}

void init_button_S1_S2_out()
{
  P1DIR &= ~BIT7;
  P2DIR &= ~BIT2;

  P1REN |= BIT7;
  P2REN |= BIT2;

  P1OUT |= BIT7;
  P2OUT |= BIT2;

  init_INT();
}

void initLCD()
{
  P5DIR |= BIT7;
  P5OUT &= BIT7;
  P5OUT |= BIT7;
  P7DIR |= BIT4;
  P7OUT &= ~BIT4;
  P5DIR |= BIT6;
  P5OUT &= ~BIT6;

  P4SEL |= BIT1 | BIT3;
  P4DIR |= BIT1 | BIT3;

  P7DIR |= BIT6;
  P7OUT |= BIT6;
  P7SEL &= ~BIT6;
}

void initSPI()
{
  UCB1CTL1 |= UCSWRST;
  UCB1CTL0 = (UCCKPH | UCMSB | UCMST | UCSYNC | UCMODE_0);
  UCB1CTL1 = UCSSEL_2 | UCSWRST;
  UCB1BR0 = 0x02;
  UCB1BR1 = 0;

  UCB1CTL1 &= ~UCSWRST;
  UCB1IFG &= ~UCRXIFG;
  Dogs102x6_writeCommand(Dogs102x6_initMacro, 13);
  P7OUT |= BIT4;
}



int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;

  init_button_S1_S2_out();
  initLCD();
  initSPI();
  
  clear();
  displayNum();

  __bis_SR_register(GIE);
  __no_operation();

  return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void button_S1(void)
{
	delay(DELAY);

	if(getS1State())
	{
		clear();
		number = number - subtrahend;
		displayNum();
	}

	P1IFG &= ~BIT7;
}


#pragma vector = PORT2_VECTOR
__interrupt void button_S2(void)
{
	delay(DELAY);

	if(getS2State())
	{
		clear();

		if(mirror_status == 0)
		{
			COLUMN_START_ADDRESS = 0;
			mirror_status = 1;
		}
		else
		{
			COLUMN_START_ADDRESS = 31;
			mirror_status = 0;
		}

		Dogs102x6_writeCommand(set_mirror_seg[mirror_status], 1);
		displayNum();
	}

	P2IFG &= ~BIT2;
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
