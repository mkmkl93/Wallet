#include <iostream>
#include <ctime>
#include <typeinfo>
#include <algorithm>
#include "wallet.h"

Wallet::coins_t Wallet::LeftCoins = 2'100'000 * UNITS;

bool Wallet::EnoughCoins(Wallet::coins_t coins){
    return coins <= Wallet::LeftCoins;
}

Wallet::coins_t Wallet::StringToCoins(std::string str){
    unsigned int DotPos = str.find(".",0);
    unsigned int ComaPos = str.find(",",0);
    if(DotPos != str.size() || ComaPos != str.size())
        for(unsigned int i = str.size(); i < std::min(DotPos,ComaPos) + 8; i++)
            str += "0";
    coins_t count = 0;
    for(unsigned int i = 0; i < str.size(); i++)
    {
        if(str[i] < '0' || str[i] > '9')
            continue;
        count = count * 10 + str[i] - '0';
        if(!EnoughCoins(count))
            break;
    }
    if(!EnoughCoins(count))
        throw(NotEnoughCoins());
    return count;
}

Wallet::Wallet(){
    HistoryOfOperations.NewEvent("Created empty Wallet");
}

Wallet::Wallet(std::string str){
    //miejsce na parser
    try{
        coins = Wallet::StringToCoins(str);
    }catch(NotEnoughCoins &e){
        throw(NotEnoughCoins());
    }

    HistoryOfOperations.NewEvent("Created Wallet from string with value " + std::to_string(coins));
    LeftCoins -= coins;
}

Wallet::Wallet(coins_t coins){

    try{
        EnoughCoins(coins * UNITS);
    }catch(NotEnoughCoins &e){
        throw(NotEnoughCoins());
    }
    std::cout<<"z inta\n";
    HistoryOfOperations.NewEvent("Created Wallet from integer with value " + std::to_string(coins));
    LeftCoins -= coins * UNITS;
}

Wallet::Wallet(Wallet &&w){
    coins = w.coins;
    HistoryOfOperations = w.HistoryOfOperations;
    w.coins = 0;
    w.HistoryOfOperations.Operations.clear();
    HistoryOfOperations.NewEvent("Created Wallet from another Wallet with value " + std::to_string(coins));
}

Wallet::Wallet(Wallet &&w1, Wallet &&w2){
    coins = w1.coins + w2.coins;
    HistoryOfOperations = w1.HistoryOfOperations;
    HistoryOfOperations += w2.HistoryOfOperations;
    HistoryOfOperations.sort();
    w1.HistoryOfOperations.Operations.clear();
    w2.HistoryOfOperations.Operations.clear();
    w1.coins = 0;
    w2.coins = 0;
    HistoryOfOperations.NewEvent("Created Wallet from other Wallets with value " + std::to_string(coins));
}

Wallet Wallet::fromBinary(std::string str)
{
    coins_t count = 0;
    //str.size() duze?
//    try{
        for(unsigned int i = 0; i < str.size(); i++){
            count = count * 2 + str[i] - '0';
            if(!EnoughCoins(count))
                break;
        }
        if(!EnoughCoins(count)){

        }
        else{

        }
//    }

    return Wallet(count);
}

Wallet::coins_t Wallet::getUnits(){
    return Wallet::coins;
}

unsigned int Wallet::opSize(){
    return HistoryOfOperations.Operations.size();
}

Wallet::coins_t Wallet::getCoins()
{
    return coins;
}

const Wallet Empty()
{
    return Wallet();
}

Operation::Operation(time_t t, std::string s){
    time = t;
    name = s;
}

void History::NewEvent(std::string event){
    Operations.emplace_back( Operation( time(0), event) );
}

Wallet& Wallet::operator=(Wallet&& other) // copy assignment
{
    //"Jeżeli oba obiekty są tym samym obiektem, to nic nie robi" nie zachodzi?
    if(this != &other)
    {
        this->coins = other.coins;
        this->HistoryOfOperations = other.HistoryOfOperations;
        this->HistoryOfOperations.NewEvent("Assigned value from another Wallet " + std::to_string(coins));
    }
    return *this;
}

History& History::operator+=(const History &rhs){
    for(unsigned int i = 0; i < rhs.Operations.size(); i++)
        this->Operations.emplace_back(rhs.Operations[i]);
    return *this;
}

Wallet operator+(Wallet&& lhs, Wallet rhs)
{
    Wallet ret;
    ret.coins = lhs.coins + rhs.coins;
    ret.HistoryOfOperations = lhs.HistoryOfOperations;
    ret.HistoryOfOperations += rhs.HistoryOfOperations;
    ret.HistoryOfOperations.sort();
    lhs.coins = 0;
    rhs.coins = 0;
    rhs.HistoryOfOperations.NewEvent("Assigned value to another Wallet " + std::to_string(rhs.coins));
    return ret;
}

bool operator<(const Operation &lhs, const Operation &rhs){
    if(lhs.time == rhs.time)
        return lhs.name < rhs.name;
    return lhs.time < rhs.time;
}

void History::sort()
{
    std::sort(Operations.begin(), Operations.end());
}
