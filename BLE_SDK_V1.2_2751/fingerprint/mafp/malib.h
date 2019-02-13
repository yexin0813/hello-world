/*
 * malib.h
 * Copyright (C) MicroArray Microelectronics Corp.,LTD 2016 All rights reserved
 *
 * @Author: AK <ywj@microarray.com.cn>
 * @Date:   2016-12-02 15:02:44
 * @Last Modified by:   AK
 * @Last Modified time: 2016-12-19 10:29:52
 * 
 * @Desription 
 */

#ifndef MALIB_H
#define MALIB_H

#include "mafp_defs.h"

typedef unsigned char uchar;
typedef unsigned short WORD; // 定义WORD为两个字节的类型
typedef unsigned long DWORD; // 定义DWORD为e四个字节的类型


struct BMP_FILE_HEADER { // 位图文件头
	WORD bType; // 文件标识符
	DWORD bSize; // 文件的大小
	WORD bReserved1; // 保留值,必须设置为0
	WORD bReserved2; //  保留值,必须设置为0
	DWORD bOffset; // 文件头的最后到图像数据位开始的偏移量
};

struct BMP_INFO { // 位图信息头
	DWORD bInfoSize; // 信息头的大小
	DWORD bWidth; // 图像的宽度
	DWORD bHeight; // 图像的高度
	WORD bPlanes; // 图像的位面数
	WORD bBitCount; // 每个像素的位数
	DWORD bCompression; // 压缩类型
	DWORD bmpImageSize; // 图像的大小,以字节为单位
	DWORD bXPelsPerMeter; // 水平分辨率
	DWORD bYPelsPerMeter; // 垂直分辨率
	DWORD bClrUsed; // 使用的色彩数
	DWORD bClrImportant; // 重要的颜色数
};

struct RGBQUAD { // 彩色表
	uchar rgbBlue; // 蓝色强度
	uchar rgbGreen; // 绿色强度
	uchar rgbRed; // 红色强度
	uchar rgbReserved; // 保留值
};


extern void printn(uint8_t *buf, int32_t len);
extern void printn_int(int32_t *buf, int32_t len);
extern int  save_bmp_file(uchar *file, int file_size, uchar *buf, int w, int h);
extern int  load_bmp_body(uchar *file, uchar *buf, int buf_size, int *w, int *h);

extern uint8_t CRC8(uint8_t *dat, uint32_t len);
extern uint32_t getHashIndex(const uint8_t *data);

#endif

