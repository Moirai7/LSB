#pragma once
/**
* Author:ƯƯ���᰽�
* Date: 2015/5/19
*/

#ifndef LANLAN_LSBUtil   //���û�ж��������
#define LANLAN_LSBUtil   //���������

//����ļ���LSB���ܽ��ܵ��ļ�
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

