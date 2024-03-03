#pragma once

// 从Windows头文件中排除极少使用的内容
#define WIN32_LEAN_AND_MEAN
// Windows头文件
#include <windows.h>

// COM音量组件
#include <mmdeviceapi.h>
#include <endpointvolume.h>

// 字符串处理
#include <string>

// 现代C++元组
#include <tuple>

// 异常处理
#include<stdexcept>

/**
 * @brief 获取或者设置音量
 *
 * @param [in] 是否为设置音量，如果为true，那么newVolume参数才有效，否则会忽略newVolume参数的内容。
 * @param [in] 新音量的值。如果isSet为true本参数才有意义，否则将忽略。
 * @param [in] 新静音状态的值。如果isSet为true本参数才有意义，否则将忽略。
 *
 * @return 如果isSet为true，那么返回值恒为tuple[0.0f,true,std::wstring(L"")]，否则就返回获取到的tuple[音量值,是否静音,设备ID]
 *
 * @throw runtime_error 如果遇到异常会抛出带解释说明和错误码的纯英文错误信息
 */
std::tuple<float, bool, std::wstring> GetOrSetAudio(bool isSet = false, float newVolume = 0.0f, bool newMute = true);