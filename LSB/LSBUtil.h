#pragma once
/**
* Author:漂漂的岚岚姐
* Date: 2015/5/19
*/

#ifndef LANLAN_LSBUtil   //如果没有定义这个宏
#define LANLAN_LSBUtil   //定义这个宏

#define U 30 //高斯分布的均值
#define D 10 //高斯分布的均方差
#define SALT 0.1
#define PEPPER 0.01
//这个文件是LSB加密解密的文件
class LSBUtil
{
public:
	LSBUtil(int,int);
	~LSBUtil(void);

	bool HandlePixels(unsigned char*,unsigned char*,int=0,int =7);
	bool ExtractPixels(unsigned char*,int =0,int =7);
	unsigned char* GetPixelsInfo();
	unsigned char* GetTextData();
	bool SetGaussNoise(unsigned char* ,int );
	bool SaveInfoLength(int ,unsigned char* );
	bool CountLSB(unsigned char*);
	int GetInfoLength(unsigned char* );
	int GetZero();
	int GetOne();

private:
	int* BuildRandomSequence(int,int,int);
	int Gaus_S();
	unsigned char* LSBPixels;
	int bmpWidth;
	int bmpHeight;
	unsigned char* pTextFile;
	int zeroCount;
	int oneCount;
};
#endif

