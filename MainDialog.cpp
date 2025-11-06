// MainDialog.cpp: 实现文件
//

#include "resource.h"
#include "afxdialogex.h"
#include "MainDialog.h"
#include <shlobj.h>       // SHGetKnownFolderPath
#include <knownfolders.h> // FOLDERID_Downloads
#include "showinfo.h"

// MainDialog 对话框

IMPLEMENT_DYNAMIC(MainDialog, CDialogEx)

MainDialog::MainDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

MainDialog::~MainDialog()
{
}

void MainDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MainDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &MainDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// MainDialog 消息处理程序
// 工具函数：打开文件对话框，返回 UTF-8 std::string 多路径
static std::vector<std::string> OpenFilesDialog(HWND hWnd, const std::string& initialDir)
{
    OPENFILENAMEA ofn = {};
    char szFiles[65536] = {};  // 支持多选
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFiles;
    ofn.nMaxFile = sizeof(szFiles) / sizeof(szFiles[0]);
    ofn.lpstrFilter = "图片文件 (*.jpg;*.jpeg;*.png)\0*.jpg;*.jpeg;*.png\0所有文件 (*.*)\0*.*\0";
    ofn.Flags = OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST | OFN_EXPLORER;
    ofn.lpstrInitialDir = initialDir.c_str();

    std::vector<std::string> result;
    if (GetOpenFileNameA(&ofn))
    {
        char * ptr = ofn.lpstrFile;
        std::string folder = ptr; // 第一个是文件夹路径
        ptr += folder.size() + 1;

        if (*ptr == 0)
        {
            // 只选了一个文件
            std::string fullPath = folder;
            result.push_back(fullPath.c_str());
        }
        else
        {
            // 选了多个文件
            while (*ptr)
            {
                std::string fileName = ptr;
                std::string fullPath = folder + "\\" + fileName;

                result.push_back(fullPath.c_str());
                ptr += fileName.size() + 1;
            }
        }
    }
    return result;
}
void MainDialog::OnBnClickedButton1()
{

    // 此电脑目录
    char szPath[MAX_PATH] = { 0 };
    SHGetSpecialFolderPathA(NULL, szPath, CSIDL_DRIVES, FALSE);

    // 调用新的文件选择函数
    std::vector<std::string> files = OpenFilesDialog(this->m_hWnd, szPath);
    if (!files.empty())
    {
        for (size_t i = 0; i < files.size(); i++)
        {
			showinfo * dlg = new showinfo;

            dlg->Create(IDD_DIALOG2, nullptr); // 非模态
            dlg->createshowinfo(files[i]);
            
            dlg->ShowWindow(SW_SHOW);
        }
        
    }
}

void MainDialog::setEditText()
{

}
