#pragma once

// 从Windows头文件中排除极少使用的内容
#define WIN32_LEAN_AND_MEAN
// Windows头文件
#include <windows.h>

// 字符串处理
#include <string>
#include "mybase64.hpp"

// map
#include <map>

// 文件流
#include <fstream>

// 现代C++元组
#include <tuple>

// 调试日志
#include "DebugLog.hpp"

// 音量API
#include "AudioAPI.h"

// 开机自启
#include "AutoStartOnBoot.hpp"

// 当前目录
#include "CurrentDir.h"

// 音量设置结构体
struct audioSetting {
	float vol; // 音量
	bool mute; // 是否静音
};

// 窗口句柄
HWND hwnd;

// 时间计次  计时1分钟
int timecnt;

// 上一次读取的音量
float gLastVolume;

// 上一次读取的静音状态
bool gLastMute;

// 上一次的设备ID
std::wstring gLastDID;

// 设备ID转音量设置映射表
std::map<std::wstring, audioSetting> DIDmap;

// 是否需要把map持久化到文件
bool isNeedSave;