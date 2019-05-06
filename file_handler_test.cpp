

#include "tools.h"


int main() {
    FileHandler fp("./test.txt");
    fp.put('a');
    fp.close();
}