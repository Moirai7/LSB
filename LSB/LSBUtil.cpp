
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
	zeroCount = 0;
	oneCount = 0;
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

//�����ļ�����
//��ñ��浽�ļ�ͷ��
bool LSBUtil::SaveInfoLength(int length,unsigned char* fileInfo)
{
	int* pfile = (int *)(fileInfo+6);
	*pfile = length;

	return true;
}

//��ȡ�ļ�����
int LSBUtil::GetInfoLength(unsigned char* fileInfo)
{  
	DWORD saveLength = 0x00000000;   
	BYTE bit;   
	//��ȡ�������ݳ���  

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
	
	if(length<=0 || length>bmpWidth*bmpHeight) return false;
	//��ʼ��ȡ   
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
	//����ʹ�ò鿴�����Ƶ��ļ��鿴������д����ȡ���ı��������
	CFile textFile;   
    textFile.Open(CString("./image/result/Extract.bin"),CFile::modeCreate | CFile::modeWrite);   
    textFile.Write((char*)pTextFile,length);   
    textFile.Close(); 
	return true;
}

int LSBUtil::Gaus_S(){ //������˹��������UΪ��ֵ��DΪ������
	double sum=0; 
	for(int i=0;i<12;i++) 
		sum+=rand()/32767.00;

	//�������rand()����Ϊ��32767����32767��2^15-1��
	//��sum��Ϊ0��1֮��ľ����������
	//������ֵΪU����׼��ΪD�ĸ�˹�ֲ���������������

	return int(U+D*(sum-6));
}

//����������������ĺ���
//lsb->SetGaussNoise(fu->GetFileInfo(),fu->GetBitCount());
//fileInfo��ͼ������
//type��ͼ������� �Ҷ� ���� 24
bool LSBUtil::SetGaussNoise(unsigned char* fileInfo,int type){
	if (fileInfo== NULL) return false;
	int x,y,p;
	srand((unsigned)time(NULL));  //�����������
	double temp;
	for(y=0;y<bmpHeight;y++)
	{
		for(x=0;x<bmpWidth;x++)
		{
			temp = double (rand ()) / RAND_MAX;
            if (temp > 1 - SALT) {
				if(type == 8){
					p=x+y*bmpWidth; //pΪ���ڴ�������ص�
					fileInfo[p]+=Gaus_S();
				}else{
					p=x*3+y*bmpWidth; //pΪ���ڴ�������ص�
					//��R��G��B���������ֱ���Ӹ�˹����
					fileInfo[p+2]+=Gaus_S();
					fileInfo[p+1]+=Gaus_S();
					fileInfo[p]+=Gaus_S();
				}
            } else if (temp < PEPPER) {
				if(type == 8){
					p=x+y*bmpWidth; //pΪ���ڴ�������ص�
					fileInfo[p]+=Gaus_S();
				}else{
					p=x*3+y*bmpWidth; //pΪ���ڴ�������ص�
					//��R��G��B���������ֱ���Ӹ�˹����
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