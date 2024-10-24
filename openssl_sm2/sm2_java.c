#include <string.h>
#include "sm2_java.h"

int JavaBigIntToByteArray(unsigned char *dst, const unsigned char *src, const int srcSize){
	int len = srcSize;
	for(int i = 0; i < srcSize; ++i) {
		if (src[i] >= 0x80){
			++len;
			break;
		}
		if (src[i] == 0x00){
			--len;
			continue;
		}
		break;
	}
	if(len > srcSize){
		dst[0] = 0x00;
		memcpy(&dst[1], src, srcSize);
	}else if(len < srcSize){
		memcpy(dst, &src[srcSize - len], len);
	}else{
		memcpy(dst, src, srcSize);
	}
	return len;
}
