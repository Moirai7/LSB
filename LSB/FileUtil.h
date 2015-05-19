#pragma once

/**
* Author:漂漂的岚岚姐
* Date: 2015/5/19
*/

#ifndef LANLAN_FileUtil   //如果没有定义这个宏
#define LANLAN_FileUtil   //定义这个宏

using namespace std;

//这个是BMP文件读写操作用的
class FileUtil
{
public:
	FileUtil(void);
	~FileUtil(void);

	bool ReadFile(CString);
	bool SaveFile(CString,unsigned char*);
	unsigned char* GetPixels(CString,unsigned char* =NULL);
	unsigned char* GetFileInfo();

	int GetWidth();
	int GetHeight();
	int GetLineByte();

	//可以隐藏的数据信息
	int GetSize();

private:
	unsigned char* fileInfo;
	int bmpWidth;//图像的宽  
	int bmpHeight;//图像的高  
	int biBitCount;//图像类型，每像素位数
	RGBQUAD *pColorTable;//颜色表指针
	int lineByte;

	BITMAPINFOHEADER head;
	BITMAPFILEHEADER fileHead;

	unsigned char* SetPixels(unsigned char*);
};
#endif

