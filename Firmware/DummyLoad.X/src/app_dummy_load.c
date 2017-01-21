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

#include "app_dummy_load.h"

#include <stdint.h>
#include <system.h>
#include <usb/usb_device.h>

#include "max7219.h"

#define SCREEN_REFRESH_CYCLES 32
#define ADC_READ_CYCLES 2

static char need_measure = 0;
static char need_read_adc = 0;
static unsigned set_current = 0;
static unsigned stored_current = 0;

static int adc_accum = 0;
static int adc_counter = 0;

__EEPROM_DATA(0, 0, 0, 0, 0, 0, 0, 0);

/* TODO(sergey): Move to more generic module. */
static void EEPROM_Write(int addr, char data) {
  EEADR = addr;
  EEDATA = data;
  EECON1bits.EEPGD = 0;  /* Access data EEPROM memory. */
  EECON1bits.CFGS = 0;   /* Access Flash program or data EEPROM memory. */
  EECON1bits.WREN = 1;   /* Allows write cycles to Flash program/data EEPROM. */
  INTCONbits.GIE = 0;    /* Disable all interrupts. */
  EECON2 = 0x55;
  EECON2 = 0xAA;
  EECON1bits.WR = 1;     /* WR Control bit initiates write operation. */
  INTCONbits.GIE = 1;
  while(!PIR2bits.EEIF);
  PIR2bits.EEIF = 0;
  EECON1bits.WREN = 0;   /* Disable Writing to EEPROM. */
}

static unsigned char EEPROM_Read(int addr)
{
  EEADR = addr;
  EECON1bits.EEPGD = 0;  /* Access data EEPROM memory. */
  EECON1bits.CFGS = 0;   /* Access Flash program or data EEPROM memory. */
  EECON1bits.RD = 1;     /* EEPROM Read Enable Bit. */
  return EEDATA;
}

static void update_pwm() {
  CCPR2L = (set_current >> 2) & 0xff;
  CCP2CON = 0b00001100 | ((set_current & 0x3) << 5);
}

static void delay_50ms(int n) {
  int x;
  for (x = n; x > 0; --x) {
      __delay_ms(10);
      __delay_ms(10);
      __delay_ms(10);
      __delay_ms(10);
      __delay_ms(10);
  }
}

void APP_Initialize() {
  /* Read prevous value from EEPROM. */
  set_current = EEPROM_Read(1) << 8 | EEPROM_Read(0);
  stored_current = set_current;

  /* Configure output lines for LED driver. */
  TRISAbits.RA2 = 0;  /* DIN of the driver */
  TRISCbits.RC0 = 0;  /* CLK */
  TRISCbits.RC1 = 0;  /* LOAD */

  /* Configure input lines for rotary encoder. */
  TRISCbits.RC2 = 1;
  TRISCbits.RC6 = 1;
  TRISCbits.RC7 = 1;

  /* Configure ADC. */
  TRISAbits.RA0 = 1;  /* Sense wire is at RA0. */
  ADCON0 = 0b00000000;  /* Channel 0 (AN0), disabled yet. */
  ADCON1 = 0b00001110;  /* AN0 analog only, reference using Vss and Vdd. */
  ADCON2 = 0b00100010;  /* Left justified, Tacq=10*Tad, clock Fosc/32. */
  ADCON0bits.ADON = 1;  /* Enable A/D module. */

  /* Initialize LED driver. */

  /* Do a delay, so all transition processes are settled down. */
  delay_50ms(5);

  maxDriverStartupInitialize();

  /* Set per-digit decode mode.
   * - If bit for the digit is 0, no decode is used.
   * - If bit for the digit is 1, code B is used
   */
  maxDriverSendData(MAX_ADDRESS_DECODE_MODE, 0xff);

  /* Use three digits only. */
  maxDriverSendData(MAX_ADDRESS_SCAN_LIMIT, MAX_LIMIT_DIGITS_012);

  /* Half intensity. */
  maxDriverSendData(MAX_ADDRESS_INTENSITY, MAX_INTENSITY_17_32);

  /* Switch tp normal mode. */
  maxDriverSendData(MAX_ADDRESS_SHUTDOWN, MAX_MODE_NORMAL);

  /* Do a momentary LED test. */
  maxDriverSendData(MAX_ADDRESS_DISPLAY_TEST, 0x01);
  maxDriverSendData(MAX_ADDRESS_DIGIT2, 0);
  maxDriverSendData(MAX_ADDRESS_DIGIT1, 0);
  maxDriverSendData(MAX_ADDRESS_DIGIT0, MAX_CHAR_DOT_FLAG);
  delay_50ms(5);
  maxDriverSendData(MAX_ADDRESS_DISPLAY_TEST, 0x00);

  /* Set up timers. */
  T0CONbits.T08BIT = 0; /* 16 bit. */
  T0CONbits.T0CS = 0;   /* Internal clock. */
  T0CONbits.PSA = 1;    /* No prescaler. */
  INTCONbits.T0IF = 0;  /* Clear the flag */
  INTCONbits.T0IE = 1;  /* Enable the interrupt */
  T0CONbits.TMR0ON = 1;

  /* Timer2 is used for PWM. */
  T2CONbits.TOUTPS = 0;  /* No postscale. */
  T2CONbits.T2CKPS = 0;  /* No prescale. */
  T2CONbits.TMR2ON = 1;

  /* Configure PWM */
  TRISBbits.RB3 = 0;
  PR2 = 0xf9;
  update_pwm();

  /* Force current measure after system boot. */
  need_measure = SCREEN_REFRESH_CYCLES;
  need_read_adc = ADC_READ_CYCLES;
}

