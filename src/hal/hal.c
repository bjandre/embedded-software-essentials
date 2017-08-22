#include "platform_defs.h"

#if (PLATFORM == PLATFORM_FRDM)
#include "initialize_frdm_kl25z.h"
#include "rtc_frdm_kl25z.h"
#include "gpio_frdm_kl25z.h"
#include "pwm_timer_frdm_kl25z.h"
#include "dma_frdm_kl25z.h"
#include "spi_frdm_kl25z.h"
#include "shutdown_frdm_kl25z.h"
#elif (PLATFORM == PLATFORM_BBB)
#include "gpio_bbb.h"
#endif

#include "hal.h"

/**
   Shutdown the GPIO hardware
 */
void shutdown_gpio(void);

void initialize_hardware(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize();
#endif
}

void initialize_rtc(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_rtc();
#endif
}

void initialize_gpio(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_gpio();
    frdm_kl25z_initialize_pwm_timer();
#elif (PLATFORM == PLATFORM_BBB)
    gpio_bbb_initialize();
#endif
}

void initialize_spi(void)
{
#if (PLATFORM == PLATFORM_FRDM)

#endif
}

void initialize_dma(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_dma();
#endif
}

void shutdown_hardware(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_shutdown();
#endif
    shutdown_gpio();
}

void shutdown_gpio(void)
{
#if (PLATFORM == PLATFORM_BBB)
    gpio_bbb_shutdown();
#endif
}
