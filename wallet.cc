#include <iostream>
#include "wallet.h"

Wallet::llong Wallet::LeftCoins = 2'100'000 * UNITS;

bool Wallet::EnoughCoins(Wallet::llong coins){
    return coins <= Wallet::LeftCoins;
}

Wallet::Wallet(){

}

Wallet::Wallet(std::string str){

}

Wallet::Wallet(llong coins){

}

Wallet::Wallet(Wallet &&w){

}

Wallet::Wallet(Wallet &&w1, Wallet &&w2){

}

Wallet Wallet::fromBinary(std::string str)
{
    llong count = 0;
    //str.size() duze?
    for(unsigned int i = 0; i < str.size(); i++){
        count = count * 2 + str[i] - '0';
        if(EnoughCoins(count))
            break;
    }
    if(EnoughCoins(count)){

    }
    else{

    }
    return Wallet(count);
}


