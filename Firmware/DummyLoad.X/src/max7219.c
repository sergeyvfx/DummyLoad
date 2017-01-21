/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2014 Sergey Sharybin <sergey.vfx@gmail.com>
 * All rights reserved.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */
#include <htc.h>
#include <pic18f2550.h>

#include "max7219.h"
#include "max7219_config.h"

#include "system.h"

void maxDriverStartupInitialize(void) {
  LOAD = 1;  /* Load line is high by default. */
  CLK = 0;   /* Ensure no clock pulse is here. */
  DOUT = 0;  /* Zero data output */
}

void maxDriverSendData(uint8_t address, uint8_t data) {
#define SEND_BIT(bit) { DOUT = (bit); CLK = 1; CLK = 0; DOUT = 0; } (void)0

  LOAD = 0;

  /* 4 MSB are "don't care" */
  SEND_BIT(0);
  SEND_BIT(0);
  SEND_BIT(0);
  SEND_BIT(0);

  SEND_BIT((address & 0x08) >> 3);
  SEND_BIT((address & 0x04) >> 2);
  SEND_BIT((address & 0x02) >> 1);
  SEND_BIT((address & 0x01) >> 0);

  SEND_BIT((data & 0x80) >> 7);
  SEND_BIT((data & 0x40) >> 6);
  SEND_BIT((data & 0x20) >> 5);
  SEND_BIT((data & 0x10) >> 4);
  SEND_BIT((data & 0x08) >> 3);
  SEND_BIT((data & 0x04) >> 2);
  SEND_BIT((data & 0x02) >> 1);
  SEND_BIT((data & 0x01) >> 0);

  DOUT = 0;
  LOAD = 1;

#undef SEND_BIT
}
