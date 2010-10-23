#!/bin/sh
./client \
	-ORBInitRef AuthService=corbaloc:iiop:localhost:31337/AuthService \
	-ORBnativeCharCodeSet UTF-8
