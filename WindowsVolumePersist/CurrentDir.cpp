// 当前目录
#include "CurrentDir.h"

// 从Windows头文件中排除极少使用的内容
#define WIN32_LEAN_AND_MEAN
// Windows头文件
#include <windows.h>

std::wstring CurrentDir;

// 存音量设置的文件名
std::wstring savefilename;

#ifdef DEBUGMOD
std::wstring logfilename;
#endif

void refreshCurDir() {
    // 取运行目录
    wchar_t path[MAX_PATH * 2];
    GetModuleFileName(NULL, path, MAX_PATH * 2);
    std::wstring modulePath(path);
    std::wstring::size_type pos = modulePath.find_last_of(L"\\");
    if (pos != std::wstring::npos)
    {
        CurrentDir = modulePath.substr(0, pos);
        savefilename = CurrentDir + L"\\volumedata.txt";
#ifdef DEBUGMOD
        logfilename = CurrentDir + std::wstring(L"\\log.txt");
#endif
    }
    else
    {
        // 处理文件打开失败的情况
        std::wstring errorMsgW = L"音量持久化程序遇到异常：无法获取工作目录！";
        // 输出错误信息
        MessageBox(NULL, errorMsgW.c_str(), L"错误", MB_ICONERROR | MB_SETFOREGROUND);
        PostQuitMessage(2); // 退出程序
    }
}