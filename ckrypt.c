
#include "intern.h"


struct ckrypt_vtable vt;

HANDLE hmod[2];

void ckrypt_init()
{
	hmod[0] = LoadLibrary("C:\\Windows\\System32\\ntdll.dll");
	if(!hmod[0]){
	
	}
	vt.RtlRandomEx = (void*)GetProcAddress(hmod[0], "RtlRandomEx");


	hmod[1] = LoadLibrary("C:\\Windows\\System32\\advapi32.dll");
	if(!hmod[1]){
	
	}
	vt.GetAdaptersInfo = (void*)GetProcAddress(hmod[0], "GetAdaptersInfo");
}

void ckrypt_term()
{
	FreeLibrary(hmod[0]);
	FreeLibrary(hmod[1]);
}

/**
 * @return full length of number in buffer
**/
int __cdecl itoa_zt (int n, char* buf,
unsigned char bufsize, unsigned char padding,
unsigned char base)
{
//check that the base is valid
	char *ptr = buf,
	 *ptr1 = buf,
	 tmp_char,
	 _buf[16];
	int nn, i = 0;

	if(!buf) return 0;
	if(base < 2 || base > 36) return 0;

	do {
	nn = n;
	n /= base;

	*ptr++ = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210" //71 chars
		 "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		[35 + (nn-n*base)];
		i++;
	} while(n);

	while(i < padding) {
		*ptr++ = '0';
		i++;
	}

	//Apply negative sign
	if(nn < 0) {
		*ptr++ = '-'; /* todo: check size left */
		i++;
	}
	*ptr-- = '\0';

	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return i;
}


