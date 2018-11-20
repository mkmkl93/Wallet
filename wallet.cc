#include <algorithm>
#include <regex>
#include <iostream>
#include "wallet.h"

Wallet::coins_t Wallet::LeftCoins = 21'000'000 * UNITS;
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

Wallet::Operation::Operation(Wallet::coins_t coins){
    time = std::chrono::system_clock::now();
    coinsAfterOp = coins;
}

Wallet::coins_t Wallet::Operation::getUnits() const {
    return coinsAfterOp;
}

bool Wallet::Operation::operator<(const Wallet::Operation &rhs) const{
    return this->time < rhs.time;
}

bool Wallet::Operation::operator>(const Wallet::Operation &rhs) const{
    return this->time > rhs.time;
}

bool Wallet::Operation::operator<=(const Wallet::Operation &rhs) const{
    return this->time <= rhs.time;
}

bool Wallet::Operation::operator>=(const Wallet::Operation &rhs) const{
    return this->time >= rhs.time;
}

bool Wallet::Operation::operator==(const Wallet::Operation &rhs) const{
    return this->time == rhs.time;
}

bool Wallet::Operation::operator!=(const Wallet::Operation &rhs) const{
    return this->time != rhs.time;
}

std::string CoinsToString(Wallet::coins_t coins){
    if(coins == 0)
    {
        return "0";
    }
    std::stringstream resSS;

    long long satoshi = coins % Wallet::UNITS;
    if (satoshi) {
        std::string aux = std::to_string(satoshi);
        std::reverse(aux.begin(), aux.end());
        while(aux.size() < 8)
            aux += "0";
        std::reverse(aux.begin(), aux.end());

        std::size_t idx = 7;
        char digit = aux[idx];

        while (digit == '0') {
            digit = aux[idx-1];
            aux.erase(idx--);
        }

        resSS << coins / (Wallet::UNITS) << "," << aux;
    } else {
        resSS << coins / (Wallet::UNITS);
    }

    return resSS.str();
}

std::ostream &operator<<(std::ostream &output, const Wallet::Operation &op) {
    time_t tt = std::chrono::system_clock::to_time_t(op.time);
    tm local_tm = *localtime(&tt);
    output<<"Wallet balance is "<<CoinsToString(op.coinsAfterOp)<<" B after operation made at day "<<
                    local_tm.tm_year + 1900<<"-"<<local_tm.tm_mon + 1<<"-"<<local_tm.tm_mday<<"\n";
    return output;
}

Wallet::Wallet(){
    NewEvent(0);
}

Wallet::Wallet(coins_t coins){
    try{
        EnoughCoins(coins * UNITS);
    }catch(NotEnoughCoins &e){
        throw(NotEnoughCoins());
    }
    this->coins = coins * UNITS;
    NewEvent(this->coins);
    LeftCoins -= coins * UNITS;
}

Wallet::Wallet(int coins) : Wallet::Wallet(static_cast<coins_t>(coins)) {}

Wallet::Wallet(short coins) : Wallet::Wallet(static_cast<coins_t>(coins)) {}

Wallet::Wallet(std::string str){
    std::regex pattern(R"(\s*(([1-9][0-9]{0,7})|(0))([.,][0-9]{1,8})?\s*)");

    if (std::regex_match(str, pattern)) {
        try {
            coins = Wallet::StringToCoins(str);
        } catch (NotEnoughCoins &e) {
            throw (NotEnoughCoins());
        }
        NewEvent(coins);
        LeftCoins -= coins;
    } else {
        throw InvalidInput();
    }
}

Wallet::Wallet(const char* str) : Wallet::Wallet(std::string(str)) {}

Wallet::Wallet(Wallet &&w){
    coins = w.coins;
    Operations = w.Operations;
    w.coins = 0;
//    w.Operations.Operations.clear();
    NewEvent(coins);
}

Wallet::Wallet(Wallet &&w1, Wallet &&w2){
    coins = w1.coins + w2.coins;
    Operations.reserve(w1.opSize()+w2.opSize());
    std::merge(w1.Operations.begin(), w1.Operations.end(),
               w2.Operations.begin(), w2.Operations.end(),
               std::back_inserter(Operations));
    w1.Operations.clear();
    w2.Operations.clear();
    w1.coins = 0;
    w2.coins = 0;
    NewEvent(coins);
}

Wallet::~Wallet(){
    LeftCoins += coins;
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
    return coins;
}

unsigned long Wallet::opSize() const{
    return Operations.size();
}

Wallet& Wallet::operator=(Wallet&& other) // move assignment
{
    //"Jeżeli oba obiekty są tym samym obiektem, to nic nie robi" nie zachodzi?
    if(this != &other)
    {
        this->coins = other.coins;
        this->Operations = other.Operations;
        this->NewEvent(coins);
    }
    return *this;
}

Wallet&& operator+(Wallet&& lhs, Wallet &rhs)
{
    lhs += rhs;
    return std::move(lhs);
}

Wallet&& operator+(Wallet&& lhs, Wallet &&rhs)
{
    lhs += rhs;
    return std::move(lhs);
}

