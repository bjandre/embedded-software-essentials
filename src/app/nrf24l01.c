
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "nrf24l01.h"
#include "nrf24l01-const.h"

/**
   \param[in] register to read
   \return read command for the specified register
 */
NRF24_command nrf24_create_cmd_read_register(NRF24_register reg);

/**
   \param[in] register to write
   \return write command for the specified register
 */
NRF24_command nrf24_create_cmd_read_register(NRF24_register reg);

/**
   \param[in] pipe to write
   \return write command for the specified pipe
 */
NRF24_command nrf24_create_cmd_read_register(NRF24_register reg);


NRF24_command nrf24_create_cmd_read_register(NRF24_register reg)
{
    return NRF24_CMD_R_REGISTER | reg;
}

NRF24_command nrf24_create_cmd_write_register(NRF24_register reg)
{
    return NRF24_CMD_W_REGISTER | reg;
}

NRF24_command nrf24_create_cmd_write_ack_payload(uint8_t pipe)
{
    return NRF24_CMD_W_ACK_PAYLOAD | pipe;
}

uint8_t nrf24_read_register(NRF24_register reg)
{
    return 0;
}

void nrf24_write_register(NRF24_register reg, uint8_t value)
{
}

uint8_t nrf24_read_status(void)
{
    return 0;
}

void nrf24_write_config(uint8_t config)
{
}

uint8_t nrf24_read_config(void)
{
    return 0;
}

uint8_t nrf24_read_rf_setup(void)
{
    return 0;
}

void nrf24_write_rf_setup(uint8_t config)
{
}

uint8_t nrf24_read_rf_ch(void)
{
    return 0;
}

void nrf24_write_rf_ch(uint8_t channel)
{
}

uint8_t *nrf24_read_TX_ADDR(void)
{
    return NULL;
}

void nrf24_write_TX_ADDR(uint8_t *tx_addr)
{
}

uint8_t nrf24_read_fifo_status(void)
{
    return 0;
}

void nrf24_flush_tx_fifo(void)
{
}

void nrf24_flush_rx_fifo(void)
{
}

