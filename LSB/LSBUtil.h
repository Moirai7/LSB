#pragma once
/**
* Author:ƯƯ���᰽�
* Date: 2015/5/19
*/

#ifndef LANLAN_LSBUtil   //���û�ж��������
#define LANLAN_LSBUtil   //���������

#define U 30 //��˹�ֲ��ľ�ֵ
#define D 10 //��˹�ֲ��ľ�����
#define SALT 0.1
#define PEPPER 0.01
//����ļ���LSB���ܽ��ܵ��ļ�
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

