
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

#include "platform-defs.h"
#if (PLATFORM == PLATFORM_FRDM) || (PLATFORM == PLATFORM_BBB)
#include "nrf24l01.h"
#include "nrf24l01-const.h"
#endif

#include "post-common.h"
#include "post-nrf24l01.h"

/**
   \file post-nrf24l01.c

   Series of test to ensure we can talk to the nrF24L01+ over SPI.

   Note: all tests in this suite assume that we are testing all the registers
   are in their restart state after power up.
 */

POSTstatus post_nrf24_read_status()
{
    POSTstatus status = POST_SKIPPED;
#if (PLATFORM == PLATFORM_FRDM) || (PLATFORM == PLATFORM_BBB)
    uint8_t data = 0x00;
    uint8_t expected = 0x0E;
    nrf24_read_status(&data);
    assert(data == expected);
    status = POST_PASS;
#endif
    return status;
}

POSTstatus post_nrf24_read_write_config()
{
    POSTstatus status = POST_SKIPPED;
#if (PLATFORM == PLATFORM_FRDM) || (PLATFORM == PLATFORM_BBB)
    /* read the default state */
    uint8_t data = 0x00;
    uint8_t expected = 0x08;
    nrf24_read_config(&data);
    assert(data == expected);

    /* set a different value in the register */
    data |= NRF24_CFG_PWR_UP_MASK;
    expected |= NRF24_CFG_PWR_UP_MASK;
    nrf24_write_config(data);

    /* read back the changed value */
    data = 0x00;
    nrf24_read_config(&data);
    assert(data == expected);

    /* now restore to the original state */
    data &= ~NRF24_CFG_PWR_UP_MASK;
    nrf24_write_config(data);
    status = POST_PASS;
#endif
    return status;
}

POSTstatus post_nrf24_read_write_rf_setup()
{
    POSTstatus status = POST_SKIPPED;
#if (PLATFORM == PLATFORM_FRDM) || (PLATFORM == PLATFORM_BBB)
    /* read the default state */
    uint8_t data = 0x00;
    uint8_t expected = 0x0F;
    nrf24_read_rf_setup(&data);
    assert(data == expected);

    /* set a different value in the register */
    data &= ~NRF24_REG_RF_SETUP_RF_DR(1);
    expected = data;
    nrf24_write_rf_setup(data);

    /* read back the changed value */
    data = 0x00;
    nrf24_read_rf_setup(&data);
    assert(data == expected);

    /* now restore to the original state */
    data |= NRF24_REG_RF_SETUP_RF_DR(1);
    nrf24_write_rf_setup(data);
    status = POST_PASS;
#endif
    return status;
}

POSTstatus post_nrf24_read_write_rf_channel()
{
    POSTstatus status = POST_SKIPPED;
#if (PLATFORM == PLATFORM_FRDM) || (PLATFORM == PLATFORM_BBB)
    /* read the default state */
    uint8_t data = 0x00;
    uint8_t expected = 0x02;
    nrf24_read_rf_ch(&data);
    assert(data == expected);

    /* set a different value in the register */
    data |= 0x01;
    expected = data;
    nrf24_write_rf_ch(data);

    /* read back the changed value */
    data = 0x00;
    nrf24_read_rf_ch(&data);
    assert(data == expected);

    /* now restore to the original state */
    data &= ~0x01;
    nrf24_write_rf_ch(data);
    status = POST_PASS;
#endif
    return status;
}

POSTstatus post_nrf24_read_write_tx_addr()
{
    POSTstatus status = POST_SKIPPED;
#if (PLATFORM == PLATFORM_FRDM) || (PLATFORM == PLATFORM_BBB)
    /* read the default state */
    uint64_t tx_addr = 0x0;
    uint64_t expected = 0xE7E7E7E7E7;
    nrf24_read_TX_ADDR((NRF24_size_t)sizeof(tx_addr), (uint8_t *)&tx_addr);
    assert(tx_addr == expected);


    /* set a different value in the register */
    tx_addr = 0x7E7E7E7E7E;
    expected = tx_addr;
    nrf24_write_TX_ADDR((NRF24_size_t)sizeof(tx_addr), (uint8_t *)&tx_addr);

    /* read back the changed value */
    tx_addr = 0x0;
    nrf24_read_TX_ADDR((NRF24_size_t)sizeof(tx_addr), (uint8_t *)&tx_addr);
    uint64_t diff = tx_addr - 0x7E7E7E7E7E;
    assert(diff == 0);

    /* now restore to the original state */
    tx_addr = 0xE7E7E7E7E7;
    nrf24_write_TX_ADDR((NRF24_size_t)sizeof(tx_addr), (uint8_t *)&tx_addr);
    status = POST_PASS;
#endif
    return status;
}

POSTstatus post_nrf24_read_fifo_status()
{
    POSTstatus status = POST_SKIPPED;
#if (PLATFORM == PLATFORM_FRDM) || (PLATFORM == PLATFORM_BBB)
    /* read the default state */
    uint8_t data = 0x00;
    uint8_t expected = 0x11;
    nrf24_read_fifo_status(&data);
    assert(data == expected);
    status = POST_PASS;
#endif
    return status;
}

POSTstatus post_nrf24_flush_tx_fifo()
{
    POSTstatus status = POST_SKIPPED;
#if (PLATFORM == PLATFORM_FRDM) || (PLATFORM == PLATFORM_BBB)
    nrf24_flush_tx_fifo();
    status = POST_PASS;
#endif
    return status;
}

POSTstatus post_nrf24_flush_rx_fifo()
{
    POSTstatus status = POST_SKIPPED;
#if (PLATFORM == PLATFORM_FRDM) || (PLATFORM == PLATFORM_BBB)
    nrf24_flush_rx_fifo();
    status = POST_PASS;
#endif
    return status;
}
