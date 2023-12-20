#include <iostream>
#include <x86intrin.h>

int main() {
    constexpr long long iterations = 10^5;
    double a = 333.444, b = 555.666;
    double result = 0.0;

    unsigned long long start = __rdtsc();
    
    for (int i = 0; i < iterations; ++i) {
        result += a / b;
    }
    
    unsigned long long end = __rdtsc();
    
    
    double avg_ticks = static_cast<double>(end - start);
    std::cout << "Average ticks for float division: " << avg_ticks << std::endl;
   

    return 0;
}
