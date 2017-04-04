#include "platform-defs.h"

#if (PLATFORM == PLATFORM_FRDM)
#include "initialize-frdm-kl25z.h"
#include "rtc-frdm-kl25z.h"
#include "gpio-frdm-kl25z.h"
#include "dma-frdm-kl25z.h"
#include "spi-frdm-kl25z.h"
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
#endif
}

void initialize_spi(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_spi();
#endif
}

void initialize_dma(void)
{
#if (PLATFORM == PLATFORM_FRDM)
    frdm_kl25z_initialize_dma();
#endif
}
