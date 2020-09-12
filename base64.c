#include "intern.h"


unsigned ckrypt_base64_encode_start(ckrypt_base64_ctx *ctx,
	char out[], unsigned outsz, const char t[64])
{
	ctx->table = t;
	ctx->pout  = out;
	ctx->outsz = outsz;
	ctx->ncache = 0;
	ctx->iter  = 0;
	return 1;
}

unsigned ckrypt_base64_encode_input(ckrypt_base64_ctx *ctx,
	const char *in, unsigned nbits)
{
	char *ptr = ctx->pout;
	const char *t = ctx->table;
	unsigned x, i, r, l;

	i = nbits/24; r = nbits%24;
	l = (i*4)+(r?4:0); //calculate num bytes needed

	if(ctx->outsz < l) return 0;

	if(__little_endian()){
		while(i--){
			x = *(int*)in;
			x = (__bswap32(x) & 0xffffff00) >> ;
			*ptr++ = t[x>>26];
			*ptr++ = t[(x>>20)&0x3f];
			*ptr++ = t[(x>>14)&0x3f];
			*ptr++ = t[(x>>8)&0x3f];
			in+=3;
		}
	}
	else {
		while(i--){
			x = *(int*)in;
			*ptr++ = t[x>>26];
			*ptr++ = t[(x>>20)&0x3f];
			*ptr++ = t[(x>>14)&0x3f];
			*ptr++ = t[(x>>8)&0x3f];
			in+=3;
		}
	}
	//*ptr++ = t[x>>j];

	if(r > 18){
	
	}
	else if(r > 12){
	
	}

	ctx->iter++;
	return 1;
}

unsigned ckrypt_base64_encode_end(ckrypt_base64_ctx *ctx)
{
	char *ptr = ctx->pout;

	if(!ctx->ncache) return 0;

	/*
	if(ctx->r == 2){
		x = *(int*)in;	
		x = __bswap32(x) & 0xffff0000;
		*ptr++ = t[x>>26];
		*ptr++ = t[(x>>20)&0x3f];
		*ptr++ = t[(x>>14)&0x3f];
		*ptr++ = '=';
	}
	else {
		x = *(int*)in;
		x = __bswap32(x) & 0xff000000;	
		*ptr++ = t[x>>26];
		*ptr++ = t[(x>>20)&0x3f];
		*ptr++ = '=';
		*ptr++ = '=';
	}
	*/
	
	return 1;
}

static unsigned
base64_encode_compute(const char *in, unsigned insize, char out[], unsigned outsize, const char t[64])
{
	char *ptr = out;
	unsigned x, i, r, l;

	insize = insize == 0xffffffff ? strlen(in) : insize;
	
	i = insize/3;
	r = insize%3;
	l = (i*4)+(r?4:0); //caculate outsize needed 
	if(outsize < l) return 0;


	if(__little_endian()){
		while(i--){
			x = *(int*)in;
			x = __bswap32(x);
			*ptr++ = t[x>>26];
			*ptr++ = t[(x>>20)&0x3f];
			*ptr++ = t[(x>>14)&0x3f];
			*ptr++ = t[(x>>8)&0x3f];
			in+=3;
		}
		if(r == 2){
			x = *(int*)in;
			x = __bswap32(x) & 0xffff0000;
			*ptr++ = t[x>>26];
			*ptr++ = t[(x>>20)&0x3f];
			*ptr++ = t[(x>>14)&0x3f];
			*ptr++ = '=';
		}
		else {
			x = *(int*)in;
			x = __bswap32(x) & 0xff000000;	
			*ptr++ = t[x>>26];
			*ptr++ = t[(x>>20)&0x3f];
			*ptr++ = '=';
			*ptr++ = '=';
		}
	}
	else {

		while(i--){
			x = *(int*)in;
			*ptr++ = t[x>>26];
			*ptr++ = t[(x>>20)&0x3f];
			*ptr++ = t[(x>>14)&0x3f];
			*ptr++ = t[(x>>8)&0x3f];
			in+=3;
		}
		if(r == 2){
			x = *(int*)in & 0xffff0000;
			*ptr++ = t[x>>26];
			*ptr++ = t[(x>>20)&0x3f];
			*ptr++ = t[(x>>14)&0x3f];
			*ptr++ = '=';
		}
		else {
			x = *(int*)in & 0xff000000;
			*ptr++ = t[x>>26];
			*ptr++ = t[(x>>20)&0x3f];
			*ptr++ = '=';
			*ptr++ = '=';
		}
	}

	return l;

}

