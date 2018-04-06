all: parsevac clean

parsevac: main.cpp Gumbo.hpp
	g++ main.cpp -pipe -O2 -Wall -m64 -Ofast -flto -march=native -funroll-loops -I /usr/local/include/gq -L /usr/local/lib -std=c++1z -march=native -lboost_system -lgumbo -lcrypto -lssl -pthread -lcpprest -lgq -o parsevac

clean:
	rm -f  *o parsevac