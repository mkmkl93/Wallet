#include "wallet.h"

#include <cassert>
#include <iostream>
#include <exception>
#include <chrono>
#include <vector>
#include <numeric>
#include <chrono>

//static const uint UNITS_IN_B = 100000000;

using std::move;

int main() {
//        Wallet w1, w2;
//    w1 = Wallet(1); // OK
//    w1 = w2; // błąd kompilacji
//
//    Wallet w1(1);
//    std::chrono::high_resolution_clock::time_point before = std::chrono::high_resolution_clock::now();
//    std::vector<int> v;
//    for(int i=0;i<1000;i++){
//        v.emplace_back(i);
//    }
//    std::chrono::high_resolution_clock::time_point Now = std::chrono::high_resolution_clock::now();
//    std::chrono::nanoseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(Now - Wallet::TimeStart);
//    std::cout<<time.count()<<"\n";
//    Wallet suma1 = w1 + Wallet(1); // błąd kompilacji
//    Wallet suma2 = Wallet(2) + w2; // OK, w w2 jest 0 B po operacji
                                   // i jeden nowy wpis w historii,
                                   // a w suma2 jest w2.getUnits() + 2 B.
                                   // Historia operacji powstałego obiektu
                                   // zależy od implementacji.
//    Wallet suma3 = suma1 + suma2;  // błąd kompilacji
//    Wallet suma4 = Wallet(1) + Wallet(2);  // OK, suma4 ma dwa wpisy
                                           // w historii i 3 B
//    assert(Empty() == 0);
//    assert(Empty() < Wallet(1));
//    assert(Empty().getUnits() == 0);
//    assert(Empty().opSize() == 1);
//    assert(Empty()[0].getUnits() == 0);
//
//    Wallet w1;
//    assert(w1 == Empty());
//    assert(w1.opSize() == 1);
//
//    Wallet w2(1), w3(1);
//    assert(w2.opSize() == 1);
//    assert(w2.getUnits() == UNITS_IN_B);
//
//    assert(Wallet::fromBinary("100") == Wallet(4));
//    assert(Wallet(" 1.2000 ") == Wallet("1,2"));
//
//    assert(Wallet(2) + w2 == Wallet(3));
//    assert(Wallet(1) + Wallet(2) == Wallet(3));
//    assert(1 + 2 == Wallet(3));
//
//    Wallet w4;
//    assert(w4.opSize() == 1);
//
//    w4 += 2;
//    assert(w4.opSize() == 2);
//    assert(w4[0] <= w4[1]);
//    assert(w4[1].getUnits() == w4.getUnits());
//
//    w4 *= 3;
//    assert(w4.opSize() == 3);
//    assert(w4[2].getUnits() == 6 * UNITS_IN_B);
//
//    assert(Wallet(move(w4)).opSize() == 4);
//
//    Wallet w5{2};
//    Wallet w6 = Wallet(10) - w5;
//    assert(w5 == Wallet(4));
//    assert(w6 == Wallet(8));
}
