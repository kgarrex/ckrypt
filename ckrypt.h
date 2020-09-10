
#include "uuid.h"
#include "sha1.h"

typedef unsigned int ck_uint32_t;
typedef unsigned long long ck_uint64_t;


void ckrypt_init();

void ckrypt_term();


void ck_init()
{
}


unsigned ck_base64_encode(
	const char *in, unsigned insize,
	char out[], unsigned outsize);



unsigned ck_base64_decode(
	const char *in, unsigned insize,
	char out[], unsigned outsize);	


void ck_sha1(char digest[40], const char *msg, unsigned int len);
