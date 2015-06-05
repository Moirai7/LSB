#include "stdafx.h"
#include "FileUtil.h"
/**
* Author:ƯƯ���᰽�
* Date: 2015/5/19
*/

FileUtil::FileUtil(void)
{
}


FileUtil::~FileUtil(void)
{
	delete []fileInfo;  
  
    if(biBitCount==8)  
        delete []pColorTable; 
}

//����Ƕ�ȡ��ʱ����õ�
//���÷�������
//  FileUtil* fu = new FileUtil();
//	if(fu->ReadFile(CString("./image/24/test2.BMP")))
//	{
//		
//  }
//file ָ����BMP�ļ���
//
bool FileUtil::ReadFile(CString file){
	CFile fp(file, CFile::modeRead);
	
	if(fp == NULL)
		return false;
  
	fp.Read(&fileHead, sizeof(BITMAPFILEHEADER));
	  
	fp.Read(&head, sizeof(BITMAPINFOHEADER));

	bmpWidth = head.biWidth;  
  
    bmpHeight = head.biHeight;  
  
    biBitCount = head.biBitCount;//�������������ͼ��ÿ��������ռ���ֽ�����������4�ı�����  
  
    lineByte=(bmpWidth * biBitCount/8+3)/4*4;//�Ҷ�ͼ������ɫ������ɫ�����Ϊ256  
	
	if(biBitCount==8)  
    {  
        pColorTable=new RGBQUAD[256];  
		fp.Read(pColorTable,sizeof(RGBQUAD)*256);  
    }  

	fileInfo =new unsigned char[lineByte*bmpHeight];  
  
	fp.Read(fileInfo,lineByte*bmpHeight);  
  
	fp.Close();  

	return true;
}

//����Ǳ����ʱ����õ�
//���÷�������
//	FileUtil* fu = new FileUtil();
//	fu->SaveFile(CString("./image/result/1.bmp"),lsb->GetPixelsInfo());
//file ָ���Ǳ����BMP�ļ���
//imgBuf ָ���Ǳ�����ǲ���ͼ�����ݡ�ǰ������Ǳ����lsb����������
//
bool FileUtil::SaveFile(CString file,unsigned char *imgBuf){
	//���λͼ����ָ��Ϊ0����û�����ݴ��룬��������  
    if(!imgBuf)  
        return false;  
  
	CFile fp(file, CFile::modeCreate|CFile::modeWrite);

    if(fp==NULL)  
        return false;  
  
    //д�ļ�ͷ���ļ� 
	fp.Write(&fileHead, sizeof(BITMAPFILEHEADER));
  
    //дλͼ��Ϣͷ���ڴ�  
	fp.Write(&head, sizeof(BITMAPINFOHEADER));  
  
    //����Ҷ�ͼ������ɫ��д���ļ�   
  
    if(biBitCount==8) 
		fp.Write(pColorTable, sizeof(RGBQUAD) * 256 );   
  
    //дλͼ���ݽ��ļ�  

	fp.Write(imgBuf, bmpHeight*lineByte); 

    //�ر��ļ�  
  
    fp.Close();

	return true;
}

