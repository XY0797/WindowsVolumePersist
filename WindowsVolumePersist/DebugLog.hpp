#pragma once

// 调试模式
#define DEBUGMOD

#ifdef DEBUGMOD
#define DEBUGOUTPUT(LOGCONTENT) debuglog(LOGCONTENT)
#define DEBUGOUTPUTRAW(LOGCONTENT) debuglog(LOGCONTENT,true)
#else
#define DEBUGOUTPUT(LOGCONTENT)
#define DEBUGOUTPUTRAW(LOGCONTENT)
#endif

// 调试输出专用函数
#ifdef DEBUGMOD
std::string wstr2astr(const std::wstring& wstr) {
	wchar_t* wideCharBuffer = new wchar_t[wstr.length() + 1];
	wideCharBuffer[wstr.length()] = 0;
	for (size_t i = 0; i < wstr.length(); i++)
	{
		wideCharBuffer[i] = wstr[i];
	}
	int size_needed = WideCharToMultiByte(CP_ACP, 0, wideCharBuffer, -1, NULL, 0, NULL, NULL);
	char* charBuffer = new char[size_needed + 1];
	WideCharToMultiByte(CP_ACP, 0, wideCharBuffer, -1, charBuffer, size_needed, NULL, NULL);
	charBuffer[size_needed] = 0;
	std::string strTo(charBuffer);
	delete[] wideCharBuffer;
	delete[] charBuffer;
	return strTo;
}
#include <chrono>
#include <ctime>
#include <iomanip>
void debuglog(std::string content, bool israw = false) {
	std::ofstream logFile("log.txt", std::ios_base::app);
	if (logFile.is_open()) {
		if (israw) {
			logFile << content;
			logFile.close();
			return;
		}
		auto now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		struct tm timeinfo;
		localtime_s(&timeinfo, &now_c);

		logFile << "[" << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S") << "] " << content << "\n";
		logFile.close();
	}
	else {
		// 处理文件打开失败的情况
		std::wstring errorMsgW = L"音量持久化程序遇到异常：无法打开日志文件！";
		// 输出错误信息
		MessageBox(NULL, errorMsgW.c_str(), L"错误", MB_ICONERROR | MB_SETFOREGROUND);
		PostQuitMessage(2); // 退出程序
	}
}

void debuglog(std::wstring content, bool israw = false) {
	// 将宽字符转换为窄字符
	std::string narrowContent = wstr2astr(content);
	debuglog(narrowContent, israw);
}
#endif