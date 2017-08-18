
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
#    if defined(__STDC__)
#        if defined(__STDC_VERSION__)
#            if (__STDC_VERSION__ >= 199901L)
#ifdef __STATIC_INLINE
#undef __STATIC_INLINE
#endif
#ifdef __INLINE
#undef __INLINE
#endif
#                define __INLINE         inline        /*!< inline keyword for GNU Compiler */
#                define __STATIC_INLINE  static inline
#            endif
#        else
/* we have a lovely c89 compiler.... */
#ifdef __STATIC_INLINE
#undef __STATIC_INLINE
#endif
#ifdef __INLINE
#undef __INLINE
#endif
#        define __INLINE
#        define __STATIC_INLINE  static
#       endif
#    endif
#endif

#endif/* ESE_UTIL_COMPILER_COMPAT_H_ */
