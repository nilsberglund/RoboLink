/*
HD44780 LCD device driver for AVR MCUs.
Copyright (C) 2011  Philippe Proulx (eeppeliteloop@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _HD44780_LOW_H
#define _HD44780_LOW_H

#include <avr/io.h>


/*
Bit definitions for HD44780 functions.
*/
#define HD44780_L_DISP_D_ON	1	/* Display ON */
#define HD44780_L_DISP_D_OFF	0	/* Display OFF */
#define HD44780_L_DISP_C_ON	1	/* Cursor ON */
#define HD44780_L_DISP_C_OFF	0	/* Cursor OFF */
#define HD44780_L_DISP_B_ON	1	/* Blink ON */
#define HD44780_L_DISP_B_OFF	0	/* Blink OFF */
#define HD44780_L_EMS_ID_INC	1	/* Increment address after each write */
#define HD44780_L_EMS_ID_DEC	0	/* Decrement address after each write */
#define HD44780_L_EMS_S_ON	1	/* Display shift ON */
#define HD44780_L_EMS_S_OFF	0	/* Display shift OFF */
#define HD44780_L_CDS_SC_SHIFT	1	/* Shift display */
#define HD44780_L_CDS_SC_MOVE	0	/* Move cursor */
#define HD44780_L_CDS_RL_RIGHT	1	/* Shift right */
#define HD44780_L_CDS_RL_LEFT	0	/* Shift left */
#define HD44780_L_FS_DL_8BIT	1	/* 8-bit data length */
#define HD44780_L_FS_DL_4BIT	0	/* 4-bit data length */
#define HD44780_L_FS_N_DUAL	1	/* 2 lines */
#define HD44780_L_FS_N_SINGLE	0	/* 1 line */
#define HD44780_L_FS_F_510	1	/* 5 * 10 dots character font */
#define HD44780_L_FS_F_58	0	/* 5 * 8 dots character font */

#ifdef __cplusplus
extern "C" {
#endif

/*
HD44780 configuration structure (index and AVR port used for each LCD physical
pin, line 1 and line 2 (if exists) base addresses). 
*/
struct hd44780_l_conf {
	uint8_t rs_i;			/* RS pin index */
	uint8_t rw_i;			/* R/W pin index */
	uint8_t en_i;			/* EN pin index */
	uint8_t db7_i;			/* DB7 pin index */
	uint8_t	db6_i;			/* DB6 pin index */
	uint8_t	db5_i;			/* DB5 pin index */
	uint8_t	db4_i;			/* DB4 pin index */
	uint8_t	db3_i;			/* DB3 pin index */
	uint8_t	db2_i;			/* DB2 pin index */
	uint8_t	db1_i;			/* DB1 pin index */
	uint8_t	db0_i;			/* DB0 pin index */
	volatile uint8_t* rs_port;	/* AVR port for RS pin */
	volatile uint8_t* rw_port;	/* AVR port for R/W pin */
	volatile uint8_t* en_port;	/* AVR port for EN pin */
	volatile uint8_t* db7_port;	/* AVR port for DB7 pin */
	volatile uint8_t* db6_port;	/* AVR port for DB6 pin */
	volatile uint8_t* db5_port;	/* AVR port for DB5 pin */
	volatile uint8_t* db4_port;	/* AVR port for DB4 pin */
	volatile uint8_t* db3_port;	/* AVR port for DB3 pin */
	volatile uint8_t* db2_port;	/* AVR port for DB2 pin */
	volatile uint8_t* db1_port;	/* AVR port for DB1 pin */
	volatile uint8_t* db0_port;	/* AVR port for DB0 pin */
	uint8_t line1_base_addr;	/* Line 1 base address */
	uint8_t line2_base_addr;	/* Line 2 base address (if exists) */
	uint8_t dl;			/* Data length (refer to bit defs.) */
};

/**
 * Device function: clear display.
 *
 * @param conf		HD44780 configuration
 */
void hd44780_l_clear_disp(const struct hd44780_l_conf* conf);

/**
 * Device function: return home.
 *
 * @param conf		HD44780 configuration
 */
void hd44780_l_return_home(const struct hd44780_l_conf* conf);

/**
 * Device function: entry mode set.
 *
 * @param conf		HD44780 configuration
 * @param id		Increment or decrement cursor after each write
 * @param s		Shift display on/off
 */
void hd44780_l_ems(const struct hd44780_l_conf* conf, uint8_t id, uint8_t s);

/**
 * Device function: display on/off control.
 *
 * @param conf		HD44780 configuration
 * @param d		Whole display on/off
 * @param c		Cursor on/off
 * @param b		Blink on/off
 */
void hd44780_l_disp(const struct hd44780_l_conf* conf, uint8_t d, uint8_t c, uint8_t b);

/**
 * Device function: cursor or display shift.
 *
 * @param conf		HD44780 configuration
 * @param sc		Move cursor/shift display
 * @param rl		Right/left
 */
void hd44780_l_cds(const struct hd44780_l_conf* conf, uint8_t sc, uint8_t rl);

/**
 * Device function: function set.
 *
 * @param conf		HD44780 configuration
 * @param dl		Data length (4-bit/8-bit)
 * @param n		Number of display lines
 * @param f		Character font
 */
void hd44780_l_fs(const struct hd44780_l_conf* conf, uint8_t dl, uint8_t n, uint8_t f);

/**
 * Device function: set CGRAM address.
 *
 * @param conf		HD44780 configuration
 * @param addr		CGRAM address (6-bit)
 */
void hd44780_l_set_cgram_addr(const struct hd44780_l_conf* conf, uint8_t addr);

/**
 * Device function: set DDRAM address.
 *
 * @param conf		HD44780 configuration
 * @param addr		DDRAM address (7-bit)
 */
void hd44780_l_set_ddram_addr(const struct hd44780_l_conf* conf, uint8_t addr);

/**
 * Device function: write data to CGRAM/DDRAM (last selected).
 *
 * @param conf		HD44780 configuration
 * @param data		Data to be written (8-bit)
 */
void hd44780_l_write(const struct hd44780_l_conf* conf, uint8_t data);

/**
 * Initializes device according to HD44780 configuration's data length.
 *
 * @param conf		HD44780 configuration
 * @param n		Number of display lines
 * @param f		Character font
 * @param id		Increment or decrement cursor after each write
 * @param s		Shift display on/off
 */
void hd44780_l_init(const struct hd44780_l_conf* conf, uint8_t n, uint8_t f, uint8_t id, uint8_t s);

#ifdef __cplusplus
}
#endif

#endif /* _HD44780_LOW_H */
