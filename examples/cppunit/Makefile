CXXFLAGS = -I../../stub
LDFLAGS = -lomniORB4 -lcppunit -ldl

check: test
	./test

test: test.o

test.o: test.cc $(wildcard *.hh)
