
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

typedef uint8_t NRF24_command;

static const NRF24_command NRF24_CMD_R_REGISTER = 0b00000000;
static const NRF24_command NRF24_CMD_W_REGISTER = 0b00100000;
static const NRF24_command NRF24_CMD_R_RX_PAYLOAD = 0b01100001;
static const NRF24_command NRF24_CMD_W_TX_PAYLOAD = 0b10100000;
static const NRF24_command NRF24_CMD_FLUSH_TX 0b11100001;
static const NRF24_command NRF24_CMD_FLUSH_RX = 0b11100010;
static const NRF24_command NRF24_CMD_REUSE_TX_PL = 0b11100011;
static const NRF24_command NRF24_CMD_ACTIVATE = 0b01010000;
static const NRF24_command NRF24_CMD_R_RX_PL_WID = 0b01100000;
static const NRF24_command NRF24_CMD_W_ACK_PAYLOAD = 0b10101000;
static const NRF24_command NRF24_CMD_W_TX_PAYLOAD_NOACK = 0b10110000;
static const NRF24_command NRF24_CMD_NOP = 0b11111111;

//!< Register map
typedef uint8_t NRF24_register;
typedef uint8_t NRF24_mask;

/**
   CONFIG: Configuration Register
*/
static const NRF24_register NRF24_REG_CONFIG = 0x00;

/** Mask interrupt caused by RX_DR, 1: Interrupt not reflected on the IRQ pin,
    0: Reflect RX_DR as active low interrupt on the IRQ pin */
static const NRF24_mask NRF24_CFG_PRIM_RX_MASK = (1 << 0);
/** Mask interrupt caused by TX_DS, 1: Interrupt not reflected on the IRQ pin,
    0: Reflect TX_DS as active low interrupt on the IRQ pin */
static const NRF24_mask NRF24_CFG_PWR_UP_MASK = (1 << 1);
/** Mask interrupt caused by MAX_RT, 1: Interrupt not reflected on the IRQ pin,
    0: Reflect MAX_RT as active low interrupt on the IRQ pin */
static const NRF24_mask NRF24_CFG_CRCO_MASK = (1 << 2);
/**  Enable CRC. Forced high if one of the bits in the EN_AA is high */
static const NRF24_mask NRF24_CFG_EN_CRC_MASK = (1 << 3);
/** CRC encoding scheme '0' - 1 byte, '1' – 2 bytes */
static const NRF24_mask NRF24_CFG_MASK_MAX_RT_MASK = (1 << 4);
/** 1: POWER UP, 0:POWER DOWN */
static const NRF24_mask NRF24_CFG_MASK_TX_DS_MASK = (1 << 5);
/** RX/TX control 1: PRX, 0: PTX */
static const NRF24_mask NRF24_CFG_MASK_RX_DR_MASK = (1 << 6);


/**
   EN_AA: Enhanced ShockBurst

   Enable ‘Auto Acknowledgment’
*/
static const NRF24_register NRF24_REG_EN_AA = 0x01;

/**
   EN_RXADDR: Enabled RX Addresses
 */
static const NRF24_register NRF24_REG_EN_RXADDR = 0x02;

/**
   SETUP_AW: Setup of Address Widths (common for all data pipes)
 */
static const NRF24_register NRF24_REG_SETUP_AW = 0x03;

/**
   SETUP_RETR: Setup of Automatic Retransmission
 */
static const NRF24_register NRF24_REG_SETUP_RETR = 0x04;
/**
   RF_CH: RF Channel
*/
static const NRF24_register NRF24_REG_RF_CH = 0x05;

/**
   RF_SETUP: RF Setup Register
 */
static const NRF24_register NRF24_REG_RF_SETUP = 0x06;

/**
   STATUS: Status Register (In parallel to the SPI command word applied on the
   MOSI pin, the STATUS reg- ister is shifted serially out on the MISO pin)
 */
static const NRF24_register NRF24_REG_STATUS = 0x07;

/**
   TX FIFO full flag.
   1: TX FIFO full.
   0: Available locations in TX FIFO.
 */
static const NRF24_mask NRF24_STATUS_TX_FULL = (1 << 0);
/**
   Data pipe number for the payload available for reading from RX_FIFO
   000-101: Data Pipe Number
   110: Not Used
   111: RX FIFO Empty
 */
