#pragma once

/**
* Author:ƯƯ���᰽�
* Date: 2015/5/19
*/

#ifndef LANLAN_FileUtil   //���û�ж��������
#define LANLAN_FileUtil   //���������

using namespace std;

//�����BMP�ļ���д�����õ�
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

	//�������ص�������Ϣ
	int GetSize();

private:
	unsigned char* fileInfo;
	int bmpWidth;//ͼ��Ŀ�  
	int bmpHeight;//ͼ��ĸ�  
	int biBitCount;//ͼ�����ͣ�ÿ����λ��
	RGBQUAD *pColorTable;//��ɫ��ָ��
	int lineByte;

	BITMAPINFOHEADER head;
	BITMAPFILEHEADER fileHead;

	unsigned char* SetPixels(unsigned char*);
};
#endif

