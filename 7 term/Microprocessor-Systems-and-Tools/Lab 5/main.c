#include <msp430.h>
#include <math.h>


typedef unsigned char uchar;

/* LCD --------------------------------------------------------------------------------------------------------------------- */

#define SCROL_CTL					0x40  //Scroll image up by SL rows (SL = last 5 bits), range:0-63

#define SET_MIRROR_COL				0xA0  //Normal mirror SEG (column) mapping (set bit0 to mirror columns)

#define SET_MIRROR_ROW				0xC0  //Normal mirror COM (row) mapping (set bit3 to mirror rows)

#define ALL_PIXEL_ON				0xA4  //Disable all pixel on (last bit 1 to turn on all pixels - does not affect memory)

#define LCD_INVERSE					0xA6  //Inverse display off (last bit 1 to invert display - does not affect memory)

#define BIAS_RATIO_VCC				0xA2  //Set voltage bias ratio (BR = bit0)

#define POW_CTL						0x2F  //Set Power control - booster, regulator, and follower on

#define SET_CONTRAST_RESISTOR		0x27  //Set internal resistor ratio Rb/Ra to adjust contrast
#define MSB_SELECT_VOLUME			0x81  //Set Electronic Volume "PM" to adjust contrast
#define LSB_SELECT_VOLUME			0x10  //Set Electronic Volume "PM" to adjust contrast (PM = last 5 bits)

#define ADV_CTL_MSB					0xFA  //Set temp. compensation curve to -0.11%/C
#define ADV_CTL_LSB					0x90

#define COLUMN_ADR_MSB				0x10  //Set SRAM col. addr. before write, last 4 bits = ca4-ca7
#define COLUMN_ADR_LSB				0x00  //Set SRAM col. addr. before write, last 4 bits = ca0-ca3
#define PAGE_ADR					0xB0  //Set SRAM page addr (pa = last 4 bits), range:0-8

#define LCD_EN						0xAF  //Enable display (exit sleep mode & restore power)

/* LCD --------------------------------------------------------------------------------------------------------------------- */

/* Accelerometer ----------------------------------------------------------------------------------------------------------- */

#define NONE						0
#define READ_X_AXIS_DATA 			0x18  //read from address 06h
#define READ_Y_AXIS_DATA 			0x1C  //read from address 07h
#define READ_Z_AXIS_DATA 			0x20  //read from address 08h

/* Accelerometer ----------------------------------------------------------------------------------------------------------- */

#define ROWS 9
#define COLUMNS 6
#define PAGES 2
#define DELAY 1000
#define COLUMN_OFFSET_BIG 30
#define COLUMN_OFFSET_NONE 0


void Delay(long int value);

void SetupAccelerometer();
void SetupLCD();

void __LCD_SetAddress(uchar page, uchar column);
void Dogs102x6_writeData(uchar *sData, uchar i);
void Dogs102x6_writeCommand(uchar *sCmd, uchar i);

uchar cma3000_SPI(uchar byte_one, uchar byte_two);
long int get_mili_g_from_byte(uchar projection_byte);

void Clear(void);
void ShowNumber(long int number);


// 4 columns (+2 offset) and 9 rows. Each byte => 8 rows == 1 page
uchar plus[PAGES][COLUMNS]  = {{0x18, 0x7E, 0x7E, 0x18, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00}};
uchar minus[PAGES][COLUMNS] = {{0x18, 0x18, 0x18, 0x18, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00}};
uchar digits[10][PAGES][COLUMNS] = {
  {{0x7F, 0xC1, 0xC1, 0x7F, 0x00, 0x00}, {0x00, 0x80, 0x80, 0x00, 0x00, 0x00}}, // digit 0
  {{0xFF, 0xFF, 0x06, 0x04, 0x00, 0x00}, {0x80, 0x80, 0x00, 0x00, 0x00, 0x00}}, // digit 1
  {{0xCF, 0xDD, 0xF9, 0xF3, 0x00, 0x00}, {0x00, 0x80, 0x80, 0x00, 0x00, 0x00}}, // digit 2
  {{0x77, 0xDD, 0xC9, 0x63, 0x00, 0x00}, {0x00, 0x80, 0x80, 0x00, 0x00, 0x00}}, // digit 3
  {{0xFF, 0x08, 0x08, 0x0F, 0x00, 0x00}, {0x80, 0x00, 0x00, 0x80, 0x00, 0x00}}, // digit 4
  {{0xF9, 0xC9, 0xC9, 0xCF, 0x00, 0x00}, {0x80, 0x80, 0x80, 0x80, 0x00, 0x00}}, // digit 5
  {{0x73, 0xC9, 0xC9, 0x7F, 0x00, 0x00}, {0x00, 0x80, 0x80, 0x00, 0x00, 0x00}}, // digit 6
  {{0x03, 0x0C, 0x30, 0xC0, 0x00, 0x00}, {0x80, 0x80, 0x80, 0x80, 0x00, 0x00}}, // digit 7
  {{0x77, 0xC9, 0xC9, 0x77, 0x00, 0x00}, {0x00, 0x80, 0x80, 0x00, 0x00, 0x00}}, // digit 8
  {{0x7F, 0xC9, 0xC9, 0x67, 0x00, 0x00}, {0x00, 0x80, 0x80, 0x00, 0x00, 0x00}}  // digit 9
};

