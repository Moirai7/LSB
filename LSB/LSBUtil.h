#pragma once
/**
* Author:漂漂的岚岚姐
* Date: 2015/5/19
*/

#ifndef LANLAN_LSBUtil   //如果没有定义这个宏
#define LANLAN_LSBUtil   //定义这个宏

//这个文件是LSB加密解密的文件
class LSBUtil
{
public:
	LSBUtil(int,int);
	~LSBUtil(void);

	bool HandlePixels(unsigned char*,unsigned char*,int,int =7);
	bool ExtractPixels(unsigned char*,int =0,int =7);
	unsigned char* GetPixelsInfo();
	unsigned char* GetTextData();

private:
	int* BuildRandomSequence(int,int,int);
	unsigned char* LSBPixels;
	int bmpWidth;
	int bmpHeight;
	unsigned char* pTextFile;
};
#endif

