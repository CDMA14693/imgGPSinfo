// showinfo.cpp: 实现文件
//

#include "resource.h"
#include "afxdialogex.h"
#include "showinfo.h"
#include <atlimage.h>

// showinfo 对话框

IMPLEMENT_DYNAMIC(showinfo, CDialogEx)
//写入剪贴板
void WriteTextToClipboard(const std::string& text)
{
    if (text.empty()) return;

    // 打开剪贴板（nullptr 表示当前进程）
    if (!OpenClipboard(nullptr))
        return;

    // 不调用 EmptyClipboard()，保留历史数据
    // （系统仍会自动在 Win+V 里保存旧条目）
    EmptyClipboard();
    // 分配全局内存，用于存储文本数据
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (hGlobal)
    {
        char* pData = static_cast<char*>(GlobalLock(hGlobal));
        memcpy(pData, text.c_str(), text.size() + 1);
        GlobalUnlock(hGlobal);

        // 写入 ANSI 文本（若用 Unicode 则换成 CF_UNICODETEXT）
        SetClipboardData(CF_TEXT, hGlobal);
    }

    CloseClipboard();
}

void showinfo::DisplayImage(const std::string& filepath)
{
    CImage img;
    HRESULT hr = img.Load(CA2W(filepath.c_str()));
    if (FAILED(hr)) return;

    CRect rc;
    imgshow.GetClientRect(&rc);

    int imgWidth = img.GetWidth();
    int imgHeight = img.GetHeight();
    double scaleX = static_cast<double>(rc.Width()) / imgWidth;
    double scaleY = static_cast<double>(rc.Height()) / imgHeight;
    double scale = std::min(scaleX, scaleY);

    int newWidth = static_cast<int>(imgWidth * scale);
    int newHeight = static_cast<int>(imgHeight * scale);

    CImage scaledImg;
    scaledImg.Create(newWidth, newHeight, 32);

    HDC hdcDest = scaledImg.GetDC();
    HDC hdcSrc = img.GetDC();

    // 设置高质量拉伸模式
    SetStretchBltMode(hdcDest, HALFTONE);
    SetBrushOrgEx(hdcDest, 0, 0, nullptr);

    StretchBlt(
        hdcDest,
        0, 0, newWidth, newHeight,
        hdcSrc,
        0, 0, imgWidth, imgHeight,
        SRCCOPY
    );

    img.ReleaseDC();
    scaledImg.ReleaseDC();

    if (hBitmap_) DeleteObject(hBitmap_);
    hBitmap_ = scaledImg.Detach();
    imgshow.SetBitmap(hBitmap_);
}


showinfo::showinfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, hBitmap_(nullptr) // 初始化 hBitmap_ 为 nullptr
{

}

bool showinfo::createshowinfo(const std::string& filepath)
{
	reader_.load(filepath);
	info_= reader_.getInfo();

	

    DisplayImage(filepath);
	wgs84_n.SetWindowTextW(CA2W(std::to_string(info_.wgsLat).c_str()));
	wgs84_e.SetWindowTextW(CA2W(std::to_string(info_.wgsLon).c_str()));
	cgj02_n.SetWindowTextW(CA2W(std::to_string(info_.gcjLat).c_str()));
	cgj02_e.SetWindowTextW(CA2W(std::to_string(info_.gcjLon).c_str()));
	URLGD.SetWindowTextW(CA2W(info_.amapUrl.c_str()));

    SetWindowText(CA2W(filepath.c_str()));
	return false;
}

showinfo::~showinfo()
{
}

void showinfo::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, Picture, imgshow);
    DDX_Control(pDX, IDC_EDIT1, wgs84_e);
    DDX_Control(pDX, IDC_EDIT4, wgs84_n);
    DDX_Control(pDX, IDC_EDIT3, cgj02_e);
    DDX_Control(pDX, IDC_EDIT5, cgj02_n);
    DDX_Control(pDX, IDC_EDIT6, URLGD);
}


BEGIN_MESSAGE_MAP(showinfo, CDialogEx)
    ON_STN_CLICKED(Picture, &showinfo::OnStnClickedPicture)
    ON_BN_CLICKED(IDC_BUTTON1, &showinfo::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &showinfo::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &showinfo::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &showinfo::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON5, &showinfo::OnBnClickedButton5)
END_MESSAGE_MAP()


// showinfo 消息处理程序

void showinfo::OnStnClickedPicture()
{
    
}

void showinfo::OnBnClickedButton1()
{
    WriteTextToClipboard(std::to_string(info_.wgsLon).c_str());
}

void showinfo::OnBnClickedButton2()
{
    WriteTextToClipboard(std::to_string(info_.wgsLat).c_str());
}

void showinfo::OnBnClickedButton3()
{
    WriteTextToClipboard(std::to_string(info_.gcjLon).c_str());
}

void showinfo::OnBnClickedButton4()
{
    WriteTextToClipboard(std::to_string(info_.gcjLat).c_str());
}

void showinfo::OnBnClickedButton5()
{
    WriteTextToClipboard(info_.amapUrl);
}
