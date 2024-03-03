/**
 * 判断当前CPU是大端还是小端
 *
 * by XY0797
 * 2024.3.3
 */
#include <iostream>
int main() {
	int test = 1;
	if ((*(char*)(void*)&test) == 1)
		printf("Little-Endian\n");
	else
		printf("Big-Endian\n");
	return 1;
}
