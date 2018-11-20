#include "wallet.h"

#include <cassert>
#include <iostream>
#include <exception>
#include <chrono>
#include <vector>
#include <numeric>
#include <chrono>

static const uint UNITS_IN_B = 100000000;

using std::move;

int main() {
    assert(Empty() == 0);
    int i = 1;
    std::cout << i++ << std::endl;

    Wallet w1000("1.1000");
    std::cout << i++ << std::endl;
    Wallet w999("1,1");
    std::cout << i++ << std::endl;
    assert(Wallet("1,1     ") == Wallet("1.1"));
    std::cout << i++ << std::endl;
    Wallet w1001(1);
    std::cout << i++ << std::endl;
    Wallet w1002(0);
    std::cout << i++ << std::endl;

    assert(Empty() < Wallet(1));
    std::cout << i++ << std::endl;
    assert(Empty().getUnits() == 0);
    std::cout << i++ << std::endl;
    assert(Empty().opSize() == 1);
    std::cout << i++ << std::endl;
    assert(Empty()[0].getUnits() == 0);
    std::cout << i++ << std::endl;
    Wallet w1;
    assert(w1 == Empty());
    std::cout << i++ << std::endl;
    assert(w1.opSize() == 1);
    std::cout << i++ << std::endl;

    Wallet w2(1), w3(1);
    assert(w2.opSize() == 1);
    std::cout << i++ << std::endl;
    assert(w2.getUnits() == UNITS_IN_B);
    std::cout << i++ << std::endl;

    assert(Wallet::fromBinary("100") == Wallet(4));
    std::cout << i++ << std::endl;
    assert(Wallet(" 1.2000 ") == Wallet("1,2"));
    std::cout << i++ << std::endl;

    assert(Wallet(2) + w2 == Wallet(3));
    std::cout << i++ << std::endl;
    assert(Wallet(1) + Wallet(2) == Wallet(3));
    std::cout << i++ << std::endl;
    assert(1 + 2 == Wallet(3));
    std::cout << i++ << std::endl;

    Wallet w4;
    assert(w4.opSize() == 1);
    std::cout << i++ << std::endl;
//
    w4 += 2;
    std::cout << i++ << std::endl;
    assert(w4.opSize() == 2);
    std::cout << i++ << std::endl;
    assert(w4[0] <= w4[1]);
    std::cout << i++ << std::endl;
    assert(w4[1].getUnits() == w4.getUnits());
    std::cout << i++ << std::endl;
//
    w4 *= 3;
    std::cout << i++ << std::endl;
    assert(w4.opSize() == 3);
    std::cout << i++ << std::endl;
    assert(w4[2].getUnits() == 6 * UNITS_IN_B);
    std::cout << i++ << std::endl;
//
    assert(Wallet(move(w4)).opSize() == 4);
    std::cout << i++ << std::endl;
//
    Wallet w5{2};
    std::cout << i++ << std::endl;
    Wallet w6 = Wallet(10) - w5;
    std::cout << i++ << std::endl;
    assert(w5 == Wallet(4));
    std::cout << i++ << std::endl;
    assert(w6 == Wallet(8));
    std::cout << i++ << std::endl;

    std::cout << w4[0];
    std::cout << w4[1];
    std::cout << w4[2];

    Wallet w997("1.0123");
    std::cout << w997[0];

    std::cout << w997;
    std::cout << w4;
    std::cout << w1001;
    std::cout << w1000;
}
