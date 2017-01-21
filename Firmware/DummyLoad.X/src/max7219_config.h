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

#ifndef MAX7219_CONFIG_H_
#define MAX7219_CONFIG_H_

/* NOTE: ports shall be configured for the outout by the main function.
 *       Driver's code doesnt' touch TRIS bits at all.
 */

/* Port used to send data to driver's Din line. */
#define DOUT PORTAbits.RA2

/* Port used for driver's CLK line. */
#define CLK  PORTCbits.RC0

/* Port used for driver's LOAD line. */
#define LOAD PORTCbits.RC1

#endif  /* MAX7219_CONFIG_H_ */
