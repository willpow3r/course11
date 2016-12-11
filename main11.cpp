#include <stdio.h>
#include <stdlib.h>

/*
будем брать последовательность из файла
*/

const int grow_step = 30;
int capacity = grow_step;
int *A;

void print_r(FILE *f, int *A, int len) {
	for (int i = 0; i < len; i++) {
		fprintf(f, "%d ", A[i]);
	}
	fprintf(f, "\n");
}

void swap(int &a, int &b) {
	int tmp = a;
	a = b;
	b = tmp;
}

int sort_bubble(int *A, int len) {
	int iter = 0;
	for (int i = 0; i < len; i++) {
		for (int j = i + 1; j < len; j++) {
			if (A[i] > A[j]) {
				swap(A[i], A[j]);
				iter++;
			}
		}
	}
	return iter;
}

int sort_quick(int *Ar, int len) {
	int pos = (Ar - A);
	printf("Start sorting from %d to %d\n", pos, pos + len - 1);
	int l = 0, r = len - 1;
	int tmp, iter = 0;
	int m = Ar[len / 2];
	
	do {
		while (Ar[l] < m) l++;
		while (Ar[r] > m) r--;
		
		if (l <= r) {
			swap(Ar[l], Ar[r]);
			l++;
			r--;
			print_r(stderr, A, 8);
			iter++;
		}
	} while (l <= r);
	if (r > 0) {
		fprintf(stderr, "Sorting left...\n");
		iter += sort_quick(Ar, r + 1);
	}
	if (len - 1 > l) {
		fprintf(stderr, "Sorting right\n");
		iter += sort_quick(Ar + l, len - l);
	}
	return iter;
}

int sort_quick(int *Ar, int start, int end) {
	printf("Start sorting from %d to %d\n", start, end - 1);
	long l = start, r = end - 1; // поставить указатели на исходные места
	int temp, m, iter = 0;

	m = Ar[start + ((end - start) / 2)]; // центральный элемент

	do { // процедура разделения
		while (Ar[l] < m) l++;
		while (Ar[r] > m) r--;

		if (l <= r) {
			printf("Swapping %ld with %ld\n", l, r);
			swap(Ar[l], Ar[r]);
			l++; 
			r--;
			iter++;
		}
	} while (l <= r);

	if (r > start) {
		fprintf(stderr, "Sorting left from %d to %ld...\n", start, r);
		iter += sort_quick(Ar, start, r + 1);
	}
	if (end - 1 > l) {
		fprintf(stderr, "Sorting right from %ld to %d...\n", l, end - 1);
		iter += sort_quick(Ar, l, end);
	}
	return iter;
}

int main(int argc, char **argv) {
	FILE *f = fopen("input.txt", "r");
	if (!f) {
		printf("[x] No input file found\n");
		return 2;
	}
	A = (int*)malloc(sizeof(int) * capacity);
	int i = 0;
	while (!feof(f)) {
		fscanf(f, "%d", &(A[i++]));
		if (i >= capacity) {
			capacity += grow_step;
			A = (int*)realloc(A, sizeof(int) * capacity);
		}
	}
	fclose(f);
	A = (int*)realloc(A, sizeof(int) * i);
	
	printf("Source array:\n");
	print_r(stdout, A, i);
	int iter = sort_quick(A, i);
	printf("Result array:\n");
	print_r(stdout, A, i);
	printf("Iterations: %d\n", iter);
	delete A;
	return 0;
}