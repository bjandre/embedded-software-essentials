#include <stddef.h>
#include <stdint.h>

#include "logger.h"

#include "debug-uart-data.h"

void debug_uart_silence_compiler_warnings(void)
{
    (void)null_payload;
}

void debug_uart(uint8_t tx_or_rx, uint8_t *buffer, size_t buffer_size)
{
    if (tx_or_rx) {
        debug_uart_transmit();
    } else {
        debug_uart_receive_transmit(buffer, buffer_size);
    }
}

void debug_uart_transmit(void)
{
    uint8_t byte = 0x55u;
    log_data(sizeof(uint8_t), &byte);
    log_data(sizeof(data_dead_beef), (uint8_t *)(&data_dead_beef));
    byte = 0xAAu;
    log_data(sizeof(uint8_t), &byte);
    log_data(sizeof(data_dead_code), (uint8_t *)(&data_dead_code));
    byte = 0xAAu;
    log_string((uint8_t *)ese_hello);
    byte = 0xAAu;
    log_integer(data_negative_int);
}

void debug_uart_receive_transmit(uint8_t *buffer, size_t buffer_size)
{
    // NOTE(bja, 2017-03) this recives a byte and makes a decision about what to
    // transmit back based on that byte. It needs polling instead of interrupt
    // logging to make sense.
    uint8_t byte;
    log_receive_data(sizeof(byte), &byte);
    switch (byte) {
    case '1':
        log_data(sizeof(data_dead_beef), (uint8_t *)(&data_dead_beef));
        break;
    case '2':
        log_data(sizeof(data_dead_code), (uint8_t *)(&data_dead_code));
        break;
    case '3':
        log_data(sizeof(data_aaaa_5555), (uint8_t *)(&data_aaaa_5555));
        break;
    case 'a':
        log_receive_data(sizeof(byte), &byte);
        uint8_t num_bytes = byte - '0';
        if (num_bytes > buffer_size) {
            num_bytes = buffer_size;
        }
        log_receive_data((size_t)num_bytes, buffer);
        log_data(sizeof(num_bytes), (uint8_t *)(&num_bytes));
        log_data(num_bytes, buffer);
        break;
    case 's':
        byte = 0x55;
        log_data(sizeof(uint8_t), &byte);
        log_integer(-4577);
        log_data(sizeof(uint8_t), &byte);
        log_string((uint8_t *)"\n");
        log_data(sizeof(uint8_t), &byte);
        break;
    default:
        //log_data(sizeof(byte), &byte);
        break;
    }
}
