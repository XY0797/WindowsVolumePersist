/**
 * C++17标准
 *
 * 开机时设置系统音量为保存值，并且保存音量变动
 *
 * by XY0797
 * 2024.3.3
 */

#include "WindowsVolumePersist.h"

std::wstring cstr2wstr(const char* str) {
	int wideCharLen = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0);
	// 使用数组做中转，C++标准不保证wstring内部连续
	wchar_t* wideCharBuffer = new wchar_t[wideCharLen + 1];
	MultiByteToWideChar(CP_ACP, 0, str, -1, wideCharBuffer, wideCharLen);
	wideCharBuffer[wideCharLen] = 0;
	std::wstring tmpwstr(wideCharBuffer);
	delete[] wideCharBuffer;
	return tmpwstr;
}

/**
 * @brief 读入音量信息到map
 */
void readVolume() {
	// 先清空map
	DIDmap.clear();
	isNeedSave = false;

	std::ifstream file(filename);
	if (!file) {
		// 文件打开失败
		return;
	}

	int len;
	file >> len;
	if (file.fail()) {
		// 值异常
		DIDmap.clear();
		file.close();
		return;
	}
	DEBUGOUTPUT("读入文件内数据: ");
	DEBUGOUTPUTRAW("读入音量数量: ");
	DEBUGOUTPUTRAW(std::to_string(len));
	DEBUGOUTPUTRAW("\n");

	for (int i = 0; i < len; i++)
	{
		audioSetting setting;
		std::string base64DID;
		file >> setting.vol >> setting.mute >> base64DID;
		std::wstring DID = decodeBase64(base64DID);

		// 逻辑或是短路求值，所以说直接判断即可
		if (file.fail() || setting.vol > 1.0f || setting.vol < 0.0f || DID.empty()) {
			// 值异常
			DIDmap.clear();
			break;
		}

		DIDmap[DID] = setting;

		DEBUGOUTPUTRAW("读入设备ID: ");
		DEBUGOUTPUTRAW(DID);
		DEBUGOUTPUTRAW("\n");
		DEBUGOUTPUTRAW("读入音量: ");
		DEBUGOUTPUTRAW(std::to_string(setting.vol));
		DEBUGOUTPUTRAW("\n");
		DEBUGOUTPUTRAW("读入静音状态: ");
		DEBUGOUTPUTRAW((setting.mute ? "是" : "否"));
		DEBUGOUTPUTRAW("\n");
	}

	// 关闭文件
	file.close();
}

/**
 * @brief 把map里面的音量信息保存到文件
 *
 * @throw runtime_error 如果遇到异常会抛出纯英文错误信息
 */
void saveVolume() {
	// 打开文件以覆盖现有内容（如果存在则清空）
	std::ofstream outfile(filename, std::ios_base::out | std::ios_base::trunc);

	// 检查文件是否成功打开
	if (outfile.is_open()) {
		// 获取映射表大小
		size_t len = DIDmap.size();

		// 先写入映射表大小
		outfile << len << std::endl;

		// 遍历映射表，将音量设置写入文件
		for (const auto& entry : DIDmap) {
			const audioSetting& setting = entry.second;
			std::string base64DID = encodeBase64(entry.first);
			outfile << setting.vol << std::endl << setting.mute << std::endl << base64DID << std::endl;
		}

		// 刷新并关闭文件
		outfile.flush();
		outfile.close();

		DEBUGOUTPUT("持久化map状态成功");
	}
	else {
		throw std::runtime_error("Unable to open volumedata.txt");
	}
}