static const NRF24_mask NRF24_STATUS_RX_P_NO = (1 << 1);
/**
   Maximum number of TX retransmits interrupt Write 1 to clear bit. If MAX_RT is
   asserted it must be cleared to enable further communication.
 */
static const NRF24_mask NRF24_STATUS_MAX_RT = (1 << 4);
/**
   Data Sent TX FIFO interrupt. Asserted when packet transmitted on TX. If
   AUTO_ACK is acti- vated, this bit is set high only when ACK is received.
   Write 1 to clear bit.
 */
static const NRF24_mask NRF24_STATUS_TX_DS = (1 << 5);
/**
   Data Ready RX FIFO interrupt. Asserted when new data arrives RX FIFOb. Write
   1 to clear bit.
 */
static const NRF24_mask NRF24_STATUS_RX_DR = (1 << 6);

/**
   OBSERVE_TX: Transmit observe register
 */
static const NRF24_register NRF24_REG_OBSERVE_TX = 0x08;
/**
   CD: Carrier Detect
 */
static const NRF24_register NRF24_REG_CD = 0x09;
/**
   RX_ADDR_P0: Receive address data pipe 0. 5 Bytes maximum length. (LSByte is
   written first. Write the number of bytes defined by SETUP_AW)
 */
static const NRF24_register NRF24_REG_RX_ADDR_P0 = 0x0A;
/**
   RX_ADDR_P1: Receive address data pipe 1. 5 Bytes maximum length. (LSByte is
   written first. Write the number of bytes defined by SETUP_AW)
 */
static const NRF24_register NRF24_REG_RX_ADDR_P1 = 0x0B;
/**
   RX_ADDR_P2: Receive address data pipe 2. 5 Bytes maximum length. (LSByte is
   written first. Write the number of bytes defined by SETUP_AW)
 */
static const NRF24_register NRF24_REG_RX_ADDR_P2 = 0x0C;
/**
   RX_ADDR_P3: Receive address data pipe 3. 5 Bytes maximum length. (LSByte is
   written first. Write the number of bytes defined by SETUP_AW)
 */
static const NRF24_register NRF24_REG_RX_ADDR_P3 = 0x0D;
/**
   RX_ADDR_P4: Receive address data pipe 4. 5 Bytes maximum length. (LSByte is
   written first. Write the number of bytes defined by SETUP_AW)
 */
static const NRF24_register NRF24_REG_RX_ADDR_P4 = 0x0E;
/**
   RX_ADDR_P5: Receive address data pipe 5. 5 Bytes maximum length. (LSByte is
   written first. Write the number of bytes defined by SETUP_AW)
 */
static const NRF24_register NRF24_REG_RX_ADDR_P5 = 0x0F;
/**
   TX_ADDR: Transmit address. Used for a PTX device only. (LSByte is written first)
 */
static const NRF24_register NRF24_REG_TX_ADDR = 0x10;
/**
   RX_PW_P0: Number of bytes in RX payload in data pipe 0 (1 to 32 bytes).
 */
static const NRF24_register NRF24_REG_RX_PW_P0 = 0x11;
/**
   RX_PW_P1: Number of bytes in RX payload in data pipe 1 (1 to 32 bytes).
 */
static const NRF24_register NRF24_REG_RX_PW_P1 = 0x12;
/**
   RX_PW_P2: Number of bytes in RX payload in data pipe 2 (1 to 32 bytes).
 */
static const NRF24_register NRF24_REG_RX_PW_P2 = 0x13;
/**
   RX_PW_P3: Number of bytes in RX payload in data pipe 3 (1 to 32 bytes).
 */
static const NRF24_register NRF24_REG_RX_PW_P3 = 0x14;
/**
   RX_PW_P4: Number of bytes in RX payload in data pipe 4 (1 to 32 bytes).
 */
static const NRF24_register NRF24_REG_RX_PW_P4 = 0x15;
/**
   RX_PW_P5: Number of bytes in RX payload in data pipe 5 (1 to 32 bytes).
 */
static const NRF24_register NRF24_REG_RX_PW_P5 = 0x16;
/**
   FIFO_STATUS: FIFO Status Register
 */
static const NRF24_register NRF24_REG_FIFO_STATUS = 0x17;
/**
   DYNPD: Enable dynamic payload length
 */
static const NRF24_register NRF24_REG_DYNPD = 0x1C;
/**
   FEATURE: Feature Register
 */
static const NRF24_register NRF24_REG_FEATURE = 0x1D;

#endif // ESE_APP_NRF24L01_H_
