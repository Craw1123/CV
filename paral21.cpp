#include <iostream>
#include <immintrin.h>
#include <x86intrin.h>

int main() {
    constexpr long long iterations = 10^9;
    __m128d vecA = _mm_set1_pd(333.444);
    __m128d vecB = _mm_set1_pd(555.666);
    __m128d sum = _mm_setzero_pd();

    unsigned long long start = __rdtsc();

    for (int i = 0; i < iterations; ++i) {
        sum = _mm_add_pd(sum, _mm_div_pd(vecA, vecB));
    }

    unsigned long long end = __rdtsc();
    
    double avg_ticks = static_cast<double>(end - start) / iterations;
    std::cout << "Average ticks for vector float division: " << avg_ticks << " ticks." << std::endl;



    return 0;
}
