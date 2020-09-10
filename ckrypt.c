
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
