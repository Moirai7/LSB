#include "stdafx.h"
#include "FileUtil.h"
/**
* Author:漂漂的岚岚姐
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

//这个是读取的时候调用的
//调用方法举例
//  FileUtil* fu = new FileUtil();
//	if(fu->ReadFile(CString("./image/24/test2.BMP")))
//	{
//		
//  }
//file 指的是BMP文件名
//
bool FileUtil::ReadFile(CString file){
	CFile fp(file, CFile::modeRead);
	
	if(fp == NULL)
		return false;
  
	fp.Read(&fileHead, sizeof(BITMAPFILEHEADER));
	  
	fp.Read(&head, sizeof(BITMAPINFOHEADER));

	bmpWidth = head.biWidth;  
  
    bmpHeight = head.biHeight;  
  
    biBitCount = head.biBitCount;//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）  
  
    lineByte=(bmpWidth * biBitCount/8+3)/4*4;//灰度图像有颜色表，且颜色表表项为256  
	
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

//这个是保存的时候调用的
//调用方法举例
//	FileUtil* fu = new FileUtil();
//	fu->SaveFile(CString("./image/result/1.bmp"),lsb->GetPixelsInfo());
//file 指的是保存的BMP文件名
//imgBuf 指的是保存的那部分图像数据。前面举例是保存的lsb处理后的数据
//
bool FileUtil::SaveFile(CString file,unsigned char *imgBuf){
	//如果位图数据指针为0，则没有数据传入，函数返回  
    if(!imgBuf)  
        return false;  
  
	CFile fp(file, CFile::modeCreate|CFile::modeWrite);

    if(fp==NULL)  
        return false;  
  
    //写文件头进文件 
	fp.Write(&fileHead, sizeof(BITMAPFILEHEADER));
  
    //写位图信息头进内存  
	fp.Write(&head, sizeof(BITMAPINFOHEADER));  
  
    //如果灰度图像，有颜色表，写入文件   
  
    if(biBitCount==8) 
		fp.Write(pColorTable, sizeof(RGBQUAD) * 256 );   
  
    //写位图数据进文件  

	fp.Write(imgBuf, bmpHeight*lineByte); 

    //关闭文件  
  
    fp.Close();

	return true;
}

//这个是这样的，因为实际bmp要求一行数据是4的倍数，所以如果读入的bmp宽不是4的倍数的话，后面就有0填充位
//这个的意思就是去得到有效的数据，并且保存下来，可以用二进制的软件打开看
//但是这个方法你们不用调用的，我就写来测试的。如果调用了话，GetFileInfo()就是修改后的了，所以不要调用
//调用方法举例
//	fu->GetPixels(CString("./image/result/Pixels.bin"),lsb->GetPixelsInfo());
//filename就是保存有效数据的文件名
//data 默认为NULL就使用现在对象中的fileInfo
//
unsigned char* FileUtil::GetPixels(CString filename,unsigned char* data){
	CFile outfile(filename,CFile::modeCreate|CFile::modeWrite);
	unsigned char* fileInfo = this->fileInfo;
	if(data!=NULL)
		fileInfo = data;

	if(biBitCount==8) {
		//提取有效数据  
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
			return pBmpBufData; //有效数据  
		}  
	}else if(biBitCount==24){
		//提取有效数据  
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
			return pBmpBufData; //有效数据  
		}
	}
	outfile.Close();
	return fileInfo;
}

//这个和上面那个就是相反的意思了，将一行数据扩展到4的倍数
//这个方法你们不用调用的，我就写来测试的
//bmpData 就是需要处理的数据
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
				//因为是4的倍数
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

//得到一行数据的宽度
int FileUtil::GetLineByte(){
	return lineByte;
}

//这个函数就是用来得到可以保存的数据量的
//注意这里表示图片有这么多的像素，每个像素是一个bit，所以你们算的时候，如果数据是用的byte就要除以8才行哦
int FileUtil::GetSize(){
	return lineByte*bmpHeight;
}

//返回图片类型
int FileUtil::GetBitCount(){
	return biBitCount;
}

//这里是得到图像数据的
unsigned char* FileUtil::GetFileInfo(){
	return fileInfo;
}

unsigned char* FileUtil::GetHead(){
	return (unsigned char*)&fileHead;
}
