
#ifndef CKRYPT_H
#define CKRYPT_H

#include <windows.h>
#include <iphlpapi.h>

// max bytes in a 64 bit number
#define CKRYPT_MAX_BYTES 0x1fffffffffffffff

#define EXCEEDS_MAX_BYTES(h,l) (l<0xffffffff?0:((h<=0x1fffffff)?0:1))


typedef struct {
	char *pout;
	unsigned outsz;	
	const char *table;
	int cache;
	char ncache;
	int iter;
	int error;
} ckrypt_base64_ctx;
		

/*
typedef struct {
	union {

	};
} ckrypt_ctx;
*/


typedef unsigned int ck_uint32_t;
typedef unsigned long long ck_uint64_t;

typedef __int64 uuid_node_t;
typedef __int64 uuid_time_t;
typedef short   uuid_seq_t;


typedef unsigned char ck_uint8_t;
typedef unsigned int ck_uint32_t;
typedef unsigned long long ck_uint64_t;


void ckrypt_init();

void ckrypt_term();


unsigned ck_base64_encode(
	const char *in, unsigned insize,
	char out[], unsigned outsize);


unsigned ck_base64_decode(
	const char *in, unsigned insize,
	char out[], unsigned outsize);	


void ck_sha1(char digest[40], const char *msg, unsigned int len);


#undef uuid_t
typedef struct {
	unsigned int time_low;
	unsigned short time_mid;
	unsigned short time_hi_and_version;
	unsigned char clock_seq_hi_and_reserved;
	unsigned char clock_seq_low;
	char node[6];
} uuid_t;


void uuid_create_v1(uuid_t *);

void uuid_string(uuid_t *, char uuidstr[36]);

void ck_sha1(char digest[20], const char *msg, ck_uint32_t len);

int test_sha1();

#endif
