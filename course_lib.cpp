#include <stdlib.h>
#include "course_lib.h"

template<typename T> void swap(T &a, T &b) {
	T tmp = a;
	a = b;
	b = tmp;
}

template<typename T> void sort_bubble(T *A, int len, long &iter, long &cmp) {
	iter = 0, cmp = 0;
	bool f;
	for (int i = 0; i < len; i++) {
		f = false;
		for (int j = 0; j < len - i - 1; j++) {
			if (A[j] > A[j + 1]) {
				swap(A[j], A[j + 1]);
				f = true;
				iter++;
			}
			cmp++;
		}
		if (!f) break;
	}
}

template<typename T> void sort_quick(T *A, int len, long &iter, long &cmp) {
	int l = 0, r = len - 1, tmp;
	//int m = Ar[len / 2];
	T m = A[rand() % len];
	
	do {
		while (A[l] < m) { l++; cmp++; }
		while (A[r] > m) { r--; cmp++; }
		
		if (l <= r) {
			swap<T>(A[l], A[r]);
			l++;
			r--;
			iter++;
		}
	} while (l <= r);
	if (r > 0) sort_quick(A, r + 1, iter, cmp);
	if (len - 1 > l) sort_quick(A + l, len - l, iter, cmp);
}

// explicit instantiation of template
template void sort_bubble<int>(int *A, int len, long &iter, long &cmp);
template void sort_bubble<float>(float *A, int len, long &iter, long &cmp);
template void sort_bubble<double>(double *A, int len, long &iter, long &cmp);
template void sort_bubble<char>(char *A, int len, long &iter, long &cmp);
template void sort_bubble<bool>(bool *A, int len, long &iter, long &cmp);
template void sort_quick<int>(int *A, int len, long &iter, long &cmp);
template void sort_quick<float>(float *A, int len, long &iter, long &cmp);
template void sort_quick<double>(double *A, int len, long &iter, long &cmp);
template void sort_quick<char>(char *A, int len, long &iter, long &cmp);
template void sort_quick<bool>(bool *A, int len, long &iter, long &cmp);
