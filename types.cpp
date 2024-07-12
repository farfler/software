#include <iostream>
#include <cstdint>

class MagicNumber {
private:
    int32_t value;

public:
    MagicNumber(int32_t val) : value(val) {}
    
    operator int32_t() const {
        return value;
    }
};

int main() {
    MagicNumber m1(42);
    int32_t n1 = m1;

    int32_t n2 = MagicNumber(100);

    std::cout << "n1: " << n1 << ", n2: " << n2 << std::endl;

    // Additional demonstration
    MagicNumber m2 = 55;  // Implicit conversion from int to MagicNumber
    int32_t n3 = m2;

    std::cout << "n3: " << n3 << std::endl;

    return 0;
}