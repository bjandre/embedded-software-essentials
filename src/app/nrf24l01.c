
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include <assert.h>
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
    GPIO_PINS pin = PTD_NRF24_CHIP_ACTIVATE;
    {
        uint32_t interrupt_state = start_critical_region();
        // FIXME(bja, 2017-04) frdm-kl25z specific GPIO pin! Needs to be abstracted!
        global_async_data.nrf24.chip_active_pin = pin;
        end_critical_region(interrupt_state);
    }
#if (PLATFORM == PLATFORM_FRDM)
    PORTD->PCR[pin] |= PORT_PCR_MUX(1);
    frdm_kl25z_initialize_port_d_output_pin(pin);
#endif
}


void nrf24_read_register(NRF24_register reg, NRF24_size_t num_bytes,
                         NRF24_data *data)
{
    //
    // load the SPI transmit buffer with the data we want to send
    //

    // first byte of send packet is the read command
    uint8_t cmd = nrf24_create_cmd_read_register(reg);
    {
        uint32_t interrupt_state = start_critical_region();
        CircularBufferAddItem(global_async_data.nrf24.spi.transmit_buffer, &cmd);
        end_critical_region(interrupt_state);
    }
    // need to send num_bytes of no-op to received num_bytes
    for (uint8_t i = 0; i < num_bytes; i++) {
        cmd = NRF24_CMD_NOP;
        {
            uint32_t interrupt_state = start_critical_region();
            CircularBufferAddItem(global_async_data.nrf24.spi.transmit_buffer, &cmd);
            end_critical_region(interrupt_state);
        }
    }

    //
    // send the buffered data
    //
    NRF24_size_t total_bytes = NRF24_1_byte +
                               num_bytes; // one byte command + payload
    nrf24_chip_activate();
    {
        uint32_t interrupt_state = start_critical_region();
        global_async_data.nrf24.spi.polling_transmit_receive_n_bytes(
            &(global_async_data.nrf24.spi), total_bytes);
        end_critical_region(interrupt_state);
    }
    nrf24_chip_deactivate();

    //
    // extract the buffered data to send back to the caller
    //

    // first byte is the automatic status byte
    uint8_t byte;
    bool is_empty = true;
    while (is_empty) {
        {
            uint32_t interrupt_state = start_critical_region();
            CircularBufferIsEmpty(
                global_async_data.nrf24.spi.receive_buffer, &is_empty);
            end_critical_region(interrupt_state);
        }
    }
    {
        uint32_t interrupt_state = start_critical_region();
        CircularBufferRemoveItem(
            global_async_data.nrf24.spi.receive_buffer, &byte);
        end_critical_region(interrupt_state);
    }
    // FIXME(bja, 2017-04) some sort of error checking on status? Should
    // probably be done lower in the stack.

    // read the actual data bytes that are returned to the caller
    for (uint8_t i = 0; i < num_bytes; i++) {
        bool is_empty = true;
        while (is_empty) {
            {
                uint32_t interrupt_state = start_critical_region();
                CircularBufferIsEmpty(
                    global_async_data.nrf24.spi.receive_buffer, &is_empty);
                end_critical_region(interrupt_state);
            }
        }
        {
            uint32_t interrupt_state = start_critical_region();
            CircularBufferRemoveItem(
                global_async_data.nrf24.spi.receive_buffer, &byte);
            end_critical_region(interrupt_state);
        }
        *(data + i) = byte;
    }

}

