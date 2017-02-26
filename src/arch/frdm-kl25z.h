#ifndef ESE_ARCH_FRDM_KL25Z_H_
#define ESE_ARCH_FRDM_KL25Z_H_

#include <stddef.h>
#include <stdint.h>

#include "platform-generics.h"

/** FRDM-KL25Z development board

 Chip: MKL25Z128VLK4 in an 80 LQFP package
       32-bit ARM Cortex-M0+ core
       128 KB flash
       16 KB SRAM
       4 MHz and 32 kHz internal reference clock

       Serial port connection between the OpenSDA MCU and pins PTA1 and PTA2 of the KL25Z. PTA1 and PTA2 must be set to alternate function 2

*/

typedef enum BoardStatus {
    Board_OK,
    Board_ERROR, // generic unspecified error
    Board_Allocation_Error,
    Board_Unknown_Port,
} BoardStatus;

/*
** Port control registers
*/
#define PCR_NUM_PORTS 32
#define PCR_ISFR_OFFSET 24
static const size_t port_size = 32;

// Chapter 11 Port control and interrupts, Section 5, Port Memory Map Table pg. 177
//
// unused section, not explicitly stated, but appears to be consistent across ports
typedef struct port_control_registers_t {
    volatile uint32_t pcr[PCR_NUM_PORTS];
    volatile uint32_t gpclr; // global port control low register
    volatile uint32_t gcphr; // global port control high register
    volatile uint32_t _unused_[PCR_ISFR_OFFSET];
    volatile uint32_t isfr; // interrupt status flag register
} port_control_registers_t;

typedef struct gpio_registers_t {
    volatile uint32_t pdor; // PDOR port data output registers
    volatile uint32_t psor; // PSOR port set output registers
    volatile uint32_t pcor; // PCOR port clear output registers
    volatile uint32_t ptor; // PTOR port toggle output register
    volatile uint32_t pdir; // PDIR port data input registers
    volatile uint32_t pddr; // PDDR port data direction registers
} gpio_registers_t;


typedef struct board_t {
    volatile port_control_registers_t *port_a;
    volatile port_control_registers_t *port_b;
    volatile port_control_registers_t *port_c;
    volatile port_control_registers_t *port_d;
    volatile port_control_registers_t *port_e;

    volatile gpio_registers_t *gpio_a;
    volatile gpio_registers_t *gpio_b;
    volatile gpio_registers_t *gpio_c;
    volatile gpio_registers_t *gpio_d;
    volatile gpio_registers_t *gpio_e;
} board_t;


BoardStatus BoardInitialize(void **board);
BoardStatus BoardInitializePort(void *board, PortNames portname);
BoardStatus BoardInitializeGPIO(void *board, PortNames portname);

#endif // ESE_ARCH_FRDM_KL25Z_H_
