
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include <stdint.h>

#include "spi-peripheral-factory.h"

#include "nrf24l01.h"
#include "nrf24l01-const.h"
#include "async-global.h"

#if (PLATFORM == PLATFORM_FRDM)
#include "gpio-frdm-kl25z.h"
#else
#define PTD_NRF24_CHIP_ACTIVATE 0
#endif

extern volatile async_data_t global_async_data;

/**
   Update GPIO pin to activate the receiving chip
 */
static inline void nrf24_chip_activate(void)
{
    // active high
    {
        uint32_t interrupt_state = start_critical_region();
#if (PLATFORM == PLATFORM_FRDM)
        GPIOD->PSOR |= (1 << global_async_data.nrf24.chip_active_pin);
#endif
        end_critical_region(interrupt_state);
    }
}

/**
   Update GPIO pin to deactivate the receiving chip
 */
static inline void nrf24_chip_deactivate(void)
{
    // inactive low
    {
        uint32_t interrupt_state = start_critical_region();
#if (PLATFORM == PLATFORM_FRDM)
        GPIOD->PCOR |= (1 << global_async_data.nrf24.chip_active_pin);
#endif
        end_critical_region(interrupt_state);
    }
}

/**
   \param[in] register to read
   \return read command for the specified register
 */
static inline NRF24_command nrf24_create_cmd_read_register(NRF24_register reg)
{
    return NRF24_CMD_R_REGISTER | reg;
}

/**
   \param[in] register to write
   \return write command for the specified register
 */
static inline NRF24_command nrf24_create_cmd_write_register(NRF24_register reg)
{
    return NRF24_CMD_W_REGISTER | reg;
}

/**
   \param[in] pipe to write
   \return write command for the specified pipe
 */
NRF24_command nrf24_create_cmd_write_ack_payload(uint8_t pipe)
{
    return NRF24_CMD_W_ACK_PAYLOAD | pipe;
}

void nrf24_initialize(const size_t num_bytes_buffer)
{
    {
        uint32_t interrupt_state = start_critical_region();
        SPICreate(&(global_async_data.nrf24.spi), SPI_nRF24, num_bytes_buffer);
        end_critical_region(interrupt_state);
    }

    // GPIO radio enable line, active high, inactive low
    GPIO_PINS pin;
    {
        uint32_t interrupt_state = start_critical_region();
        // FIXME(bja, 2017-04) frdm-kl25z specific GPIO pin! Needs to be abstracted!
        global_async_data.nrf24.chip_active_pin = PTD_NRF24_CHIP_ACTIVATE;
        pin = global_async_data.nrf24.chip_active_pin;
        end_critical_region(interrupt_state);
    }
#if (PLATFORM == PLATFORM_FRDM)
    PORTD->PCR[pin] |= PORT_PCR_MUX(1);
    frdm_kl25z_initialize_port_d_output_pin(pin);
#endif
}

uint8_t nrf24_read_register(NRF24_register reg)
{
    nrf24_chip_activate();
    NRF24_command cmd[2];
    cmd[0] = nrf24_create_cmd_read_register(reg);
    cmd[1] = NRF24_CMD_NOP;
    {
        uint32_t interrupt_state = start_critical_region();
        global_async_data.nrf24.spi.polling_transmit_receive_n_bytes(
            &(global_async_data.nrf24.spi), (uint8_t *)cmd, sizeof(cmd));
        end_critical_region(interrupt_state);
    }
    nrf24_chip_deactivate();
    // return the second byte we received
    return cmd[1];
}

void nrf24_write_register(NRF24_register reg, uint8_t value)
{
    nrf24_chip_activate();
    NRF24_command cmd[2];
    cmd[0] = nrf24_create_cmd_write_register(reg);
    cmd[1] = value;
    {
        uint32_t interrupt_state = start_critical_region();
        global_async_data.nrf24.spi.polling_transmit_receive_n_bytes(
            &(global_async_data.nrf24.spi), (uint8_t *)cmd, sizeof(cmd));
        end_critical_region(interrupt_state);
    }
    nrf24_chip_deactivate();
}

uint8_t nrf24_read_status(void)
{
    nrf24_chip_activate();
    uint8_t byte = NRF24_CMD_NOP;
    {
        uint32_t interrupt_state = start_critical_region();
        global_async_data.nrf24.spi.polling_transmit_receive_byte(
            &(global_async_data.nrf24.spi), &byte);
        end_critical_region(interrupt_state);
    }
    nrf24_chip_deactivate();
    return byte;
}

void nrf24_write_config(uint8_t config)
{
    nrf24_write_register(NRF24_REG_CONFIG, config);
}

uint8_t nrf24_read_config(void)
{
    return nrf24_read_register(NRF24_REG_CONFIG);
}

uint8_t nrf24_read_rf_setup(void)
{
    return nrf24_read_register(NRF24_REG_RF_SETUP);
}

void nrf24_write_rf_setup(uint8_t config)
{
    nrf24_write_register(NRF24_REG_RF_SETUP, config);
}

uint8_t nrf24_read_rf_ch(void)
{
    return 0;
}

void nrf24_write_rf_ch(uint8_t channel)
{
    nrf24_write_register(NRF24_REG_RF_CH, channel);
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