// 窗口过程函数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_TIMER:
		if (wParam == 1) // 检查定时器ID
		{
			timecnt+=3;
			try {
				// C++17结构化绑定
				auto [curVolume, curMute, curDID] = GetOrSetAudio(); // 获取当前音频数据
				if (gLastDID == curDID) {
					if (curVolume != gLastVolume || curMute != gLastMute) {
						// 音量设置变动，需要修改
						DIDmap[curDID] = audioSetting{ curVolume, curMute };
						isNeedSave = true;

						DEBUGOUTPUT("音量设置变动，需要修改");
						DEBUGOUTPUTRAW("当前音量: ");
						DEBUGOUTPUTRAW(std::to_string(curVolume));
						DEBUGOUTPUTRAW("\n是否静音: ");
						DEBUGOUTPUTRAW((curMute ? "是" : "否"));
						DEBUGOUTPUTRAW("\n设备ID: ");
						DEBUGOUTPUTRAW(curDID);
						DEBUGOUTPUTRAW("\n");
					}
				}
				else {
					// 换设备了，需要重新判断
					DEBUGOUTPUT("换设备了，需要重新判断");
					if (DIDmap.find(curDID) != DIDmap.end()) {
						// 记录过这个设备
						DEBUGOUTPUT("记录过这个设备");
						audioSetting setting = DIDmap[curDID];
						if (abs(curVolume - setting.vol) >= 0.01f || setting.mute != curMute) {
							// 偏差大于1% 或 静音状态不一样，需要修改音量设置
							GetOrSetAudio(true, setting.vol, setting.mute);

							DEBUGOUTPUT("需要修改音量设置，现已修改完成");
							DEBUGOUTPUTRAW("存储的音量: ");
							DEBUGOUTPUTRAW(std::to_string(setting.vol));
							DEBUGOUTPUTRAW("\n是否静音: ");
							DEBUGOUTPUTRAW((setting.mute ? "是" : "否"));
							DEBUGOUTPUTRAW("\n");

							// 以修改后的状态为准
							std::tie(curVolume, curMute, curDID) = GetOrSetAudio();
						}
					}
					else {
						// 第一次见这个设备
						DEBUGOUTPUT("第一次见这个设备");
						if (curVolume >= 0.9f) {
							// 要聋了，先设置个低音量再说
							DEBUGOUTPUT("要聋了，先设置个低音量再说");
							GetOrSetAudio(true, 0.4f, false);
						}
						// 上一次音量设置个-1，这样子下一次就会保存了
						curVolume = -1.0f;
						// 要求下一次就持久化map内容到文件
						timecnt = 57;
					}
					// 更新上一次的设备记录
					gLastDID = curDID;
					DEBUGOUTPUT("换设备处理完成");
				}
				// 更新上一次的音量设置
				gLastVolume = curVolume;
				gLastMute = curMute;
				// 处理map数据持久化
				if (timecnt >= 60) {
					timecnt = 0;
					if (isNeedSave) {
						saveVolume();
						isNeedSave = false;
					}
				}
			}
			catch (const std::exception& e) {
				// 捕获到标准异常
				// 先销毁定时器
				KillTimer(hwnd, 1);
				// 转换为wstr
				std::wstring errorMsgW = L"音量持久化程序遇到异常：" + cstr2wstr(e.what());
				// 输出错误信息
				DEBUGOUTPUT(errorMsgW);
				MessageBox(NULL, errorMsgW.c_str(), L"错误", MB_ICONERROR | MB_SETFOREGROUND);
				PostQuitMessage(1); // 退出应用程序
			}
			catch (...) {
				// 捕获到其他异常
				// 先销毁定时器
				KillTimer(hwnd, 1);
				// 输出错误信息
				DEBUGOUTPUT("音量持久化程序遇到异常：未知类型的异常");
				MessageBox(NULL, L"音量持久化程序遇到异常：未知类型的异常", L"错误", MB_ICONERROR | MB_SETFOREGROUND);
				PostQuitMessage(1); // 退出程序
			}
			break;
		}
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

