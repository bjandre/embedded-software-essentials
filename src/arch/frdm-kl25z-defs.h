#ifndef ESE_ARCH_FRDM_KL25Z_DEFS_H_
#define ESE_ARCH_FRDM_KL25Z_DEFS_H_

// FIXME(bja, 2017-02) how much of this is KL25Z specific, and how much is generic ARM cortex M0+...?

// Source: KL25_Family_ReferenceManual_P80M48SF0RM.pdf

//
// Chapter 11 - Port control and interrupts
//

// Section 5, Port Memory Map Table pg. 177
#define PORTA (port_control_registers_t *)0x40049000
#define PORTB (port_control_registers_t *)0x4004A000
#define PORTC (port_control_registers_t *)0x4004B000
#define PORTD (port_control_registers_t *)0x4004C000
#define PORTE (port_control_registers_t *)0x4004D000

// below not needed, handled by the port_control_registers_t
// #define PORTA_PCR0 (uint32_t*)0x40049000
// #define PORTA_PCR1 (uint32_t*)0x40049004
// #define PORTA_PCR2 (uint32_t*)0x40049008

// Section 11.5.1 - Pin Control Register n, pg 184
#define PCR_ISF_SHIFT 24

#define PCR_IRQC_SHIFT 16
#define PCR_IRQC_DMA_DISABLED_MASK 0x0000 // Interrupt/DMA request disabled.
#define PCR_DMA_RISING_MASK 0b0001 // DMA request on rising edge.
#define PCR_DMA_FALLING_MASK 0b0010 // DMA request on falling edge.
#define PCR_DMA_EITHER_MASK 0b0011 // DMA request on either edge.
#define PCR_IRQC_ZERO_MASK 0b1000 // Interrupt when logic zero.
#define PCR_IRQC_RISING_MASK 0b1001 // Interrupt on rising edge.
#define PCR_IRQC_FALLING_MASK 0b1010 // Interrupt on falling edge.
#define PCR_IRQC_EITHER_MASK 0b1011 // Interrupt on either edge.
#define PCR_IRQC_ONE_MASK 0b1100 // Interrupt when logic one.

#define PCR_MUX_SHIFT 8
#define PCR_MUX_PIN_DISABLED 0b000
#define PCR_MUX_ALT1_GPIO (0b001 << MUX_BIT_SHIFT) // 0x00000100
#define PCR_MUX_ALT2 0b010
#define PCR_MUX_ALT3 0b011
#define PCR_MUX_ALT4 0b100
#define PCR_MUX_ALT5 0b101
#define PCR_MUX_ALT6 0b110
#define PCR_MUX_ALT7 0b111

#define PCR_DSE_SHIFT 6 // drive strength enable, 0 = low, 1 = high
#define PCR_PFE_SHIFT 4 // passive filter enable, 0 = disabled, 1 = enabled
#define PCR_SRE_SHIFT 2 // slew rate enable, 0 = fast, 1 = slow
#define PCR_PE_SHIFT 1 // pull up/down enabled, 0 = disabled, 1 = enabled
#define PCR_PS_SHIFT 0 // pull select, 0 = pull down, 1 = pull up

// Section 11.5.2 - Global Pin Control Low Register (PORTx_GPCLR)

// Section 11.5.3 - Global Pin Control High Register (PORTx_GPCHR)

// Section 11.5.4 - Interrupt Status Flag Register (PORTx_ISFR)

//
// Chapten 12 - System Integration Module
//

// SOPT - System Options Register
// Section 5.4 Clock definitions
//   MCG - Multipurpose Clock Generator
//       MCGIRCLK - internal reference clock, slow or fast
//       MCGFLLCLK - frequency loop lock
//       MCGPLLCLK - phase loop lock
//   OSC - System Oscillator
//       OSCERCLK - external reference clock

// Section 12.2.1 - System Options Register 1 (SIM_SOPT1)
#define SIM_SOPT1 (uint32_t*)0x40047000

// Section 12.2.2 - SOPT1 Configuration Register (SIM_SOPT1CFG)
#define SIM_SOPT1CFG (uint32_t*)0x4004_7004

