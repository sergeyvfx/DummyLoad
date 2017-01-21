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

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <xc.h>
#include <stdbool.h>

#include "fixed_address_memory.h"
#include "io_mapping.h"

#define _XTAL_FREQ 20000000

/* Initializes the system. */
void SYSTEM_Initialize(void);

/* System level tasks that keep the system running */
void SYSTEM_Tasks(void);

#endif  /* __SYSTEM_H__ */
