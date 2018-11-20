#include <string>
#include <ctime>
#include <vector>
#include <exception>
#include <chrono>
//ullong czy llong


class Wallet {



  public:
    typedef long long coins_t;
    // czy da sie to dac jako private?
    class Operation {
        private:

            std::chrono::system_clock::time_point time;

            coins_t coinsAfterOp;

            std::string CoinsToString(coins_t coins);

        public:

            Operation(Wallet::coins_t coins);

            Operation() = delete;
//            Operation(const Operation &) = delete;
//            Operation(Operation &&) = delete;

            coins_t getUnits() const;

            friend bool operator<(const Operation &lhs, const Operation &rhs);
            friend bool operator>(const Operation &lhs, const Operation &rhs);
            friend bool operator<=(const Operation &lhs, const Operation &rhs);
            friend bool operator>=(const Operation &lhs, const Operation &rhs);
            friend bool operator==(const Operation &lhs, const Operation &rhs);
            friend bool operator!=(const Operation &lhs, const Operation &rhs);
            friend std::ostream& operator<<(std::ostream &output, const Operation &op);
    };

    std::vector<Operation> Operations;

    void NewEvent(coins_t coins);

    unsigned int OperationsSize();

    const static coins_t UNITS = 1000'000'00;

    static coins_t LeftCoins;

    coins_t coins = 0;

    void EnoughCoins(coins_t coins);

    coins_t StringToCoins(std::string str);


//    public:

    Wallet();

    Wallet(std::string str);

    Wallet(coins_t coins);

    Wallet(const Wallet &) = delete;

    Wallet(Wallet &&w);

    Wallet(Wallet &&w1, Wallet &&w2);

    ~Wallet();

    Wallet fromBinary(std::string str);

    coins_t getUnits() const;

    unsigned int opSize() const;

    coins_t getCoins();

    Wallet &operator=(Wallet &&other);

    Wallet &operator=(const Wallet &other) = delete;

    friend Wallet operator+(Wallet &&lhs, Wallet &rhs);
    friend Wallet operator+(Wallet &&lhs, Wallet &&rhs);
    friend Wallet operator+(Wallet &lhs, Wallet &&rhs) = delete;
    friend Wallet operator+(Wallet &lhs, Wallet &rhs) = delete;

    friend Wallet operator-(Wallet &&lhs, Wallet &rhs);
    friend Wallet operator-(Wallet &&lhs, Wallet &&rhs);
    friend Wallet operator-(Wallet &lhs, Wallet &&rhs) = delete;
    friend Wallet operator-(Wallet &lhs, Wallet &rhs) = delete;

    friend Wallet operator*(coins_t lhs, Wallet rhs);

    friend Wallet operator*(Wallet lhs, coins_t rhs);

    friend bool operator<(const Wallet &lhs, const Wallet &rhs);
    friend bool operator>(const Wallet &lhs, const Wallet &rhs);
    friend bool operator<=(const Wallet &lhs, const Wallet &rhs);
    friend bool operator>=(const Wallet &lhs, const Wallet &rhs);
    friend bool operator==(const Wallet &lhs, const Wallet &rhs);
    friend bool operator!=(const Wallet &lhs, const Wallet &rhs);
    friend std::ostream& operator<<(std::ostream &output, const Wallet &w);


};

const Wallet Empty();
