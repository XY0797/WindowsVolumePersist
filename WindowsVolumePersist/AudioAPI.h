#pragma once

// ��Windowsͷ�ļ����ų�����ʹ�õ�����
#define WIN32_LEAN_AND_MEAN
// Windowsͷ�ļ�
#include <windows.h>

// COM�������
#include <mmdeviceapi.h>
#include <endpointvolume.h>

// �ַ�������
#include <string>

// �ִ�C++Ԫ��
#include <tuple>

// �쳣����
#include<stdexcept>

/**
 * @brief ��ȡ������������
 *
 * @param [in] �Ƿ�Ϊ�������������Ϊtrue����ônewVolume��������Ч����������newVolume���������ݡ�
 * @param [in] ��������ֵ�����isSetΪtrue�������������壬���򽫺��ԡ�
 * @param [in] �¾���״̬��ֵ�����isSetΪtrue�������������壬���򽫺��ԡ�
 *
 * @return ���isSetΪtrue����ô����ֵ��Ϊtuple[0.0f,true,std::wstring(L"")]������ͷ��ػ�ȡ����tuple[����ֵ,�Ƿ���,�豸ID]
 *
 * @throw runtime_error ��������쳣���׳�������˵���ʹ�����Ĵ�Ӣ�Ĵ�����Ϣ
 */
std::tuple<float, bool, std::wstring> GetOrSetAudio(bool isSet = false, float newVolume = 0.0f, bool newMute = true);