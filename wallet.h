#include <string>
#include <ctime>
#include <vector>
#include <exception>
#include <chrono>
//ullong czy llong


class Wallet{
    public:
        // czy da sie to dac jako private?
        class History
        {
            public:
                class Operation{
                    private:

                        std::chrono::milliseconds ms;

                        std::string name;

                        friend bool operator<(const Operation &lhs, const Operation &rhs);

                    public:

                        Operation(std::chrono::milliseconds ms, std::string s);
                };
            private:

                std::vector<Operation> Operations;

            public:
                void NewEvent(std::string event);

                History& operator+=(const History &rhs);

                unsigned int OperationsSize();

                void clear();

                void sort();
        };
    //TODO
//    private:

        typedef long long coins_t;
        const static coins_t UNITS = 1000'000'00;
        static std::chrono::high_resolution_clock::time_point TimeStart;

        static coins_t LeftCoins;

        History HistoryOfOperations;
        coins_t coins = 0;

        void EnoughCoins(coins_t coins);
        coins_t StringToCoins(std::string str);


//    public:

        Wallet();

        Wallet(std::string str);

        Wallet(coins_t coins);

        Wallet(const Wallet&) = default;

        Wallet(Wallet &&w);

        Wallet(Wallet &&w1, Wallet &&w2);

        ~Wallet();

        Wallet fromBinary(std::string str);

        coins_t getUnits();

        unsigned int opSize();

        coins_t getCoins();

        Wallet& operator=(Wallet&& other);

        friend Wallet operator+(Wallet&& lhs, Wallet rhs);

        friend Wallet operator-(Wallet&& lhs, Wallet rhs);

        friend Wallet operator*(coins_t lhs, Wallet rhs);

        friend Wallet operator*(Wallet lhs, int rhs);

};

const Wallet Empty();
