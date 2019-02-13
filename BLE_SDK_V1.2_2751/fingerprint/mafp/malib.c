#include "malib.h"
#include <string.h>
 

int32_t hex_to_str(int8_t *buf, uint8_t *src, int32_t len)
{
	uint8_t i;
	int32_t n = 0;

	for (i = 0; i < len; i++) {
		n += sprintf((char *)buf + n, " %02x", src[i]);
	}

	return n;
}


void printn(uint8_t *buf, int32_t len)
{
	int8_t des[200];
	uint8_t i, remind = len % 20;

	for (i = 20; i <= len; i += 20) {
		hex_to_str(des, buf + i - 20, 20);
		LOGE("[%3d] %s", i - 20, des);
	}
	
	if (remind) {
		hex_to_str(des, buf + i - 20, remind);
		LOGE("[%3d] %s", i - 20, des);
	}
}


int32_t int_to_str(int8_t *buf, int32_t *src, int32_t len)
{
	uint8_t i;
	int32_t n = 0;

	for (i = 0; i < len; i++) {
		n += sprintf((char *)buf + n, " %8d", src[i]);
	}

	return n;
}


void printn_int(int32_t *buf, int32_t len)
{
	int8_t des[200];
	uint8_t i, remind = len % 10;

	for (i = 10; i <= len; i += 10) {
		int_to_str(des, buf + i - 10, 10);
		LOGE("[%3d] %s", i - 10, des);
	}
	
	if (remind) {
		int_to_str(des, buf + i - 10, remind);
		LOGE("[%3d] %s", i - 10, des);
	}
}


int save_bmp_file(uchar *file, int file_size, uchar *buf, int w, int h)
{
	int i, j, remind = w % 4;
	struct BMP_FILE_HEADER shead;
	struct BMP_INFO sinfo;
	struct RGBQUAD pal[256];
	char head[54] = {
		0x42,0x4d,0x36,0xcc,0x00,0x00,0x00,0x00,0x00,0x00,
		0x36,0x04,0x00,0x00,0x28,0x00,0x00,0x00,0xc8,0x00,
		0x00,0x00,0x00,0x01,0x00,0x00,0x01,0x00,0x08,0x00,
		0x00,0x00,0x00,0x00,0x00,0xc8,0x00,0x00,0x0c,0x00,
		0x00,0x00,0x0c,0x00,0x00,0x00,0x00,0x01,0x00,0x00,
		0x00,0x00,0x00,0x00
	};
	uchar *p = file;

	if (file_size < w * h) {
		LOGE("!!!!!! DEST Buffer is too small, %d < %d !!!!", file_size, w * h);
		return -1;
	}

	shead.bType = 0x4d42;
	shead.bSize = w * h + 1078;
	shead.bReserved1 = 0x00;
	shead.bReserved2 = 0x00;
	shead.bOffset = 0x0436;
	sinfo.bInfoSize = 0x28;
	sinfo.bWidth = w;
	sinfo.bHeight = h;
	sinfo.bPlanes = 0x01;
	sinfo.bBitCount = 0x08;
	sinfo.bCompression = 0x00;
	sinfo.bmpImageSize = w * h;
	sinfo.bXPelsPerMeter = 0x0c;
	sinfo.bYPelsPerMeter = 0x0c;
	sinfo.bClrUsed = 0x100;
	sinfo.bClrImportant = 0x00;
	memcpy(head+2, &shead.bSize, 4);
	memcpy(head+18, &sinfo.bWidth, 4);
	memcpy(head+22, &sinfo.bHeight, 4);
	memcpy(head+34, &sinfo.bmpImageSize, 4);

	memcpy(p, head, 54);
	p +=54;
	
	for (i = 0; i < 256; i++) {
		pal[i].rgbReserved = 0;
		pal[i].rgbBlue = i;
		pal[i].rgbGreen = i;
		pal[i].rgbRed = i;

		memcpy(p, &pal[i], sizeof(struct RGBQUAD));
		p += sizeof(struct RGBQUAD);
	}

	if (!remind) {
		memcpy(p, buf, w*h);
		p += w*h;
	} else {
		for (i = 0; i < h; i++) {
			memcpy(p, buf + w * i, w);
			p += w;

			for (j = 0; j < 4 - remind; j++) {
				*p++ = 0;
			}
		}
	}

	return p - file;
}


