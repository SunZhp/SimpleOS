#include<os_string.h>

int32_t strlen(int8_t* string){
	uint32_t slen = 0;

	if(!string) return -1;

	while(*(string++)){
		slen++;
	}

	return slen;
}


uint32_t memcpy(int8_t* dst,int8_t* src,uint32_t len){
	int32_t i = 0;

	while((i++) < len){
		dst[i] = src[i];
	}

	return i;
}
