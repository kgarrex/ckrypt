
// 0x5a827999  0 <= t <= 19
// 0x6ed9eba1 20 <= t <= 39
// 0x8f1bbcdc 40 <= t <= 59
// 0xca62c1d6 60 <= t <= 79

int __big_endian(){
	short n = 0x100; return ((char*)(&n))[0] == 1;
}

int __little_endian(){
	short n = 0x1; return ((char*)(&n))[0] == 1;
}

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
#define __bswap32(w) (((w>>24)&0xff)|((w>>8)&0xff00)|((w<<8)&0xff0000)|((w<<24)&0xff000000))
#define rol(w, b) (((w)<<(b))|((w)>>(32-(b))))

#define st E=D;D=C;C=rol(B,30);B=A;A=T
#define r1(i) T=rol(A,5)+((B&C)|((~B)&D))+E+w[i]+k0;st
#define r2(i) T=rol(A,5)+(B^C^D)+E+w[i]+k1;st
#define r3(i) T=rol(A,5)+((B&C)|(B&D)|(C&D))+E+w[i]+k2;st
#define r4(i) T=rol(A,5)+(B^C^D)+E+w[i]+k3;st

#define a(i) w[i]=(b[i<<2]<<24)|(b[(i<<2)+1]<<16)|(b[(i<<2)+2]<<8)|(b[(i<<2)+3])
#define b(i) w[i]=rol((w[i-3]^w[i-8]^w[i-14]^w[i-16]),1)

#define f(b,w,i) b[i]=h[w&0xf];w>>=4; 

char *itoh32(int n, int sz, char buf[]);

