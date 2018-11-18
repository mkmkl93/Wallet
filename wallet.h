#include <string>
#include <ctime>
#include <vector>
#include <exception>
//ullong czy llong

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



class Operation{
    //TODO
    public:
//    private:
        time_t time;
        std::string name;
//    public:
        Operation(time_t t, std::string s);
        friend bool operator<(const Operation &lhs, const Operation &rhs);
};

class History
{
    //TODO
    public:
//    private:
        std::vector<Operation> Operations;

//    public:
        void NewEvent(std::string event);
        History& operator+=(const History &rhs);
        void sort();
};

class Wallet{

    //TODO
    public:
//    private:

        typedef long long coins_t;
        const static coins_t UNITS = 1000'000'00;
        static coins_t LeftCoins;

        History HistoryOfOperations;
        coins_t coins = 0;

        bool EnoughCoins(coins_t coins);
        coins_t StringToCoins(std::string str);


//    public:

        Wallet();

        Wallet(std::string str);

        Wallet(coins_t coins);

        Wallet(const Wallet&) = default;

        Wallet(Wallet &&w);

        Wallet(Wallet &&w1, Wallet &&w2);

//        Wallet(const Wallet&);

        Wallet fromBinary(std::string str);

        coins_t getUnits();

        unsigned int opSize();

        coins_t getCoins();

        Wallet& operator=(Wallet&& other);

        friend Wallet operator+(Wallet&& lhs, Wallet rhs);

};

const Wallet Empty();
