#include <afxwin.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "MainDialog.h"




int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    if (!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
    {
        wprintf(L"错误: MFC 初始化失败\n");
        return 1;
    }
	MainDialog * m = new MainDialog();
    m->DoModal();
}