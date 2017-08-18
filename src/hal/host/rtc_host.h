
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_HAL_HOST_RTC_H_
#define ESE_HAL_HOST_RTC_H_

/*
  Platform specific routine to initialize RTC.
 */
void host_initialize_rtc(void);

/*
  Dummy ISR for the host to manually trigger a heartbeat
 */
void host_initialize_rtc(void);


#endif/* ESE_HAL_HOST_RTC_H_ */
