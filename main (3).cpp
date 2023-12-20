#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include "PRNG_functions.h"
#include "functions.h"
#include "matstat.h"

//1 или 2 prng; характеристика; значение характеристики
//using my_tuple_matstat = std::tuple<std::string, std::string, double>;

int main()
{
    study::StudyPRNG1 custom_prng_1(std::random_device{}());
    study::StudyPRNG2 custom_prng_2(std::random_device{}());

   std::vector<std::vector<std::uint32_t>> samples1(10), samples2(10);
   for (std::size_t i = 0; i < 10; ++i)
   {
       for (std::size_t j = 0; j < 15000; ++j)
       {
           samples1[i].push_back(custom_prng_1());
           samples2[i].push_back(custom_prng_2());
       }
   }

   std::vector<my_tuple_matstat> matstat1;
   std::vector<my_tuple_matstat> matstat2;

//   for (const auto& sample : samples1)
//   {
//       for (std::size_t i = 0; i < sample.size(); ++i)
//           std::cout << sample[i] << " ";
//       std::cout << '\n';
//   }

   // Вектор квантилей для уровня значимости alpha = 0.5
   std::vector<double> quantile05 = {0.4549, 1.3863, 2.366, 3.3567, 4.3515, 5.3481, 6.3458, 7.3441, 8.3428,
                                    9.3418, 10.341, 11.3403, 12.3398, 13.3393, 14.3389, 15.3385, 16.3882, 17.3379,
                                    18.3377, 19.3374, 20.3372, 21.3370, 22.3369, 23.3367, 24.3366, 25.3365, 26.3363,
                                    27.3362, 28.3361, 29.3360, 30.3359, 31.3359, 32.3358, 33.3356, 34.3356, 35.3357,
                                    36.3355, 37.3355, 38.3354, 39.3353, 40.3353, 41.3352, 42.3352, 43.3352, 44.3351,
                                    45.3351, 46.335, 47.335, 48.335, 49.3349};

   std::cout << "Calculating mean, variance, variability, checking chi-squared... " << std::flush;
   for (std::size_t i = 0; i < 10; ++i)
   {
       matstat1.emplace_back(get_matstat<std::uint32_t>(samples1[i], custom_prng_1.min(), custom_prng_1.max(), quantile05));
       matstat2.emplace_back(get_matstat<std::uint32_t>(samples2[i], custom_prng_2.min(), custom_prng_2.max(), quantile05));
   }
   std::cout << "Done\n";

   matstat_to_csv("matstat1.csv", matstat1);
   matstat_to_csv("matstat2.csv", matstat2);

   std::mt19937 mtprng(std::random_device{}());

   std::vector<std::size_t> sizes = {1000, 7000, 10000, 15000, 20000, 50000,
                                     150000, 180000, 250000, 370000, 430000,
                                     563000, 700000, 832000, 945000, 1000000};

   using my_tuple_time = std::tuple<std::size_t, std::string, std::uint32_t>;
   std::vector<my_tuple_time> timings;

   using namespace std::chrono;
   time_point<high_resolution_clock> start, end;
   std::string name_prng;
   std::uint32_t time;
   std::uint32_t generated;

   for (std::size_t size : sizes)
   {
       std::cout << "Working with size: " << size << "\n";
       name_prng = "First custom PRNG";
       start = high_resolution_clock::now();
       for (std::size_t t = 0; t < size; ++t)
           generated = custom_prng_1();
       end = high_resolution_clock::now();
       time = static_cast<std::uint32_t>(duration_cast<nanoseconds>(end - start).count());
       timings.emplace_back(std::make_tuple(size, name_prng, time));

       name_prng = "Second custom PRNG";
       start = high_resolution_clock::now();
       for (std::size_t t = 0; t < size; ++t)
           generated = custom_prng_2();
       end = high_resolution_clock::now();
       time = static_cast<std::uint32_t>(duration_cast<nanoseconds>(end - start).count());
       timings.emplace_back(std::make_tuple(size, name_prng, time));

       name_prng = "Mersenne-Twister PRNG";
       start = high_resolution_clock::now();
       for (std::size_t t = 0; t < size; ++t)
           generated = mtprng();
       end = high_resolution_clock::now();
       time = static_cast<std::uint32_t>(duration_cast<nanoseconds>(end - start).count());
       timings.emplace_back(std::make_tuple(size, name_prng, time));
   }

    times_to_csv("timings.csv", timings);
}