void sha1(char digest[40], const char *msg, uint32_t len)
{
	const char *b;
	char     buf[128] = {0};
	uint8_t  c = 0, f = 0;
	uint32_t  nbits = len << 3;
	uint32_t nb;
	uint32_t w[80];
	uint32_t k0,k1,k2,k3;
	uint32_t h0,h1,h2,h3,h4;
	uint32_t A,B,C,D,E,T;

	/*
	if(!__big_endian){
		h0 = __bswap32(0x67453201);
		h1 = __bswap32(0xefcdab89);
		h2 = __bswap32(0x98badcfe);
		h3 = __bswap32(0x10325476);
		h4 = __bswap32(0xc3d2e1f0);
	}
	else {
	*/
		h0 = 0x67452301;
		h1 = 0xefcdab89;
		h2 = 0x98badcfe;
		h3 = 0x10325476;
		h4 = 0xc3d2e1f0;
	/*
	}

	if(!__big_endian){
		k0 = __bswap32(0x5a827999);
		k1 = __bswap32(0x6ed9eba1);
		k2 = __bswap32(0x8f1bbcdc);
		k3 = __bswap32(0xca62c1d6);
	}
	else {
	*/
		k0 = 0x5a827999;
		k1 = 0x6ed9eba1;
		k2 = 0x8f1bbcdc;
		k3 = 0xca62c1d6;
	/*
	}
	*/

	nb = len>>6; //divide by 64
	b = msg;

	//outer loop through each 64-byte (512-bit) block
loop:
	for(int i = 0; i < nb; i++)
	{
		a(0x0);a(0x1);a(0x2);a(0x3);
		a(0x4);a(0x5);a(0x6);a(0x7);
		a(0x8);a(0x9);a(0xA);a(0xB);
		a(0xC);a(0xD);a(0xE);a(0xF);

		b(0x10);b(0x11);b(0x12);b(0x13);
		b(0x14);b(0x15);b(0x16);b(0x17);
		b(0x18);b(0x19);b(0x1A);b(0x1B);
		b(0x1C);b(0x1D);b(0x1E);b(0x1F);
		b(0x20);b(0x21);b(0x22);b(0x23);
		b(0x24);b(0x25);b(0x26);b(0x27);
		b(0x28);b(0x29);b(0x2A);b(0x2B);
		b(0x2C);b(0x2D);b(0x2E);b(0x2F);
		b(0x30);b(0x31);b(0x32);b(0x33);
		b(0x34);b(0x35);b(0x36);b(0x37);
		b(0x38);b(0x39);b(0x3A);b(0x3B);
		b(0x3C);b(0x3D);b(0x3E);b(0x3F);
		b(0x40);b(0x41);b(0x42);b(0x43);
		b(0x44);b(0x45);b(0x46);b(0x47);
		b(0x48);b(0x49);b(0x4A);b(0x4B);
		b(0x4C);b(0x4D);b(0x4E);b(0x4F);

		A=h0;B=h1;C=h2;D=h3;E=h4;

		r1(0x00);r1(0x01);r1(0x02);r1(0x03);
		r1(0x04);r1(0x05);r1(0x06);r1(0x07);
		r1(0x08);r1(0x09);r1(0x0A);r1(0x0B);
		r1(0x0C);r1(0x0D);r1(0x0E);r1(0x0F);
		r1(0x10);r1(0x11);r1(0x12);r1(0x13);
		r2(0x14);r2(0x15);r2(0x16);r2(0x17);
		r2(0x18);r2(0x19);r2(0x1A);r2(0x1B);
		r2(0x1C);r2(0x1D);r2(0x1E);r2(0x1F);
		r2(0x20);r2(0x21);r2(0x22);r2(0x23);
		r2(0x24);r2(0x25);r2(0x26);r2(0x27);
		r3(0x28);r3(0x29);r3(0x2A);r3(0x2B);
		r3(0x2C);r3(0x2D);r3(0x2E);r3(0x2F);
		r3(0x30);r3(0x31);r3(0x32);r3(0x33);
		r3(0x34);r3(0x35);r3(0x36);r3(0x37);
		r3(0x38);r3(0x39);r3(0x3A);r3(0x3B);
		r4(0x3C);r4(0x3D);r4(0x3E);r4(0x3F);
		r4(0x40);r4(0x41);r4(0x42);r4(0x43);
		r4(0x44);r4(0x45);r4(0x46);r4(0x47);
		r4(0x48);r4(0x49);r4(0x4A);r4(0x4B);
		r4(0x4C);r4(0x4D);r4(0x4E);r4(0x4F);

		h0+=A;h1+=B;h2+=C;h3+=D;h4+=E;

		b+=0x40;       //advance to next block
	}

	if(f) goto finalize;

	c = len % 64;
	memcpy(buf, b, c); 
	buf[c++] = 0x80;
	c = 64 - c;           //remaining bytes in block 1
	if(c < 8){
		//set length in second block;
		nb=2;
	}
	else {
		buf[56] = 0;
		buf[57] = 0;
		buf[58] = 0;
		buf[59] = 0;
		buf[60] = (nbits >> 24) & 0xff;
		buf[61] = (nbits >> 16) & 0xff;
		buf[62] = (nbits >> 8) & 0xff;
		buf[63] = nbits & 0xff;
		nb=1;
	}
	b=buf; f=1;
	goto loop;

finalize:
	char *d = digest;
	char h[] =
	{'0','1','2','3','4','5','6','7',
	 '8','9','a','b','c','d','e','f'};

	/*
	if(!__big_endian){
		h0 = __bswap32(h0);	
		h1 = __bswap32(h1);
		h2 = __bswap32(h2);
		h3 = __bswap32(h3);
		h4 = __bswap32(h4);
	}
	*/

	f(d,h0,0); f(d,h0,1); f(d,h0,2); f(d,h0,3);
	f(d,h0,4); f(d,h0,5); f(d,h0,6); f(d,h0,7);
	f(d,h1,8); f(d,h1,9); f(d,h1,10);f(d,h1,11);
	f(d,h1,12);f(d,h1,13);f(d,h1,14);f(d,h1,15);
	f(d,h2,16);f(d,h2,17);f(d,h2,18);f(d,h2,19);
	f(d,h2,20);f(d,h2,21);f(d,h2,22);f(d,h2,23);
	f(d,h3,24);f(d,h3,25);f(d,h3,26);f(d,h3,27);
	f(d,h3,28);f(d,h3,29);f(d,h3,30);f(d,h3,31);
	f(d,h4,32);f(d,h4,33);f(d,h4,34);f(d,h4,35);
	f(d,h4,36);f(d,h4,37);f(d,h4,38);f(d,h4,39);
}

int test_sha1()
{
	char buf[40];

	const char *str = "The quick brown fox jumps over the lazy dog";
	const char *digest = "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12";

	printf("little-endian: %u\n", __little_endian());
	printf("big-endian: %u\n", __big_endian());

	sha1(buf, str, strlen(str));	

	printf("sha1 target: %.*s\n", 40, digest);
	printf("sha1 result: %.*s\n", 40, buf);

	if(memcmp(buf, digest, 40)){
		return 0;
	}

	return 1;
}

char *itoh32(int n, int sz, char buf[])
{
	char *p = buf;
	if(sz > 4) return 0;
	char c[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

	printf("n: %u\n", n);
	sz<<=1;
	for(int i=0; i < sz; i++){
		buf[i] = c[n&0xf];
		n>>=4;
	}
	return p+sz;
}
