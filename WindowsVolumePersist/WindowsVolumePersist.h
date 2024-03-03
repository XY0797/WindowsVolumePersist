#pragma once

// ��Windowsͷ�ļ����ų�����ʹ�õ�����
#define WIN32_LEAN_AND_MEAN
// Windowsͷ�ļ�
#include <windows.h>

// �ַ�������
#include <string>
#include "mybase64.hpp"

// map
#include <map>

// �ļ���
#include <fstream>

// �ִ�C++Ԫ��
#include <tuple>

// ������־
#include "DebugLog.hpp"

// ����API
#include "AudioAPI.h"

// ��������
#include "AutoStartOnBoot.hpp"

// �������ýṹ��
struct audioSetting {
	float vol; // ����
	bool mute; // �Ƿ���
};

// ���������õ��ļ���
const char* filename = "volumedata.txt";

// ���ھ��
HWND hwnd;

// ʱ��ƴ�  ��ʱ1����
int timecnt;

// ��һ�ζ�ȡ������
float gLastVolume;

// ��һ�ζ�ȡ�ľ���״̬
bool gLastMute;

// ��һ�ε��豸ID
std::wstring gLastDID;

// �豸IDת��������ӳ���
std::map<std::wstring, audioSetting> DIDmap;

// �Ƿ���Ҫ��map�־û����ļ�
bool isNeedSave;