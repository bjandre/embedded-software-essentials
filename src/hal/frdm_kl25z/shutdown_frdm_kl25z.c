
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "MKL25Z4.h"

#include "shutdown_frdm_kl25z.h"

void frdm_kl25z_shutdown(void)
{
    NVIC_SystemReset();

}

