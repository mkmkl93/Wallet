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

            bool operator<(const Operation &rhs) const;
            bool operator>(const Operation &rhs) const;
            bool operator<=(const Operation &rhs) const;
            bool operator>=(const Operation &rhs) const;
            bool operator==(const Operation &rhs) const;
            bool operator!=(const Operation &rhs) const;

            friend std::ostream& operator<<(std::ostream &output, const Operation &op);
    };

    const static coins_t UNITS = 100'000'000;
    std::vector<Operation> Operations;
    static coins_t LeftCoins;
    coins_t coins = 0;

    Wallet();
    Wallet(coins_t coins);
    explicit Wallet(std::string str);
    Wallet(const Wallet &) = delete;
    Wallet(Wallet &&w);
    Wallet(Wallet &&w1, Wallet &&w2);
    ~Wallet();

    void NewEvent(coins_t coins);
    static void EnoughCoins(coins_t coins);
    coins_t StringToCoins(std::string str);
    static Wallet fromBinary(std::string str);

    coins_t getUnits() const;
    unsigned long opSize() const;
    coins_t getCoins();

    Wallet &operator=(Wallet &&other);
    Wallet &operator=(const Wallet &other) = delete;

    friend Wallet&& operator+(Wallet &&lhs, Wallet &rhs);
    friend Wallet&& operator+(Wallet &&lhs, Wallet &&rhs);

    friend Wallet&& operator-(Wallet &&lhs, Wallet &rhs);
    friend Wallet&& operator-(Wallet &&lhs, Wallet &&rhs);

    friend Wallet&& operator*(coins_t lhs, Wallet &rhs);
    friend Wallet&& operator*(coins_t lhs, Wallet &&rhs);

    Wallet& operator*=(coins_t n);
    Wallet&& operator*(coins_t n);

    friend Wallet& operator+=(Wallet &lhs, Wallet &rhs);
    friend Wallet& operator+=(Wallet &lhs, Wallet &&rhs);

    friend Wallet& operator-=(Wallet &lhs, Wallet &rhs);
    friend Wallet& operator-=(Wallet &lhs, Wallet &&rhs);

    bool operator<(const Wallet &rhs) const;
    bool operator>(const Wallet &rhs) const;
    bool operator<=(const Wallet &rhs) const;
    bool operator>=(const Wallet &rhs) const;
    bool operator==(const Wallet &rhs) const;
    bool operator!=(const Wallet &rhs) const;

    friend bool operator==(Wallet &&lhs, Wallet &&rhs);

//    friend bool operator<(const Wallet &lhs, const Wallet &rhs);
//    friend bool operator>(const Wallet &lhs, const Wallet &rhs);
//    friend bool operator<=(const Wallet &lhs, const Wallet &rhs);
//    friend bool operator>=(const Wallet &lhs, const Wallet &rhs);
//    friend bool operator==(const Wallet &lhs, const Wallet &rhs);
//    friend bool operator!=(const Wallet &lhs, const Wallet &rhs);
    const Operation& operator[](unsigned int n) const;

    friend std::ostream& operator<<(std::ostream &output, const Wallet &w);
};

const Wallet& Empty();
