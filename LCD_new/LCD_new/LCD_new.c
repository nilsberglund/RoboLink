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

#define F_CPU 14745600UL

#include <avr/io.h>
#include <util/delay.h>

#include "hd44780_low.h"

/**
 * Fix a single bit.
 *
 * @param port		AVR port
 * @param index		Pin index on AVR port
 * @param value		Value (0 is false; anything else is true)
 */
static inline void _hd44780_l_fb(volatile uint8_t* port, uint8_t index, uint8_t value) {
	if (value == 0) {
		*port &= ~_BV(index);
	} else {
		*port |= _BV(index);
	}
}

/**
 * Clears all pins.
 *
 * @param conf	HD44780 configuration
 */
static inline void _hd44780_l_ca(const struct hd44780_l_conf* conf) {
	*(conf->rs_port) &= ~_BV(conf->rs_i);
	*(conf->rw_port) &= ~_BV(conf->rw_i);
	*(conf->en_port) &= ~_BV(conf->en_i);
	*(conf->db7_port) &= ~_BV(conf->db7_i);
	*(conf->db6_port) &= ~_BV(conf->db6_i);
	*(conf->db5_port) &= ~_BV(conf->db5_i);
	*(conf->db4_port) &= ~_BV(conf->db4_i);
	
	if (conf->dl == HD44780_L_FS_DL_8BIT) {
		*(conf->db3_port) &= ~_BV(conf->db3_i);
		*(conf->db2_port) &= ~_BV(conf->db2_i);
		*(conf->db1_port) &= ~_BV(conf->db1_i);
		*(conf->db0_port) &= ~_BV(conf->db0_i);
	}
}

/**
 * EN cycle.
 *
 * @param conf	HD44780 configuration
 */
static inline void _hd44780_l_ec(const struct hd44780_l_conf* conf) {
	/* Max write setup time (data) is 80 ns: */
	_delay_us(0.08); /* _delay_us overhead is probably > 80 ns here... */
	
	/* Set EN: */
	*(conf->en_port) |= _BV(conf->en_i);
	
	/* Wait for EN pulse width (high level), 230 ns: */
	_delay_us(0.230);
	
	/* Clear EN: */
	*(conf->en_port) &= ~_BV(conf->en_i);
	
	/* Wait for rest of EN cycle time (500 ns): */
	_delay_us(0.270);
}

/**
 * HD44780 device function.
 *
 * @param conf		HD44780 configuration
 * @param rs		RS pin value
 * @param rw		R/W pin value
 * @param db		Data (DB7 down to DB0)
 * @param w_us		Number of µs to wait after
 */
static void _hd44780_l_func(const struct hd44780_l_conf* conf, uint8_t rs, uint8_t rw, uint8_t db, uint16_t w_us) {
	uint16_t i;
	
	/* Fix RS and R/W: */
	_hd44780_l_fb(conf->rs_port, conf->rs_i, rs);
	_hd44780_l_fb(conf->rw_port, conf->rw_i, rw);
	
	/* Fix DB7 downto DB4: */
	_hd44780_l_fb(conf->db7_port, conf->db7_i, db & _BV(7));
	_hd44780_l_fb(conf->db6_port, conf->db6_i, db & _BV(6));
	_hd44780_l_fb(conf->db5_port, conf->db5_i, db & _BV(5));
	_hd44780_l_fb(conf->db4_port, conf->db4_i, db & _BV(4));
	
	/* Write high-order bits first if configured as 4-bit data length: */
	if (conf->dl == HD44780_L_FS_DL_4BIT) {
		/* EN cycle: */
		_hd44780_l_ec(conf);
	}
	
	/* Fix DB3 downto DB0: */
	if (conf->dl == HD44780_L_FS_DL_8BIT) {
		_hd44780_l_fb(conf->db3_port, conf->db3_i, db & _BV(3));
		_hd44780_l_fb(conf->db2_port, conf->db2_i, db & _BV(2));
		_hd44780_l_fb(conf->db1_port, conf->db1_i, db & _BV(1));
		_hd44780_l_fb(conf->db0_port, conf->db0_i, db & _BV(0));
	} else {
		_hd44780_l_fb(conf->db7_port, conf->db7_i, db & _BV(3));
		_hd44780_l_fb(conf->db6_port, conf->db6_i, db & _BV(2));
		_hd44780_l_fb(conf->db5_port, conf->db5_i, db & _BV(1));
		_hd44780_l_fb(conf->db4_port, conf->db4_i, db & _BV(0));	
	}
	
	/* EN cycle: */
	_hd44780_l_ec(conf);
	
	/* Wait after function: */
	for (i = 0; i < w_us; ++i) {
		_delay_us(1.0);
	}
}

void hd44780_l_clear_disp(const struct hd44780_l_conf* conf) {
	_hd44780_l_func(conf, 0, 0, _BV(0), 1640);
}

