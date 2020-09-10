
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
