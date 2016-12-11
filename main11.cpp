#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for rand()
#include <string.h>
#include <getopt.h>
#include <math.h>

#define OPTL 15

/*
будем брать последовательность из файла
*/

const int grow_step = 256;
int capacity = grow_step;
int *A;

void print_r(FILE *f, int *A, int len) {
	for (int i = 0; i < len; i++) {
		fprintf(f, "%d ", A[i]);
	}
	fprintf(f, "\n");
}

template<class T> void swap(T &a, T &b) {
	T tmp = a;
	a = b;
	b = tmp;
}

template<class T> void sort_bubble(T *A, int len, long &iter, long &cmp) {
	for (int i = 0; i < len; i++) {
		for (int j = i + 1; j < len; j++) {
			if (A[i] > A[j]) {
				swap(A[i], A[j]);
				iter++;
			}
			cmp++;
		}
	}
}

template<class T> void sort_quick(T *Ar, int len, long &iter, long &cmp) {
	int l = 0, r = len - 1, tmp;
	//int m = Ar[len / 2];
	T m = Ar[rand() % len];
	
	do {
		while (Ar[l] < m) { l++; cmp++; }
		while (Ar[r] > m) { r--; cmp++; }
		
		if (l <= r) {
			swap<T>(Ar[l], Ar[r]);
			l++;
			r--;
			iter++;
		}
	} while (l <= r);
	if (r > 0) sort_quick(Ar, r + 1, iter, cmp);
	if (len - 1 > l) sort_quick(Ar + l, len - l, iter, cmp);
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
	bool bubble_sort = true;
	char default_infile [] = "input.txt", default_outfile [] = "output.txt";
	char *infile = default_infile, *outfile = default_outfile;
	int N = 0; // размерность генератора тестов
	char buf[512];
	FILE *f;
	// supported command-line parameters
	const char options [][OPTL] = {
		"-b",
		"-q",
		"-in=",
		"-out=",
		"-test="
	};
	// parse options
	for (int i = 1; i < argc; i++) {
		for (int j = 0; j < (sizeof(options) / OPTL); j++) {
			if (!strncmp(argv[i], options[j], strlen(options[j]))) {
				//printf("Got option: %s\n", argv[i]);
				switch (j) { // в зависимости от того, с каким элементом массива options совпал аргумент, меняем поведение
					case 0: /* bubble sort */ bubble_sort = true; break;
					case 1: /* quick sort */ bubble_sort = false; break;
					case 2: infile = argv[i] + strlen(options[j]); break;
					case 3: outfile = argv[i] + strlen(options[j]); break;
					case 4:
						N = atoi(argv[i] + strlen(options[j]));
						if (N <= 0 || N > 31) {
							printf("[x] Wrong parameter for %s: '%s', must be in [1; 31]\n", 
							       options[j], argv[i] + strlen(options[j]));
							exit(2);
						}
					break;
				}
				// jump to next argv[]
				break;
			}
		}
	}
	
	// устанавливаем генератор случайных чисел
	srand(time(NULL));
#pragma region генератор тестов
	if (N > 0) {
		int cnt = pow(2, N);
		int max = cnt / 16;
		// случайные числа
		sprintf(buf, "test-%02d-rnd.txt", N);
		f = fopen(buf, "w");
		if (!f) {
			printf("[x] Could not open outfile '%s'\n", outfile);
			exit(2);
		}
		for (int i = 0; i < cnt; i++) {
			fprintf(f, "%d ", rand() % max);
		}
		fclose(f);
		// возрастающие числа
		sprintf(buf, "test-%02d-asc.txt", N);
		f = fopen(buf, "w");
		if (!f) {
			printf("[x] Could not open outfile '%s'\n", outfile);
			exit(2);
		}
		for (int i = 0; i < cnt; i++) {
			fprintf(f, "%d ", i);
		}
		fclose(f);
		// убывающие числа
		sprintf(buf, "test-%02d-dsc.txt", N);
		f = fopen(buf, "w");
		if (!f) {
			printf("[x] Could not open outfile '%s'\n", outfile);
			exit(2);
		}
		for (int i = 1; i <= cnt; i++) {
			fprintf(f, "%d ", cnt - i);
		}
		fclose(f);
		exit(0);
	}
#pragma endregion
	
	f = fopen(infile, "r");
	if (!f) {
		printf("[x] No input file found\n");
		return 2;
	}
	// выделяем память динамически
	A = (int*)malloc(sizeof(int) * capacity);
	int i = 0;
	while (!feof(f)) { // пока не конец файла
		if (i >= capacity) { // если памяти не хватает
			capacity += grow_step;
			A = (int*)realloc(A, sizeof(int) * capacity); // добавим ещё
		}
		if (fscanf(f, "%d", &(A[i++])) < 1) // запишем в массив текущий элемент
			break;
	}
	fclose(f);
	A = (int*)realloc(A, sizeof(int) * --i); // обрежем лишнюю память
#ifdef DEBUG
	printf("Source array:\n");
	print_r(stdout, A, i);
#endif
	long iter = 0, cmp = 0;
	// замеряем время
	struct timespec time1, time2;
	clock_gettime(CLOCK_REALTIME, &time1);
	
	if (bubble_sort)
		sort_bubble<int>(A, i, iter, cmp);
	else
		sort_quick<int>(A, i, iter, cmp);
	
	clock_gettime(CLOCK_REALTIME, &time2);
	double elapsed_time = time2.tv_sec - time1.tv_sec + 0.000000001 * (time2.tv_nsec - time1.tv_nsec);
#ifdef DEBUG
	printf("Result array:\n");
	print_r(stdout, A, i);
#endif
	//printf("Long size: %lu\n", sizeof(long));
	printf("Sequence length: %d\n", i);
	printf("Time elapsed: %.6lf\n", elapsed_time);
	printf("Compares: %ld\n", cmp);
	printf("Swaps: %ld\n", iter);
	f = fopen(outfile, "w");
	if (!f) {
		printf("[!] Could not open outfile '%s'\n", outfile);
	}
	fprintf(f, "%8d\t%.6f\t%14ld\t%14ld\t%s\n", i, elapsed_time, cmp, iter, infile);
	fclose(f);
	delete A;
	return 0;
}