void hd44780_l_return_home(const struct hd44780_l_conf* conf) {
	_hd44780_l_func(conf, 0, 0, _BV(1), 1640);
}

void hd44780_l_ems(const struct hd44780_l_conf* conf, uint8_t id, uint8_t s) {
	_hd44780_l_func(conf, 0, 0, (_BV(2) | (id << 1) | s), 40);
}

void hd44780_l_disp(const struct hd44780_l_conf* conf, uint8_t d, uint8_t c, uint8_t b) {
	_hd44780_l_func(conf, 0, 0, (_BV(3) | (d << 2) | (c << 1) | b), 40);
}

void hd44780_l_cds(const struct hd44780_l_conf* conf, uint8_t sc, uint8_t rl) {
	_hd44780_l_func(conf, 0, 0, (_BV(4) | (sc << 3) | (rl << 2)), 40);
}

void hd44780_l_fs(const struct hd44780_l_conf* conf, uint8_t dl, uint8_t n, uint8_t f) {
	_hd44780_l_func(conf, 0, 0,
		(_BV(5) | (dl << 4) | (n << 3) | (f << 2)), 40);
}

void hd44780_l_set_cgram_addr(const struct hd44780_l_conf* conf, uint8_t addr) {
	_hd44780_l_func(conf, 0, 0, ((_BV(6) | addr) & ~_BV(7)), 40);
}

void hd44780_l_set_ddram_addr(const struct hd44780_l_conf* conf, uint8_t addr) {
	_hd44780_l_func(conf, 0, 0, (_BV(7) | addr), 40);
}

void hd44780_l_write(const struct hd44780_l_conf* conf, uint8_t data) {
	_hd44780_l_func(conf, 1, 0, data, 45);
}

void hd44780_l_init(const struct hd44780_l_conf* conf, uint8_t n, uint8_t f, uint8_t id, uint8_t s) {
	/* Wait after Vcc rises: */
	_delay_ms(15.0);
	
	/* Clear all pins: */
	_hd44780_l_ca(conf);
	
	/* Special function set (for data length): */
	*(conf->db5_port) |= _BV(conf->db5_i);
	*(conf->db4_port) |= _BV(conf->db4_i);
	_hd44780_l_ec(conf);
	
	/* Wait again: */
	_delay_ms(4.1);
	
	/* Special function set (for data length): */
	_hd44780_l_ec(conf);
	
	/* Wait again: */
	_delay_us(100.0);
	
	/* Special function set (for data length): */
	_hd44780_l_ec(conf);
	
	/* 4-bit specific: */
	if (conf->dl == HD44780_L_FS_DL_4BIT) {
		*(conf->db4_port) &= ~_BV(conf->db4_i);
		_hd44780_l_ec(conf);
	}
	
	/* Remaining process: */
	hd44780_l_fs(conf, conf->dl, n, f);
	hd44780_l_disp(conf, HD44780_L_DISP_D_OFF, HD44780_L_DISP_C_OFF, HD44780_L_DISP_B_OFF);
	hd44780_l_clear_disp(conf);
	hd44780_l_ems(conf, id, s);
}


int main(void)
{
	
	DDRA = 0xFF;
	DDRD = (1 << DDD5)|(1 << DDD6)|(1 << DDD7);
	
	 struct hd44780_l_conf low_conf;

	 low_conf.rs_i = 5;
	 low_conf.rw_i = 6;
	 low_conf.en_i = 7;
	 low_conf.db7_i = 7;
	 low_conf.db6_i = 6;
	 low_conf.db5_i = 5;
	 low_conf.db4_i = 4;
	 low_conf.db3_i = 3;
	 low_conf.db2_i = 2;
	 low_conf.db1_i = 1;
	 low_conf.db0_i = 0;
	 low_conf.rs_port = &PORTD;
	 low_conf.rw_port = &PORTD;
	 low_conf.en_port = &PORTD;
	 low_conf.db7_port = &PORTA;
	 low_conf.db6_port = &PORTA;
	 low_conf.db5_port = &PORTA;
	 low_conf.db4_port = &PORTA;
	 low_conf.db3_port = &PORTA;
	 low_conf.db2_port = &PORTA;
	 low_conf.db1_port = &PORTA;
	 low_conf.db0_port = &PORTA;
	 low_conf.line1_base_addr = 0x00;
	 low_conf.line2_base_addr = 0x40;
	 low_conf.dl = HD44780_L_FS_DL_8BIT;
	 
	 hd44780_l_init(&low_conf, HD44780_L_FS_N_DUAL, HD44780_L_FS_F_58, HD44780_L_EMS_ID_INC, HD44780_L_EMS_S_OFF);
	 
	 hd44780_l_disp(&low_conf, HD44780_L_DISP_D_ON, HD44780_L_DISP_C_ON, HD44780_L_DISP_B_ON);
	 
	 hd44780_l_write(&low_conf, 0x48);
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}