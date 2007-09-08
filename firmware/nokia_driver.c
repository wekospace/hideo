#include "nokia_driver.h"
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "date.h"

#define _NOK_MODE_NORMAL   0x00
#define _NOK_MODE_INVERTED 0xFF

/////////////////////////////////////////////////////////////////////////////////


const prog_uchar ascii_table[128][5] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },  // sp
	{ 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0xc4, 0xc8, 0x10, 0x26, 0x46 },   // %
	{ 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x50, 0x30, 0x00 },   // ,
	{ 0x10, 0x10, 0x10, 0x10, 0x10 },   // -
	{ 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
	{ 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
	{ 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
	{ 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
	{ 0x7E, 0x11, 0x11, 0x11, 0x7E },   // A
	{ 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
	{ 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
	{ 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
	{ 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
	{ 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
	{ 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
	{ 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
	{ 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
	{ 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
	{ 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
	{ 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
	{ 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
	{ 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
	{ 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
	{ 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
	{ 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
	{ 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
	{ 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
	{ 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
	{ 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
	{ 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
	{ 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
	{ 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
	{ 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
	{ 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
	{ 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
	{ 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x0C, 0x52, 0x52, 0x52, 0x3E },   // g
	{ 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x20, 0x40, 0x44, 0x3D, 0x00 },   // j
	{ 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x7C, 0x14, 0x14, 0x14, 0x08 },   // p
	{ 0x08, 0x14, 0x14, 0x18, 0x7C },   // q
	{ 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
	{ 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
	{ 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
	{ 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
	{ 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
	{ 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
	{ 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
	{ 0x0C, 0x50, 0x50, 0x50, 0x3C },   // y
	{ 0x44, 0x64, 0x54, 0x4C, 0x44 }    // z
};

static uint8 reverse_mode = _NOK_MODE_NORMAL;
static uint8 printed_cols = 0;

/** @fn void nokia_init(void)
		@brief nokia LCD initialisation
*/
void nokia_init(void)
{
	NOK_DDR = 0xFF;
	NOK_DC=1;
	NOK_CS=1;					// chip disabled
	_delay_us(80);
	NOK_RES=0;				// reset chip during 250ms
	_delay_us(96);		// max (in us) = 768 / F_CPU (in Mhz) => at 8Mhz, 96 us
	NOK_RES=1;

	/** @note nokia 3210 LCD use pcd8544 controller with LPH7366-1, LPH7779 or LPH7677 LCD matrix
	*/
	nokia_send_command(0x21);     // Active the LCD. set extended instruction set ; in function set
	nokia_send_command(0xC5);     // Set LCD Vop (Contrast). Vop  was 0xC5 // better is 0xa0  ; vop set
	nokia_send_command(0x13);     // LCD bias system 1:48.

	/** @note unsigned chars are stored in the display data ram, address counter, incremented automatically
	*/
	nokia_send_command(0x20);			// 0x20 for horizontal mode from left to right, X axe are incremented automatically,
																// 0x22 for vertical addressing ,back on normal instruction set too
	nokia_send_command(0x09);			// all on ; display control

//  	nokia_send_command(0x41);    // ??
//  	nokia_send_command(0x90);    // ??
//  	nokia_send_command(0x20);    // ??

	nokia_clear();
	nokia_gotoxy(0,0);
}

void nokia_send_command(const unsigned char command)
{
	NOK_DC=0;	// unsigned char is a command it is read with the eight SCLK pulse
	NOK_CS=0;	// chip enabled
	nokia_write(command);
	NOK_CS=1;	// chip disabled
}

void nokia_send_data(const unsigned char data)
{
	NOK_DC=1;
	NOK_CS=0;	// chip enabled
	nokia_write(data  ^ reverse_mode);
	NOK_CS=1;	// chip disabled
	printed_cols++;
}

void nokia_write(unsigned char data)
{
	unsigned char bit;
	for (bit=8;bit>0;bit--)
	{
		NOK_SCLK=0;

		if (data & 0x80) {
			NOK_SDA=1;
		} else {
			NOK_SDA=0;
		}
		NOK_SCLK=1;
		_delay_us(1);

		data = data << 1;
	}
}

/** @fn void nokia_reset_DDRAM(void)
		@brief reset all DDRAM (set all bits to zero)
*/
void nokia_reset_DDRAM(void)
{
	signed char dch ;
	signed char dcm ;
	signed char dcl ;
	NOK_SDA=0;		// all data bits will be always set with 0
	NOK_DC=1;
	NOK_CS=0;
	for (dch=6;dch>0;dch--) {				// 6 rows
		for (dcm=84;dcm>0;dcm--) {			// 84 columns
			for (dcl=8;dcl>0;dcl--) {		// 8 pixels
				NOK_SCLK=0;
				NOK_SCLK=1;
			}
		}
	}
	NOK_CS=1;
}

//////////////////////////////////////////////////////////////////////////////////
void nokia_gotoxy (const unsigned char x, const unsigned char y)	// Nokia LCD Position cursor
{
	nokia_send_command(0x40|( y & 0x07 )),    // Y axe initialisation: 0100 0yyy	
	nokia_send_command(0x80|( x & 0x7F ));    // X axe initialisation: 1xxx xxxx
	printed_cols = x * 6;
}

//////////////////////////////////////////////////////////////////////////////////
void nokia_display_char(const unsigned char ascii)
{
	unsigned char data[5];
	memcpy_P( data, ascii_table[ascii-32], 5 );

	nokia_send_data(0x00);  // Display a blank vertical line
	nokia_send_data(data[0]);
	nokia_send_data(data[1]);
	nokia_send_data(data[2]);
	nokia_send_data(data[3]);
	nokia_send_data(data[4]);
}

/////////////////////////////////////////////////////////////////////////////////
void nokia_display_string(const char *string)
{
  char c;
  while ((c = pgm_read_byte(string++)))
    nokia_display_char(c);
}

////////////////////////////////////////////////////////////////////////////////
void nokia_clear(void)
{
	nokia_reset_DDRAM();	// reset DDRAM, otherwise the lcd is blurred with random pixels

// 	_delay_us(80);           // 80s
	nokia_send_command(0x08);    // mod control blank change (all off)
// 	_delay_us(80);           // 80s
	nokia_send_command(0x0c);    // mod control normal change
// 	_delay_us(80);           // 80s
	printed_cols = 0;
}

void nokia_set_mode(uint8 mode)
{
	reverse_mode = (mode) ? _NOK_MODE_INVERTED : _NOK_MODE_NORMAL ;
}

void nokia_finish_line(void)
{
	while (printed_cols < 84) {
		nokia_send_data(reverse_mode);
	}
}
