
#include <iostream>
#include "tools.h"
#include "generate.h"

int main ()
{
	auto functory = bivari::normal_distribution::generator<int>(1.0, 0.1, 1.0, 0.1);
	auto p = functory();
	std::cout << p.first << " " << p.second << std::endl;
	p = functory();
	std::cout << p.first << " " << p.second << std::endl;

	auto functory_ex = bivari::normal_distribution::functory_fixed<double>(4, 0.1);
	auto funcx = functory_ex(2, 2);
	p = funcx();
	std::cout << p.first << " " << p.second << std::endl;
	p = funcx();
	std::cout << p.first << " " << p.second << std::endl;

	auto funcy = functory_ex(2, 4);
	p = funcy();
	std::cout << p.first << " " << p.second << std::endl;
	p = funcy();
	std::cout << p.first << " " << p.second << std::endl;
	p = funcy();
	std::cout << p.first << " " << p.second << std::endl;
	p = funcy();
	std::cout << p.first << " " << p.second << std::endl;
	p = funcy();
	std::cout << p.first << " " << p.second << std::endl;
	p = funcy();
	std::cout << p.first << " " << p.second << std::endl;
	p = funcy();
	std::cout << p.first << " " << p.second << std::endl;
	p = funcy();
	std::cout << p.first << " " << p.second << std::endl;
	p = funcy();
	std::cout << p.first << " " << p.second << std::endl;
	p = funcy();
	std::cout << p.first << " " << p.second << std::endl;

}