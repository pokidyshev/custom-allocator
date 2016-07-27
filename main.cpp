#include <iostream>
#include <vector>

int main()
{
    std::vector<int> v = { 1, 2, 3 };
    std::vector<int>::iterator i = v.begin();
    std::cout << (--i > v.begin());
    for (int j = 0; j < 100; j++) {
        --i;
    }
    i = v.end();
    for (int j = 0; j < 100; j++) {
        ++i;
    }
    std::cout << (--i > v.begin());
    return 0;
}