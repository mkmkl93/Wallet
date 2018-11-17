#include <string>
//ullong czy llong





class Wallet{

    private:

        typedef long long llong;
        const static llong UNITS = 1000'000'00;
        static llong LeftCoins;
        llong coins = 0;

        bool EnoughCoins(llong coins);

    public:


        Wallet();

        Wallet(std::string str);

        Wallet(llong coins);

        Wallet(Wallet &&w);

        Wallet(Wallet &&w1, Wallet &&w2);

        Wallet(const Wallet&);

        Wallet fromBinary(std::string str);

//        static bool EnoughCoins(llong coins);


        class History{

        };


};

