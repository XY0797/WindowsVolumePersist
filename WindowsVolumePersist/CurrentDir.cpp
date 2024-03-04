// ��ǰĿ¼
#include "CurrentDir.h"

// ��Windowsͷ�ļ����ų�����ʹ�õ�����
#define WIN32_LEAN_AND_MEAN
// Windowsͷ�ļ�
#include <windows.h>

std::wstring CurrentDir;

// ���������õ��ļ���
std::wstring savefilename;

#ifdef DEBUGMOD
std::wstring logfilename;
#endif

void refreshCurDir() {
    // ȡ����Ŀ¼
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
        // �����ļ���ʧ�ܵ����
        std::wstring errorMsgW = L"�����־û����������쳣���޷���ȡ����Ŀ¼��";
        // ���������Ϣ
        MessageBox(NULL, errorMsgW.c_str(), L"����", MB_ICONERROR | MB_SETFOREGROUND);
        PostQuitMessage(2); // �˳�����
    }
}