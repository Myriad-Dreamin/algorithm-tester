
# ifndef GENERATE_SORT_DATA
# define GENERATE_SORT_DATA

# ifndef TYPEDEF_ArrType
# define TYPEDEF_ArrType
typedef int ArrType;
# endif

#include <iostream>
#include <stdexcept>

#include <functional>
#include <random>
#include <ctime>

#include "std_distribution_desc.h"


class Generate
{
protected:
    int protect_size;
    unsigned long long random_seed;
    Generate (const int test_size, unsigned long long your_seed=0)
    {
        protect_size = test_size;
        random_seed = your_seed;
    }
};

class BiGenerate
{
protected:
    int protect_size;
    unsigned long long row_seed, col_seed;
    BiGenerate (const int test_size, unsigned long long your_seed=0)
    {
        protect_size = test_size;
        row_seed = your_seed;
        col_seed = your_seed * 0x123456879abcdefULL;
    }
};

/*
 * 生成器
 *
 * random_init: 随机生成, constant_array: 常值数列
 * increase: 升序数列, decrease: 降序数列
 * increase_step: 分段升序数列, decrease_step: 分段降序数列
 * distribute: 生成带分布的数列
 * 其他和standard random library中同名的函数为对应的分布生成函数.
 */

template<typename ArrType>
class LinearGenerate: public Generate
{
public:
    LinearGenerate ()
    : Generate(1 << 31, 0x32103123343678abULL) 
    {}

    LinearGenerate (const int test_size, unsigned long long your_seed=0)
    : Generate(test_size, your_seed) 
    {}

    inline void random_init(ArrType load_arr[], const int len)
    {
        static std::mt19937 mt_rand(random_seed);
        if (len >= protect_size) {
            throw std::invalid_argument("length to initialize is out of range");
        }
        for (int i = 0; i < len; i++) {
            load_arr[i] = mt_rand();
        }
    }

    inline void constant_array(ArrType load_arr[], const int len)
    {
        if (len >= protect_size) {
            throw std::invalid_argument("length to initialize is out of range");
        }
        for (int i = 0; i < len; i++) {
            load_arr[i] = 1;
        }
    }

    inline void increase(ArrType load_arr[], const int len)
    {
        if (len >= protect_size) {
            throw std::invalid_argument("length to initialize is out of range");
        }
        for (int i = 0; i < len; i++) {
            load_arr[i] = i;
        }
    }

    inline void increase_step(ArrType load_arr[], const int len, const int step)
    {
        if (len >= protect_size) {
            throw std::invalid_argument("length to initialize is out of range");
        }
        int elem=0;
        for (int i = 0; i < len; i+=step) {
            for (int j = std::min(step, len - i - 1); ~j; j--) {
                load_arr[i + j] = elem;
            }
        }
    }

    inline void decrease(ArrType load_arr[], const int len)
    {
        if (len >= protect_size) {
            throw std::invalid_argument("length to initialize is out of range");
        }
        for (int i = 0; i < len; i++) {
            load_arr[i] = len - i;
        }
    }

    inline void decrease_step(ArrType load_arr[], const int len, const int step)
    {
        if (len >= protect_size) {
            throw std::invalid_argument("length to initialize is out of range");
        }
        int elem = (1LL << 31) - 1;
        for (int i = 0; i < len; i+=step) {
            for (int j = std::min(step, len - i - 1); ~j; j--) {
                load_arr[i + j] = elem;
            }
        }
    }
};

template<typename ArrType>
class DistributionGenerate: public Generate
{
public:
    DistributionGenerate ()
    : Generate(1 << 31, 0x32103124745678abULL) 
    {}

    DistributionGenerate (const int test_size, unsigned long long your_seed=0)
    : Generate(test_size, your_seed) 
    {}

    inline void uniform_int_distribution(
        ArrType load_arr[],
        const int len,
        const int Interval_l,
        const int Interval_r
    ) {
        static std::mt19937 mt_rand((random_seed * 23333 + 6049));
        if (len >= protect_size) {
            throw std::invalid_argument("length to initialize is out of range");
        }
        auto distribution_generator = std::bind(
            std::uniform_int_distribution<ArrType>(Interval_l, Interval_r),
            mt_rand
        );
        for (int i = 0; i < len; i++) {
            load_arr[i] = distribution_generator();
        }
    }

    inline void normal_distribution(
        ArrType load_arr[],
        const int len,
        const double mu,
        const double varx
    ) {
        static std::mt19937 mt_rand((random_seed * 23333 + 9999));
        if (len >= protect_size) {
            throw std::invalid_argument("length to initialize is out of range");
        }
        auto distribution_generator = std::bind(
            std::normal_distribution<double>(mu, varx),
            mt_rand
        );
        for (int i = 0; i < len; i++) {
            load_arr[i] = ArrType(distribution_generator());
        }
    }

