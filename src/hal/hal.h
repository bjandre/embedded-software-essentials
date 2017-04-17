
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_HAL_H_
#define ESE_HAL_HAL_H_

/*
  Generic routine to initialize hardware.

  Wrapper around platform specific code.
 */
void initialize_hardware(void);

/*
  Generic routine to initialize RTC.

  Wrapper around platform specific code.
 */
void initialize_rtc(void);

/*
  Generic routine to initialize GPIO.

  Wrapper around platform specific code.
 */
void initialize_gpio(void);

/*
  Generic routine to initialize SPI.

  Wrapper around platform specific code.
 */
void initialize_spi(void);

/*
  Generic routine to initialize DMA.

  Wrapper around platform specific code.
 */
void initialize_dma(void);

#endif/* ESE_HAL_HAL_H_ */
