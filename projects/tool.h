#pragma once
#ifndef _TOOL_
#define _TOOL_

inline void swap(int &a, int &b) {
	int temp = a;
	a = b;
	b = temp;
}

inline int max(int a, int b) {
	return a > b ? a : b;
}

inline int min(int a, int b) {
	return a < b ? a : b;
}


#endif