int column_offset = COLUMN_OFFSET_BIG; // 0 - default is COLUMN_OFFSET_BIG, 1 - mirror horizonta is COLUMN_OFFSET_NONE

uchar LCD_INIT_COMMANDS_PART_1[7] = {
	SCROL_CTL,
	SET_MIRROR_COL,
	SET_MIRROR_ROW,
	ALL_PIXEL_ON,
	LCD_INVERSE,
	BIAS_RATIO_VCC,
	POW_CTL
};
uchar LCD_INIT_COMMANDS_PART_2[6] = {
	SET_CONTRAST_RESISTOR,
	MSB_SELECT_VOLUME,
	LSB_SELECT_VOLUME,
	ADV_CTL_MSB,
	ADV_CTL_LSB,
	LCD_EN,
};
int AMOUNT_OF_COMMANDS_1 = 7;
int AMOUNT_OF_COMMANDS_2 = 6;

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;

  P1DIR |= BIT1;	// set led 4 to input mode
  P1OUT &= ~BIT1;	// turn it off

  SetupLCD();
  SetupAccelerometer();

  __bis_SR_register(GIE + LPM0_bits);
  __no_operation();

  return 0;
}

void SetupLCD()
{
	// Reset LCD
	P5DIR |= BIT7;	// port init for LCD operations
	P5OUT &= ~BIT7;	// set RST (active low)
	P5OUT |= BIT7;	// reset RST (inactive is high)

	// Delay for at least 5ms
	__delay_cycles(550);

	// Choosing slave
	P7DIR |= BIT4;	// select LCD for chip
	P7OUT &= ~BIT4;	// CS is active low

	// Setting up LCD_D/C
	P5DIR |= BIT6;	// Command/Data for LCD
	P5OUT &= ~BIT6;	// CD low for command

	// Set up P4.1 -- SIMO, P4.3 -- SCLK (select PM_UCB1CLK)
	P4SEL |= BIT1 | BIT3;
	P4DIR |= BIT1 | BIT3;

	// Set up back light
	P7DIR |= BIT6;	// initializing
	P7OUT |= BIT6;	// back light
	P7SEL &= ~BIT6; // USE PWM to control brightness

	// Deselect slave
	P7OUT |= BIT4;	// CS = 1 (Deselect LCD) (stop setting it up)

	UCB1CTL1 |= UCSWRST;	// set UCSWRST bit to disable USCI and change its control registers

	UCB1CTL0 = (
		UCCKPH 	&	// UCCKPH - 1: change out on second signal change, capture input on first one)
		~UCCKPL |	// UCCKPL - 0: active level is 1
		UCMSB 	|	// MSB comes first, LSB is next
		UCMST 	|	// Master mode
		UCSYNC 	|	// Synchronous mode
		UCMODE_0	// 3 pin SPI mode
	);

	// set SMCLK as source and keep RESET
	UCB1CTL1 = UCSSEL_2 | UCSWRST;

	// set frequency divider
	UCB1BR0 = 0x01;	// LSB to 1
	UCB1BR1 = 0;	// MSB to 0

	UCB1CTL1 &= ~UCSWRST;	// enable USCI
	UCB1IFG &= ~UCRXIFG;	// reset int flag (which is set after input shift register gets data)
	Dogs102x6_writeCommand(LCD_INIT_COMMANDS_PART_1, AMOUNT_OF_COMMANDS_1);

	// delay to wait at least 120 ms
	__delay_cycles(12500);

	Dogs102x6_writeCommand(LCD_INIT_COMMANDS_PART_2, AMOUNT_OF_COMMANDS_2);

	Clear();
}

