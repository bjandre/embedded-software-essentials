
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "MKL25Z4.h"

#include "memory-common.h"
#include "memory-cpu.h"
#include "dma-frdm-kl25z.h"
#include "async-global.h"

/**
   DMA MUX sources are defined in the KL25Z subfamily reference manual section
   3.4.8.1, Table 3.20

   Unlisted values are reserved and not available on the KL25Z.
*/
typedef enum {
    Channel_disabled = 0,
    UART0_Receive = 2,
    UART0_Transmit = 3,
    UART1_Receive = 4,
    UART1_Transmit = 5,
    UART2_Receive = 6,
    UART2_Transmit = 7,
    SPI0_Receive = 16,
    SPI0_Transmit = 17,
    SPI1_Receive = 18,
    SPI1_Transmit = 19,
    I2C_0 = 22,
    I2C_1 = 23,
    TPM0_Channel0 = 24,
    TPM0_Channel1 = 25,
    TPM0_Channel2 = 26,
    TPM0_Channel3 = 27,
    TPM0_Channel4 = 28,
    TPM0_Channel5 = 29,
    TPM1_Channel0 = 32,
    TPM1_Channel1 = 33,
    TPM2_Channel0 = 34,
    TPM2_Channel1 = 35,
    ADC_0 = 40,
    CMP_0 = 41,
    DAC_0 = 45,
    PCM_PORTA = 49,
    PCM_PORTD = 52,
    TPM0_Overflow = 54,
    TPM1_Overflow = 55,
    TPM2_Overflow = 56,
    TSI = 57,
    ALWAYS_ENABLE0 = 60,
    ALWAYS_ENABLE1 = 61,
    ALWAYS_ENABLE2 = 62,
    ALWAYS_ENABLE3 = 63
} DMAMUX_SRC;

static const uint8_t channel_m2m = 2;

typedef enum {
    DMA_SIZE_32BIT = 0U,
    DMA_SIZE_8BIT = 1U,
    DMA_SIZE_16BIT = 2U,
} DMA_TRANSFER_SIZE;

/**
   Get the correct bit pattern for the transfer size.
 */
uint8_t get_transfer_size_indicator(uint8_t bytes_per_item);


void frdm_kl25z_initialize_dma(void)
{
    NVIC_EnableIRQ(DMA2_IRQn);

    // enable the clocks for the DMA multiplexer and the DMA.
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX(1);
    SIM->SCGC7 |= SIM_SCGC7_DMA(1);

    // assign a source to each channel. Note: channels 0 and 1 can have a
    // periodic trigger.

    // set channel 2 for memory to memory transfer by setting to always
    // enable.
    DMAMUX0->CHCFG[channel_m2m] |= DMAMUX_CHCFG_ENBL(1) | DMAMUX_CHCFG_SOURCE(
                                       ALWAYS_ENABLE0);

}


uint8_t get_transfer_size_indicator(uint8_t bytes_per_item)
{
    uint8_t size_indicator;
    if (bytes_per_item == 4) {
        size_indicator = DMA_SIZE_32BIT;
    } else if (bytes_per_item == 2) {
        size_indicator = DMA_SIZE_16BIT;
    } else {
        size_indicator = DMA_SIZE_8BIT;
    }
    return size_indicator;
}