    inline void student_t_distribution(ArrType load_arr[], const int len, const double nt)
    {
        static std::mt19937 mt_rand((random_seed * 23333 + 12366));
        if (len >= protect_size) {
            throw std::invalid_argument("length to initialize is out of range");
        }
        auto distribution_generator = std::bind(
            std::student_t_distribution<double>(nt),
            mt_rand
        );
        for (int i = 0; i < len; i++) {
            load_arr[i] = ArrType(distribution_generator());
        }
    }

    inline void exponential_distribution(ArrType load_arr[], const int len, const double lambda)
    {
        static std::mt19937 mt_rand((random_seed * 23333 + 19923));
        if (len >= protect_size) {
            throw std::invalid_argument("length to initialize is out of range");
        }
        auto distribution_generator = std::bind(
            std::exponential_distribution<double>(lambda),
            mt_rand
        );
        for (int i = 0; i < len; i++) {
            load_arr[i] = ArrType(distribution_generator());
        }
    }

    inline void negative_binomial_distribution(
        ArrType load_arr[],
        const int len,
        const int nt,
        const double p
    ) {
        static std::mt19937 mt_rand((random_seed * 23333 + 3535));
        if (len >= protect_size) {
            throw std::invalid_argument("length to initialize is out of range");
        }
        auto distribution_generator = std::bind(
            std::negative_binomial_distribution<ArrType>(nt, p),
            mt_rand
        );
        for (int i = 0; i < len; i++) {
            load_arr[i] = distribution_generator();
        }
    }

    void distribution(ArrType load_arr[], const int len, std_distribution input_distribution)
    {
        static std::mt19937 mt_rand((random_seed * 23333 + 1234567));
        if (len >= protect_size) {
            throw std::invalid_argument("length to initialize is out of range");
        }
        switch(input_distribution)
        {
            case std_distribution::uniform_int_distribution: {
                auto distribution_generator = std::bind(
                    std::uniform_int_distribution<ArrType>(0, 20),
                    mt_rand
                );
                for (int i = 0; i < len; i++) {
                    load_arr[i] = distribution_generator();
                }
                break;
            }

            case std_distribution::normal_distribution: {
                auto distribution_generator = std::bind(
                    std::normal_distribution<double>(0, 1),
                    mt_rand
                );
                for (int i = 0; i < len; i++) {
                    load_arr[i] = ArrType(distribution_generator());
                }
                break;
            }

            case std_distribution::student_t_distribution: {
                auto distribution_generator = std::bind(
                    std::student_t_distribution<double>(20),
                    mt_rand
                );
                for (int i = 0; i < len; i++) {
                    load_arr[i] = ArrType(distribution_generator());
                }
                break;
            }

            case std_distribution::negative_binomial_distribution: {
                auto distribution_generator = std::bind(
                    std::negative_binomial_distribution<ArrType>(20, 0.5),
                    mt_rand
                );
                for (int i = 0; i < len; i++) {
                    load_arr[i] = distribution_generator();
                }
                break;
            }

            case std_distribution::exponential_distribution: {
                auto distribution_generator = std::bind(
                    std::exponential_distribution<double>(1),
                    mt_rand
                );
                for (int i = 0; i < len; i++) {
                    load_arr[i] = ArrType(distribution_generator());
                }
                break;
            }
            default: {
                throw("no such distribution supported" + distribute_desc(input_distribution));
            }
        }
    }
};

template<typename ArrType>
class BiDistributionGenerate: public BiGenerate
{
public:
    BiDistributionGenerate ()
    : BiGenerate(1 << 31, 0x32103215745678abULL) 
    {}

    BiDistributionGenerate (const int test_size, unsigned long long your_seed=0)
    : BiGenerate(test_size, your_seed) 
    {}

    // dependent bi variable
    inline void bi_normal_distribution(
        std::pair<ArrType, ArrType> load_arr[],
        const int length,
        const double mu1 = 1,
        const double varx1 = 0,
        const double mu2 = 1,
        const double varx2 = 0
    ) {
        static std::mt19937 mt_rand_row((row_seed * 23333 + 9999));
        static std::mt19937 mt_rand_col((col_seed * 23333 + 9999));
        if (length >= protect_size) {
            throw std::invalid_argument("row length to initialize is out of range");
        }

        auto row_distribution_generator = std::bind(
            std::normal_distribution<double>(mu1, varx1),
            mt_rand_row
        );
        auto col_distribution_generator = std::bind(
            std::normal_distribution<double>(mu2, varx2),
            mt_rand_col
        );

        for (int i = 0; i < length; i++) {
            load_arr[i] = std::make_pair(
                ArrType(row_distribution_generator()),
                ArrType(col_distribution_generator())
            );
        }
    }
};

# endif // GENERATE_SORT_DATA