void SetupAccelerometer()
{
    // INT signal
    P2DIR  &= ~BIT5;	// mode: input
    P2REN  |=  BIT5;	// enable pull up resistor
    P2IE   |=  BIT5;	// interrupt enable
    P2IES  &= ~BIT5;	// process on interrupt's front
    P2IFG  &= ~BIT5;	// clear interrupt flag

    // set up cma3000 (CBS - Chip Select (active - 0))
    P3DIR  |=  BIT5;	// mode: output
    P3OUT  |=  BIT5;	// disable cma3000 SPI data transfer

    // set up ACCEL_SCK (SCK - Serial Clock)
    P2DIR  |=  BIT7;	// mode: output
    P2SEL  |=  BIT7;	// clk is  UCA0CLK

    // Setup SPI communication
    P3DIR  |= (BIT3 | BIT6);	// Set MOSI and PWM pins to output mode
    P3DIR  &= ~BIT4;			// Set MISO to input mode
    P3SEL  |= (BIT3 | BIT4);	// Set mode : P3.3 - UCA0SIMO , P3.4 - UCA0SOMI
    P3OUT  |= BIT6;				// Power cma3000

    UCA0CTL1 |= UCSWRST;		// set UCSWRST bit to disable USCI and change its control registers

    UCA0CTL0 = (
		UCCKPH 	&	// UCCKPH - 1: change out on second signal change, capture input on first one)
		~UCCKPL |	// UCCKPL - 0: active level is 1
		UCMSB 	|	// MSB comes first, LSB is next
		UCMST 	|	// Master mode
		UCSYNC 	|	// Synchronous mode
		UCMODE_0	// 3 pin SPI mode
	);

	// set SMCLK as source and keep RESET
	UCA0CTL1 = UCSSEL_2 | UCSWRST;

	// set frequency divider
	UCA0BR0 = 0x30;	// LSB to 48
	UCA0BR1 = 0x0;	// MSB to 0

	UCA0CTL1 &= ~UCSWRST;	// enable USCI

	// dummy read from REVID
	cma3000_SPI(0x04, NONE);
	__delay_cycles(550);

	// write to CTRL register
	cma3000_SPI(
		0x0A,
		BIT4 |	// restrict I2C
		BIT2 |	// detect motion
		BIT1	// frequency of 40Hz
	);
	__delay_cycles(10500);
}

// byte_one - frame part 1 (8-2: address, 1: R/W, 0: always 0)
// byte_two - frame part 2 (data when W or anything when R)
uchar cma3000_SPI(uchar byte_one, uchar byte_two)
{
    char indata;

    P3OUT &= ~BIT5;		// enable cma3000 SPI data transfer

    // dummy read
    indata = UCA0RXBUF;

    // USCI_A0 TX buffer ready?
    while(!(UCA0IFG & UCTXIFG));

    UCA0TXBUF = byte_one;

    // USCI_A0 RX buffer ready?
    while(!(UCA0IFG & UCRXIFG));

    indata = UCA0RXBUF;

    // USCI_A0 TX buffer ready?
    while(!(UCA0IFG & UCTXIFG));

    UCA0TXBUF = byte_two;

    // USCI_A0 RX buffer ready?
    while(!(UCA0IFG & UCRXIFG));

    indata = UCA0RXBUF;

    // Wait for all TX/RX to finish
    while(UCA0STAT & UCBUSY);

    P3OUT |= BIT5;	// disable cma3000 SPI data transfer

    return indata;
}

void __LCD_SetAddress(uchar page, uchar column)
{
	uchar cmd[1];

	if (page > 7)
	{
		page = 7;
	}

	if (column > 101)
	{
		column = 101;
	}

	cmd[0] = PAGE_ADR + (7 - page);
	uchar command_high = 0x00;
	uchar command_low = 0x00;
	uchar column_address[] = { COLUMN_ADR_MSB, COLUMN_ADR_LSB };

	command_low = (column & 0x0F);
	command_high = (column & 0xF0);
	command_high = (command_high >> 4);

	column_address[0] = COLUMN_ADR_LSB + command_low;
	column_address[1] = COLUMN_ADR_MSB + command_high;

	Dogs102x6_writeCommand(cmd, 1);
	Dogs102x6_writeCommand(column_address, 2);
}

void Dogs102x6_writeCommand(uchar *sCmd, uchar i)
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

	// Dummy read to empty RX buffer and Clear any overrun conditions
	UCB1RXBUF;

	// CS High
	P7OUT |= BIT4;
}

