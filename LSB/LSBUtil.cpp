
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
	zeroCount = 0;
	oneCount = 0;
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

//保存文件长度
//最好保存到文件头里
bool LSBUtil::SaveInfoLength(int length,unsigned char* fileInfo)
{
	int* pfile = (int *)(fileInfo+6);
	*pfile = length;

	return true;
}

//读取文件长度
int LSBUtil::GetInfoLength(unsigned char* fileInfo)
{  
	DWORD saveLength = 0x00000000;   
	BYTE bit;   
	//获取保存数据长度  

	int* pfile = (int *)(fileInfo+6);

	return *pfile; 
}

bool LSBUtil::CountLSB(unsigned char* fileInfo){
	zeroCount = 0;
	oneCount = 0;
	unsigned char infoData;
	for(int i=0; i<bmpHeight*bmpWidth; ++i){
		for(int j=0; j<8; ++j){
			infoData = fileInfo[i]>>j;
			infoData = infoData & 0x01;
			if(infoData == 0)   
			{   
				zeroCount += 1;
			}   
			else   
			{   
				oneCount += 1; 
			}  
		}
	} 
	return true;
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
	
	if(length<=0 || length>bmpWidth*bmpHeight) return false;
	//开始提取   
	pTextFile = new unsigned char[length+1];   
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
	pTextFile[length] = '\0';
	//可以使用查看二进制的文件查看，这里写入提取到的保存的数据
	CFile textFile;   
    textFile.Open(CString("./image/result/Extract.bin"),CFile::modeCreate | CFile::modeWrite);   
    textFile.Write((char*)pTextFile,length);   
    textFile.Close(); 
	return true;
}

int LSBUtil::Gaus_S(){ //产生高斯样本，以U为均值，D为均方差
	double sum=0; 
	for(int i=0;i<12;i++) 
		sum+=rand()/32767.00;

	//计算机中rand()函数为－32767～＋32767（2^15-1）
	//故sum＋为0～1之间的均匀随机变量
	//产生均值为U，标准差为D的高斯分布的样本，并返回

	return int(U+D*(sum-6));
}

//这个是添加随机噪声的函数
//lsb->SetGaussNoise(fu->GetFileInfo(),fu->GetBitCount());
//fileInfo是图像数据
//type是图像的类型 灰度 还是 24
bool LSBUtil::SetGaussNoise(unsigned char* fileInfo,int type){
	if (fileInfo== NULL) return false;
	int x,y,p;
	srand((unsigned)time(NULL));  //种下随机种子
	double temp;
	for(y=0;y<bmpHeight;y++)
	{
		for(x=0;x<bmpWidth;x++)
		{
			temp = double (rand ()) / RAND_MAX;
            if (temp > 1 - SALT) {
				if(type == 8){
					p=x+y*bmpWidth; //p为现在处理的象素点
					fileInfo[p]+=Gaus_S();
				}else{
					p=x*3+y*bmpWidth; //p为现在处理的象素点
					//向R、G、B三个分量分别添加高斯噪声
					fileInfo[p+2]+=Gaus_S();
					fileInfo[p+1]+=Gaus_S();
					fileInfo[p]+=Gaus_S();
				}
            } else if (temp < PEPPER) {
				if(type == 8){
					p=x+y*bmpWidth; //p为现在处理的象素点
					fileInfo[p]+=Gaus_S();
				}else{
					p=x*3+y*bmpWidth; //p为现在处理的象素点
					//向R、G、B三个分量分别添加高斯噪声
					fileInfo[p+2]+=Gaus_S();
					fileInfo[p+1]+=Gaus_S();
					fileInfo[p]+=Gaus_S();
				}
            }
		}
	} 
	LSBPixels = fileInfo;
	return true;
}

unsigned char* LSBUtil::GetTextData(){
	return pTextFile;
}


unsigned char* LSBUtil::GetPixelsInfo(){
	return LSBPixels;
}

int LSBUtil::GetZero(){
	return zeroCount;
}

int LSBUtil::GetOne(){
	return oneCount;
}