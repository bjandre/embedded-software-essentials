
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_APP_POST_NRF24L01_H_
#define ESE_APP_POST_NRF24L01_H_

#include "post-common.h"

/**
   Test that we can read the status register on the nRF24L01p
 */
POSTstatus post_nrf24_read_status();

/**
   Test that we can read and write to the config register on the nRF24L01p
 */
POSTstatus post_nrf24_read_write_config();

/**
 */
POSTstatus post_nrf24_read_write_rf_setup();

/**
 */
POSTstatus post_nrf24_read_write_rf_channel();

/**
   Test that we can read and write to the tx addr register on the nRF24L01p
 */
POSTstatus post_nrf24_read_write_tx_addr();

/**
 */
POSTstatus post_nrf24_read_fifo_status();
/**
 */
POSTstatus post_nrf24_flush_tx_fifo();
/**
 */
POSTstatus post_nrf24_flush_rx_fifo();
#endif // ESE_APP_POST_NRF24L01_H_
