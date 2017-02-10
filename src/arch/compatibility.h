#ifndef __ESE_COMPATIBILITY_H__
#define __ESE_COMPATIBILITY_H__

// compiler for the kl25z treates uint32_t as a long unsigned int. Other
// platforms treat it as an unsinged int.

#define STRINGIFY(STR) #STR
#define FMT(fmt) STRINGIFY(fmt)

#ifdef UINT32_FMT
#define UI32 FMT(UINT32_FMT)
#else
#define UI32 FMT(u)
#endif

#endif // __ESE_COMPATIBILITY_H__