// Section 12.2.2 - System Options Register 2 (SIM_SOPT2)
#define SIM_SOPT2 (uint32_t*)0x40048004
#define SIM_SOPT2_SRC_DISABLED // 00 - disable clock
#define SIM_SOPT2_SRC_MCGFLLCLK // 01 - mcg internal reference clock
#define SIM_SOPT2_SRC_OSCERCLK // 10 - external referenc clock
#define SIM_SOPT2_SRC_MCGIRCLK // 11 - mcg internal clock
#define SIM_SOPT2_UART0SRC_SHIFT 26 // clock source select for uart0
#define SIM_SOPT2_UART0SRC_DISABLED_MASK ~(3 << SIM_SOPT2_UART0SRC_SHIFT)
#define SIM_SOPT2_UART0SRC_MCGFLLCLK_MASK (1 << SIM_SOPT2_UART0SRC_SHIFT)
#define SIM_SOPT2_UART0SRC_OSCERCLK_MASK (2 << SIM_SOPT2_UART0SRC_SHIFT)
#define SIM_SOPT2_UART0SRC_MCGIRCLK_MASK (3 << SIM_SOPT2_UART0SRC_SHIFT)

// 12.2.4 System Options Register 4 (SIM_SOPT4)

// 12.2.5 System Options Register 5 (SIM_SOPT5)
#define SIM_SOPT5 (uint32_t*)0x40048010
#define SIM_SOPT5_UART0TXSRC_SHIFT 0
#define SIM_SOPT5_UART0TXSRC_UART0_TX_pin 0  // 00
#define SIM_SOPT5_UART0TXSRC_UART0_TX_pin_TPM1 1 // 01
#define SIM_SOPT5_UART0TXSRC_UART0_TX_pin_TPM2 2 // 10
#define SIM_SOPT5_UART0TXSRC_Reserved 11
#define SIM_SOPT5_UART0RXSRC_SHIFT 2
#define SIM_SOPT5_UART0RXSRC_UART0_RX_pin 0
#define SIM_SOPT5_UART0RXSRC_CMP0_output 1
#define SIM_SOPT5_UART1TXSRC
#define SIM_SOPT5_UART1RXSRC
#define SIM_SOPT5_UART0ODE
#define SIM_SOPT5_UART1ODE
#define SIM_SOPT5_UART2ODE

// 12.2.6 System Options Register 7 (SIM_SOPT7)

// 12.2.7 System Device Identification Register (SIM_SDID)

// 12.2.8 System Clock Gating Control Register 4 (SIM_SCGC4)
#define SIM_SCGC4 (uint32_t*)0x40048034
#define SIM_SCGC4_I2C0_MASK (1 << 6)
#define SIM_SCGC4_I2C1_MASK (1 << 7)
#define SIM_SCGC4_UART0_MASK (1 << 10)
#define SIM_SCGC4_UART1_MASK (1 << 11)
#define SIM_SCGC4_UART2_MASK (1 << 12)
#define SIM_SCGC4_USBOTG_MASK (1 << 18)
#define SIM_SCGC4_CMP_MASK (1 << 19)
#define SIM_SCGC4_SPI0_MASK (1 << 22)
#define SIM_SCGC4_SPI1_MASK (1 << 23)

// 12.2.9 System Clock Gating Control Register 5 (SIM_SCGC5)
#define SIM_SCGC5 (uint32_t*)0x40048038
#define SIM_SCGC5_PORTA_MASK (uint32_t)(1 << 9)
#define SIM_SCGC5_PORTB_MASK (uint32_t)(1 << 10)
#define SIM_SCGC5_PORTC_MASK (uint32_t)(1 << 11)
#define SIM_SCGC5_PORTD_MASK (uint32_t)(1 << 12)
#define SIM_SCGC5_PORTE_MASK (uint32_t)(1 << 13)

// 12.2.10 System Clock Gating Control Register 6 (SIM_SCGC6)

// 12.2.11 System Clock Gating Control Register 7 (SIM_SCGC7)

// 12.2.12 System Clock Divider Register 1 (SIM_CLKDIV1)

// 12.2.13 Flash Configuration Register 1 (SIM_FCFG1)

// 12.2.14 Flash Configuration Register 2 (SIM_FCFG2)

// 12.2.15 Unique Identification Register Mid-High (SIM_UIDMH)

// 12.2.16 Unique Identification Register Mid Low (SIM_UIDML)

// 12.2.17 Unique Identification Register Low (SIM_UIDL)

// 12.2.18 COP Control Register (SIM_COPC)

// 12.2.19 Service COP Register (SIM_SRVCOP)

//
// Chapter 13 - System Mode Controller (SMC)
//

//
// Chapter 14 - Power Management Controller (PMC)
//

//
// Chapter 15 - Low-Leakage Wakeup Unit (LLWU)
//

//
// Chapter 16 - Reset Control Module (RCM)
//

