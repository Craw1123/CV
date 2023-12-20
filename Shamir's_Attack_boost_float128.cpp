#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <boost/multiprecision/float128.hpp>

using namespace boost::multiprecision;

std::vector<int> w = {2, 7, 11, 21, 42};
int n = 5;
int q = 101;
int r = 93;
std::vector<int> b = {85, 45, 13, 34, 68};

std::vector<float128> find_interval_points(std::vector<int> hard_seq) {
    std::vector<float128> i_list;
    
    for (int i = 1; i < hard_seq[0]; i++) {
        for (int j = 1; j < hard_seq[1]; j++) {
            if (0 <= static_cast<float128>(i) / hard_seq[0] - static_cast<float128>(j) / hard_seq[1] && static_cast<float128>(i) / hard_seq[0] - static_cast<float128>(j) / hard_seq[1] <= 1 / (std::pow(2, 5 - 2) * hard_seq[1])) {
                for (int k = 1; k < hard_seq[2]; k++) {
                    if (0 <= static_cast<float128>(i) / hard_seq[0] - static_cast<float128>(k) / hard_seq[2] && static_cast<float128>(i) / hard_seq[0] - static_cast<float128>(k) / hard_seq[2] <= 1 / (std::pow(2, 5 - 3) * hard_seq[2])) {
                        for (int l = 1; l < hard_seq[3]; l++) {
                            if (0 <= static_cast<float128>(i) / hard_seq[0] - static_cast<float128>(l) / hard_seq[3] && static_cast<float128>(i) / hard_seq[0] - static_cast<float128>(l) / hard_seq[3] <= 1 / (std::pow(2, 5 - 4) * hard_seq[3])) {
                                i_list.push_back(static_cast<float128>(i));
                            }
                        }
                    }
                }
            }
        }
    }
    
    std::sort(i_list.begin(), i_list.end(), std::greater<float128>());
    i_list.erase(std::unique(i_list.begin(), i_list.end()), i_list.end());
    
    return i_list;
}

std::vector<float128> find_discontinuity_points_on_interval(std::vector<int> hard_seq, float128 start_point) {
    std::vector<float128> breakings;
    std::pair<float128, float128> inter = std::make_pair(start_point / hard_seq[0], (start_point + 1) / hard_seq[0]);
    breakings.push_back(start_point / hard_seq[0]);
    
    for (int i : hard_seq) {
        for (int j = 0; j < i; j++) {
            if (static_cast<float128>(j) / i >= inter.second) {
                break;
            }
            else if (static_cast<float128>(j) / i <= inter.first) {
                continue;
            }
            else {
                breakings.push_back(static_cast<float128>(j) / i);
            }
        }
    }
    
    breakings.push_back((start_point + 1) / hard_seq[0]);
    
    std::sort(breakings.begin(), breakings.end());
    breakings.erase(std::unique(breakings.begin(), breakings.end()), breakings.end());
    
    return breakings;
}

std::vector<float128> find_possible_intervals_for_WM(std::vector<float128> intervals, std::vector<int> hard_seq) {
    std::vector<float128> res;
    std::vector<int> c;
    
    for (int j = 0; j < intervals.size() - 1; j++) {
        bool is_f = true;
        float128 left_border = intervals[j];
        float128 right_border = intervals[j + 1];
        
        for (int i = 0; i < hard_seq.size(); i++) {
            c.push_back(int(hard_seq[i] * left_border));
        }
        
        float128 d = (1.0 + std::accumulate(c.begin(), c.end(), 0)) / std::accumulate(hard_seq.begin(), hard_seq.end(), 0);
        
        if (d < left_border) {
            continue;
        }
        else if (d < right_border) {
            right_border = d;
        }
        
        for (int i = 2; i < hard_seq.size(); i++) {
            int part_hard_seq_sum = 0;
            int part_c_sum = 0;
            
            for (int k = 0; k < i; k++) {
                part_hard_seq_sum += hard_seq[k];
            }
            
            float128 alpha = -hard_seq[i] + part_hard_seq_sum;
            
            for (int k = 0; k < i; k++) {
                part_c_sum += c[k];
            }
            
            float128 beta = -c[i] + part_c_sum;
            
            if (alpha > 0) {
                if ((beta / alpha) <= left_border) {
                    is_f = false;
                    break;
                }
                else if ((beta / alpha) < right_border) {
                    right_border = beta / alpha;
                }
            }
            else if (alpha < 0) {
                if ((beta / alpha) >= right_border) {
                    is_f = false;
                    break;
                }
                else if ((beta / alpha) > left_border) {
                    left_border = beta / alpha;
                }
            }
        }
        
        if (is_f) {
            res.push_back(left_border);
            res.push_back(right_border);
            return res;
        }
    }
    
    return res;
}

std::pair<int, int> find_fraction_in_interval(std::pair<float128, float128> interval) {
    int numerator = 0;
    int denominator = 1;
    
    while (true) {
        if (numerator / static_cast<float128>(denominator) <= interval.first) {
            numerator += 1;
        }
        else if (numerator / static_cast<float128>(denominator) >= interval.second) {
            denominator += 1;
        }
        else {
            return std::make_pair(numerator, denominator);
        }
    }
}

std::vector<int> get_unique_numbers(std::vector<int> numbers) {
    std::vector<int> unique;
    
    for (int number : numbers) {
        if (std::find(unique.begin(), unique.end(), number) == unique.end()) {
            unique.push_back(number);
        }
    }
    
    std::sort(unique.begin(), unique.end());
    
    return unique;
}

bool is_superincreasing_seq(std::vector<int> seq) {
    int counter = 0;
    
    for (int i : seq) {
        if (counter < i) {
            counter += i;
        }
        else {
            return false;
        }
    }
    
    return true;
}

std::vector<int> construct_superincreasing_seq(int factor, int modulus, std::vector<int> hard_seq) {
    std::vector<int> easy_seq;
    
    for (int i : hard_seq) {
        easy_seq.push_back((i * factor) % modulus);
    }
    
    return easy_seq;
}

int main() {
    std::vector<float128> start_int_points = find_interval_points(b);
    
    for (float128 i : start_int_points) {
        std::vector<float128> inters = find_discontinuity_points_on_interval(b, i);
        
        std::vector<float128> res = find_possible_intervals_for_WM(inters, b);
        
        if (!res.empty()) {
            std::pair<int, int> fraction = find_fraction_in_interval(std::make_pair(res[0], res[1]));
            std::cout << "fraction is " << fraction.first << " / " << fraction.second << std::endl;
            
            std::vector<int> easy_seq = construct_superincreasing_seq(fraction.first, fraction.second, b);
            
            if (is_superincreasing_seq(easy_seq)) {
                for (int i : easy_seq) {
                    std::cout << i << " ";
                }
                std::cout << std::endl;
            }
        }
    }
    
    return 0;
}

