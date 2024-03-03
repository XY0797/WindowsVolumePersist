#include <iostream>
#include <Windows.h>
#include <string>

bool IsAppAutoStartEnabled() {
	HKEY hKey;
	const char* subkey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	if (RegOpenKeyExA(HKEY_CURRENT_USER, subkey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
		char path[MAX_PATH];
		DWORD size = sizeof(path);
		if (RegQueryValueExA(hKey, "MYAPPStartOnBoot", NULL, NULL, (LPBYTE)path, &size) == ERROR_SUCCESS) {
			RegCloseKey(hKey);
			return true;
		}
		RegCloseKey(hKey);
	}
	return false;
}

void AddAppToAutoStart() {
	HKEY hKey;
	const char* subkey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	if (RegOpenKeyExA(HKEY_CURRENT_USER, subkey, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
		char path[MAX_PATH + 2];
		GetModuleFileNameA(NULL, path + 1, MAX_PATH);
		path[0] = '"';
		int i = 0;
		while (path[++i]);
		path[i] = '"';
		path[i + 1] = 0;
		RegSetValueExA(hKey, "MYAPPStartOnBoot", 0, REG_SZ, (LPBYTE)path, strlen(path) + 1);
		RegCloseKey(hKey);
	}
}

int main() {
	if (!IsAppAutoStartEnabled()) {
		std::cout << "Your application is not set to start on boot. Do you want to add it to startup? (y/n): ";
		std::string input;
		std::cin >> input;
		if (input == "y" || input == "Y") {
			AddAppToAutoStart();
			std::cout << "Your application has been added to startup." << std::endl;
		} else {
			std::cout << "Your application will not start on boot." << std::endl;
		}
	} else {
		std::cout << "Your application is already set to start on boot." << std::endl;
	}
	std::cout << "Input any number to exit..." << std::endl;
	int i;
	std::cin >> i;
	return 0;
}