Wallet&& operator-(Wallet&& lhs, Wallet &rhs)
{
    try{
        lhs -= rhs;
    }
    catch(NegativeBankBalance &e){
        throw NegativeBankBalance();
    }
    return std::move(lhs);
}

Wallet&& operator-(Wallet&& lhs, Wallet &&rhs)
{
    try{
        lhs -= rhs;
    }
    catch(NegativeBankBalance &e){
        throw NegativeBankBalance();
    }
    return std::move(lhs);
}

Wallet&& operator*(Wallet::coins_t lhs, Wallet &rhs) {
    rhs *= lhs;
    return std::move(rhs);
}

Wallet&& operator*(Wallet::coins_t lhs, Wallet &&rhs) {
    rhs *= lhs;
    return std::move(rhs);
}

Wallet& Wallet::operator*=(Wallet::coins_t n) {
    if (n == 0) {
        coins = 0;
        NewEvent(coins);
    } else {
        Wallet::EnoughCoins((n - 1) * coins);

        coins *= n;
        LeftCoins -= (n - 1) * coins;
        NewEvent(coins);
    }

    return *this;
}

Wallet&& Wallet::operator*(Wallet::coins_t n) {
    *this *= n;

    return std::move(*this);
}

Wallet& operator+=(Wallet &lhs, Wallet &rhs) {
    lhs.coins += rhs.coins;
    rhs.coins = 0;
    lhs.NewEvent(lhs.coins);
    rhs.NewEvent(rhs.coins);

    return lhs;
}

Wallet& operator+=(Wallet &lhs, Wallet &&rhs) {
    lhs.coins += rhs.coins;
    rhs.coins = 0;
    lhs.NewEvent(lhs.coins);
    rhs.NewEvent(rhs.coins);

    return lhs;
}

Wallet& operator-=(Wallet &lhs, Wallet &rhs) {
    if(lhs.getUnits() < rhs.getUnits())
        throw NegativeBankBalance();
    lhs.coins -= rhs.coins;
    rhs.coins *= 2;
    lhs.NewEvent(lhs.coins);
    rhs.NewEvent(rhs.coins);

    return lhs;
}

Wallet& operator-=(Wallet &lhs, Wallet &&rhs) {
    if(lhs.getUnits() < rhs.getUnits())
        throw NegativeBankBalance();
    lhs.coins -= rhs.coins;
    rhs.coins *= 2;
    lhs.NewEvent(lhs.coins);
    rhs.NewEvent(rhs.coins);

    return lhs;
}

//bool operator==(Wallet &&lhs, Wallet &&rhs) {
//    return lhs.getUnits() == rhs.getUnits();
//}

bool operator<(const Wallet &lhs, const Wallet &rhs){
    return lhs.getUnits() < rhs.getUnits();
}

bool operator>(const Wallet &lhs, const Wallet &rhs){
    return lhs.getUnits() > rhs.getUnits();
}

bool operator<=(const Wallet &lhs, const Wallet &rhs){
    return lhs.getUnits() <= rhs.getUnits();
}

bool operator>=(const Wallet &lhs, const Wallet &rhs){
    return lhs.getUnits() >= rhs.getUnits();
}

bool operator==(const Wallet &lhs, const Wallet &rhs){
    return lhs.getUnits() == rhs.getUnits();
}

bool operator!=(const Wallet &lhs, const Wallet &rhs){
    return lhs.getUnits() != rhs.getUnits();
}

const Wallet::Operation& Wallet::operator[](unsigned int n) const {
    if (n >= Wallet::Operations.size()) {
        throw(InvalidInput());
    } else {
        return Wallet::Operations[n];
    }
}

std::ostream& operator<<(std::ostream &output, const Wallet &w) {
    output << "Wallet[" << CoinsToString(w.getUnits()) << " B]";

    return output;
}

const Wallet& Empty()
{
    const static Wallet empty = Wallet();

    return empty;
}

void Wallet::NewEvent(Wallet::coins_t coins){
    Operations.emplace_back( Operation(coins) );
}

void Wallet::EnoughCoins(Wallet::coins_t coins){
    if(coins > Wallet::LeftCoins)
        throw(NotEnoughCoins());
}

Wallet::coins_t Wallet::StringToCoins(std::string str){
    unsigned int DotPos = str.find(".",0);
    unsigned int ComaPos = str.find(",",0);
    unsigned int delimiter = std::min(DotPos, ComaPos);

    if(DotPos != str.size() || ComaPos != str.size()) {
        for (unsigned int i = delimiter + 1; i < delimiter + 9; i++) {
            if (i >= str.size()) {
                str += '0';
            } else if (str[i] < '0' || str[i] > '9') {
                str[i] = '0';
            }
        }
    }
    coins_t count = 0;
    try{
        for(unsigned int i = 0; i < str.size(); i++)
        {
            if(str[i] < '0' || str[i] > '9')
                continue;
            count = count * 10 + str[i] - '0';
            EnoughCoins(count);
        }
    }catch(NotEnoughCoins &e){
        throw(NotEnoughCoins());
    }
    return count;
}

