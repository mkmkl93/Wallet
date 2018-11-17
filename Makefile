go:
	g++ -Wall -Wextra -O2 -std=c++17 -c wallet.cc -o wallet.o
	g++ -Wall -Wextra -O2 -std=c++17 -c wallet_example.cc -o wallet_example.o
	g++ -Wall -Wextra -O2 -std=c++17 wallet.o wallet_example.o -o wallet