void nrf24_write_register(NRF24_register reg, uint8_t num_bytes,
                          NRF24_data *data)
{
    //
    // load the SPI transmit buffer with the data we want to send
    //

    // first byte of send packet is the read command
    uint8_t cmd = nrf24_create_cmd_write_register(reg);
    {
        uint32_t interrupt_state = start_critical_region();
        CircularBufferAddItem(global_async_data.nrf24.spi.transmit_buffer, &cmd);
        end_critical_region(interrupt_state);
    }
    // need to send num_bytes of data
    for (uint8_t i = 0; i < num_bytes; i++) {
        {
            uint32_t interrupt_state = start_critical_region();
            CircularBufferAddItem(global_async_data.nrf24.spi.transmit_buffer,
                                  (data + i));
            end_critical_region(interrupt_state);
        }
    }

    //
    // send the buffered data
    //
    NRF24_size_t total_bytes = NRF24_1_byte +
                               num_bytes; // one byte command + payload
    nrf24_chip_activate();
    {
        uint32_t interrupt_state = start_critical_region();
        global_async_data.nrf24.spi.polling_transmit_receive_n_bytes(
            &(global_async_data.nrf24.spi), total_bytes);
        end_critical_region(interrupt_state);
    }
    nrf24_chip_deactivate();

    // FIXME(bja, 2017-04) Check return codes to ensure packet send successfully!

    //
    // we received total_bytes into the receive buffer. We need to excract them
    // from the buffer even though there is nothing to send back to the caller.
    //

    // first byte is the automatic status byte
    uint8_t byte;
    bool is_empty = true;
    while (is_empty) {
        {
            uint32_t interrupt_state = start_critical_region();
            CircularBufferIsEmpty(
                global_async_data.nrf24.spi.receive_buffer, &is_empty);
            end_critical_region(interrupt_state);
        }
    }
    {
        uint32_t interrupt_state = start_critical_region();
        CircularBufferRemoveItem(
            global_async_data.nrf24.spi.receive_buffer, &byte);
        end_critical_region(interrupt_state);
    }
    // FIXME(bja, 2017-04) some sort of error checking on status? Should
    // probably be done lower in the stack.

    // read the actual data bytes that are returned to the caller
    for (uint8_t i = 0; i < num_bytes; i++) {
        bool is_empty = true;
        while (is_empty) {
            {
                uint32_t interrupt_state = start_critical_region();
                CircularBufferIsEmpty(
                    global_async_data.nrf24.spi.receive_buffer, &is_empty);
                end_critical_region(interrupt_state);
            }
        }
        {
            uint32_t interrupt_state = start_critical_region();
            CircularBufferRemoveItem(
                global_async_data.nrf24.spi.receive_buffer, &byte);
            end_critical_region(interrupt_state);
        }
        *(data + i) = byte;
    }

}

void nrf24_read_status(uint8_t *status)
{
    nrf24_read_register(NRF24_REG_STATUS, NRF24_1_byte, status);
}

void nrf24_write_config(uint8_t config)
{
    nrf24_write_register(NRF24_REG_CONFIG, NRF24_1_byte, &config);
}

void nrf24_read_config(uint8_t *config)
{
    nrf24_read_register(NRF24_REG_CONFIG, NRF24_1_byte, config);
}

void nrf24_read_rf_setup(uint8_t *setup)
{
    nrf24_read_register(NRF24_REG_RF_SETUP, NRF24_1_byte, setup);
}

void nrf24_write_rf_setup(uint8_t setup)
{
    nrf24_write_register(NRF24_REG_RF_SETUP, NRF24_1_byte, &setup);
}

void nrf24_read_rf_ch(uint8_t *channel)
{
    nrf24_read_register(NRF24_REG_RF_CH, NRF24_1_byte, channel);
}

void nrf24_write_rf_ch(uint8_t channel)
{
    nrf24_write_register(NRF24_REG_RF_CH, NRF24_1_byte, &channel);
}

void nrf24_read_TX_ADDR(NRF24_size_t buffer_size, uint8_t *tx_addr)
{
    assert(buffer_size >= NRF24_TX_ADDR_SIZE);
    nrf24_read_register(NRF24_REG_TX_ADDR, NRF24_TX_ADDR_SIZE, tx_addr);
}

void nrf24_write_TX_ADDR(NRF24_size_t buffer_size, uint8_t *tx_addr)
{
    assert(buffer_size >= NRF24_TX_ADDR_SIZE);
    nrf24_write_register(NRF24_REG_TX_ADDR, NRF24_TX_ADDR_SIZE, tx_addr);
}

void nrf24_read_fifo_status(uint8_t *data)
{
    nrf24_read_register(NRF24_REG_FIFO_STATUS, NRF24_1_byte, data);
}

void nrf24_flush_tx_fifo(void)
{
    uint8_t data;
    nrf24_write_register(NRF24_CMD_FLUSH_TX, NRF24_0_byte, &data);
}

void nrf24_flush_rx_fifo(void)
{
    uint8_t data;
    nrf24_write_register(NRF24_CMD_FLUSH_RX, NRF24_0_byte, &data);
}

