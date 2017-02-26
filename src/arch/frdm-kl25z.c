#include <stdlib.h>

#include "frdm-kl25z.h"

#include "frdm-kl25z-defs.h"

#include "platform-generics.h"

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
