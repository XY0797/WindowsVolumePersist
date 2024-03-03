/**
 * ��װbase64_rfc4648�⣬��wstring����Ϊ�޿��ַ�string
 *
 * ������x86CPU(С����)
 *
 * by XY0797
 * 2024.3.3
 */
#ifndef MYBASE64_HPP
#define MYBASE64_HPP

#include <string>
#include "base64/base64_rfc4648.hpp"

using base64 = cppcodec::base64_rfc4648;

// wstr����Ϊbase64��str���������쳣�׳�
std::string encodeBase64(const std::wstring& wstr) {
	size_t len = wstr.length();
	wchar_t* wcharArray = new wchar_t[len];
	std::copy(wstr.begin(), wstr.end(), wcharArray);
	len *= 2;
	unsigned char* ucharptr = (unsigned char*)wcharArray;
	std::string encodeedstr = base64::encode(ucharptr, len);
	delete[] wcharArray;
	return encodeedstr;
}

// base64��str����Ϊwstr���������쳣�׳�
std::wstring decodeBase64(const std::string& str) {
	std::vector<uint8_t> tmpdecode = base64::decode(str);
	size_t len = tmpdecode.size();

	if (len % 2) {
		++len;
	}

	std::wstring decodedStr;
	decodedStr.reserve(len / 2);

	wchar_t wchar;
	// ���ֽڶ�д
	char* rawptr0 = (char*)&wchar;
	char* rawptr1 = rawptr0 + 1;

	for (size_t i = 0; i < len; i += 2) {
		*rawptr0 = tmpdecode[i];
		*rawptr1 = tmpdecode[i + 1];
		decodedStr.push_back(wchar);
	}

	return decodedStr;
}

#endif // MYBASE64_HPP