unsigned base64_encode(const char *in, unsigned insize, char out[], unsigned outsize)
{
	const char table[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef"
			"ghijklmnopqrstuvwxyz0123456789+/";

	return base64_encode_compute(in, insize, out, outsize, table);
}

unsigned base64url_encode(const char *in, unsigned insize, char out[], unsigned outsize)
{
	const char table[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef"
			"ghijklmnopqrstuvwxyz0123456789-_";

	return base64_encode_compute(in, insize, out, outsize, table); 

}

#define B64_CALC_SIZE(n) (n+(4-(n%4)))

static unsigned
base64_decode_compute(const char *in, unsigned insize, char out[], unsigned outsize, const char t[128])
{
char *ptr = out;
unsigned x, i, r, l;
	insize = insize == 0xffffffff ? strlen(in) : insize;

	i = insize/4;
	r = in[insize-1] == '=' ? (in[insize-2] == '=' ? 2 : 1) : 0;
	l = (i*3) - r;

	if(outsize < l || !i) return 0;
	i--;

	if(__little_endian()){

		while(i--){
			x = t[*in++] << 26;
			x |= t[*in++] << 20;
			x |= t[*in++] << 14;
			x |= t[*in++] << 8;
			x = __bswap32(x);
			*(int*)ptr = x;
			ptr+=3;
		}
		if(r == 2){
			x = t[*in++] << 26;
			x |= t[*in++] << 20;
			x = __bswap32(x);
			*(int*)ptr = x;
		}
		else if(r == 1){
			x = t[*in++] << 26;
			x |= t[*in++] << 20;
			x |= t[*in++] << 14;
			x = __bswap32(x);
			*(int*)ptr = x;
		}
		else {
			x = t[*in++] << 26;
			x |= t[*in++] << 20;
			x |= t[*in++] << 14;
			x |= t[*in++] << 8;
			x = __bswap32(x);
			*(int*)ptr = x;
		}

	}
	else {
		while(i--){
			x = t[*in++] << 26;
			x |= t[*in++] << 20;
			x |= t[*in++] << 14;
			x |= t[*in++] << 8;
			*(int*)ptr = x;
			ptr+=3;
		}
		if(r == 2){
			x = t[*in++] << 26;
			x |= t[*in++] << 20;
			*(int*)ptr = x;
		}
		else if(r == 1){
			x = t[*in++] << 26;
			x |= t[*in++] << 20;
			x |= t[*in++] << 14;
			*(int*)ptr = x;
		}
		else {
			x = t[*in++] << 26;
			x |= t[*in++] << 20;
			x |= t[*in++] << 14;
			x |= t[*in++] << 8;
			*(int*)ptr = x;
		}
	}
	return l;
}

unsigned ckrypt_base64_decode(const char *in, unsigned insize, char out[], unsigned outsize)
{
	const char table[128] =
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
	 52,53,54,55,56,57,58,59,60,61,-1,-1,-1, 0,-1,-1,
	 -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
	 15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
	 -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
	 41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1};

	return base64_decode_compute(in, insize, out, outsize, table);

}

unsigned base64url_decode(const char *in, unsigned insize, char out[], unsigned outsize)
{
	//if(uri) unbase64[43] = -1, unbase64[45] = 62,
	//				unbase64[47] = -1, unbase64[95] = 63;

	return base64_decode_compute(in, insize, out, outsize, 0);
}

void ckrypt_base64_test1()
{
	char buf[256];
	int n;

	const char *str = "The quick brown fox jumps over the lazy dog";
	const char *target = "VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZw==";

	n = base64_encode(str, -1, buf, 256);
	if(n != 60) {
		printf("Test 1 Error: Wrong size\n");	
		return ;
	}

	if(memcmp(target, buf, 60)){
		printf("Test 1 Error: Mismatch string\n");	
		return;
	}

	printf("Test 1 passed!\n");

}

void ckrypt_base64_test2()
{
	char buf[256];
	int n;

	const char *str = "VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZw==";
	const char *target = "The quick brown fox jumps over the lazy dog";


	n = ckrypt_base64_decode(str, -1, buf, 256);

	if(n != 43) {
		printf("Test 2 Error: Wrong size\n");	
		return ;
	}

	if(memcmp(target, buf, 43)){
		printf("Test 2 Error: Mismatch string\n");	
		return;
	}

	printf("Test 2 passed!\n");
}

void ckrypt_bas64_test3()
{
	char buf[256];
	int n;

	//sha1: The quick brown fox jumps over the lazy dog
	const char *str = "de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3";

	n = base64_encode(str, 40, buf, 256);
	printf("base64: %.*s\n", n, buf);

	n = ckrypt_base64_decode(buf, n, buf, 256);
	printf("unbase64: %.*s\n", n, buf);

}
