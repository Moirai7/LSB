
// LSBDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSB.h"
#include "LSBDlg.h"
#include "afxdialogex.h"
#include "FileUtil.h"
#include "LSBUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLSBDlg �Ի���



CLSBDlg::CLSBDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLSBDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLSBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLSBDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_READ_FILE, &CLSBDlg::OnBnClickedButtonReadFile)
	ON_BN_CLICKED(IDC_BUTTON_READ_INFO, &CLSBDlg::OnBnClickedButtonReadInfo)
END_MESSAGE_MAP()


// CLSBDlg ��Ϣ�������

BOOL CLSBDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CLSBDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLSBDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLSBDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLSBDlg::OnBnClickedButtonReadFile()
{
	FileUtil* fu = new FileUtil();
	if(fu->ReadFile(CString("./image/24/test1.BMP")))
	{
		fu->SaveFile(CString("./image/copy/4.bmp"),fu->GetFileInfo());

		LSBUtil* lsb = new LSBUtil(fu->GetLineByte(),fu->GetHeight());

		lsb->CountLSB(fu->GetFileInfo());
		int zero_before = lsb->GetZero();
		int one_before = lsb->GetOne();

		//lsb->SetGaussNoise(fu->GetFileInfo(),fu->GetBitCount());

		lsb->HandlePixels(fu->GetFileInfo(),(unsigned char*)"lanlanjielanlanjielanlanjielanlanjielanlanjie",45);

		lsb->CountLSB(fu->GetFileInfo());
		int zero_after = lsb->GetZero();
		int one_after = lsb->GetOne();

		std::cout<<"zero: "<<zero_before<<"one: "<<one_before;


		fu->SaveFile(CString("./image/result/4.bmp"),lsb->GetPixelsInfo());
	}
}


void CLSBDlg::OnBnClickedButtonReadInfo()
{
	FileUtil* fu = new FileUtil();
	if(fu->ReadFile(CString("./image/result/4.bmp")))
	{
		LSBUtil* lsb = new LSBUtil(fu->GetLineByte(),fu->GetHeight());
		lsb->ExtractPixels(fu->GetFileInfo(),45);


		//MessageBox((LPCTSTR)lsb->GetTextData());
	}
}
