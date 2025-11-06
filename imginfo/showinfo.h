#pragma once
#include "afxdialogex.h"
#include "ImageMetaReader.h"

// showinfo 对话框

class showinfo : public CDialogEx
{
	DECLARE_DYNAMIC(showinfo)

public:
	void DisplayImage(const std::string& filepath);
	showinfo(CWnd* pParent = nullptr);   // 标准构造函数
	bool createshowinfo(const std::string& filepath);
	virtual ~showinfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	ImageMetaReader reader_;
	ImageMetaInfo info_;
	DECLARE_MESSAGE_MAP()
private:
	HBITMAP hBitmap_;
public:
	CStatic imgshow;
	afx_msg void OnStnClickedPicture();
	afx_msg void OnBnClickedButton1();
	CEdit wgs84_e;
	CEdit wgs84_n;
	CEdit cgj02_e;
	CEdit cgj02_n;
	CEdit URLGD;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
