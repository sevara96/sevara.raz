search: search.cpp webpage.h setutility.h
	g++ -g -Wall -std=c++11 search.cpp webpage.cpp -o search

$(BIN_DIR)/webpage.o:  webpage.h webpage.cpp
	g++ -g -Wall -std=c++11 -c webpage.cpp -o webpage.o
