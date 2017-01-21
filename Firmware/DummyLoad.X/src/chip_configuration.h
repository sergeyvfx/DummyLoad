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

#ifndef __CHIP_CONFIGURATION__
#define __CHIP_CONFIGURATION__

/* Ensure we have the correct target PIC device family */
#if !defined(__18F2550)
#  error "This firmware only was tested on PIC18F2550 microcontrollers."
#endif

/* PIC18F4550/PIC18F2550 configuration for the WFF Generic HID test device */
#pragma config PLLDIV   = 5         /* 20Mhz external oscillator */
#pragma config CPUDIV   = OSC1_PLL2
#pragma config USBDIV   = 2         /* Clock source from 96MHz PLL/2 */
#pragma config FOSC     = HSPLL_HS
//#pragma config FOSC     = INTOSC_EC
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config PWRT     = OFF
#pragma config BOR      = ON
#pragma config BORV     = 3
#pragma config VREGEN   = ON
#pragma config WDT      = OFF
#pragma config WDTPS    = 32768
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
#pragma config PBADEN   = OFF
#pragma config CCP2MX   = OFF
#pragma config STVREN   = ON
#pragma config LVP      = OFF
/* #pragma config ICPRT    = OFF */
#pragma config XINST    = OFF
#pragma config CP0      = OFF
#pragma config CP1      = OFF
/* #pragma config CP2      = OFF */
/* #pragma config CP3      = OFF */
#pragma config CPB      = OFF
/* #pragma config CPD      = OFF */
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
/* #pragma config WRT2     = OFF */
/* #pragma config WRT3     = OFF */
#pragma config WRTB     = OFF
#pragma config WRTC     = OFF
/* #pragma config WRTD     = OFF */
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
/* #pragma config EBTR2    = OFF */
/* #pragma config EBTR3    = OFF */
#pragma config EBTRB    = OFF

#endif  /* __CHIP_CONFIGURATION__ */
