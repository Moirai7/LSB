
#include "stdafx.h"
#include "LSBUtil.h"

/**
* Author:漂漂的岚岚姐
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

//这个是生成不重复的随机数序列的
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

//这个是嵌入的时候调用的
//调用方法举例
//  FileUtil* fu = new FileUtil();
//	if(fu->ReadFile(CString("./image/24/test2.BMP")))
//	{
//		LSBUtil* lsb = new LSBUtil(fu->GetLineByte(),fu->GetHeight());
//		lsb->HandlePixels(fu->GetFileInfo(),(unsigned char*)"lanlanjielanlanjielanlanjielanlanjielanlanjie",45);
//  }
//fileInfo 是指的BMP原图的图像数据部分
//saveInfo 是指要嵌入的信息
//length 是指要嵌入的文字长度
//key 是指生成随机序列的key，默认为7
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
	//在前四个字节中写入加密数据长度   
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

	//可以使用查看二进制的文件查看，这里写入了保存的数据
	CFile textFile;   
    textFile.Open(CString("./image/copy/1.bin"),CFile::modeCreate | CFile::modeWrite);   
    textFile.Write((char*)pTextFile,length);   
    textFile.Close(); 

	LSBPixels = fileInfo;
	return true;
}

//这个是解密的时候调用的
//调用方法举例
//  FileUtil* fu = new FileUtil();
//	if(fu->ReadFile(CString("./image/result/test2.BMP")))
//	{
//		LSBUtil* lsb = new LSBUtil(fu->GetLineByte(),fu->GetHeight());
//		lsb->ExtractPixels(fu->GetFileInfo());
//		lsb->GetTextData();
//  }
//fileInfo 是指的BMP加密图的图像数据部分
//length 是指要嵌入的文字长度，默认为0，如果知道文件长度就赋值，不知道会从加密文件里读
//key 是指生成随机序列的key，默认为7
//
bool LSBUtil::ExtractPixels(unsigned char* fileInfo,int length,int key){
	if(length == 0)
	{
		DWORD saveLength = 0x00000000;   
		BYTE bit;   
		//获取保存数据长度   
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
	//开始提取   
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
	//可以使用查看二进制的文件查看，这里写入提取到的保存的数据
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