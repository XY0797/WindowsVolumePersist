/**
 * C++17标准
 *
 * 提供音量获取/设置API
 *
 * by XY0797
 * 2024.3.3
 */

#include "AudioAPI.h"

std::tuple<float, bool, std::wstring> GetOrSetAudio(bool isSet, float newVolume, bool newMute) {
	HRESULT InitCOMhr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (InitCOMhr != S_OK && InitCOMhr != S_FALSE) {
		throw std::runtime_error(std::string("Failed to initialize COM, Error Code: ") + std::to_string(InitCOMhr));
	}

	IMMDeviceEnumerator* deviceEnumerator = NULL;
	IMMDevice* defaultDevice = NULL;
	IAudioEndpointVolume* endpointVolume = NULL;

	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
	if (FAILED(hr)) {
		CoUninitialize();
		throw std::runtime_error(std::string("Failed to create device enumerator, Error Code: ") + std::to_string(hr));
	}

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	if (FAILED(hr)) {
		deviceEnumerator->Release();
		CoUninitialize();
		throw std::runtime_error(std::string("Failed to get default audio endpoint, Error Code: ") + std::to_string(hr));
	}

	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (LPVOID*)&endpointVolume);
	if (FAILED(hr)) {
		defaultDevice->Release();
		deviceEnumerator->Release();
		CoUninitialize();
		throw std::runtime_error(std::string("Failed to activate audio endpoint, Error Code: ") + std::to_string(hr));
	}

	// 当前音量
	float currentVolume = 0.0f;

	// 当前是否静音
	bool currentMute = true;

	// 当前设备ID
	std::wstring currentDeviceId;

	if (isSet) {
		// 设置音量
		hr = endpointVolume->SetMasterVolumeLevelScalar(newVolume, NULL);
		if (FAILED(hr)) {
			endpointVolume->Release();
			defaultDevice->Release();
			deviceEnumerator->Release();
			CoUninitialize();
			throw std::runtime_error(std::string("Failed to set current volume, Error Code: ") + std::to_string(hr));
		}
		// 设置静音状态
		hr = endpointVolume->SetMute((newMute ? TRUE : FALSE), NULL);
		if (FAILED(hr)) {
			endpointVolume->Release();
			defaultDevice->Release();
			deviceEnumerator->Release();
			CoUninitialize();
			throw std::runtime_error(std::string("Failed to set current mute, Error Code: ") + std::to_string(hr));
		}
	}
	else {
		// 获取当前音量
		hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
		if (FAILED(hr)) {
			endpointVolume->Release();
			defaultDevice->Release();
			deviceEnumerator->Release();
			CoUninitialize();
			throw std::runtime_error(std::string("Failed to get current volume, Error Code: ") + std::to_string(hr));
		}
		// 获取当前静音状态
		BOOL isMute;
		hr = endpointVolume->GetMute(&isMute);
		if (FAILED(hr)) {
			endpointVolume->Release();
			defaultDevice->Release();
			deviceEnumerator->Release();
			CoUninitialize();
			throw std::runtime_error(std::string("Failed to get current mute, Error Code: ") + std::to_string(hr));
		}
		currentMute = (isMute ? true : false);
		// 获取设备ID
		LPWSTR deviceId = NULL;
		hr = defaultDevice->GetId(&deviceId);
		if (FAILED(hr)) {
			endpointVolume->Release();
			defaultDevice->Release();
			deviceEnumerator->Release();
			CoUninitialize();
			throw std::runtime_error(std::string("Failed to get device ID, Error Code: ") + std::to_string(hr));
		}
		currentDeviceId = std::wstring(deviceId);
		// GetId内部申请的内存，需要CoTaskMemFree释放内存
		CoTaskMemFree(deviceId);
	}

	endpointVolume->Release();
	defaultDevice->Release();
	deviceEnumerator->Release();
	CoUninitialize();

	return std::make_tuple(currentVolume, currentMute, currentDeviceId);
}