//����������ģ���Ϊʵ��bmpҪ��һ��������4�ı�����������������bmp����4�ı����Ļ����������0���λ
//�������˼����ȥ�õ���Ч�����ݣ����ұ��������������ö����Ƶ�����򿪿�
//��������������ǲ��õ��õģ��Ҿ�д�����Եġ���������˻���GetFileInfo()�����޸ĺ���ˣ����Բ�Ҫ����
//���÷�������
//	fu->GetPixels(CString("./image/result/Pixels.bin"),lsb->GetPixelsInfo());
//filename���Ǳ�����Ч���ݵ��ļ���
//data Ĭ��ΪNULL��ʹ�����ڶ����е�fileInfo
//
unsigned char* FileUtil::GetPixels(CString filename,unsigned char* data){
	CFile outfile(filename,CFile::modeCreate|CFile::modeWrite);
	unsigned char* fileInfo = this->fileInfo;
	if(data!=NULL)
		fileInfo = data;

	if(biBitCount==8) {
		//��ȡ��Ч����  
		if (bmpWidth < lineByte)  
		{  
			unsigned char* pBmpBufData = new unsigned char[(bmpWidth) * (bmpHeight)]();  
			for (int i=0; i<bmpHeight; i++)  
			{  
				for (int j=0; j<bmpWidth; j++)  
				{  
					*(pBmpBufData + i * (bmpWidth) + j) = *(fileInfo + i * lineByte + j);      
				}  
			}  
			outfile.Write(pBmpBufData,(bmpWidth) * (bmpHeight));
			return pBmpBufData; //��Ч����  
		}  
	}else if(biBitCount==24){
		//��ȡ��Ч����  
		if (bmpWidth < lineByte)  
		{  
			unsigned char* pBmpBufData = new unsigned char[(bmpWidth) * (bmpHeight)]();  
			for(int i=0;i<bmpHeight;i++)  
			{  
				for(int j=0;j<bmpWidth;j++)  
				{  
					for(int k=0;k<3;k++) 
					{   
						*(pBmpBufData + i * (bmpWidth) + j) = *(fileInfo+i*lineByte+j*3+k);  
					}  
				}  
			}
			outfile.Write(pBmpBufData,(bmpWidth) * (bmpHeight));
			return pBmpBufData; //��Ч����  
		}
	}
	outfile.Close();
	return fileInfo;
}

//����������Ǹ������෴����˼�ˣ���һ��������չ��4�ı���
//����������ǲ��õ��õģ��Ҿ�д�����Ե�
//bmpData ������Ҫ���������
unsigned char* FileUtil::SetPixels(unsigned char* bmpData){
	if(biBitCount==8) {
		if (bmpWidth < lineByte)  
		{  
			unsigned char* pBmpBufData = new unsigned char[(lineByte) * (bmpHeight)]();  
			for (int i=0; i<bmpHeight; i++)  
			{  
				for (int j=0; j<bmpWidth; j++)  
				{  
					*(pBmpBufData + i * (lineByte) + j) = *(bmpData + i * bmpWidth + j);      
				}
				//��Ϊ��4�ı���
				for (int j=bmpWidth; j<lineByte; j++){
					*(pBmpBufData + i * (lineByte) + j) = 0;
				}
			}  
			return pBmpBufData; 
		}  
	}else if(biBitCount==24){
		if (bmpWidth < lineByte)  
		{  
			unsigned char* pBmpBufData = new unsigned char[(lineByte) * (bmpHeight)]();  
			for(int i=0;i<bmpHeight;i++)  
			{  
				for(int j=0;j<bmpWidth;j++)  
				{  
					for(int k=0;k<3;k++) 
					{   
						*(pBmpBufData + i * (lineByte) + j) = *(bmpData+i*bmpWidth+j*3+k);  
					}  
				}  
				for(int j=bmpWidth;j<lineByte;j++)  
				{  
					for(int k=0;k<3;k++) 
					{   
						*(pBmpBufData + i * (lineByte) + j*3+k) = 0;  
					}  
				} 
			}
			return pBmpBufData;   
		}
	}
	return bmpData;
}

int FileUtil::GetWidth(){
	return bmpWidth;
}

int FileUtil:: GetHeight(){
	return bmpHeight;
}

//�õ�һ�����ݵĿ��
int FileUtil::GetLineByte(){
	return lineByte;
}

//����������������õ����Ա������������
//ע�������ʾͼƬ����ô������أ�ÿ��������һ��bit�������������ʱ������������õ�byte��Ҫ����8����Ŷ
int FileUtil::GetSize(){
	return lineByte*bmpHeight;
}

//����ͼƬ����
int FileUtil::GetBitCount(){
	return biBitCount;
}

//�����ǵõ�ͼ�����ݵ�
unsigned char* FileUtil::GetFileInfo(){
	return fileInfo;
}

unsigned char* FileUtil::GetHead(){
	return (unsigned char*)&fileHead;
}
