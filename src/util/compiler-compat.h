
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_UTIL_COMPILER_COMPAT_H_
#define ESE_UTIL_COMPILER_COMPAT_H_


#if defined(__GNUC__)
#define __INLINE         inline                                     /*!< inline keyword for GNU Compiler */
#define __STATIC_INLINE  static inline
#endif

#if defined(__STDC__)
/* we have a lovely c89 compiler.... */
#endif

#endif // ESE_UTIL_COMPILER_COMPAT_H_
