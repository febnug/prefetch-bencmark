#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ARRAY_SIZE 100000000 // Ukuran array untuk pengujian

// Menggunakan RDTSC untuk membaca Time-Stamp Counter
static inline uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__(
        "rdtsc" : "=a"(lo), "=d"(hi)
    );
    return ((uint64_t)hi << 32) | lo;
}

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

    // Mulai pengukuran waktu dengan RDTSC
    uint64_t start = rdtsc();

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

    // Selesai pengukuran waktu dengan RDTSC
    uint64_t end = rdtsc();
    uint64_t duration = end - start;

    printf("Benchmark dengan prefetch selesai dalam %lu siklus CPU\n", duration);

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

    // Mulai pengukuran waktu dengan RDTSC
    uint64_t start = rdtsc();

    // Akses array tanpa prefetch
    for (int i = 0; i < ARRAY_SIZE; i++) {
        int temp = array[i];
    }

    // Selesai pengukuran waktu dengan RDTSC
    uint64_t end = rdtsc();
    uint64_t duration = end - start;

    printf("Benchmark tanpa prefetch selesai dalam %lu siklus CPU\n", duration);

    // Bebaskan memori
    free(array);
}

int main() {
    printf("Mengukur waktu dengan dan tanpa prefetch menggunakan RDTSC:\n");

    // Jalankan benchmark dengan prefetch
    benchmark_prefetch();

    // Jalankan benchmark tanpa prefetch
    benchmark_no_prefetch();

    return 0;
}

