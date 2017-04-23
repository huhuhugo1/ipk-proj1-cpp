CC=g++

all: ftrest ftrestd

ftrest: ftrest.cpp common/box.cpp common/commands.cpp common/message.cpp common/regex.cpp common/responses.cpp common/socket.cpp
	$(CC) -std=c++11 -o ftrest ftrest.cpp

ftrestd: ftrestd.cpp common/box.cpp common/commands.cpp common/message.cpp common/regex.cpp common/responses.cpp common/socket.cpp
	$(CC) -std=c++11 -o ftrestd ftrestd.cpp
