
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_APP_NRF24L01_H_
#define ESE_APP_NRF24L01_H_

#include <stdint.h>

#include "gpio_common.h"

#include "spi_peripheral.h"

#include "nrf24l01_const.h"


typedef struct _nrf24l01p {
    spi_peripheral_t spi;
    gpio_pin_num_t chip_active_pin;
} nrf24l01p_t;


typedef uint8_t NRF24_data;

/**
   Initialize the nRF24L01+
   \param[in] number of bytes in the FIFO tx rx buffers.
*/
void nrf24_initialize(const size_t num_bytes_buffer);

/**
   Read the register and return the value
   \param[in] register to be read
   \return contents of the specified register
*/
void nrf24_read_register(NRF24_register reg,
                         NRF24_size_t num_bytes, NRF24_data *data);

/**
   Write to the given register with the data.
   \param[in] reg register to be writen
   \param[in] data to be written
 */
void nrf24_write_register(NRF24_register reg,
                          NRF24_size_t num_bytes, NRF24_data *data);

/**
   Reads the STATUS register
   \return contents of the STATUS register
 */
void nrf24_read_status(uint8_t *status);

/**
   Write to CONFIG register
   \param[in] value to write to the config register
 */
void nrf24_write_config(uint8_t config);

/**
   Read the CONFIG register
   \return contents of the CONFIG register
*/
void nrf24_read_config(uint8_t *config);

/**
   Reads RF_SETUP register
   \return contents of the RF_SETUP register
*/
void nrf24_read_rf_setup(uint8_t *setup);

/**
   Writes to the RF_SETUP register
   \param[in] contents to write to the RF_SETUP register
*/
void nrf24_write_rf_setup(uint8_t setup);

/**
   Reads RF_CH register
   \return contents of the RF_SETUP register
*/
void nrf24_read_rf_ch(uint8_t *channel);

/**
   Writes to the RF_CH register
   \param[in] channel to use
*/
void nrf24_write_rf_ch(uint8_t channel);

/**
   Reads the 8 bytes of the TX_ADDR register
   \return contents of the  tx_addr register
*/
void nrf24_read_TX_ADDR(NRF24_size_t buffer_size, uint8_t *tx_addr);

/**
   Writes the 8 byte TX_ADDR register
   \param[in] tx_addr pointer to TX_ADDR register
*/
void nrf24_write_TX_ADDR(NRF24_size_t buffer_size, uint8_t *tx_addr);

/**
   Reads FIFO_STATUS register
   \return fifo status
*/
void nrf24_read_fifo_status(uint8_t *data);

/**
   Sends the command FLUSH_TX
*/
void nrf24_flush_tx_fifo(void);

/**
   Sends the command FLUSH_RX
*/
void nrf24_flush_rx_fifo(void);

#endif/* ESE_APP_NRF24L01_H_ */
