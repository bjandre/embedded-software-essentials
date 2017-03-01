#include <stdlib.h>

#include "frdm-kl25z.h"

#include "frdm-kl25z-defs.h"

#include "platform-generics.h"

#include "bit-operations.h"

BoardStatus BoardInitialize(void **the_board)
{
    BoardStatus status = Board_OK;
    board_t *board = *the_board;
    board = malloc(sizeof(board_t));
    if (NULL == board) {
        status = Board_Allocation_Error;
    } else {
        board->port_a = PORTA;
        board->port_b = PORTB;
        board->port_c = PORTC;
        board->port_d = PORTD;
        board->port_e = PORTE;

        board->gpio_a = GPIOA;
        board->gpio_b = GPIOB;
        board->gpio_c = GPIOC;
        board->gpio_d = GPIOD;
        board->gpio_e = GPIOE;
    }
    return status;
}

BoardStatus BoardInitializePort(void *board, PortNames port_name)
{

    BoardStatus status = Board_OK;

    switch (port_name) {
    case Port_A_Name:
        //board->port_A = PORTA;
        *SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
        break;
    case Port_B_Name:
        //board->port_B = PORTB;
        *SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
        break;
    case Port_C_Name:
        //board->port_C = PORTC;
        *SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
        break;
    case Port_D_Name:
        //board->port_D = PORTD;
        *SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
        break;
    case Port_E_Name:
        //board->port_E = PORTE;
        *SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
        break;
    default:
        status = Board_Unknown_Port;
    }
    return status;
}

BoardStatus BoardInitializeGPIO(void *board, PortNames port_name)
{
    BoardStatus status = Board_OK;

    status = BoardInitializePort(board, port_name);

    switch (port_name) {
    case Port_A_Name:
        //board->gpio_a = GPIOA;
        break;
    case Port_B_Name:
        //board->gpio_b = GPIOB;
        break;
    case Port_C_Name:
        //board->gpio_c = GPIOC;
        break;
    case Port_D_Name:
        //board->gpio_d = GPIOD;
        break;
    case Port_E_Name:
        //board->gpio_e = GPIOE;
        break;
    default:
        status = Board_Unknown_Port;
    }
    return status;
}


BoardStatus InitializeLoggingInterface(void *board)
{
    // Setup logging interface using UART0
    // UART0
    //   PTA1 - Port A, Pin 1, Alternate function 2, UART0_RX
    //   PTA2 - Port A, Pin 2, Alternate function 2, UART0_TX
    BoardStatus status = Board_OK;

    status = BoardInitializePort(board, Port_A_Name);
    // FIXME(bja, 2017-02) multi-bit fields. only works for setting ones/zeros
    // at initialization as originally hard coded! Won't change correctly!
    // set the uart clock source
    set_bit_one((uint32_t *)MCG_C1, MCG_C1_IRCLKEN_SHIFT);

    *SIM_SOPT2 |= SIM_SOPT2_UART0SRC_MCGFLLCLK_MASK; // clock source for uart0
    *SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;  // enable uart0
    // set the rx tx source
    *SIM_SOPT5 &= ~(3 << SIM_SOPT5_UART0TXSRC_SHIFT);
    *SIM_SOPT5 &= ~(1 << SIM_SOPT5_UART0RXSRC_SHIFT);

    // NOTE(bja, 2017-02) uart registers are 8 bit!

    // set baud rate
    uint32_t baud_rate = 19200U;
    uint8_t osr = ((uint32_t)(*UART0_C4)) & (0b00011111);
    uint32_t br_clock = 32000U;
    // baud rate is 13 bit field in registers
    uint16_t baud_rate_reg = br_clock / ((osr + 1) * baud_rate);
    uint8_t high_mask = (0b00011111 & (baud_rate_reg >> 8));
    *UART0_BDH |= high_mask; // set ones
    *UART0_BDH &= ~high_mask; // set zeros
    *UART0_BDL = *UART0_BDL;


    // set stop bit
    set_bit_zero((uint32_t *)UART0_BDH, UART0_BDH_SBNS_SHIFT);

    // set number of data bits
    set_bit_zero((uint32_t *)UART0_C1, UART0_C1_M_SHIFT); // 8 bit

    // set parity
    set_bit_zero((uint32_t *)UART0_C1, UART0_C1_PE_SHIFT); // no parity

    return status;
}