void APP_Interrupts() {
  if (INTCONbits.TMR0IE && INTCONbits.T0IF) {
    INTCONbits.T0IF = 0;
    ++need_measure;
    ++need_read_adc;
  }
}

void APP_Tasks() {
  static unsigned prev_encoder_a = 1;

  /* Check rotary encoder motion. */
  unsigned encoder_a = ENCODER_PORT_A;
  if (encoder_a != prev_encoder_a) {
    unsigned encoder_b = ENCODER_PORT_B;
    if (encoder_a == 1) {
      if (encoder_a != encoder_b) {
        if (set_current < 1023) {
          ++set_current;
        }
      }
      else {
        if (set_current > 0) {
          --set_current;
        }
      }
    }
    update_pwm();
    prev_encoder_a = encoder_a;
  }

  if (need_read_adc == ADC_READ_CYCLES) {
    /* Read the voltage. */
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO_nDONE);
    adc_accum += ADRESH;
    ++adc_counter;
    need_read_adc = 0;
  }

  if (need_measure == SCREEN_REFRESH_CYCLES) {
    if (ENCODER_BUT == 0) {
      float average = adc_accum / (float)adc_counter;
      int voltage = (int)(average / 255.0f * 500.0f / CURRENT_GAIN);
      adc_accum = 0;
      adc_counter = 0;
      need_read_adc = 0;

      maxDriverSendData(MAX_ADDRESS_DIGIT2, (voltage % 10));
      maxDriverSendData(MAX_ADDRESS_DIGIT1, (voltage / 10) % 10);
      maxDriverSendData(MAX_ADDRESS_DIGIT0, ((voltage / 100) % 10) | MAX_CHAR_DOT_FLAG);
    }
    else {
      maxDriverSendData(MAX_ADDRESS_DIGIT2, (set_current % 10));
      maxDriverSendData(MAX_ADDRESS_DIGIT1, (set_current / 10) % 10);
      maxDriverSendData(MAX_ADDRESS_DIGIT0, ((set_current / 100) % 10) | MAX_CHAR_DOT_FLAG);
    }

    /* If current changed, store it in EEPROM. */
    if (stored_current != set_current) {
      update_pwm();

      EEPROM_Write(0, set_current & 0xff);
      EEPROM_Write(1, (set_current >> 8) & 0xff);
      stored_current = set_current;
    }
    need_measure = 0;
  }
}
