#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 100000000 // Ukuran array untuk pengujian

void benchmark_prefetch() {
    // Alokasikan array
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return;
    }

    // Inisialisasi array
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i;
    }

    // Mulai pengukuran waktu
    clock_t start = clock();

    // Akses array dengan prefetch menggunakan inline assembly
    for (int i = 0; i < ARRAY_SIZE - 8; i++) { // Menghindari akses out-of-bounds
        // Inline assembly untuk prefetch data ke depan
        __asm__ __volatile__(
            "prefetcht0 (%0)" // Prefetch ke cache level 0 untuk elemen array[i + 8]
            :
            : "r"(&array[i + 8])
            : "memory"
        );
        int temp = array[i];
    }

    // Selesai pengukuran waktu
    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Benchmark dengan prefetch selesai dalam waktu: %.4f detik\n", duration);

    // Bebaskan memori
    free(array);
}

void benchmark_no_prefetch() {
    // Alokasikan array
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return;
    }

    // Inisialisasi array
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i;
    }

    // Mulai pengukuran waktu
    clock_t start = clock();

    // Akses array tanpa prefetch
    for (int i = 0; i < ARRAY_SIZE; i++) {
        int temp = array[i];
    }

    // Selesai pengukuran waktu
    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Benchmark tanpa prefetch selesai dalam waktu: %.4f detik\n", duration);

    // Bebaskan memori
    free(array);
}

int main() {
    printf("Mengukur waktu dengan dan tanpa prefetch:\n");

    // Jalankan benchmark dengan prefetch
    benchmark_prefetch();

    // Jalankan benchmark tanpa prefetch
    benchmark_no_prefetch();

    return 0;
}

