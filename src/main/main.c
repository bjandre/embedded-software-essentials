#ifdef BARE_METAL
#define PRINTF(X)
#else
#define PRINTF(ARGS) printf(ARGS)
#include <stdio.h>
#endif

#include <stdint.h>
#include <stdlib.h>

#include "platform-defs.h"
#include "logger.h"

#include "debug-uart-data.h"

// Global, asynchronously accessed logger instance
volatile BinaryLogger_t logger;

// define the pins for the leds. FIXME(bja, 2017-03) Should be platform
// dependent!
typedef enum GPIO_PINS {
    LED_PIN_RED = 18,
    LED_PIN_GREEN = 19,
} GPIO_PINS;

/*
  initialize_gpio()

  Generic routine to initialize GPIO.

  Wrapper around platform specific code.

  Parameters: none

  Return: none
 */
void initialize_gpio(void);

/*
  initialize_led_pin()

  Generic routine to initialize an led pin.

  Wrapper around platform specific code.

  Parameters: led pin enumeration

  Return: none
 */
void initialize_led_pin(GPIO_PINS led_pin);

/*
  update_leds()

  Generic routine to update led status.

  Wrapper around platform specific code.

  Parameters: none

  Return: none
 */
void update_leds(void);

/*
  hello_logger()

  Output hello through the binary logger.

  Parameters: none

  Return: none
 */
void hello_logger(void);

#if (PLATFORM == PLATFORM_FRDM)
#include "MKL25Z4.h"

/*
  frdm_kl25z_initialize_gpio()

  Platform specific code to initialize GPIO.

  Parameters: none

  Return: none
 */
void frdm_kl25z_initialize_gpio(void);

/*
  frdm_kl25z_initialize_leds()

  Platform specific code to initialize leds.

  Parameters: gpio pin for the led

  Return: none
 */
void frdm_kl25z_initialize_led_pin(GPIO_PINS led_pin);

/*
  frdm_kl25z_update_leds()

  Platform specific code to update led state

  Parameters: none

  Return: none
 */
void frdm_kl25z_update_leds(void);
#endif


int main(int argc, char **argv)
{
    PRINTF("Hello from Emebbed Software Essentials Project!\n");

#if (PLATFORM == PLATFORM_FRDM)
    NVIC_EnableIRQ(UART0_IRQn);
#endif

    BinaryLoggerStatus logger_status = BinaryLogger_OK;
    logger_status = BinaryLoggerInitialize(32);
    if (BinaryLogger_OK != logger_status) {
        abort();
    }
    log_item_t *item;
    logger_status = CreateLogItem(&item, LOGGER_INITIALIZED, 0, NULL);
    if (BinaryLogger_OK != logger_status) {
        abort();
    }
    log_item(item);
    logger_status = DestroyLogItem(&item);

    initialize_gpio();

    logger_status = CreateLogItem(&item, SYSTEM_INITIALIZED, 0, NULL);
    if (BinaryLogger_OK != logger_status) {
        abort();
    }
    log_item(item);
    logger_status = DestroyLogItem(&item);

    if (0) {
        hello_logger();
    }

    size_t const buffer_size = 32 * sizeof(uint8_t);
    uint8_t *buffer = malloc(buffer_size);
    /* Add your code here */

    while (1) { /* main event loop */
        __asm("NOP"); /* breakpoint to stop while looping */
        update_leds();
#define DEBUG_UART
#ifdef DEBUG_UART
        uint8_t tx_or_rx = 0;
        debug_uart(tx_or_rx, buffer, buffer_size);
#endif
    }
    free(buffer);
    return 0;
}

void hello_logger(void)
{
    log_item_t *item;
    BinaryLoggerStatus logger_status = BinaryLogger_OK;
    char hello[] = "Hello from Emebbed Software Essentials Project!\n";
    logger_status = CreateLogItem(&item, INFO, sizeof(hello), &hello);
    if (BinaryLogger_OK != logger_status) {
        abort();
    }
    log_item(item);
    logger_status = DestroyLogItem(&item);
}

void initialize_gpio(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_gpio();
#endif
}
void initialize_led_pin(GPIO_PINS led_pin)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_led_pin(led_pin);
#else
    (void)led_pin;
#endif
}

void update_leds(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_update_leds();
#endif
}

#if (PLATFORM == PLATFORM_FRDM)
//
// platform specific code for the frdm-kl25z
//
void frdm_kl25z_initialize_gpio(void)
{
    // enable clock for gpio led pins.
    SIM->SCGC5 |= SIM_SCGC5_PORTB(1);

    initialize_led_pin(LED_PIN_RED);
    initialize_led_pin(LED_PIN_GREEN);
    // toggle led pins
    GPIOB->PTOR |= (1 << LED_PIN_GREEN);

    log_item_t *item;
    BinaryLoggerStatus logger_status = BinaryLogger_OK;
    logger_status = CreateLogItem(&item, GPIO_INITIALIZED, 0, NULL);
    if (BinaryLogger_OK != logger_status) {
        abort();
    }
    log_item(item);
    logger_status = DestroyLogItem(&item);
}

void frdm_kl25z_initialize_led_pin(GPIO_PINS led_pin)
{
    // Initialize the gpio pin for the led
    PORTB->PCR[led_pin] = PORT_PCR_MUX(1);
    // set pin to output
    GPIOB->PDDR |= (1 << led_pin);
    // initial state of pin is on
    GPIOB->PDOR |= (1 << led_pin);
}

void frdm_kl25z_update_leds(void)
{
    for (uint32_t i = 0; i < 200000; i++) {
        // do nothing for a while.
    }
    // toggle led pins
    GPIOB->PTOR |= (1 << LED_PIN_RED);
    GPIOB->PTOR |= (1 << LED_PIN_GREEN);
}

extern void UART0_IRQHandler(void)
{
    CircularBufferStatus cb_status = CB_No_Error;
    uint8_t byte;
    // What triggered the interrupt...
    if (UART0->S1 & UART0_S1_RDRF_MASK) {
        // received data register full
        byte = UART0->D;
        cb_status = CircularBufferAddItem(logger.receive_buffer, &byte);
        if (CB_No_Error == cb_status) {
            // do nothing? status flag is automatically reset
        } else {
            // error handling?
        }
    } else if (UART0->S1 & UART0_S1_TDRE_MASK) {
        // transmit data register empty
        cb_status = CircularBufferRemoveItem(logger.transmit_buffer, &byte);
        if (CB_No_Error == cb_status) {
            // successfully removed item.
            UART0->D = byte;
        } else {
            // nothing else to send
            UART0->C2 &= ~UART0_C2_TIE(1);
        }
    } else {
        // other interrupts to handle?
    }
}

#endif
