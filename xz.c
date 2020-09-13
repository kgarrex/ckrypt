
int xz_header(char magic[6])
{
	const char xz_header_magic[6] = {0xfd,0x37,0x7a,0x58,0x5a,0x00};
	const char xz_footer_magic[2] = {0x59,0x5a};
}

unsigned xz_encode_int(const char *ptr, xz_uint64 n)
{
	//return number of bytes
	return 0;
}

unsigned xz_decode_multibyte_integer(const char *ptr, xz_uint64 *n)
{
	return 0;
}

int xz_decode(const char *file)
{
	xz_memcmp(
}
