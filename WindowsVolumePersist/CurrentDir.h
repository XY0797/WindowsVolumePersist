#pragma once

#include <string>

#include "DebugLogbase.h"

// 导出工作目录变量
extern std::wstring CurrentDir;

// 导出存音量设置的文件名
extern std::wstring savefilename;

#ifdef DEBUGMOD
// 导出日志文件变量
extern std::wstring logfilename;
#endif

void refreshCurDir();