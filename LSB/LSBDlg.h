
// LSBDlg.h : ͷ�ļ�
//

#pragma once


// CLSBDlg �Ի���
class CLSBDlg : public CDialogEx
{
// ����
public:
	CLSBDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LSB_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonReadFile();
	afx_msg void OnBnClickedButtonReadInfo();
};
