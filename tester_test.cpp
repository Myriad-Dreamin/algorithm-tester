
#define DEBUG

#include <iostream>
#include "tester.h"


class MyTester: Tester
{
public:
    MyTester(): Tester() {}
    MyTester(const std::string &b): Tester(b) {}
    void primitive(int p1)
    {
        p1 = 1;
        run([p1]() mutable -> void {
            p1 = 2;
            std::cout << "test func: " << p1 << std::endl;
        },  [p1]() mutable -> void {
            std::cout << "assert func: " << p1 << std::endl;
        });
    }
} tester;


int main()
{
    int p1 = 3;
    tester.primitive(p1);
}
