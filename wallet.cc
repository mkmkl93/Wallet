#include <iostream>
#include <ctime>
#include <typeinfo>
#include <algorithm>
#include <chrono>
#include "wallet.h"

Wallet::coins_t Wallet::LeftCoins = 2'100'000 * UNITS;

std::chrono::high_resolution_clock::time_point Wallet::TimeStart = std::chrono::high_resolution_clock::now();

void Wallet::EnoughCoins(Wallet::coins_t coins){
    if(coins > Wallet::LeftCoins)
        throw(NotEnoughCoins());
}

Wallet::coins_t Wallet::StringToCoins(std::string str){
    unsigned int DotPos = str.find(".",0);
    unsigned int ComaPos = str.find(",",0);
    if(DotPos != str.size() || ComaPos != str.size())
        for(unsigned int i = str.size(); i < std::min(DotPos,ComaPos) + 8; i++)
            str += "0";
    coins_t count = 0;
    try{
        for(unsigned int i = 0; i < str.size(); i++)
        {
            if(str[i] < '0' || str[i] > '9')
                continue;
            count = count * 10 + str[i] - '0';
            EnoughCoins(count);
        }
    }catch(NotEnoughCoins e){
        throw(NotEnoughCoins());
    }
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
    HistoryOfOperations.NewEvent("Created Wallet from integer with value " + std::to_string(coins));
//    std::cout<<HistoryOfOperations.Operations[0].ms.count()<<"\n";
    LeftCoins -= coins * UNITS;
}

Wallet::Wallet(Wallet &&w){
    coins = w.coins;
    HistoryOfOperations = w.HistoryOfOperations;
    w.coins = 0;
//    w.HistoryOfOperations.Operations.clear();
    HistoryOfOperations.NewEvent("Created Wallet from another Wallet with value " + std::to_string(coins));
}

Wallet::Wallet(Wallet &&w1, Wallet &&w2){
    coins = w1.coins + w2.coins;
    HistoryOfOperations = w1.HistoryOfOperations;
    HistoryOfOperations += w2.HistoryOfOperations;
    HistoryOfOperations.sort();
//    w1.HistoryOfOperations.Operations.clear();
//    w2.HistoryOfOperations.Operations.clear();
    w1.coins = 0;
    w2.coins = 0;
    HistoryOfOperations.NewEvent("Created Wallet from other Wallets with value " + std::to_string(coins));
}

Wallet Wallet::fromBinary(std::string str)
{
    coins_t count = 0;
    try{
        for(unsigned int i = 0; i < str.size(); i++){
            count = count * 2 + str[i] - '0';
            EnoughCoins(count);
        }
    }
    catch(NotEnoughCoins e){
        throw(NotEnoughCoins());
    }
    return Wallet(count);
}

Wallet::coins_t Wallet::getUnits(){
    return Wallet::coins;
}

unsigned int Wallet::opSize(){
    return HistoryOfOperations.OperationsSize();
}

Wallet::coins_t Wallet::getCoins()
{
    return coins;
}

const Wallet Empty()
{
    return Wallet();
}



void Wallet::History::NewEvent(std::string event){
    std::chrono::high_resolution_clock::time_point Now = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(Now - Wallet::TimeStart);
    Operations.emplace_back( Operation( time, event) );
}

Wallet::History::Operation::Operation(std::chrono::milliseconds ms, std::string s){
    this->ms = ms;
    name = s;
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

Wallet::History& Wallet::History::operator+=(const Wallet::History &rhs){
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

Wallet operator-(Wallet&& lhs, Wallet rhs)
{
    if(lhs.coins < rhs.coins)
        throw(NegativeBankBalance());
    Wallet ret;
    ret.coins = lhs.coins - rhs.coins;
    ret.HistoryOfOperations = lhs.HistoryOfOperations;
    ret.HistoryOfOperations += rhs.HistoryOfOperations;
    ret.HistoryOfOperations.sort();
    lhs.coins -= rhs.coins;
    rhs.coins *= 2;
    rhs.HistoryOfOperations.NewEvent("Substracted value from another Wallet " + std::to_string(rhs.coins));
    return ret;
}

bool operator<(const Wallet::History::Operation &lhs, const Wallet::History::Operation &rhs){
    if(lhs.ms == rhs.ms)
        return lhs.name < rhs.name;
    return lhs.ms < rhs.ms;
}

void Wallet::History::sort()
{
    std::sort(Operations.begin(), Operations.end());
}

Wallet::~Wallet(){
    HistoryOfOperations.clear();
    LeftCoins += coins;
}

Wallet operator*(Wallet::coins_t lhs, Wallet rhs){
    try{
        rhs.EnoughCoins( lhs * rhs.UNITS );
    }catch(NotEnoughCoins e){
        throw(NotEnoughCoins());
    }
    Wallet ret(lhs * rhs.UNITS);
    ret.HistoryOfOperations.NewEvent("Created from multiplication coins_t times Wallet.coins " + std::to_string(ret.coins));
    return ret;
}

Wallet operator*(Wallet lhs, Wallet::coins_t rhs){
    try{
        lhs.EnoughCoins( rhs * lhs.UNITS );
    }catch(NotEnoughCoins e){
        throw(NotEnoughCoins());
    }
    Wallet ret(rhs * lhs.UNITS);
    ret.HistoryOfOperations.NewEvent("Created from multiplication Wallet coins times coins_t " + std::to_string(ret.coins));
    return ret;
}

unsigned int Wallet::History::OperationsSize(){
    return Operations.size();
}

void Wallet::History::clear(){
    Operations.clear();
}