void Dogs102x6_writeData(uchar *sData, uchar i)
{
	// CS Low
	P7OUT &= ~BIT4;
	//CD High
	P5OUT |= BIT6;

	while (i)
	{
		// USCI_B1 TX buffer ready?
		while (!(UCB1IFG & UCTXIFG));

		// Transmit data and increment pointer
		UCB1TXBUF = *sData++;

		// Decrement the Byte counter
		i--;
	}

	// Wait for all TX/RX to finish
	while (UCB1STAT & UCBUSY);

	// Dummy read to empty RX buffer and Clear any overrun conditions
	UCB1RXBUF;

	// CS High
	P7OUT |= BIT4;
}

void ShowNumber(long int number)
{
	volatile long int length = 1;
	volatile long int digit = 0;
	volatile long int i = 0;

	long int temp = number > 0 ? number : (-1) * number;
	while(1)
	{
		if (temp < 10)
		{
			break;
		}

		temp /= 10;

		length++;
	}

	temp = number;
	for(i = 0; i < length; i++)
	{
		digit = (long int)(temp % 10);

		digit = digit < 0 ? (-1) * digit : digit;

		if (digit < 10)
			{
			__LCD_SetAddress(0, column_offset + i * COLUMNS);
			Dogs102x6_writeData(digits[digit][0], COLUMNS);
			__LCD_SetAddress(1, column_offset + i * COLUMNS);
			Dogs102x6_writeData(digits[digit][1], COLUMNS);
		}

		temp /= 10;
	}

	if (number >= 0)
	{
		__LCD_SetAddress(0, column_offset + length * COLUMNS);
		Dogs102x6_writeData(plus[0], COLUMNS);
		__LCD_SetAddress(1, column_offset + length * COLUMNS);
		Dogs102x6_writeData(plus[1], COLUMNS);
	}
	else
	{
		__LCD_SetAddress(0, column_offset + length * COLUMNS);
		Dogs102x6_writeData(minus[0], COLUMNS);
		__LCD_SetAddress(1, column_offset + length * COLUMNS);
		Dogs102x6_writeData(minus[1], COLUMNS);
	}
}

void Clear(void)
{
	uchar lcd_data[] = {0x00};
	uchar page, column;

	for (page = 0; page < 8; page++)
	{
		__LCD_SetAddress(page, 0);
		for (column = 0; column < 132; column++)
		{
			Dogs102x6_writeData(lcd_data, 1);
		}
	}
}

long int get_mili_g_from_byte(uchar projection_byte)
{
	uchar is_negative = projection_byte & BIT7;
	volatile int value_bits = 7;
	uchar bits[] = { BIT6, BIT5, BIT4, BIT3, BIT2, BIT1, BIT0 };
	long int mapping[] = { 4571, 2286, 1141, 571, 286, 143, 71 };

	int i = 0;
	long int projection = 0;
	for (; i < value_bits; i++)
	{
		if (!is_negative)
		{
			projection += (bits[i] & projection_byte) ? mapping[i] : 0;
		}
		else
		{
			projection += (bits[i] & projection_byte) ? 0 : mapping[i];
		}
	}

	projection = is_negative ? projection * (-1) : projection;

	return projection;
}

long int get_angle(long int projection)
{
	double precised_projection = projection;

	// from mili g to g
	double ratio = precised_projection / 1000;

	ratio = ratio > 1 ? 1 : ratio < -1 ? -1 : ratio;

	volatile double angle = acos(ratio);

	// convert rad to deg
	angle *= 57.3;

	return (long int)angle;
}

#pragma vector = PORT2_VECTOR
__interrupt void __Accelerometer_ISR(void)
{
	Delay(DELAY);

	Clear();

	volatile uchar x_projection_byte = cma3000_SPI(READ_X_AXIS_DATA, NONE);
  __delay_cycles(550);

	volatile long int x_projection = get_mili_g_from_byte(x_projection_byte);

	volatile uchar z_projection_byte = cma3000_SPI(READ_Z_AXIS_DATA, NONE);
    __delay_cycles(550);

	volatile long int z_projection = get_mili_g_from_byte(z_projection_byte);

	// 3600 * 3600 / 1609 * 9.81 -- convert to miles per hour^2
	long int coeff = 79020;
	long int miles_per_hour_squared = x_projection * coeff;

	ShowNumber(miles_per_hour_squared);

	long int angle = get_angle(x_projection);

	angle = z_projection > 0 ? angle : angle * (-1);

	if ((angle >= -180) && (angle <= 0))
	{
		P1OUT |= BIT1;
	}
	else
	{
		P1OUT &= ~BIT1;
	}
}

void Delay(long int value)
{
	volatile long int i = 0;
	volatile long int temp = 0;
	for (; i < value; i++)
	{
		temp++;
	}
}
