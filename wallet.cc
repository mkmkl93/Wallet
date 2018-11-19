#include <iostream>
#include <ctime>
#include <typeinfo>
#include <algorithm>
#include <chrono>
#include <regex>
#include "wallet.h"

struct NotEnoughCoins : public std::exception
{
	const char * what () const throw (){
    	return "NotEnoughCoins";
    }
};

struct NegativeBankBalance : public std::exception
{
	const char * what () const throw (){
    	return "NegativeBankBalance";
    }
};

struct InvalidInput : public std::exception {
    const char * what () const throw() {
        return "InvalidInput";
    }
};

Wallet::coins_t Wallet::LeftCoins = 21'000'000 * UNITS;

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
    NewEvent("Created empty Wallet");
}

Wallet::Wallet(std::string str){
    static std::regex pattern(R"(\s*(([1-9][0-9]{0,7})|(0))([.,][0-9]{1,8})?\s*)");

    if (std::regex_match(str, pattern)) {
        try {
            coins = Wallet::StringToCoins(str);
        } catch (NotEnoughCoins &e) {
            throw (NotEnoughCoins());
        }
        NewEvent("Created Wallet from string with value " + std::to_string(coins));
        LeftCoins -= coins;
    } else {
        throw InvalidInput();
    }
}

Wallet::Wallet(coins_t coins){

    try{
        EnoughCoins(coins * UNITS);
    }catch(NotEnoughCoins &e){
        throw(NotEnoughCoins());
    }
    NewEvent("Created Wallet from integer with value " + std::to_string(coins));
    LeftCoins -= coins * UNITS;
}

Wallet::Wallet(Wallet &&w){
    coins = w.coins;
    Operations = w.Operations;
    w.coins = 0;
//    w.Operations.Operations.clear();
    NewEvent("Created Wallet from another Wallet with value " + std::to_string(coins));
}

Wallet::Wallet(Wallet &&w1, Wallet &&w2){
    coins = w1.coins + w2.coins;
    std::merge(w1.Operations.begin(), w1.Operations.end(),
               w2.Operations.begin(), w2.Operations.end(),
               Operations.begin());
    w1.Operations.clear();
    w2.Operations.clear();
    w1.coins = 0;
    w2.coins = 0;
    NewEvent("Created Wallet from other Wallets with value " + std::to_string(coins));
}

Wallet Wallet::fromBinary(std::string str)
{
    std::regex pattern(R"((1[01]{0,24})|(0))");

    if (std::regex_match(str, pattern)) {
        coins_t count = 0;
        try {
            for (unsigned int i = 0; i < str.size(); i++) {
                count = count * 2 + str[i] - '0';
                EnoughCoins(count);
            }
        }
        catch (NotEnoughCoins &e) {
            throw (NotEnoughCoins());
        }
        return Wallet(count);
    } else {
        throw(InvalidInput());
    }
}

Wallet::coins_t Wallet::getUnits() const {
    return Wallet::coins;
}

Wallet::coins_t Wallet::Operation::getUnits() const {
    return coinsAfterOp;
}

unsigned int Wallet::opSize(){
    return Operations.size();
}

Wallet::coins_t Wallet::getCoins()
{
    return coins;
}

const Wallet Empty()
{
    return Wallet();
}



void Wallet::NewEvent(std::string event){
    Operations.emplace_back( Operation(event) );
}

Wallet::Operation::Operation(std::string s){
    this->time = std::chrono::system_clock::now();
    name = s;
}

Wallet& Wallet::operator=(Wallet&& other) // move assignment
{
    //"Jeżeli oba obiekty są tym samym obiektem, to nic nie robi" nie zachodzi?
    if(this != &other)
    {
        this->coins = other.coins;
        this->Operations = other.Operations;
        this->NewEvent("Assigned value from another Wallet " + std::to_string(coins));
    }
    return *this;
}

Wallet operator+(Wallet&& lhs, Wallet &rhs)
{
    Wallet ret;
    ret.coins = lhs.coins + rhs.coins;
    std::merge(lhs.Operations.begin(), lhs.Operations.end(),
               rhs.Operations.begin(), rhs.Operations.end(),
               ret.Operations.begin());
    lhs.coins = 0;
    rhs.coins = 0;
    rhs.NewEvent("Assigned value to another Wallet " + std::to_string(rhs.coins));
    return ret;
}

Wallet operator+(Wallet&& lhs, Wallet &&rhs)
{
    Wallet ret;
    ret.coins = lhs.coins + rhs.coins;
    std::merge(lhs.Operations.begin(), lhs.Operations.end(),
               rhs.Operations.begin(), rhs.Operations.end(),
               ret.Operations.begin());
    lhs.coins = 0;
    rhs.coins = 0;
    rhs.NewEvent("Assigned value to another Wallet " + std::to_string(rhs.coins));
    return ret;
}

