#ifndef ESE_MAIN_DEBUG_UART_DATA_H_
#define ESE_MAIN_DEBUG_UART_DATA_H_

#include <stddef.h>
#include <stdint.h>

// some interesting byte patterns to send over uart.
static const uint32_t data_dead_beef = 0xEFBEADDEu;
static const uint32_t data_dead_code = 0xDEC0ADDEu;
static const uint32_t data_aaaa_5555 = 0x5555AAAAu;
static const uint32_t data_negative_int = -40577;
static const char ese_hello[] =
    "Hello from Emebbed Software Essentials Project!\n";


/**
   debug_uart()

   Routine to send and receive some data for debugging the uart logger.

   Params: data_switch - flag to control which uart data test is run.

   Returns: none

 */
void debug_uart(uint8_t data_switch, uint8_t *buffer, size_t buffer_size);

/**
   debug_uart_transmit()

   Routine to send some fixed byte patterns to the logger

   Params: none

   Returns: none

 */
void debug_uart_transmit(void);

/**
   debug_uart()

   Routine to receive a byte and send back different data based on the received
   data. Note: this only works if the logging algorithm is set to polling.

   Params: none

   Returns: none

 */
void debug_uart_receive_transmit(uint8_t *buffer, size_t buffer_size);

#endif // ESE_MAIN_DEBUG_UART_DATA_H_