//
// Chapter 18 - Miscellaneous Control Module (MCM)
//

//
// Chapter 19 - Micro Trace Buffer (MTB)
//

//
// Chapter 20 - Crossbar Switch Lite (AXBS-Lite)
//

//
// Chapter 21 - Peripheral Bridge (AIPS-Lite)
//

//
// Chapter 22 - Direct Memory Access Multiplexer (DMAMUX)
//

//
// Chapter 23 - DMA Controller Module
//

//
// Chapter 24 - Multipurpose Clock Generator (MCG)
//

// NOTE control registers are 8 bits!
#define MCG_C1 (uint8_t*) 0x40064000
#define MCG_C1_IRCLKEN_SHIFT 0 // internal reference clock, 0=disable, 1=enabled, 

#define MCG_C2 (uint8_t*) 0x40064001
#define MCG_C2_IRCS_SHIFT 0 // internal reference clock select, 0=slow 32 kHz IRC, 1=fast (4 MHz IRC)

//
// Chapter 25 - Oscillator (OSC)
//

//
// Chapter 26 - Flash Memory Controller (FMC)
//

//
// Chapter 27 - Flash Memory Module (FTFA)
//

//
// Chapter 28 - Analog-to-Digital Converter (ADC)
//

//
// Chapter 29 - Comparator (CMP)
//

//
// Chapter 30 - 12-bit Digital-to-Analog Converter (DAC)
//

//
// Chapter 31 - Timer/PWM Module (TPM)
//

//
// Chapter 32 - Periodic Interrupt Timer (PIT)
//

//
// Chapter 33 - Low-Power Timer (LPTMR)
//

//
// Chapter 34 - Real Time Clock (RTC)
//

//
// Chapter 35 - Universal Serial Bus OTG Controller (USBOTG)
//

//
// Chapter 36 - USB Voltage Regulator
//

//
// Chapter 37 - Serial Peripheral Interface (SPI)
//

//
// Chapter 38 - Inter-Integrated Circuit (I2C)
//

//
// Chapter 39 - Universal Asynchronous Receiver/Transmitter (UART0)
//

// All UART registers are 8 bit!
#define UART0_BDH (uint8_t*) 0x4006A000 // UART Baud Rate Register High
#define UART0_BDH_LBKDIE_SHIFT 7 // LIN break detect interrupt enable, 0 = disabled, 1 = enabled
#define UART0_BDH_RXEDGIE_SHIFT 6 // RX Input Active Edge Interrupt Enable (for RXEDGIF), 0 = disabled, 1 = enabled
#define UART0_BDH_SBNS_SHIFT 5 // Stop Bit Number Select, 0 = 1 stop bit, 1 = two stop bits
#define UART0_BDH_SBR_SHIFT 0 // Baud Rate Modulo Divisor

// Baud Rate Modulo Divisor, The 13 bits in SBR[12:0] are referred to
// collectively as BR, and they set the modulo divide rate for the baud rate
// generator. When BR is 1 - 8191, the baud rate equals baud clock / ((OSR+1) ×
// BR).

#define UART0_BDL (uint8_t*) 0x4006A001 // UART Baud Rate Register Low
#define UART0_BDL_SBR_SHIFT 0

#define UART0_C1 (uint8_t*) 0x4006A002 // UART Control Register 1
#define UART0_C1_LOOPS_SHIFT 7 // Loop Mode Select, 0 = normal rx-tx separate pins, 1 = single wire
#define UART0_C1_DOSEEN_SHIFT 6 // Doze Enable, 0 = enabled in wait mode, 1 = disabled in waite mode
#define UART0_C1_RSRC_SHIFT 5 // Receiver Source Select
#define UART0_C1_M_SHIFT 4 // 9-bit or 8-bit mode select, 0 = rx and tx use 8-bit data characters, 1 = 9 bit
#define UART0_C1_WAKE_SHIFT 3 // Receiver Wakeup Method Select, 0 = idle line wakeup, 1 = address-mark wakeup
#define UART0_C1_ILT_SHIFT 2 // Idle Line Type Select - 0 = idle character count starts after start bit, 1 = after stop bit.
#define UART0_C1_PE_SHIFT 1 // Parity Enable - 0 = no hardware parity generation/checking, 1 = parity enabled
#define UART0_C1_PT_SHIFT 0 // Parity Type - 0 = even parity, 1 = odd parity