Wallet operator-(Wallet&& lhs, Wallet &rhs)
{
    if(lhs.coins < rhs.coins)
        throw(NegativeBankBalance());
    Wallet ret;
    ret.coins = lhs.coins - rhs.coins;
    std::merge(lhs.Operations.begin(), lhs.Operations.end(),
               rhs.Operations.begin(), rhs.Operations.end(),
               ret.Operations.begin());
    lhs.coins -= rhs.coins;
    rhs.coins *= 2;
    rhs.NewEvent("Substracted value from another Wallet " + std::to_string(rhs.coins));
    return ret;
}

Wallet operator-(Wallet&& lhs, Wallet &&rhs)
{
    if(lhs.coins < rhs.coins)
        throw(NegativeBankBalance());
    Wallet ret;
    ret.coins = lhs.coins - rhs.coins;
    std::merge(lhs.Operations.begin(), lhs.Operations.end(),
               rhs.Operations.begin(), rhs.Operations.end(),
               ret.Operations.begin());
    lhs.coins -= rhs.coins;
    rhs.coins *= 2;
    rhs.NewEvent("Substracted value from another Wallet " + std::to_string(rhs.coins));
    return ret;
}

bool operator<(const Wallet::Operation &lhs, const Wallet::Operation &rhs){
    return lhs.time < rhs.time;
}

bool operator>(const Wallet::Operation &lhs, const Wallet::Operation &rhs) {
    return lhs.time > rhs.time;
}

bool operator<=(const Wallet::Operation &lhs, const Wallet::Operation &rhs) {
    return lhs.time <= rhs.time;
}

bool operator>=(const Wallet::Operation &lhs, const Wallet::Operation &rhs) {
    return lhs.time >= rhs.time;
}

bool operator==(const Wallet::Operation &lhs, const Wallet::Operation &rhs) {
    return lhs.time == rhs.time;
}

bool operator!=(const Wallet::Operation &lhs, const Wallet::Operation &rhs) {
    return lhs.time != rhs.time;
}

std::string CoinsToString(Wallet::coins_t coins){
    std::string res;
    res += coins/(Wallet().UNITS) + ".";
    std::string pom = std::to_string(coins%Wallet().UNITS);
    std::reverse(pom.begin(),pom.end());
    while(pom.size() < 8)
        pom += "0";
    std::reverse(pom.begin(),pom.end());
    return res;
}

std::ostream &operator<<(std::ostream &output, const Wallet::Operation &op) {
    time_t tt = std::chrono::system_clock::to_time_t(op.time);
    tm local_tm = *localtime(&tt);
    output<<"Wallet balance is "<<CoinsToString(op.coinsAfterOp)<<" B after operation made at day "<<
                    local_tm.tm_year + 1900<<"-"<<local_tm.tm_mon + 1<<"-"<<local_tm.tm_mday<<"\n";
    return output;
}

Wallet::~Wallet(){
    LeftCoins += coins;
}

Wallet operator*(Wallet::coins_t lhs, Wallet rhs){
    try{
        rhs.EnoughCoins( lhs * Wallet::UNITS );
    }catch(NotEnoughCoins &e){
        throw(NotEnoughCoins());
    }
    Wallet ret(lhs * Wallet::UNITS);
    ret.NewEvent("Created from multiplication coins_t times Wallet.coins " + std::to_string(ret.coins));
    return ret;
}

Wallet operator*(Wallet lhs, Wallet::coins_t rhs){
    try{
        lhs.EnoughCoins( rhs * Wallet::UNITS );
    }catch(NotEnoughCoins &e){
        throw(NotEnoughCoins());
    }
    Wallet ret(rhs * Wallet::UNITS);
    ret.NewEvent("Created from multiplication Wallet coins times coins_t " + std::to_string(ret.coins));
    return ret;
}

bool operator<(const Wallet &lhs, const Wallet &rhs) {
    return lhs.getUnits() < rhs.getUnits();
}

bool operator<=(const Wallet &lhs, const Wallet &rhs) {
    return lhs.getUnits() <= rhs.getUnits();
}

bool operator>(const Wallet &lhs, const Wallet &rhs) {
    return lhs.getUnits() > rhs.getUnits();
}

bool operator>=(const Wallet &lhs, const Wallet &rhs) {
    return lhs.getUnits() >= rhs.getUnits();
}

bool operator==(const Wallet &lhs, const Wallet &rhs) {
    return lhs.getUnits() == rhs.getUnits();
}

bool operator!=(const Wallet &lhs, const Wallet &rhs) {
    return lhs.getUnits() != rhs.getUnits();
}

std::ostream& operator<<(std::ostream &output, const Wallet &w) {
    //TODO
}