MemStatus frdm_kl25z_memmove_dma(
    uint8_t *destination, uint8_t const *const source,
    uint32_t const num_items, uint8_t const bytes_per_item)
{
    if (NULL == destination || NULL == source) {
        return MemStatus_Null_Pointer;
    }

    if (source == destination) {
        // do nothing
    } else if (source < destination && destination < source + num_items) {
        // NOTE(bja, 2017-03) requires copying backward from end of source (not
        // possible with DMA) or copying the overlapped region, wasteful, may
        // not be possible on memory constrained system.... For now just call
        // memmove_cpu...
        memmove_cpu(destination, source, num_items * bytes_per_item);
    } else {
        // setup dma to copy forward from start of source.

        // is the channel busy?
        uint32_t status = DMA0->DMA[channel_m2m].DSR_BCR & DMA_DSR_BCR_BSY(1);
        // is there a pending request but the channel isn't selected?
        status = DMA0->DMA[channel_m2m].DSR_BCR & DMA_DSR_BCR_REQ(1);
        if (status) {
            abort();
        }

        // stop the DMA channel and clear status?
        DMA0->DMA[channel_m2m].DSR_BCR = DMA_DSR_BCR_DONE(1);

        // FIXME(bja, 2017-03) assert num_items > 0, return?
        DMA0->DMA[channel_m2m].SAR = DMA_SAR_SAR(source);
        DMA0->DMA[channel_m2m].DAR = DMA_DAR_DAR(destination);

        uint32_t num_bytes = num_items * bytes_per_item;
        // number of bytes to copy
        DMA0->DMA[channel_m2m].DSR_BCR |= DMA_DSR_BCR_BCR(num_bytes);

        // clear control register
        DMA0->DMA[channel_m2m].DCR = 0x0U;
        // enable interrupt
        DMA0->DMA[channel_m2m].DCR |= DMA_DCR_EINT(1);
        // source increment
        DMA0->DMA[channel_m2m].DCR |= DMA_DCR_SINC(1);
        uint8_t size_indicator = get_transfer_size_indicator(bytes_per_item);
        // one byte source size
        DMA0->DMA[channel_m2m].DCR |= DMA_DCR_SSIZE(size_indicator);
        // one byte destination increment
        DMA0->DMA[channel_m2m].DCR |= DMA_DCR_DINC(1);
        // one byte destination size
        DMA0->DMA[channel_m2m].DCR |= DMA_DCR_DSIZE(size_indicator);

        set_global_async_dma_complete(false);

        DMA0->DMA[channel_m2m].DCR |= DMA_DCR_START(1);

    }

    return MemStatus_Success;
}

MemStatus frdm_kl25z_memset_dma(
    uint8_t *destination, uint8_t const *const source,
    uint32_t const num_items, uint8_t const bytes_per_item)
{
    if (NULL == destination || NULL == source) {
        return MemStatus_Null_Pointer;
    }
    if (bytes_per_item != 1 &&
            bytes_per_item != 2 &&
            bytes_per_item != 4) {
        return MemStatus_Transfer_Size_Error;
    }
    // is the channel busy?
    uint32_t status = DMA0->DMA[channel_m2m].DSR_BCR & DMA_DSR_BCR_BSY(1);
    // is there a pending request but the channel isn't selected?
    status = DMA0->DMA[channel_m2m].DSR_BCR & DMA_DSR_BCR_REQ(1);
    if (status) {
        abort();
    }

    // stop the DMA channel and clear status?
    DMA0->DMA[channel_m2m].DSR_BCR = DMA_DSR_BCR_DONE(1);

    // FIXME(bja, 2017-03) assert num_items > 0, return?
    DMA0->DMA[channel_m2m].SAR = DMA_SAR_SAR(source);
    DMA0->DMA[channel_m2m].DAR = DMA_DAR_DAR(destination);

    uint32_t num_bytes = num_items * bytes_per_item;
    // number of bytes to copy
    DMA0->DMA[channel_m2m].DSR_BCR |= DMA_DSR_BCR_BCR(num_bytes);

    // clear control register
    DMA0->DMA[channel_m2m].DCR = 0x0U;
    // enable interrupt
    DMA0->DMA[channel_m2m].DCR |= DMA_DCR_EINT(1);
    // no source increment
    DMA0->DMA[channel_m2m].DCR &= ~DMA_DCR_SINC(1);
    uint8_t size_indicator = get_transfer_size_indicator(bytes_per_item);
    // one byte source size
    DMA0->DMA[channel_m2m].DCR |= DMA_DCR_SSIZE(size_indicator);
    // one byte destination increment
    DMA0->DMA[channel_m2m].DCR |= DMA_DCR_DINC(1);
    // one byte destination size
    DMA0->DMA[channel_m2m].DCR |= DMA_DCR_DSIZE(size_indicator);

    set_global_async_dma_complete(false);

    DMA0->DMA[channel_m2m].DCR |= DMA_DCR_START(1);

    return MemStatus_Success;
}

extern void DMA2_IRQHandler(void)
{
    uint32_t status = 0;
    // bus error on source?
    status += DMA0->DMA[channel_m2m].DSR_BCR & DMA_DSR_BCR_BES(1);
    // bus error on destination?
    status += DMA0->DMA[channel_m2m].DSR_BCR & DMA_DSR_BCR_BED(1);
    // dma configuration error?
    status += DMA0->DMA[channel_m2m].DSR_BCR & DMA_DSR_BCR_CE(1);
    if (status) {
        abort();
    }
    // clear the done flag
    DMA0->DMA[channel_m2m].DCR &= ~DMA_DCR_EINT(1); // enable interrupt
    DMA0->DMA[channel_m2m].DSR_BCR |= DMA_DSR_BCR_DONE(1);

    set_global_async_dma_complete(true);
}