#define UART0_C2 (uint8_t*) 0x4006A003 // UART Control Register 2
#define UART0_C2_TIE_SHIFT 7 // Transmission Interrupt Enable for TDRE, 0 = disabled, 1 = enabled
#define UART0_C2_TCIE_SHIFT 6 // Transmission complete Interrupt enable, 0 = hardware interrupt disabled, 1 = enabled
#define UART0_C2_RIE_SHIFT 5 // Receiver Interrupt Enable for RDRF, 0 = hardware interrupt disabled, 1 = enabled
#define UART0_C2_ILIE_SHIFT 4 // Idle line interrupt enable, 0 = hardware interrupts from IDEL disabled, 1 = enabled
#define UART0_C2_TE_SHIFT 3 // Transmitter Enable, 0 = transmitter disabled, 1 = transmitter enabled
#define UART0_C2_RE_SHIFT 2 // Receiver Enable, 0 = receiver disabled, 1 = enabled
#define UART0_C2_RWU_SHIFT 1 // Receiver Wakeup Control, 0 = normal
#define UART0_C2_SBK_SHIFT 0 // Send break, 0 = normal

#define UART0_S1 (uint8_t*) 0x4006A004 // UART Status Register 1
#define UART0_S1_TDRE_SHIFT 7 // Transmit data register empty flag, 0 = transmit data buffer full, 1 = buffer empty
#define UART0_S1_TC_SHIFT 6 // Transmission Complete Flag, 0 = transmitter active, 1 = transmitter idle
#define UART0_S1_RDRF_SHIFT 5 // Receive Data Register Full Flag, 0 = receive data buffer empty, 1 = full
#define UART0_S1_IDLE_SHIFT 4 // Idle Line Flag - 0 = no idle line, 1 = idle line
#define UART0_S1_OR_SHIFT 3 // Receiver Overrun Flag - 0 = no overnun, 1 = overrun new data lost
#define UART0_S1_NF_SHIFT 2 // Noise Flag, 0 = no noise detected, 1 = noise detected
#define UART0_S1_FE_SHIFT 1 // Framing Error Flag, 0 = no error, 1 = error
#define UART0_S1_PF_SHIFT 0 // Parity Error Flag, 0 = no error, 1 = error

#define UART0_S2 (uint8_t*) 0x4006A005 // UART Status Register 2
#define UART0_S2_LBKDIF_SHIFT 7 // LIN Break detect interrupt flag
#define UART0_S2_RXEDGIF_SHIFT 6 // RX pin active edge interrupt flag
#define UART0_S2_MSBF_SHIFT 5 // MSB first
#define UART0_S2_RXINV_SHIFT 4 // receive data inversion
#define UART0_S2_RWUID_SHIFT 3 // receive wakeup idle detection
#define UART0_S2_BRK13_SHIFT 2 // break character generation length
#define UART0_S2_LBKDE_SHIFT 1 // LIN break detection enable 
#define UART0_S2_RAF_SHIFT 0 // Receiver active flag

#define UART0_C3 (uint8_t*) 0x4006A006 // UART Control Register 3

#define UART0_D (uint8_t*) 0x4006A007 // UART Data Register, two separate buffers, contents of the read only receive buffer , or contents of the write only transmit buffer.
#define UART0_MA1 (uint8_t*) 0x4006A008 // UART Match Address Registers 1
#define UART0_MA2 (uint8_t*) 0x4006A009 // UART Match Address Registers 2

#define UART0_C4 (uint8_t*) 0x4006A00A // UART Control Register 4
// Over Sampling Ratio bits 4-0 4x-32x. 4x=(00011), 32=(11111), default 16=(01111)
#define UART0_C4_OSR_SHIFT 0

#define UART0_C5 (uint8_t*) 0x4006A00B // UART Control Register 5

//
// Chapter 40 - Universal Asynchronous Receiver/Transmitter (UART1 and UART2)
//

//
// Chapter 41 General-Purpose Input/Output (GPIO)
//

// Locations of specific registers handled by gpio_registers_t.
// PDOR, PSOR, PCOR, PTOR, PDIR, PDDR

#define GPIOA (gpio_registers_t *)0x400FF000
#define GPIOB (gpio_registers_t *)0x400FF040
#define GPIOC (gpio_registers_t *)0x400FF080
#define GPIOD (gpio_registers_t *)0x400FF0C0
#define GPIOE (gpio_registers_t *)0x400FF100

//
// Chapter 42 - Touch Sensing Input (TSI)
//


#endif  // ESE_ARCH_FRDM_KL25Z_DEFS_H_

