
#ifndef CKRYPT_INTERN_H
#define CKRYPT_INTERN_H

#include <stdio.h>
#include "ckrypt.h"

/* the vtable for platform and lib procedures */
struct ckrypt_vtable {
	ULONG (WINAPI *RtlRandomEx)(PULONG seed);
	ULONG (WINAPI *GetAdaptersInfo)(PIP_ADAPTER_INFO, PULONG);
};

extern struct ckrypt_vtable vt;


inline int __big_endian(){
	short n = 0x100; return ((char*)(&n))[0] == 1;
}

inline int __little_endian(){
	short n = 0x1; return ((char*)(&n))[0] == 1;
}

inline unsigned __bswap32(unsigned int n)
{
	return ((n>>24)&0xff)|((n>>8)&0xff00)|((n<<8)&0xff0000)|((n<<24)&0xff000000);
}

inline unsigned rol32(unsigned int d, unsigned char n)
{
	return (d<<n)|(d>>(32-n));
}

inline unsigned ch32(unsigned x, unsigned y, unsigned z)
{
	return (x&y)^((~x)&z);
}

inline unsigned maj32(unsigned x, unsigned y, unsigned z)
{
	return (x&y)^(x&z)^(y&z);
}

#endif
