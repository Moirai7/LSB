
#include "stdafx.h"
#include "LSBUtil.h"

/**
* Author:ƯƯ���᰽�
* Date: 2015/5/19
*/

LSBUtil::LSBUtil(int w,int h)
{
	bmpWidth = w;
	bmpHeight = h;
}


LSBUtil::~LSBUtil(void)
{
}

//��������ɲ��ظ�����������е�
int* LSBUtil::BuildRandomSequence(int low, int high,int key) {
	srand(key);
	int x = 0, tmp = 0;
	if (low > high) {
		tmp = low;
		low = high;
		high = tmp;
	}
	int* array = new int[high - low + 1];
	for (int i = low; i <= high; i++) {
		array[i - low] = i;
	}
	for (int i = high - low -1; i > 0; i--) {
		x = rand() % (i+1);
		tmp = array[i];
		array[i] = array[x];
		array[x] = tmp;
	}
	return array;
}

//�����Ƕ���ʱ����õ�
//���÷�������
//  FileUtil* fu = new FileUtil();
//	if(fu->ReadFile(CString("./image/24/test2.BMP")))
//	{
//		LSBUtil* lsb = new LSBUtil(fu->GetLineByte(),fu->GetHeight());
//		lsb->HandlePixels(fu->GetFileInfo(),(unsigned char*)"lanlanjielanlanjielanlanjielanlanjielanlanjie",45);
//  }
//fileInfo ��ָ��BMPԭͼ��ͼ�����ݲ���
//saveInfo ��ָҪǶ�����Ϣ
//length ��ָҪǶ������ֳ���
//key ��ָ����������е�key��Ĭ��Ϊ7
//
bool LSBUtil::HandlePixels(unsigned char* fileInfo,unsigned char* saveInfo,int length, int key){
	unsigned char infoData;
	unsigned char* pTextFile = saveInfo;
	int* array = BuildRandomSequence(0,bmpWidth*bmpHeight,key);
	//int k = sizeof(saveInfo);
	for(int i=0,k=0; i<length; ++i){
		for(int j=0; j<8; ++j){
			infoData = pTextFile[i]>>j;
			infoData = infoData & 0x01;
			if(infoData == 0)   
			{   
				fileInfo[array[k]] = fileInfo[array[k]]&0xfe;   
			}   
			else   
			{   
				fileInfo[array[k]] = fileInfo[array[k]]|0x01;   
			}  
			++k;
		}
	}
	//��ǰ�ĸ��ֽ���д��������ݳ���   
    DWORD saveLength;   
    for(int m=0; m<32; ++m)   
    {   
		saveLength = length>>m;   
        saveLength = saveLength&0x00000001;   
        if(saveLength==0)   
        {   
            fileInfo[m] = fileInfo[m]&0x1e;   
        }   
        else   
        {   
            fileInfo[m] = fileInfo[m]|0x01;   
        }   
    }   

	//����ʹ�ò鿴�����Ƶ��ļ��鿴������д���˱��������
	CFile textFile;   
    textFile.Open(CString("./image/copy/1.bin"),CFile::modeCreate | CFile::modeWrite);   
    textFile.Write((char*)pTextFile,length);   
    textFile.Close(); 

	LSBPixels = fileInfo;
	return true;
}

//����ǽ��ܵ�ʱ����õ�
//���÷�������
//  FileUtil* fu = new FileUtil();
//	if(fu->ReadFile(CString("./image/result/test2.BMP")))
//	{
//		LSBUtil* lsb = new LSBUtil(fu->GetLineByte(),fu->GetHeight());
//		lsb->ExtractPixels(fu->GetFileInfo());
//		lsb->GetTextData();
//  }
//fileInfo ��ָ��BMP����ͼ��ͼ�����ݲ���
//length ��ָҪǶ������ֳ��ȣ�Ĭ��Ϊ0�����֪���ļ����Ⱦ͸�ֵ����֪����Ӽ����ļ����
//key ��ָ����������е�key��Ĭ��Ϊ7
//
bool LSBUtil::ExtractPixels(unsigned char* fileInfo,int length,int key){
	if(length == 0)
	{
		DWORD saveLength = 0x00000000;   
		BYTE bit;   
		//��ȡ�������ݳ���   
		for(int i=0; i<32; ++i)   
		{   
			bit = fileInfo[i]&0x01;   
			if(bit==0)   
			{   
				saveLength = saveLength&0x7fffffff;   
			}   
			else   
			{   
				saveLength = saveLength|0x80000000;   
			}   
			if (i<31)       
				saveLength = saveLength>>1;   
		}  
		length = saveLength;
	}
	//��ʼ��ȡ   
	pTextFile = new unsigned char[length];   
	int* array = BuildRandomSequence(0,bmpWidth*bmpHeight,key);
	unsigned char infoData;

	for(int i=0,k=0; i<length*8; ++i){
		if(i && i%8==0)
			++k;  
		infoData = fileInfo[array[i]] & 0x01;

        if(infoData==0)   
        {   
            pTextFile[k] = pTextFile[k]&0x7f;   
        }   
        else   
        {   
            pTextFile[k] = pTextFile[k]|0x80;   
        }   
        if (i%8 != 7)    
            pTextFile[k] = pTextFile[k]>>1;   
	}
	//����ʹ�ò鿴�����Ƶ��ļ��鿴������д����ȡ���ı��������
	CFile textFile;   
    textFile.Open(CString("./image/result/1.bin"),CFile::modeCreate | CFile::modeWrite);   
    textFile.Write((char*)pTextFile,length);   
    textFile.Close(); 
	return true;
}

unsigned char* LSBUtil::GetTextData(){
	return pTextFile;
}


unsigned char* LSBUtil::GetPixelsInfo(){
	return LSBPixels;
}