
#include "generate.h"
#include "tools.h"
#include <utility>
#include <iostream>

int main()
{
    auto bigen = BiDistributionGenerate<int>(11, 233);
    std::pair<int, int> gen_space[20];
    bigen.bi_normal_distribution(gen_space, 10, 1, 0, 10, 4);
    print_arr<int>(gen_space, 10);
}