
#include "intern.h"

void itoha32(int n, int size, char buf[])
{
	if(size > 4) return;
	char c[] =
	{'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

	size <<= 1;
	for(int i = 0; i < size; i++){
		buf[i] = c[n & 0xf];
		n >>= 4;
	}

	printf("uuid: %.*s\n", size, buf);
}

void uuid_to_string_pretty(uuid_t *uuid, char str[36])
{
	itoha32(uuid->time_low, sizeof(int), &str[0]);
	str[8] = '-';
	itoha32(uuid->time_mid, sizeof(short), &str[9]);
	str[13] = '-';
	itoha32(uuid->time_hi_and_version, sizeof(short), &str[14]);
	str[18] = '-';
	itoha32(uuid->clock_seq_hi_and_reserved, sizeof(char), &str[19]);
	itoha32(uuid->clock_seq_low, sizeof(char), &str[21]);
	str[23] = '-';
	itoha32( *((int*)uuid->node), sizeof(int), &str[24]);
	itoha32( *((int*)uuid->node+4), sizeof(short), &str[32]);
}


void uuid_to_string(uuid_t *uuid, char str[32])
{
	itoha32(uuid->time_low, sizeof(int), &str[0]);

	itoha32(uuid->time_mid, sizeof(short), &str[8]);
	itoha32(uuid->time_hi_and_version, sizeof(short), &str[12]);
	itoha32(uuid->clock_seq_hi_and_reserved, sizeof(char), &str[16]);
	itoha32(uuid->clock_seq_low, sizeof(char), &str[18]);
	itoha32( *((int*)uuid->node), sizeof(int), &str[20]);
	itoha32( *((int*)uuid->node+4), sizeof(short), &str[28]);
}


uuid_node_t get_mac_address()
{
	IP_ADAPTER_INFO buffer[1] = {0}, *adapter_info = buffer;
	PIP_ADAPTER_INFO adapter;
	DWORD error;
	ULONG bufsize;
	uuid_node_t mac = 0;

	bufsize = sizeof(buffer);
	error = vt.GetAdaptersInfo(adapter_info, &bufsize);
	if(error != ERROR_SUCCESS){
		if(error != ERROR_BUFFER_OVERFLOW){
			printf("Error: Could not get mac address\n");
			return mac;
		}
		adapter_info = malloc(bufsize);
		if(!adapter_info){
			printf("Error: Could not get mac address\n");	
			return mac;
		}
		error = vt.GetAdaptersInfo(adapter_info, &bufsize);
	}

	adapter = adapter_info;
	do {
		if(adapter->Type == MIB_IF_TYPE_ETHERNET ||
		adapter->Type == IF_TYPE_IEEE80211)
		{
			//Need to check for endianness
			for(int i = 0; i < 6; i++)
				mac |= (uuid_node_t)adapter->Address[i] << (8*i);	

			printf("MAC: %02X-%02X-%02X-%02X-%02X-%02X\n",
				adapter->Address[0], adapter->Address[1],
				adapter->Address[2], adapter->Address[3],
				adapter->Address[4], adapter->Address[5]);

			break;
		}
	} while(adapter = adapter->Next);

	if(adapter_info != buffer){
		free(adapter_info);	
	}

	return mac & 0x0000FFFFFFFFFFFF;
}

int get_random_bytes(char buf[], int length)
{
	BOOLEAN (__stdcall *RtlGenRandom)(PVOID RandomBuffer, ULONG BufferLength);
	HMODULE hmod;

	hmod = LoadLibrary("advapi32.dll");
	RtlGenRandom = (void*)GetProcAddress(hmod, "SystemFunction036");

	buf[0] = 0; buf[1] = 0;
	if(!RtlGenRandom(buf, length)){
		printf("Error (%d): Could not get random bytes\n", GetLastError());	
		return 0;
	}

	FreeLibrary(hmod);
	return 1;
}

#define NANOSECOND_INTERVALS_PER_YEAR 315569520000000
#define NANOSECOND_INTERVALS_PER_DAY  864000000000
#define OFFSET 5751099360000000

//5680251360000000 + 66528000000000 = 5746779360000000
//5746779360000000 + 4320000000000  = 5751099360000000 (+5 leap days)

//offset: 100 nanosecond intervals per year x years since January 1, 1601
// 100 nanosecond intervals per day x number of days from October 15, 1582

uuid_time_t get_timestamp()
{
	uuid_time_t t = 0;
	FILETIME filetime, *pfiletime = &filetime;
	ULARGE_INTEGER large_int;

	GetSystemTimeAsFileTime(pfiletime);
	large_int.LowPart = filetime.dwLowDateTime;
	large_int.HighPart = filetime.dwHighDateTime;

	return large_int.QuadPart + OFFSET;
}

uuid_node_t get_node()
{
	uuid_node_t node = 0;
	char buf[6];

	node = get_mac_address();
	if(!node){
		//no mac address so get a random number	
		get_random_bytes(buf, 6);
		for(int i = 0; i < 6; i++)
			node |= buf[i] << (8*i);
	}
	return node;
}

//could also return this from a store
short get_clock_sequence()
{
	short seq = 0;
	char buf[2];

	get_random_bytes(buf, 2);
	seq = buf[1];
	seq <<= 8;
	//printf("seq: 0x%x\n", 0xffff & seq);
	seq |= buf[0];
	//printf("seq2: 0x%x\n", 0xffff & seq);
	//printf("0x%x 0x%x\n", 0x0ff & buf[1], 0x0ff & buf[0]);
	return seq & (short)0xff;
}

void uuid_create_v1(uuid_t *uuid)
{
	uuid_time_t tm;
	uuid_node_t node;
	short seq;

	//variant 0x80
	//version 0x10

	tm  = get_timestamp();
	seq = get_clock_sequence();
	node = get_node();

	uuid->time_low = (unsigned long)tm & 0xffffffff;
	uuid->time_mid = (unsigned short)((tm >> 32) & 0xffff);
	uuid->time_hi_and_version = ((unsigned short)((tm >> 48) & 0x0fff)) | 0x1000;
	uuid->clock_seq_low = seq & 0xff;
	uuid->clock_seq_hi_and_reserved = ((seq & 0x3f00) >> 8) | 0x80; 
	
	printf("HERE\n");

	/*
	char buf[36];
	uuid_to_string_pretty(uuid, buf);
	printf("pretty string: %.*s\n", 36, buf);

	uuid_to_string(uuid, buf);
	printf("string: %.*s\n", 32, buf);


	printf("node: 0x%llx\n", node);
	printf("sequence: 0x%x\n", 0x0ffff & seq);
	printf("time: 0x%llx\n", tm);
	*/
}