int load_bmp_body(uchar *file, uchar *buf, int buf_size, int *w, int *h)
{
	int remind, nw, i;
	unsigned char *s = file, *d = buf;
		
	memcpy(w, &file[18], 4);
	memcpy(h, &file[22], 4);

	remind = (*w) % 4;
	nw = remind? ((*w) - remind + 4) : (*w);

	if ((file[0] != 0x42) || (file[1] != 0x4d)) {
		LOGE("not a bmp file.");
		*w = 0;
		*h = 0;
		return -1;
	}

	if ((*w) * (*h) > buf_size) {
		LOGE("!!!!!! DEST Buffer is too small, %d < %d !!!!", buf_size, (*w) * (*h));
		*w = 0;
		*h = 0;
		return -1;
	}

	LOGE("--> BMP: w = %d, nw = %d, h = %d", *w, nw, *h);

	s += 1078;
	for (i = 0; i < *h; i++) {
		memcpy(d, s, nw);
		d += *w;
		s += nw;
	}

	return 0;
}


const uint8_t CRC8_Tab[]=
{
	0,  94, 188, 226,  97,  63, 221, 131, 194, 156, 126,  32, 163, 253,  31,  65,
	157, 195,  33, 127, 252, 162,  64,  30,  95,   1, 227, 189,  62,  96, 130, 220,
	35, 125, 159, 193,  66,  28, 254, 160, 225, 191,  93,   3, 128, 222,  60,  98,
	190, 224,   2,  92, 223, 129,  99,  61, 124,  34, 192, 158,  29,  67, 161, 255,
	70,  24, 250, 164,  39, 121, 155, 197, 132, 218,  56, 102, 229, 187,  89,   7,
	219, 133, 103,  57, 186, 228,   6,  88,  25,  71, 165, 251, 120,  38, 196, 154,
	101,  59, 217, 135,   4,  90, 184, 230, 167, 249,  27,  69, 198, 152, 122,  36,
	248, 166,  68,  26, 153, 199,  37, 123,  58, 100, 134, 216,  91,   5, 231, 185,
	140, 210,  48, 110, 237, 179,  81,  15,  78,  16, 242, 172,  47, 113, 147, 205,
	17,  79, 173, 243, 112,  46, 204, 146, 211, 141, 111,  49, 178, 236,  14,  80,
	175, 241,  19,  77, 206, 144, 114,  44, 109,  51, 209, 143,  12,  82, 176, 238,
	50, 108, 142, 208,  83,  13, 239, 177, 240, 174,  76,  18, 145, 207,  45, 115,
	202, 148, 118,  40, 171, 245,  23,  73,   8,  86, 180, 234, 105,  55, 213, 139,
	87,   9, 235, 181,  54, 104, 138, 212, 149, 203,  41, 119, 244, 170,  72,  22,
	233, 183,  85,  11, 136, 214,  52, 106,  43, 117, 151, 201,  74,  20, 246, 168,
	116,  42, 200, 150,  21,  75, 169, 247, 182, 232,  10,  84, 215, 137, 107,  53
};


uint8_t CRC8(uint8_t *dat, uint32_t len)
{
	uint8_t crc8 = 0;
	
	while(len--){
		crc8 = CRC8_Tab[crc8 ^ *dat];
		dat++;
	}
	
	return crc8;
}


uint32_t getHashIndex(const uint8_t *data)
{
	uint32_t nHash = 0;
	uint16_t pos = 0;

	if (!data) return 0;

	while (pos < 260) {
		nHash = (nHash<<5) + nHash + data[pos];
		pos++;
	}
        
	return ( nHash % 260);
}
