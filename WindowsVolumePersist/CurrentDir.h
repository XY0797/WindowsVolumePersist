#pragma once

#include <string>

#include "DebugLogbase.h"

// ��������Ŀ¼����
extern std::wstring CurrentDir;

// �������������õ��ļ���
extern std::wstring savefilename;

#ifdef DEBUGMOD
// ������־�ļ�����
extern std::wstring logfilename;
#endif

void refreshCurDir();