#include "platform_defs.h"

#if (PLATFORM == PLATFORM_FRDM)
#include "initialize_frdm_kl25z.h"
#include "rtc_frdm_kl25z.h"
#include "gpio_frdm_kl25z.h"
#include "pwm_timer_frdm_kl25z.h"
#include "dma_frdm_kl25z.h"
#include "spi_frdm_kl25z.h"
#endif

#include "hal.h"

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
