/**
 * 控制台单文件程序，用于测试mybase64是否正常
 *
 * by XY0797
 * 2024.3.3
 */
#include <iostream>
#include "../mybase64.hpp"
int main() {
	std::wstring a = L"测试abcdefg";
	for (auto e : a) {
		void* vptr = (void*)(&e);
		unsigned char* ucharptr = (unsigned char*)vptr;
		std::cout << int(*ucharptr) << " " << int(*(ucharptr + 1)) << " ";
	}
	std::cout << std::endl;
	std::cout << "strlen:" << a.length() << " " << sizeof(a[0]) << std::endl;
	std::string b = encodeBase64(a);
	std::cout << b << std::endl;
	std::wstring c = decodeBase64(b);
	if (c == a) {
		for (auto e : c) {
			void* vptr = (void*)(&e);
			unsigned char* ucharptr = (unsigned char*)vptr;
			std::cout << int(*ucharptr) << " " << int(*(ucharptr + 1)) << " ";
		}
		std::cout << std::endl;
		std::wcout << L"OK" << std::endl;
	} else {
		for (auto e : c) {
			void* vptr = (void*)(&e);
			unsigned char* ucharptr = (unsigned char*)vptr;
			std::cout << int(*ucharptr) << " " << int(*(ucharptr + 1)) << " ";
		}
		std::cout << std::endl;
		std::wcout << L"NOTOK" << std::endl;
	}
	return 0;
}
