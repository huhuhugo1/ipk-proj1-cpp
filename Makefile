CC=g++

all: ftrest fdtrest

ftrest: ftrest.cpp
	$(CC) -std=c++11 -o ftrest ftrest.cpp

fdtrest: fdtrest.cpp
	$(CC) -std=c++11 -o fdtrest fdtrest.cpp
