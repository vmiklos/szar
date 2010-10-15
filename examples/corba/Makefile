CC = g++
CXXFLAGS = -g -Wall -Werror -I. -Istub
LDFLAGS = -lomniORB4

all: client server

client: stub/echoSK.o client.o

server: stub/echoSK.o server.o

server.o: server.cc stub/echo.hh

client.o: client.cc stub/echo.hh

stub/echoSK.o: stub/echoSK.cc

stub/echoSK.cc stub/echo.hh: echo.idl
	mkdir -p stub
	omniidl -bcxx -Cstub echo.idl