// 入口函数
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// 注册窗口类
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// 创建窗口
	hwnd = CreateWindowEx(0, CLASS_NAME, L"Hidden Window", 0, CW_USEDEFAULT, CW_USEDEFAULT, 200, 100, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		return 0;
	}

	// 窗口设置为隐藏
	ShowWindow(hwnd, SW_HIDE);

	if (!IsAppAutoStartEnabled()) {
		DEBUGOUTPUT("程序不在开机自启，询问要不要加入开机自启");
		int result = MessageBox(NULL, L"音量持久化程序没有在开机启动项，是否要加入开机自启？", L"询问", MB_YESNO | MB_ICONQUESTION);
		if (result == IDYES) {
			AddAppToAutoStart();
			DEBUGOUTPUT("把程序添加到了开机自启");
			MessageBox(NULL, L"已添加到开机自启", L"提示", MB_ICONINFORMATION);
		}
	}

	// 创建定时器，每隔3秒触发一次
	SetTimer(hwnd, 1, 3000, NULL);
	timecnt = 0;

	try {
		readVolume();

		std::tie(gLastVolume, gLastMute, gLastDID) = GetOrSetAudio();
		if (DIDmap.size() == 0) {
			// 读入失败，说明是第一次用
			DEBUGOUTPUT("读入文件失败，说明是第一次用，下一次将覆写map文件");
			if (gLastVolume >= 0.9f) {
				// 要聋了，先设置个低音量再说
				DEBUGOUTPUT("要聋了，先设置个低音量再说");
				GetOrSetAudio(true, 0.4f, false);
			}
			// 赋个-1.0f这样子下一次就会覆写文件
			gLastVolume = -1.0f;
			// 要求下一次就持久化map内容到文件
			timecnt = 60;
		}
		else if (DIDmap.find(gLastDID) != DIDmap.end()) {
			// 记录过这个设备
			audioSetting setting = DIDmap[gLastDID];
			DEBUGOUTPUT("记录过这个设备");
			if (abs(gLastVolume - setting.vol) >= 0.01f || gLastMute != setting.mute) {
				// 偏差大于1% 或 静音状态不一样，需要修改音量设置
				GetOrSetAudio(true, setting.vol, setting.mute);
				// 以修改后的状态为准
				std::tie(gLastVolume, gLastMute, gLastDID) = GetOrSetAudio();
				DEBUGOUTPUT("与记录的有偏差，更改了系统音量设置");
				DEBUGOUTPUTRAW("音量: ");
				DEBUGOUTPUTRAW(std::to_string(gLastVolume));
				DEBUGOUTPUTRAW("\n是否静音: ");
				DEBUGOUTPUTRAW((gLastMute ? "是" : "否"));
				DEBUGOUTPUTRAW("\n");
			}
		}
		else {
			// 说明之前用过，但是这一次启动时是新设备
			DEBUGOUTPUT("之前用过，但是这一次启动时是新设备");
			if (gLastVolume >= 0.9f) {
				// 要聋了，先设置个低音量再说
				DEBUGOUTPUT("要聋了，先设置个低音量再说");
				GetOrSetAudio(true, 0.4f, false);
			}
			// 赋个-1.0f这样子下一次就会覆写到文件
			gLastVolume = -1.0f;
			// 要求下一次就持久化map内容到文件
			timecnt = 60;
		}
		DEBUGOUTPUT("初始化结束");
	}
	catch (const std::exception& e) {
		// 捕获到标准异常
		// 先销毁定时器
		KillTimer(hwnd, 1);
		// 转换为wstr
		std::wstring errorMsgW = L"音量持久化程序遇到异常：" + cstr2wstr(e.what());
		// 输出错误信息
		DEBUGOUTPUT(errorMsgW);
		MessageBox(NULL, errorMsgW.c_str(), L"错误", MB_ICONERROR | MB_SETFOREGROUND);
		PostQuitMessage(1); // 退出程序
	}
	catch (...) {
		// 捕获到其他异常
		// 先销毁定时器
		KillTimer(hwnd, 1);
		// 输出错误信息
		DEBUGOUTPUT("音量持久化程序遇到异常：未知类型的异常");
		MessageBox(NULL, L"音量持久化程序遇到异常：未知类型的异常", L"错误", MB_ICONERROR | MB_SETFOREGROUND);
		PostQuitMessage(1); // 退出程序
	}

	// 窗口消息循环
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.wParam != 1) {
		// 销毁定时器
		KillTimer(hwnd, 1);
	}

	return 0;
}