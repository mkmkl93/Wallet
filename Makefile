go:
	g++ -Wall -g -Wextra -O2 -std=c++17 -c wallet.cc -o wallet.o
	g++ -Wall -g -Wextra -O2 -std=c++17 -c wallet_example.cc -o wallet_example.o
	g++ -Wall -g -Wextra -O2 -std=c++17 wallet.o wallet_example.o -o wallet
go2:
	g++ -Wall -g -Wextra -O2 -std=c++17 -c wallet.cc -o wallet.o
	g++ -Wall -g -Wextra -O2 -std=c++17 -c wallet_example2.cc -o wallet_example2.o
	g++ -Wall -g -Wextra -O2 -std=c++17 wallet.o wallet_example2.o -o wallet2