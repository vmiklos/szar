#!/bin/sh
./server "$@" \
	-ORBendPoint giop:tcp::31337 \
	-ORBnativeCharCodeSet UTF-8
